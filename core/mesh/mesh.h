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
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <vml2.0/math/3d/aabbox.h>
#include <vml2.0/opengl/shaders/shader.h>

//////////////////////////////////////////////////////////////////////////////
// 

namespace vml
{
	namespace meshes
	{

		/////////////////////////////////////////////////////////////////
		// mesh class

		class Mesh3d : public vml::resources::SharedResource
		{

			private:

				unsigned int				Surfaces;				// Triangles for this mesh
				unsigned int				Vertices;				// Vertices for this mesh
				unsigned int				Indices;				// vbo indices for surfaces
				vml::geo3d::AABBox			BoundingBox;			// bounding box
				float	 					Radius;					// radius
				GLuint						VAOid;					// vertex array object id
				GLuint						IndexBufferObject;		// surface index buffer object
				GLuint						BufferObjects[8];		// buffer objects
				unsigned int				Flags;
				std::vector<float>			VertexArray;			
				std::vector<float>			NormalArray;
				std::vector<float>			UVArray;
				std::vector<float>			ColorArray;
				std::vector<unsigned int>	SurfaceIndices;
				
				// ---------------------------------------------------------------
				// Memory clearing :
				// Resets all data to initial values (0) and
				// clears vertex, surface and surface indices arrays
				// releases also vao(s).

				void ReleaseAll()
				{
					// sets data members to initial values

					Surfaces    = 0;
					Vertices    = 0;
					Indices     = 0;
					Radius      = 0;
					BoundingBox = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

					VertexArray.clear();
					NormalArray.clear();
					UVArray.clear();
					ColorArray.clear();
					SurfaceIndices.clear();

					// delete vbo

					if (VAOid)			   { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0])  { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1])  { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					if (BufferObjects[2])  { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
					if (BufferObjects[3])  { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }
					if (BufferObjects[4])  { glDeleteBuffers(1, &BufferObjects[4]); BufferObjects[4] = 0; }
					if (BufferObjects[5])  { glDeleteBuffers(1, &BufferObjects[5]); BufferObjects[5] = 0; }
					if (BufferObjects[6])  { glDeleteBuffers(1, &BufferObjects[6]); BufferObjects[6] = 0; }
					if (BufferObjects[7])  { glDeleteBuffers(1, &BufferObjects[7]); BufferObjects[7] = 0; }

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Relasing Mesh Instance", ResourceFileName, "Done");
				}

				// ---------------------------------------------------------------
				// vbo creation

				void CreateVBO()
				{
					// Create the vertex array object for the mesh.
					//
					// The buffer objects that are to be used by the vertex stage of the GL
					// are collected together to form a vertex array object. All state related
					// to the definition of data used by the vertex processor is encapsulated
					// in a vertex array object.
					//
					// Vertex array objects maintain state. Any required vertex and index
					// buffer objects are created and bound to the vertex array object. Then
					// any vertex shader generic shader input variables are mapped to the
					// vertex data stored in the vertex buffer objects bound to the vertex
					// array object. All of this only needs to be done once when using vertex
					// array objects.

					// Create the VAO

					// Map the generic vertex attributes used by the mesh's shader program to
					// the mesh's vertex data stored in the vertex buffer object.

					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
					GLuint AttributeNormal   = vml::shaders::GlShaderProgram::ATTRIBUTE_NORMAL;
					GLuint AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;
					GLuint AttributeColor    = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);
										
					// Create the Vertex Buffer Object 

					glGenBuffers(1, &BufferObjects[0]);
					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, (VertexArray.size()) * sizeof(float), &VertexArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glGenBuffers(1, &BufferObjects[1]);
					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, (NormalArray.size()) * sizeof(float), &NormalArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeNormal);
					glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(unsigned int), &SurfaceIndices[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					
				}

				// ---------------------------------------------------------------
				// load mesh , path is embedded into filename ,no args needed

				bool Load(const std::string& resourcefilename)
				{
					// validate extension

					if ( !resourcefilename.ends_with(".3df"))
						vml::os::Message::Error("Mesh3d : ","Wrong extension ' ", resourcefilename.c_str()," '");

					//		vml::os::Trace("%s\n",resourcefilename.c_str());

					FILE* stream;

					// models are loaded according to the model's path

					errno_t err = fopen_s(&stream, resourcefilename.c_str(), "rb");

					if (err == 0)
					{
						unsigned int flags;
						unsigned int vertices;
						unsigned int indices;
						float radius;
						float Min[4];
						float Max[4];

						// clears everything before loading

						ReleaseAll();

						// read number of vertices

						fread(&vertices, sizeof(unsigned int), 1, stream);

						VertexArray.resize((size_t)vertices * 4);
						NormalArray.resize((size_t)vertices * 3);
						UVArray.resize((size_t)vertices * 2);
						ColorArray.resize((size_t)vertices * 4);

						// read data

						for (size_t i = 0; i < vertices; i++)
						{

							float p[4];
							float n[3];
							float c[4];
							float t[2];

							fread(&p, sizeof(float), 4, stream);
							fread(&n, sizeof(float), 3, stream);
							fread(&t, sizeof(float), 2, stream);
							fread(&c, sizeof(float), 4, stream);

							size_t offset;

							offset = i * 4;

							VertexArray[offset    ] = p[0];
							VertexArray[offset + 1] = p[1];
							VertexArray[offset + 2] = p[2];
							VertexArray[offset + 3] = 1.0f;

					//		std::cout << i << " " << p[0] << " " << p[1] << " " << p[2] << std::endl;
							offset = i * 3;

							NormalArray[offset    ] = n[0];
							NormalArray[offset + 1] = n[1];
							NormalArray[offset + 2] = n[2];

							offset = i * 2;

							UVArray[offset    ] = t[0];
							UVArray[offset + 1] = t[1];

							offset = i * 4;

							ColorArray[offset    ] = c[0];
							ColorArray[offset + 1] = c[1];
							ColorArray[offset + 2] = c[2];
							ColorArray[offset + 3] = c[3];

							//	vml::os::Trace("AddVertex(glm::vec3 (%f,%f,%f),glm::vec3 (%f,%f,%f),glm::vec2(%f,%f),glm::vec4(%f,%f,%f,%f)\n",
							//					p[0], p[1], p[2], 
							//					n[0], n[1], n[2],
							//					t[0], t[1],
							//					c[0], c[1], c[2], c[3]);

							// increase vertex counter

							Vertices++;

						}

						// read surface indices for vbo indexing

						fread(&indices, sizeof(unsigned int), 1, stream);

						Indices = indices;

						Surfaces = Indices / 3;

						// read surfaces

						SurfaceIndices.resize(indices);

						fread(&SurfaceIndices[0], sizeof(int), indices, stream);

						// read metrics

						fread(&Min, sizeof(float), 3, stream);
						fread(&Max, sizeof(float), 3, stream);
						fread(&radius, sizeof(float), 1, stream);

						//read mesh flags

						fread(&flags, sizeof(unsigned int), 1, stream);

						// fill metrics data

						BoundingBox.Set(glm::vec3(Min[0], Min[1], Min[2]), glm::vec3(Max[0], Max[1], Max[2]));

						Radius = radius;

						// create vbos

						CreateVBO();

						// once the data is uploaded into the gpu , it should be rleased
						// see the meshsotre class, here we *DO NOT* release the data
						// since the data itslef might be needed for further computation
						// see the meshsotre clss where the data is explicilty releasde
					
						// close stream

						if (fclose(stream) != 0)
							vml::os::Message::Error("Mesh3d : ","Cannot close ' ", resourcefilename.c_str()," '");

						return true;
					}
					else
					{
						vml::os::Message::Error("Mesh3d : ", "Cannot load mesh ' ", resourcefilename.c_str(), " '"," ( error code : ", err," )");
					}

					return false;
				}

		public:

				// ---------------------------------------------------------------
				//	query functions

				// get mesh Vertex array size

				unsigned int				     GetVertexCount()	 const { return Vertices; }												// get mesh Vertex array size
				float						     GetRadius()		 const { return Radius; }												// get mesh bounding sphere radius
				unsigned int				     GetIndicesCount()	 const { return Indices; }												// get mesh indices array
				unsigned int				     GetSurfaceCount()	 const { return Surfaces; }												// gets number of surfaces
				GLuint						     GetVAOId()			 const { return VAOid; }												// get mesh vao id
				bool							 IsValid()			 const { return Vertices> 0 && Surfaces> 0; }
				const vml::geo3d::AABBox		&GetBoundingBox()	 const { return BoundingBox; }
				const std::vector<float>		&GetVertexArray()	 const { return VertexArray; }
				const std::vector<float>		&GetNormalArray()	 const { return NormalArray; }
				const std::vector<float>		&GetUVArray()		 const { return UVArray; }
				const std::vector<float>		&GetColorArray()	 const { return ColorArray; }
				const std::vector<unsigned int> &GetSurfaceIndices() const { return SurfaceIndices; }
				
				// ---------------------------------------------------------------
				// this function clear the data buffer once the VAO are created
				// since we might need to preserve the data in some cases 
				// we explicilty delete the buffer when the meshstore 
				// loads this class instance, see the meshstore for more info
				
				void ReleaseVertexBufferData()
				{
					if (!VAOid)
						vml::os::Message::Error("Mesh3d : ","Cannot delete buffers if VAO has not been created yet");
					VertexArray.clear();
					NormalArray.clear();
					UVArray.clear();
					ColorArray.clear();
					SurfaceIndices.clear();
					Vertices = 0;
					Surfaces = 0;
				}
				
				// ---------------------------------------------------------------
				// dumps mesh 

				const std::string Dump(bool full=false)
				{
					std::string text;
					text =  std::format("Vertices : {}\n", Vertices);
					text += std::format("Surfaces : {}\n", Surfaces);
					text += std::format("Indices  : {}\n", Indices);
					text += std::format("Bounding box minimum : {} , {} , {}\n", BoundingBox.GetMin().x, BoundingBox.GetMin().y, BoundingBox.GetMin().z);
					text += std::format("Bounding box maximum : {} , {} , {}\n", BoundingBox.GetMax().x, BoundingBox.GetMax().y, BoundingBox.GetMax().z);
					text += std::format("Bounding box extents : {} , {} , {}\n", BoundingBox.GetExtents().x, BoundingBox.GetExtents().y, BoundingBox.GetExtents().z);
					text += std::format("Radius : {}\n", Radius);
					
					if (full)
					{
						// vertex and surfaces count

						text += std::format("const unsigned int Vertices = {};\n", Vertices);
						text += std::format("const unsigned int Indices = {};\n", Indices );

						text += "inline static float VertexPositionArray[] =\n";
						text += "{\n";
					
						for (size_t i = 0; i < Vertices*4; i+=4)
						{
							text += std::format("{} , {} , {} , {} ,\n", VertexArray[i], VertexArray[i+1], VertexArray[i+2], VertexArray[i+3]);
						}
						text += "};\n";

						text += "inline static unsigned int TrianglesArray[] =\n";
						text += "{\n";

						for (size_t i = 0; i < Indices / 3; ++i)
						{
							size_t offset = i * 3;
							text += std::format("{} , {} , {} ,\n", SurfaceIndices[offset], SurfaceIndices[offset + 1], SurfaceIndices[offset + 2]);
						}

						text += "};\n";
					}

					return text;
				}

				//-----------------------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Mesh3d(Mesh3d& mesh) = delete;

				//-----------------------------------------------------------------------------------
				// overload operator is private,
				// no copies allowed since classes
				// are referenced

				void operator=(const Mesh3d& mesh) = delete;

				// ---------------------------------------------------------------
				// constructors / destructors

				Mesh3d(const std::string& resourcefilename) : vml::resources::SharedResource(resourcefilename)
				{
					Surfaces           = 0;
					Vertices           = 0;
					Indices            = 0;
					Flags			   = 0;
					Radius             = 0;
					BoundingBox		   = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					VAOid              = 0;
					IndexBufferObject  = 0;
					BufferObjects[0]   = 0;
					BufferObjects[1]   = 0;
					BufferObjects[2]   = 0;
					BufferObjects[3]   = 0;
					BufferObjects[4]   = 0;
					BufferObjects[5]   = 0;
					BufferObjects[6]   = 0;
					BufferObjects[7]   = 0;

					// cleans everything and uses raii paradigm to load mesh resource

					Load(resourcefilename);
				}

				// destructor

				~Mesh3d()
				{
					ReleaseAll();
				}

		};

	} // end of meshes namespace

} // end of vml namespace


