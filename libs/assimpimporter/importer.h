#pragma once

///////////////////////////////////////////////////////////////////////

#include <vml2.0\strings\stringconverter.h>
#include <vml2.0/core/mesh/meshbuilder.h>
#include <vml2.0\libs\assimp\cimport.h>
#include <vml2.0\libs\assimp\scene.h>
#include <vml2.0\libs\assimp\postprocess.h>
#include <vml2.0/libs/rapidjson/rapidjson.h>
#include <vml2.0/libs/rapidjson/document.h>

///////////////////////////////////////////////////////////////////////
// Assimp Importer

namespace vml
{
	namespace importer
	{
		
		class ModelImporter3d
		{
			
			private:

				std::string ImportError;

			private:

				#define aisgl_min(x,y) (x<y?x:y)
				#define aisgl_max(x,y) (y>x?y:x)

			// -----------------------------------------------------------------------

			class TextureFileNames
			{
				public:

					std::string Path[32];

					// -------------------------------------------------------------------------------------------------
					
					const std::string Dump()
					{
						std::string text;
						if (Path[aiTextureType_DIFFUSE].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_DIFFUSE) + " " + Path[aiTextureType_DIFFUSE] + "\n";
						if (Path[aiTextureType_SPECULAR].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_SPECULAR) + " " + Path[aiTextureType_SPECULAR] + "\n";
						if (Path[aiTextureType_AMBIENT].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_AMBIENT) + " " + Path[aiTextureType_AMBIENT] + "\n";
						if (Path[aiTextureType_EMISSIVE].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_EMISSIVE) + " " + Path[aiTextureType_EMISSIVE] + "\n";
						if (Path[aiTextureType_HEIGHT].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_HEIGHT) + " " + Path[aiTextureType_HEIGHT] + "\n";
						if (Path[aiTextureType_NORMALS].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_NORMALS) + " " + Path[aiTextureType_NORMALS] + "\n";
						if (Path[aiTextureType_SHININESS].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_SHININESS) + " " + Path[aiTextureType_SHININESS] + "\n";
						if (Path[aiTextureType_OPACITY].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_OPACITY) + " " + Path[aiTextureType_OPACITY] + "\n";
						if (Path[aiTextureType_DISPLACEMENT].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_DISPLACEMENT) + " " + Path[aiTextureType_DISPLACEMENT] + "\n";
						if (Path[aiTextureType_LIGHTMAP].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_LIGHTMAP) + " " + Path[aiTextureType_LIGHTMAP] + "\n";
						if (Path[aiTextureType_REFLECTION].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_REFLECTION) + " " + Path[aiTextureType_REFLECTION] + "\n";
						if (Path[aiTextureType_BASE_COLOR].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_BASE_COLOR) + " " + Path[aiTextureType_BASE_COLOR] + "\n";
						if (Path[aiTextureType_NORMAL_CAMERA].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_NORMAL_CAMERA) + " " + Path[aiTextureType_NORMAL_CAMERA] + "\n";
						if (Path[aiTextureType_EMISSION_COLOR].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_EMISSION_COLOR) + " " + Path[aiTextureType_EMISSION_COLOR] + "\n";
						if (Path[aiTextureType_METALNESS].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_METALNESS) + " " + Path[aiTextureType_METALNESS] + "\n";
						if (Path[aiTextureType_DIFFUSE_ROUGHNESS].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_DIFFUSE_ROUGHNESS) + " " + Path[aiTextureType_DIFFUSE_ROUGHNESS] + "\n";
						if (Path[aiTextureType_AMBIENT_OCCLUSION].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_AMBIENT_OCCLUSION) + " " + Path[aiTextureType_AMBIENT_OCCLUSION] + "\n";
						if (Path[aiTextureType_SHEEN].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_SHEEN) + " " + Path[aiTextureType_SHEEN] + "\n";
						if (Path[aiTextureType_CLEARCOAT].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_CLEARCOAT) + " " + Path[aiTextureType_CLEARCOAT] + "\n";
						if (Path[aiTextureType_TRANSMISSION].size() > 0)
							text += "Type :" + vml::strings::StringConverter::FromIntToString(aiTextureType_TRANSMISSION) + " " + Path[aiTextureType_TRANSMISSION] + "\n";
						return text;
					}
					
					// ---------------------------------------------------------
					// ctor / dtor

					TextureFileNames()
					{
					}

					~TextureFileNames()
					{}

			};
			
			// -----------------------------------------------------------------------
			// compute scene bounding box

			void GetBoundingBoxForNode(const struct aiScene* scene,
										const struct aiNode* nd,
										aiVector3D* min,
										aiVector3D* max,
										aiMatrix4x4* trafo)
			{
				aiMatrix4x4 prev;
				unsigned int n = 0, t;

				prev = *trafo;

				aiMultiplyMatrix4(trafo, &nd->mTransformation);

				for (; n < nd->mNumMeshes; ++n)
				{
					const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

					for (t = 0; t < mesh->mNumVertices; ++t)
					{

						aiVector3D tmp = mesh->mVertices[t];

						aiTransformVecByMatrix4(&tmp, trafo);

						min->x = aisgl_min(min->x, tmp.x);
						min->y = aisgl_min(min->y, tmp.y);
						min->z = aisgl_min(min->z, tmp.z);

						max->x = aisgl_max(max->x, tmp.x);
						max->y = aisgl_max(max->y, tmp.y);
						max->z = aisgl_max(max->z, tmp.z);
				
					}
			
				}

				for (n = 0; n < nd->mNumChildren; ++n)
				{
					GetBoundingBoxForNode(scene, nd->mChildren[n], min, max, trafo);
				}

				*trafo = prev;
			}
			
			// -----------------------------------------------------------------------
			// compute scene bounding box

			void GetBoundingBox(const struct aiScene* scene, aiVector3D* min, aiVector3D* max)
			{
				aiMatrix4x4 trafo;
				aiIdentityMatrix4(&trafo);
				min->x = min->y = min->z = 1e10f;
				max->x = max->y = max->z = -1e10f;
				GetBoundingBoxForNode(scene, scene->mRootNode, min, max, &trafo);
			}

			// -----------------------------------------------------------------------

			void GetMaterial(const aiMaterial* materialPtr, aiTextureType type, TextureFileNames& texfname)
			{
				if (materialPtr->GetTextureCount(type) > 0)
				{
					aiString aiTexturePath;

					if (materialPtr->GetTexture(type, 0, &aiTexturePath) == AI_SUCCESS)
					{
						texfname.Path[type] = aiTexturePath.C_Str();
					}
				}
			}
			
			// -----------------------------------------------------------------------

			std::vector<TextureFileNames> ExtractMaterials(const struct aiScene* scene, const std::string& filePath)
			{
				std::vector<TextureFileNames> texturefilenames;
				for (auto i = 0; i < scene->mNumMaterials; i++)
				{
					const auto materialPtr = scene->mMaterials[i];
					TextureFileNames texfname;
					GetMaterial(materialPtr, aiTextureType_DIFFUSE, texfname);
					GetMaterial(materialPtr, aiTextureType_SPECULAR, texfname);
					GetMaterial(materialPtr, aiTextureType_AMBIENT, texfname);
					GetMaterial(materialPtr, aiTextureType_EMISSIVE, texfname);
					GetMaterial(materialPtr, aiTextureType_HEIGHT, texfname);
					GetMaterial(materialPtr, aiTextureType_NORMALS, texfname);
					GetMaterial(materialPtr, aiTextureType_SHININESS, texfname);
					GetMaterial(materialPtr, aiTextureType_OPACITY, texfname);
					GetMaterial(materialPtr, aiTextureType_DISPLACEMENT, texfname);
					GetMaterial(materialPtr, aiTextureType_LIGHTMAP, texfname);
					GetMaterial(materialPtr, aiTextureType_REFLECTION, texfname);
					GetMaterial(materialPtr, aiTextureType_BASE_COLOR, texfname);
					GetMaterial(materialPtr, aiTextureType_NORMAL_CAMERA, texfname);
					GetMaterial(materialPtr, aiTextureType_EMISSION_COLOR, texfname);
					GetMaterial(materialPtr, aiTextureType_METALNESS, texfname);
					GetMaterial(materialPtr, aiTextureType_DIFFUSE_ROUGHNESS, texfname);
					GetMaterial(materialPtr, aiTextureType_AMBIENT_OCCLUSION, texfname);
					GetMaterial(materialPtr, aiTextureType_SHEEN, texfname);
					GetMaterial(materialPtr, aiTextureType_CLEARCOAT, texfname);
					GetMaterial(materialPtr, aiTextureType_TRANSMISSION, texfname);
					texturefilenames.emplace_back(texfname);
				}
				return texturefilenames;
			}
		

		public:

			bool Verbose;
		
			// -----------------------------------------------------------------------
			// import model from various file formats, thanks to assimp

			bool Import(const std::string& filename,
						const std::string& destdir,
						const glm::vec3& Rot = glm::vec3(0, 0, 0),
						const glm::vec3& Scale = glm::vec3(1, 1, 1))
			{
			
				const struct aiScene* scene;
				unsigned int vertices;
				unsigned int texcoords;
				unsigned int normalcoords;
				unsigned int colorcoords;
				unsigned int tangents;
				unsigned int bitangents;
				unsigned int surfaces;
				aiString name;
				aiVector3D min, max;
				struct aiLogStream stream;
				aiVector3D scene_min, scene_max;
				float scaleFactor;

				// start importing

				ImportError = "NoError";

				if (filename.empty())
				{
					ImportError = "Fatal : No input filaname";
					return false;
				}

				if (destdir.empty())
				{
					ImportError = "Fatal : No output path";
					return false;
				}

				// get a handle to the predefined STDOUT log stream and attach
				// it to the logging system. It remains active for all further
				// calls to aiImportFile(Ex) and aiApplyPostProcessing.

				//	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
				//	aiAttachLogStream(&stream);

				// ... same procedure, but this stream now writes the
				// log messages to assimp_log.txt
				stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
				aiAttachLogStream(&stream);

				// load the model
				// we are taking one of the postprocessing presets to avoid
				// spelling out 20+ single postprocessing flags here.

				scene = aiImportFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

				if (!scene)
				{
					ImportError = "Fatal : An error occured when reading file" + filename + "( " + aiGetErrorString() + " )";
			
					// cleanup - calling 'aiReleaseImport' is important, as the library 
					// keeps internal resources until the scene is freed again. Not 
					// doing so can cause severe resource leaking.

					aiReleaseImport(scene);

					// We added a log stream to the library, it's our job to disable it
					// again. This will definitely release the last resources allocated
					// by Assimp.

					aiDetachAllLogStreams();

					return false;
				}
				
				std::string title = vml::strings::SplitPath::GetTitle(filename);
				std::string sourcepath = vml::strings::SplitPath::GetDirectory(filename);
				std::string destpath = destdir + title + "_imp";

				std::cout << filename << std::endl;
				std::cout << destdir << std::endl;
				std::cout << destpath << std::endl;
				std::cout << title << std::endl;

								
				// appne further data to assimp_log file

			//	std::ofstream fout("assimp_log.txt", std::ios_base::app);

				// gets scene bounding box

				GetBoundingBox(scene, &scene_min, &scene_max);

				float tmp = scene_max.x - scene_min.x;
				tmp = scene_max.y - scene_min.y > tmp ? scene_max.y - scene_min.y : tmp;
				tmp = scene_max.z - scene_min.z > tmp ? scene_max.z - scene_min.z : tmp;
				scaleFactor = 1.f / tmp;

				// read materials

				std::vector<TextureFileNames> extractedtexturefilenames;

				extractedtexturefilenames = ExtractMaterials(scene, filename);

				// read in meshes

				std::string modelname = vml::strings::SplitPath::GetTitle(filename);
				std::string descriptor = destpath + "\\" + modelname + ".txt";
				std::filesystem::path path{ descriptor };
				std::filesystem::create_directories(destpath);
				std::string texturepath = destpath + "\\textures\\";
				std::string binpath = destpath + "\\3dfs\\";
				std::filesystem::create_directories(texturepath);
				std::filesystem::create_directories(binpath);

	//			std::cout << "Source Folder : " << sourcepath << std::endl;
	//			fout << std::endl;
	//			fout << "Source Folder : " << sourcepath << std::endl;
	//			fout << "Dest Folder : " << destpath << std::endl;
	//			fout << "Dest Texture path : " << texturepath << std::endl; 
	//			fout << "Dest 3dfs path : " << binpath << std::endl;
	//			fout << "Descriptor file : " << descriptor << std::endl;

				// search recursively for textures

				std::vector<std::string> foundtextures;

				try
				{
			//		fout << "Searching for textures" << "\n";

					for (std::filesystem::directory_entry entry : std::filesystem::recursive_directory_iterator(sourcepath))
					{

						if (std::filesystem::is_regular_file(entry.path()))
						{
							std::string ext = entry.path().extension().string();

							// accepted image file extensions are jpg , png

							if (ext == ".jpeg" || ext == ".png")
							{
								std::string filePath = entry.path().string();

								foundtextures.emplace_back(filePath);
							}
						}

					}
				}
				catch (const std::filesystem::filesystem_error& ex)
				{
					ImportError = "Exception: " + std::string(ex.what());
					return false;
				}

				// emit a warning if textures have not been found

				if (!foundtextures.size())
				{
					ImportError = "Warning : No Texture dir found";
				}
				else
				{
					if (Verbose)
					{
				//		std::cout << "found textures" << std::endl;

						for (size_t i = 0; i < foundtextures.size(); ++i)
						{
							std::cout << foundtextures[i] << std::endl;
						}
					}
				}

				// create object description file in json

				std::ofstream ofs(path);

				ofs << "{\n\n";
				ofs << "	\"Object3d\" :\n";
				ofs << "	{\n\n";
				ofs << "		\"Models\" :\n";
				ofs << "		[\n\n";

				for (unsigned int n = 0; n < scene->mNumMeshes; ++n)
				{

					const struct aiMesh* mesh = scene->mMeshes[n];

					// check if mesh is valid

					if (!mesh->HasPositions() || !mesh->HasFaces())
					{
						ImportError = "Mesh : " + std::string(name.C_Str()) + " has no valid data";

						// cleanup - calling 'aiReleaseImport' is important, as the library 
						// keeps internal resources until the scene is freed again. Not 
						// doing so can cause severe resource leaking.

						aiReleaseImport(scene);

						// We added a log stream to the library, it's our job to disable it
						// again. This will definitely release the last resources allocated
						// by Assimp.

						aiDetachAllLogStreams();

						return false;

					}

					name     = mesh->mName;
					vertices = mesh->mNumVertices;
					surfaces = mesh->mNumFaces;
		
					mesh->HasNormals()				 ? normalcoords = vertices : normalcoords = 0;
					mesh->HasTextureCoords(0)		 ? texcoords    = vertices : texcoords	  = 0;
					mesh->HasVertexColors(0)		 ? colorcoords  = vertices : colorcoords  = 0;
					mesh->HasTangentsAndBitangents() ? tangents		= vertices : tangents	  = 0;
					mesh->HasTangentsAndBitangents() ? bitangents	= vertices : bitangents   = 0;

					// move extracted textures to texture destination path

					std::vector<std::string> textures;

					int materialindex = mesh->mMaterialIndex;

					for (size_t j = 0; j < 32; ++j)
					{
						std::string sourcetexturename = extractedtexturefilenames[materialindex].Path[j];

						if (sourcetexturename.size() > 0)
						{

							auto it = foundtextures.begin();

							while (it != foundtextures.end())
							{

								std::string texturename = vml::strings::SplitPath::GetName(*it);

								if (texturename == vml::strings::SplitPath::GetName(sourcetexturename))
								{

									//	std::cout << "FOUND : " << (*it) << std::endl;
									//	std::cout << "MOVED TO : " << texturepath + texturename << std::endl;

									const auto copyOptions = std::filesystem::copy_options::update_existing;

									std::filesystem::copy((*it), texturepath + texturename, copyOptions);

									textures.emplace_back(texturename);

									// `erase()` invalidates the iterator, use returned iterator

									it = foundtextures.erase(it);
								}
								else
								{
									++it;
								}
							}
						}
					}

					// write json file

					ofs << "			{\n\n";

					// create transform parms

					ofs << "				\"Name\"   : \"" << name.C_Str() << "\",\n\n";
					ofs << "				\"Pos\"    :\n";
					ofs << "				[\n";
					ofs << "				   " << 0 << " , " << 0 << " , " << 0 << "\n";
					ofs << "				],\n\n";

					ofs << "				\"Scale\"  :\n";
					ofs << "				[\n";
					ofs << "					1,1,1\n";
					ofs << "				],\n\n";

					ofs << "				\"Angles\" :\n";
					ofs << "				[\n";
					ofs << "					0,0,0\n";
					ofs << "				],\n\n";

					// create material array

					ofs << "				\"Materials\" :\n";
					ofs << "				[\n";

					for (size_t i = 0; i < textures.size(); ++i)
					{
						if (i < textures.size() - 1)
							ofs << "					\"" << textures[i] << "\",\n";
						else
							ofs << "					\"" << textures[i] << "\" \n";
					}

					ofs << "				],\n\n";

					// create geometry parms

					ofs << "				\"Geometry\" :\n";
					ofs << "				[\n";
					ofs << "					\"" << name.C_Str() << ".3df\"\n";
					ofs << "				]\n\n";

					if (n < scene->mNumMeshes - 1)
						ofs << "			},\n\n";
					else
						ofs << "			}\n\n";

					// crete geometry data

					std::string binfilename = binpath + name.C_Str() + ".3df";
										
				//	fout << binfilename << "\n";
				//	fout << "Object name : " << binfilename << "\n"; 
				//	fout << "Vertices : " << vertices << "\n";
				//	fout << "Normals : " << normalcoords << "\n"; 
				//	fout << "Uvs : " << texcoords << "\n"; 
				//	fout << "ColorCoords : " << colorcoords << "\n"; 
				//	fout << "Tangents : " << tangents << "\n";

					// import mesh

					vml::meshes::MeshBuilder mb;

					mb.Begin(binfilename);

					for (size_t i = 0; i < vertices; ++i)
					{

						glm::vec3 vertex;
						glm::vec3 normal;
						glm::vec3 tangent;
						glm::vec3 bitangent;
						glm::vec4 color;
						glm::vec2 uv;

						vertex=glm::vec3(mesh->mVertices[i].x , mesh->mVertices[i].y , mesh->mVertices[i].z);

						if (normalcoords >0 && normalcoords ==vertices)
							normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
						else
							normal = glm::vec3(0, 0, 0);

						if (texcoords > 0 && texcoords == vertices)
							uv = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
						else 
							uv = glm::vec2(0, 0);

						if (colorcoords > 0 && colorcoords == vertices)
							color = glm::vec4(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a);
						else 
							color = glm::vec4(0, 0, 0, 1);

						if (tangents > 0 && tangents == vertices)
							tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
						else
							tangent = glm::vec3(0, 0, 0);

						if (tangents > 0 && tangents == vertices)
							bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
						else
							bitangent = glm::vec3(0, 0, 0);

						// add vertex to mesh

						mb.AddVertex(vertex, normal, uv, color);
					}

					// add surfaces

					for (unsigned int i = 0; i < surfaces; ++i)
					{
						const struct aiFace* face = &mesh->mFaces[i];
						
						mb.AddSurface(glm::ivec3(face->mIndices[0], face->mIndices[1], face->mIndices[2]));
					}

					// finalize 

					mb.Finalize(vml::meshes::MeshBuilder::CUSTOM_NORMALS | vml::meshes::MeshBuilder::RECENTER, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

					// save mesh

					mb.Save();

				}

				if (scene->mNumMeshes > 1)
				{
					ofs << "		],\n\n";
					ofs << "		\"Hierarchy\":\n";
					ofs << "		[\n\n";

					// take first mesh as root

					const struct aiMesh* mesh = scene->mMeshes[0];

					aiString rootname = mesh->mName;

					for (unsigned int n = 1; n < scene->mNumMeshes; ++n)
					{
						const struct aiMesh* mesh = scene->mMeshes[n];

						name = mesh->mName;

						ofs << "			{\n";

						ofs << "				\"";
						ofs << rootname.C_Str();
						ofs << "\"";
						ofs << " : ";
						ofs << "\"";
						ofs << name.C_Str();
						ofs << "\"";

						if ( n<scene->mNumMeshes-1)
							ofs << "\n			},\n\n";
						else
							ofs << "\n			}\n\n";

					}

					ofs << "		]\n\n";
				}
				else
				{
					ofs << "		]\n\n";
				}

				ofs << "	}\n";
				ofs << "}\n";
					
				// cleanup - calling 'aiReleaseImport' is important, as the library 
				// keeps internal resources until the scene is freed again. Not 
				// doing so can cause severe resource leaking.

				aiReleaseImport(scene);

				// We added a log stream to the library, it's our job to disable it
				// again. This will definitely release the last resources allocated
				// by Assimp.

				aiDetachAllLogStreams();

				// close stream

				ofs.close();

				return true;
			}
			
			// -----------------------------------------------------------------------
			// ctor / dtor

			ModelImporter3d()
			{
				Verbose = true;
			}

			~ModelImporter3d()
			{}

		};


		/////////////////////////////////////////////////////////////////////////

		class ObjectParser
		{
			private:
				
				std::string		LastError;
				std::string		FileName;
				
			public:
				
				// ------------------------------------------------------

				bool Load(const std::string& filename)
				{
					LastError = "No error";
					FileName = filename;
					const std::string in = vml::strings::StringUtils::LoadText(FileName);
					
					rapidjson::Document d;

					if (d.Parse(in.c_str()).HasParseError())
					{
						LastError = "Parsing error";
						return false;
					}
					
					std::cout << in << std::endl;

					return true;
				}
				
				// ------------------------------------------------------

				const std::string& GetLastError() const
				{
					return LastError;
				}
				
				// ------------------------------------------------------
				// ctor / dtor

				ObjectParser()
				{
					LastError = "No Error";
					FileName = "";
				}

				~ObjectParser()
				{
				}
			
		};

	}
}

