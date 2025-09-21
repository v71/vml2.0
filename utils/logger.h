#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2020 v71 
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

///////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
		class Logger
		{
			private:

				// --------------------------------------------------------------------
				// data struct for loggin from memory

				struct SingleDebugLine
				{
					std::vector<std::string> SingleLine;
				};

				// --------------------------------------------------------------------

				struct DebugLinesStruct
				{
					std::vector<SingleDebugLine> Lines;
			
					DebugLinesStruct()
					{
					}

					DebugLinesStruct(const SingleDebugLine &line)
					{
						Lines.emplace_back(line);
					}
				};

				// --------------------------------------------------------------------

			private:

				std::vector<std::string>	  Columns;										// Number of columns
				std::vector<std::string>	  Strings;										// Store strings to be logged out
				std::basic_stringstream<char> Stream;										// File stream 
				std::string					  FileName;										// string holding log's filename
				unsigned int				  Flags;										// flags
				std::ofstream				  FileStream;									// file stream
				int							  BackGroundColor[3]  = {  76 , 175 ,  80 };	// Html background color
				int							  BorderColor[3]	  = {   0 ,   0 ,   0 };	// Html border color
				int							  ItemColor[3]		  = { 242 , 242 , 242 };	// Html item colors
				int							  ItemColorHovered[3] = { 220 , 220 , 220 };	// html item color when hovered
				bool						  HtmlFile;										// flag to check if file passed as log is html or plain text

				// ---------------------------------------------
				// singleton design pattern
				// A singlton is a class instantiated only once.
				// Usually it is globally accessiblae,
				// constructor and destructors are kept private to the 
				// final user. User has the responsability to delete
				// memory associated with the class explicitly at
				// main entry point closure

				static Logger* Singleton;

				// ---------------------------------------------
				// private ctor / dtor

				Logger()
				{

					// sets stream precision to 4 decimals

					Stream.precision(4);

					// inits flags

					FileName = "";
					Flags = 0;
					HtmlFile=false;

				}

				// --------------------------------------------------------------------

				bool operator==(const Logger& other) const
				{
					return false;
				}

				// --------------------------------------------------------------------
				// The base case: we just have a single element

				template <typename T>
				void Print(T t)
				{
					Stream.clear();
					Stream.str(std::string());
					Stream << t;
					Strings.emplace_back(Stream.str());
				}

				// --------------------------------------------------------------------

				template <typename T, typename... Rest>
				void Print(T t, Rest... rest)
				{
					Stream.clear();
					Stream.str(std::string());
					Stream << t;
					Strings.emplace_back(Stream.str());
					Print(rest...);
				}

				// --------------------------------------------------------------------
				// Check whether file has html or txt extension

				bool ValidateOutputFile(const std::string &filename)
				{
				//	std::cout << "filename : " << filename << std::endl;
					std::string ext;
					// count number of '.' in the filename
					size_t count = std::count_if(filename.begin(), filename.end(), [](char c) {return c == '.'; });
					if (count > 1) {
					//	std::cout << "number of dots : " << count << std::endl;
						return false;	}
					size_t dot = filename.rfind(".");
					// no dot or malformed
					if (dot == std::string::npos || dot == 0) {
					//	std::cout << "malformed" << std::endl;
						return false;	}
					// get extension
					ext = filename.substr(dot + 1, (filename.size() - dot - 1));
					if (ext.size() == 0) {
					//	std::cout << "no extension provied" << std::endl;
						return false;	}
					// convert extension into lower case if necessary
					// and check for extension , accepted are 'html' or 'txt' 
					// else we have an error
					std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
					if (ext == "html") HtmlFile = true;
					else  if (ext == "mem") HtmlFile = false;
						else return false;
					//std::cout << "externsion : " << ext << std::endl;
					//std::cout << "dot position : " << dot << std::endl;
					//std::cout << "number of dots : " << count << std::endl;
					return true;
				}

				//---------------------------------------------------------------------

				const static uint32 INITTED	 = vml::bits32::BIT0;
				const static size_t MAXWIDTH = 25;

			public:

				DebugLinesStruct			  DebugLines;

				//---------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Logger(Logger& other) = delete;
				
				//---------------------------------------------------------------------
				// overload operator is private, 
				// no copies allowed since classes
				// are referenced

				void operator=(const Logger&) = delete;

				//---------------------------------------------------------------------
				// get instance of singleton , if singleton is null , which is mandatory at
				// application startup, it will be allocated only once
				// Eventually, to reallocate , user must call the delete member function

				static Logger* GetInstance()
				{
					if (Singleton == nullptr) 
						Singleton = new Logger();
					return Singleton;
				}

				// -----------------------------------------------------------------------------------
				// gets current date using std time lib

				const std::string GetCurrentDate()
				{
					std::time_t l_ttime;
					std::tm l_curTime;
					time(&l_ttime);
					if (localtime_s(&l_curTime, &l_ttime))
						vml::os::Message::Error("Logger : ","Cannot convert date string for file : ' ", FileName.c_str()," '");
					char l_cStart[128] = { 0 };
					strftime(l_cStart, 128, "%c", &l_curTime);
					return l_cStart;
				}

				// --------------------------------------------------------------------
				// Prints out a single line to log

				template <typename T, typename... Rest>
				void Out(T t, Rest... rest)
				{
					if (!vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
						vml::os::Message::Error("Logger : ","Logger not initialized(call Init function)");

					Strings.clear();

					// recursively obtain strings 

					Print(t, rest...);

					// validate number of strings

					size_t strsize = Strings.size();
					size_t colsize = Columns.size();

					if (strsize > colsize )
						vml::os::Message::Error("Logger : ","Number of Columns mismatch for file ' ", FileName.c_str()," '");
					if (strsize <= 0)
						vml::os::Message::Error("Logger : ","Number of Columns can't be zero for file ' ", FileName.c_str(), " '");

					// print out htlm text to stream
				
					if (HtmlFile)
					{
						std::string text = "<tr>";

						for (size_t i = 0; i < strsize; ++i)
						{
							text += "<td>";
							text += Strings[i];
							text += "</td>";
						}

						// print out unused field(s)

						for (size_t i = 0; i < colsize - strsize; ++i)
						{
							text += "<td></td>";
						}

						text += "</tr>";

						FileStream << text;

					}
					else
					{

						SingleDebugLine sdl;

						for (size_t i = 0; i < strsize; ++i)
						{
					//		std::cout << Strings[i] << " ";
							sdl.SingleLine.emplace_back(Strings[i]);
						}

						DebugLines.Lines.emplace_back(sdl);

					}

				}

				// -----------------------------------------------------------
				// getters

				bool			   IsInitted()	 const { return vml::bits32::Get(Flags, INITTED); }		// is logger initted ?
				unsigned int	   GetFlags()	 const { return Flags; }								// get logger flags ( see Internal flags enum class 
				const std::string& GetFileName() const { return FileName; }								// returns logger filename
				bool			   IsHtmlFile()	 const { return HtmlFile; }								// returns if file is an html file or not

				// ------------------------------------------------------------
				// initialise debug logger

				void Init(const std::string& filename)
				{
					if (vml::bits32::Get(Flags, INITTED))
						vml::os::Message::Error("Logger : ","Logger already initted, delete and restart");
					if (filename.empty())
						vml::os::Message::Error("Logger : ","FileName is empty");
					Flags     = 0;
					FileName  = filename;
					HtmlFile  = false;
					if (!ValidateOutputFile(FileName))
						vml::os::Message::Error("Logger : ","Unknown Outpu file extension must be either html or mem");
					if (HtmlFile) {
						FileStream.open(FileName.c_str(), std::ios::out);
						if (!FileStream.is_open())
							vml::os::Message::Error("Logger : ","Cannot open file ' ", FileName.c_str()," '");
					}
					vml::bits32::SetToTrue(Flags, INITTED);
					Columns.clear();
					DebugLines.Lines.clear();
				}

				// ------------------------------------------------------------

				void AddColumn(const std::string& name)
				{
					if (!vml::bits32::Get(Flags, INITTED))
						vml::os::Message::Error("Logger : ","Logger not initted");
					Columns.emplace_back(name);
				}

				// ------------------------------------------------------------

				std::string RGBToHex(const int c[3]) const
				{
					if (!vml::bits32::Get(Flags, INITTED))
						vml::os::Message::Error("Logger : ", "Logger not initted");

					if (c[0] == 0 && c[1] == 0 && c[2] == 0) return "#000";
					std::stringstream ss;
					ss << "#" << std::hex << (c[0] << 16 | c[1] << 8 | c[2]);
					return ss.str();
				}

				// ------------------------------------------------------------
				// closes stream

				void Close()
				{
					if (!vml::bits32::Get(Flags, INITTED))
						vml::os::Message::Error("Logger : ", "Logger not initted");

					vml::bits32::SetToFalse(Flags, INITTED);

					// close file stream

					if (HtmlFile)
						FileStream.close();

					// delete singleton

					vml::SafeDelete(Singleton);
				}

				// ------------------------------------------------------------
				// Html page initialisation

				void Preamble()
				{
					if (HtmlFile)
					{
						std::string text;
						std::string backgroundcolor  = RGBToHex(BackGroundColor);
						std::string bordercolor      = RGBToHex(BorderColor);
						std::string itemcolor        = RGBToHex(ItemColor);
						std::string itemhoveredcolor = RGBToHex(ItemColorHovered);

						text = "<!DOCTYPE html><html>";
						text += "<head><style>";
						text += "#customers{";
						text += "font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;";
						text += "border-collapse: collapse;";
						text += "width: 100%;}";
						text += "#customers td, #customers th{";
						text += "border: 1px solid " + bordercolor + ";";
						text += "text-align: center;";
						text += "width: " + vml::strings::StringConverter::FromIntToString(100 / (int)Columns.size()) + "%;";
						text += "padding: 8px;}";
						text += "#customers tr:nth-child(even){ background-color: " + itemcolor + "; }";
						text += "#customers tr:hover{ background-color: " + itemhoveredcolor + "; }";
						text += "#customers th{";
						text += "padding-top: 12px;";
						text += "padding-bottom: 12px;";
						text += "text-align: center;";
						text += "background-color: " + backgroundcolor + ";";
						text += "color: white;}";
						text += "</style></head><body>";
						text += "<table id=\"customers\">";
						text += "<tr>";
						
						for (size_t i = 0; i < Columns.size(); ++i)
							text += "<th>" + Columns[i] + "</th>";
						text += "</tr>";

						FileStream << text;

					}
					else
					{
						SingleDebugLine sdl;

						for (size_t i = 0; i < Columns.size(); ++i)
							sdl.SingleLine.emplace_back(Columns[i]);

						DebugLines.Lines.emplace_back(sdl);
					}

				}

				// ------------------------------------------------------------
				// Html page finalization

				void Epilogue()
				{
					if (HtmlFile)
						FileStream << "</table></body></html>";
					else
					{
						// if file is not an html file do nothing
					}
				}

				// -------------------------------------------------------------
				// public destrctuor

				~Logger()
				{
				}

		};

		// init singleton pointer initially set to null
		// this goes into a .cpp file

		//		Logger *Logger::Singleton=nullptr;

} // end of vml namespace

