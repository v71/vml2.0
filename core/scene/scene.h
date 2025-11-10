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
				
				void Clear()
				{
					// release memory

					vml::SafeDelete(Level);
					vml::SafeDelete(ObjectManager);

					// reallocate classes

					Name		  = "";
					Level		  = new vml::scenes::Level();
					ObjectManager = new vml::objects::ObjectManager_2();
				
					// set flags

					InternalFlags = vml::InternalFlags::INITTED;

					// log message

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Scene", "Cleared", "Done");

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

					// set flags

					InternalFlags = vml::InternalFlags::INITTED;

					// log message

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Scene", "Init", "Done");
				}

				virtual ~Scene()
				{
					vml::SafeDelete(Level);
					vml::SafeDelete(ObjectManager);

					// log message

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Scene", "Releasing", "Done");
				}
		};

	}
}
