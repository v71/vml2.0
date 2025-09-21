#pragma once

#include<string>
#include<vector>
#include <filesystem>

namespace ImGuiFileDialog
{

	enum ImGuiFileDialogType
	{
		ImGuiFileDialogType_OpenFile,
		ImGuiFileDialogType_SaveFile
	};

	enum ImGuiFileDialogSortOrder
	{
		ImGuiFileDialogSortOrder_Up,
		ImGuiFileDialogSortOrder_Down,
		ImGuiFileDialogSortOrder_None
	};

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //

	class ImFileDialogInfo
	{
		public:

			std::string									  Title;
			ImGuiFileDialogType							  Type;
			std::filesystem::path						  FileName;
			std::filesystem::path						  DirectoryPath;
			std::filesystem::path						  ResultPath;
			bool										  RefreshInfo;
			int											  CurrentIndex;
			std::vector<std::filesystem::directory_entry> CurrentFiles;
			std::vector<std::filesystem::directory_entry> CurrentDirectories;

			// ---------------------------------------------------------------------

			void UpdateInfo()
			{
				RefreshInfo = false;
				CurrentIndex = 0;
				CurrentFiles.clear();
				CurrentDirectories.clear();

				for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(DirectoryPath))
				{
					if (entry.is_directory())
					{
						CurrentDirectories.emplace_back(entry);
					}
					else
					{
						CurrentFiles.emplace_back(entry);
					}
				}
			}

			// ------------------------------------------------
			// ctor / dtor

			ImFileDialogInfo()
			{
				Type = ImGuiFileDialogType_OpenFile;
				RefreshInfo = false;
				CurrentIndex = -1;
			}

			~ImFileDialogInfo()
			{
			}

	};

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //

	class ImGuiFileDialog
	{
		private:

		public:

			// ------------------------------------------------------------------------
			//

			bool ShowDialog(bool* open, ImFileDialogInfo* dialogInfo)
			{
               
                static float initialSpacingColumn0 = 230.0f;
				static float initialSpacingColumn1 = 80.0f;
				static float initialSpacingColumn2 = 90.0f;
				static ImGuiFileDialogSortOrder fileNameSortOrder = ImGuiFileDialogSortOrder_None;
				static ImGuiFileDialogSortOrder sizeSortOrder = ImGuiFileDialogSortOrder_None;
				static ImGuiFileDialogSortOrder dateSortOrder = ImGuiFileDialogSortOrder_None;
				static ImGuiFileDialogSortOrder typeSortOrder = ImGuiFileDialogSortOrder_None;
                static float windowW = 740.0f;
                static float windowH = 500.0f;
                static float buttonSpacing = 5.0f;

				assert(dialogInfo != nullptr);

				bool complete = false;
                                
				ImGui::PushID(dialogInfo);
				ImGui::SetNextWindowSize(ImVec2(windowW, windowH), ImGuiCond_FirstUseEver);

				if (ImGui::Begin(dialogInfo->Title.c_str(), nullptr, ImGuiWindowFlags_NoResize |
                                                                     ImGuiWindowFlags_NoDocking |
                                                                     ImGuiWindowFlags_NoCollapse))
				{
                    
					if (dialogInfo->CurrentFiles.empty() && dialogInfo->CurrentDirectories.empty() || dialogInfo->RefreshInfo)
					{
						dialogInfo->UpdateInfo();
					}

					// Draw path

					ImGui::Text("Path: %s", dialogInfo->DirectoryPath.string().c_str());
                    
					ImGui::BeginChild("##browser", ImVec2(ImGui::GetWindowContentRegionWidth(), 300), true, ImGuiWindowFlags_HorizontalScrollbar);
					ImGui::Columns(4);

					// Columns size
					if (initialSpacingColumn0 > 0)
					{
						ImGui::SetColumnWidth(0, initialSpacingColumn0);
						initialSpacingColumn0 = 0.0f;
					}
					if (initialSpacingColumn1 > 0)
					{
						ImGui::SetColumnWidth(1, initialSpacingColumn1);
						initialSpacingColumn1 = 0.0f;
					}
					if (initialSpacingColumn2 > 0)
					{
						ImGui::SetColumnWidth(2, initialSpacingColumn2);
						initialSpacingColumn2 = 0.0f;
					}
                    
					// File Columns
					if (ImGui::Selectable("Name"))
					{
						sizeSortOrder = ImGuiFileDialogSortOrder_None;
						dateSortOrder = ImGuiFileDialogSortOrder_None;
						typeSortOrder = ImGuiFileDialogSortOrder_None;
						fileNameSortOrder = fileNameSortOrder == ImGuiFileDialogSortOrder_Down ? ImGuiFileDialogSortOrder_Up : ImGuiFileDialogSortOrder_Down;
					}
					ImGui::NextColumn();
					if (ImGui::Selectable("Size"))
					{
						fileNameSortOrder = ImGuiFileDialogSortOrder_None;
						dateSortOrder = ImGuiFileDialogSortOrder_None;
						typeSortOrder = ImGuiFileDialogSortOrder_None;
						sizeSortOrder = sizeSortOrder == ImGuiFileDialogSortOrder_Down ? ImGuiFileDialogSortOrder_Up : ImGuiFileDialogSortOrder_Down;
					}
					ImGui::NextColumn();
					if (ImGui::Selectable("Type"))
					{
						fileNameSortOrder = ImGuiFileDialogSortOrder_None;
						dateSortOrder = ImGuiFileDialogSortOrder_None;
						sizeSortOrder = ImGuiFileDialogSortOrder_None;
						typeSortOrder = typeSortOrder == ImGuiFileDialogSortOrder_Down ? ImGuiFileDialogSortOrder_Up : ImGuiFileDialogSortOrder_Down;
					}
					ImGui::NextColumn();
					if (ImGui::Selectable("Date"))
					{
						fileNameSortOrder = ImGuiFileDialogSortOrder_None;
						sizeSortOrder = ImGuiFileDialogSortOrder_None;
						typeSortOrder = ImGuiFileDialogSortOrder_None;
						dateSortOrder = dateSortOrder == ImGuiFileDialogSortOrder_Down ? ImGuiFileDialogSortOrder_Up : ImGuiFileDialogSortOrder_Down;
					}
					ImGui::NextColumn();
                    
					// File Separator
					ImGui::Separator();

					// Sort directories
					auto* directories = &dialogInfo->CurrentDirectories;

					if (fileNameSortOrder != ImGuiFileDialogSortOrder_None || sizeSortOrder != ImGuiFileDialogSortOrder_None || typeSortOrder != ImGuiFileDialogSortOrder_None)
					{
						std::sort(directories->begin(), directories->end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b)
							{
								if (fileNameSortOrder == ImGuiFileDialogSortOrder_Down)
								{
									return a.path().filename() > b.path().filename();
								}

								return a.path().filename() < b.path().filename();
							});
					}
					else if (dateSortOrder != ImGuiFileDialogSortOrder_None)
					{
						std::sort(directories->begin(), directories->end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b)
							{
								if (dateSortOrder == ImGuiFileDialogSortOrder_Down)
								{
									return a.last_write_time() > b.last_write_time();
								}
								return a.last_write_time() < b.last_write_time();
							});
					}

					// Sort files
					auto* files = &dialogInfo->CurrentFiles;

					if (fileNameSortOrder != ImGuiFileDialogSortOrder_None)
					{
						std::sort(files->begin(), files->end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b)
							{
								if (fileNameSortOrder == ImGuiFileDialogSortOrder_Down)
								{
									return a.path().filename() > b.path().filename();
								}

								return a.path().filename() < b.path().filename();
							});
					}
					else if (sizeSortOrder != ImGuiFileDialogSortOrder_None)
					{
						std::sort(files->begin(), files->end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b)
							{
								if (sizeSortOrder == ImGuiFileDialogSortOrder_Down)
								{
									return a.file_size() > b.file_size();
								}
								return a.file_size() < b.file_size();
							});
					}
					else if (typeSortOrder != ImGuiFileDialogSortOrder_None)
					{
						std::sort(files->begin(), files->end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b)
							{
								if (typeSortOrder == ImGuiFileDialogSortOrder_Down)
								{
									return a.path().extension() > b.path().extension();
								}

								return a.path().extension() < b.path().extension();
							});
					}
					else if (dateSortOrder != ImGuiFileDialogSortOrder_None)
					{
						std::sort(files->begin(), files->end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b)
							{
								if (dateSortOrder == ImGuiFileDialogSortOrder_Down)
								{
									return a.last_write_time() > b.last_write_time();
								}
								return a.last_write_time() < b.last_write_time();
							});
					}

					size_t index = 0;
                    
                    // Draw parent
					if (dialogInfo->DirectoryPath.has_parent_path())
					{
						if (ImGui::Selectable("..", dialogInfo->CurrentIndex == index, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
						{
							dialogInfo->CurrentIndex = (int)index;

							if (ImGui::IsMouseDoubleClicked(0))
							{
								dialogInfo->DirectoryPath = dialogInfo->DirectoryPath.parent_path();
								dialogInfo->RefreshInfo = true;
							}
						}

						ImGui::NextColumn();
						ImGui::TextUnformatted("-");
						ImGui::NextColumn();
						ImGui::TextUnformatted("<parent>");
						ImGui::NextColumn();
						ImGui::TextUnformatted("-");
						ImGui::NextColumn();

						index++;
					}
                    
					// Draw directories
					for (size_t i = 0; i < directories->size(); ++i)
					{
						std::filesystem::directory_entry& directoryEntry = dialogInfo->CurrentDirectories[i];
						const std::filesystem::path& directoryPath = directoryEntry.path();
						auto directoryName = directoryPath.filename();

						if (ImGui::Selectable(directoryName.string().c_str(), dialogInfo->CurrentIndex == index, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
						{
							dialogInfo->CurrentIndex = (int)index;

							if (ImGui::IsMouseDoubleClicked(0))
							{
								dialogInfo->DirectoryPath = directoryPath;
								dialogInfo->RefreshInfo = true;
							}
						}

						ImGui::NextColumn();
						ImGui::TextUnformatted("-");
						ImGui::NextColumn();
						ImGui::TextUnformatted("<directory>");
						ImGui::NextColumn();

						auto lastWriteTime = directoryEntry.last_write_time();
						auto st = std::chrono::time_point_cast<std::chrono::system_clock::duration>(lastWriteTime - decltype(lastWriteTime)::clock::now() + std::chrono::system_clock::now());
						std::time_t tt = std::chrono::system_clock::to_time_t(st);
						std::tm mt;
						localtime_s(&mt, &tt);
						std::stringstream ss;
						ss << std::put_time(&mt, "%F %R");
						ImGui::TextUnformatted(ss.str().c_str());
						ImGui::NextColumn();

						index++;
					}
                    
					// Draw files
					for (size_t i = 0; i < files->size(); ++i)
					{
                        std::filesystem::directory_entry &fileEntry = dialogInfo->CurrentFiles[i];
                        std::filesystem::path filePath = fileEntry.path();
                        std::filesystem::path fileName = filePath.filename();

						if (ImGui::Selectable(fileName.string().c_str(), dialogInfo->CurrentIndex == index, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
						{
							dialogInfo->CurrentIndex = (int)index;
							dialogInfo->FileName = fileName;
						}

						ImGui::NextColumn();
						ImGui::TextUnformatted(std::to_string(fileEntry.file_size()).c_str());
						ImGui::NextColumn();
						ImGui::TextUnformatted(filePath.extension().string().c_str());
						ImGui::NextColumn();

						auto lastWriteTime = fileEntry.last_write_time();
						auto st = std::chrono::time_point_cast<std::chrono::system_clock::duration>(lastWriteTime - decltype(lastWriteTime)::clock::now() + std::chrono::system_clock::now());
						std::time_t tt = std::chrono::system_clock::to_time_t(st);
						std::tm mt;
						localtime_s(&mt, &tt);
						std::stringstream ss;
						ss << std::put_time(&mt, "%F %R");
						ImGui::TextUnformatted(ss.str().c_str());
						ImGui::NextColumn();

						index++;
					}
                    
					ImGui::EndChild();
                    
					// Draw filename
					static const size_t fileNameBufferSize = 200;
					static char fileNameBuffer[fileNameBufferSize];

					std::string fileNameStr = dialogInfo->FileName.string();
					size_t fileNameSize = fileNameStr.size();

					if (fileNameSize >= fileNameBufferSize)	fileNameSize = fileNameBufferSize - 1;
					std::memcpy(fileNameBuffer, fileNameStr.c_str(), fileNameSize);
					fileNameBuffer[fileNameSize] = 0;

					ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
                    ImGui::Dummy(ImVec2(0.0f, buttonSpacing));
                  
                    ImGui::Text("FileName");
                    ImGui::SameLine();

                    if (ImGui::InputText("##FileName", fileNameBuffer, fileNameBufferSize))
					{
						dialogInfo->FileName = std::string(fileNameBuffer);
						dialogInfo->CurrentIndex = 0;
					}

                    ImGui::Dummy(ImVec2(0.0f, buttonSpacing));

                    if (dialogInfo->Type == ImGuiFileDialogType_OpenFile)
					{
						if (ImGui::Button("Open"))
						{
							dialogInfo->ResultPath = dialogInfo->DirectoryPath / dialogInfo->FileName;

							if (std::filesystem::exists(dialogInfo->ResultPath))
							{
								fileNameSortOrder = ImGuiFileDialogSortOrder_None;
								sizeSortOrder = ImGuiFileDialogSortOrder_None;
								typeSortOrder = ImGuiFileDialogSortOrder_None;
								dateSortOrder = ImGuiFileDialogSortOrder_None;

								dialogInfo->RefreshInfo = false;
								dialogInfo->CurrentIndex = 0;
								dialogInfo->CurrentFiles.clear();
								dialogInfo->CurrentDirectories.clear();

								complete = true;
								*open = false;
							}
						}

                        ImGui::SameLine();

                        if (ImGui::Button("Cancel"))
                        {
                            fileNameSortOrder = ImGuiFileDialogSortOrder_None;
                            sizeSortOrder = ImGuiFileDialogSortOrder_None;
                            typeSortOrder = ImGuiFileDialogSortOrder_None;
                            dateSortOrder = ImGuiFileDialogSortOrder_None;

                            dialogInfo->RefreshInfo = false;
                            dialogInfo->CurrentIndex = 0;
                            dialogInfo->CurrentFiles.clear();
                            dialogInfo->CurrentDirectories.clear();

                            *open = false;
                        }

                    }
					else if (dialogInfo->Type == ImGuiFileDialogType_SaveFile)
					{
						if (ImGui::Button("Save"))
						{
							dialogInfo->ResultPath = dialogInfo->DirectoryPath / dialogInfo->FileName;

							if (!std::filesystem::exists(dialogInfo->ResultPath))
							{
								fileNameSortOrder = ImGuiFileDialogSortOrder_None;
								sizeSortOrder = ImGuiFileDialogSortOrder_None;
								typeSortOrder = ImGuiFileDialogSortOrder_None;
								dateSortOrder = ImGuiFileDialogSortOrder_None;

								dialogInfo->RefreshInfo = false;
								dialogInfo->CurrentIndex = 0;
								dialogInfo->CurrentFiles.clear();
								dialogInfo->CurrentDirectories.clear();

								complete = true;
								*open = false;
							}
						}

                        ImGui::SameLine();

                        if (ImGui::Button("Cancel"))
                        {
                            fileNameSortOrder = ImGuiFileDialogSortOrder_None;
                            sizeSortOrder = ImGuiFileDialogSortOrder_None;
                            typeSortOrder = ImGuiFileDialogSortOrder_None;
                            dateSortOrder = ImGuiFileDialogSortOrder_None;

                            dialogInfo->RefreshInfo = false;
                            dialogInfo->CurrentIndex = 0;
                            dialogInfo->CurrentFiles.clear();
                            dialogInfo->CurrentDirectories.clear();

                            *open = false;
                        }

                    }

                   ImGui::PopItemWidth();

				}

                ImGui::End();
				ImGui::PopID();
                
				return complete;

			}

			// -----------------------------------------------------
			// ctor / dtor

			ImGuiFileDialog()
			{}

			~ImGuiFileDialog()
			{}

	};
}
