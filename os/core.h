#pragma once

////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	
	class Core
	{

		private:

			// -----------------------------------------------------------------------------
			// system bit flags

			uint32 InternalFlags;
			uint32 PreferencesFlags;

			// -------------------------------------------------------------------------------

		public:

			// -------------------------------------------------------------------------------
			// public classes

			vml::os::Timer Timer;

			//-----------------------------------------------------------------------------------
			// copy constructor is private
			// no copies allowed 

			Core(Core& main) = delete;

			//-----------------------------------------------------------------------------------
			// overload operator is private,
			// no copies allowed since classes
			// are referenced

			void operator=(const Core& main) = delete;

			// -----------------------------------------------------------------------------
			// getters

			bool IsInitialized()   const { return vml::bits32::Get(InternalFlags,vml::InternalFlags::INITTED); }
			bool IsVerbose()       const { return vml::bits32::Get(PreferencesFlags,vml::PreferencesFlags::VERBOSE); }
			bool IsQuiet()         const { return vml::bits32::Get(PreferencesFlags,vml::PreferencesFlags::QUIET); }
			bool ArePathsInitted() const { return vml::bits32::Get(InternalFlags,vml::InternalFlags::PATHSINITTED); }
			bool IsDebugLogHtml()  const { return vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::HTMLLOG); }
			bool IsDebugLogMem()   const { return vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::MEMLOG); }

			// -----------------------------------------------------------------------------
			//

			void Init(unsigned int preferencesflags)
			{
				
				if (vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("Core : ","Cannot restart context before it has not been closed");
				
				// set preferences flags

				PreferencesFlags = preferencesflags;

				if (IsVerbose() && IsQuiet())
					vml::os::Message::Error("Core : ", "Cannot set both verbose and quiet flags\n");

				// validate preferences flags

				if ( IsVerbose() && IsQuiet() )
					vml::os::Message::Error("Core : ", "Cannot set both verbose and quiet flags");

				// set internal system flags

				vml::bits32::SetToFalse(InternalFlags, vml::InternalFlags::INITTED);
				
				// init logger

				std::string logfilename;

				if (IsDebugLogHtml())
					logfilename = "log.html";

				if (IsDebugLogMem())
					logfilename = "log.mem";

				if (!IsDebugLogHtml() && !IsDebugLogMem())
					logfilename = "log.html";

				vml::Logger::GetInstance()->Init(logfilename);
				
				// add columns

				vml::Logger::GetInstance()->AddColumn("TimeStamp");
				vml::Logger::GetInstance()->AddColumn("Class");
				vml::Logger::GetInstance()->AddColumn("Action");
				vml::Logger::GetInstance()->AddColumn("Result");
			
				// ceate preamble

				vml::Logger::GetInstance()->Preamble();

				//start measuring

				std::string currentdate = vml::Logger::GetInstance()->GetCurrentDate();

				vml::Logger::GetInstance()->Out(currentdate, "Core", "Initting core system", "In progress...");

				if (IsVerbose())
					vml::Logger::GetInstance()->Out(currentdate, "Core", "Verbose flag set to ", "True");
				
				if (IsQuiet())
					vml::Logger::GetInstance()->Out(currentdate, "Core", "Verbose flag set to ", "False");

				// init timer
				vml::Logger::GetInstance()->Out(currentdate, "Core", "Initting performance timer", "In progress...");

				Timer.Init();

				vml::Logger::GetInstance()->Out(currentdate, "Core", "Initting performance timer", "Done");

				// initting system info 

				vml::os::SystemInfo SystemInfo;

				SystemInfo.Measure();
				
				vml::Logger::GetInstance()->Out(currentdate, "Core"   , "Retrieving system info", "In progress...");
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Vendor : "                      + SystemInfo.GetCpuVendorString());
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "CodeName : "                    + SystemInfo.GetCpuCodeNameString());
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Brand : "                       + SystemInfo.GetCpuBrandString());
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "VendorId : "                    + vml::strings::StringConverter::FromIntToString(SystemInfo.GetVendorId()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "CodeNameId : "                  + vml::strings::StringConverter::FromIntToString(SystemInfo.GetCodeNameId()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Stepping : "                    + vml::strings::StringConverter::FromIntToString(SystemInfo.GetStepping()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Family : "                      + vml::strings::StringConverter::FromIntToString(SystemInfo.GetFamily()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Model : "                       + vml::strings::StringConverter::FromIntToString(SystemInfo.GetModel()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "CPUMaxFunction : "              + vml::strings::StringConverter::FromIntToString(SystemInfo.GetCPUIdMaxFunction()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Processr Type : "               + vml::strings::StringConverter::FromIntToString(SystemInfo.GetProcessorType()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Extended Model : "              + vml::strings::StringConverter::FromIntToString(SystemInfo.GetExtendedModel()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Extended Family : "             + vml::strings::StringConverter::FromIntToString(SystemInfo.GetExtendedFamily()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Brand Index : "                 + vml::strings::StringConverter::FromIntToString(SystemInfo.GetBrandIndex()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "CLFLUSH Cache line size : "     + vml::strings::StringConverter::FromIntToString(SystemInfo.GetCLFLUSHCacheLineSize()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "API Physical ID : "             + vml::strings::StringConverter::FromIntToString(SystemInfo.GetAPICPhysicalID()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Cache line size : "             + vml::strings::StringConverter::FromIntToString(SystemInfo.GetCacheLineSize()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "L2 Associativity : "            + vml::strings::StringConverter::FromIntToString(SystemInfo.GetL2Associativity()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Cache size : "                  + vml::strings::StringConverter::FromIntToString(SystemInfo.GetCacheSize()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Physical address : "            + vml::strings::StringConverter::FromIntToString(SystemInfo.GetPhysicalAddress()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Virtual address : "             + vml::strings::StringConverter::FromIntToString(SystemInfo.GetVirtualAddress()));
				vml::Logger::GetInstance()->Out(currentdate, "SysInfo", "Logical processor(s) number : " + vml::strings::StringConverter::FromIntToString(SystemInfo.GetLogicalProcessorsNumber()));
				vml::Logger::GetInstance()->Out(currentdate, "Core"   , "Retrieving system info", "Done");

				// init session logger

				vml::Session::GetInstance()->Init();

				// init stores

				// set context as started
				
				vml::bits32::SetToTrue(InternalFlags, vml::InternalFlags::INITTED);

				vml::Logger::GetInstance()->Out(currentdate, "Core", "Initting core system", "Done");
				
			}

			// -----------------------------------------------------------------------------
			// closes Core

			void Close()
			{
				
				if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("Core : ","Cannot close context before it has not been started");
				
				// log closes by itself in its destructor
				
				std::string currentdate = vml::Logger::GetInstance()->GetCurrentDate();
				
				vml::Logger::GetInstance()->Out(currentdate, "Core", "Shutting down core system", "In progress...");
				
				// defaults flags

				PreferencesFlags = 0;
				InternalFlags	 = 0;
				
				// close stores
				
				vml::SafeDelete(vml::ShaderStore);
				vml::SafeDelete(vml::MeshStore);
				vml::SafeDelete(vml::TextureStore);

				// close session

				vml::Session::GetInstance()->Close();
		
				vml::Logger::GetInstance()->Out(currentdate, "Core", "Shutting down Core system", "Done");

				// close logger 

				vml::Logger::GetInstance()->Epilogue();
				
				// delete singleton logger

				vml::Logger::GetInstance()->Close();
				
			}

			// -----------------------------------------------------------------------------
			// set wrking directory and paths

			void SetMainPath(const std::string& mainpath)
			{
				if (vml::bits32::Get(InternalFlags, vml::InternalFlags::PATHSINITTED))
					vml::os::Message::Error("Core : ","Paths already initted");

				// setting main path

				vml::Session::GetInstance()->SetPath(mainpath);

				std::string currentdate= vml::Logger::GetInstance()->GetCurrentDate();

				// checks if main path exists

				if (std::filesystem::status_known(std::filesystem::file_status{}) ? std::filesystem::exists(std::filesystem::file_status{}) : std::filesystem::exists(mainpath))
					vml::Logger::GetInstance()->Out(currentdate, "Main Path", vml::Session::GetInstance()->GetMainPath());
				else vml::os::Message::Error("Core : ","Missing Main Path");

				// init shader store

				std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath()+"\\content\\opengl\\shaders";

				if (std::filesystem::status_known(std::filesystem::file_status{}) ? std::filesystem::exists(std::filesystem::file_status{}) : std::filesystem::exists(shaderstorepath))
					vml::Logger::GetInstance()->Out(currentdate, "ShaderStore Path", shaderstorepath);
				else vml::os::Message::Error("Core : ","Missing ShaderStore Path");

				vml::ShaderStore = new vml::shaders::ShaderStore("ShaderStore");

				// end of shaderstore init

				// init mesh store
				
				std::string meshesstorepath = vml::Session::GetInstance()->GetMainPath()+"\\content\\opengl\\meshes";

				if (std::filesystem::status_known(std::filesystem::file_status{}) ? std::filesystem::exists(std::filesystem::file_status{}) : std::filesystem::exists(meshesstorepath))
					vml::Logger::GetInstance()->Out(currentdate, "MeshStore Path", meshesstorepath);
				else vml::os::Message::Error("Core : ","Missing MeshStore Path");

				vml::MeshStore = new vml::meshes::MeshStore("MeshStore");
				
				// end of meshesstore init

				// init texture store 

				std::string texturestorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\textures";

				if (std::filesystem::status_known(std::filesystem::file_status{}) ? std::filesystem::exists(std::filesystem::file_status{}) : std::filesystem::exists(texturestorepath))
					vml::Logger::GetInstance()->Out(currentdate, "TextureStore Path", texturestorepath);
				else vml::os::Message::Error("Core : ","Missing TextureStore Path");

				vml::TextureStore = new vml::textures::TextureStore("TextureStore");

				// set paths as initted

				currentdate = vml::Logger::GetInstance()->GetCurrentDate();
				vml::bits32::SetToTrue(InternalFlags, vml::InternalFlags::PATHSINITTED);
				vml::Logger::GetInstance()->Out(currentdate, "Core", "Initting stores", "Done");
				
			}

			// -----------------------------------------------------------------------------
			
			void SetCurrentUserName(const std::string& name)
			{
				vml::Session::GetInstance()->SetUserName(name);
			}

			// -----------------------------------------------------------------------------
			
			void SetCurrentPath()
			{
				SetMainPath(std::filesystem::current_path().string());
			}

			// -----------------------------------------------------------------------------

			const std::string& GetCurrentPath() 
			{
				return vml::Session::GetInstance()->GetMainPath();
			}

			// -----------------------------------------------------------------------------

			const std::string GetFpsString()
			{
				return vml::strings::StringConverter::FromIntToString((int)Timer.GetFPS());
			}

			// -----------------------------------------------------------------------------
			// virtual function members

			virtual bool LoadResources() = 0;
			virtual void DisposeResources() = 0;

			// -----------------------------------------------------------------------------
			// ctor / dtor

			Core()
			{
				InternalFlags    = 0;
				PreferencesFlags = 0;
			}

			virtual ~Core()
			{
			}
	};
			

} // end of vml namespace



