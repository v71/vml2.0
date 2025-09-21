#pragma once

namespace EditorsTag
{
	static const unsigned int DEBUG_WND = 3;
}

class DebugLogWindow
{
	private:

		// -----------------------------------------------------------------------------------

		class MessageType
		{
			public:

				int			ID;
				int			Type;
				std::string ClassString;
				std::string TimeStampString;
				std::string ActionString;
				std::string ResultString;

				static const unsigned int ALL = 0;

				static const unsigned int CORE		   = 1;
				static const unsigned int DEBUG_RENDER = 2;
				static const unsigned int LEVEL		   = 3;
				static const unsigned int MANAGERS	   = 4;
				static const unsigned int OPENGL	   = 5;
				static const unsigned int PATHS		   = 6;
				static const unsigned int SYSINFO	   = 7;
				static const unsigned int SESSION      = 8;
				static const unsigned int STORES       = 9;
				static const unsigned int SCENE        = 10;
				static const unsigned int SHADER       = 11;
				static const unsigned int TEXTURE      = 12;


				MessageType(const int id,const int type, const std::string timestamp, const std::string& classstring, const std::string& actionstring, const std::string& resultstring)
				{
					ID              = id;
					Type            = type;
					TimeStampString = timestamp;
					ClassString     = classstring;
					ActionString    = actionstring;
					ResultString    = resultstring;
				}

				~MessageType()
				{
				}

		};

		// -----------------------------------------------------------------------------------
		// gets current date using std time lib

		const std::string GetCurrentDate()
		{
			std::time_t l_ttime;
			std::tm l_curTime;
			time(&l_ttime);
			if (localtime_s(&l_curTime, &l_ttime))
				vml::os::Message::Error("GUI Logger : ","Cannot convert date string");
			char l_cStart[128] = { 0 };
			strftime(l_cStart, 128, "%c", &l_curTime);
			return l_cStart;
		}

		// -----------------------------------------------------------------------------------

		bool					 Added;
		bool		             AutoScroll;
		std::vector<MessageType> Messages;
		std::vector<MessageType> FilteredMessages;
		std::string				 ComboItemString[1024];
		bool					 ComboButton[1024];
		int						 ComboStringCount;
		bool					 AllSelected;

	public:

		// -----------------------------------------------------------------------------------

		void Clear()
		{
			Messages.clear();
			FilteredMessages.clear();
		}

		// -----------------------------------------------------------------------------------
		
		void AddLine(const std::string& timestampstring, const std::string& classstring, const std::string &actionstring, const std::string &resultstring)
		{
			int type=-1;
			
			if (classstring == "Core")		      type = MessageType::CORE;
			if (classstring == "Debug Render")    type = MessageType::DEBUG_RENDER;
			if (classstring == "Level")		      type = MessageType::LEVEL;
			if (classstring == "Object Manager")  type = MessageType::MANAGERS;
			if (classstring == "OpenGl Context")  type = MessageType::OPENGL;
			if (classstring == "Main Path" ||
				classstring == "ShaderStore Path" || 
			    classstring == "TextureStore Path" || 
				classstring == "MeshStore Path" ) type = MessageType::PATHS;
			if (classstring == "SysInfo")         type = MessageType::SYSINFO;
			if (classstring == "Session")         type = MessageType::SESSION;
			if (classstring == "ShaderStore" ||
				classstring == "TextureStore" ||
				classstring == "MeshStore")       type = MessageType::STORES;
			if (classstring == "Scene")           type = MessageType::SCENE;
			if (classstring == "Shader")          type = MessageType::SHADER;
			if (classstring == "Texture")         type = MessageType::TEXTURE;

		//	std::cout << classstring << std::endl;

			Messages.emplace_back(MessageType((int)Messages.size(),type, timestampstring, classstring, actionstring, resultstring));
			Added = 1;
		}
		
		// -----------------------------------------------------------------------------------

		void ComposeMessages()
		{
			FilteredMessages.clear();
			
			if (AllSelected)
			{
				for (size_t i = 0; i < Messages.size(); ++i)
					FilteredMessages.emplace_back(Messages[i]);
			}
			else
			{
				for (size_t i = 0; i < Messages.size(); ++i)
				{
					if (ComboButton[Messages[i].Type])
						FilteredMessages.emplace_back(Messages[i]);
				}
			}
		}

		// -----------------------------------------------------------------------------------
		// Main window

		void Draw()
		{

			// use font if given

			float fx = 1.0f / 1.5f;
			float fy = 1.0f / 4.0f;
			ImGuiIO io = ImGui::GetIO();
			float display_width = (float)io.DisplaySize.x;
			float display_height = (float)io.DisplaySize.y;
			float demo_window_pos_x = display_width * (0.5 - fx * 0.5f);
			float demo_window_pos_y = display_height * (0.5 - fy * 0.5f);
			float demo_window_size_x = display_width * fx;
			float demo_window_size_y = display_height * fy;

			// We specify a default position/size in case there's no data in the .ini file. 
			// Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
			ImGui::SetNextWindowPos(ImVec2(demo_window_pos_x, demo_window_pos_y), ImGuiCond_Appearing);
			ImGui::SetNextWindowSize(ImVec2(demo_window_size_x, demo_window_size_y), ImGuiCond_Appearing);

	//		ImGui::Begin("DebugLog", &WindowOpen);

			ImGuiStyle* style = &ImGui::GetStyle();

			ImGui::Text("Filter :");
			ImGui::SameLine();

			static const char* current_item = ComboItemString[0].c_str();
			static size_t current_item_id = 0;

			const float width = ImGui::GetWindowWidth();
			const float combo_width = width * 0.12f;
			ImGui::SetNextItemWidth(combo_width);

			if (ImGui::BeginCombo("##combo", current_item))
			{
				for (size_t i = 0; i < ComboStringCount; ++i)
					if (ImGui::MenuItem(ComboItemString[i].c_str(), "", &ComboButton[i])) { Added = 1; current_item = ComboItemString[i].c_str(); current_item_id = i; }
				ImGui::EndCombo();
			}

			// filter handling

			if (current_item_id == 0)
			{
				for (size_t i = 0; i < ComboStringCount; ++i)
					ComboButton[i] = 0;
				ComboButton[0] = 1;
				AllSelected = 1;
			}
			else
			{
				for (size_t i = 0; i < ComboStringCount; ++i)
					ComboButton[i] = 0;
				ComboButton[current_item_id] = 1;
				AllSelected = 0;
			}

			// Using those as a base value to create width/height that are factor of the size of our font

			ImGuiTableFlags mflags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
									 ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
									 ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

			float inner_width_with_scroll = 0.0f;
			float inner_width_to_use = (mflags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
			float row_min_height = 0;

			if (ImGui::BeginTable("table_0", 4, mflags, ImVec2(0, 0), inner_width_to_use))
			{

				// Declare columns

				ImGui::TableSetupColumn("TimeStamp", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 0);
				ImGui::TableSetupColumn("Class", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 1);
				ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 2);
				ImGui::TableSetupColumn("Result", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 3);
				ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible

				//	ImGui::TableHeadersRow();

				// Instead of calling TableHeadersRow() we'll submit custom headers ourselves

				ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

				for (int column = 0; column < 4; column++)
				{
					ImGui::TableSetColumnIndex(column);
					const char* column_name = ImGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
					float column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(0) - ImGui::CalcTextSize(column_name).x) * 0.5f;
					if (column1_x > ImGui::GetCursorPosX())
						ImGui::SetCursorPosX(column1_x);
					ImGui::TableHeader(column_name);
				}

				//

				static int item_current_idx = -1;
				float column1_x = 0.0f;
				ImVec4 color = ImColor{ IM_COL32(0,0,0,255) };;
				ImGuiListClipper clipper;

				if (Added)
				{
					ComposeMessages();
					Added = 0;
				}

				clipper.Begin((int)FilteredMessages.size());

				while (clipper.Step())
				{

					for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
					{

						MessageType* item = &FilteredMessages[i];

						color = ImColor{ IM_COL32(255,255,255,255) };  
		
						ImGui::PushID(item->ID);

						ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);
						
						// timestamp 

						ImGui::TableSetColumnIndex(0);
						column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(0) - ImGui::CalcTextSize(item->TimeStampString.c_str()).x) * 0.5f;
						if (column1_x > ImGui::GetCursorPosX())
							ImGui::SetCursorPosX(column1_x);
						ImGui::PushStyleColor(ImGuiCol_Text, color);
						if (ImGui::Selectable(item->TimeStampString.c_str(), (item_current_idx == i), ImGuiSelectableFlags_SpanAllColumns))
							item_current_idx = (int)i;
						ImGui::PopStyleColor();
						
						// class

						ImGui::TableSetColumnIndex(1);
						column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(1) - ImGui::CalcTextSize(item->ClassString.c_str()).x) * 0.5f;
						if (column1_x > ImGui::GetCursorPosX())
							ImGui::SetCursorPosX(column1_x);
						ImGui::PushStyleColor(ImGuiCol_Text, color);
						ImGui::TextUnformatted(item->ClassString.c_str());
						ImGui::PopStyleColor();
						
						// action

						ImGui::TableSetColumnIndex(2);
						column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(1) - ImGui::CalcTextSize(item->ActionString.c_str()).x) * 0.5f;
						if (column1_x > ImGui::GetCursorPosX())
							ImGui::SetCursorPosX(column1_x);
						ImGui::PushStyleColor(ImGuiCol_Text, color);
						ImGui::TextUnformatted(item->ActionString.c_str());
						ImGui::PopStyleColor();
						
						// result

						ImGui::TableSetColumnIndex(3);
						column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(1) - ImGui::CalcTextSize(item->ResultString.c_str()).x) * 0.5f;
						if (column1_x > ImGui::GetCursorPosX())
							ImGui::SetCursorPosX(column1_x);
						ImGui::PushStyleColor(ImGuiCol_Text, color);
						ImGui::TextUnformatted(item->ResultString.c_str());
						ImGui::PopStyleColor();

						ImGui::PopID();
					}
				}

				// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
				// Using a scrollbar or mouse-wheel will take away from the bottom edge.

				if (AutoScroll)
				{
					ImGui::SetScrollHereY(1.0f);
				}

				clipper.End();

				ImGui::EndTable();

			}

		//	ImGui::End();

		}

		// ------------------------------------------------------------------------------

		void FillLog()
		{
			//	std::cout << "loading log.mem" << std::endl;

			Clear();

			// start from 1 becasue line 0 contains columns

			for (size_t i = 1; i < vml::Logger::GetInstance()->DebugLines.Lines.size(); ++i)
			{

				if (vml::Logger::GetInstance()->DebugLines.Lines[i].SingleLine.size() == 3)
					AddLine(vml::Logger::GetInstance()->DebugLines.Lines[i].SingleLine[0],
						vml::Logger::GetInstance()->DebugLines.Lines[i].SingleLine[1],
						vml::Logger::GetInstance()->DebugLines.Lines[i].SingleLine[2],
						"-");

				if (vml::Logger::GetInstance()->DebugLines.Lines[i].SingleLine.size() == 4)
					AddLine(vml::Logger::GetInstance()->DebugLines.Lines[i].SingleLine[0],
						vml::Logger::GetInstance()->DebugLines.Lines[i].SingleLine[1],
						vml::Logger::GetInstance()->DebugLines.Lines[i].SingleLine[2],
						vml::Logger::GetInstance()->DebugLines.Lines[i].SingleLine[3]);
			}
		}

		// -----------------------------------------------------------------------------------

		DebugLogWindow()
		{
			AutoScroll = 0;
			Added = 0;
			ComboStringCount = 0;
			AllSelected=0;

			ComboItemString[ComboStringCount++] = "All";
			ComboItemString[ComboStringCount++] = "Core";
			ComboItemString[ComboStringCount++] = "Debug Render";
			ComboItemString[ComboStringCount++] = "Level";
			ComboItemString[ComboStringCount++] = "Managers";
			ComboItemString[ComboStringCount++] = "OpenGl Context";
			ComboItemString[ComboStringCount++] = "Paths";
			ComboItemString[ComboStringCount++] = "SysInfo";
			ComboItemString[ComboStringCount++] = "Session";
			ComboItemString[ComboStringCount++] = "Stores";
			ComboItemString[ComboStringCount++] = "Scene";
			ComboItemString[ComboStringCount++] = "Shaders";
			ComboItemString[ComboStringCount++] = "Textures";

			for (size_t i = 0; i < ComboStringCount; ++i)
				ComboButton[i] = 0;
			ComboButton[0] = 1;
		}

		~DebugLogWindow()
		{
		}

};

