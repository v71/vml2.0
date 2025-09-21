#pragma once

namespace vml
{
	namespace scenes
	{
		class Scene
		{

			private:

				// -------------------------------------------------------------------
				//

				std::string  Name;
				uint32		 InternalFlags;

				// -------------------------------------------------------------------
				//

				vml::scenes::Level			  *Level;
				vml::objects::ObjectManager_2 *ObjectManager;
				vml::NvidiaPhysXEngine		  *NvPhysX;

			public:


				// -------------------------------------------------------------------
				//

				vml::objects::Object3d_2* AddCube(const std::string &scname,const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling = glm::vec3(1, 1, 1),
					                              int flags= vml::objects::Model3d_2::EULER)
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ","Scene is not initted");

					// add object

					vml::objects::Object3d_2* currentobject;
					
					// create an integer 'name' for this object

					currentobject = new vml::objects::Object3d_2(scname);

					// add model to object

					currentobject->AddModel("Root",
											vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\meshes\\cube.3df",
											vml::objects::ZeroPos, vml::objects::ZeroRot, vml::objects::UnaryScale,
											flags );

					// finalize object

					currentobject->Finalize(pos, rot, scaling);

					// add this shit

					ObjectManager->AddObject(currentobject);

					// return object pointer

					return currentobject;
				}

				// -------------------------------------------------------------------
				//

				vml::objects::Object3d_2* AddPlane(const std::string& scname, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling = glm::vec3(1, 1, 1),
												   int flags = vml::objects::Model3d_2::EULER)
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ","Scene is not initted");

					// add object

					// generate screenname automatically

					vml::objects::Object3d_2* currentobject;

					// create an integer 'name' for this object

					currentobject = new vml::objects::Object3d_2(scname);

					// add model to object

					currentobject->AddModel("Root",
											vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\meshes\\plane.3df",
											vml::objects::ZeroPos, vml::objects::ZeroRot, vml::objects::UnaryScale,
											flags );

					// finalize object

					currentobject->Finalize(pos, rot, scaling);

					// add this shit

					ObjectManager->AddObject(currentobject);

					// return object pointer

					return currentobject;
				}

				// -------------------------------------------------------------------
				//

				void LoadLevel(const std::string& filename)
				{
					// remove all objects frm obejct manaer

					ObjectManager->RemoveAllObjects();

					// load level

					Level->Load(filename);
					
				}

				// ---------------------------------------------------------------
				//

				void BuildLevelPhysics() const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");
					if ( !Level->IsLoaded())
						vml::os::Message::Error("Level : ", "Level is not initted");

					// cretae the bvh 

					NvPhysX->CreateBVHFromMesh(Level->GetCollisionMesh());

					// once the bvh is created we do not need anymore the navmesh data

					Level->GetNavMesh()->ReleaseVertexBufferData();

				}

				// -------------------------------------------------------------------
				//

				vml::objects::ObjectManager_2* GetObjectManager() const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ", "Scene is not initted");

					return ObjectManager;
				}

				// -------------------------------------------------------------------
				//

				vml::scenes::Level* GetLevel() const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ","Scene is not initted");

					return Level;
				}

				// -------------------------------------------------------------------
				//

				void TransformObjects(vml::views::View* view)
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ","Scene is not initted");

					ObjectManager->TransformPipeline(view);
				}

				// -------------------------------------------------------------------
				//
			
				void StepPhysics()
				{
					NvPhysX->StepPhysics();
				}

				// -------------------------------------------------------------------
				//
				
				void Clear()
				{
					// release memory

					vml::SafeDelete(Level);
					vml::SafeDelete(ObjectManager);
					vml::SafeDelete(NvPhysX);

					// reallocate classes

					Name		  = "";
					Level		  = new vml::scenes::Level();
					ObjectManager = new vml::objects::ObjectManager_2();
					NvPhysX		  = new vml::NvidiaPhysXEngine();
				
					// set flags

					InternalFlags = vml::InternalFlags::INITTED;

					// log message

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Scene", "Cleared", "Done");

				}
				
				// -------------------------------------------------------------------
				//
				
				bool RayCastClosest(const glm::vec3 &o,const glm::vec3 &dir, physx::PxRaycastBuffer &hit,const float maxdist= FLT_MAX)
				{
					physx::PxVec3 origin(physx::PxVec3(o.x, o.y, o.z));
					physx::PxVec3 unitDir(dir.x, dir.y, dir.z);
					physx::PxReal maxDistance = maxdist;
					return NvPhysX->gScene->raycast(origin, unitDir, maxDistance, hit);
				}
				
				bool RayCastAny(const glm::vec3& o, const glm::vec3& dir, physx::PxRaycastBuffer& hit, const float maxdist = FLT_MAX)
				{
					physx::PxVec3 origin(physx::PxVec3(o.x, o.y, o.z));
					physx::PxVec3 unitDir(dir.x, dir.y, dir.z);
					physx::PxReal maxDistance = maxdist;
					PxQueryFilterData fdAny;
					fdAny.flags |= PxQueryFlag::eANY_HIT; // note the OR with the default value
					return NvPhysX->gScene->raycast(origin, unitDir, maxDistance, hit, PxHitFlags(PxHitFlag::eDEFAULT), fdAny);
				}
				
				// -------------------------------------------------------------------
				//

				void DrawDebugObjects(vml::views::View* view)
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ","Scene is not initted");

					for (size_t j = 0; j < ObjectManager->GetObjectsCount(); ++j)
					{
						vml::objects::Object3d_2* object = ObjectManager->GetObjectAt(j);

						if (object->IsInFrustum())
						{
							for (size_t i = 0; i < object->GetModelsCount(); ++i)
							{
								vml::objects::Model3d_2* model = object->GetModelAt(i);

							//	vml::DebugRender->DrawAOBBox(view, model, vml::colors::Yellow, false, 0.1f);
								
								if ( model->IsWire())
									vml::DebugRender->DrawModelWire(view, model);
								else
									vml::DebugRender->DrawModelSolid(view, model);
							}
						}
					}
				}

				// -------------------------------------------------------------------
				//

				void DrawDebugObject(vml::views::View* view, vml::objects::Object3d_2* object) const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
						vml::os::Message::Error("Scene : ","Scene is not initted");

					if (object->IsInFrustum())
					{
						for (size_t i = 0; i < object->GetModelsCount(); ++i)
						{
							vml::objects::Model3d_2* model = object->GetModelAt(i);

							//	vml::DebugRender->DrawAOBBox(view, model, vml::colors::Yellow, false, 0.1f);

							if (model->IsWire())
								vml::DebugRender->DrawModelWire(view, model);
							else
								vml::DebugRender->DrawModelSolid(view, model);
						}
					}
				}
							
	
				// -------------------------------------------------------------------
				// ctor / dtor

				Scene()
				{
					Name		  = "";
					Level		  = new vml::scenes::Level();
					ObjectManager = new vml::objects::ObjectManager_2();
					NvPhysX		  = new vml::NvidiaPhysXEngine();

					// set flags

					InternalFlags = vml::InternalFlags::INITTED;

					// log message

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Scene", "Init", "Done");
				}

				virtual ~Scene()
				{
					vml::SafeDelete(Level);
					vml::SafeDelete(ObjectManager);
					vml::SafeDelete(NvPhysX);

					// log message

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Scene", "Releasing", "Done");
				}
		};

	}
}
