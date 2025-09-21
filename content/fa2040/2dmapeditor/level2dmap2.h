#pragma once

#include <vml2.0/content/fa2040/2dmapeditor/gridtriangle.h>
#include <vml2.0/content/fa2040/2dmapeditor/gridblock.h>
#include <vml2.0/content/fa2040/2dmapeditor/trianglelayerrender.h>
#include <vml2.0/content/fa2040/2dmapeditor/borderlayer.h>
#include <vml2.0/content/fa2040/2dmapeditor/trianglelayer.h>
#include <vml2.0/content/fa2040/2dmapeditor/level2dgenerator.h>

namespace fa2040
{
	namespace tools
	{
		class Level2dMap
		{

			private:
				
				const unsigned int Vertices = 4;	// Vertices for this mesh
				const unsigned int Indices  = 6;	// vbo indices for surfaces

				// vertices

				inline static float VertexPositionArray[] =
				{
					-1 ,  1 ,  0 , 1 ,
					 1 ,  1 ,  0 , 1 ,
					 1 , -1 ,  0 , 1 ,
					-1 , -1 ,  0 , 1 ,
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

				inline static GLuint SurfaceIndices[] =
				{
					  0 , 1 , 3,
					  1 , 2 , 3,
				};

				// private data
				
				GLuint						   VAOid;									// vertex array object id
				GLuint						   BufferObjects[2];						// buffer objects
				GLuint						   IndexBufferObject;						// surface index buffer object
				GLuint						   ModelViewProjectionMatrixLocation;
				GLuint						   TextureSamplerLocation;
				GLuint						   ShaderProgram;
				vml::shaders::GlShaderProgram *Shader;
				vml::views::View			  *View;

				// projection matrix for ortho rendering

				glm::mat4 ProjMat;
				float	  Dp;
				float	  Ds;
				
				// opengl texture

				vml::textures::Texture *Texture;
				float					TextureRatio;
				unsigned int			TextureW;
				unsigned int			TextureH;
				unsigned int			TextureSize;
				
				// texture strings

				std::string TextureWidthString;
				std::string TextureHeightString;
				std::string TextureAspectRatioString;
				std::string TextureBppString;
				std::string TextureIdString;
				std::string TextureMinificationString;
				std::string TextureMagnificationString;
				std::string TextureFormatString;
				std::string RepeatSString;
				std::string RepeatTString;
				
				// file related strings

				std::string	FileName;


			public:

				glm::vec2 Pos2d;
				float     Scale;
				
				// 2d levelgenerator

				fa2040::tools::Level2dGenerator* Level2dGenerator;

				bool TriangleLayerVisible;
				bool ExternalTriangleLayerVisible;
				bool BorderTriangleLayerVisible;
				bool TextureVisible;

				// ---------------------------------------------------------------

				void BeginOrtho()
				{
					float width  = View->GetViewPortWidth();
					float height = View->GetViewPortHeight();

					ProjMat = glm::ortho( Pos2d.x - width  * 0.5f * Scale,
										  Pos2d.x + width  * 0.5f * Scale,
										 -Pos2d.y + height * 0.5f * Scale * TextureRatio,
										 -Pos2d.y - height * 0.5f * Scale * TextureRatio,
										 -1.f, 1.f);
				}

				// ---------------------------------------------------------------

				void EndOrtho()
				{

				}

				// ---------------------------------------------------------------
				
				void DrawLayers()
				{
					if (!Texture)
						return;

					if (!Level2dGenerator)
						return;

					if (!Level2dGenerator->IsCompiled())
						return;

					if (ExternalTriangleLayerVisible) 
						Level2dGenerator->ExternalTriangleLayer2.Draw(ProjMat, Texture->GetWidth(), Texture->GetHeight(), vml::colors::Green);
				
					if (BorderTriangleLayerVisible) 
						Level2dGenerator->BorderLayer.Draw(ProjMat, Texture->GetWidth(), Texture->GetHeight(), vml::colors::Yellow);
					
					if (TriangleLayerVisible) 
						Level2dGenerator->TriangleLayer.Draw(ProjMat, Texture->GetWidth(), Texture->GetHeight(), vml::colors::Red);
				}
				
				// ---------------------------------------------------------------

				void DrawTexture()
				{
					if (!Texture)
						return;

					if (!TextureVisible)
						return;

					glUseProgram(ShaderProgram);
					glUniformMatrix4fv(ModelViewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMat));
					glUniform1i(TextureSamplerLocation, 0);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, Texture->GetID());
					glBindVertexArray(VAOid);
					glDisable(GL_DEPTH);
					glDisable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawElements(GL_TRIANGLES, Indices, GL_UNSIGNED_INT, 0);
					glEnable(GL_CULL_FACE);
					glEnable(GL_DEPTH);
					glBindTexture(GL_TEXTURE_2D, 0);
					glBindVertexArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);
				}

				// ---------------------------------------------------------------

				void LoadTexture(const std::string& filename)
				{
					
					// clear all texture data but 
					// doesn't releases sahder and vao for rendering

					Clear();
					
					// load texture

					FileName = filename;
					
					Texture = new vml::textures::Texture(FileName,
														 vml::textures::Texture::TEXTURE_CLAMP_TO_EDGE_S,
														 vml::textures::Texture::TEXTURE_CLAMP_TO_EDGE_T,
														 vml::textures::Texture::TEXTURE_FILTER_MAG_NEAREST, // turns off linear filtering to preserve 'crisp' pixels when magnified
														 vml::textures::Texture::TEXTURE_FILTER_MIN_NEAREST, // turns off linear filtering to preserve 'crisp' pixels when minified
														 vml::textures::Texture::TEXTURE_MIPMAP_FALSE,
														 vml::textures::Texture::TEXTURE_GAMMA_FALSE,
														 vml::textures::Texture::TEXTURE_RELEASE_DATA_FALSE,
														 vml::textures::Texture::TEXTURE_RESIDENT_FALSE);

					// cache texture dimensions

					TextureW     = Texture->GetWidth();
					TextureH     = Texture->GetHeight();
					TextureRatio = (float)TextureW / (float)TextureH;
					TextureSize  = TextureW * TextureH;

					// create string for faster rendering using dearimgui
					 
					TextureWidthString       = vml::strings::StringConverter::FromIntToString(Texture->GetWidth());
					TextureHeightString      = vml::strings::StringConverter::FromIntToString(Texture->GetHeight());
					TextureAspectRatioString = vml::strings::StringConverter::FromFloatToString( (float)Texture->GetHeight() / (float)Texture->GetWidth());
					TextureBppString         = vml::strings::StringConverter::FromIntToString(Texture->GetBPP());
					TextureIdString          = vml::strings::StringConverter::FromIntToString(Texture->GetID());
					
					int mini = Texture->GetMinificationFilter();
					if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_NEAREST)				 TextureMinificationString = "TEXTURE_FILTER_MIN_NEAREST";
					if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_LINEAR)				 TextureMinificationString = "TEXTURE_FILTER_MIN_LINEAR";
					if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_NEAREST_MIPMAP)		 TextureMinificationString = "TEXTURE_FILTER_MIN_NEAREST_MIPMAP";
					if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_LINEAR_MIPMAP)		 TextureMinificationString = "TEXTURE_FILTER_MIN_LINEAR_MIPMAP";
					if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR) TextureMinificationString = "TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR";
					int maxi = Texture->GetMagnificationFilter();
					if (maxi == vml::textures::Texture::TEXTURE_FILTER_MAG_NEAREST) TextureMagnificationString = "TEXTURE_FILTER_MAG_NEAREST";
					if (maxi == vml::textures::Texture::TEXTURE_FILTER_MAG_LINEAR)  TextureMagnificationString = "TEXTURE_FILTER_MAG_LINEAR";
					int format = Texture->GetFormat();
					if (format == vml::textures::Texture::TEXTURE_RED)		  TextureFormatString = "GL_RED";
					if (format == vml::textures::Texture::TEXTURE_RGB)		  TextureFormatString = "GL_RGB";
					if (format == vml::textures::Texture::TEXTURE_SRGB)	 	  TextureFormatString = "GL_SRGB";
					if (format == vml::textures::Texture::TEXTURE_RGBA)		  TextureFormatString = "GL_RGBA";
					if (format == vml::textures::Texture::TEXTURE_SRGB_ALPHA) TextureFormatString = "GL_SRGB_ALPHA";
					int repeats = Texture->GetRepeatS();
					if (repeats != vml::textures::Texture::TEXTURE_REPEAT_S)          RepeatSString = "TEXTURE_REPEAT_S";
					if (repeats != vml::textures::Texture::TEXTURE_MIRRORED_S)        RepeatSString = "TEXTURE_MIRRORED_S";
					if (repeats != vml::textures::Texture::TEXTURE_CLAMP_TO_EDGE_S)	  RepeatSString = "TEXTURE_CLAMP_TO_EDGE_S";
					if (repeats != vml::textures::Texture::TEXTURE_CLAMP_TO_BORDER_S) RepeatSString = "TEXTURE_CLAMP_TO_BORDER_S";
					int repeatt = Texture->GetRepeatT();
					if (repeatt != vml::textures::Texture::TEXTURE_REPEAT_S)          RepeatTString = "TEXTURE_REPEAT_S";
					if (repeatt != vml::textures::Texture::TEXTURE_MIRRORED_S)        RepeatTString = "TEXTURE_MIRRORED_S";
					if (repeatt != vml::textures::Texture::TEXTURE_CLAMP_TO_EDGE_S)	  RepeatTString = "TEXTURE_CLAMP_TO_EDGE_S";
					if (repeatt != vml::textures::Texture::TEXTURE_CLAMP_TO_BORDER_S) RepeatTString = "TEXTURE_CLAMP_TO_BORDER_S";

					// allocate level generator

					Level2dGenerator = new fa2040::tools::Level2dGenerator();
					
				}

				// ---------------------------------------------------------------

				bool CompileAndSaveMap(int navmeshscalingfactor, int navmesherosionfactor)
				{
					if (Level2dGenerator)
					{
						
						// level generatoo accepts, texture data, scaling factor and erosion factor
						// scaling factor is the dowsampling, erosionfact is  the number of times
						// the bitmap is 'eroded' to be thinnier , this is becasue pathfinding
						// doing so , won't traces path too near to borders

						Level2dGenerator->Go(GetTexture(), navmeshscalingfactor, navmesherosionfactor);
					
						Level2dGenerator->Convert2dMapTo3dFile();
						
						return true;
					}

					return false;
				}

				// ---------------------------------------------------------------

				void Init()
				{
					// Init map View

					View = new vml::views::View( "fpsview0",
												 0.03f,
												 10000,
												 60,
												 glm::vec3(0, 65, 0),
												 glm::vec3(0, vml::math::PIDIV2, 0),
												 0.25f,
												 0.001f,
												 vml::views::View::EULER );

					// Panning and Zooming

					Dp    = 0.1f;
					Ds    = 1.1f;
					Scale = 0.005f;

				}

				// ------------------------------------------------------------------------------------
				// 

				void Clear()
				{
					if (Texture)
					{
						// delete texture

						vml::SafeDelete(Texture);

						// set texture data to default

						TextureRatio = 0.0f;
						TextureW = 0;
						TextureH = 0;
						TextureSize = 0;

						// texture strings

						TextureWidthString = "";
						TextureHeightString = "";
						TextureAspectRatioString = "";
						TextureBppString = "";
						TextureIdString = "";
						TextureMinificationString = "";
						TextureMagnificationString = "";
						TextureFormatString = "";
						RepeatSString = "";
						RepeatTString = "";

						// delete level generator

						vml::SafeDelete(Level2dGenerator);

						TriangleLayerVisible = true;
						ExternalTriangleLayerVisible = true;
						BorderTriangleLayerVisible = true;
						TextureVisible = true;
					}

				}

				// ------------------------------------------------------------------------------------
				// 

				void UpdateInput(const vml::OpenGLContextWindow* window)
				{

					if (!Level2dGenerator)
						return;

					double mousex   = window->GetMouseXDelta();
					double mousey   = window->GetMouseYDelta();
					int leftbutton  = window->GetLeftButtonState();
					int rightbutton = window->GetRightButtonState();

					float dp       = Dp * Scale * 60;
					float ds       = Ds;
					float minscale = 0.00001f;

					// forward movement

					if (rightbutton && !leftbutton)
					{

						if (mousey > 0)
						{
							Scale *= ds;
						}
						else if (mousey < 0)
						{
							Scale /= ds;
						}

						if (Scale < minscale)
						{
							Scale = minscale;
						}
					}

					// strafe movement

					if (leftbutton )
					{

						if (mousex < 0)
						{
							Pos2d.x -= dp;
						}
						else if (mousex > 0)
						{
							Pos2d.x += dp;
						}

						if (mousey > 0)
						{
							Pos2d.y -= dp;
						}
						else if (mousey < 0)
						{
							Pos2d.y += dp;
						}
					}
				}
				
				// ---------------------------------------------------------------

				const vml::textures::Texture* GetTexture() const { return Texture; }
				const std::string& GetFileName() const { return FileName; }
				const std::string& GetTextureWidthString() const { return TextureWidthString; }
				const std::string& GetTextureHeightString() const { return TextureHeightString; }
				const std::string& GetTextureAspectRatioString() const { return TextureAspectRatioString; }
				const std::string& GetTextureBppString() const { return TextureBppString; }
				const std::string& GetTextureIdString() const { return TextureIdString; }
				const std::string& GetTextureMinificationString() const { return TextureMinificationString; }
				const std::string& GetTextureMagnificationString() const { return TextureMagnificationString; }
				const std::string& GetTextureFormatString() const { return TextureFormatString; }
				const std::string& GetRepeatSString() const { return RepeatSString; }
				const std::string& GetRepeatTString() const { return RepeatTString; }
				vml::views::View* GetView() const { return View; }
				const glm::mat4 GetProj() const { return ProjMat; }

				// ---------------------------------------------------------------
				// constructors / destructors

				Level2dMap()
				{
					
					VAOid             = 0;
					BufferObjects[0]  = 0;
					BufferObjects[1]  = 0;
					IndexBufferObject = 0;
					Shader            = nullptr;

					ProjMat = glm::mat4(0);
					Pos2d   = glm::vec2(0, 0);
					Dp      = 0.0f;
					Ds      = 0.0f;
					Scale   = 0.0f;

					View    = nullptr;
					Texture = nullptr;

					TextureRatio = 0.0f;
					TextureW     = 0;
					TextureH     = 0;
					TextureSize  = 0;
					
					Level2dGenerator = nullptr;

					TriangleLayerVisible         = true;
					ExternalTriangleLayerVisible = true;
					BorderTriangleLayerVisible   = true;
					TextureVisible				 = true;

					// Create the VAO for the map rendering

					glGenVertexArrays(1, &VAOid);
					glBindVertexArray(VAOid);

					// Create the Vertex Buffer Object 

					// Map the generic vertex attributes used by the mesh's shader program to
					// the mesh's vertex data stored in the vertex buffer object.

					GLuint AttributePosition = vml::shaders::GlShaderProgram::ATTRIBUTE_POSITION;
					GLuint AttributeTexCoord = vml::shaders::GlShaderProgram::ATTRIBUTE_TEXCOORD;

					glGenBuffers(1, &BufferObjects[0]);
					glGenBuffers(1, &BufferObjects[1]);

					// Generate and populate the buffers with vertex attributes and the indices

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
					glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionArray), &VertexPositionArray[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(AttributePosition);
					glVertexAttribPointer(AttributePosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
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
					glUseProgram(0);

					glDisableVertexAttribArray(AttributePosition);
					glDisableVertexAttribArray(AttributeTexCoord);

					std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\shaders";

					Shader = vml::ShaderStore->Load(shaderstorepath + "\\debug\\debug_texture.shd");
					
					glUseProgram(ShaderProgram);
					ShaderProgram = Shader->GetID();
					TextureSamplerLocation = glGetUniformLocation(ShaderProgram, "TextureSampler");
					ModelViewProjectionMatrixLocation = Shader->GetModelViewProjectionMatrixLocation();
					glUseProgram(0);
					
				}

				~Level2dMap()
				{
					
					// delete view

					vml::SafeDelete(View);

					// delete texture

					vml::SafeDelete(Texture);
					
					// delete level generator

					vml::SafeDelete(Level2dGenerator);

					// delete opengl buffers

					if (VAOid) { glDeleteVertexArrays(1, &VAOid); VAOid = 0; }
					if (IndexBufferObject) { glDeleteBuffers(1, &IndexBufferObject); IndexBufferObject = 0; }
					if (BufferObjects[0]) { glDeleteBuffers(1, &BufferObjects[0]); BufferObjects[0] = 0; }
					if (BufferObjects[1]) { glDeleteBuffers(1, &BufferObjects[1]); BufferObjects[1] = 0; }
					
					// unload shader 

					std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\shaders";
					vml::ShaderStore->UnLoad(shaderstorepath + "\\debug\\debug_texture.shd");
					
				}

		};
	}
}
