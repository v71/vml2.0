#pragma once

namespace EditorsTag
{
	static const unsigned int MESSAGE_WND = 2;
}

class MessageLogWindow
{
	private:

		// -----------------------------------------------------------------------------------

		class MessageType
		{
			public:

				int			ID;
				int			Type;
				std::string IDString;
				std::string TimeStamp;
				std::string Message;
				std::string Severity;

				static const unsigned int MSG_ERROR	  = 0;
				static const unsigned int MSG_WARNING = 1;
				static const unsigned int MSG_SUCCESS = 2;

				MessageType(int id, const int type, const std::string timestamp, const std::string& message, const std::string& severity)
				{
					ID = id;
					Type = type;
					TimeStamp = timestamp;
					Message = message;
					Severity = severity;
					IDString = "[ " + std::to_string(ID) + " ]";
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

		bool AllSelected;

	public:

		bool		             AutoScroll;
		bool					 Added;
		std::vector<MessageType> Messages;
		std::vector<MessageType> FilteredMessages;

		// -----------------------------------------------------------------------------------

		void Clear()
		{
			Messages.clear();
			FilteredMessages.clear();
		}

		// -----------------------------------------------------------------------------------

		void AddSuccessMessage(const char* format, ...)
		{
			va_list args;
			int len;
			char* buffer;
			va_start(args, format);
			len = _vscprintf(format, args) + 1;					// _vscprintf doesn't count terminating '\0'
			if (len == 0) throw;
			buffer = (char*)malloc(len * sizeof(char));
			if (!buffer) throw;
			vsprintf_s(buffer, len, format, args);
			Messages.emplace_back(MessageType((int)Messages.size(), MessageType::MSG_SUCCESS, GetCurrentDate().c_str(), buffer, "Ok"));
			Added = 1;
			free(buffer);
			va_end(args);
		}

		// -----------------------------------------------------------------------------------

		void AddWarningMessage(const char* format, ...)
		{
			va_list args;
			int len;
			char* buffer;
			va_start(args, format);
			len = _vscprintf(format, args) + 1;					// _vscprintf doesn't count terminating '\0'
			if (len == 0) throw;
			buffer = (char*)malloc(len * sizeof(char));
			if (!buffer) throw;
			vsprintf_s(buffer, len, format, args);
			Messages.emplace_back(MessageType((int)Messages.size(), MessageType::MSG_WARNING, GetCurrentDate().c_str(), buffer, "Warning"));
			Added = 1;
			free(buffer);
			va_end(args);
		}

		// -----------------------------------------------------------------------------------

		void AddErrorMessage(const char* format, ...)
		{
			va_list args;
			int len;
			char* buffer;
			va_start(args, format);
			len = _vscprintf(format, args) + 1;					// _vscprintf doesn't count terminating '\0'
			if (len == 0) throw;
			buffer = (char*)malloc(len * sizeof(char));
			if (!buffer) throw;
			vsprintf_s(buffer, len, format, args);
			Messages.emplace_back(MessageType((int)Messages.size(), MessageType::MSG_ERROR, GetCurrentDate().c_str(), buffer, "Error"));
			Added = 1;
			free(buffer);
			va_end(args);
		}

		// -----------------------------------------------------------------------------------

		void AddSuccessMessage(const std::string& message)
		{
			Messages.emplace_back(MessageType((int)Messages.size(), MessageType::MSG_SUCCESS, GetCurrentDate().c_str(), message, "Ok"));
			Added = 1;
		}

		// -----------------------------------------------------------------------------------

		void AddWarningMessage(const std::string& message)
		{
			Messages.emplace_back(MessageType((int)Messages.size(), MessageType::MSG_WARNING, GetCurrentDate().c_str(), message, "Warning"));
			Added = 1;
		}

		// -----------------------------------------------------------------------------------

		void AddErrorMessage(const std::string& message)
		{
			Messages.emplace_back(MessageType((int)Messages.size(), MessageType::MSG_ERROR, GetCurrentDate().c_str(), message, "Error"));
			Added = 1;
		}

		// -----------------------------------------------------------------------------------

		void ComposeMessages(const bool addall,const bool addok, const bool addwarning, const bool adderror)
		{
			FilteredMessages.clear();

			if (AllSelected)
			{
				for (size_t i = 0; i < Messages.size(); ++i)
				{
					if (Messages[i].Type == MessageType::MSG_SUCCESS) FilteredMessages.emplace_back(Messages[i]);
					if (Messages[i].Type == MessageType::MSG_WARNING) FilteredMessages.emplace_back(Messages[i]);
					if (Messages[i].Type == MessageType::MSG_ERROR)   FilteredMessages.emplace_back(Messages[i]);
				}
			}

			for (size_t i = 0; i < Messages.size(); ++i)
			{
				if (addok && Messages[i].Type == MessageType::MSG_SUCCESS) FilteredMessages.emplace_back(Messages[i]);
				if (addwarning && Messages[i].Type == MessageType::MSG_WARNING) FilteredMessages.emplace_back(Messages[i]);
				if (adderror && Messages[i].Type == MessageType::MSG_ERROR)   FilteredMessages.emplace_back(Messages[i]);
			}
		}

		// -----------------------------------------------------------------------------------
		// Main window

		void Draw()
		{

			static bool ok_button_pressed = true;
			static bool warning_button_pressed = true;
			static bool bug_button_pressed = true;
			static bool all_button_pressed = true;
			static int current_item_id = 0;

			float fx = 1.0f / 2.0f;
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
			//ImGui::SetNextWindowPos(ImVec2(demo_window_pos_x, demo_window_pos_y), ImGuiCond_Appearing);
			//ImGui::SetNextWindowSize(ImVec2(demo_window_size_x, demo_window_size_y), ImGuiCond_Appearing);

		//	ImGui::Begin(&title[0], &WindowOpen);

			// use font if given

			ImGuiStyle* style = &ImGui::GetStyle();

			ImGui::Text("Filter :");
			ImGui::SameLine();

			const char* ComboItemString[] = { "All","Finalized Messages", "Warning Messages", "Error Messages" };
			static const char* current_item = ComboItemString[0];

			const float width = ImGui::GetWindowWidth();
			const float combo_width = width * 0.12f;
			ImGui::SetNextItemWidth(combo_width);

			if (ImGui::BeginCombo("##combo", current_item))
			{
				if (ImGui::MenuItem(ComboItemString[0], "", &all_button_pressed)) { Added = 1; current_item = ComboItemString[0]; current_item_id = 0; }
				if (ImGui::MenuItem(ComboItemString[1], "", &ok_button_pressed)) { Added = 1; current_item = ComboItemString[1]; current_item_id = 1; }
				if (ImGui::MenuItem(ComboItemString[2], "", &warning_button_pressed)) { Added = 1; current_item = ComboItemString[2]; current_item_id = 2; }
				if (ImGui::MenuItem(ComboItemString[3], "", &bug_button_pressed)) { Added = 1; current_item = ComboItemString[3]; current_item_id = 3; }
				ImGui::EndCombo();
			}

			ImGui::SameLine();

			if (ImGui::Button("Clean"))
				Clear();
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("Clear Messages Log");

			// pop font back

			if (current_item_id == 0)
			{
				ok_button_pressed = 0;
				warning_button_pressed = 0;
				bug_button_pressed	= 0;
				all_button_pressed = 1;
				AllSelected = 1;
			}
			else
			{
				if (current_item_id == 1) { warning_button_pressed = 0; bug_button_pressed = 0; }
				if (current_item_id == 2) { bug_button_pressed = 0; ok_button_pressed = 0; }
				if (current_item_id == 3) { warning_button_pressed = 0; ok_button_pressed = 0; }

				all_button_pressed = 0;
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

				ImGui::TableSetupColumn("Message Id", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 0);
				ImGui::TableSetupColumn("Time Stamp", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 1);
				ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 2);
				ImGui::TableSetupColumn("Severity", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 3);
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
				int maxvalue = 255;
				ImGuiListClipper clipper;

				if (Added)
				{
					ComposeMessages(all_button_pressed,ok_button_pressed, warning_button_pressed, bug_button_pressed);
					Added = 0;
				}

				clipper.Begin((int)FilteredMessages.size());

				while (clipper.Step())
				{

					for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
					{

						MessageType* item = &FilteredMessages[i];

						switch (item->Type)
						{
							case MessageType::MSG_ERROR:	color = ImColor{ IM_COL32(0,maxvalue,maxvalue,255) };   break;
							case MessageType::MSG_SUCCESS:	color = ImColor{ IM_COL32(0,maxvalue,0,255) };   break;
							case MessageType::MSG_WARNING:	color = ImColor{ IM_COL32(maxvalue,maxvalue,0,255) }; break;
						}

						ImGui::PushID(item->ID);
						ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

						// message id

						ImGui::TableSetColumnIndex(0);
						column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(0) - ImGui::CalcTextSize(item->IDString.c_str()).x) * 0.5f;
						if (column1_x > ImGui::GetCursorPosX())
							ImGui::SetCursorPosX(column1_x);
						ImGui::PushStyleColor(ImGuiCol_Text, color);
						if (ImGui::Selectable(item->IDString.c_str(), (item_current_idx == i), ImGuiSelectableFlags_SpanAllColumns))
							item_current_idx = (int)i;
						ImGui::PopStyleColor();

						// timestamp

						ImGui::TableSetColumnIndex(1);
						column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(1) - ImGui::CalcTextSize(item->TimeStamp.c_str()).x) * 0.5f;
						if (column1_x > ImGui::GetCursorPosX())
							ImGui::SetCursorPosX(column1_x);
						ImGui::PushStyleColor(ImGuiCol_Text, color);
						ImGui::TextUnformatted(item->TimeStamp.c_str());
						ImGui::PopStyleColor();

						// message

						ImGui::TableSetColumnIndex(2);

						column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(1) - ImGui::CalcTextSize(item->Message.c_str()).x) * 0.5f;
						if (column1_x > ImGui::GetCursorPosX())
							ImGui::SetCursorPosX(column1_x);
						ImGui::PushStyleColor(ImGuiCol_Text, color);
						ImGui::TextUnformatted(item->Message.c_str());
						ImGui::PopStyleColor();

						// severuty

						ImGui::TableSetColumnIndex(3);
						column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(1) - ImGui::CalcTextSize(item->Severity.c_str()).x) * 0.5f;
						if (column1_x > ImGui::GetCursorPosX())
							ImGui::SetCursorPosX(column1_x);
						ImGui::PushStyleColor(ImGuiCol_Text, color);
						ImGui::TextUnformatted(item->Severity.c_str());
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

		// -----------------------------------------------------------------------------------

		MessageLogWindow()
		{
			AutoScroll = 0;
			AllSelected=0;
			Added = 0;
		}

		~MessageLogWindow()
		{
		}

};

