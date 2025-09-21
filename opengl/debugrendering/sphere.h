#pragma once

//////////////////////////////////////////////////////////////////////////////
// TO DO :

namespace vml
{
	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// mesh class

		class Sphere
		{

			private:

				float						   Radius;								// radius
				vml::geo3d::AABBox			   BoundingBox;							// bounding box
				GLuint						   VAOid;								// vertex array object id
				GLuint						   BufferObject;						// buffer objects
				GLuint						   IndexBufferObject;					// surface index buffer object
				GLuint						   ColorLocation;
				GLuint						   ModelViewProjectionMatrixLocation;
				GLuint						   ShaderProgram;
				vml::shaders::GlShaderProgram* Shader;
				int						       Vertices;
				int							   Indices;

			public:
			
				/////////////////////////////////////////////////////////////////////////////////////////
				// draw bounding sphere for model's object
			
				void Draw(vml::views::View *view,
						  const glm::vec3 &pos,
						  const float radius,
						  const glm::vec4 &col)
				{
					if (view)
					{
					
						glm::mat4 m;

						float* mptr = glm::value_ptr(m);

						// scale parent matrix
				
						mptr[0] = radius*2;
						mptr[1] = 0;
						mptr[2] = 0;
						mptr[3] = 0;
						mptr[4] = 0;
						mptr[5] = radius*2;
						mptr[6] = 0;
						mptr[7] = 0;
						mptr[8] = 0;
						mptr[9] = 0;
						mptr[10] = radius*2;
						mptr[11] = 0;
						mptr[12] = pos.x;
						mptr[13] = pos.y;
						mptr[14] = pos.z;
						mptr[15] = 1;

						glm::mat4 mvp = view->GetViewProjection() * m;

						// Use debug shader

						glUseProgram(ShaderProgram);
						glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvp));
						glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);

						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						glDisable(GL_CULL_FACE);
						glBindVertexArray(VAOid);
						glDrawElements(
										GL_TRIANGLES,		// mode
										Indices,			// count
										GL_UNSIGNED_INT,	// type
										(void*)0			// element array buffer offset
									);

						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);
					
					}
					else
					{
						vml::os::Message::Error("Sphere : ","Null matrix cam for cube rendering");
					}
				}

				/////////////////////////////////////////////////////////////////////////////////////////
				// draw bounding sphere for model's object

				void Draw(vml::views::View* view,
						  const glm::mat4& m,
						  const glm::vec4& col)
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
						glBindVertexArray(VAOid);
						glDrawElements(
							GL_TRIANGLES,		// mode
							Indices,			// count
							GL_UNSIGNED_INT,	// type
							(void*)0			// element array buffer offset
						);

						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glUseProgram(0);

					}
					else
					{
						vml::os::Message::Error("Sphere : ","Null matrix cam for cube rendering");
					}
				}

				// ---------------------------------------------------------------
				// constructors / destructors

				Sphere()
				{
					Radius			   = 1;		
					BoundingBox		   = vml::geo3d::AABBox( glm::vec3(-0.5, -0.5, -0.5),glm::vec3(0.5, 0.5, 0.5));
				
					VAOid			  = 0;
					IndexBufferObject = 0;
					BufferObject	  = 0;

					Vertices = vml::meshes::procedural::debugsphere::Vertices;
					Indices = vml::meshes::procedural::debugsphere::Indices;

					const float* varray = vml::meshes::procedural::debugsphere::VertexPositionArray;
					const unsigned int* trianglesarray = vml::meshes::procedural::debugsphere::TrianglesArray;

					// Create the VAO for axis aligned bounding box

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					glGenBuffers(1, &BufferObject);

					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;

					glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
					glBufferData(GL_ARRAY_BUFFER, (long long)Vertices * 4 * sizeof(float), &varray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object 

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices * sizeof(GLuint), &trianglesarray[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);

					// attach shader

					std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\shaders";

					Shader = vml::ShaderStore->Load(shaderstorepath + "\\debug\\debug_single_color.shd");
					ShaderProgram = Shader->GetID();
					glUseProgram(ShaderProgram);
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					ColorLocation = glGetUniformLocation(ShaderProgram, "Color");
					glUseProgram(0);
					
				}

				// destructor

				~Sphere()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObject) { glDeleteBuffers(1, &BufferObject); BufferObject = 0; }
					std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\shaders";
					vml::ShaderStore->UnLoad(shaderstorepath + "\\debug\\debug_single_color.shd");
				}

		};

	} // end of meshes namespace

} // end of vml namespace
