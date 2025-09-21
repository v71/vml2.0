#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

/////////////////////////////////////////////////////////////////////////////////////////////

#include <vml2.0/math/3d/points.h>
#include <vml2.0/math/3d/triangles.h>
#include <vml2.0/math/3d/aabbox.h>
#include <vml2.0/core/mesh/procedural.h>
#include <vml2.0/core/mesh/removeduplicates.h>
#include <format>
#include <map>


/////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace meshes
	{
		typedef std::vector<vml::geo3d::Vertex>			 VertexArray;			// array of vertices
		typedef std::vector<vml::geo3d::IndexedTriangle> IndexedTriangleArray;	// array of indexed triangles
	}
}

//////////////////////////////////////////////////////////////////////////////
// TO DO :

namespace vml
{
	namespace meshes
	{

		/////////////////////////////////////////////////////////////////
		// mesh class
			
		class MeshBuilder 
		{
			
			private:
	
				// ---------------------------------------------------------------
				
				unsigned int					  InternalFlags;		// operation on mesh data flag
				unsigned int					  PreferencesFlags;		// preferences flag
				float							  Radius;				// Mesh radius
				vml::geo3d::AABBox				  BoundingBox;			// Mesh Bounding Box
				vml::meshes::VertexArray		  VertexArray;			// array of vertices
				vml::meshes::IndexedTriangleArray SurfaceArray;			// array of surfaces
				std::vector<int>				  SurfaceIndices;		// vbo surface indices 	
				unsigned int					  VertexBufferObject;	// vertex buffer object
				unsigned int					  IndexBufferObject;	// surface index buffer object
				std::string						  ResourceFileName;		// resource file name

				// ---------------------------------------------------------------
				// Memory clearing :
				// Resets all data to initial values (0) and 
				// clears vertex, surface and surface indices arrays
				// releases also vao(s).

				void ReleaseAll()
				{
					// sets data members to initial values

					InternalFlags	    = 0;
					ResourceFileName	= "";

					Radius			    = 0;
					BoundingBox			= vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

					// cancel main data buffers

					VertexArray.clear();
					SurfaceArray.clear();
					SurfaceIndices.clear();
					
					// set data as not initted

					vml::bits32::SetToFalse(InternalFlags, INITTED);
				}
			
				// ---------------------------------------------------------------
				// tranform object
				// this function transforms directly the mesh definition
				// often is usefull to modify the mesh' s geometry 
				// for example after importing , this is not
				// a rednering pipeline tranformation, this is a 
				// direct modification of mesh's geometry

				void Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot transform, mesh is not initted");

					float sx = sin(rot.x * vml::math::DEGTORAD);
					float sy = sin(rot.y * vml::math::DEGTORAD);
					float sz = sin(rot.z * vml::math::DEGTORAD);
					float cx = cos(rot.x * vml::math::DEGTORAD);
					float cy = cos(rot.y * vml::math::DEGTORAD);
					float cz = cos(rot.z * vml::math::DEGTORAD);

					// get parent matrix pointer

					float M[16] = { 0 };

					// compute model matrix

					M[ 0] = (cy * cz) * scale.x;
					M[ 1] = (cy * sz) * scale.x;
					M[ 2] = (-sy) * scale.x;
					M[ 3] = 0;
					M[ 4] = (sx * sy * cz - cx * sz) * scale.y;
					M[ 5] = (sx * sy * sz + cx * cz) * scale.y;
					M[ 6] = (sx * cy) * scale.y;
					M[ 7] = 0;
					M[ 8] = (cx * sy * cz + sx * sz) * scale.z;
					M[ 9] = (cx * sy * sz - sx * cz) * scale.z;
					M[10] = (cx * cy) * scale.z;
					M[11] = 0;
					M[12] = pos.x;
					M[13] = pos.y;
					M[14] = pos.z;
					M[15] = 1;

					std::vector<vml::geo3d::Vertex> p = VertexArray;

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{
						p[i].Pos.x = VertexArray[i].Pos.x * M[0] + VertexArray[i].Pos.y * M[4] + VertexArray[i].Pos.z * M[ 8] + M[12];
						p[i].Pos.y = VertexArray[i].Pos.x * M[1] + VertexArray[i].Pos.y * M[5] + VertexArray[i].Pos.z * M[ 9] + M[13];
						p[i].Pos.z = VertexArray[i].Pos.x * M[2] + VertexArray[i].Pos.y * M[6] + VertexArray[i].Pos.z * M[10] + M[14];
				
						p[i].Normal.x = VertexArray[i].Normal.x * M[0] + VertexArray[i].Normal.y * M[4] + VertexArray[i].Normal.z * M[ 8] + M[12];
						p[i].Normal.y = VertexArray[i].Normal.x * M[1] + VertexArray[i].Normal.y * M[5] + VertexArray[i].Normal.z * M[ 9] + M[13];
						p[i].Normal.z = VertexArray[i].Normal.x * M[2] + VertexArray[i].Normal.y * M[6] + VertexArray[i].Normal.z * M[10] + M[14];
					}

					VertexArray = p;

					// recompute metrics

					ComputeMetrics();

				}

				// ---------------------------------------------------------------
				// The bounding box is computed given a vertexarray

				vml::geo3d::AABBox ComputeVertexArrayBoundingBox(const vml::meshes::VertexArray& vertexarray)
				{
					glm::vec3 boundingboxmin( FLT_MAX,  FLT_MAX,  FLT_MAX);
					glm::vec3 boundingboxmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (size_t i = 0; i < vertexarray.size(); i++)
					{
						if (vertexarray[i].Pos.x <= boundingboxmin.x) boundingboxmin.x = vertexarray[i].Pos.x;
						if (vertexarray[i].Pos.y <= boundingboxmin.y) boundingboxmin.y = vertexarray[i].Pos.y;
						if (vertexarray[i].Pos.z <= boundingboxmin.z) boundingboxmin.z = vertexarray[i].Pos.z;
						if (vertexarray[i].Pos.x >= boundingboxmax.x) boundingboxmax.x = vertexarray[i].Pos.x;
						if (vertexarray[i].Pos.y >= boundingboxmax.y) boundingboxmax.y = vertexarray[i].Pos.y;
						if (vertexarray[i].Pos.z >= boundingboxmax.z) boundingboxmax.z = vertexarray[i].Pos.z;
					}

					return vml::geo3d::AABBox(boundingboxmin, boundingboxmax);
				}

				// ---------------------------------------------------------------
				// Updates am indexed triangle 

				void UpdateIndexedTriangle(const vml::meshes::VertexArray& vertexarray, vml::meshes::IndexedTriangleArray& trianglearray)
				{
					// recompute surface metrics

					for (size_t i = 0; i < trianglearray.size(); i++)
					{

						size_t i0 = trianglearray[i].I0;
						size_t i1 = trianglearray[i].I1;
						size_t i2 = trianglearray[i].I2;

						glm::vec3 v0(vertexarray[i0].Pos.x, vertexarray[i0].Pos.y, vertexarray[i0].Pos.z);
						glm::vec3 v1(vertexarray[i1].Pos.x, vertexarray[i1].Pos.y, vertexarray[i1].Pos.z);
						glm::vec3 v2(vertexarray[i2].Pos.x, vertexarray[i2].Pos.y, vertexarray[i2].Pos.z);

						trianglearray[i].Set((int)i, (int)i0, (int)i1, (int)i2, v0, v1, v2);

					}
				}

				// ---------------------------------------------------------------
				// rescale model to fit in a unit box along its major axis 

				void FitToUnitBox()
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot compute box fitting, mesh is not initted");

					if (vml::bits32::Get(InternalFlags, FINALIZED))
						vml::os::Message::Error("MeshBuilder : ","Cannot compute shared normals if mesh has already been finalized");
					
					float l = 1.0 / vml::math::ranges::Max(BoundingBox.GetExtents().x, BoundingBox.GetExtents().y, BoundingBox.GetExtents().z);

					for (size_t i = 0; i < VertexArray.size(); i++)
					{
						VertexArray[i].Pos.x *= l;
						VertexArray[i].Pos.y *= l;
						VertexArray[i].Pos.z *= l;
					}

					// recompute metrics

					ComputeMetrics();

				}

				// ---------------------------------------------------------------
				// compute bounding box center

				void ReCenter()
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot compute metrics, mesh is not initted");

					// recompute bounding box

					BoundingBox = ComputeVertexArrayBoundingBox(VertexArray);

					// subtract center from every vertex this centres 
					// the mesh at (0,0,0) in mesh's coordinates

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{
						VertexArray[i].Pos.x -= BoundingBox.GetCenter().x;
						VertexArray[i].Pos.y -= BoundingBox.GetCenter().y;
						VertexArray[i].Pos.z -= BoundingBox.GetCenter().z;
					}
					
					// recompute metrics

					ComputeMetrics();

				}

				// ---------------------------------------------------------------
				// Compute mesh metrics, bounding box, boung spheres and
				// center model at local origin

				void ComputeMetrics()
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot compute metrics, mesh is not initted");

					// compute bounding box

					BoundingBox = ComputeVertexArrayBoundingBox(VertexArray);

					// compute mesh bounding sphere
					
					Radius = -FLT_MAX;

					glm::vec3 center(0, 0, 0);
					
					for (size_t i = 0; i < VertexArray.size(); i++)
						center+=glm::vec3(VertexArray[i].Pos.x,VertexArray[i].Pos.y,VertexArray[i].Pos.z);
					
					float n = (float)VertexArray.size();
					
					center.x /= n;
					center.y /= n;
					center.z /= n;

					for (size_t i = 0; i < VertexArray.size(); i++)
					{
						float dx = VertexArray[i].Pos.x - center.x;
						float dy = VertexArray[i].Pos.y - center.y;
						float dz = VertexArray[i].Pos.z - center.z;

						float d = dx * dx + dy * dy + dz * dz;

						if (d >= Radius) 
							Radius = d;

					}

					Radius = sqrtf(Radius);
					
					// compute surface metrics

					UpdateIndexedTriangle(VertexArray, SurfaceArray);

					// setting flag for metrics having been computed

					vml::bits32::SetToTrue(InternalFlags, METRICS_COMPUTED);
				}


				// ---------------------------------------------------------------
				// create indices

				void CreateSurfaceIndices()
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot compute indices, mesh is not initted");

					SurfaceIndices.resize(SurfaceArray.size() * 3);

					for (size_t i = 0; i < SurfaceArray.size(); ++i)
					{
						SurfaceIndices[i * 3    ] = SurfaceArray[i].I0;
						SurfaceIndices[i * 3 + 1] = SurfaceArray[i].I1;
						SurfaceIndices[i * 3 + 2] = SurfaceArray[i].I2;

					}

					// setting flag for surface idices

					vml::bits32::SetToTrue(InternalFlags, SURFACE_INDICES_COMPUTED);

				}

				// ---------------------------------------------------------------
				// classic normal computation

				void ComputePerFaceNormals(const float eps = vml::math::EPSILON)
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot compute per face normals if mesh is not initted");

					if (vml::bits32::Get(InternalFlags, FINALIZED))
						vml::os::Message::Error("MeshBuilder : ","Cannot compute shared normals if mesh has already been finalized");

					// zero out the entire array

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{
						VertexArray[i].Normal[0] = 0.0f;
						VertexArray[i].Normal[1] = 0.0f;
						VertexArray[i].Normal[2] = 0.0f;
					}

					// compute mesh normals

					for (size_t i = 0; i < SurfaceArray.size(); ++i)
					{

						int p1 = SurfaceArray[i].I0;
						int p2 = SurfaceArray[i].I1;
						int p3 = SurfaceArray[i].I2;

						const glm::vec3& n = SurfaceArray[i].Normal;

						VertexArray[p1].Normal[0] += n.x;
						VertexArray[p1].Normal[1] += n.y;
						VertexArray[p1].Normal[2] += n.z;

						VertexArray[p2].Normal[0] += n.x;
						VertexArray[p2].Normal[1] += n.y;
						VertexArray[p2].Normal[2] += n.z;

						VertexArray[p3].Normal[0] += n.x;
						VertexArray[p3].Normal[1] += n.y;
						VertexArray[p3].Normal[2] += n.z;

					}

					// normalize vertex normals

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{

						float d = sqrtf(VertexArray[i].Normal[0] * VertexArray[i].Normal[0] +
										VertexArray[i].Normal[1] * VertexArray[i].Normal[1] +
										VertexArray[i].Normal[2] * VertexArray[i].Normal[2]);

						if (d > -eps && d < eps) d = 1.0f / eps;
							else d = 1.0f / d;

						VertexArray[i].Normal[0] *= d;
						VertexArray[i].Normal[1] *= d;
						VertexArray[i].Normal[2] *= d;
					}

				}

				// ---------------------------------------------------------------
				// shared vertices normal computation

				void ComputeSharedNormals()
				{

					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot compute shared normals if mesh is not initted");

					if (vml::bits32::Get(InternalFlags, FINALIZED))
						vml::os::Message::Error("MeshBuilder : ","Cannot compute shared normals if mesh has already been finalized");

					// vertex data structure for multimap hashing

					struct HashedVertex
					{
						glm::vec3 v;

						HashedVertex(glm::vec3 p) : v(p)
						{
						}

						__forceinline bool operator< (const HashedVertex& a) const
						{
							return memcmp(&v[0], &a.v[0], 3 * sizeof(float)) < 0;
						}

					};

					// multimap for vertex hashing

					std::multimap< HashedVertex, int > Hash;

					// zero out the entire array

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{
						VertexArray[i].Normal[0] = 0.0f;
						VertexArray[i].Normal[1] = 0.0f;
						VertexArray[i].Normal[2] = 0.0f;
					}

					// normal computation taking
					// account of shared vertices

					for (size_t i = 0; i < SurfaceArray.size(); ++i)
					{
						Hash.insert(std::pair<HashedVertex, int>(HashedVertex(VertexArray[SurfaceArray[i].I0].Pos), (int)i));
						Hash.insert(std::pair<HashedVertex, int>(HashedVertex(VertexArray[SurfaceArray[i].I1].Pos), (int)i));
						Hash.insert(std::pair<HashedVertex, int>(HashedVertex(VertexArray[SurfaceArray[i].I2].Pos), (int)i));
					}

					// find which vertex belongs to which surface and
					// accumulate normals , normalize and store in the 
					// mesh's vertex data struct

					std::multimap<HashedVertex, int>::iterator it;

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{

						std::pair<std::multimap<HashedVertex, int>::iterator, std::multimap<HashedVertex, int>::iterator> ht;

						ht = Hash.equal_range(HashedVertex(VertexArray[i].Pos));

						glm::vec3 N(0, 0, 0);

						int count = 0;

						for (it = ht.first; it != ht.second; ++it)
						{

							int SurfaceIndex = (*it).second;

							const glm::vec3& n = SurfaceArray[SurfaceIndex].Normal;

							N += n;

							count++;

						}

						N /= count;

						glm::vec3 normal = glm::normalize(N);

						VertexArray[i].Normal[0] = normal.x;
						VertexArray[i].Normal[1] = normal.y;
						VertexArray[i].Normal[2] = normal.z;

					}

				}

				// ---------------------------------------------------------------
				//

				void RemoveDuplicates()
				{
					RemoveDuplicates3D rd;
					rd.Begin(BoundingBox.GetMin(), BoundingBox.GetMax());
					rd.RemoveDuplicates(VertexArray, SurfaceArray);
					rd.Finalize();
				}

			public:

				// ---------------------------------------------------------------
				// mesh modifiers flags ( internal, written in flags member data )

				static const unsigned int INITTED					= vml::bits32::BIT0;
				static const unsigned int FINALIZED					= vml::bits32::BIT1;
				static const unsigned int METRICS_COMPUTED		    = vml::bits32::BIT2;
				static const unsigned int SURFACE_INDICES_COMPUTED  = vml::bits32::BIT3;

				// ---------------------------------------------------------------
				// mesh modifiers flags ( written in flags member data )

				static const unsigned int FIT_TO_UNIT_BOX		= vml::bits32::BIT0;
				static const unsigned int TRANSFORM_VERTICES    = vml::bits32::BIT1;
				static const unsigned int PER_FACE_NORMALS		= vml::bits32::BIT2;
				static const unsigned int SHARED_NORMALS		= vml::bits32::BIT3;
				static const unsigned int CUSTOM_NORMALS		= vml::bits32::BIT4;
				static const unsigned int RECENTER				= vml::bits32::BIT5;
				static const unsigned int REMOVE_DUPLICATES		= vml::bits32::BIT6;
							
				// ---------------------------------------------------------------
				// closes mesh definition and finalizes mesh data

				void Finalize( unsigned int prefs, const glm::vec3 &pos, const glm::vec3& rot, const glm::vec3& scale)
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot validate mesh is not initted");

					if (VertexArray.size() == 0 )
						vml::os::Message::Error("MeshBuilder : ","Cannot validate, mesh has null vertices arrays");

					if ( SurfaceArray.size() == 0)
						vml::os::Message::Error("MeshBuilder : ","Cannot validate, mesh has null surfaces arrays");

					PreferencesFlags = prefs;
					
					// do not change the order

					if (vml::bits32::Get(PreferencesFlags, PER_FACE_NORMALS) &&
						vml::bits32::Get(PreferencesFlags, SHARED_NORMALS) &&
						vml::bits32::Get(PreferencesFlags, CUSTOM_NORMALS) )
							vml::os::Message::Error("MeshBuilder : ","Cannot compute normals");
				
					//
					/*
					glm::vec3 center(0, 0, 0);
					for (size_t i = 0; i < VertexArray.size(); i++)
						center += glm::vec3(VertexArray[i].Pos.x, VertexArray[i].Pos.y, VertexArray[i].Pos.z);
					float n = (float)VertexArray.size();
					center.x /= n;
					center.y /= n;
					center.z /= n;
					for (size_t i = 0; i < VertexArray.size(); i++)
					{
						VertexArray[i].Pos.x -= center.x;
						VertexArray[i].Pos.y -= center.y;
						VertexArray[i].Pos.z -= center.z;
					}

					std::cout << "Center 1 :" << center.x << " " << center.y << " " << center.z << std::endl;
					*/
					//
					
					// compute metrics 

					ComputeMetrics();
					
					// compute further operations accoridng to flag

					if (vml::bits32::Get(PreferencesFlags, RECENTER))
						ReCenter();

					if (vml::bits32::Get(PreferencesFlags, FIT_TO_UNIT_BOX))
						FitToUnitBox();

					if (vml::bits32::Get(PreferencesFlags, TRANSFORM_VERTICES))
						Transform(pos, rot, scale);

					if (vml::bits32::Get(PreferencesFlags, PER_FACE_NORMALS))
						ComputePerFaceNormals();

					if (vml::bits32::Get(PreferencesFlags, SHARED_NORMALS))
						ComputeSharedNormals();

					if (vml::bits32::Get(PreferencesFlags, CUSTOM_NORMALS))
					{
						// normals are given by user
					}

					if (vml::bits32::Get(PreferencesFlags, REMOVE_DUPLICATES))
						RemoveDuplicates();

					// create surface indices for vbo indexing

					CreateSurfaceIndices();

					// finalize mesh

					vml::bits32::SetToTrue(InternalFlags, FINALIZED);

			
				}

				// ---------------------------------------------------------------
				// add a surface to this mesh
				
				void AddSurface(const glm::ivec3& vertexindices)
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot add surface, mesh is not initted");
					if (vml::bits32::Get(InternalFlags, FINALIZED))
						vml::os::Message::Error("MeshBuilder : ","Cannot add surface, mesh is already finalize");
					
					// retrieve vertex indices

					int I0 = vertexindices.x;
					int I1 = vertexindices.y;
					int I2 = vertexindices.z;

					int skipface = 0;

					if (I0 < 0 || I0 >= VertexArray.size())
					{
				//		vml::os::Warning("MeshBuilder : Out of range vertex indices I0 : %d for mesh : %s", I0, ResourceFileName.c_str());
						skipface = 1;
					}

					if (I1 < 0 || I1 >= VertexArray.size())
					{
				//		vml::os::Warning("MeshBuilder : Out of range vertex indices I1 : %d for mesh : %s", I1, ResourceFileName.c_str());
						skipface = 1;
					}

					if (I2 < 0 || I2 >= VertexArray.size())
					{
				//		vml::os::Warning("MeshBuilder : Out of range vertex indices I2 : %d for mesh : %s", I2, ResourceFileName.c_str());
						skipface = 1;
					}

					//	checks for equal indicies 

					if (I0 == I1 || I1 == I2 || I0 == I2)
					{
						vml::os::Message::Trace("MeshBuilder : ","Skipping coincident vertex indices face I0 : ",I0," I1 : ",I1," I2 : ", I2);
						skipface = 1;
					}

					// if we get here triangle is not degenerated
					// so we can create a new surface and insert into the
					// surface array 

					if (!skipface)
					{
						// retrieve point coordinates and transform into glm vectors

						glm::vec3 V0 = glm::vec3(VertexArray[I0].Pos.x, VertexArray[I0].Pos.y, VertexArray[I0].Pos.z);
						glm::vec3 V1 = glm::vec3(VertexArray[I1].Pos.x, VertexArray[I1].Pos.y, VertexArray[I1].Pos.z);
						glm::vec3 V2 = glm::vec3(VertexArray[I2].Pos.x, VertexArray[I2].Pos.y, VertexArray[I2].Pos.z);

						// check equal for points 

						if ((vml::math::ranges::CloseEnough(V0.x, V1.x) && 
							 vml::math::ranges::CloseEnough(V0.y, V1.y) && 
							 vml::math::ranges::CloseEnough(V0.z, V1.z)))
								vml::os::Message::Warning("MeshBuilder : ","Coincident vertex V0 : ", V0.x, V0.y, V0.z, " V1 :",V1.x, V1.y, V1.z);

						if ((vml::math::ranges::CloseEnough(V1.x, V2.x) && 
							 vml::math::ranges::CloseEnough(V1.y, V2.y) && 
							 vml::math::ranges::CloseEnough(V1.z, V2.z)))
								vml::os::Message::Warning("MeshBuilder : ","Coincident vertex V1 : ", V1.x, V1.y, V1.z, "V2 :" ,V2.x, V2.y, V2.z);

						if ((vml::math::ranges::CloseEnough(V0.x, V2.x) && 
							 vml::math::ranges::CloseEnough(V0.y, V2.y) && 
							 vml::math::ranges::CloseEnough(V0.z, V2.z)))
								vml::os::Message::Warning("MeshBuilder : ","Coincident vertex V1 : ", V0.x, V0.y, V0.z, "V2 :", V2.x, V2.y, V2.z);

						// add surface

						SurfaceArray.emplace_back(vml::geo3d::IndexedTriangle((int)SurfaceArray.size(), I0, I1, I2, V0, V1, V2));

					}
					
				}
								
				// ---------------------------------------------------------------
				// vertex ,normal and color adding

				void AddVertex(const glm::vec3 &p,
							   const glm::vec3 &n  = glm::vec3(0,0,0),
							   const glm::vec2 &uv = glm::vec2(0,0),
							   const glm::vec4 &c  = glm::vec4(1,1,1,1))
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot add vertex, mesh is not initted");
					if (vml::bits32::Get(InternalFlags, FINALIZED))
						vml::os::Message::Error("MeshBuilder : ","Cannot add surface, mesh is already finalize");

					// push new vertex structure into vertex array
					
					VertexArray.emplace_back (vml::geo3d::Vertex(p, n, uv, c));

				}

				// ---------------------------------------------------------------
				// begin mesh definition

				void Begin(const std::string& resourcefilename)
				{
					InternalFlags      = 0;
					PreferencesFlags   = 0;
					Radius             = 0;
					BoundingBox		   = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					ResourceFileName   = resourcefilename;
					VertexArray.clear();
					SurfaceArray.clear();
					SurfaceIndices.clear();
					vml::bits32::SetToTrue(InternalFlags, INITTED);
				}

				// ---------------------------------------------------------------
				// save mesh given current path

				void Save()
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot save, mesh : not initted");

					vml::os::Message::Trace("MeshBuilder : ","Saving ", ResourceFileName.c_str());
					
					FILE * stream;

					errno_t err = fopen_s(&stream, ResourceFileName.c_str(), "wb");

					if (err == 0)
					{

	//					vml::os::Trace("BMin : %f %f %f\n", BoundingBox.GetMin().x, BoundingBox.GetMin().y, BoundingBox.GetMin().z);
	//					vml::os::Trace("BMax : %f %f %f\n", BoundingBox.GetMax().x, BoundingBox.GetMax().y, BoundingBox.GetMax().z);
	//					vml::os::Trace("Extents : %f %f %f\n", BoundingBox.GetExtents().x, BoundingBox.GetExtents().y, BoundingBox.GetExtents().z);
//						vml::os::Trace("Radius : %f\n", Radius);

						// save number of vertices
						
						unsigned int vertices = (unsigned int)VertexArray.size();
						fwrite(&vertices, sizeof(unsigned int), 1, stream);

						vml::os::Message::Trace("MeshBuilder : ","Saving ",vertices," vertices");

						// save vertices

						for (size_t i = 0; i < vertices; ++i)
						{
							fwrite(&VertexArray[i].Pos    , sizeof(float), 4, stream);
							fwrite(&VertexArray[i].Normal , sizeof(float), 3, stream);
							fwrite(&VertexArray[i].UV     , sizeof(float), 2, stream);
							fwrite(&VertexArray[i].Color  , sizeof(float), 4, stream);
						}
					
						// save surfaces indices
						
						unsigned int indices = (int)SurfaceIndices.size();
						fwrite(&indices, sizeof(unsigned int), 1, stream);
						fwrite(&SurfaceIndices[0], sizeof(unsigned int), indices, stream);

						vml::os::Message::Trace("MeshBuilder : ","Saving ",indices,"indices");

						// save metrics

						fwrite(&BoundingBox.GetMin(), sizeof(float), 3, stream);
						fwrite(&BoundingBox.GetMax(), sizeof(float), 3, stream);
						fwrite(&Radius				, sizeof(float), 1, stream);

						// save flags

						fwrite(&InternalFlags, sizeof(unsigned int), 1, stream);

						// Close stream if it is not NULL 

						if (fclose(stream) != 0)
							vml::os::Message::Error("MeshBuilder : ","Cannot close mesh file ' ", ResourceFileName.c_str()," '");

						vml::os::Message::Trace("MeshBuilder : ","Finished Saving ", ResourceFileName.c_str());

					}
					else
					{
						vml::os::Message::Error("MeshBuilder : ","Cannot save mesh ' ", ResourceFileName.c_str()," ' (error code : ", err," )");
					}
					
				}

				// ---------------------------------------------------------------
				// dump to string

				const std::string DumpMeshDataToString()
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : Cannot dump, mesh ", ResourceFileName.c_str() ," is not initted");

					std::string text;

					text = "MeshBuilder *Mesh = new MeshBuilder(\"" + ResourceFileName+"\");\n";
					text += "Mesh->Begin();\n";

					for (size_t i = 0; i < VertexArray.size(); ++i)
					{

						float x = VertexArray[i].Pos.x;
						float y = VertexArray[i].Pos.y;
						float z = VertexArray[i].Pos.z;
						float w = 1.0f;

						float nx = VertexArray[i].Normal[0];
						float ny = VertexArray[i].Normal[1];
						float nz = VertexArray[i].Normal[2];

						float u = VertexArray[i].UV[0];
						float v = VertexArray[i].UV[1];

						text += std::format("Mesh->AddVertex( glm::vec3( {0} , {1} , {2} ),glm::vec3( {3} , {4} , {5} ),glm::vec2 ( {6} ,{7}) );\n",x, y, z, nx, ny, nz, u, v);
						
					}

					for (size_t i = 0; i < SurfaceArray.size(); ++i)
					{
						int i0 = SurfaceArray[i].I0;
						int i1 = SurfaceArray[i].I1;
						int i2 = SurfaceArray[i].I2;

						text += std::format("Mesh->AddSurface(glm::ivec3( {} , {} , {} ) );\n", i0, i1, i2);

					}
					
					text += "Mesh->End();\n";
					text += "Mesh->Save();\n";
					text += "delete Mesh;\n";

					return text;
				}

				// ---------------------------------------------------------------
				// dumps mesh data to file 

				void DumpMeshDataToFile(const std::string &filename)
				{
					std::string FileName = vml::strings::StringUtils::TrimAndLower(filename);
					std::ofstream stream;
					stream.open(FileName);
					stream << DumpMeshDataToString().c_str();
					stream.close();
				}

				// ---------------------------------------------------------------
				// dumps mesh data to file 

				void DumpMeshInfoToFile(const std::string &filename)
				{
					std::string FileName = vml::strings::StringUtils::TrimAndLower(filename);
					std::ofstream stream;
					stream.open(FileName);
					stream << DumpMeshInfoToString().c_str();
					stream.close();
				}
				
				// ---------------------------------------------------------------
				// dumps mesh flags

				const std::string DumpMeshInfoToString()
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot dump, mesh ' " , ResourceFileName.c_str()," ' is not initted");

					// dump mesh

					std::string text = std::format("FileName : {}\n", ResourceFileName.c_str());

					if (vml::bits32::Get(InternalFlags, INITTED)) text += "Initted : yes\n";
					else text += "Initted : no\n";

					text += std::format("Vertices : {}\n", VertexArray.size());
					text += std::format("Surfaces : {}\n", SurfaceArray.size());
					text += std::format("Indices  : {}\n", SurfaceIndices.size());
					text += std::format("Bounding box minimum : {} , {} , {}\n", BoundingBox.GetMin().x, BoundingBox.GetMin().y, BoundingBox.GetMin().z);
					text += std::format("Bounding box maximum : {} , {} , {}\n", BoundingBox.GetMax().x, BoundingBox.GetMax().y, BoundingBox.GetMax().z);
					text += std::format("Bounding box extents : {} , {} , {}\n", BoundingBox.GetExtents().x, BoundingBox.GetExtents().y, BoundingBox.GetExtents().z);
					text += std::format("Radius : {}\n", Radius);

					if (vml::bits32::Get(InternalFlags, METRICS_COMPUTED))
						text += "Metrics computed\n";

					if (vml::bits32::Get(InternalFlags, SURFACE_INDICES_COMPUTED))
						text += "Surface Indices computed\n";

					if (vml::bits32::Get(InternalFlags, FINALIZED))
						text += "Mesh is finalized: yes\n";
					else
						text += "Mesh is finalized : no\n";

					if (vml::bits32::Get(PreferencesFlags, TRANSFORM_VERTICES))
						text += "Vertices transformed\n";

					if (vml::bits32::Get(PreferencesFlags, FIT_TO_UNIT_BOX))
						text += "Fit to unit box\n";

					if (vml::bits32::Get(PreferencesFlags, PER_FACE_NORMALS))
						text += "Normals: Per face\n";

					if (vml::bits32::Get(PreferencesFlags, SHARED_NORMALS))
						text += "Normals: Shared vertices";

					text += std::format("Bitflag array: {}\n", InternalFlags);

					return text;
				}

				// ---------------------------------------------------------------
				
				void CreateMeshDataBase(const std::string &path)
				{
					vml::os::Message::Trace("MeshBuilder : ","Initting mesh database generator, path : ", path.c_str());
					
					// create cube

					{
						vml::os::Message::Trace("MeshBuilder : ","Creating cube - starting -");

						int verts = vml::meshes::procedural::cube::Vertices;
						int indices = vml::meshes::procedural::cube::Surfaces;

						const float* varray = vml::meshes::procedural::cube::VertexPositionArray;
						const float* narray = vml::meshes::procedural::cube::VertexNormalArray;
						const float* uvarray = vml::meshes::procedural::cube::VertexUVArray;
						const unsigned int* trianglesarray = vml::meshes::procedural::cube::TrianglesArray;

						Begin(path + "cube.3df");

						for (int i = 0; i < verts; ++i)
						{
							glm::vec4 position(varray[i * 4    ],
											   varray[i * 4 + 1],
											   varray[i * 4 + 2],
											   varray[i * 4 + 3]);

							glm::vec3 normal(narray[i * 3    ],
											 narray[i * 3 + 1],
											 narray[i * 3 + 2]);

							glm::vec2 uv(uvarray[i * 2    ],
										 uvarray[i * 2 + 1]);

							AddVertex(position, normal, uv);
						}

						for (int i = 0; i < indices; ++i)
						{
							AddSurface(glm::ivec3(trianglesarray[i * 3    ],
												  trianglesarray[i * 3 + 1],
												  trianglesarray[i * 3 + 2]));
						}

						Finalize(0, vml::geo3d::NullVec, vml::geo3d::NullVec, vml::geo3d::UnaryVec);

						Save();

						vml::os::Message::Trace("MeshBuilder : ","Creating cube - finish -");

						vml::os::Message::Trace("MeshBuilder : ", DumpMeshInfoToString().c_str());
						vml::os::Message::Trace("MeshBuilder : ", DumpMeshDataToString().c_str());
					
					}

					// create plane

					{
					
						vml::os::Message::Trace("MeshBuilder : ","Creating plane - starting -");

						int verts = vml::meshes::procedural::plane::Vertices;
						int indices = vml::meshes::procedural::plane::Surfaces;

						const float* varray = vml::meshes::procedural::plane::VertexPositionArray;
						const float* narray = vml::meshes::procedural::plane::VertexNormalArray;
						const float* uvarray = vml::meshes::procedural::plane::VertexUVArray;
						const unsigned int* trianglesarray = vml::meshes::procedural::plane::TrianglesArray;

						Begin(path + "plane.3df");

						for (int i = 0; i < verts; ++i)
						{
							glm::vec4 position(varray[i * 4    ],
											   varray[i * 4 + 1],
											   varray[i * 4 + 2],
											   varray[i * 4 + 3]);

							glm::vec3 normal(narray[i * 3    ],
											 narray[i * 3 + 1],
											 narray[i * 3 + 2]);

							glm::vec2 uv(uvarray[i * 2   ],
										 uvarray[i * 2 + 1]);

							AddVertex(position, normal, uv);
						}

						for (int i = 0; i < indices; ++i)
						{
							AddSurface(glm::ivec3(trianglesarray[i * 3    ],
												  trianglesarray[i * 3 + 1],
												  trianglesarray[i * 3 + 2]));
						}

						Finalize(0, vml::geo3d::NullVec, vml::geo3d::NullVec, vml::geo3d::UnaryVec);

						Save();

						vml::os::Message::Trace("MeshBuilder : ", "Creating plane - finish -");

						vml::os::Message::Trace("MeshBuilder : ", DumpMeshInfoToString().c_str());
						vml::os::Message::Trace("MeshBuilder : ", DumpMeshDataToString().c_str());

					}


					vml::os::Message::Trace("MeshBuilder : ","Closing mesh database generator");
				}

				// ---------------------------------------------------------------
				//	query functions 

				unsigned int GetFlags()						const { return InternalFlags; }
				bool AreNormalsComputed()					const { return vml::bits32::Get(PreferencesFlags, PER_FACE_NORMALS); }
				bool AreSharedNormalsComputed()				const { return vml::bits32::Get(PreferencesFlags, SHARED_NORMALS); }
				bool IsBoxNormalized()						const { return vml::bits32::Get(PreferencesFlags, FIT_TO_UNIT_BOX); }
				bool AreVerticesTransformed()				const { return vml::bits32::Get(PreferencesFlags, TRANSFORM_VERTICES); }
				bool IsFinalized()							const { return vml::bits32::Get(InternalFlags, FINALIZED); }
				size_t GetIndicesCount()					const { return SurfaceIndices.size(); }
				size_t GetSurFaceCount()					const { return SurfaceArray.size(); }
				size_t GetVertexCount()						const { return VertexArray.size(); }
				float GetRadius()							const { return Radius; }
				float GetWidth()							const { return BoundingBox.GetExtents().x; }
				float GetHeight()							const { return BoundingBox.GetExtents().y; }
				float GetDepth()							const { return BoundingBox.GetExtents().z; }
				bool IsInitted()							const { return vml::bits32::Get(InternalFlags, INITTED); }
				const vml::geo3d::AABBox &GetBoundingBox()  const { return BoundingBox; }
				const std::string& GetResourceFileName()	const { return ResourceFileName; }
				bool ValidateMesh()							const { return VertexArray.size() > 0 && SurfaceArray.size() > 0; }
				glm::vec3 GetVertexPosAt(const size_t i)	const { return glm::vec3(VertexArray[i].Pos.x, VertexArray[i].Pos.y, VertexArray[i].Pos.z); }
				glm::vec3 GetVertexNormalAt(const size_t i) const { return glm::vec3(VertexArray[i].Normal[0], VertexArray[i].Normal[1], VertexArray[i].Normal[2]); }
				glm::vec2 GetVertexUVAt(const size_t i)		const { return glm::vec2(VertexArray[i].UV[0], VertexArray[i].UV[1]); }
				glm::vec4 GetVertexColorAt(const size_t i)	const { return glm::vec4(VertexArray[i].Color[0], VertexArray[i].Color[1], VertexArray[i].Color[2], VertexArray[i].Color[3]); }

				// ---------------------------------------------------------------
				// returns a mesh vertex3d data struct from mesbuilder vertex class

				vml::geo3d::Vertex GetVertexAt(int pos)
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot return vertex, mesh is not initted");
					return VertexArray[pos];
				}

				// ---------------------------------------------------------------
				// returns pointer to surface at position

				const vml::geo3d::IndexedTriangle& GetSurfaceAt(int pos) const
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot return face, mesh is not initted");
					return SurfaceArray[pos];
				}

				// ----------------------------------------------------------------
				// get a vector containig the indices af a single surface

				const glm::ivec3 GetIndicesAtSurface(const size_t id) const
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot return indices, mesh is not initted");
					return glm::ivec3(SurfaceArray[id].I0, SurfaceArray[id].I1, SurfaceArray[id].I2);
				}

				// ---------------------------------------------------------------
				// returns pointer to vertex array

				const std::vector<vml::geo3d::Vertex>& GetVertexArray() const
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot return vertex array, mesh is not initted");
					return VertexArray;
				}

				// ---------------------------------------------------------------
				// returns pointer to surface index buffer

				const std::vector<int>& GetSurfaceIndexBuffer() const
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot return surface indices array, mesh is not initted");

					return SurfaceIndices;
				}

				// array of surfaces

				const std::vector<vml::geo3d::IndexedTriangle>& GetSurfaceArray() const
				{
					if (!vml::bits32::Get(InternalFlags, INITTED))
						vml::os::Message::Error("MeshBuilder : ","Cannot return surface array, mesh is not initted");
					return SurfaceArray;
				}

				// ---------------------------------------------------------------
				// constructors / destructors

				MeshBuilder()
				{
					InternalFlags		= 0;
					PreferencesFlags	= 0;
					Radius				= 0;
					VertexBufferObject  = 0;
					IndexBufferObject   = 0;
					BoundingBox			= vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					ResourceFileName    = "";
				}

				// destructor

				~MeshBuilder()
				{
					ReleaseAll();
				}
		
		};	
			
	} // end of meshes namespace

} // end of vml namespace

