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
//	The above copyright notice and this permission notice shall be included in-
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#include <vml2.0\opengl\shaders\shaderstore.h>
namespace vml { extern vml::shaders::ShaderStore* ShaderStore; }

namespace vml
{
	namespace debugrendering
	{

			/////////////////////////////////////////////////////////////////
			// mesh class

			class CheckeredPlane
			{
				private:
	
					const unsigned int Vertices = 4;	// Vertices for this mesh
					const unsigned int Indices  = 6;	// vbo indices for surfaces
					
					// vertices

					inline static float VertexPositionArray[] =
					{
						 1 , 0 ,  1 , 1 ,
						 1 , 0 , -1 , 1 ,
						-1 , 0 , -1 , 1 ,
						-1 , 0 ,  1 , 1 ,
					};

					// normals

					inline static float VertexNormalArray[] =
					{
						 0 , 1 , 0 ,
						 0 , 1 , 0 ,
						 0 , 1 , 0 ,
						 0 , 1 , 0 ,
					};

					// uv coords

					inline static float VertexUVArray[] =
					{
						 0 , 0 ,
						 1 , 0 ,
						 1 , 1 ,
						 0 , 1 ,
					};

					// indices

					inline static const GLuint SurfaceIndices[] =
					{
						  0 , 1 , 3,
						  1 , 2 , 3,
					};

				public:
							
					GLuint							VAOid;								// vertex array object id
					glm::mat4						M, MV, MVP, V;
					glm::mat3						N;
					GLuint							BufferObjects[4];					// buffer objects
					GLuint							IndexBufferObject;					// surface index buffer object
					GLuint						    ModelViewProjectionMatrixLocation;
					GLuint							ViewMatrixLocation;
					GLuint							ProjectionMatrixLocation;
					GLuint							ModelMatrixLocation;
					GLuint							NormalMatrixLocation;
					GLuint							ModelViewMatrixLocation;
					GLuint							ShaderProgram;
					GLuint							ColorLocation1;
					GLuint							ColorLocation2;
					GLuint							CheckSize;
					vml::shaders::GlShaderProgram  *Shader;
				
				public:

					// ---------------------------------------------------------------
					// simple textured quad

					void Draw(vml::views::View* view,float scale, const glm::vec3& center, unsigned int n, const glm::vec4& c1, const glm::vec4& c2)
					{
						if (view)
						{

							float* mptr = glm::value_ptr(M);

							mptr[0] = scale;
							mptr[1] = 0;
							mptr[2] = 0;
							mptr[3] = 0;
							mptr[4] = 0;
							mptr[5] = scale;
							mptr[6] = 0;
							mptr[7] = 0;
							mptr[8] = 0;
							mptr[9] = 0;
							mptr[10] = scale;
							mptr[11] = 0;
							mptr[12] = center.x;
							mptr[13] = center.y;
							mptr[14] = center.z;
							mptr[15] = 1;
					
							MV = view->GetView() * M;
							MVP = view->GetViewProjection() * M;

							glUseProgram(ShaderProgram);
							glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, view->GetVptr());
							glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, view->GetPptr());
							glUniformMatrix4fv(ModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(M));
							glUniformMatrix4fv(NormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(N));
							glUniformMatrix4fv(ModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(MV));
							glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVP));

							glUniform1ui(CheckSize, n);
							glUniform4f(ColorLocation1, c1.r, c1.g, c1.b, 1);
							glUniform4f(ColorLocation2, c2.r, c2.g, c2.b, 1);

							glBindVertexArray(VAOid);
							glDisable(GL_CULL_FACE);
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

								glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);

							glEnable(GL_CULL_FACE);
							glBindVertexArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							glUseProgram(0);
						}
						else
						{
							vml::os::Message::Error("CheckeredPlane :","Null matrix cam for plane debug rendering");
						}
					}

					// ---------------------------------------------------------------
					// constructors / destructors

					CheckeredPlane()
					{

						VAOid			  = 0;
						BufferObjects[0]  = 0; 
						BufferObjects[1]  = 0;
						BufferObjects[2]  = 0;
						BufferObjects[3]  = 0;
						IndexBufferObject = 0;

						M   = glm::mat4(1.0f);
						N   = glm::mat3(1.0f);
						V   = glm::mat4(1.0f);
						MV  = glm::mat4(1.0f);
						MVP = glm::mat4(1.0f);

						// Create the VAO

						glGenVertexArrays(1, &VAOid);
						glBindVertexArray(VAOid);

						// Create the Vertex Buffer Object 

						// Map the generic vertex attributes used by the mesh's shader program to
						// the mesh's vertex data stored in the vertex buffer object.

						GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
						GLuint AttributeNormal   = vml::shaders::GlShaderProgram::ATTRIBUTE_NORMAL;
						GLuint AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;
						GLuint AttributeColor    = vml::shaders::GlShaderProgram::ATTRIBUTE_COLOR;
			
						glGenBuffers(1, &BufferObjects[0]);
						glGenBuffers(1, &BufferObjects[1]);
						glGenBuffers(1, &BufferObjects[2]);

						 // Generate and populate the buffers with vertex attributes and the indices

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0],GL_STATIC_DRAW);
						glEnableVertexAttribArray(AttributePosition);
						glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormalArray), &VertexNormalArray[0],	GL_STATIC_DRAW);
						glEnableVertexAttribArray(AttributeNormal);
						glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

						glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUVArray), &VertexUVArray[0], GL_STATIC_DRAW);
						glEnableVertexAttribArray(AttributeTexCoord);
						glVertexAttribPointer(AttributeTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
						
						// Create the Index Buffer Object 

						glGenBuffers(1, &IndexBufferObject);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(GLuint), &SurfaceIndices[0], GL_STATIC_DRAW);

						// unbinds buffers

						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

						glDisableVertexAttribArray(AttributePosition);
						glDisableVertexAttribArray(AttributeNormal);
						glDisableVertexAttribArray(AttributeTexCoord);
						glDisableVertexAttribArray(AttributeColor);

						std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\shaders";

						Shader=vml::ShaderStore->Load(shaderstorepath+"\\debug\\debug_checkered.shd");

						// no lit checkred plane shader locations

						ShaderProgram					  = Shader->GetID();
						ViewMatrixLocation				  = Shader->GetViewMatrixLocation();
						ProjectionMatrixLocation		  = Shader->GetProjectionMatrixLocation();
						ModelMatrixLocation				  = Shader->GetModelMatrixLocation();
						NormalMatrixLocation			  = Shader->GetNormalMatrixLocation();
						ModelViewMatrixLocation			  = Shader->GetModelViewMatrixLocation();
						ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
						
						// store locations for grid rednering
						glUseProgram(ShaderProgram);
						CheckSize						  = glGetUniformLocation(ShaderProgram, "chsize");
						ColorLocation1					  = glGetUniformLocation(ShaderProgram, "col1");
						ColorLocation2					  = glGetUniformLocation(ShaderProgram, "col2");
						glUseProgram(0);

					}

					~CheckeredPlane()
					{
						if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
						if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
						if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
						if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
						if (BufferObjects[2]) { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
						if (BufferObjects[3]) { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }

						std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\shaders";
						vml::ShaderStore->UnLoad(shaderstorepath + "\\debug\\debug_checkered.shd");

					}

			};

		} // end of debugrender namespace

} // end of vml namespace
