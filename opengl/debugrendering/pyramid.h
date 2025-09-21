#pragma once


namespace vml
{
	namespace debugrendering
	{
		class Pyramid
		{

			private:
				
				unsigned int  Vertices;				// Vertices for this mesh
				unsigned int  Indices;				// vbo indices for surfaces
				GLuint		  VAOid;				// vertex array object id
				GLuint		  IndexBufferObject;	// surface index buffer object
				GLuint		  BufferObjects[4];		// buffer objects

			public:

				// ---------------------------------------------------------------

				GLuint					     GetVAOId()			 const { return VAOid; }											// get mesh vao id
				unsigned int			     GetIndicesCount()	 const { return vml::meshes::procedural::pyramid::Surfaces * 3; }	// get mesh indices array

				// ---------------------------------------------------------------
				// constructors / destructors

				Pyramid()
				{
					VAOid             = 0;
					IndexBufferObject = 0;
					BufferObjects[0]  = 0;
					BufferObjects[1]  = 0;
					BufferObjects[2]  = 0;
					BufferObjects[3]  = 0;

					Vertices = vml::meshes::procedural::pyramid::Vertices;
					Indices = vml::meshes::procedural::pyramid::Surfaces;

					const float* varray = vml::meshes::procedural::pyramid::VertexPositionArray;
					const float* narray = vml::meshes::procedural::pyramid::VertexNormalArray;
					const unsigned int* trianglesarray = vml::meshes::procedural::pyramid::TrianglesArray;

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
					glBufferData(GL_ARRAY_BUFFER, (long long)Vertices* 3 * sizeof(float), &narray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributeNormal);
					glVertexAttribPointer(AttributeNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

					// Create the Index Buffer Object

					glGenBuffers(1, &IndexBufferObject);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferObject);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long long)Indices * 3 * sizeof(unsigned int), &trianglesarray[0], GL_STATIC_DRAW);

					// unbinds buffers

					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					
				}

				// destructor

				~Pyramid()
				{
					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					if (BufferObjects[2]) { glDeleteBuffers(1, &BufferObjects[2]); BufferObjects[2] = 0; }
					if (BufferObjects[3]) { glDeleteBuffers(1, &BufferObjects[3]); BufferObjects[3] = 0; }
				}

		};

	} // end of meshes namespace

} // end of vml namespace
