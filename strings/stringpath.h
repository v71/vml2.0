#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
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

////////////////////////////////////////////////////////////////
// class for path splitting

namespace vml
{
	namespace strings
	{

		class SplitPath
		{

				// -------------------------------------------------------
				// this character it MUST be \\

				static const char _SEPARATOR_ = '\\';

			public:

				// -------------------------------------------------------
				// Divides a path up into pieces.

				static int Split(const std::string& Path, std::vector<std::string>& Pieces)
				{
					std::string piece;

					// find _SEPARATOR_

					for (size_t i = 0; i < Path.size(); i++)
					{
						char c = Path[i];

						if (c == _SEPARATOR_)
						{
							if (piece.empty()) continue;

							Pieces.push_back(piece);

							piece = "";
						}

						else piece += c;
					}

					// add last piece

					if (piece.size())
					{
						Pieces.push_back(piece);
					}

					// return number of pieces

					return int(Pieces.size());
				}

				// -------------------------------------------------------
				// Returns the drive's letter from the passed path.

				static std::string GetDrive(const std::string& Path)
				{
					std::string m_buffer;

					size_t colon = (size_t)Path.find(':');

					if (colon != std::string::npos)
					{
						m_buffer = Path[colon - 1];
					}

					return m_buffer;
				}

				// -------------------------------------------------------
				// Returns the directory from the passed path.

				static std::string GetDirectory(const std::string &Path)
				{
					std::string m_buffer;

					// the highest valid index
					int last = (int)Path.size() - 1;

					// no valid index
					if (last < 0) return "";

					// last _SEPARATOR_
					int sep = (int)Path.rfind(_SEPARATOR_);

					// _SEPARATOR_ is last character
					if (sep == last)
					{
						m_buffer = Path;
					}

					// _SEPARATOR_ is anywhere else
					else if (sep != std::string::npos)
					{
						m_buffer = Path.substr(0, size_t(sep)+1);
					}

					return m_buffer;
				}

				// -------------------------------------------------------
				// Returns the file's folder from the passed path.

				static std::string GetFolder(const std::string &Path)
				{
					std::string m_buffer;

					// get directory
					std::string dir = GetDirectory(Path);

					// next to last _SEPARATOR_

					size_t sep;

					sep = dir.rfind(_SEPARATOR_);
					sep = dir.rfind(_SEPARATOR_, sep - 1);

					// no next to last _SEPARATOR_
					if (sep == std::string::npos) return "";

					// get folder
					m_buffer = dir.substr( sep+1, dir.size() - sep - 2);

					return m_buffer;
				}

				// -------------------------------------------------------
				// Returns the file's name from the passed path.

				static std::string GetName(const std::string &Path)
				{
					std::string m_buffer;

					// number of characters in path
					
					size_t psize = Path.size();

					// number of characters in dir
				
					size_t dsize = (size_t) GetDirectory(Path).length();

					if (dsize < psize)
					{
						m_buffer = Path.substr(dsize, psize-dsize);
					}
	
					// there is no name
					
//					else return "";

					return m_buffer;
				}

				// -------------------------------------------------------
				// Returns the file's title from the passed path.
				// title is filename without extension
				
				static std::string GetTitle(const std::string &Path)
				{
					std::string m_buffer;

					// get name
					std::string name = GetName(Path);

					// last dot
					int dot = (int)name.rfind(".");

					// no dot
					if (dot == std::string::npos)
					{
						m_buffer = name;
					}

					// dot is anywhere else
					else
					{
						m_buffer = name.substr(0, dot);
					}

					return m_buffer;
				}
				
				// -------------------------------------------------------
				// Returns the file's extension from the passed path.

				static std::string GetExtensionFromName(const std::string &Path)
				{
					std::string m_buffer;

					// get name
					std::string name = GetName(Path);

					// last dot
					size_t dot = name.rfind(".");

					// no dot
					if (dot == std::string::npos) return "";

					// get extension
					m_buffer = name.substr( dot+1, (name.size() - dot - 1));

					return m_buffer;
				}
				
				// -------------------------------------------------------
				// gets file extension 

				static std::string GetExtension(const std::string &Path)
				{
					std::string m_buffer;

					// last dot
					size_t dot = Path.rfind(".");

					// no dot
					if (dot == std::string::npos) return "";

					// get extension
					m_buffer = Path.substr( dot+1, (Path.size() - dot - 1));

					return m_buffer;
				}
				
				// -------------------------------------------------------
				// remove extension from filename, filename is not changed

				static std::string RemoveExtension(const std::string filename)
				{
					size_t lastdot = filename.find_last_of(".");
					if (lastdot == std::string::npos) return filename;
					return filename.substr(0, lastdot);
				}
				
				// -------------------------------------------------------
				// Returns the absolute path starting at Origin.

				static std::string GetAbsPath(const std::string &RelPath, const std::string &Origin)
				{
					std::string m_buffer;

					std::string relpath(RelPath);
					std::string name = GetName(RelPath);
					std::string dir = GetDirectory(Origin);

					m_buffer = "";

					// RelPath is already absolute
					if (relpath.find(':') != std::string::npos)
					{
						m_buffer = relpath;
						return m_buffer;
					}

					// separated folders
					std::vector<std::string> fPath;
					std::vector<std::string> fOrigin;

					// split into folders
					Split(relpath, fPath);
					Split(dir, fOrigin);

					// nothing splitted
					if (fPath.empty()) return "";
					if (fOrigin.empty()) return "";

					// remove 'current dir'
					if (fPath[0] == ".") fPath.erase(fPath.begin());
					if (fOrigin[0] == ".") fOrigin.erase(fOrigin.begin());

					// nemove filename
					if (name.size()) fPath.pop_back();

					// number of dotted folders
					unsigned int dotted;

					// count dotted folders
					for (dotted = 0; dotted < (unsigned int)fPath.size(); dotted++)
					{
						if (fPath[dotted] != "..") break;
					}

					// add absolute pieces
					for (unsigned int abs = 0; abs < (unsigned int)(fOrigin.size() - dotted); abs++)
					{
						m_buffer += fOrigin[abs];
						m_buffer += _SEPARATOR_;
					}

					// add relative pieces
					for (unsigned int rel = dotted; rel < (unsigned int)fPath.size(); rel++)
					{
						m_buffer += fPath[rel];
						m_buffer += _SEPARATOR_;
					}

					// add filename
					m_buffer += name;

					return m_buffer;
				}
				
				// -------------------------------------------------------
				// Returns the relative path starting at Origin.

				static std::string GetRelPath(const std::string &AbsPath, const std::string &Origin)
				{
					std::string m_buffer;

					std::string abspath(AbsPath);
					std::string name = GetName(AbsPath);
					std::string dir = GetDirectory(Origin);

					m_buffer = "";

					// AbsPath is already relative
					if (abspath.find(':') == std::string::npos)
					{
						m_buffer = abspath;
						return m_buffer;
					}

					// separated folders
					std::vector<std::string> fPath;
					std::vector<std::string> fOrigin;

					// split into folders
					Split(abspath, fPath);
					Split(dir, fOrigin);

					// nothing splitted
					if (fPath.empty()) return "";
					if (fOrigin.empty()) return "";

					// remove 'current dir'
					if (fPath[0] == ".") fPath.erase(fPath.begin());
					if (fOrigin[0] == ".") fOrigin.erase(fOrigin.begin());

					// remove filename
					if (name.size()) fPath.pop_back();

					// number of equal folders
					unsigned int equal;

					// count equal folders (from the beginning)
					for (equal = 0; ((equal < (unsigned int)fOrigin.size()) && (equal < (unsigned int)fPath.size())); equal++)
					{
						// get length
						unsigned int size1 = (unsigned int)fPath[equal].size();
						unsigned int size2 = (unsigned int)fOrigin[equal].size();

						// different folders found (length)
						if (size1 != size2)
						{
							break;
						}

						// different folders found (lexicographically)
						else if (_strnicmp(fPath[equal].c_str(), fOrigin[equal].c_str(), size1))
						{
							break;
						}
					}

					// different drives
					if (equal == 0)
					{
						m_buffer = abspath;
						return m_buffer.c_str();
					}

					// move up
					for (unsigned int up = equal; up < (unsigned int)fOrigin.size(); up++)
					{
						m_buffer += "..";
						m_buffer += _SEPARATOR_;
					}

					// move down
					for (unsigned int dn = equal; dn < (unsigned int)fPath.size(); dn++)
					{
						m_buffer += fPath[dn];
						m_buffer += _SEPARATOR_;
					}

					// add filename
					m_buffer += name;

					// current directory
					if (m_buffer.empty())
					{
						m_buffer = "." + _SEPARATOR_;
					}

					return m_buffer;
				}
				
				// -------------------------------------------------------
				// subtract path s string from another

				static std::string RemovePath(std::string &a, const std::string& b)
				{
					size_t s = a.find(b);
					if (s == std::string::npos)
						return a;
					return a.erase(int(s), b.length());
				}
				
				// -------------------------------------------------------
				// splits a given path into a vector of folders ,last element is file

				static std::vector<std::string> DirSplit(const std::string& s)
				{
					std::vector<std::string> sd;

					if (s.size() == 0)
						return sd;

					const char c = _SEPARATOR_;

					std::string::size_type i = 0;
					std::string::size_type j = s.find(c);

					while (j != std::string::npos)
					{
						sd.emplace_back(std::string(s.substr(i, j - i)));

						i = ++j;

						j = s.find(c, j);

						if (j == std::string::npos)
							sd.emplace_back(std::string(s.substr(i, s.length())));
					}

					return sd;
				}
				
				// -------------------------------------------------------
				// ctor / dtor

				SplitPath()
				{}
				~SplitPath()
				{}

			};

	} // end of namespace strings

} // end of namespace vml