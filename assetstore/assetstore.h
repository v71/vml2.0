#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2030 v71 
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

////////////////////////////////////////////////////////////
// resource manager 
// the resource manager needs the log class to be initialized

namespace vml
{

	namespace resources
	{
		class SharedResource
		{
			private:

				template < class T>
				friend class SharedResourceStore;

			protected:

				// ----------------------------------------------------------------
				// protected data

				std::string		  ResourceFileName;
				int				  Instances;

			public:

				// ----------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				SharedResource(const SharedResource& sharedresource) = delete;

				// ----------------------------------------------------------------
				// overload operator = is private
				// no copies allowed since classes
				// are referenced

				SharedResource& operator = (const SharedResource& sharedresource) = delete;

				// --------------------------------------------------------------------------------
				// getters

				const std::string& GetResourceFileName() const
				{
					return ResourceFileName;
				}

				int GetInstancesCount() const
				{
					return Instances;
				}

				// -----------------------------------------------------------------------
				// dump info


				const std::string Dump()
				{
					std::string text;
				
					text =  std::format("Full path filename : {}\n", ResourceFileName.c_str());
					text += std::format("File directory     : {}\n", vml::strings::SplitPath::GetDirectory(ResourceFileName).c_str());
					text += std::format("File extension     : {}\n", vml::strings::SplitPath::GetExtension(ResourceFileName).c_str());
					text += std::format("File path          : {}\n", vml::strings::SplitPath::GetDrive(ResourceFileName).c_str());
					text += std::format("Instances : {}\n", Instances);

				//	text  = vml::strings::StringFormat::Text("Full path filename : {0}\n", ResourceFileName.c_str());
				//	text += vml::strings::StringFormat::Text("File directory     : {0}\n", vml::strings::SplitPath::GetDirectory(ResourceFileName).c_str());
				//	text += vml::strings::StringFormat::Text("File extension     : {0}\n", vml::strings::SplitPath::GetExtension(ResourceFileName).c_str());
				//	text += vml::strings::StringFormat::Text("File path          : {0}\n", vml::strings::SplitPath::GetDrive(ResourceFileName).c_str());
				//	text += vml::strings::StringFormat::Text("Instances : {0}\n", Instances);
					
					return text;
				}

				// --------------------------------------------------------------------------------
				// ctor / dtor

				SharedResource(const std::string& key)
				{
					if (key.empty())
						vml::os::Message::Error("SharedResource : ","Null SharedResource name is not allowed");
			
					ResourceFileName = key;
					Instances        = 0;
				}

				virtual ~SharedResource()
				{
				}

		};

	}  // end of namespace SharedResources

} // end of namespace vml


/////////////////////////////////////////////////////////////////////////////////////////
// SharedResource manager

namespace vml
{
	namespace resources
	{
		
		template< class T>
		class SharedResourceStore
		{
			private:
			
				// --------------------------------------------------------------------------------
				// private data

				std::string						   Name;
				uint32_t						   InternalFlags;
				uint32_t						   PreferencesFlags;
				std::unordered_map<std::string,T*> Cache;

				// --------------------------------------------------------------------------------
				// delete all cache content

				void ReleaseAll()
				{

					if (vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::VERBOSE))
					{
						vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), Name, "Releasing " + Name, "Done");
					}

					// force release of all elements in the cache

					for (auto it = Cache.begin(); it != Cache.end(); ++it)
					{
						T* item = it->second;

						if (vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::VERBOSE))
						{
							vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(),
															Name, 
															"Relasing " + item->GetResourceFileName() + ", instances : " + vml::strings::StringConverter::FromIntToString(item->GetInstancesCount()));
						}

						delete it->second;
					}

					Cache.clear();

					// sets internal flags

					vml::bits32::SetToFalse(InternalFlags, vml::InternalFlags::INITTED);
				
				}
				
			public:
				
				//---------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				SharedResourceStore<T>(const SharedResourceStore<T> &SharedResourcestore) = delete;

				//---------------------------------------------------------------------
				// overload operator is private, 
				// no copies allowed since classes
				// are referenced

				SharedResourceStore<T>& operator = (const SharedResourceStore<T> &SharedResourcestore) = delete;

				// --------------------------------------------------------------------------------
				// adds new element to the cache

				T* Load(const std::string &fullfilename)
				{
					if (fullfilename.empty())
						vml::os::Message::Error("SharedResource Store : ","Null filename");
					
					//std::string fullfilename = Path + filename;

					// checks if resource is already loaded 

					auto it = Cache.find(fullfilename);

					if (it == Cache.end())
					{
						// allocate new resource

						T *item = new T(fullfilename);
			
						Cache.insert(std::pair<std::string, T*>(fullfilename, item));
					
						item->Instances++;

						if (vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::VERBOSE))
						{
							vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(),
								                            Name, 
								                            "Adding " + fullfilename + ", instances : " + vml::strings::StringConverter::FromIntToString(item->GetInstancesCount()));
						}

						return item;
					}

					// if resource is already present, icrease reference count and return its pointer

					it->second->Instances++;

					if (vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::VERBOSE))
					{
						vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), 
							                            Name, 
							                            fullfilename + " : is already loaded, instances : " + vml::strings::StringConverter::FromIntToString(it->second->GetInstancesCount()));
					}

					return it->second;
				
				}

				// --------------------------------------------------------------------------------
				// remove element from database

				bool UnLoad(const std::string &filename)
				{
					if (filename.empty())
						vml::os::Message::Error("SharedResource Store : ","Null filename when unloading");

					std::string key = filename;
				
					auto it = Cache.find(key);

					if (it != Cache.end())
					{
						
						T* item = it->second;

						if (vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::VERBOSE))
						{
							vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), Name, "Found " + key + ", instances : " + vml::strings::StringConverter::FromIntToString(item->GetInstancesCount()));
						}

						item->Instances--;

						if (item->Instances == 0)
						{
							if (vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::VERBOSE))
							{
								vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), Name, "no more references for : " + key + ", instances : " + vml::strings::StringConverter::FromIntToString(item->GetInstancesCount()));
							}

							delete (it->second);
						
							Cache.erase(it);

							return true;
						}
					
					}
					else
					{
						if (vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::VERBOSE))
						{
							vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), Name, "couldn't find : " + key);
						}
					}

					return false;
				}
			
				// --------------------------------------------------------------------------------
				// dumps content of cache map

				const std::string Dump()
				{
					std::string text = Name + " : Dumping cache\n";

					for (auto it = Cache.begin(); it != Cache.end(); ++it)
					{
						T* item = it->second;
						
						text += item->ResourceFileName + " : instances : " + vml::strings::StringConverter::FromIntToString(item->Instances) + "\n";
					}

					return text;
				}

				// --------------------------------------------------------------------------------
				// gets database name
			
				const std::string &GetName() const
				{
					return Name;
				}
		
				bool IsVerbose() const
				{
					return vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::VERBOSE) == true;
				}

				bool IsQuiet() const
				{
					return vml::bits32::Get(PreferencesFlags, vml::PreferencesFlags::VERBOSE) == false;
				}
				
				// --------------------------------------------------------------------------------
				// ctor / dtor

				[[nodiscard("AssetStore Without Instance!")]] SharedResourceStore(const std::string &name, 
																				  uint32_t flags = vml::PreferencesFlags::VERBOSE)
				{
					
					PreferencesFlags = flags;
					InternalFlags	 = 0;
					
					// check if we have a valid name 
					
					if (name.empty())
						vml::os::Message::Error("SharedResourceStore : ","Name cannot be empty");
				
					// sets SharedResource store name

					Name = name;

					// validate preferences flags

					if (IsVerbose() && IsQuiet())
						vml::os::Message::Error("SharedResourceStore : ","Cannot set both verbose and quiet flags");

					// set verbose flag if needed
					
					if (IsVerbose())
						vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), Name, "Initting Store " + Name ,"Done");

					// set initted to true

					vml::bits32::SetToTrue(InternalFlags, vml::InternalFlags::INITTED);
				
				}

				virtual ~SharedResourceStore()
				{
					ReleaseAll();
				}
				
		};
		
	}  // end of namespace SharedResources

} // end of namespace vml

