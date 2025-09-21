#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	namespace scenes
	{

		// -------------------------------------------------------------------
		// Level class, this class holds everything about 
		// the level mesh, collision and navigation data

		class Level
		{

			private:

				std::string			       FileName;
				std::string			       MapMeshFileName;
				std::string			       ColMeshFileName;
				std::string			       NavMeshFileName;
				vml::octree::OctTree*      OctTree;
				vml::geo2d::PathFinder*    PathFinder;
				vml::meshes::Mesh3d*       MapMesh;
				vml::meshes::Mesh3d*       CollisionMesh;
				vml::meshes::Mesh3d*       NavMesh;
				vml::octree::MeshCollider* MeshCollider;

				bool Loaded;

				// -------------------------------------------------------------------
				// release memory

				void ReleaseAll()
				{
					// release memory

					vml::SafeDelete(MapMesh);
					vml::SafeDelete(NavMesh);
					vml::SafeDelete(CollisionMesh);
					vml::SafeDelete(OctTree);
					vml::SafeDelete(PathFinder);
					vml::SafeDelete(MeshCollider);

					// set loaded flag as false

					Loaded = false;

				}

			public:

				// -------------------------------------------------------------------
				// 

				bool Load(const std::string& filename)
				{
					
					if (filename.empty())
						vml::os::Message::Error("Level : ","FileName is empty");
				
					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Level", "Loading Level", "Start");

					// reset data

					ReleaseAll();

					// set filname

					FileName = filename;

					// create map mesh data

					std::string mainpath = vml::Session::GetInstance()->GetMainPath();

					MapMeshFileName = mainpath + "\\content\\fa2040\\meshes\\" + FileName + "\\" + FileName + ".3df";
					ColMeshFileName = mainpath + "\\content\\fa2040\\meshes\\" + FileName + "\\" + FileName + "_col.3df";
					NavMeshFileName = mainpath + "\\content\\fa2040\\meshes\\" + FileName + "\\" + FileName + "_nav.3df";

					MapMesh		  = new vml::meshes::Mesh3d(MapMeshFileName);
					CollisionMesh = new vml::meshes::Mesh3d(ColMeshFileName);
					NavMesh		  = new vml::meshes::Mesh3d(NavMeshFileName);

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Level", "Loading MapMesh", MapMeshFileName.c_str());
					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Level", "Loading CollisionMesh", ColMeshFileName.c_str());
					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Level", "Loading NavMesh", NavMeshFileName.c_str());
					
					// load bitmap mask for pathfinding 
					
					PathFinder = new vml::geo2d::PathFinder(mainpath + "\\content\\fa2040\\meshes\\" + FileName + "\\"+ FileName + "_nav_mask.txt");

					// create octree

					OctTree = new vml::octree::OctTree;

					// compile map

					OctTree->Go(MapMesh);

					// create data struct for collision

					MeshCollider = new vml::octree::MeshCollider();
					MeshCollider->Go(CollisionMesh, 16);

					// log out

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Level", "Octree Compiled", "");

					// we can release map mesh data since we don't use it anymore once the octree is computed

					MapMesh->ReleaseVertexBufferData();
					NavMesh->ReleaseVertexBufferData();

					// se loaded flags as true
					
					Loaded = true;

					// log out

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Level", "Loading Level", "Done");
					
					return true;
				}

				// -------------------------------------------------------------------
				// getters

				vml::octree::OctTree      *GetOctTree()	          const { return OctTree; }
				vml::octree::MeshCollider *GetMeshCollider()	  const { return MeshCollider; }
				vml::meshes::Mesh3d       *GetMapMesh()	          const { return MapMesh; }
				vml::meshes::Mesh3d       *GetCollisionMesh()     const { return CollisionMesh; }
				vml::meshes::Mesh3d       *GetNavMesh()           const { return NavMesh; }
				vml::geo2d::PathFinder    *GetPathFinder()	      const { return PathFinder; }
				const std::string         &GetFileName()	      const { return FileName; }
				const std::string         &GetMapMeshFileName()	  const { return MapMeshFileName; }
				const std::string         &GetColMeshFileName()	  const { return ColMeshFileName; }
				const std::string         &GetNavMeshFileName()	  const { return NavMeshFileName; }

				// -------------------------------------------------------------------
				// query

				bool  IsLoaded() const { return Loaded; }

				// -------------------------------------------------------------------
				// 

				// -------------------------------------------------------------------
				// ctor / dtor

				Level()
				{
					FileName      = "";
					MapMesh       = nullptr;
					NavMesh		  = nullptr;
					CollisionMesh = nullptr;
					OctTree       = nullptr;
					PathFinder	  = nullptr;
					MeshCollider  = nullptr;
					Loaded		  = false;

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Level", "Initting Level", "Done");
				}

				~Level()
				{
					ReleaseAll();

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Level", "Releasing Level", "Done");
				}

		};

	}
}
