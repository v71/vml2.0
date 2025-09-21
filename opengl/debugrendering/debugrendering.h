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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <vml2.0\core\mesh\procedural.h>
#include<vml2.0\opengl\debugrendering\checkeredplane.h>
#include<vml2.0\opengl\debugrendering\singlepoint.h>
#include<vml2.0/opengl/debugrendering/segment.h>
#include<vml2.0\opengl\debugrendering\quads.h>
#include<vml2.0\opengl\debugrendering\triangles.h>
#include<vml2.0\opengl\debugrendering\cube.h>
#include<vml2.0/opengl/debugrendering/pyramid.h>
#include<vml2.0/opengl/debugrendering/sphere.h>
#include<vml2.0\opengl\textures\texture.h>
#include<vml2.0/opengl/debugrendering/fbo.h>
#include<vml2.0\opengl\debugrendering\billboard.h>
#include <vml2.0/math/3d/aaboxintersections.h>
#include <vml2.0/math/3d/sphereintersections.h>
#include <vml2.0/math/3d/geometry3d.h>
#include <vml2.0\core\mesh\meshstore.h>
namespace vml { extern vml::meshes::MeshStore* MeshStore; }
#include <vml2.0/opengl/view/frustum3d.h>
#include <vml2.0/core/mesh/objectmanager2.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{

	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// directional light

		class DebugDirectionalLight
		{

			public:

				//------------------------------------------------
				// light properties

				glm::vec4 Ambient;
				glm::vec4 Diffuse;
				glm::vec4 Specular;
				glm::vec4 Direction;
				glm::vec4 CameraSpaceDirection;
				float	  Power;

				//------------------------------------------------
				// ctor / dtor

				DebugDirectionalLight()
				{
					Ambient = glm::vec4(1, 1, 1, 1);
					Diffuse = glm::vec4(1, 1, 1, 1);
					Specular = glm::vec4(1, 1, 1, 1);
					Direction = glm::vec4(0, 0, 0, 1);
					CameraSpaceDirection = glm::vec4(0, 0, 0, 1);
					Power = 1.0f;
				}

				DebugDirectionalLight(const glm::vec4 ambient,
									  const glm::vec4 diffuse,
									  const glm::vec4 specular,
									  const glm::vec4 direction,
									  float power)
				{
					Ambient = ambient;
					Diffuse = diffuse;
					Specular = specular;
					Direction = direction;
					Power = power;
					CameraSpaceDirection = glm::vec4(0, 0, 0, 0);
				}

				~DebugDirectionalLight()
				{}

		};

	}

}


namespace vml
{

	namespace debugrendering
	{

		////////////////////////////////////////////////////////////////
		// simple material 

		class DebugMaterial
		{

			public:

				// ---------------------------------------------------------------
				// private data

				vml::colors::RGBA	Ambient;			// ambient component
				vml::colors::RGBA	Diffuse;			// diffuse component
				vml::colors::RGBA	Specular;			// specular component
				vml::colors::RGBA	Emission;			// emission component
				float				Shininess;			// shininess component

				// ---------------------------------------------------------------
				// ctor / dtor

				DebugMaterial(const glm::vec4 &ambient, 
							  const glm::vec4 &diffuse,
							  const glm::vec4 &specular,
							  const glm::vec4 &emission,
							  const float shininess)
				{
					Ambient = ambient;
					Diffuse = diffuse;
					Specular = specular;
					Emission = emission;
					Shininess = shininess;
				}

				DebugMaterial()
				{
					Ambient   = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
					Diffuse   = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
					Specular  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
					Emission  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					Shininess = 15;
				}

				~DebugMaterial()
				{
				}

		};
	}
}

namespace vml
{
	namespace debugrendering
	{
		static DebugMaterial Material0( glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
										glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
										glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
										glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
										15);

		static DebugMaterial Material1( glm::vec4(0.3f, 0.0f, 0.0f, 1.0f),
										glm::vec4(0.3f, 0.0f, 0.0f, 1.0f),
										glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
										glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
										15);

		static DebugMaterial Material2( glm::vec4(0.0f, 0.3f, 0.0f, 1.0f),
										glm::vec4(0.0f, 0.3f, 0.0f, 1.0f),
										glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
										glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
										15);
	}
}

namespace vml
{
	/////////////////////////////////////////////////
	// debug rendering class
	
	class OpenglDebugRender
	{
		private:

			// ---------------------------------------------------------------------------------------

			vml::debugrendering::CheckeredPlane			*CheckeredPlaneMesh;
			vml::debugrendering::Quad					*QuadMesh;
			vml::debugrendering::QuadPerVertexColor	    *QuadPerVertexColorMesh;
			vml::debugrendering::TexturedQuad			*QuadTexturedMesh;
			vml::debugrendering::AlphaTexturedQuad		*AlphaTexturedQuadMesh;
			vml::debugrendering::AlphaColorTexturedQuad *AlphaTexturedColorQuadMesh;

			vml::debugrendering::Triangle				*TriangleMesh;
			vml::debugrendering::TrianglePerVertexColor *TrianglePerVertexColorMesh;
			vml::debugrendering::TexturedTriangle		*TriangleTexturedMesh;

			vml::debugrendering::WireCube			    *WireCubeMesh;
			vml::debugrendering::SolidCube				*SolidCubeMesh;
			vml::debugrendering::Pyramid				*PyramidMesh;
			vml::debugrendering::BillBoard				*BillBoardMesh;
			vml::debugrendering::SinglePoint			*SinglePoint;
			vml::debugrendering::Sphere					*WireSphereMesh;
			vml::debugrendering::Segment			    *Segment;

			unsigned int Flags;		// flags to set initted state

			// debug texture

			vml::textures::Texture *Texture;

			// debug shader

			vml::shaders::GlShaderProgram *DebugShader;
			vml::shaders::GlShaderProgram *SingleColorShader;
			vml::shaders::GlShaderProgram* TextureShaderAlpha;
			vml::shaders::GlShaderProgram* TextureShaderAlphaColor;

			// debug directional light shader locations

			GLint LightAmbientLocation;
			GLint LightDiffuseLocation;
			GLint LightSpecularLocation;
			GLint LightDirectionLocation;
			GLint LightPowerLocation;
			GLint LightCameraSpaceLocation;

			// debug directiona light debug material 

			GLint MaterialAmbientLocation;
			GLint MaterialDiffuseLocation;
			GLint MaterialSpecularLocation;
			GLint MaterialShininessLocation;

			// color location for debug rendering

			GLint ColorLocation;

		public:

			static const unsigned int NO_DRAW_POINTS = 0;
			static const unsigned int DRAW_POINTS	 = 1;
			static const unsigned int DRAW_WIRE		 = 0;
			static const unsigned int DRAW_SOLID	 = 1;

			// -----------------------------------------------------------------------------------
			// 

			glm::vec4 ClearColor;			// background color
			glm::vec4 WireFrameColor;		// color for wireframe rendering

			// -----------------------------------------------------------------------------------
			// directional light shader and material

			vml::debugrendering::DebugDirectionalLight  DirectionalLight;

			// -----------------------------------------------------------------------------------
			
			vml::shaders::GlShaderProgram* GetDebugShader() { return DebugShader; }
			vml::shaders::GlShaderProgram* GetSingleColorShader() { return SingleColorShader; }
			GLuint GetTextureID() const { return Texture->GetID(); }
			vml::textures::Texture* GetTexture() const { return Texture; }
			GLint GetMaterialAmbientLocation() const { return MaterialAmbientLocation; }
			GLint GetMaterialDiffuseLocation() const { return MaterialDiffuseLocation; }
			GLint GetMaterialSpecularLocation() const { return MaterialSpecularLocation; }
			GLint GetMaterialShininessLocation() const { return MaterialShininessLocation; }
			GLint GetLightAmbientLocation() const { return LightAmbientLocation; }
			GLint GetLightDiffuseLocation() const { return LightDiffuseLocation; }
			GLint GetLightSpecularLocation() const { return LightSpecularLocation; }
			GLint GetLightDirectionLocation() const { return LightDirectionLocation; }
			GLint GetLightPowerLocation() const { return LightPowerLocation; }
			GLint GetLightCameraSpaceLocation() const { return LightCameraSpaceLocation; }

			// -----------------------------------------------------------------------------------
			// plane rendering

			void DrawCheckeredQuad( vml::views::View* view,
									float scale, const glm::vec3& center, 
									unsigned int n, const glm::vec4& c1, const glm::vec4& c2)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				CheckeredPlaneMesh->Draw(view, scale, center, n, c1, c2);
			}

			// -----------------------------------------------------------------------------------
			// point drawing

			void DrawPoint(vml::views::View* view,
						   const glm::vec3& p,
						   const glm::vec4& col)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for point debug rendering");
				SinglePoint->Draw(p, col, view);
			}

			// -----------------------------------------------------------------------------------
			// point drawing

			void DrawLine(vml::views::View* view,
						  const glm::vec3& p,
						  const glm::vec3& q,
						  const glm::vec4& col,bool drawpoints)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for point debug rendering");
				Segment->Draw(p, q,col, view,drawpoints);
			}

			// -----------------------------------------------------------------------------------
			// triangle drawing

			void DrawTriangle(vml::views::View* view,
							  const float scale, const glm::vec3& pos,
							  const glm::vec4& col,bool wire=false,bool drawpoints=false)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				TriangleMesh->Draw(view, scale, pos, col, wire, drawpoints);
			}

			// -----------------------------------------------------------------------------------

			void DrawTriangle(vml::views::View* view,
							  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
							  const glm::vec4& col, bool wire=false, bool drawpoints=false)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				TriangleMesh->Draw(view, p0, p1, p2, col, wire, drawpoints);
			}

			// -----------------------------------------------------------------------------------

			void DrawTriangle(vml::views::View* view,
							  const float scale, const glm::vec3& pos,
							  const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2, const glm::vec4& c3)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				TrianglePerVertexColorMesh->Draw(view, scale, pos, c0, c1, c2);
			}

			// -----------------------------------------------------------------------------------

			void DrawTriangle(vml::views::View* view,
							  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,
							  const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				TrianglePerVertexColorMesh->Draw(view, p0, p1, p2, c0, c1, c2);
			}

			// -----------------------------------------------------------------------------------
			// textured quad drawing 

			void DrawTexturedTriangle(vml::views::View* view,
									  const float scale, const glm::vec3& pos,
									  GLuint texid)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				TriangleTexturedMesh->Draw(view, scale, pos, texid);
			}

			// -----------------------------------------------------------------------------------

			void DrawTexturedTriangle(vml::views::View* view,
									  const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2,
									  const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2,
									  GLuint texid)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				TriangleTexturedMesh->Draw(view, p0, p1, p2, uv0, uv1, uv2, texid);
			}

			// -----------------------------------------------------------------------------------
			// single color quad drawing
			
			void DrawQuad(vml::views::View* view,
						  const float scale, const glm::vec3& pos,
						  const glm::vec4& col,int mode)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				QuadMesh->Draw(view, scale, pos, col,mode);
			}
		
			// -----------------------------------------------------------------------------------
			// single color quad drawing using user coordinates

			void DrawQuad(vml::views::View* view,
				          const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
						  const glm::vec4& col,int mode)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				QuadMesh->Draw(view, p0, p1, p2, p3, col,mode);
			}

			// -----------------------------------------------------------------------------------
			// per vertex color quad drawing

			void DrawQuad(vml::views::View* view,
						  const float scale, const glm::vec3& pos,
						  const glm::vec4 &c0,const glm::vec4 &c1,const glm::vec4 &c2,const glm::vec4 &c3 )
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				QuadPerVertexColorMesh->Draw(view, scale, pos, c0, c1, c2, c3);
			}

			// -----------------------------------------------------------------------------------
			// per vertex color quad drawing using user coordinates

			void DrawQuad(vml::views::View* view,
						  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
				          const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2, const glm::vec4& c3)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				QuadPerVertexColorMesh->Draw(view, p0, p1, p2, p3, c0, c1, c2, c3);
			}

			// -----------------------------------------------------------------------------------
			// textured quad drawing 

			void DrawTexturedQuad(vml::views::View* view,
								  const float scale, const glm::vec3& pos,
								  GLuint texid)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				QuadTexturedMesh->Draw(view, scale, pos, texid);
			}

			// -----------------------------------------------------------------------------------

			void DrawTexturedQuad(vml::views::View* view,
								  const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
								  GLuint texid)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				QuadTexturedMesh->Draw(view, p0, p1, p2, p3, texid);
			}

			// -----------------------------------------------------------------------------------
			// alpha textured quad drawing 

			void DrawAlphaTexturedQuad(vml::views::View* view,const float scale, const glm::vec3& pos,GLuint texid,const float alpha, const int blendmode)
			{
				if (!view) vml::os::Message::Error("DebugRender : ", "Null view matrix for triangle debug rendering");
				AlphaTexturedQuadMesh->Draw(view, scale, pos, texid,alpha,blendmode);
			}

			void DrawAlphaTexturedQuad(vml::views::View* view, const glm::vec3 &p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, GLuint texid, const float alpha, const int blendmode)
			{
				if (!view) vml::os::Message::Error("DebugRender : ", "Null view matrix for triangle debug rendering");
				AlphaTexturedQuadMesh->Draw(view, p0,p1,p2,p3, texid, alpha, blendmode);
			}

			// -----------------------------------------------------------------------------------
			// alpha textured colored quad drawing 

			void DrawAlphaColorTexturedQuad(vml::views::View* view, const float scale, const glm::vec3& pos, const glm::vec3 &col,GLuint texid, const float alpha, const int blendmode)
			{
				if (!view) vml::os::Message::Error("DebugRender : ", "Null view matrix for triangle debug rendering");
				AlphaTexturedColorQuadMesh->Draw(view, scale, pos, col,texid, alpha, blendmode);
			}

			void DrawAlphaColorTexturedQuad(vml::views::View* view, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, 
																    const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& c2, const glm::vec3& c3,
																	GLuint texid, const float alpha, const int blendmode)
			{
				if (!view) vml::os::Message::Error("DebugRender : ", "Null view matrix for triangle debug rendering");
				AlphaTexturedColorQuadMesh->Draw(view, p0, p1, p2, p3, c0,c1,c2,c3,texid, alpha, blendmode);
			}

			// -----------------------------------------------------------------------------------
			// billboards

			void DrawBillBoard(vml::views::View* view,
							   const float scale, const glm::vec3& pos,
							   GLuint texid)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				BillBoardMesh->Draw(view, scale, pos, texid);
			}

			// -----------------------------------------------------------------------------------

			void DrawOrtho(vml::views::View* view,
							const glm::vec2 &scale, const glm::vec2& pos,
							GLuint texid)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for triangle debug rendering");
				BillBoardMesh->DrawOrtho(view, scale, pos, texid);
			}
		
			// -----------------------------------------------------------------------------------
			// axis aligned bounding box drawing

			void DrawWireCube(vml::views::View* view,
							  const glm::mat4 &m,
							  const glm::vec4& col,
							  int drawpoints = NO_DRAW_POINTS)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view  matrix for bounding box debug rendering");
				if (drawpoints == NO_DRAW_POINTS)
					WireCubeMesh->Draw(view,m,col, false);
				if (drawpoints == DRAW_POINTS)
					WireCubeMesh->Draw(view,m,col, true);
			}

			// -----------------------------------------------------------------------------------
			// axis aligned bounding box drawing

			void DrawAABBox(vml::views::View* view,
							const glm::vec3& bmin, const glm::vec3& bmax,
							const glm::vec4& col,
							int drawpoints = NO_DRAW_POINTS)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view  matrix for bounding box debug rendering");
				if (drawpoints == NO_DRAW_POINTS)
					WireCubeMesh->Draw(view,bmin, bmax, col, false);
				if (drawpoints == DRAW_POINTS)
					WireCubeMesh->Draw(view,bmin, bmax, col, true);
			}

			// -----------------------------------------------------------------------------------

			void DrawAABBox(vml::views::View* view,
							const vml::geo3d::AABBox& aabbox,
							const glm::vec4& col,
							int drawpoints = NO_DRAW_POINTS)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view  matrix for bounding box debug rendering");
				if (drawpoints == NO_DRAW_POINTS)
					WireCubeMesh->Draw(view,aabbox.GetMin(), aabbox.GetMax(), col, false);
				if (drawpoints == DRAW_POINTS)
					WireCubeMesh->Draw(view,aabbox.GetMin(), aabbox.GetMax(), col, true);
			}

			// -----------------------------------------------------------------------------------

			void DrawAABBox(vml::views::View* view,
							vml::meshes::Mesh3d* mesh,
							const glm::vec4& col,
							int drawpoints = NO_DRAW_POINTS)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view  matrix for bounding box debug rendering");
				if (drawpoints == NO_DRAW_POINTS)
					WireCubeMesh->Draw(view,mesh->GetBoundingBox().GetMin(), mesh->GetBoundingBox().GetMax(), col, false);
				if (drawpoints == DRAW_POINTS)
					WireCubeMesh->Draw(view,mesh->GetBoundingBox().GetMin(), mesh->GetBoundingBox().GetMax(), col, true);
			}
			
			// -----------------------------------------------------------------------------------

			void DrawAOBBox(vml::views::View* view,
							const glm::vec3& b0, const glm::vec3& b1, const glm::vec3& b2, const glm::vec3& b3,
							const glm::vec3& b4, const glm::vec3& b5, const glm::vec3& b6, const glm::vec3& b7,
							const glm::vec4& col,
							bool drawpoints = true)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view  matrix for bounding box debug rendering");
				WireCubeMesh ->Draw(b0, b1, b2, b3, b4, b5, b6, b7, col, view, drawpoints);
			}

			// -----------------------------------------------------------------------------------
			// draw bounding sphere for model's object

			void DrawSphere(vml::views::View* view,
							const glm::vec3& pos,
							const float radius,
							const glm::vec4& col)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for sphere rendering");
				WireSphereMesh->Draw(view, pos, radius, col);
			}

			// -----------------------------------------------------------------------------------

			void DrawSphere(vml::views::View* view,
							const glm::mat4& m,
							const glm::vec4& col)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for sphere rendering");
				WireSphereMesh->Draw(view, m, col);
			}

			// -----------------------------------------------------------------------------------
			// draw bounding sphere for model's object

			void DrawEllipsoid(vml::views::View* view,
							   vml::objects::Model3d_2* model,
							   const glm::vec4& col)
			{
				if (view)
				{
					if (model)
					{
						vml::geo3d::collisions::EllipsoidCollider* ec = model->GetEllipsoidCollider();
						DrawSphere(view, ec->GetMatrix(), col);
					}
					else
					{
						vml::os::Message::Error("DebugRender : ","Null model pointer for sphere rendering");
					}
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view matrix for sphere rendering");
				}
			}

			void DrawEllipsoid(vml::views::View* view,const glm::mat4 &m,const glm::vec4& col)
			{
				if (view)
				{
					DrawSphere(view, m, col);
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view matrix for sphere rendering");
				}
			}

			// -----------------------------------------------------------------------------------

			void DrawMesh(vml::views::View* view,
				          const vml::meshes::Mesh3d *mesh,
				          const vml::debugrendering::DebugMaterial &material=vml::debugrendering::Material0)
			{
				if (view)
				{
					DrawAABBox(view, mesh->GetBoundingBox().GetMin(), mesh->GetBoundingBox().GetMax(), vml::colors::Blue, true);

					// model matrix

					glm::mat4 M = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
					glm::mat4 V = view->GetView();
					glm::mat4 P = view->GetProjection();
					glm::mat4 MV = V * M;
					glm::mat4 MVP = P * MV;

					// get normal matrix pointer

					glm::mat3 NV;

					// get normal matrix

					float* NormalViewMatrix = glm::value_ptr(NV);

					// get model * view matrix

					float* ModelViewMatrix = glm::value_ptr(MV);

					// normal matrix is computed as the inverse transpose
					// of the model view matrix, this causes the 
					// first 3x3 order for this matrix is to be divided 
					// by each scaling factor along correspondig axes, so 
					// we need to unskew normal matrix

					float Determinant = + ModelViewMatrix[0] * (ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[9])
										- ModelViewMatrix[1] * (ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[8])
										+ ModelViewMatrix[2] * (ModelViewMatrix[4] * ModelViewMatrix[ 9] - ModelViewMatrix[5] * ModelViewMatrix[8]);

					if (Determinant > -vml::math::EPSILON && Determinant < vml::math::EPSILON) Determinant = 1.0f / vml::math::EPSILON;
						else Determinant = 1.0f / Determinant;

					NormalViewMatrix[0] = +(ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[6]) * Determinant;
					NormalViewMatrix[1] = -(ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[6]) * Determinant;
					NormalViewMatrix[2] = +(ModelViewMatrix[4] * ModelViewMatrix[ 9] - ModelViewMatrix[8] * ModelViewMatrix[5]) * Determinant;
					NormalViewMatrix[3] = -(ModelViewMatrix[1] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[4] = +(ModelViewMatrix[0] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[5] = -(ModelViewMatrix[0] * ModelViewMatrix[ 9] - ModelViewMatrix[8] * ModelViewMatrix[1]) * Determinant;
					NormalViewMatrix[6] = +(ModelViewMatrix[1] * ModelViewMatrix[ 6] - ModelViewMatrix[5] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[7] = -(ModelViewMatrix[0] * ModelViewMatrix[ 6] - ModelViewMatrix[4] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[8] = +(ModelViewMatrix[0] * ModelViewMatrix[ 5] - ModelViewMatrix[4] * ModelViewMatrix[1]) * Determinant;

					// get shader

					vml::shaders::GlShaderProgram* shader = GetDebugShader();
					
					GLuint Id = shader->GetID();

					glUseProgram(Id);

					// set shader locations

					glUniformMatrix4fv(shader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
					glUniformMatrix4fv(shader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
					glUniformMatrix4fv(shader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
					glUniformMatrix3fv(shader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
					glUniformMatrix4fv(shader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(shader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

					glUniform4fv(GetMaterialAmbientLocation(), 1, &material.Ambient[0]);
					glUniform4fv(GetMaterialDiffuseLocation(), 1, &material.Diffuse[0]);
					glUniform4fv(GetMaterialSpecularLocation(), 1, &material.Specular[0]);
					glUniform1f(GetMaterialShininessLocation(), material.Shininess);

					DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

					glUniform4fv(GetLightAmbientLocation(), 1, &DirectionalLight.Ambient[0]);
					glUniform4fv(GetLightDiffuseLocation(), 1, &DirectionalLight.Diffuse[0]);
					glUniform4fv(GetLightSpecularLocation(), 1, &DirectionalLight.Specular[0]);
					glUniform4fv(GetLightDirectionLocation(), 1, &DirectionalLight.Direction[0]);
					glUniform4fv(GetLightCameraSpaceLocation(), 1, &DirectionalLight.CameraSpaceDirection[0]);
					glUniform1f(GetLightPowerLocation(), DirectionalLight.Power);

					// draw mesh

					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT, GL_FILL);

					glBindVertexArray(mesh->GetVAOId());

					glDrawElements(
									GL_TRIANGLES,				// mode
									mesh->GetIndicesCount(),	// count
									GL_UNSIGNED_INT,			// type
									(void*)0					// element array buffer offset
								);

					glBindVertexArray(0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);
					
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view matrix for sphere rendering");
				}
			}

			// -----------------------------------------------------------------------------------

			void DrawAABBox(vml::views::View* view,
							vml::objects::Model3d_2* model,
							const glm::vec4& col,
							int drawpoints)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view  matrix for bounding box debug rendering");
				if (drawpoints == NO_DRAW_POINTS)
					WireCubeMesh->Draw(view,model->GetAABoundingBox().GetMin(), model->GetAABoundingBox().GetMax(), col, false);
				if (drawpoints == DRAW_POINTS)
					WireCubeMesh->Draw(view,model->GetAABoundingBox().GetMin(), model->GetAABoundingBox().GetMax(), col, true);
			}

			// -----------------------------------------------------------------------------------

			void DrawAABBox(vml::views::View* view,
							vml::objects::Object3d_2* object,
							const glm::vec4& col,
							int drawpoints)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view  matrix for bounding box debug rendering");
				if (drawpoints == NO_DRAW_POINTS)
					WireCubeMesh->Draw(view,object->GetAABoundingBox().GetMin(), object->GetAABoundingBox().GetMax(), col, false);
				if (drawpoints == DRAW_POINTS)
					WireCubeMesh->Draw(view,object->GetAABoundingBox().GetMin(), object->GetAABoundingBox().GetMax(), col, true);
			}

			// -----------------------------------------------------------------------------------
			
			void DrawAABBox(vml::views::View* view,
							vml::objects::Model3d_2* model,
							const glm::vec4& col,
							int drawpoints, float f)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view  matrix for bounding box debug rendering");
				glm::vec3 a = model->GetAABoundingBox().GetMin();
				glm::vec3 b = model->GetAABoundingBox().GetMax();
				glm::vec3 c = (a + b) * 0.5f;
				glm::vec3 u = glm::normalize(a - c);
				glm::vec3 v = glm::normalize(b - c);
				a += u * f;
				b += v * f;
				if (drawpoints == NO_DRAW_POINTS)
					WireCubeMesh->Draw(view,a, b, col, false);
				if (drawpoints == DRAW_POINTS)
					WireCubeMesh->Draw(view,a, b, col, true);
			}

			// -----------------------------------------------------------------------------------

			void DrawAABBox(vml::views::View* view,
							vml::objects::Object3d_2* object,
							const glm::vec4& col,
							int drawpoints, float f)
			{
				if (!view) vml::os::Message::Error("DebugRender : "," Null view  matrix for bounding box debug rendering");
				glm::vec3 a = object->GetAABoundingBox().GetMin();
				glm::vec3 b = object->GetAABoundingBox().GetMax();
				glm::vec3 c = (a + b) * 0.5f;
				glm::vec3 u = glm::normalize(a - c);
				glm::vec3 v = glm::normalize(b - c);
				a += u * f;
				b += v * f;
				if (drawpoints == NO_DRAW_POINTS)
					WireCubeMesh->Draw(view,a, b, col, false);
				if (drawpoints == DRAW_POINTS)
					WireCubeMesh->Draw(view,a, b, col, true);
			}

			// -----------------------------------------------------------------------------------
			// oriented bounding box drawing

			void DrawAOBBox(vml::views::View* view,
							vml::objects::Model3d_2* model,
							const glm::vec4& col,
							bool drawpoints = true)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null mview atrix for oriented bounding box debug rendering");
				
				if (!model)	vml::os::Message::Error("DebugRender : ","Null model pointer for bounding box rendering");
				
				if (model)
				{

					glm::vec3 boundingbox[8] = { model->GetAOBoundingBox()[0],
												 model->GetAOBoundingBox()[1],
												 model->GetAOBoundingBox()[2],
												 model->GetAOBoundingBox()[3],
												 model->GetAOBoundingBox()[4],
												 model->GetAOBoundingBox()[5],
												 model->GetAOBoundingBox()[6],
												 model->GetAOBoundingBox()[7] };

					WireCubeMesh->Draw( boundingbox[0], boundingbox[1], boundingbox[2], boundingbox[3],
										boundingbox[4], boundingbox[5], boundingbox[7], boundingbox[6],
										col, view, drawpoints);
				}
			}

			// -----------------------------------------------------------------------------------
			// oriented bounding box drawing

			void DrawAOBBox(vml::views::View* view,
							vml::objects::Model3d_2* model,
							const glm::vec4& col,
							bool drawpoints, float f)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null mview atrix for oriented bounding box debug rendering");
				
				if (!model)	vml::os::Message::Error("DebugRender : ","Null model pointer for bounding box rendering");
				
				if (model)
				{
					glm::vec3 boundingbox[8] = { model->GetAOBoundingBox()[0],
												 model->GetAOBoundingBox()[1],
												 model->GetAOBoundingBox()[2],
												 model->GetAOBoundingBox()[3],
												 model->GetAOBoundingBox()[4],
												 model->GetAOBoundingBox()[5],
												 model->GetAOBoundingBox()[6],
												 model->GetAOBoundingBox()[7] };

					glm::vec3 c = (boundingbox[0] + boundingbox[1] + boundingbox[2] + boundingbox[3] +
								   boundingbox[4] + boundingbox[5] + boundingbox[6] + boundingbox[7]) / 8.0f;
					
					glm::vec3 p[8] = { boundingbox[0],boundingbox[1],boundingbox[2],boundingbox[3],boundingbox[4],boundingbox[5],boundingbox[6],boundingbox[7] };
					glm::vec3 d[8] = { boundingbox[0]-c,boundingbox[1]-c,boundingbox[2]-c,boundingbox[3]-c,boundingbox[4]-c,boundingbox[5]-c,boundingbox[6]-c,boundingbox[7]-c };

					d[0] = glm::normalize(d[0]);
					d[1] = glm::normalize(d[1]);
					d[2] = glm::normalize(d[2]);
					d[3] = glm::normalize(d[3]);
					d[4] = glm::normalize(d[4]);
					d[5] = glm::normalize(d[5]);
					d[6] = glm::normalize(d[6]);
					d[7] = glm::normalize(d[7]);

					p[0] += d[0] * f;
					p[1] += d[1] * f;
					p[2] += d[2] * f;
					p[3] += d[3] * f;
					p[4] += d[4] * f;
					p[5] += d[5] * f;
					p[6] += d[6] * f;
					p[7] += d[7] * f;

					WireCubeMesh->Draw(p[0], p[1], p[2], p[3], p[4], p[5], p[7], p[6], col, view, drawpoints);
				}
			}

			// -----------------------------------------------------------------------------------
			// oriented bounding box drawing

			void DrawAOBBox(vml::views::View* view,
							vml::objects::Object3d_2* object,
							const glm::vec4& col,
							bool drawpoints = true)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null mview atrix for oriented bounding box debug rendering");
				
				if (!object) vml::os::Message::Error("DebugRender : ","Null model pointer for bounding box rendering");
				
				if (object)
				{
					glm::vec3 boundingbox[8] = { object->GetAOBoundingBox()[0],
												 object->GetAOBoundingBox()[1],
												 object->GetAOBoundingBox()[2],
												 object->GetAOBoundingBox()[3],
												 object->GetAOBoundingBox()[4],
												 object->GetAOBoundingBox()[5],
												 object->GetAOBoundingBox()[6],
												 object->GetAOBoundingBox()[7] };

					WireCubeMesh->Draw( boundingbox[0], boundingbox[1], boundingbox[2], boundingbox[3],
										boundingbox[4], boundingbox[5], boundingbox[7], boundingbox[6],
										col, view, drawpoints);
				}
			}

			// -----------------------------------------------------------------------------------

			void DrawAOBBox(vml::views::View* view,
							vml::objects::Object3d_2* object,
							const glm::vec4& col,
							bool drawpoints, float f)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null mview atrix for oriented bounding box debug rendering");
				
				if (!object) vml::os::Message::Error("DebugRender : ","Null model pointer for bounding box rendering");
				
				if (object)
				{
					glm::vec3 boundingbox[8] = { object->GetAOBoundingBox()[0],
												 object->GetAOBoundingBox()[1],
												 object->GetAOBoundingBox()[2],
												 object->GetAOBoundingBox()[3],
												 object->GetAOBoundingBox()[4],
												 object->GetAOBoundingBox()[5],
												 object->GetAOBoundingBox()[6],
												 object->GetAOBoundingBox()[7] };

					glm::vec3 c = (boundingbox[0] + boundingbox[1] + boundingbox[2] + boundingbox[3] +
								   boundingbox[4] + boundingbox[5] + boundingbox[6] + boundingbox[7]) / 8.0f;

					glm::vec3 p[8] = { boundingbox[0],boundingbox[1],boundingbox[2],boundingbox[3],boundingbox[4],boundingbox[5],boundingbox[6],boundingbox[7] };
					glm::vec3 d[8] = { boundingbox[0] - c,boundingbox[1] - c,boundingbox[2] - c,boundingbox[3] - c,boundingbox[4] - c,boundingbox[5] - c,boundingbox[6] - c,boundingbox[7] - c };

					d[0] = glm::normalize(d[0]);
					d[1] = glm::normalize(d[1]);
					d[2] = glm::normalize(d[2]);
					d[3] = glm::normalize(d[3]);
					d[4] = glm::normalize(d[4]);
					d[5] = glm::normalize(d[5]);
					d[6] = glm::normalize(d[6]);
					d[7] = glm::normalize(d[7]);

					p[0] += d[0] * f;
					p[1] += d[1] * f;
					p[2] += d[2] * f;
					p[3] += d[3] * f;
					p[4] += d[4] * f;
					p[5] += d[5] * f;
					p[6] += d[6] * f;
					p[7] += d[7] * f;

					WireCubeMesh->Draw(p[0], p[1], p[2], p[3], p[4], p[5], p[7], p[6], col, view, drawpoints);
				}
			}

			// -----------------------------------------------------------------------------------
			// draw bounding sphere for model's object

			void DrawSphere(vml::views::View* view, vml::objects::Model3d_2* model, const glm::vec4& col)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for sphere rendering");
				if (!model)	vml::os::Message::Error("DebugRender : ","Null model pointer for bounding box rendering");
				if (model)
					WireSphereMesh->Draw(view, model->GetTransformedPosition(), model->GetRadius(), col);
			}

			void DrawSphere(vml::views::View* view, vml::objects::Object3d_2* model, const glm::vec4& col)
			{
				if (!view) vml::os::Message::Error("DebugRender : ","Null view matrix for sphere rendering");
				if (!model)	vml::os::Message::Error("DebugRender : ","Null model pointer for bounding box rendering");
				if (model)
					WireSphereMesh->Draw(view, model->GetAABoundingBox().GetCenter(), model->GetRadius(), col);
			}

			// -----------------------------------------------------------------------------------
			// draw model

			void DrawModelSolid(vml::views::View* view, 
								vml::objects::Model3d_2* model,
								const vml::debugrendering::DebugMaterial& material = vml::debugrendering::Material0)
			{
				
				if (view)
				{
					//	DrawAABBox(view, model, vml::colors::Yellow, true);
					//	DrawAOBBox(view, model, vml::colors::Blue, true);

					// get shader
					
					vml::shaders::GlShaderProgram* shader = GetDebugShader();

					GLuint Id = shader->GetID();

					glUseProgram(Id);

					// set shader locations

					glUniformMatrix4fv(shader->GetViewMatrixLocation()				  , 1, GL_FALSE, view->GetVptr());
					glUniformMatrix4fv(shader->GetProjectionMatrixLocation()		  , 1, GL_FALSE, view->GetPptr());
					glUniformMatrix4fv(shader->GetModelMatrixLocation()				  , 1, GL_FALSE, model->GetMptr());
					glUniformMatrix3fv(shader->GetNormalMatrixLocation()			  , 1, GL_FALSE, model->GetNVptr());
					glUniformMatrix4fv(shader->GetModelViewMatrixLocation()           , 1, GL_FALSE, model->GetMVptr());
					glUniformMatrix4fv(shader->GetModelViewProjectionMatrixLocation() , 1, GL_FALSE, model->GetMVPptr());

					glUniform4fv(GetMaterialAmbientLocation(), 1, &material.Ambient[0]);
					glUniform4fv(GetMaterialDiffuseLocation(), 1, &material.Diffuse[0]);
					glUniform4fv(GetMaterialSpecularLocation(), 1, &material.Specular[0]);
					glUniform1f(GetMaterialShininessLocation(), material.Shininess);

					DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

					glUniform4fv(GetLightAmbientLocation(), 1, &DirectionalLight.Ambient[0]);
					glUniform4fv(GetLightDiffuseLocation(), 1, &DirectionalLight.Diffuse[0]);
					glUniform4fv(GetLightSpecularLocation(), 1, &DirectionalLight.Specular[0]);
					glUniform4fv(GetLightDirectionLocation(), 1, &DirectionalLight.Direction[0]);
					glUniform4fv(GetLightCameraSpaceLocation(), 1, &DirectionalLight.CameraSpaceDirection[0]);
					glUniform1f(GetLightPowerLocation(), DirectionalLight.Power);

					// draw mesh

					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT, GL_FILL);

					glBindVertexArray(model->GetCurrentMesh()->GetVAOId());

					glDrawElements(
						GL_TRIANGLES,								// mode
						model->GetCurrentMesh()->GetIndicesCount(),	// count
						GL_UNSIGNED_INT,							// type
						(void*)0									// element array buffer offset
					);

					glBindVertexArray(0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);
					
				}
				
			}

			// -----------------------------------------------------------------------------------

			void DrawModelWire(vml::views::View* view, vml::objects::Model3d_2* model)
			{
				if (view)
				{

					// upload locations

					GLuint		Id;

					// get shader

					vml::shaders::GlShaderProgram* shader = SingleColorShader;

					Id = shader->GetID();

					glUseProgram(Id);

					// set shader locations

					glUniformMatrix4fv(shader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
					glUniformMatrix4fv(shader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
					glUniformMatrix4fv(shader->GetModelMatrixLocation(), 1, GL_FALSE, model->GetMptr());
					glUniformMatrix3fv(shader->GetNormalMatrixLocation(), 1, GL_FALSE, model->GetNVptr());
					glUniformMatrix4fv(shader->GetModelViewMatrixLocation(), 1, GL_FALSE, model->GetMVptr());
					glUniformMatrix4fv(shader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, model->GetMVPptr());

					glUniform4f(ColorLocation, WireFrameColor[0], WireFrameColor[1], WireFrameColor[2], WireFrameColor[3]);

					// draw mesh

					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT, GL_LINE);

					glBindVertexArray(model->GetCurrentMesh()->GetVAOId());

					glDrawElements(
						GL_TRIANGLES,								// mode
						model->GetCurrentMesh()->GetIndicesCount(),	// count
						GL_UNSIGNED_INT,							// type
						(void*)0									// element array buffer offset
					);

					glBindVertexArray(0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);

				}

			}

			// -----------------------------------------------------------------------------------

			void DrawModelForwardVector(vml::views::View* view, vml::objects::Model3d_2* model, float length = 5.0f)
			{
				glm::vec3 P0 = model->GetTransformedPosition();
				DrawArrow(view, P0, P0 + length * model->GetForwardVector(), vml::colors::Red);
			}

			// -----------------------------------------------------------------------------------

			void DrawModelRightVector(vml::views::View* view, vml::objects::Model3d_2 *model, float length = 5.0f)
			{
				glm::vec3 P0 = model->GetTransformedPosition();
				DrawArrow(view, P0, P0 + length * model->GetRightVector(), vml::colors::Green);
			}

			// -----------------------------------------------------------------------------------

			void DrawModelUpVector(vml::views::View* view, vml::objects::Model3d_2 *model, float length = 5.0f)
			{
				glm::vec3 P0 = model->GetTransformedPosition();
				DrawArrow(view, P0, P0 + length * model->GetUpVector(), vml::colors::Blue);
			}

			// -----------------------------------------------------------------------------------

			void DrawModelRefSystem(vml::views::View* view, vml::objects::Model3d_2 *model, float length = 5.0f)
			{
				glm::vec3 P0 = model->GetTransformedPosition();
				DrawArrow(view, P0, P0 + length * model->GetRightVector(), vml::colors::Green);
				DrawArrow(view, P0, P0 + length * model->GetUpVector(), vml::colors::Blue);
				DrawArrow(view, P0, P0 + length * model->GetForwardVector(), vml::colors::Red);
			}

			// -----------------------------------------------------------------------------------

			void DrawObjectForwardVector(vml::views::View* view, vml::objects::Object3d_2* object, float length = 5.0f)
			{
				glm::vec3 P0 = object->GetTransformedRootPosition();
				DrawArrow(view, P0, P0 + length * object->GetForwardVector(), vml::colors::Red);
			}

			void DrawObjectRightVector(vml::views::View* view, vml::objects::Object3d_2* object, float length = 5.0f)
			{
				glm::vec3 P0 = object->GetTransformedRootPosition();
				DrawArrow(view, P0, P0 + length * object->GetRightVector(), vml::colors::Green);
			}

			void DrawObjectUpVector(vml::views::View* view, vml::objects::Object3d_2* object, float length = 5.0f)
			{
				glm::vec3 P0 = object->GetTransformedRootPosition();
				DrawArrow(view, P0, P0 + length * object->GetUpVector(), vml::colors::Blue);
			}

			void DrawObjectRefSystem(vml::views::View* view, vml::objects::Object3d_2* object, float length = 5.0f)
			{
				glm::vec3 P0 = object->GetTransformedRootPosition();
				DrawArrow(view, P0, P0 + length * object->GetRightVector(), vml::colors::Green);
				DrawArrow(view, P0, P0 + length * object->GetUpVector(), vml::colors::Blue);
				DrawArrow(view, P0, P0 + length * object->GetForwardVector(), vml::colors::Red);
			}

			// -----------------------------------------------------------------------------------

			void DrawRefSystem(vml::views::View* view, const glm::vec3 &p0, float length = 5.0f,float s=1.0f)
			{
				DrawArrow(view, p0, p0 + glm::vec3(length,0,0), vml::colors::Green,length,s);
				DrawArrow(view, p0, p0 + glm::vec3(0,length,0), vml::colors::Blue, length, s);
				DrawArrow(view, p0, p0 + glm::vec3(0,0,length), vml::colors::Red, length, s);
			}

			void DrawRefSystem(vml::views::View* view, const glm::vec3& p0, const glm::vec3 &up, const glm::vec3 &right, const glm::vec3 &fwd,float length = 5.0f, float s = 1.0f)
			{
				DrawArrow(view, p0, p0 + right*length , vml::colors::Green, length, s);
				DrawArrow(view, p0, p0 + up*length    , vml::colors::Blue, length, s);
				DrawArrow(view, p0, p0 + fwd*length   , vml::colors::Red, length, s);
			}

			// -----------------------------------------------------------------------------------

			void DrawArrow(vml::views::View* view, const glm::vec3& a, const glm::vec3& b, const glm::vec4& color, float f = 0.5f,float s=1.0f)
			{
				if (view)
				{

					// get View and projection matrices

					const glm::mat4& V = view->GetView();
					const glm::mat4& P = view->GetProjection();
					glm::mat4  M;
					glm::mat4  AxisMVP;
					glm::mat4  ArrowMVP;
					glm::mat4  XAxisMV;
					glm::mat4  XArrowMV;
					glm::mat3  XAxisN, ArrowN;
					glm::mat4 ParentM;
					GLuint shaderProgram;
					float* A;
					float* ChildMatrix;
					float* NormalMatrix;
					float* MVMatrix;
					float Lenght;
					float AxisScale;
					float ArrowScale;
					float Determinant;
					glm::vec3 up, right, forward, direction;

					// keep the axisscale constant 
					// even at far distances to discern axis

					direction = b - a;

					if (direction.x > -vml::math::EPSILON && direction.x < vml::math::EPSILON) direction.x += vml::math::EPSILON;
					if (direction.y > -vml::math::EPSILON && direction.y < vml::math::EPSILON) direction.y += vml::math::EPSILON;
					if (direction.z > -vml::math::EPSILON && direction.z < vml::math::EPSILON) direction.z += vml::math::EPSILON;

					AxisScale = 0.06f * f  * s;
					ArrowScale = 0.24f * f * s;

					Lenght = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z) - ArrowScale / 2;

					// get parent matrix pointer

					A = glm::value_ptr(ParentM);

					up = glm::vec3(0, 1, 0);

					direction = glm::normalize(direction);

					if (direction.x == 0.0f && direction.y == 1.0f && direction.z == 0.0f)
					{
						right = glm::vec3(1, 0, 0);
						forward = glm::vec3(0, 0, -1);
					}
					else
					{
						right = glm::cross(up, direction);
						right = glm::normalize(right);
						forward = glm::cross(direction, right);
						forward = glm::normalize(forward);
					}

					A[0] = direction.x;
					A[1] = direction.y;
					A[2] = direction.z;
					A[3] = 0;
					A[4] = right.x;
					A[5] = right.y;
					A[6] = right.z;
					A[7] = 0;
					A[8] = forward.x;
					A[9] = forward.y;
					A[10] = forward.z;
					A[11] = 0;
					A[12] = a.x;
					A[13] = a.y;
					A[14] = a.z;
					A[15] = 1;

					// create x-axis matrix

					// get child matrix pointer

					ChildMatrix = glm::value_ptr(M);

					// compute model matrix

					ChildMatrix[0] = Lenght;
					ChildMatrix[1] = 0;
					ChildMatrix[2] = 0;
					ChildMatrix[3] = 0;
					ChildMatrix[4] = 0;
					ChildMatrix[5] = AxisScale;
					ChildMatrix[6] = 0;
					ChildMatrix[7] = 0;
					ChildMatrix[8] = 0;
					ChildMatrix[9] = 0;
					ChildMatrix[10] = AxisScale;
					ChildMatrix[11] = 0;
					ChildMatrix[12] = Lenght * 0.5f;
					ChildMatrix[13] = 0;
					ChildMatrix[14] = 0;
					ChildMatrix[15] = 1;

					// concatenate parent and child matrices

					M = ParentM * M;
					XAxisMV = V * M;
					AxisMVP = P * XAxisMV;

					// get normal matrix pointer

					NormalMatrix = glm::value_ptr(XAxisN);

					// get model * view matrix

					MVMatrix = glm::value_ptr(M);

					// normal matrix is computed as the inverse transpose
					// of the model view matrix, this causes the 
					// first 3x3 order for this matrix is meant to be divided 
					// by each scaling factor along correspondig anxes, so 
					// we need to unskew normal matrix

					Determinant = 1.0f / (+MVMatrix[0] * (MVMatrix[5] * MVMatrix[10] - MVMatrix[6] * MVMatrix[9])
										  -MVMatrix[1] * (MVMatrix[4] * MVMatrix[10] - MVMatrix[6] * MVMatrix[8])
										  +MVMatrix[2] * (MVMatrix[4] * MVMatrix[9] - MVMatrix[5] * MVMatrix[8]));

					NormalMatrix[0] = +(MVMatrix[5] * MVMatrix[10] - MVMatrix[9] * MVMatrix[6]) * Determinant;
					NormalMatrix[1] = -(MVMatrix[4] * MVMatrix[10] - MVMatrix[8] * MVMatrix[6]) * Determinant;
					NormalMatrix[2] = +(MVMatrix[4] * MVMatrix[9] - MVMatrix[8] * MVMatrix[5]) * Determinant;
					NormalMatrix[3] = -(MVMatrix[1] * MVMatrix[10] - MVMatrix[9] * MVMatrix[2]) * Determinant;
					NormalMatrix[4] = +(MVMatrix[0] * MVMatrix[10] - MVMatrix[8] * MVMatrix[2]) * Determinant;
					NormalMatrix[5] = -(MVMatrix[0] * MVMatrix[9] - MVMatrix[8] * MVMatrix[1]) * Determinant;
					NormalMatrix[6] = +(MVMatrix[1] * MVMatrix[6] - MVMatrix[5] * MVMatrix[2]) * Determinant;
					NormalMatrix[7] = -(MVMatrix[0] * MVMatrix[6] - MVMatrix[4] * MVMatrix[2]) * Determinant;
					NormalMatrix[8] = +(MVMatrix[0] * MVMatrix[5] - MVMatrix[4] * MVMatrix[1]) * Determinant;

					// create x-arrow matrix

					// get child matrix pointer

					ChildMatrix = glm::value_ptr(M);

					// compute model matrix

					ChildMatrix[0] = 0;
					ChildMatrix[1] = -ArrowScale;
					ChildMatrix[2] = 0;
					ChildMatrix[3] = 0;
					ChildMatrix[4] = ArrowScale;
					ChildMatrix[5] = 0;
					ChildMatrix[6] = 0;
					ChildMatrix[7] = 0;
					ChildMatrix[8] = 0;
					ChildMatrix[9] = 0;
					ChildMatrix[10] = ArrowScale;
					ChildMatrix[11] = 0;
					ChildMatrix[12] = Lenght;
					ChildMatrix[13] = 0;
					ChildMatrix[14] = 0;
					ChildMatrix[15] = 1;

					// concatenate parent and child matrices

					M = ParentM * M;
					XArrowMV = V * M;
					ArrowMVP = P * XArrowMV;

					// get normal matrix pointer

					NormalMatrix = glm::value_ptr(ArrowN);

					// get model * view matrix

					MVMatrix = glm::value_ptr(M);

					// normal matrix is computed as the inverse transpose
					// of the model view matrix, this causes the 
					// first 3x3 order for this matrix is meant to be divided 
					// by each scaling factor along correspondig anxes, so 
					// we need to unskew normal matrix

					Determinant = 1.0f / (+ MVMatrix[0] * (MVMatrix[5] * MVMatrix[10] - MVMatrix[6] * MVMatrix[9])
										  - MVMatrix[1] * (MVMatrix[4] * MVMatrix[10] - MVMatrix[6] * MVMatrix[8])
										  + MVMatrix[2] * (MVMatrix[4] * MVMatrix[ 9] - MVMatrix[5] * MVMatrix[8]));

					NormalMatrix[0] = +(MVMatrix[5] * MVMatrix[10] - MVMatrix[9] * MVMatrix[6]) * Determinant;
					NormalMatrix[1] = -(MVMatrix[4] * MVMatrix[10] - MVMatrix[8] * MVMatrix[6]) * Determinant;
					NormalMatrix[2] = +(MVMatrix[4] * MVMatrix[ 9] - MVMatrix[8] * MVMatrix[5]) * Determinant;
					NormalMatrix[3] = -(MVMatrix[1] * MVMatrix[10] - MVMatrix[9] * MVMatrix[2]) * Determinant;
					NormalMatrix[4] = +(MVMatrix[0] * MVMatrix[10] - MVMatrix[8] * MVMatrix[2]) * Determinant;
					NormalMatrix[5] = -(MVMatrix[0] * MVMatrix[ 9] - MVMatrix[8] * MVMatrix[1]) * Determinant;
					NormalMatrix[6] = +(MVMatrix[1] * MVMatrix[ 6] - MVMatrix[5] * MVMatrix[2]) * Determinant;
					NormalMatrix[7] = -(MVMatrix[0] * MVMatrix[ 6] - MVMatrix[4] * MVMatrix[2]) * Determinant;
					NormalMatrix[8] = +(MVMatrix[0] * MVMatrix[ 5] - MVMatrix[4] * MVMatrix[1]) * Determinant;

					// drawing

					// x axis

					vml::debugrendering::DebugDirectionalLight DirectionalLight;

					DirectionalLight.Ambient = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
					DirectionalLight.Diffuse = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
					DirectionalLight.Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					DirectionalLight.Direction = glm::vec4(1, 1, 0, 0);
					DirectionalLight.Power = 2.5f;
					DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

					vml::shaders::GlShaderProgram* shader = GetDebugShader();

					shaderProgram = shader->GetID();

					glUseProgram(shaderProgram);

					// Setup material parameters

					glUniformMatrix4fv(shader->GetViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(V));
					glUniformMatrix3fv(shader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(XAxisN));
					glUniformMatrix4fv(shader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(XAxisMV));
					glUniformMatrix4fv(shader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(AxisMVP));

					// Setup a directional light that is shining down the negative z world axis.

					glUniform4fv(GetLightAmbientLocation(), 1, &DirectionalLight.Ambient[0]);
					glUniform4fv(GetLightDiffuseLocation(), 1, &DirectionalLight.Diffuse[0]);
					glUniform4fv(GetLightSpecularLocation(), 1, &DirectionalLight.Specular[0]);
					glUniform4fv(GetLightDirectionLocation(), 1, &DirectionalLight.Direction[0]);
					glUniform4fv(GetLightCameraSpaceLocation(), 1, &DirectionalLight.CameraSpaceDirection[0]);
					glUniform1f(GetLightPowerLocation(), DirectionalLight.Power);

					// Setup a silver material to show off specular lighting.

					float Ambient[4] = { color.x, color.y, color.z,1 };
					float Diffuse[4] = { 1,1,1,1 };
					float Specular[4] = { 1,1,1,1 };

					glUniform4fv(GetMaterialAmbientLocation(), 1, &Ambient[0]);
					glUniform4fv(GetMaterialDiffuseLocation(), 1, &Diffuse[0]);
					glUniform4fv(GetMaterialSpecularLocation(), 1, &Specular[0]);
					glUniform1f(GetMaterialShininessLocation(), 4.0f);

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

					glBindVertexArray(SolidCubeMesh->GetVAOId());

					glDrawElements(
						GL_TRIANGLES,							// mode
						SolidCubeMesh->GetIndicesCount(),		// count
						GL_UNSIGNED_INT,						// type
						(void*)0								// element array buffer offset
					);

					glBindVertexArray(0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);

					// x-arrow

					glUseProgram(shaderProgram);

					glUniformMatrix4fv(shader->GetViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(V));
					glUniformMatrix3fv(shader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowN));
					glUniformMatrix4fv(shader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(XArrowMV));
					glUniformMatrix4fv(shader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(ArrowMVP));

					// Setup a directional light that is shining down the negative z world axis.

					glUniform4fv(GetLightAmbientLocation(), 1, &DirectionalLight.Ambient[0]);
					glUniform4fv(GetLightDiffuseLocation(), 1, &DirectionalLight.Diffuse[0]);
					glUniform4fv(GetLightSpecularLocation(), 1, &DirectionalLight.Specular[0]);
					glUniform4fv(GetLightDirectionLocation(), 1, &DirectionalLight.Direction[0]);
					glUniform4fv(GetLightCameraSpaceLocation(), 1, &DirectionalLight.CameraSpaceDirection[0]);
					glUniform1f(GetLightPowerLocation(), DirectionalLight.Power);

					// Setup a silver material to show off specular lighting.

					glUniform4fv(GetMaterialAmbientLocation(), 1, &Ambient[0]);
					glUniform4fv(GetMaterialDiffuseLocation(), 1, &Diffuse[0]);
					glUniform4fv(GetMaterialSpecularLocation(), 1, &Specular[0]);
					glUniform1f(GetMaterialShininessLocation(), 4.0f);

					glBindVertexArray(PyramidMesh->GetVAOId());

					glDrawElements(
									GL_TRIANGLES,				// mode
									PyramidMesh->GetIndicesCount(),	// count
									GL_UNSIGNED_INT,			// type
									(void*)0					// element array buffer offset
					);

					glBindVertexArray(0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null matrix cam for ref system rendering");
				}
			}

			// -----------------------------------------------------------------------------------

			void DrawDebugMeshSolid(vml::views::View* view, 
				                    vml::meshes::Mesh3d* mesh, 
				                    const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sc,
									const vml::debugrendering::DebugMaterial& material = vml::debugrendering::Material0)
			{
				if (view)
				{
					glm::mat4 M;	// model matrix
					glm::mat4 MV;	// model * view matrix
					glm::mat4 MVP;	// model * view * projection matrix
					glm::mat3 NV;	// viewnormal matrix
					glm::mat4 V;
					glm::mat4 P;

					// get scale

					float scx = sc.x;
					float scy = sc.y;
					float scz = sc.z;

					// get angles

					float Alpha = rot.x;
					float Phi = rot.y;
					float Theta = rot.z;

					// clamp angles

					if (Alpha < 0.0) Alpha = fmod(Alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (Alpha > vml::math::PI) Alpha = fmod(Alpha, vml::math::PIMUL2);

					if (Phi < 0.0) Phi = fmod(Phi, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (Phi > vml::math::PI) Phi = fmod(Phi, vml::math::PIMUL2);

					if (Theta < 0.0) Theta = fmod(Theta, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (Theta > vml::math::PI) Theta = fmod(Theta, vml::math::PIMUL2);

					// cache trigvalues

					float sx = sin(Alpha);
					float sy = sin(Phi);
					float sz = sin(Theta);
					float cx = cos(Alpha);
					float cy = cos(Phi);
					float cz = cos(Theta);

					// get parent matrix pointer

					float* matrix = glm::value_ptr(M);

					matrix[0] = (cy * cz) * scx;
					matrix[1] = (sx * sy * cz + cx * sz) * scx;
					matrix[2] = (-cx * sy * cz + sx * sz) * scx;
					matrix[3] = 0;
					matrix[4] = (-cy * sz) * scy;
					matrix[5] = (-sx * sy * sz + cx * cz) * scy;
					matrix[6] = (cx * sy * sz + sx * cz) * scy;
					matrix[7] = 0;
					matrix[8] = (sy)*scz;
					matrix[9] = (-sx * cy) * scz;
					matrix[10] = (cx * cy) * scz;
					matrix[11] = 0;
					matrix[12] = pos.x;
					matrix[13] = pos.y;
					matrix[14] = pos.z;
					matrix[15] = 1;

					V = view->GetView();
					P = view->GetProjection();

					MV = V * M;
					MVP = P * MV;

					// get normal matrix pointer

					float* NormalViewMatrix = glm::value_ptr(NV);

					// get model * view matrix

					float* ModelViewMatrix = glm::value_ptr(MV);

					// normal matrix is computed as the inverse transpose
					// of the model view matrix, this causes the 
					// first 3x3 order for this matrix is meant to be divided 
					// by each scaling factor along correspondig axes, so 
					// we need to unskew normal matrix

					float Determinant = + ModelViewMatrix[0] * (ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[9])
										- ModelViewMatrix[1] * (ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[8])
										+ ModelViewMatrix[2] * (ModelViewMatrix[4] * ModelViewMatrix[ 9] - ModelViewMatrix[5] * ModelViewMatrix[8]);

					if (Determinant > -vml::math::EPSILON && Determinant < vml::math::EPSILON) Determinant = 1.0f / vml::math::EPSILON;
					else Determinant = 1.0f / Determinant;

					NormalViewMatrix[0] = +(ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[6]) * Determinant;
					NormalViewMatrix[1] = -(ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[6]) * Determinant;
					NormalViewMatrix[2] = +(ModelViewMatrix[4] * ModelViewMatrix[9] - ModelViewMatrix[8] * ModelViewMatrix[5]) * Determinant;
					NormalViewMatrix[3] = -(ModelViewMatrix[1] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[4] = +(ModelViewMatrix[0] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[5] = -(ModelViewMatrix[0] * ModelViewMatrix[9] - ModelViewMatrix[8] * ModelViewMatrix[1]) * Determinant;
					NormalViewMatrix[6] = +(ModelViewMatrix[1] * ModelViewMatrix[6] - ModelViewMatrix[5] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[7] = -(ModelViewMatrix[0] * ModelViewMatrix[6] - ModelViewMatrix[4] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[8] = +(ModelViewMatrix[0] * ModelViewMatrix[5] - ModelViewMatrix[4] * ModelViewMatrix[1]) * Determinant;

					// get shader

					GLuint Id = DebugShader->GetID();

					glUseProgram(Id);

					// set shader locations

					glUniformMatrix4fv(DebugShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
					glUniformMatrix4fv(DebugShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
					glUniformMatrix4fv(DebugShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
					glUniformMatrix3fv(DebugShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
					glUniformMatrix4fv(DebugShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(DebugShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

					glUniform4fv(MaterialAmbientLocation, 1, &material.Ambient[0]);
					glUniform4fv(MaterialDiffuseLocation, 1, &material.Diffuse[0]);
					glUniform4fv(MaterialSpecularLocation, 1, &material.Specular[0]);
					glUniform1f(MaterialShininessLocation, material.Shininess);

					DirectionalLight.CameraSpaceDirection = glm::normalize(view->GetView() * DirectionalLight.Direction);

					glUniform4fv(LightAmbientLocation, 1, &DirectionalLight.Ambient[0]);
					glUniform4fv(LightDiffuseLocation, 1, &DirectionalLight.Diffuse[0]);
					glUniform4fv(LightSpecularLocation, 1, &DirectionalLight.Specular[0]);
					glUniform4fv(LightDirectionLocation, 1, &DirectionalLight.Direction[0]);
					glUniform4fv(LightCameraSpaceLocation, 1, &DirectionalLight.CameraSpaceDirection[0]);
					glUniform1f(LightPowerLocation, DirectionalLight.Power);

					// draw mesh

					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT, GL_FILL);

					glBindVertexArray(mesh->GetVAOId());

					glDrawElements(
						GL_TRIANGLES,				// mode
						mesh->GetIndicesCount(),	// count
						GL_UNSIGNED_INT,			// type
						(void*)0					// element array buffer offset
					);

					glBindVertexArray(0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);
				}

			}

			// -----------------------------------------------------------------------------------

			void DrawDebugMeshWire(vml::views::View* view, vml::meshes::Mesh3d* mesh, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sc, const glm::vec4& col)
			{
				if (view)
				{
					glm::mat4 M;							// model matrix
					glm::mat4 MV;							// model * view matrix
					glm::mat4 MVP;							// model * view * projection matrix
					glm::mat3 NV;							// viewnormal matrix
					glm::mat4 V;
					glm::mat4 P;

					// get scale

					float scx = sc.x;
					float scy = sc.y;
					float scz = sc.z;

					// get angles

					float Alpha = rot.x;
					float Phi = rot.y;
					float Theta = rot.z;

					// clamp angles

					if (Alpha < 0.0) Alpha = fmod(Alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (Alpha > vml::math::PI) Alpha = fmod(Alpha, vml::math::PIMUL2);

					if (Phi < 0.0) Phi = fmod(Phi, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (Phi > vml::math::PI) Phi = fmod(Phi, vml::math::PIMUL2);

					if (Theta < 0.0) Theta = fmod(Theta, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (Theta > vml::math::PI) Theta = fmod(Theta, vml::math::PIMUL2);

					// cache trigvalues

					float sx = sin(Alpha);
					float sy = sin(Phi);
					float sz = sin(Theta);
					float cx = cos(Alpha);
					float cy = cos(Phi);
					float cz = cos(Theta);

					// get parent matrix pointer

					float* matrix = glm::value_ptr(M);

					matrix[0] = (cy * cz) * scx;
					matrix[1] = (sx * sy * cz + cx * sz) * scx;
					matrix[2] = (-cx * sy * cz + sx * sz) * scx;
					matrix[3] = 0;
					matrix[4] = (-cy * sz) * scy;
					matrix[5] = (-sx * sy * sz + cx * cz) * scy;
					matrix[6] = (cx * sy * sz + sx * cz) * scy;
					matrix[7] = 0;
					matrix[8] = (sy)*scz;
					matrix[9] = (-sx * cy) * scz;
					matrix[10] = (cx * cy) * scz;
					matrix[11] = 0;
					matrix[12] = pos.x;
					matrix[13] = pos.y;
					matrix[14] = pos.z;
					matrix[15] = 1;

					V = view->GetView();
					P = view->GetProjection();

					MV = V * M;
					MVP = P * MV;

					// get normal matrix pointer

					float* NormalViewMatrix = glm::value_ptr(NV);

					// get model * view matrix

					float* ModelViewMatrix = glm::value_ptr(MV);

					// normal matrix is computed as the inverse transpose
					// of the model view matrix, this causes the 
					// first 3x3 order for this matrix is meant to be divided 
					// by each scaling factor along correspondig axes, so 
					// we need to unskew normal matrix

					float Determinant = +ModelViewMatrix[0] * (ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[9])
										- ModelViewMatrix[1] * (ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[6] * ModelViewMatrix[8])
										+ ModelViewMatrix[2] * (ModelViewMatrix[4] * ModelViewMatrix[9] - ModelViewMatrix[5] * ModelViewMatrix[8]);

					if (Determinant > -vml::math::EPSILON && Determinant < vml::math::EPSILON) Determinant = 1.0f / vml::math::EPSILON;
					else Determinant = 1.0f / Determinant;

					NormalViewMatrix[0] = +(ModelViewMatrix[5] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[6]) * Determinant;
					NormalViewMatrix[1] = -(ModelViewMatrix[4] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[6]) * Determinant;
					NormalViewMatrix[2] = +(ModelViewMatrix[4] * ModelViewMatrix[9] - ModelViewMatrix[8] * ModelViewMatrix[5]) * Determinant;
					NormalViewMatrix[3] = -(ModelViewMatrix[1] * ModelViewMatrix[10] - ModelViewMatrix[9] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[4] = +(ModelViewMatrix[0] * ModelViewMatrix[10] - ModelViewMatrix[8] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[5] = -(ModelViewMatrix[0] * ModelViewMatrix[9] - ModelViewMatrix[8] * ModelViewMatrix[1]) * Determinant;
					NormalViewMatrix[6] = +(ModelViewMatrix[1] * ModelViewMatrix[6] - ModelViewMatrix[5] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[7] = -(ModelViewMatrix[0] * ModelViewMatrix[6] - ModelViewMatrix[4] * ModelViewMatrix[2]) * Determinant;
					NormalViewMatrix[8] = +(ModelViewMatrix[0] * ModelViewMatrix[5] - ModelViewMatrix[4] * ModelViewMatrix[1]) * Determinant;

					// upload locations

					GLuint		Id;

					// get shader

					Id = SingleColorShader->GetID();

					glUseProgram(Id);

					// set shader locations

					glUniformMatrix4fv(SingleColorShader->GetViewMatrixLocation(), 1, GL_FALSE, view->GetVptr());
					glUniformMatrix4fv(SingleColorShader->GetProjectionMatrixLocation(), 1, GL_FALSE, view->GetPptr());
					glUniformMatrix4fv(SingleColorShader->GetModelMatrixLocation(), 1, GL_FALSE, glm::value_ptr(M));
					glUniformMatrix3fv(SingleColorShader->GetNormalMatrixLocation(), 1, GL_FALSE, glm::value_ptr(NV));
					glUniformMatrix4fv(SingleColorShader->GetModelViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MV));
					glUniformMatrix4fv(SingleColorShader->GetModelViewProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(MVP));

					glUniform4f(ColorLocation, col[0], col[1], col[2], col[3]);

					// draw mesh

					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					glBindVertexArray(mesh->GetVAOId());

					glDrawElements(
						GL_TRIANGLES,				// mode
						mesh->GetIndicesCount(),	// count
						GL_UNSIGNED_INT,			// type
						(void*)0					// element array buffer offset
					);

					glBindVertexArray(0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glUseProgram(0);
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for mesh rendering");
				}

			}

			// -----------------------------------------------------------------------------------

			void DrawDebugMesh( vml::views::View* view,
								vml::meshes::Mesh3d* mesh,
								const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sc,
								int mode)
			{
				switch (mode)
				{
					case DRAW_WIRE  : DrawDebugMeshWire(view, mesh, pos, rot, sc, WireFrameColor); break;
					case DRAW_SOLID : DrawDebugMeshSolid(view, mesh, pos, rot, sc);				   break;
				}
			}

			// -----------------------------------------------------------------------------------
			// test axis aligned bounding box vs infinte ray

			bool TriangleVsRay(vml::views::View* view,
							   const glm::vec3& p0, const glm::vec3& p1,const glm::vec3& p2,
							   float MouseX, float MouseY,
							   glm::vec3& p, float& tmin)
			{
				if (view)
				{
					glm::vec3 start, end;
					view->UnprojectPosition(MouseX, MouseY, start, end);
					if (vml::geo3d::intersections::TriangleVsRay(p0, p1, p2, start, end, p, tmin) == vml::geo3d::intersections::TRIANGLE_INTERSECTS_RAY)
						return true;
					return false;
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for triangle vs ray");
				}
			}

			// -----------------------------------------------------------------------------------
			// test axis aligned bounding box vs infinte ray

			bool QuadVsRay(vml::views::View* view,
						   const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
						   float MouseX, float MouseY,
						   glm::vec3& p, float& tmin)
			{
				if (view)
				{
					glm::vec3 start, end;
					view->UnprojectPosition(MouseX, MouseY, start, end);
					if (vml::geo3d::intersections::TriangleVsRay(p0, p1, p2, start, end, p, tmin) == vml::geo3d::intersections::TRIANGLE_INTERSECTS_RAY)
							return true;
					if (vml::geo3d::intersections::TriangleVsRay(p1, p2, p3, start, end, p, tmin) == vml::geo3d::intersections::TRIANGLE_INTERSECTS_RAY)
							return true;
					return false;
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for quad vs ray");
				}
			}

			// -----------------------------------------------------------------------------------
			// test axis aligned bounding box vs infinte ray
			
			bool AABBoxVsRay(vml::views::View* view,
							 const glm::vec3 &boundingboxmin,const glm::vec3 &boundingboxmax,
							 float MouseX, float MouseY,
						  	 glm::vec3& p, glm::vec3& q,float &tmin,float &tmax)
			{
				if (view)
				{
					glm::vec3 start, end;
					view->UnprojectPosition(MouseX, MouseY, start, end);
					if (vml::geo3d::intersections::AABBoxVsRay(boundingboxmin, boundingboxmax, 
															   start, end, 
															   p, q, tmin, tmax) == vml::geo3d::intersections::AABBOX_RAY_INTERSECTS)
						return true;
					return false;
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
				}
			}
		
			// -----------------------------------------------------------------------------------
			// test axis aligned bounding box vs infinte ray

			bool AABBoxVsRay(vml::views::View* view,
							 vml::objects::Model3d_2* model,
							 float MouseX, float MouseY,
							 glm::vec3& p, glm::vec3& q, float& tmin, float& tmax)
			{
				if (view)
				{
					glm::vec3 start, end;
					view->UnprojectPosition(MouseX, MouseY, start, end);
					if (vml::geo3d::intersections::AABBoxVsRay(model->GetAABoundingBox().GetMin(), model->GetAABoundingBox().GetMax(),
															   start, end,
															   p, q, tmin, tmax) == vml::geo3d::intersections::AABBOX_RAY_INTERSECTS)
						return true;
					return false;
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
				}
			}

			// -----------------------------------------------------------------------------------
			// test axis aligned bounding box vs infinte ray

			bool AABBoxVsRay(vml::views::View* view,
							 vml::objects::Object3d_2* object,
							 float MouseX, float MouseY,
							 glm::vec3& p, glm::vec3& q, float& tmin, float& tmax)
			{
				if (view)
				{
					glm::vec3 start, end;
					view->UnprojectPosition(MouseX, MouseY, start, end);
					if (vml::geo3d::intersections::AABBoxVsRay(object->GetAABoundingBox().GetMin(), object->GetAABoundingBox().GetMax(),
						start, end,
						p, q, tmin, tmax) == vml::geo3d::intersections::AABBOX_RAY_INTERSECTS)
						return true;
					return false;
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
				}
			}

			// -----------------------------------------------------------------------------------
			// test axis aligned bounding box vs infinte ray

			bool AOBBoxVsRay(vml::views::View* view,
							 vml::objects::Model3d_2* model,
							 float MouseX, float MouseY,
							 glm::vec3& p, glm::vec3& q, float& tmin, float& tmax)
			{
				if (view)
				{
					glm::vec3 start, end;
					const glm::vec3& boundingboxmin = model->GetCurrentMesh()->GetBoundingBox().GetMin();
					const glm::vec3& boundingboxmax = model->GetCurrentMesh()->GetBoundingBox().GetMax();
					view->UnprojectPosition(MouseX, MouseY, start, end);
					if (vml::geo3d::intersections::AOBBoxVsRay(start, end, boundingboxmin, boundingboxmax, model->GetM(), p, q, tmin, tmax) == vml::geo3d::intersections::AOBBOX_RAY_INTERSECTS)
						return true;
					return false;
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
				}
			}

			// -----------------------------------------------------------------------------------
			// test sphere against infinite ray

			bool SphereVsRay(vml::views::View* view,
							 const glm::vec3& pos, const float radius,
							 float MouseX, float MouseY, glm::vec3& p, glm::vec3& q,
							 float& tmin, float& tmax)
			{
				if (view)
				{
					glm::vec3 start, end;
					view->UnprojectPosition(MouseX, MouseY, start, end);
					if (vml::geo3d::intersections::SphereVsRay(pos, radius,
						start, end, p, q, tmin, tmax) == vml::geo3d::intersections::DO_INTERSECTS)
						return true;
					return false;
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for sphere vs ray");
				}
			}

			// -----------------------------------------------------------------------------------
			// test sphere against infinite ray

			bool SphereVsRay(vml::views::View* view,
							 vml::objects::Model3d_2* model,
							 float MouseX, float MouseY, glm::vec3& p, glm::vec3& q,
							 float& tmin, float& tmax)
			{
				if (view)
				{
					glm::vec3 start, end;
					view->UnprojectPosition(MouseX, MouseY, start, end);
					if (vml::geo3d::intersections::SphereVsRay(model->GetAABoundingBox().GetCenter(), model->GetRadius(),
						start, end,
						p, q, tmin, tmax) == vml::geo3d::intersections::DO_INTERSECTS)
						return true;
					return false;
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
				}
			}

			// -----------------------------------------------------------------------------------
			// test sphere against infinite ray

			bool SphereVsRay(vml::views::View* view,
							 vml::objects::Object3d_2* object,
							 float MouseX, float MouseY, glm::vec3& p, glm::vec3& q,
							 float& tmin, float& tmax)
			{
				if (view)
				{
					glm::vec3 start, end;
					view->UnprojectPosition(MouseX, MouseY, start, end);
					if (vml::geo3d::intersections::SphereVsRay(object->GetAABoundingBox().GetCenter(), object->GetRadius(),
						start, end,
						p, q, tmin, tmax) == vml::geo3d::intersections::DO_INTERSECTS)
						return true;
					return false;
				}
				else
				{
					vml::os::Message::Error("DebugRender : ","Null view  matrix for aabbox vs ray");
				}
			}

			// -----------------------------------------------------------------------------------
			// Compute euler matrix

			void ComputeEulerMatrix(vml::objects::Model3d_2* model, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scaling = glm::vec3(1, 1, 1))
			{
				float scx = scaling.x;
				float scy = scaling.y;
				float scz = scaling.z;

				// get angles

				float alpha = rotation.x;
				float phi = rotation.y;
				float theta = rotation.z;

				// clamp angles

				if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
				else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

				if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
				else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

				if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
				else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

				// cache trigvalues

				float sx = sin(alpha);
				float sy = sin(phi);
				float sz = sin(theta);
				float cx = cos(alpha);
				float cy = cos(phi);
				float cz = cos(theta);

				// get parent matrix pointer

				float* matrix = model->GetMptr();

				matrix[0] = (cy * cz) * scx;
				matrix[1] = (sx * sy * cz + cx * sz) * scx;
				matrix[2] = (-cx * sy * cz + sx * sz) * scx;
				matrix[3] = 0;
				matrix[4] = (-cy * sz) * scy;
				matrix[5] = (-sx * sy * sz + cx * cz) * scy;
				matrix[6] = (cx * sy * sz + sx * cz) * scy;
				matrix[7] = 0;
				matrix[8] = (sy)*scz;
				matrix[9] = (-sx * cy) * scz;
				matrix[10] = (cx * cy) * scz;
				matrix[11] = 0;
				matrix[12] = position.x;
				matrix[13] = position.y;
				matrix[14] = position.z;
				matrix[15] = 1;
			}

			// -----------------------------------------------------------------------------------
			// Compute euler matrix

			glm::mat4 ComputeEulerMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scaling = glm::vec3(1, 1, 1))
			{
				float scx = scaling.x;
				float scy = scaling.y;
				float scz = scaling.z;

				// get angles

				float alpha = rotation.x;
				float phi   = rotation.y;
				float theta = rotation.z;

				// clamp angles

				if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
				else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

				if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
				else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

				if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
				else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

				// cache trigvalues

				float sx = sin(alpha);
				float sy = sin(phi);
				float sz = sin(theta);
				float cx = cos(alpha);
				float cy = cos(phi);
				float cz = cos(theta);

				// get parent matrix pointer
				
				glm::mat4 m( 1,0,0,0,
							 0,1,0,0,
							 0,0,1,0,
							 0,0,0,1 );

				float* matrix = glm::value_ptr(m);

				matrix[0] = (cy * cz) * scx;
				matrix[1] = (sx * sy * cz + cx * sz) * scx;
				matrix[2] = (-cx * sy * cz + sx * sz) * scx;
				matrix[3] = 0;
				matrix[4] = (-cy * sz) * scy;
				matrix[5] = (-sx * sy * sz + cx * cz) * scy;
				matrix[6] = (cx * sy * sz + sx * cz) * scy;
				matrix[7] = 0;
				matrix[8] = (sy)*scz;
				matrix[9] = (-sx * cy) * scz;
				matrix[10] = (cx * cy) * scz;
				matrix[11] = 0;
				matrix[12] = position.x;
				matrix[13] = position.y;
				matrix[14] = position.z;
				matrix[15] = 1;

				return m;
			}

			// -----------------------------------------------------------------------------------
			// init debug shader

			void Init()
			{
				// logging initialisation

				if (vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("DebugRender : ","Already initted");

				std::string currentdate = vml::Logger::GetInstance()->GetCurrentDate();
				vml::Logger::GetInstance()->Out(currentdate, "Debug Render", "Initting Debug Render", "In progress...");

				// sets flags to 0

				Flags = 0;

				// create debug meshes

				CheckeredPlaneMesh		   = new vml::debugrendering::CheckeredPlane();
				QuadMesh				   = new vml::debugrendering::Quad();
				QuadPerVertexColorMesh	   = new vml::debugrendering::QuadPerVertexColor();
				QuadTexturedMesh		   = new vml::debugrendering::TexturedQuad();
				AlphaTexturedQuadMesh	   = new vml::debugrendering::AlphaTexturedQuad();
				AlphaTexturedColorQuadMesh = new vml::debugrendering::AlphaColorTexturedQuad();
				TriangleMesh			   = new vml::debugrendering::Triangle();
				TrianglePerVertexColorMesh = new vml::debugrendering::TrianglePerVertexColor();
				TriangleTexturedMesh	   = new vml::debugrendering::TexturedTriangle;
				WireCubeMesh			   = new vml::debugrendering::WireCube();
				SolidCubeMesh			   = new vml::debugrendering::SolidCube();
				PyramidMesh				   = new vml::debugrendering::Pyramid();
				WireSphereMesh			   = new vml::debugrendering::Sphere();
				BillBoardMesh			   = new vml::debugrendering::BillBoard();
				SinglePoint				   = new vml::debugrendering::SinglePoint();
				Segment					   = new vml::debugrendering::Segment();

				std::string texturestorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\textures";

				Texture = new vml::textures::Texture(texturestorepath + "\\debug\\signaltest.png");

				// init memory ends

				vml::bits32::SetToTrue(Flags, vml::InternalFlags::INITTED);
				vml::Logger::GetInstance()->Out(currentdate, "Debug Render", "Initting Debug Render", "Done");

				// sets initial debug directional light, direction vector has w=0

				DirectionalLight.Ambient   = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);
				DirectionalLight.Diffuse   = glm::vec4( 1.6f, 1.6f, 1.6f, 1.0f);
				DirectionalLight.Specular  = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);
				DirectionalLight.Direction = glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f );			
			//	DirectionalLight.Direction = glm::vec4(-1.0f, 0.0f, 0.1f, 0.0f);
				DirectionalLight.Power     = 1.5f;
				
				// attach debug phong dir shader

				GLuint Id;

				std::string shaderstorepath = vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\shaders";

				DebugShader = vml::ShaderStore->Load(shaderstorepath + "\\debug\\debug_phong_dir.shd");		

				// retrive shader color locations for debug shader

				Id = DebugShader->GetID();
				glUseProgram(Id);

				MaterialAmbientLocation   = glGetUniformLocation(Id, "Material.ambient");
				MaterialDiffuseLocation   = glGetUniformLocation(Id, "Material.diffuse");
				MaterialSpecularLocation  = glGetUniformLocation(Id, "Material.specular");
				MaterialShininessLocation = glGetUniformLocation(Id, "Material.shininess");
				LightAmbientLocation      = glGetUniformLocation(Id, "DirectionalLight.ambient");
				LightDiffuseLocation      = glGetUniformLocation(Id, "DirectionalLight.diffuse");
				LightSpecularLocation     = glGetUniformLocation(Id, "DirectionalLight.specular");
				LightDirectionLocation    = glGetUniformLocation(Id, "DirectionalLight.direction");
				LightPowerLocation        = glGetUniformLocation(Id, "DirectionalLight.power");
				LightCameraSpaceLocation  = glGetUniformLocation(Id, "DirectionalLight.cameraspacedirection");

				// validate debug shaders locations

				if (MaterialAmbientLocation   == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'Material.ambient' uniform, check shader source code");
				if (MaterialDiffuseLocation   == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'MaterialDiffuseLocation' uniform, check shader source code");
				if (MaterialSpecularLocation  == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'MaterialSpecularLocation' uniform, check shader source code");
				if (MaterialShininessLocation == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'MaterialShininessLocation' uniform, check shader source code");
				if (LightAmbientLocation      == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.ambient' uniform, check shader source code");
				if (LightDiffuseLocation      == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.diffuse' uniform, check shader source code");
				if (LightSpecularLocation     == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.specular' uniform, check shader source code");
				if (LightPowerLocation        == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.power' uniform, check shader source code");
				if (LightCameraSpaceLocation  == -1) vml::os::Message::Error("GlProgram : ", "debug_phong_dir requires 'DirectionalLight.cameraspacedirection' uniform, check shader source code");

				// retrive shader color locations for single color debug shader

				SingleColorShader = vml::ShaderStore->Load(shaderstorepath + "\\debug\\debug_single_color.shd");

				Id= SingleColorShader->GetID();
				glUseProgram(Id);
				ColorLocation = glGetUniformLocation(Id, "Color");
				glUseProgram(0);

				// retrieve shader alpha textured

				TextureShaderAlpha = vml::ShaderStore->Load(shaderstorepath + "\\debug\\debug_texture_alpha.shd");

				Id = TextureShaderAlpha->GetID();

				// retrieve shader alpha textured colored
 
				TextureShaderAlphaColor = vml::ShaderStore->Load(shaderstorepath + "\\debug\\debug_texture_alpha_color.shd");

				Id = TextureShaderAlpha->GetID();

			}

			// -----------------------------------------------------------------------------------
			// begin render state , starts with default values

			void Begin()
			{
				if (!vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("DebugRender : ","Not initted");

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				// Enable depth test
				glEnable(GL_DEPTH_TEST);
				// Accept fragment if it closer to the camera than the former one
				glDepthFunc(GL_LESS);
				// Cull triangles whose normal is not towards the camera
				glEnable(GL_CULL_FACE);
				// sets ccw face 
				glFrontFace(GL_CCW);
				// unbind any texture
				glBindTexture(GL_TEXTURE_2D, 0);
				// unbind any texture 2d array
				glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
				// disable blending
				glDisable(GL_BLEND);
				// ubind texture sampler
				glBindSampler(GL_TEXTURE0, 0);
				// set clear color
				glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.w);
				// Clear required buffers
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}

			// -----------------------------------------------------------------------------------
			// set default render states back

			void End() const
			{
				if (!vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("DebugRender : ","Not initted");

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				// Enable depth test
				glDisable(GL_DEPTH_TEST);
				// Accept fragment if it closer to the camera than the former one
				glDepthFunc(GL_LESS);
				// Cull triangles whose normal is not towards the camera
				glDisable(GL_CULL_FACE);
				// unbind any texture
				glBindTexture(GL_TEXTURE_2D, 0);
				// unbind any texture 2d array
				glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
				// disable blending
				glDisable(GL_BLEND);
				// no shaders
				glUseProgram(0);
			}

			// -----------------------------------------------------------------------------------
			// closes debug renderer and release memory

			bool Close()
			{
				if (!vml::bits32::Get(Flags, vml::InternalFlags::INITTED))
					vml::os::Message::Error("DebugRender : ","Not initted");
				std::string currentdate = vml::Logger::GetInstance()->GetCurrentDate();
				vml::Logger::GetInstance()->Out(currentdate, "Debug Render", "Closing Debug Render", "In progress...");

				// sets flags to 0

				Flags = 0;

				// delete meshes

				vml::SafeDelete(CheckeredPlaneMesh);
				vml::SafeDelete(QuadMesh);
				vml::SafeDelete(QuadPerVertexColorMesh);
				vml::SafeDelete(QuadTexturedMesh);
				vml::SafeDelete(AlphaTexturedQuadMesh);
				vml::SafeDelete(AlphaTexturedColorQuadMesh);
				vml::SafeDelete(TriangleMesh);
				vml::SafeDelete(TrianglePerVertexColorMesh);
				vml::SafeDelete(TriangleTexturedMesh);
				vml::SafeDelete(WireCubeMesh);
				vml::SafeDelete(SolidCubeMesh);
				vml::SafeDelete(BillBoardMesh);
				vml::SafeDelete(SinglePoint);
				vml::SafeDelete(WireSphereMesh);
				vml::SafeDelete(PyramidMesh);
				vml::SafeDelete(Segment);

				// delete texture

				vml::SafeDelete(Texture);

				// log out 

				vml::Logger::GetInstance()->Out(currentdate, "Debug Render", "Closing Debug Render", "Done");

				return true;

			}

			// -----------------------------------------------------------------------------------
			// ctor / dtor
				
			OpenglDebugRender()
			{
				Flags			           = 0;
				ClearColor		           = vml::colors::CornFlowerBlue;
				WireFrameColor             = vml::colors::White;
				
				// meshes

				CheckeredPlaneMesh         = nullptr;
				QuadMesh		           = nullptr;
				QuadPerVertexColorMesh     = nullptr;
				QuadTexturedMesh		   = nullptr;
				TriangleMesh	           = nullptr;
				TrianglePerVertexColorMesh = nullptr;
				TriangleTexturedMesh	   = nullptr;
				WireCubeMesh			   = nullptr;
				Texture					   = nullptr;
				BillBoardMesh			   = nullptr;
				SinglePoint				   = nullptr;
				WireSphereMesh			   = nullptr;
				AlphaTexturedQuadMesh	   = nullptr;
				AlphaTexturedColorQuadMesh = nullptr;
				SolidCubeMesh			   = nullptr;
				PyramidMesh				   = nullptr;
				Segment					   = nullptr;

				// shaders

				DebugShader				   = nullptr;
				TextureShaderAlpha		   = nullptr;
				SingleColorShader		   = nullptr;
				TextureShaderAlphaColor	   = nullptr;

				// shaders locations

				LightAmbientLocation	   = -1;
				LightDiffuseLocation	   = -1;
				LightSpecularLocation	   = -1;
				LightDirectionLocation	   = -1;
				LightPowerLocation		   = -1;
				LightCameraSpaceLocation   = -1;

				MaterialAmbientLocation    = -1;
				MaterialDiffuseLocation    = -1;
				MaterialSpecularLocation   = -1;
				MaterialShininessLocation  = -1;

				Init();
			}

			~OpenglDebugRender()
			{
				Close();
			}
			
	};
	
}


