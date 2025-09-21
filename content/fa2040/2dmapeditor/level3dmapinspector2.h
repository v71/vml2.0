#pragma once

namespace EditorsTag
{
	static const unsigned int EDITOR_3D = 1;
}

namespace fa2040
{
	namespace tools
	{
		// panel for level2d inspector

		class Level3dPanel
		{

			private:
				
				// --------------------------------------------------------

				int SelectedNodeItem;
				int SelectedRenderedNodeItem;
				int SelectedNodeItemClicked;
				int SelectedRenderedNodeItemClicked;

				// ---------------------------------------------------------------

				void TextCentered(const std::string& text)
				{
					int windowWidth = ImGui::GetWindowSize().x;
					int textWidth = ImGui::CalcTextSize(text.c_str()).x;
					ImGui::SetCursorPosX((float)(windowWidth - textWidth) / 2);
					ImGui::Text(text.c_str());
				}

			public:

				// ---------------------------------------------------------------

				void Draw(const std::string& title,vml::scenes::Scene* scene)
				{
					bool sceneisvalid = false;
					bool islevelvalid = false;
					bool meshisvalid = false;
					bool navmeshisvalid = false;
					bool colmeshisvalid = false;
					bool octreeisvalid = false;
					
					sceneisvalid = scene != nullptr ? true : false;

					if (sceneisvalid)
						islevelvalid = scene->GetLevel() != nullptr ? true : false;

					if (sceneisvalid && islevelvalid)
					{
						meshisvalid = scene->GetLevel()->GetMapMesh() != nullptr ? true : false;
						navmeshisvalid = scene->GetLevel()->GetNavMesh() != nullptr ? true : false;
						colmeshisvalid = scene->GetLevel()->GetCollisionMesh() != nullptr ? true : false;
						if (scene->GetLevel()->GetOctTree())
							octreeisvalid = scene->GetLevel()->GetOctTree()->IsValid();
					}

					// init window

					ImGui::Begin(&title[0]);

					// main mesh  

					if (ImGui::CollapsingHeader("Map Mesh"))
					{

						if (meshisvalid)
						{
							// main mesh  

							vml::meshes::Mesh3d* mesh = scene->GetLevel()->GetMapMesh();

							unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

							if (ImGui::BeginTable("table_0", 2, flags))
							{
								std::string tmp;
								const std::string& filename = mesh->GetResourceFileName();

								ImGui::TableSetupColumn("Attributes");
								ImGui::TableSetupColumn("Value");
								ImGui::TableHeadersRow();

								ImGui::TableNextColumn();
								ImGui::Text("FileName");
								ImGui::TableNextColumn();
								ImGui::Text(&filename[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Vertices");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)mesh->GetVertexCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Triangles");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)mesh->GetSurfaceCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Indices");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)mesh->GetIndicesCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMin");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMin().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMin().y) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMin().z);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMax");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMax().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMax().y) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMax().z);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxExtents");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetExtents().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetExtents().y) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetExtents().z);
								ImGui::Text(&tmp[0]);

								ImGui::EndTable();

							}
						}
						else
						{
							TextCentered("Mesh has not been compiled yet");
						}

					}

					// main mesh  

					if (ImGui::CollapsingHeader("Nav Mesh"))
					{

						if (navmeshisvalid)
						{
							// main mesh  

							vml::meshes::Mesh3d* mesh = scene->GetLevel()->GetNavMesh();

							unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

							if (ImGui::BeginTable("table_1", 2, flags))
							{
								std::string tmp;
								const std::string& filename = mesh->GetResourceFileName();

								ImGui::TableSetupColumn("Attributes");
								ImGui::TableSetupColumn("Value");
								ImGui::TableHeadersRow();

								ImGui::TableNextColumn();
								ImGui::Text("FileName");
								ImGui::TableNextColumn();
								ImGui::Text(&filename[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Vertices");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)mesh->GetVertexCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Triangles");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)mesh->GetSurfaceCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Indices");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)mesh->GetIndicesCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("NavMesh Dimensions");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(scene->GetLevel()->GetPathFinder()->GetWidth()) + " , " +
									  vml::strings::StringConverter::FromFloatToString(scene->GetLevel()->GetPathFinder()->GetHeight());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMin");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMin().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMin().y) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMin().z);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMax");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMax().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMax().y) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMax().z);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxExtents");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetExtents().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetExtents().y) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetExtents().z);
								ImGui::Text(&tmp[0]);

								ImGui::EndTable();

							}
						}
						else
						{
							TextCentered("Nav Mesh has not been compiled yet");
						}

					}

					// collisoon mesh  

					if (ImGui::CollapsingHeader("Collision Mesh"))
					{

						if (navmeshisvalid)
						{
							// main mesh  

							vml::meshes::Mesh3d* mesh = scene->GetLevel()->GetCollisionMesh();

							unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

							if (ImGui::BeginTable("table_2", 2, flags))
							{
								std::string tmp;
								const std::string& filename = mesh->GetResourceFileName();

								ImGui::TableSetupColumn("Attributes");
								ImGui::TableSetupColumn("Value");
								ImGui::TableHeadersRow();

								ImGui::TableNextColumn();
								ImGui::Text("FileName");
								ImGui::TableNextColumn();
								ImGui::Text(&filename[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Vertices");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)mesh->GetVertexCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Triangles");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)mesh->GetSurfaceCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Indices");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)mesh->GetIndicesCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMin");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMin().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMin().y) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMin().z);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMax");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMax().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMax().y) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetMax().z);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxExtents");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetExtents().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetExtents().y) + " , " +
									vml::strings::StringConverter::FromFloatToString(mesh->GetBoundingBox().GetExtents().z);
								ImGui::Text(&tmp[0]);

								ImGui::EndTable();

							}
						}
						else
						{
							TextCentered("Collision Mesh has not been compiled yet");
						}

					}

					// octree
					
					SelectedNodeItemClicked = 0;

					if (ImGui::CollapsingHeader("Octree Nodes"))
					{

						// octree nodes

						if (octreeisvalid)
						{
							vml::octree::OctTree* oct = scene->GetLevel()->GetOctTree();

							float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

							unsigned int flags = ImGuiTableFlags_ScrollX |
								ImGuiTableFlags_ScrollY |
								ImGuiTableFlags_RowBg |
								ImGuiTableFlags_BordersOuter |
								ImGuiTableFlags_BordersV |
								ImGuiTableFlags_Resizable |
								ImGuiTableFlags_Reorderable |
								ImGuiTableFlags_Hideable;

							// When using ScrollX or ScrollY we need to specify a size for our table container!
							// Otherwise by default the table will fit all available space, like a BeginChild() call.

//							ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * (oct->GetNodesCount()));

							ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 24);

							if (ImGui::BeginTable("table_3", 19, flags, outer_size))
							{
								std::string headertitle = "Nodes ( " + vml::strings::StringConverter::FromIntToString((int)oct->GetNodesCount()) + " )";

								// Make the first column not hideable to match our use of TableSetupScrollFreeze()

								ImGui::TableSetupColumn(headertitle.c_str(), ImGuiTableColumnFlags_NoHide); 
								ImGui::TableSetupColumn("Id");
								ImGui::TableSetupColumn("Leaf");
								ImGui::TableSetupColumn("Visible");
								ImGui::TableSetupColumn("Vertices");
								ImGui::TableSetupColumn("Triangles");
								ImGui::TableSetupColumn("Indices");
								ImGui::TableSetupColumn("Bounding Box Min");
								ImGui::TableSetupColumn("Bounding Box Max");
								ImGui::TableSetupColumn("Bounding Box Extents");
								ImGui::TableSetupColumn("Parent Id");
								ImGui::TableSetupColumn("Child[0]");
								ImGui::TableSetupColumn("Child[1]");
								ImGui::TableSetupColumn("Child[2]");
								ImGui::TableSetupColumn("Child[3]");
								ImGui::TableSetupColumn("Child[4]");
								ImGui::TableSetupColumn("Child[5]");
								ImGui::TableSetupColumn("Child[6]");
								ImGui::TableSetupColumn("Child[7]");
								ImGui::TableSetupScrollFreeze(1, 1);
								ImGui::TableHeadersRow();

								std::string tmp;

								for (size_t i = 0; i < oct->GetNodesCount(); ++i)
								{

									const vml::octree::OctTreeNode* node = oct->GetNodeAt(i);
					
									ImGui::PushID((int)i);
									ImGui::TableNextRow();

									for (int column = 0; column < 19; column++)
									{

										// Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
										// Because here we know that:
										// - A) all our columns are contributing the same to row height
										// - B) column 0 is always visible,
										// We only always submit this one column and can skip others.
										// More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().

										if (!ImGui::TableSetColumnIndex(column) && column > 0)
											continue;

										switch (column)
										{
											case 0:

												tmp = vml::strings::StringConverter::FromIntToString((int)i);
												if (ImGui::Selectable(tmp.c_str(), (int)i == SelectedNodeItem, ImGuiSelectableFlags_SpanAllColumns))
												{
													SelectedNodeItem = (int)i;
													SelectedNodeItemClicked = 1;
												}

											break;

											case 1:

												tmp = vml::strings::StringConverter::FromIntToString((int)node->GetId());
												ImGui::Text(&tmp[0]);

											break;

											case 2:

												if (node->IsLeaf()) tmp = "yes";
												else tmp = "no";
												ImGui::Text(&tmp[0]);

											break;

											case 3:

												if (!node->IsOut()) tmp = "yes";
												else tmp = "no";
												ImGui::Text(&tmp[0]);

											break;

											case 4:

												if (!node->IsLeaf()) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetVertexCount());
												ImGui::Text(&tmp[0]);

											break;

											case 5:

												if (!node->IsLeaf()) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetSurfacesCount());
												ImGui::Text(&tmp[0]);

											break;

											case 6:

												if (!node->IsLeaf()) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetIndicesCount());
												ImGui::Text(&tmp[0]);

											break;

											case 7:

											{
												float bminx = node->GetBoundingBox().GetMin().x;
												float bminy = node->GetBoundingBox().GetMin().y;
												float bminz = node->GetBoundingBox().GetMin().z;

												if (bminx >- vml::math::EPSILON && bminx < vml::math::EPSILON) bminx = 0.0f;
												if (bminy > -vml::math::EPSILON && bminy < vml::math::EPSILON) bminy = 0.0f;
												if (bminz > -vml::math::EPSILON && bminz < vml::math::EPSILON) bminz = 0.0f;

											//	vml::os::Trace("id: %d\nbmin :%f %f %f\nbmax: %f %f %f\n", i,
											//		node->GetBoundingBox().GetMin().x, node->GetBoundingBox().GetMin().y, node->GetBoundingBox().GetMin().z,
											//		node->GetBoundingBox().GetMax().x, node->GetBoundingBox().GetMax().y, node->GetBoundingBox().GetMax().z);

												tmp = vml::strings::StringConverter::FromFloatToString(bminx) + " , " +
													  vml::strings::StringConverter::FromFloatToString(bminy) + " , " +
													  vml::strings::StringConverter::FromFloatToString(bminz);
												ImGui::Text(&tmp[0]);

											}

											break;

											case 8:

											{
												float bmaxx = node->GetBoundingBox().GetMax().x;
												float bmaxy = node->GetBoundingBox().GetMax().y;
												float bmaxz = node->GetBoundingBox().GetMax().z;

												if (bmaxx > -vml::math::EPSILON && bmaxx < vml::math::EPSILON) bmaxx = 0.0f;
												if (bmaxy > -vml::math::EPSILON && bmaxy < vml::math::EPSILON) bmaxy = 0.0f;
												if (bmaxz > -vml::math::EPSILON && bmaxz < vml::math::EPSILON) bmaxz = 0.0f;

												//	vml::os::Trace("id: %d\nbmin :%f %f %f\nbmax: %f %f %f\n", i,
												//		node->GetBoundingBox().GetMin().x, node->GetBoundingBox().GetMin().y, node->GetBoundingBox().GetMin().z,
												//		node->GetBoundingBox().GetMax().x, node->GetBoundingBox().GetMax().y, node->GetBoundingBox().GetMax().z);

												tmp = vml::strings::StringConverter::FromFloatToString(bmaxx) + " , " +
													  vml::strings::StringConverter::FromFloatToString(bmaxy) + " , " +
													  vml::strings::StringConverter::FromFloatToString(bmaxz);
												ImGui::Text(&tmp[0]);

											}

											break;

											case 9:

												tmp = vml::strings::StringConverter::FromFloatToString(node->GetBoundingBox().GetExtents().x) + " , " +
													vml::strings::StringConverter::FromFloatToString(node->GetBoundingBox().GetExtents().y) + " , " +
													vml::strings::StringConverter::FromFloatToString(node->GetBoundingBox().GetExtents().z);
												ImGui::Text(&tmp[0]);

											break;

											case 10:

												if (!node->GetParent()) tmp = "Root";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetParent()->GetId());
												ImGui::Text(&tmp[0]);

											break;

											case 11:

												if (node->IsLeaf() || !node->GetChild(0)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(0)->GetId());
												ImGui::Text(&tmp[0]);

											break;

											case 12:

												if (node->IsLeaf() || !node->GetChild(1)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(1)->GetId());
												ImGui::Text(&tmp[0]);

											break;

											case 13:

												if (node->IsLeaf() || !node->GetChild(2)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(2)->GetId());
												ImGui::Text(&tmp[0]);

											break;

											case 14:

												if (node->IsLeaf() || !node->GetChild(3)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(3)->GetId());
												ImGui::Text(&tmp[0]);

											break;

											case 15:

												if (node->IsLeaf() || !node->GetChild(4)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(4)->GetId());
												ImGui::Text(&tmp[0]);

											break;

											case 16:

												if (node->IsLeaf() || !node->GetChild(5)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(5)->GetId());
												ImGui::Text(&tmp[0]);

											break;

											case 17:

												if (node->IsLeaf() || !node->GetChild(6)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(6)->GetId());
												ImGui::Text(&tmp[0]);

											break;

											case 18:

												if (node->IsLeaf() || !node->GetChild(7)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(7)->GetId());
												ImGui::Text(&tmp[0]);

											break;

											default:

											break;

										}

									}

									ImGui::PopID();
								
								}

								// finalzie table

								ImGui::EndTable();
								
							}
							
						}
						else
						{
							TextCentered("OCtree has not been compiled yet");
						}


					}

					SelectedRenderedNodeItemClicked = 0;

					// rendered octree

					if (ImGui::CollapsingHeader("Rendered Octree Node"))
					{

						// octree nodes

						if (octreeisvalid)
						{
							vml::octree::OctTree* oct = scene->GetLevel()->GetOctTree();

							float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

							unsigned int flags = ImGuiTableFlags_ScrollX |
												 ImGuiTableFlags_ScrollY |
												 ImGuiTableFlags_RowBg |
												 ImGuiTableFlags_BordersOuter |
												 ImGuiTableFlags_BordersV |
												 ImGuiTableFlags_Resizable |
												 ImGuiTableFlags_Reorderable |
												 ImGuiTableFlags_Hideable;

							// When using ScrollX or ScrollY we need to specify a size for our table container!
							// Otherwise by default the table will fit all available space, like a BeginChild() call.

				//			ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * (oct->GetRenderedNodesCount()));

							ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 24);

							if (ImGui::BeginTable("table_3", 19, flags, outer_size))
							{
								std::string headertitle = "Rendered Octree Nodes ( " + vml::strings::StringConverter::FromIntToString((int)oct->GetRenderedNodesCount()) + " )";

								ImGui::TableSetupColumn(headertitle.c_str(), ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()
								ImGui::TableSetupColumn("Id");
								ImGui::TableSetupColumn("Leaf");
								ImGui::TableSetupColumn("Visible");
								ImGui::TableSetupColumn("Vertices");
								ImGui::TableSetupColumn("Triangles");
								ImGui::TableSetupColumn("Indices");
								ImGui::TableSetupColumn("Bounding Box Min");
								ImGui::TableSetupColumn("Bounding Box Max");
								ImGui::TableSetupColumn("Bounding Box Extents");
								ImGui::TableSetupColumn("Parent Id");
								ImGui::TableSetupColumn("Child[0]");
								ImGui::TableSetupColumn("Child[1]");
								ImGui::TableSetupColumn("Child[2]");
								ImGui::TableSetupColumn("Child[3]");
								ImGui::TableSetupColumn("Child[4]");
								ImGui::TableSetupColumn("Child[5]");
								ImGui::TableSetupColumn("Child[6]");
								ImGui::TableSetupColumn("Child[7]");
								ImGui::TableSetupScrollFreeze(1, 1);
								ImGui::TableHeadersRow();

								std::string tmp;

								for (size_t i = 0; i < oct->GetRenderedNodesCount(); ++i)
								{

									const vml::octree::OctTreeNode* node = oct->GetRenderedNodeAt(i);

									ImGui::PushID((int)i);
									ImGui::TableNextRow();

									for (int column = 0; column < 19; column++)
									{

										// Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
										// Because here we know that:
										// - A) all our columns are contributing the same to row height
										// - B) column 0 is always visible,
										// We only always submit this one column and can skip others.
										// More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().

										if (!ImGui::TableSetColumnIndex(column) && column > 0)
											continue;

										switch (column)
										{
											case 0:

												tmp = vml::strings::StringConverter::FromIntToString((int)i);
												if (ImGui::Selectable(tmp.c_str(), (int)i == SelectedRenderedNodeItem, ImGuiSelectableFlags_SpanAllColumns))
												{
													SelectedRenderedNodeItem = (int)i;
													SelectedRenderedNodeItemClicked = 1;
												}
												break;

											case 1:

												tmp = vml::strings::StringConverter::FromIntToString((int)node->GetId());
												ImGui::Text(&tmp[0]);

												break;

											case 2:

												if (node->IsLeaf()) tmp = "yes";
												else tmp = "no";
												ImGui::Text(&tmp[0]);

												break;

											case 3:

												if (!node->IsOut()) tmp = "yes";
												else tmp = "no";
												ImGui::Text(&tmp[0]);

												break;

											case 4:

												if (!node->IsLeaf()) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetVertexCount());
												ImGui::Text(&tmp[0]);

												break;

											case 5:

												if (!node->IsLeaf()) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetSurfacesCount());
												ImGui::Text(&tmp[0]);

												break;

											case 6:

												if (!node->IsLeaf()) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetIndicesCount());
												ImGui::Text(&tmp[0]);

												break;

											case 7:

												{
													float bminx = node->GetBoundingBox().GetMin().x;
													float bminy = node->GetBoundingBox().GetMin().y;
													float bminz = node->GetBoundingBox().GetMin().z;

													if (bminx > -vml::math::EPSILON && bminx < vml::math::EPSILON) bminx = 0.0f;
													if (bminy > -vml::math::EPSILON && bminy < vml::math::EPSILON) bminy = 0.0f;
													if (bminz > -vml::math::EPSILON && bminz < vml::math::EPSILON) bminz = 0.0f;

													//	vml::os::Trace("id: %d\nbmin :%f %f %f\nbmax: %f %f %f\n", i,
													//		node->GetBoundingBox().GetMin().x, node->GetBoundingBox().GetMin().y, node->GetBoundingBox().GetMin().z,
													//		node->GetBoundingBox().GetMax().x, node->GetBoundingBox().GetMax().y, node->GetBoundingBox().GetMax().z);

													tmp = vml::strings::StringConverter::FromFloatToString(bminx) + " , " +
															vml::strings::StringConverter::FromFloatToString(bminy) + " , " +
															vml::strings::StringConverter::FromFloatToString(bminz);
													ImGui::Text(&tmp[0]);

												}

											break;

											case 8:

												{
													float bmaxx = node->GetBoundingBox().GetMax().x;
													float bmaxy = node->GetBoundingBox().GetMax().y;
													float bmaxz = node->GetBoundingBox().GetMax().z;

													if (bmaxx > -vml::math::EPSILON && bmaxx < vml::math::EPSILON) bmaxx = 0.0f;
													if (bmaxy > -vml::math::EPSILON && bmaxy < vml::math::EPSILON) bmaxy = 0.0f;
													if (bmaxz > -vml::math::EPSILON && bmaxz < vml::math::EPSILON) bmaxz = 0.0f;

													//	vml::os::Trace("id: %d\nbmin :%f %f %f\nbmax: %f %f %f\n", i,
													//		node->GetBoundingBox().GetMin().x, node->GetBoundingBox().GetMin().y, node->GetBoundingBox().GetMin().z,
													//		node->GetBoundingBox().GetMax().x, node->GetBoundingBox().GetMax().y, node->GetBoundingBox().GetMax().z);

													tmp = vml::strings::StringConverter::FromFloatToString(bmaxx) + " , " +
														vml::strings::StringConverter::FromFloatToString(bmaxy) + " , " +
														vml::strings::StringConverter::FromFloatToString(bmaxz);
													ImGui::Text(&tmp[0]);

												}

												break;

											case 9:

												tmp = vml::strings::StringConverter::FromFloatToString(node->GetBoundingBox().GetExtents().x) + " , " +
													vml::strings::StringConverter::FromFloatToString(node->GetBoundingBox().GetExtents().y) + " , " +
													vml::strings::StringConverter::FromFloatToString(node->GetBoundingBox().GetExtents().z);
												ImGui::Text(&tmp[0]);

												break;

											case 10:

												if (!node->GetParent()) tmp = "Root";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetParent()->GetId());
												ImGui::Text(&tmp[0]);

												break;

											case 11:

												if (node->IsLeaf() || !node->GetChild(0)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(0)->GetId());
												ImGui::Text(&tmp[0]);

												break;

											case 12:

												if (node->IsLeaf() || !node->GetChild(1)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(1)->GetId());
												ImGui::Text(&tmp[0]);

												break;

											case 13:

												if (node->IsLeaf() || !node->GetChild(2)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(2)->GetId());
												ImGui::Text(&tmp[0]);

												break;

											case 14:

												if (node->IsLeaf() || !node->GetChild(3)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(3)->GetId());
												ImGui::Text(&tmp[0]);

												break;

											case 15:

												if (node->IsLeaf() || !node->GetChild(4)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(4)->GetId());
												ImGui::Text(&tmp[0]);

												break;

											case 16:

												if (node->IsLeaf() || !node->GetChild(5)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(5)->GetId());
												ImGui::Text(&tmp[0]);

												break;

											case 17:

												if (node->IsLeaf() || !node->GetChild(6)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(6)->GetId());
												ImGui::Text(&tmp[0]);

												break;

											case 18:

												if (node->IsLeaf() || !node->GetChild(7)) tmp = "-";
												else tmp = vml::strings::StringConverter::FromIntToString((int)node->GetChild(7)->GetId());
												ImGui::Text(&tmp[0]);

												break;

											default:

												break;

											}

									}

									ImGui::PopID();

								}

								// finalzie table

								ImGui::EndTable();

							}

						}
						else
						{
							TextCentered("OCtree has not been compiled yet");
						}

					}
					
					// finalize window

					ImGui::End();
					
				}

				// --------------------------------------------------------------------------
				
				void InitData(vml::scenes::Scene *scene)
				{
				}
			
				// -----------------------------------------------
				//

				vml::objects::Object3d_2* IsMouseOnAnyAABoxObject(vml::views::View* view, vml::scenes::Scene* scene, int window_bminx, int window_bminy) const
				{
					vml::objects::Object3d_2* objminid = nullptr;
					float objtmin = FLT_MAX;

					ImGuiIO& io = ImGui::GetIO();
					float mousex = (float)(io.MousePos.x - window_bminx);
					float mousey = (float)(io.MousePos.y - window_bminy);

					for (int i = 0; i < scene->GetObjectManager()->GetObjectsCount(); ++i)
					{
						vml::objects::Object3d_2* object = scene->GetObjectManager()->GetObjectAt(i);

						float tmin, tmax;

						glm::vec3 p, q;

						if (vml::DebugRender->SphereVsRay(view, object, mousex, mousey, p, q, tmin, tmax) == vml::geo3d::intersections::DO_INTERSECTS)
						{
							if (tmin < objtmin)
							{
								objtmin = tmin;
								objminid = object;
							}
						}
					}

					return objminid;
				}

				// --------------------------------------------------------------------------

				int GetSelectedNodeItem() const { return SelectedNodeItem; }
				int GetSelectedRenderedNodeItem() const { return SelectedRenderedNodeItem; }
				int IsSelectedNodeItemClicked() const { return SelectedNodeItemClicked; }
				int IsSelectedRenderedNodeItemClicked() const { return SelectedRenderedNodeItemClicked; }

				void ResetSelected()
				{
					SelectedNodeItem = -1;
					SelectedNodeItemClicked = 0;
				}

				void ResetRenderedSelected()
				{
					SelectedRenderedNodeItem = -1;
					SelectedRenderedNodeItemClicked = 0;
				}

				// --------------------------------------------------------
				// ctor / dtor

				Level3dPanel()
				{
					SelectedNodeItem = -1;
					SelectedRenderedNodeItem = -1;
					SelectedNodeItemClicked =0;
					SelectedRenderedNodeItemClicked =0;
				}

				~Level3dPanel()
				{}

		};
		
	}
}
