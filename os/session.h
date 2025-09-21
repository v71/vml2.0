#pragma once

////////////////////////////////////////////////////////////////////////////
// 
// 	class Session

namespace vml
{

	class Session
	{
		private:

			std::clock_t  ClockStart;	// session time
			std::string	  User;			// user name
			std::string	  MainPath;		// main path
			unsigned int  Flags;		// flags to set initted state

			// ---------------------------------------------
			// singleton design pattern
			// A singlton is a class instantiated only once.
			// Usually it is globally accessiblae,
			// constructor and destructors are kept private to the 
			// final user. User has the responsability to delete
			// memory associated with the class explicitly at
			// main entry point closure

			static Session* Singleton;

			// ---------------------------------------------
			// private ctor / dtor

			Session()
			{
				Flags      = 0;
				ClockStart = std::clock();
				MainPath   = "";
				User       = "";
			}

		public:

			// -----------------------------------------------------------------------------
			//

			void SetPath(const std::string& mainpath)
			{
				if (!vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("Session : ","Not initted");
				if (!MainPath.empty())
					vml::os::Message::Error("Session : ","Path is already set");
				if (mainpath.empty())
					vml::os::Message::Error("Session : ","Path is empty");
				MainPath = std::string(std::filesystem::path({ mainpath }).string());
				std::string currentdate = vml::Logger::GetInstance()->GetCurrentDate();
				vml::Logger::GetInstance()->Out(currentdate, "Session", "Path : " + std::string(MainPath.begin(), MainPath.end()));
			}

			// -----------------------------------------------------------------------------
			//

			void SetUserName(const std::string& name)
			{
				if (!vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("Session : ", "Not initted");
				if (name.empty())
					vml::os::Message::Error("Session : ", "Path is empty");
				// use is responsible for name validity
				// but spaces left or right if eventually present will be trimmed away
				if (!User.empty())
					vml::os::Message::Error("Session : ", "Path is already set");
				User = name;
				std::string currentdate = vml::Logger::GetInstance()->GetCurrentDate();
				vml::Logger::GetInstance()->Out(currentdate, "Session", "Setting UserName As : " + User, "Done");
			}

			// -----------------------------------------------------------------------------
			//

			const std::string GetMainPath() const
			{
				if (!vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("Session : ", "Not initted");
				return std::string(MainPath.begin(), MainPath.end());
			}

			// -----------------------------------------------------------------------------
			//

			const std::string& GetUser()
			{
				if (!vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("Session : ", "Not initted");
				return User;
			}

			//---------------------------------------------------------------------
			// get instance of singleton , if singleton is null , which is mandatory at
			// application startup, it will be allocated only once
			// Eventually, to reallocate , user must call the delete member function

			static Session* GetInstance()
			{
				if (!Singleton)
				{
					std::string currentdate = vml::Logger::GetInstance()->GetCurrentDate();
					vml::Logger::GetInstance()->Out(currentdate, "Session", "Singleton Allocation", "In progress...");
					Singleton = new Session();
					vml::Logger::GetInstance()->Out(currentdate, "Session", "Singleton Allocation", "Done");
				}
				return Singleton;
			}

			// ------------------------------------------------------------
			// initialise session

			void Init()
			{
				if (vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("Session : ","Already initted");
				std::string currentdate = vml::Logger::GetInstance()->GetCurrentDate();
				vml::Logger::GetInstance()->Out(currentdate, "Session", "Initting", "In progress...");
				Flags = 0;
				ClockStart = std::clock();
				vml::bits32::SetToTrue(Flags, vml::InternalFlags::INITTED);
				vml::Logger::GetInstance()->Out(currentdate, "Session", "Initting", "Done");
			}

			// ------------------------------------------------------------
			// closes stream

			void Close()
			{
				if (!vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("Session : ","Not initted");
				Flags = 0;
				if (Singleton)
				{
					std::string currentdate = vml::Logger::GetInstance()->GetCurrentDate();
					vml::Logger::GetInstance()->Out(currentdate, "Session", "Singleton Deallocation", "In progress...");
					vml::SafeDelete(Singleton);
					vml::Logger::GetInstance()->Out(currentdate, "Session", "Singleton Deallocation", "Done");
				}
			}

			// -----------------------------------------------------------------------------
			//

			float GetCurrentTime() const { return std::clock(); }
			float GetElapsedTime() const { return std::clock() - ClockStart; }

			// -----------------------------------------------------------------------------
			//

			const bool IsInitted() const { return vml::bits32::Get(Flags, vml::InternalFlags::INITTED) == 1; }

			//---------------------------------------------------------------------
			// copy constructor is private
			// no copies allowed since classes
			// are referenced

			Session(Session& other) = delete;

			//---------------------------------------------------------------------
			// overload operator is private, 
			// no copies allowed since classes
			// are referenced

			void operator=(const Session&) = delete;

			// -----------------------------------------------------------------------------
			// public destructor

			~Session()
			{}

	};

}
