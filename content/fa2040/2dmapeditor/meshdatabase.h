#pragma once

namespace fa2040
{
	namespace tools
	{

		class MeshDataBase
		{

			// ----------------------------------------------------------------------------
			
			class MeshDataBaseItem
			{
				public:

					std::string Name;
					size_t Count;

					// if this class is instantiated, then its count is starting with a 1

					MeshDataBaseItem(const std::string& name) : Name(name), Count(1)
					{
					}

					~MeshDataBaseItem()
					{
					}
			};

			// ----------------------------------------------------------------------------

			class MeshDataBaseRow
			{
				public:

					std::string Name;
					std::vector<MeshDataBaseItem> Items;

					std::vector<MeshDataBaseItem>::iterator FindItem(const std::string& name)
					{
						auto it = Items.begin();
						for (; it != Items.end(); ++it)
							if ((*it).Name == name) return it;
						return it;
					}

					// ----------------------------------------------------------------------------

					void SortByNames()
					{
						struct less_than_key
						{
							inline bool operator() (const MeshDataBaseItem& struct1, const MeshDataBaseItem& struct2)
							{
								return (struct1.Name < struct2.Name);
							}
						};

						std::sort(Items.begin(), Items.end(), less_than_key());
					}

					// ----------------------------------------------------------------------------

					void SortByRefs()
					{
						struct less_than_key
						{
							inline bool operator() (const MeshDataBaseItem& struct1, const MeshDataBaseItem& struct2)
							{
								return (struct1.Count < struct2.Count);
							}
						};

						std::sort(Items.begin(), Items.end(), less_than_key());
					}

					// ----------------------------------------------------------------------------
					// ctor / dtor

					MeshDataBaseRow(const std::string& name) :Name(name)
					{
					}

					~MeshDataBaseRow()
					{
					}
			};

			// ----------------------------------------------------------------------------

			std::vector<MeshDataBaseRow>::iterator FindRow(const std::string& name)
			{
				auto it = Map.begin();
				for (; it != Map.end(); ++it)
					if ((*it).Name == name) return it;
				return it;
			}

			// ---------------------------------------------------------

			void SortByNames()
			{
				for (size_t i = 0; i < Map.size(); ++i)
				{
					Map[i].SortByNames();
				}
			}

			// ---------------------------------------------------------

			void SortByRefs()
			{
				for (size_t i = 0; i < Map.size(); ++i)
				{
					Map[i].SortByRefs();
				}
			}

		public:

			// ----------------------------------------------------------------------------
			// map to store mesh references

			std::vector<MeshDataBaseRow> Map;
			bool Sort;

			// ----------------------------------------------------------------------------
			// create database

			void CreateDataBase(vml::objects::ObjectManager_2* objectmanager)
			{

				Map.clear();

				for (size_t i = 0; i < objectmanager->GetObjectsCount(); ++i)
				{
					vml::objects::Object3d_2* object = objectmanager->GetObjectAt(i);

					std::string objectname = object->GetScreenName();

					for (size_t j = 0; j < object->GetModelsCount(); ++j)
					{
						vml::objects::Model3d_2* model = object->GetModelAt(j);

						for (size_t k = 0; k < model->GetMeshesCount(); ++k)
						{
							vml::meshes::Mesh3d* mesh = model->GetMeshAt(k);

							std::string meshname = mesh->GetResourceFileName();

							//							std::cout << meshname << " " << objectname << std::endl;

														// find mesh to which attach the linked object

							auto it = FindRow(meshname);

							// if we do not find it , create a new instance of
							// meshname row

							if (it == Map.end())
							{
								MeshDataBaseRow mdbr(meshname);

								mdbr.Items.emplace_back(objectname);

								Map.emplace_back(mdbr);
							}
							else
							{
								// if we find , add the object name and
								// check if its already in the list
								// if its present, then increae the count number

								auto jt = (*it).FindItem(objectname);

								if (jt == (*it).Items.end())
								{
									(*it).Items.emplace_back(objectname);
								}
								else
								{
									(*jt).Count++;
								}
							}

						}
					}
				}

				if (Sort == 0) SortByNames();
				if (Sort == 1) SortByRefs();

				//	std::cout << std::endl;
				//	for (size_t i = 0; i < Map.size(); ++i)
				//	{
				//		std::cout << Map[i].Name << std::endl;
				//		for (size_t j = 0; j < Map[i].Items.size(); ++j)
				//		{
				//			std::cout << "	" << Map[i].Items[j].Name << " ( " << Map[i].Items[j].Count << " )" << std::endl;
				//		}
				//	}

			}

			// ---------------------------------------------------------
			// ctor / dtor

			MeshDataBase()
			{
				Sort = 0;
			}

			~MeshDataBase()
			{
			}
		};
	}
}
