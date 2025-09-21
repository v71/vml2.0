#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
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
//	OUT OF OR IN CONNECTION WITH 7THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

///////////////////////////////////////////////////////////////////////////////////
// draw a wire cube for debu rendeirng purposes

namespace vml
{
	namespace debugrendering
	{

			/////////////////////////////////////////////////////////////////
			// debug rednering mesh class

			class WireCube
			{
				
				private:

					const static unsigned int Vertices = 8;		// Vertices for this mesh
					const static unsigned int Indices  = 16;	// vbo indices for surfaces

					// vertices for aligned cube ,these vertices are constant

					inline static float AABBoxVertexPositionArray[] =
					{
						-0.5f, -0.5f, -0.5f, 1,
						 0.5f, -0.5f, -0.5f, 1, 
						 0.5f,  0.5f, -0.5f, 1, 
						-0.5f,  0.5f, -0.5f, 1, 
						-0.5f, -0.5f,  0.5f, 1, 
						 0.5f, -0.5f,  0.5f, 1, 
						-0.5f,  0.5f,  0.5f, 1, 
						 0.5f,  0.5f,  0.5f, 1, 
					};
					
					// indices

					inline static GLuint SurfaceIndices[] =
					{
						0,1,2,3,4,5,7,6,0,4,1,5,2,7,3,6
					};
					
					// vertices for axis oriented bounding box
					// these vertices will be overwritten using
					// actual vertices coordinates

					inline static float AOBBoxVertexPositionArray[] =
					{
						-0.5f, -0.5f, -0.5f, 1,
						 0.5f, -0.5f, -0.5f, 1,
						 0.5f,  0.5f, -0.5f, 1,
						-0.5f,  0.5f, -0.5f, 1,
						-0.5f, -0.5f,  0.5f, 1,
						 0.5f, -0.5f,  0.5f, 1,
						-0.5f,  0.5f,  0.5f, 1,
						 0.5f,  0.5f,  0.5f, 1,
					};

					// vaoid for axis aligned bounding box ( aabbox )

					GLuint								AABBoxVAOid;						// vertex array object id for axis aligned bounding box
					GLuint								AABBoxBufferObject;					// vertex buffer object for axis aligned bounding box
					GLuint								AABBoxIndexBufferObject;			// surface index buffer object for axis aligned bounding box

					// vaoid for axis oriented bounding box ( aobbox )

					GLuint							    AOBBoxVAOid;						// vertex array object id for axis oriented bounding box
					GLuint							    AOBBoxBufferObject;					// vertex buffer object for axis oriented bounding box
					GLuint								AOBBoxIndexBufferObject;			// surface index buffer object for axis oriented bounding box

					GLuint								ColorLocation;
					GLuint							    ModelViewProjectionMatrixLocation;
					GLuint								ShaderProgram;
					vml::shaders::GlShaderProgram	   *Shader;

				public:
					
					// ---------------------------------------------------------------
					// axis aligned bounding box drawing

					void Draw(vml::views::View* view,
							  const glm::vec3 &bmin, const glm::vec3 &bmax,
							  const glm::vec4 &col,
							  bool drawpoints = true )
					{
						if (view)
						{

							glm::mat4 m;

							float *mptr = glm::value_ptr(m);
						
							// scale parent matrix

							mptr[ 0] = (bmax.x - bmin.x);
							mptr[ 1] = 0;
							mptr[ 2] = 0;
							mptr[ 3] = 0;
							mptr[ 4] = 0;
							mptr[ 5] = (bmax.y - bmin.y);
							mptr[ 6] = 0;
							mptr[ 7] = 0;
							mptr[ 8] = 0;
							mptr[ 9] = 0;
							mptr[10] = (bmax.z - bmin.z);
							mptr[11] = 0;
							mptr[12] = (bmax.x + bmin.x)*0.5f;
							mptr[13] = (bmax.y + bmin.y)*0.5f;
							mptr[14] = (bmax.z + bmin.z)*0.5f;
							mptr[15] = 1;

							glm::mat4 mvp = view->GetViewProjection() * m;

							// Use debug shader

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
							glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							glDisable(GL_CULL_FACE);
							glBindVertexArray(AABBoxVAOid);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
							glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							glUseProgram(0);

							if (drawpoints)
							{
								glUseProgram(ShaderProgram);
								glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
								glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
								glDisable(GL_CULL_FACE);
								glBindVertexArray(AABBoxVAOid);
								glPointSize(5.0f);
								glDrawArrays(GL_POINTS, 0, Vertices);
								glEnable(GL_CULL_FACE);
								glBindVertexArray(0);
								glBindBuffer(GL_ARRAY_BUFFER, 0);
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
								glUseProgram(0);
							}

						}
						else
						{
							vml::os::Message::Error("Cube : ","Null matrix cam for cube rendering");
						}
					}

					// ---------------------------------------------------------------
					// axis aligned bounding box drawing

					void Draw(vml::views::View* view, const glm::mat4& m, const glm::vec4& col, bool drawpoints = true)
					{
						if (view)
						{

							glm::mat4 mvp = view->GetViewProjection() * m;

							// Use debug shader

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
							glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							glDisable(GL_CULL_FACE);
							glBindVertexArray(AABBoxVAOid);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
							glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							glUseProgram(0);

							if (drawpoints)
							{
								glUseProgram(ShaderProgram);
								glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
								glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
								glDisable(GL_CULL_FACE);
								glBindVertexArray(AABBoxVAOid);
								glPointSize(5.0f);
								glDrawArrays(GL_POINTS, 0, Vertices);
								glEnable(GL_CULL_FACE);
								glBindVertexArray(0);
								glBindBuffer(GL_ARRAY_BUFFER, 0);
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
								glUseProgram(0);
							}

						}
						else
						{
							vml::os::Message::Error("Cube : ", "Null matrix cam for cube rendering");
						}
					}

					// ---------------------------------------------------------------

					void Draw( const glm::vec3& a0, const glm::vec3& a1, const glm::vec3& a2, const glm::vec3& a3,
							   const glm::vec3& a4, const glm::vec3& a5, const glm::vec3& a6, const glm::vec3& a7,
							   const glm::vec4& col,
							   vml::views::View* view,
							   bool drawpoints = true)
					{
						
						if (view)
						{

							// vertices for aligned cube ,these vertices are constant
							
							AOBBoxVertexPositionArray[ 0] = a0.x;	AOBBoxVertexPositionArray[ 1] = a0.y;	AOBBoxVertexPositionArray[ 2] = a0.z; AOBBoxVertexPositionArray[ 3] = 1;
							AOBBoxVertexPositionArray[ 4] = a1.x;	AOBBoxVertexPositionArray[ 5] = a1.y;	AOBBoxVertexPositionArray[ 6] = a1.z; AOBBoxVertexPositionArray[ 7] = 1;
							AOBBoxVertexPositionArray[ 8] = a2.x;	AOBBoxVertexPositionArray[ 9] = a2.y;	AOBBoxVertexPositionArray[10] = a2.z; AOBBoxVertexPositionArray[11] = 1;
							AOBBoxVertexPositionArray[12] = a3.x;	AOBBoxVertexPositionArray[13] = a3.y;	AOBBoxVertexPositionArray[14] = a3.z; AOBBoxVertexPositionArray[15] = 1;
							AOBBoxVertexPositionArray[16] = a4.x;	AOBBoxVertexPositionArray[17] = a4.y;	AOBBoxVertexPositionArray[18] = a4.z; AOBBoxVertexPositionArray[19] = 1;
							AOBBoxVertexPositionArray[20] = a5.x;	AOBBoxVertexPositionArray[21] = a5.y;	AOBBoxVertexPositionArray[22] = a5.z; AOBBoxVertexPositionArray[23] = 1;
							AOBBoxVertexPositionArray[24] = a6.x;	AOBBoxVertexPositionArray[25] = a6.y;	AOBBoxVertexPositionArray[26] = a6.z; AOBBoxVertexPositionArray[27] = 1;
							AOBBoxVertexPositionArray[28] = a7.x;	AOBBoxVertexPositionArray[29] = a7.y;	AOBBoxVertexPositionArray[30] = a7.z; AOBBoxVertexPositionArray[31] = 1;

							glm::mat4 m;

							float* mptr = glm::value_ptr(m);

							// scale parent matrix

							mptr[ 0] = 1;
							mptr[ 1] = 0;
							mptr[ 2] = 0;
							mptr[ 3] = 0;
							mptr[ 4] = 0;
							mptr[ 5] = 1;
							mptr[ 6] = 0;
							mptr[ 7] = 0;
							mptr[ 8] = 0;
							mptr[ 9] = 0;
							mptr[10] = 1;
							mptr[11] = 0;
							mptr[12] = 0;
							mptr[13] = 0;
							mptr[14] = 0;
							mptr[15] = 1;

							glm::mat4 mvp = view->GetViewProjection() * m;

							glBindVertexArray(AOBBoxVAOid);
						
							// Buffer orphaning, a common way to improve streaming perf. 

							glBindBuffer(GL_ARRAY_BUFFER, AOBBoxBufferObject);
							glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(AABBoxVertexPositionArray), &AOBBoxVertexPositionArray[0]);

							// 2nd attribute buffer : positions of particles' centers

							glEnableVertexAttribArray(vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION);
							
							// forse si puo' togliere

							glBindBuffer(GL_ARRAY_BUFFER, AOBBoxBufferObject);
							glVertexAttribPointer(vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION,        // attribute. No particular reason for 1, but must match the layout in the shader.
									 			  4,														// size : x + y + z 
												  GL_FLOAT,													// type
												  GL_FALSE,													// normalized?
												  0,														// stride
												  (void*)0													// array buffer offset
												 );

							// use shader

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
							glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							glDisable(GL_CULL_FACE);
							glBindVertexArray(AOBBoxVAOid);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
							glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
							glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							glUseProgram(0);

							if (drawpoints)
							{
								glUseProgram(ShaderProgram);
								glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
								glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
								glDisable(GL_CULL_FACE);
								glBindVertexArray(AOBBoxVAOid);
								glPointSize(5.0f);
								glDrawArrays(GL_POINTS, 0, Vertices);
								glEnable(GL_CULL_FACE);
								glBindVertexArray(0);
								glBindBuffer(GL_ARRAY_BUFFER, 0);
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
								glUseProgram(0);
							}

						}
						else
						{
							vml::os::Message::Error("Cube : ", "Null matrix cam for cube rendering");
						}
						
					}

					// ---------------------------------------------------------------
					// constructors / destructors

					WireCube()
					{
						AOBBoxVAOid						  = 0;
						AOBBoxBufferObject				  = 0;		
						AOBBoxIndexBufferObject			  = 0;
						AABBoxVAOid						  = 0;
						AABBoxIndexBufferObject			  = 0;
						AABBoxBufferObject				  = 0;
						AABBoxIndexBufferObject			  = 0;	
						ColorLocation					  = 0;
						ModelViewProjectionMatrixLocation = 0;
						ShaderProgram					  = 0;
						Shader							  = nullptr;

						// Create the VAO for axis aligned bounding box

						glGenVertexArrays(1, &AABBoxVAOid);
						glBindVertexArray(AABBoxVAOid);

						// Create the Vertex Buffer Object 

						glGenBuffers(1, &AABBoxBufferObject);

						GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

						glBindBuffer(GL_ARRAY_BUFFER, AABBoxBufferObject);
						glBufferData(GL_ARRAY_BUFFER, sizeof(AABBoxVertexPositionArray), &AABBoxVertexPositionArray[0], GL_STATIC_DRAW);
						glEnableVertexAttribArray(AttributePosition);
						glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

						// Create the Index Buffer Object 

						glGenBuffers(1, &AABBoxIndexBufferObject);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AABBoxIndexBufferObject);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SurfaceIndices), &SurfaceIndices[0], GL_STATIC_DRAW);

						// unbinds buffers

						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

						// vao for axis oriented bounding box vertices cube

						glGenVertexArrays(1, &AOBBoxVAOid);
						glBindVertexArray(AOBBoxVAOid);
						glGenBuffers(1, &AOBBoxBufferObject);
						glBindBuffer(GL_ARRAY_BUFFER, AOBBoxBufferObject);
						// Initialize with empty (NULL) buffer : it will be updated later, each frame.
						glBufferData(GL_ARRAY_BUFFER, sizeof(AABBoxVertexPositionArray), NULL, GL_STREAM_DRAW);
						// Create the Index Buffer Object 
						glGenBuffers(1, &AOBBoxIndexBufferObject);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AOBBoxIndexBufferObject);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SurfaceIndices), &SurfaceIndices[0], GL_STREAM_DRAW);

						// attach shader

						std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\shaders";

						Shader = vml::ShaderStore->Load(shaderstorepath + "\\debug\\debug_single_color.shd");
						ShaderProgram = Shader->GetID();
						glUseProgram(ShaderProgram);
						ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
						ColorLocation = glGetUniformLocation(ShaderProgram, "Color");
						glUseProgram(0);

					}

					~WireCube()
					{
						if (AABBoxVAOid) { glDeleteVertexArrays(1, &AABBoxVAOid); AABBoxVAOid = 0; }
						if (AABBoxIndexBufferObject) { glDeleteBuffers(1, &AABBoxIndexBufferObject); AABBoxIndexBufferObject = 0; }
						if (AABBoxBufferObject) { glDeleteBuffers(1, &AABBoxBufferObject); AABBoxBufferObject = 0; }
						if (AOBBoxVAOid) { glDeleteVertexArrays(1, &AOBBoxVAOid); AOBBoxVAOid = 0; }
						if (AOBBoxIndexBufferObject) { glDeleteBuffers(1, &AOBBoxIndexBufferObject); AOBBoxIndexBufferObject = 0; }
						if (AOBBoxBufferObject) { glDeleteBuffers(1, &AOBBoxBufferObject); AOBBoxBufferObject = 0; }
						std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\shaders";
						vml::ShaderStore->UnLoad(shaderstorepath + "\\debug\\debug_single_color.shd");
					}

			};

	} // end of debugrednering namespace

} // end of vml namespace

namespace vml
{
	namespace debugrendering
	{

		class SolidCube 
		{
			
			private:
				
				unsigned int  Vertices;				// Vertices for this mesh
				unsigned int  Indices;				// vbo indices for surfaces
				GLuint		  VAOid;				// vertex array object id
				GLuint		  IndexBufferObject;	// surface index buffer object
				GLuint		  BufferObjects[4];		// buffer objects
				
			public:
			
				// ---------------------------------------------------------------

				GLuint						     GetVAOId()			 const { return VAOid; }											// get mesh vao id
				unsigned int				     GetIndicesCount()	 const { return vml::meshes::procedural::cube::Surfaces*3; }	// get mesh indices array

				// ---------------------------------------------------------------
				// constructors / destructors

				SolidCube() 
				{
					VAOid             = 0;
					IndexBufferObject = 0;
					BufferObjects[0]  = 0;
					BufferObjects[1]  = 0;
					BufferObjects[2]  = 0;
					BufferObjects[3]  = 0;

					Vertices = vml::meshes::procedural::cube::Vertices;
					Indices  = vml::meshes::procedural::cube::Surfaces;
					
					int verts = Vertices * 4;
					int nverts = Vertices * 3;
					int indices = Indices * 3;


					const float* varray  = vml::meshes::procedural::cube::VertexPositionArray;
					const float* narray  = vml::meshes::procedural::cube::VertexNormalArray;
					const float* uvarray = vml::meshes::procedural::cube::VertexUVArray;
					const unsigned int* indicesarray = vml::meshes::procedural::cube::TrianglesArray;

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
					GLuint AttributeNormal = vml::shaders::GlShaderProgram::ATTRIBUTE_NORMAL;
					GLuint AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;
					GLuint AttributeColor = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					glGenBuffers(1, &BufferObjects[0]);
					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, (long long)Vertices * 4 * sizeof(float), &varray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glGenBuffers(1, &BufferObjects[1]);
					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
					glBufferData(GL_ARRAY_BUFFER, (long long)Vertices * 3 * sizeof(float), &narray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeNormal);
					glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long long)Indices * 3 * sizeof(unsigned int), &indicesarray[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				}

				// destructor

				~SolidCube()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					if (BufferObjects[2]) { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
					if (BufferObjects[3]) { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }
				}

		};

	} // end of debugrednering namespace

} // end of vml namespace
