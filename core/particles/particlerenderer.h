#pragma once


class ParticleRenderer
{

	public:

		// ---------------------------------------------------------------
		// 

		void DrawBoundingBox(vml::views::View* view, ParticleNodeBase* node, bool forcedraw = false) const
		{
			if (forcedraw)
				vml::DebugRender->DrawAABBox(view, node->GetAABBoxMin(), node->GetAABBoxMax(), vml::colors::Yellow, true);
			else {
				if (!node->IsBoundingBoxVisible())
					return;
				vml::DebugRender->DrawAABBox(view, node->GetAABBoxMin(), node->GetAABBoxMax(), vml::colors::Yellow, true);
			}
		}

		// ---------------------------------------------------------------
		// 

		void DrawBoundingSphere(vml::views::View* view, ParticleNodeBase* node, bool forcedraw = false) const
		{
			if (forcedraw)
				vml::DebugRender->DrawSphere(view, node->GetAABBoxCenter(), node->GetRadius(), vml::colors::Red);
			else {
				if (!node->IsBoundingSphereVisible())
					return;
				vml::DebugRender->DrawSphere(view, node->GetAABBoxCenter(), node->GetRadius(), vml::colors::Red);
			}
		}
		// ---------------------------------------------------------------
		// 

		void DrawRefSystem(vml::views::View* view, ParticleNodeBase* node, bool forcedraw = false) const
		{
			if (forcedraw)
				vml::DebugRender->DrawRefSystem(view, node->GetPosition(), node->GetUpVector(), node->GetRightVector(), node->GetForwardVector(), 15.0f, 0.1f);
			else {
				if (node->IsRefSystemVisible())
					vml::DebugRender->DrawRefSystem(view, node->GetPosition(), node->GetUpVector(), node->GetRightVector(), node->GetForwardVector(), 15.0f, 0.1f);
			}
		}

		// ---------------------------------------------------------------
		// 

		void DrawWireBuffer(vml::views::View* view, ParticleNodeBase* node) const
		{
			const float* posbuffer = node->GetParticlePositionBuffer();

			for (size_t i = 0; i < node->GetParticleBufferSize(); ++i)
			{
				size_t j = i * 16;

				glm::vec3 p0(posbuffer[j], posbuffer[j + 1], posbuffer[j + 2]);
				glm::vec3 p1(posbuffer[j + 4], posbuffer[j + 5], posbuffer[j + 6]);
				glm::vec3 p2(posbuffer[j + 8], posbuffer[j + 9], posbuffer[j + 10]);
				glm::vec3 p3(posbuffer[j + 12], posbuffer[j + 13], posbuffer[j + 14]);

				vml::DebugRender->DrawQuad(view, p0, p1, p2, p3, vml::colors::Yellow, 0);
			}
		}

		// ---------------------------------------------------------------
		// 

		void DrawSolidBuffer(vml::views::View* view, ParticleNodeBase* node) const
		{
			const float* posbuffer = node->GetParticlePositionBuffer();
			const float* colbuffer = node->GetParticleColorBuffer();

			for (size_t i = 0; i < node->GetParticleBufferSize(); ++i)
			{
				size_t j = i * 16;

				glm::vec3 p0(posbuffer[j], posbuffer[j + 1], posbuffer[j + 2]);
				glm::vec3 p1(posbuffer[j + 4], posbuffer[j + 5], posbuffer[j + 6]);
				glm::vec3 p2(posbuffer[j + 8], posbuffer[j + 9], posbuffer[j + 10]);
				glm::vec3 p3(posbuffer[j + 12], posbuffer[j + 13], posbuffer[j + 14]);

				glm::vec4 c0(colbuffer[j], colbuffer[j + 1], colbuffer[j + 2], colbuffer[j + 3]);
				glm::vec4 c1(colbuffer[j + 4], colbuffer[j + 5], colbuffer[j + 6], colbuffer[j + 7]);
				glm::vec4 c2(colbuffer[j + 8], colbuffer[j + 9], colbuffer[j + 10], colbuffer[j + 11]);
				glm::vec4 c3(colbuffer[j + 12], colbuffer[j + 13], colbuffer[j + 14], colbuffer[j + 15]);

				vml::DebugRender->DrawQuad(view, p0, p1, p2, p3, c0, c1, c2, c3);

			}
		}

		// ---------------------------------------------------------------
		// 

		void DrawTexturedBuffer(vml::views::View* view, ParticleNodeBase* node) const
		{

			//	std::cout << "textured bills" << std::endl;

			const float* posbuffer = node->GetParticlePositionBuffer();
			const float* uvbuffer = node->GetParticleUVBuffer();

			GLuint texid = 0;

			if (node->GetTexture())
				texid = node->GetTexture()->GetID();
			else
				vml::DebugRender->GetTextureID();

			for (size_t i = 0; i < node->GetParticleBufferSize(); ++i)
			{
				size_t j = i * 16;

				glm::vec3 p0(posbuffer[j], posbuffer[j + 1], posbuffer[j + 2]);
				glm::vec3 p1(posbuffer[j + 4], posbuffer[j + 5], posbuffer[j + 6]);
				glm::vec3 p2(posbuffer[j + 8], posbuffer[j + 9], posbuffer[j + 10]);
				glm::vec3 p3(posbuffer[j + 12], posbuffer[j + 13], posbuffer[j + 14]);

				size_t k = i * 8;

				glm::vec2 u0(uvbuffer[k], uvbuffer[k + 1]);
				glm::vec2 u1(uvbuffer[k + 2], uvbuffer[k + 3]);
				glm::vec2 u2(uvbuffer[k + 4], uvbuffer[k + 5]);
				glm::vec2 u3(uvbuffer[k + 6], uvbuffer[k + 7]);

				vml::DebugRender->DrawTexturedQuad(view, p0, p1, p2, p3, texid);

			}
	
		}

		// ---------------------------------------------------------------
		// 

		void DrawTexturedAlphaBuffer(vml::views::View* view, ParticleNodeBase* node,int blendmode) const
		{
		
			const float* posbuffer = node->GetParticlePositionBuffer();
			const float* uvbuffer = node->GetParticleUVBuffer();

			GLuint texid = 0;

			if (node->GetTexture())
				texid = node->GetTexture()->GetID();
			else
				vml::DebugRender->GetTextureID();

			for (size_t i = 0; i < node->GetParticleBufferSize(); ++i)
			{
				size_t j = i * 16;

				glm::vec3 p0(posbuffer[j], posbuffer[j + 1], posbuffer[j + 2]);
				glm::vec3 p1(posbuffer[j + 4], posbuffer[j + 5], posbuffer[j + 6]);
				glm::vec3 p2(posbuffer[j + 8], posbuffer[j + 9], posbuffer[j + 10]);
				glm::vec3 p3(posbuffer[j + 12], posbuffer[j + 13], posbuffer[j + 14]);

				size_t k = i * 8;

				glm::vec2 u0(uvbuffer[k], uvbuffer[k + 1]);
				glm::vec2 u1(uvbuffer[k + 2], uvbuffer[k + 3]);
				glm::vec2 u2(uvbuffer[k + 4], uvbuffer[k + 5]);
				glm::vec2 u3(uvbuffer[k + 6], uvbuffer[k + 7]);

				vml::DebugRender->DrawAlphaTexturedQuad(view, p0, p1, p2, p3, texid, 1.0f, blendmode);

			}
			
		}

		// ---------------------------------------------------------------
		// 

		void DrawTexturedAlphaColorBuffer(vml::views::View* view, ParticleNodeBase* node,int blendmode) const
		{
			const float* posbuffer = node->GetParticlePositionBuffer();
			const float* uvbuffer = node->GetParticleUVBuffer();
			const float* colbuffer = node->GetParticleColorBuffer();

			GLuint texid = 0;

			if (node->GetTexture())
				texid = node->GetTexture()->GetID();
			else
				vml::DebugRender->GetTextureID();
					
			for (size_t i = 0; i < node->GetParticleBufferSize(); ++i)
			{
				size_t j = i * 16;

				glm::vec3 p0(posbuffer[j     ], posbuffer[j +  1], posbuffer[j +  2]);
				glm::vec3 p1(posbuffer[j +  4], posbuffer[j +  5], posbuffer[j +  6]);
				glm::vec3 p2(posbuffer[j +  8], posbuffer[j +  9], posbuffer[j + 10]);
				glm::vec3 p3(posbuffer[j + 12], posbuffer[j + 13], posbuffer[j + 14]);

				size_t k = i * 8;

				glm::vec2 u0(uvbuffer[k    ], uvbuffer[k + 1]);
				glm::vec2 u1(uvbuffer[k + 2], uvbuffer[k + 3]);
				glm::vec2 u2(uvbuffer[k + 4], uvbuffer[k + 5]);
				glm::vec2 u3(uvbuffer[k + 6], uvbuffer[k + 7]);

				glm::vec3 c0(colbuffer[j     ], colbuffer[j +  1], colbuffer[j +  2]);
				glm::vec3 c1(colbuffer[j +  4], colbuffer[j +  5], colbuffer[j +  6]);
				glm::vec3 c2(colbuffer[j +  8], colbuffer[j +  9], colbuffer[j + 10]);
				glm::vec3 c3(colbuffer[j + 12], colbuffer[j + 13], colbuffer[j + 14]);

				vml::DebugRender->DrawAlphaColorTexturedQuad(view, p0, p1, p2, p3, c0,c1,c2,c3,texid, 1.0f, blendmode);

			}

		}

		// ---------------------------------------------------------------
		// 

		void DrawMesh(vml::views::View* view, size_t emitters, const glm::vec3* posbuffer, size_t emitterindicescount, const unsigned int* emitterindices) const
		{
			for (size_t i = 0; i < emitters; ++i)
				vml::DebugRender->DrawPoint(view, posbuffer[i], vml::colors::Yellow);

			for (size_t i = 0; i < emitterindicescount / 3; ++i)
			{
				size_t j = i * 3;

				vml::DebugRender->DrawTriangle(view,
												posbuffer[emitterindices[j    ]],
												posbuffer[emitterindices[j + 1]],
												posbuffer[emitterindices[j + 2]], vml::colors::White, true);
			}
		}

		// ---------------------------------------------------------------
		// 

		void DrawPoints(vml::views::View* view, ParticleNodeBase* node)
		{
			for (size_t i = 0; i < node->GetParticlesCount(); ++i)
			{
				Particle* p = node->GetParticleAt(i);

				vml::DebugRender->DrawPoint(view, p->GetR0(), vml::colors::Red);
				vml::DebugRender->DrawPoint(view, p->GetR1(), vml::colors::Green);
				vml::DebugRender->DrawPoint(view, p->GetR2(), vml::colors::Blue);
				vml::DebugRender->DrawPoint(view, p->GetR3(), vml::colors::Yellow);
			}
		}

		// ---------------------------------------------------------------
		// accepted blendmodes are :
		//  vml::debugrendering::AlphaTexturedQuad::ADDICTIVE_BLENDING_NO_ALPHA;
		//	vml::debugrendering::AlphaTexturedQuad::ADDICTIVE_BLENDING;
		//	vml::debugrendering::AlphaTexturedQuad::TRANSPARENCY;

		void DrawDebugPoolBuffer(vml::views::View* view, ParticleNodeBase* node,int blendmode) const
		{

			if (vml::bits32::Get(node->GetDrawMode(), Particle::WIRE_BILLBOARDS) ||
				vml::bits32::Get(node->GetDrawMode(), Particle::WIRE_QUADS))
			{
				DrawWireBuffer(view, node);
				return;
			}

			if (vml::bits32::Get(node->GetDrawMode(), Particle::SOLID_BILLBOARDS) ||
				vml::bits32::Get(node->GetDrawMode(), Particle::SOLID_QUADS))
			{
				DrawSolidBuffer(view, node);
				return;
			}

			if (vml::bits32::Get(node->GetDrawMode(), Particle::TEXTURED_BILLBOARDS) ||
				vml::bits32::Get(node->GetDrawMode(), Particle::TEXTURED_QUADS))
			{
				DrawTexturedBuffer(view, node);
				return;
			}

			if (vml::bits32::Get(node->GetDrawMode(), Particle::TEXTURED_ALPHA_BILLBOARDS) ||
				vml::bits32::Get(node->GetDrawMode(), Particle::TEXTURED_ALPHA_QUADS))
			{
				DrawTexturedAlphaBuffer(view, node,blendmode);
				return;
			}

			if (vml::bits32::Get(node->GetDrawMode(), Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS) ||
				vml::bits32::Get(node->GetDrawMode(), Particle::TEXTURED_ALPHA_COLORED_QUADS))
			{
				DrawTexturedAlphaColorBuffer(view, node,blendmode);
				return;
			}

		}

		// ---------------------------------------------------------------
		// ctor / dtor

		ParticleRenderer()
		{}

		~ParticleRenderer()
		{}

};
