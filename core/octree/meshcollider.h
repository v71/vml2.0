#pragma once

//////////////////////////////////////////////////////////////
//	octree

namespace vml
{
	namespace octree
	{

			class ObjectMTV : public vml::geo3d::collisions::MTV
			{
				
				public:
					
					vml::objects::Object3d_2* Object;
					vml::objects::Model3d_2*  Model;
					
					ObjectMTV& operator=(const vml::geo3d::collisions::MTV& mtv)
					{
						Normal        = mtv.Normal;
						P0            = mtv.P0;
						P1            = mtv.P1;
						P2            = mtv.P2;
						Distance      = mtv.Distance;
						IsValid       = mtv.IsValid;
						ContactPoint  = mtv.ContactPoint;
						ContactNormal = mtv.ContactNormal;
						return *this;
					}

					// ---------------------------------------------------
					// ctor / dtor

					ObjectMTV() 
					{
						Object=nullptr;
						Model=nullptr;
					}

					~ObjectMTV()
					{}
			};

			////////////////////////////////////////////////////////////////////////////
			//

			struct OctTreeSurface
			{
				glm::vec3 P0, P1, P2;
				glm::vec3 Bmin, Bmax;
				glm::vec3 Normal;
				bool Visited;

				OctTreeSurface(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
				{
					P0 = p0;
					P1 = p1;
					P2 = p2;

					Normal = glm::normalize(glm::cross(P1 - P0, P2 - P0));

					Visited = false;

					Bmin = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
					Bmax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					if (P0.x < Bmin.x) Bmin.x = P0.x;
					if (P1.x < Bmin.x) Bmin.x = P1.x;
					if (P2.x < Bmin.x) Bmin.x = P2.x;

					if (P0.y < Bmin.y) Bmin.y = P0.y;
					if (P1.y < Bmin.y) Bmin.y = P1.y;
					if (P2.y < Bmin.y) Bmin.y = P2.y;

					if (P0.z < Bmin.z) Bmin.z = P0.z;
					if (P1.z < Bmin.z) Bmin.z = P1.z;
					if (P2.z < Bmin.z) Bmin.z = P2.z;

					if (P0.x > Bmax.x) Bmax.x = P0.x;
					if (P1.x > Bmax.x) Bmax.x = P1.x;
					if (P2.x > Bmax.x) Bmax.x = P2.x;

					if (P0.y > Bmax.y) Bmax.y = P0.y;
					if (P1.y > Bmax.y) Bmax.y = P1.y;
					if (P2.y > Bmax.y) Bmax.y = P2.y;

					if (P0.z > Bmax.z) Bmax.z = P0.z;
					if (P1.z > Bmax.z) Bmax.z = P1.z;
					if (P2.z > Bmax.z) Bmax.z = P2.z;


				}
			};

			////////////////////////////////////////////////////////////////////////////
			//

			class MeshColliderNode
			{

				private:

				public:
				
					// -------------------------------------------------------------

					vml::geo3d::AABBox			 BoundingBox;					// bounding box
					std::vector<OctTreeSurface*> CollisionSurfaces;

					// -------------------------------------------------------------

					const glm::vec3& GetBoundingBoxMin()         const { return BoundingBox.GetMin(); }
					const glm::vec3& GetBoundingBoxMax()         const { return BoundingBox.GetMax(); }
					const glm::vec3& GetBoundingBoxHalfExtents() const { return BoundingBox.GetHalfExtents(); }
					const glm::vec3& GetBoundingBoxCenter()      const { return BoundingBox.GetCenter(); }

					// -------------------------------------------------------------
					// draw axis oriented node bounding box

					void DrawAABBox(vml::views::View* view, const glm::vec4& col, bool drawpoints = true) const
					{
						vml::DebugRender->DrawAABBox(view, BoundingBox.GetMin(), BoundingBox.GetMax(), col, drawpoints);
					}

					void Draw(vml::views::View* view, const glm::vec4& col, bool drawpoints = true) const
					{
						vml::DebugRender->DrawAABBox(view, BoundingBox.GetMin(), BoundingBox.GetMax(), col, drawpoints);
						for (size_t i = 0; i < CollisionSurfaces.size(); ++i)
							vml::DebugRender->DrawTriangle(view, CollisionSurfaces[i]->P0, CollisionSurfaces[i]->P1, CollisionSurfaces[i]->P2, vml::colors::Green,true);
					}

					void DrawTriangle(vml::views::View* view, int i, const glm::vec4& col)
					{
						vml::DebugRender->DrawTriangle(view, CollisionSurfaces[i]->P0, CollisionSurfaces[i]->P1, CollisionSurfaces[i]->P2, col, true);
					}

					// ----------------------------------------------------------------
					// ctor / dtor

					MeshColliderNode()
					{
						BoundingBox = vml::geo3d::NullAABBox;
					}

					~MeshColliderNode()
					{
					}
				
			};

		////////////////////////////////////////////////////////////////////////////
		//
		
		class MeshCollider
		{

			private:

				static const unsigned int MAX_MTVS = 128;

				// ----------------------------------------------------------------
				// private data

				std::string					 ResourceFileName;
				vml::geo3d::AABBox			 BoundingBox;			// bounding box
				glm::vec3					 GridDelta;
				int							 GridSize;
				int							 GridSizeX2;
				int							 CellsCount;
				MeshColliderNode			*Cells;
				std::vector<OctTreeSurface>	 Surfaces;
				ObjectMTV					 Mtvs[MAX_MTVS];
				unsigned int				 MtvsCount;

				// ----------------------------------------------------------------
				// converts data from seprate vertex, normal and uv array to
				//  we are not really interested into pre vertex normals and uv coords

				void ConvertData(const std::vector<float>* sourcevertexarray, 
					             const std::vector<unsigned int>* sourcesurfaceindices)
				{
					
					for (int i = 0; i < sourcesurfaceindices->size() / 3; ++i)
					{
						// indices have a 3 elements stride

						int idx = i * 3;

						int i0 = sourcesurfaceindices->at((size_t)idx    ) * 4;
						int i1 = sourcesurfaceindices->at((size_t)idx + 1) * 4;
						int i2 = sourcesurfaceindices->at((size_t)idx + 2) * 4;

						glm::vec4 p0 = glm::vec4(sourcevertexarray->at(i0), sourcevertexarray->at(i0 + 1), sourcevertexarray->at(i0 + 2),1);
						glm::vec4 p1 = glm::vec4(sourcevertexarray->at(i1), sourcevertexarray->at(i1 + 1), sourcevertexarray->at(i1 + 2),1);
						glm::vec4 p2 = glm::vec4(sourcevertexarray->at(i2), sourcevertexarray->at(i2 + 1), sourcevertexarray->at(i2 + 2),1);

						Surfaces.emplace_back(OctTreeSurface(p0, p1, p2));
					}
							
				}
				
				// ----------------------------------------------------------------
				//

				void ReleaseAll()
				{
					// delete cells array

					vml::SafeDeleteArray(Cells);
					vml::SafeDeleteArray(QuerySurfaces);

					// clear surfaces array

					Surfaces.clear();
			
					// reset data

					BoundingBox = vml::geo3d::NullAABBox;
					GridDelta   = glm::vec3(0, 0, 0);
					GridSize    = 0;
					GridSizeX2  = 0;
					CellsCount  = 0;
					QuerySurfacesCount = 0;

				}
					
			public:

				OctTreeSurface** QuerySurfaces;
				int				 QuerySurfacesCount;

				// ----------------------------------
				//

				const glm::vec3&  GetBoundingBoxMin() const { return BoundingBox.GetMin(); }
				const glm::vec3&  GetBoundingBoxMax() const { return BoundingBox.GetMax(); }
				const glm::vec3&  GetGridDelta()      const { return GridDelta; }
				int				  GetGridSize()       const { return GridSize; }
				int				  GetCellsCount()     const { return CellsCount; }
				MeshColliderNode* GetCell(int i)      const { return &Cells[i]; }
				
				// ----------------------------------
				//

				int GetCellAddress(int i, int j, int k) const
				{
					return i + j * GridSize + k * GridSize * GridSize;
				}

				// ----------------------------------
				//

				const glm::vec3 MapPointToCell(const glm::vec3& p) const
				{
					return (p - BoundingBox.GetMin()) / GridDelta;
				}
				
				// ----------------------------------------------------------------
				//

				void Go(const vml::meshes::Mesh3d *mesh,int gridsize)
				{
					
					if (mesh)
					{
						if (!mesh->IsValid())
							vml::os::Message::Error("MeshCollider : ","Mesh is not valid");
						
						ResourceFileName = mesh->GetResourceFileName();

						std::cout << " creating collison grid for mesh : " << ResourceFileName << std::endl;
						
						// free octree data

						ReleaseAll();

						// converts mesh data to internal format 

						ConvertData(&mesh->GetVertexArray(), &mesh->GetSurfaceIndices());
						
						QuerySurfaces = new OctTreeSurface *[mesh->GetSurfaceCount()];
						QuerySurfacesCount = 0;

						// get bounding box

						BoundingBox = mesh->GetBoundingBox();

						// compute grid metrics

						GridSize = gridsize;
						GridSizeX2 = GridSize* GridSize;
						CellsCount = GridSize * GridSize * GridSize;
						GridDelta = (BoundingBox.GetMax() - BoundingBox.GetMin()) / (float)GridSize;
						
						// alloctae cells

						Cells = new MeshColliderNode[CellsCount];
						
						// create grid of cells

						glm::vec3 p0 = BoundingBox.GetMin();

						size_t cellcount = 0;

						float eps = 0.001f;

						glm::vec3 thickness = glm::vec3(eps, eps, eps);

						// if we do not check for Cells validity, the compiler complaints

						for (int k = 0; k < GridSize; ++k)
						{
							for (int j = 0; j < GridSize; ++j)
							{
								for (int i = 0; i < GridSize; ++i)
								{
									
									MeshColliderNode* cell = &Cells[cellcount];

									glm::vec3 bmin(p0.x + i * GridDelta.x				, p0.y + j * GridDelta.y, p0.z + k * GridDelta.z);
									glm::vec3 bmax(p0.x + i * GridDelta.x + GridDelta.x , p0.y + j * GridDelta.y + GridDelta.y , p0.z + k * GridDelta.z + GridDelta.z);

									cell->BoundingBox=vml::geo3d::AABBox(bmin-thickness, bmax+thickness);

									cellcount++;
									
								}
							}
						}
			
						// store collision surfaces for each grid cell

						for (int k = 0; k < GridSize; ++k)
						{
							for (int j = 0; j < GridSize; ++j)
							{
								for (int i = 0; i < GridSize; ++i)
								{
										
									// get cell address

									int address= i + j * GridSize + k * GridSize * GridSize;

									MeshColliderNode* cell = &Cells[address];

									glm::vec3 bmin   = cell->BoundingBox.GetMin();
									glm::vec3 bmax	 = cell->BoundingBox.GetMax();
									glm::vec3 center = cell->BoundingBox.GetCenter();
									
									// enlarging node bounding box by an epsilon to prevent
									// faces being considered out when they are lying on 
									// the bounding box planes
										
									glm::vec3 u = bmin - center;
									glm::vec3 v = bmax - center;

									float thickness = 0.1f;

									u = glm::normalize(u);
									v = glm::normalize(v);

									bmin += u * thickness;
									bmax += v * thickness;

									// check if triangles inside boundng box
									// and store a pointer if a cell intersect a triangle aabbox

									for (int s = 0; s < Surfaces.size(); ++s)
									{
										if ( vml::geo3d::intersections::AABBoxVsAABBox(Surfaces[s].Bmin, Surfaces[s].Bmax, bmin, bmax )!= vml::geo3d::intersections::AABBOX_DISJOINT)
										{
											cell->CollisionSurfaces.emplace_back(&Surfaces[s]);
										}
									}
								}
							}
						}
						
					}
					else
					{
						vml::os::Message::Error("Mesh Collider : ","Mesh is null");
					}
				}

				// ----------------------------------------------------------------
				//

				bool FindGridCellsInterceptedByObjectAABBOX(vml::views::View* view,const vml::geo3d::AABBox &box)
				{

					QuerySurfacesCount = 0;

					for (int k = 0; k < GridSize; ++k)
					{
						for (int j = 0; j < GridSize; ++j)
						{
							for (int i = 0; i < GridSize; ++i)
							{
								// get cell address

								int address = i + j * GridSize + k * GridSizeX2;

								MeshColliderNode* cell = &Cells[address];

								if ( vml::geo3d::intersections::AABBoxVsAABBox(box.GetMin(),box.GetMax(), 
																			   cell->BoundingBox.GetMin(), cell->BoundingBox.GetMax())!=vml::geo3d::intersections::AABBOX_DISJOINT)
								{

								//	vml::DebugRender->DrawAABBox(view, cell->BoundingBox.GetMin(), cell->BoundingBox.GetMax(), vml::colors::White);
								
									//	DrawGridCell(view, i, j, k);

									for (int s = 0; s < cell->CollisionSurfaces.size(); ++s)
									{
										OctTreeSurface* surface = cell->CollisionSurfaces[s];

									//	cell->DrawTriangle(view, s, vml::colors::Green);
										
										// if surface has not been visited , set flag to true
										// to prevent multiple checks

										if (!surface->Visited)
										{
											QuerySurfaces[QuerySurfacesCount++] = surface;
											surface->Visited = true;
										}
									}
								}
							}
						}
					}

					// clear visited flag

					for (size_t i = 0; i < QuerySurfacesCount; ++i)
					{
						QuerySurfaces[i]->Visited = false;
					}

					return false;
				}

				// ----------------------------------------------------------------
				//

				bool CollisionResoluzion(vml::views::View* view, vml::objects::Object3d_2* object, ObjectMTV &objmtv)
				{
					// find grid cells intercepted by object bounding box

					vml::geo3d::AABBox exbox = object->GetExpandedBoundingBox();

					const glm::vec3& exboxBmin = exbox.GetMin();
					const glm::vec3& exboxBmax = exbox.GetMax();

					FindGridCellsInterceptedByObjectAABBOX(view, exbox);

					// compute collision 

					MtvsCount = 0;

				//	vml::DebugRender->DrawAABBox(view, exboxBmin, exboxBmax, vml::colors::Yellow);

					for (size_t k = 0; k < object->GetModelsCount(); ++k)
					{
						vml::objects::Model3d_2* model = object->GetModelAt(k);

						vml::geo3d::collisions::EllipsoidCollider* ec = model->GetEllipsoidCollider();

						if (ec)
						{
						
						//		vml::DebugRender->DrawEllipsoid(view, model, vml::colors::Red);
						
							for (size_t i = 0; i < QuerySurfacesCount; ++i)
							{

								OctTreeSurface* surf = QuerySurfaces[i];
							
								// check if surface triangle bounding box is inersected with ellipsoid bouding box

								if (exboxBmax.x > surf->Bmin.x &&
									exboxBmin.x < surf->Bmax.x &&
									exboxBmax.y > surf->Bmin.y &&
									exboxBmin.y < surf->Bmax.y &&
									exboxBmax.z > surf->Bmin.z &&
									exboxBmin.z < surf->Bmax.z)
								{

								//	vml::DebugRender->DrawTriangle(view, surf->P0, surf->P1, surf->P2, vml::colors::Green, true);

									vml::geo3d::collisions::MTV mtv;

									if (vml::geo3d::collisions::TriangleEllipsoidCollision2(ec->GetPosition(),
																							ec->GetRightVector(),
																							ec->GetUpVector(),
																							ec->GetForwardVector(),
																							ec->GetRadii(),
																							surf->P0,
																							surf->P1,
																							surf->P2,
																							surf->Normal,
																							mtv) == 1)
									{

										// add the mts to the mtv list

										if (MtvsCount < MAX_MTVS)
										{

										//	glm::vec3 p0 = surf->P0;
										//	glm::vec3 p1 = surf->P1;
										//	glm::vec3 p2 = surf->P2;

										//	vml::DebugRender->DrawTriangle(view, surf->P0, surf->P1, surf->P2, vml::colors::Yellow, false);
											Mtvs[MtvsCount] = mtv;
											Mtvs[MtvsCount].Object = object;
											Mtvs[MtvsCount].Model = model;
											MtvsCount++;
										}
									}
								}
							}
						}
					}

					// if no collisons have been found, then exit

					if (MtvsCount == 0)
						return false;

					int maxidx = -1;
					float maxdist = -FLT_MAX;

					for (size_t i = 0; i < MtvsCount; ++i)
					{
						//	std::cout << i << " " << Mtvs[i].Distance << std::endl;

						if (Mtvs[i].Distance > maxdist)
						{
							maxdist = Mtvs[i].Distance;
							maxidx = (int)i;
						}
					}

					//	std::cout << std::endl;
					//	std::cout << "max : " << maxidx << " : " << maxdist << std::endl;
					//	std::cout << std::endl;

					// if we get here mexids will never be -1
					// since there will be at least one mtv

					if (maxidx != -1)
					{
						objmtv = Mtvs[maxidx];
					}
				
					return true;
				}

				// -------------------------------------

				void DrawGridCell(vml::views::View* view,int i, int j, int k)
				{
					Cells[i + j * GridSize + k * GridSize * GridSize].Draw(view,vml::colors::Gray);
				}
			
				void DrawAABBox(vml::views::View* view,const glm::vec4 &col)
				{
					vml::DebugRender->DrawAABBox(view, BoundingBox.GetMin(), BoundingBox.GetMax(), col, true);
				}

				void Draw(vml::views::View* view, const glm::vec4& col)
				{
					for (size_t i = 0; i < CellsCount; ++i)
						Cells[i].DrawAABBox(view, col, true);
				}
				
				void DrawTriangle(vml::views::View* view, int i,const glm::vec4& col)
				{
					vml::DebugRender->DrawTriangle(view, Surfaces[i].P0, Surfaces[i].P1, Surfaces[i].P2, col);
				}

				void DrawTriangles(vml::views::View* view,const glm::vec4& col)
				{
					for ( size_t i=0; i<Surfaces.size(); ++i)
						vml::DebugRender->DrawTriangle(view, Surfaces[i].P0, Surfaces[i].P1, Surfaces[i].P2, col,true);
				}

				//-----------------------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				MeshCollider(MeshCollider& meshcollider) = delete;

				//-----------------------------------------------------------------------------------
				// overload operator is private,
				// no copies allowed since classes
				// are referenced

				void operator=(const MeshCollider& meshcollider) = delete;
						
				// ----------------------------------------------------------------
				// ctor / dtor

				MeshCollider()
				{
					BoundingBox        = vml::geo3d::NullAABBox;
					GridDelta          = glm::vec3(0, 0, 0);
					GridSize           = 0;
					GridSizeX2         = 0;
					CellsCount         = 0;
					Cells              = nullptr;
					QuerySurfaces      = nullptr;
					QuerySurfacesCount = 0;
					MtvsCount		   = 0;
				}

				~MeshCollider()
				{
					ReleaseAll();
				}

		};
			
	}
}
