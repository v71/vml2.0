#pragma once

namespace fa2040
{
	namespace tools
	{

		enum ContentsType
		{
			CT_Text,
			CT_Button,
			CT_SmallButton,
			CT_FillButton,
			CT_Selectable,
			CT_SelectableSpanRow
		};

		// enums for triangle layer debuf window

		enum TriangleItemColumnID
		{
			TriangleItemColumnID_TRIANGLE_ID,
			TriangleItemColumnID_TRIANGLE_P0,
			TriangleItemColumnID_TRIANGLE_P1,
			TriangleItemColumnID_TRIANGLE_P2,
			TriangleItemColumnID_TRIANGLE_LINKS,
		};

		// class for storing info about triangle layer debug window

		class TriangleItem
		{
			public:

				int  ID;
				int	 P0, P1, P2;
				char IDString[32] = { 0 };
				char P0String[32] = { 0 };
				char P1String[32] = { 0 };
				char P2String[32] = { 0 };

				TriangleItem(int id, int p0, int p1, int p2) : ID(id), P0(p0), P1(p1), P2(p2)
				{
					sprintf_s(IDString, 32, "%d", id);
					sprintf_s(P0String, 32, "%d", p0);
					sprintf_s(P1String, 32, "%d", p1);
					sprintf_s(P2String, 32, "%d", p2);
				}

				~TriangleItem()
				{}

		};

		// class for storing info about triangle layer debug window

		class BorderItem
		{
			public:

				int  ID;
				int	 P0, P1;
				char IDString[32] = { 0 };
				char P0String[32] = { 0 };
				char P1String[32] = { 0 };

				BorderItem(int id, int p0, int p1) : ID(id), P0(p0), P1(p1)
				{
					sprintf_s(IDString, 32, "%d", id);
					sprintf_s(P0String, 32, "%d", p0);
					sprintf_s(P1String, 32, "%d", p1);
				}

				~BorderItem()
				{}

		};

		// panel for level2d inspector

		class Level2dPanel
		{
			private:

				// ---------------------------------------------------------------

				void TextCentered(const std::string& text)
				{
					int windowWidth = ImGui::GetWindowSize().x;
					int textWidth = ImGui::CalcTextSize(text.c_str()).x;
					ImGui::SetCursorPosX((float)(windowWidth - textWidth) / 2);
					ImGui::Text(text.c_str());
				}

			public:

				bool					  WindowOpen;
				ImVector<int>			  MainTriangleSelection;
				ImVector<int>			  ExternalTriangleSelection;
				ImVector<int>			  BorderSelection;
				std::vector<TriangleItem> MainTriangleItems;
				std::vector<TriangleItem> ExternalTriangleItems;
				std::vector<BorderItem>   BorderItems;

				// ---------------------------------------------------------------

				void Draw(fa2040::tools::Level2dMap* level2dmap)
				{

					if (!WindowOpen)
						return;

					float f = 1.0f / 4.0f;
					ImGuiIO io = ImGui::GetIO();
					float display_width = (float)io.DisplaySize.x;
					float display_height = (float)io.DisplaySize.y;
					float demo_window_pos_x = display_width * (0.5-f*0.5f);
					float demo_window_pos_y = display_height *(0.5-f*0.5f);
					float demo_window_size_x = display_width * f;
					float demo_window_size_y = display_height * f;

					// We specify a default position/size in case there's no data in the .ini file. 
					// Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
					ImGui::SetNextWindowPos(ImVec2(demo_window_pos_x, demo_window_pos_y), ImGuiCond_Appearing);
					ImGui::SetNextWindowSize(ImVec2(demo_window_size_x, demo_window_size_y), ImGuiCond_Appearing);

					ImGui::Begin("2d Level Info", &WindowOpen);

					// texture info 

					if (ImGui::CollapsingHeader("Texture Info"))
					{
						const vml::textures::Texture* tex = level2dmap->GetTexture();

						unsigned int flags = ImGuiTableFlags_Resizable + ImGuiTableFlags_Borders;

						if (ImGui::BeginTable("table1", 2, flags))
						{
							std::string filename = level2dmap->GetFileName();
							std::string mainpath = vml::strings::SplitPath::GetDirectory(filename);
							std::string contentpath = vml::strings::SplitPath::GetFolder(filename);

							ImGui::TableSetupColumn("Attributes");
							ImGui::TableSetupColumn("Value");
							ImGui::TableHeadersRow();

							ImGui::TableNextColumn();
							ImGui::Text("Main Path");
							ImGui::TableNextColumn();
							ImGui::Text(&mainpath[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Content Path");
							ImGui::TableNextColumn();
							ImGui::Text(&contentpath[0]);

							ImGui::TableNextColumn();
							ImGui::Text("FileName");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetFileName()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Width");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetTextureWidthString()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Heigth");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetTextureHeightString()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Aspect Ratio");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetTextureAspectRatioString()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Byte Per Pixel");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetTextureBppString()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("TextureId");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetTextureIdString()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Minification Filter");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetTextureMinificationString()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Magnification Filter");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetTextureMagnificationString()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Format");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetTextureFormatString()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("RepeatS");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetRepeatSString()[0]);

							ImGui::TableNextColumn();
							ImGui::Text("RepeatT");
							ImGui::TableNextColumn();
							ImGui::Text(&level2dmap->GetRepeatTString()[0]);

							ImGui::EndTable();
						}

					}

					// main triangle layer 

					if (ImGui::CollapsingHeader("Main Triangle Layer"))
					{
					
						bool valid = false;

						if (level2dmap->Level2dGenerator)
							if (level2dmap->Level2dGenerator->IsCompiled())
								valid = true;

						if (valid)
						{
							
							fa2040::tools::TriangleLayer& trilayer = level2dmap->Level2dGenerator->TriangleLayer;

							unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

							if (ImGui::BeginTable("table_0", 2, flags))
							{
								std::string tmp;

								ImGui::TableSetupColumn("Attributes");
								ImGui::TableSetupColumn("Value");
								ImGui::TableHeadersRow();

								ImGui::TableNextColumn();
								ImGui::Text("Vertices");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)trilayer.GetVerticesCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Triangles");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)trilayer.GetTrianglesCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMin");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxMin().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxMin().y);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMax");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxMax().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxMax().y);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxExtents");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxExtents().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxExtents().y);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBox Aspect Ratio");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxExtents().y / trilayer.GetBoundingBoxExtents().x);
								ImGui::Text(&tmp[0]);

								ImGui::EndTable();
							}

							// Using those as a base value to create width/height that are factor of the size of our font

							float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
							float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
							ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
							ImGuiTableFlags mflags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
								ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
								ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

							bool outer_size_enabled = true;
							float inner_width_with_scroll = 0.0f;
							float inner_width_to_use = (mflags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
							float row_min_height = 0;
							ImVec2 outer_size_value = ImVec2(0.0f, TEXT_BASE_HEIGHT * 12);

							// main triangle layer

							//	if (ImGui::CollapsingHeader("Indexed Triangle List"))
							
							if (ImGui::BeginTable("table_advanced_0", 4, mflags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
							{

								// Declare columns

								ImGui::TableSetupColumn("Triangle ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_ID);
								ImGui::TableSetupColumn("P0", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P0);
								ImGui::TableSetupColumn("P1", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P1);
								ImGui::TableSetupColumn("P2", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P2);

								ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
								ImGui::TableHeadersRow();
								int contents_type = CT_SelectableSpanRow;
								ImGuiSelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap : ImGuiSelectableFlags_None;

								// Demonstrate using clipper for large vertical lists

								ImGuiListClipper clipper;

								clipper.Begin((int)MainTriangleItems.size());

								while (clipper.Step())
								{
									for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
									{

										TriangleItem* item = &MainTriangleItems[row_n];

										ImGui::PushID(item->ID);
										ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

										ImGui::TableSetColumnIndex(0);
										//sprintf_s(label, 32, "%d", item->TriangleID);

										const bool item_is_selected = MainTriangleSelection.contains(item->ID);

										if (ImGui::Selectable(item->IDString, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
										{
											if (ImGui::GetIO().KeyCtrl)
											{
												if (item_is_selected)
													MainTriangleSelection.find_erase_unsorted(item->ID);
												else
													MainTriangleSelection.push_back(item->ID);
											}
											else
											{
												MainTriangleSelection.clear();
												MainTriangleSelection.push_back(item->ID);
											}
										}

										ImGui::TableSetColumnIndex(1);
										ImGui::TextUnformatted(item->P0String);

										ImGui::TableSetColumnIndex(2);
										ImGui::TextUnformatted(item->P1String);

										ImGui::TableSetColumnIndex(3);
										ImGui::TextUnformatted(item->P2String);

										ImGui::PopID();

									}
								}

								ImGui::EndTable();
							}
							
						}
						else
						{
							if (!level2dmap->Level2dGenerator)
								TextCentered("Map has not been loaded");
							else if (!level2dmap->Level2dGenerator->IsCompiled())
								TextCentered("Map has not been compiled");
						}
					}

					// external triangle layer

					if (ImGui::CollapsingHeader("External Triangle Layer"))
					{
						bool valid = false;
						if (level2dmap->Level2dGenerator)
							if (level2dmap->Level2dGenerator->IsCompiled())
								valid = true;
						
						if (valid)
						{
							
							fa2040::tools::TriangleLayer& trilayer = level2dmap->Level2dGenerator->ExternalTriangleLayer2;

							unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

							if (ImGui::BeginTable("table_1", 2, flags))
							{
								std::string tmp;

								ImGui::TableSetupColumn("Attributes");
								ImGui::TableSetupColumn("Value");
								ImGui::TableHeadersRow();

								ImGui::TableNextColumn();
								ImGui::Text("Vertices");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)trilayer.GetVerticesCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Triangles");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)trilayer.GetTrianglesCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMin");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxMin().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxMin().y);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMax");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxMax().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxMax().y);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxExtents");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxExtents().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxExtents().y);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBox Aspect Ratio");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(trilayer.GetBoundingBoxExtents().y / trilayer.GetBoundingBoxExtents().x);
								ImGui::Text(&tmp[0]);

								ImGui::EndTable();
							}

							// Using those as a base value to create width/height that are factor of the size of our font

							float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
							float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
							ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
							ImGuiTableFlags mflags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
								ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
								ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

							bool outer_size_enabled = true;
							float inner_width_with_scroll = 0.0f;
							float inner_width_to_use = (mflags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
							float row_min_height = 0;
							ImVec2 outer_size_value = ImVec2(0.0f, TEXT_BASE_HEIGHT * 12);

							// main triangle layer

							if (ImGui::BeginTable("table_advanced_1", 4, mflags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
							{

								// Declare columns

								ImGui::TableSetupColumn("Triangle ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_ID);
								ImGui::TableSetupColumn("P0", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P0);
								ImGui::TableSetupColumn("P1", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P1);
								ImGui::TableSetupColumn("P2", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P2);

								ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
								ImGui::TableHeadersRow();
								int contents_type = CT_SelectableSpanRow;
								ImGuiSelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap : ImGuiSelectableFlags_None;

								// Demonstrate using clipper for large vertical lists

								ImGuiListClipper clipper;

								clipper.Begin((int)ExternalTriangleItems.size());

								while (clipper.Step())
								{
									for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
									{

										TriangleItem* item = &ExternalTriangleItems[row_n];

										ImGui::PushID(item->ID);
										ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

										ImGui::TableSetColumnIndex(0);

										const bool item_is_selected = ExternalTriangleSelection.contains(item->ID);

										if (ImGui::Selectable(item->IDString, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
										{
											if (ImGui::GetIO().KeyCtrl)
											{
												if (item_is_selected)
													ExternalTriangleSelection.find_erase_unsorted(item->ID);
												else
													ExternalTriangleSelection.push_back(item->ID);
											}
											else
											{
												ExternalTriangleSelection.clear();
												ExternalTriangleSelection.push_back(item->ID);
											}
										}

										ImGui::TableSetColumnIndex(1);
										ImGui::TextUnformatted(item->P0String);

										ImGui::TableSetColumnIndex(2);
										ImGui::TextUnformatted(item->P1String);

										ImGui::TableSetColumnIndex(3);
										ImGui::TextUnformatted(item->P2String);

										ImGui::PopID();

									}
								}

								ImGui::EndTable();
							}
						}
						else
						{
							if (!level2dmap->Level2dGenerator)
								TextCentered("Map has not been loaded");
							else if (!level2dmap->Level2dGenerator->IsCompiled())
								TextCentered("Map has not been compiled");
					
						}
					
					}

					// border triangle layer

					if (ImGui::CollapsingHeader("Border Triangle Layer"))
					{
						bool valid = false;
						if (level2dmap->Level2dGenerator)
							if (level2dmap->Level2dGenerator->IsCompiled())
								valid = true;

						if (valid)
						{

							fa2040::tools::BorderLayer& borderlayer = level2dmap->Level2dGenerator->BorderLayer;

							unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

							if (ImGui::BeginTable("table_2", 2, flags))
							{

								std::string tmp;

								ImGui::TableSetupColumn("Attributes");
								ImGui::TableSetupColumn("Value");
								ImGui::TableHeadersRow();

								ImGui::TableNextColumn();
								ImGui::Text("Vertices");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)borderlayer.GetVerticesCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("Edges");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromIntToString((int)borderlayer.GetEdgesCount());
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMin");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(borderlayer.GetBoundingBoxMin().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(borderlayer.GetBoundingBoxMin().y);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxMax");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(borderlayer.GetBoundingBoxMax().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(borderlayer.GetBoundingBoxMax().y);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBoxExtents");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(borderlayer.GetBoundingBoxExtents().x) + " , " +
									vml::strings::StringConverter::FromFloatToString(borderlayer.GetBoundingBoxExtents().y);
								ImGui::Text(&tmp[0]);

								ImGui::TableNextColumn();
								ImGui::Text("BoundingBox Aspect Ratio");
								ImGui::TableNextColumn();
								tmp = vml::strings::StringConverter::FromFloatToString(borderlayer.GetBoundingBoxExtents().y / borderlayer.GetBoundingBoxExtents().x);
								ImGui::Text(&tmp[0]);

								ImGui::EndTable();

							}

							// Using those as a base value to create width/height that are factor of the size of our font

							float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
							float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
							ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
							ImGuiTableFlags mflags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
								ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
								ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

							bool outer_size_enabled = true;
							float inner_width_with_scroll = 0.0f;
							float inner_width_to_use = (mflags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
							float row_min_height = 0;
							ImVec2 outer_size_value = ImVec2(0.0f, TEXT_BASE_HEIGHT * 12);

							// main triangle layer

							if (ImGui::BeginTable("table_advanced_2", 3, mflags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
							{

								// Declare columns

								ImGui::TableSetupColumn("Edge ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_ID);
								ImGui::TableSetupColumn("P0", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P0);
								ImGui::TableSetupColumn("P1", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P1);

								ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
								ImGui::TableHeadersRow();
								int contents_type = CT_SelectableSpanRow;
								ImGuiSelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap : ImGuiSelectableFlags_None;

								// Demonstrate using clipper for large vertical lists

								ImGuiListClipper clipper;

								clipper.Begin((int)BorderItems.size());

								while (clipper.Step())
								{
									for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
									{

										BorderItem* item = &BorderItems[row_n];

										ImGui::PushID(item->ID);
										ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

										ImGui::TableSetColumnIndex(0);

										const bool item_is_selected = BorderSelection.contains(item->ID);

										if (ImGui::Selectable(item->IDString, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
										{
											if (ImGui::GetIO().KeyCtrl)
											{
												if (item_is_selected)
													BorderSelection.find_erase_unsorted(item->ID);
												else
													BorderSelection.push_back(item->ID);
											}
											else
											{
												BorderSelection.clear();
												BorderSelection.push_back(item->ID);
											}
										}

										ImGui::TableSetColumnIndex(1);
										ImGui::TextUnformatted(item->P0String);

										ImGui::TableSetColumnIndex(2);
										ImGui::TextUnformatted(item->P1String);

										ImGui::PopID();

									}
								}
								ImGui::EndTable();
							}
						}
						else
						{
							if (!level2dmap->Level2dGenerator)
								TextCentered("Map has not been loaded");
							else if (!level2dmap->Level2dGenerator->IsCompiled())
								TextCentered("Map has not been compiled");
						}
					}

					ImGui::End();

				}

				// --------------------------------------------------------------------------

				void InitData(fa2040::tools::Level2dMap* level2dmap)
				{

					// cache maintriangle layer

					fa2040::tools::TriangleLayer& maintrilayer = level2dmap->Level2dGenerator->TriangleLayer;

					MainTriangleItems.clear();

					for (int i = 0; i < maintrilayer.GetTrianglesCount(); ++i)
					{
						fa2040::tools::GridTriangle tri = maintrilayer.GetTriangleAt(i);
						MainTriangleItems.emplace_back(TriangleItem(i, tri.GetI0(), tri.GetI1(), tri.GetI2()));
					}

					// cache external triangle layer

					fa2040::tools::TriangleLayer& externaltrilayer = level2dmap->Level2dGenerator->ExternalTriangleLayer2;

					ExternalTriangleItems.clear();

					for (int i = 0; i < externaltrilayer.GetTrianglesCount(); ++i)
					{
						fa2040::tools::GridTriangle tri = externaltrilayer.GetTriangleAt(i);
						ExternalTriangleItems.emplace_back(TriangleItem(i, tri.GetI0(), tri.GetI1(), tri.GetI2()));
					}

					// cache border layer

					fa2040::tools::BorderLayer& borderlayer = level2dmap->Level2dGenerator->BorderLayer;

					BorderItems.clear();

					for (int i = 0; i < borderlayer.GetEdgesCount(); ++i)
					{
						BorderItems.emplace_back(BorderItem(i, borderlayer.GetI0(i), borderlayer.GetI1(i)));
					}
				}

			// --------------------------------------------------------
			// ctor / dtor

			Level2dPanel()
			{
				WindowOpen = false;
			}

			~Level2dPanel()
			{}

		};
	}
}
