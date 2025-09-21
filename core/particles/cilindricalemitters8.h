#pragma once

class CilindricalEmitters8 : public ParticleNodeBase
{
	private:

		inline constexpr static unsigned int Indices[] = {
															 0,  8,  1,
															 1,  8,  9,
															 1,  9,  2,
															 2,  9, 10,
															 2, 10,  3,
															 3, 10, 11,
															 3, 11,  4,
															 4, 11, 12,
															 4, 12,  5,
															 5, 12, 13,
															 5, 13,  6,
															 6, 13, 14,
															 6, 14,  7,
															 7, 14, 15,
															15,  0,  7,
															 8,  0, 15

		};

		glm::vec3 StartingEmitters[16];
		glm::vec3 TransformedStartingEmitters[16];
		glm::vec3 Directions[8];
		float	  Height;
		float	  InitialStartingRadius;
		float	  InitialFinalRadius;
		float	  StartingRadius;
		float	  FinalRadius;

	public:

		// ---------------------------------------------------------------
		// setters

		void SetStartingRadius(float val)
		{
			StartingRadius = val;
		}

		void SetFinalRadius(float val)
		{
			FinalRadius = val;
		}

		void ResetStartingRadius()
		{
			StartingRadius = InitialStartingRadius;
		}

		void ResetFinalgRadius()
		{
			StartingRadius = InitialStartingRadius;
		}

		void Reset()
		{
			StartingRadius = InitialStartingRadius;
			ParticleNodeBase::Reset();
		}

		// ---------------------------------------------------------------
		// getters

		float GetStartingRadius() const
		{
			return StartingRadius;
		}

		float GetFinalRadius() const
		{
			return FinalRadius;
		}

		int GetEmittersCount() const
		{
			return 8;
		}

		const glm::vec3* GetTransformedStartingEmitters()
		{
			return TransformedStartingEmitters;
		}

		const unsigned int* GetIndices()
		{
			return Indices;
		}

		// ---------------------------------------------------------------

		void ComputeRadii()
		{
			float dr;
			float rad;
			float deg;

			// compute angle increment

			dr = 360.0f / (float)8;

			// first ring

			deg = 0;

			rad = deg * vml::math::DEGTORAD; StartingEmitters[0] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[1] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[2] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[3] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[4] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[5] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[6] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[7] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;

			// second ring

			deg = 0;

			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 8] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 9] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[10] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[11] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[12] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[13] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[14] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[15] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;

		}

		// ---------------------------------------------------------------
		// add new particle to the pool

		void Add()
		{
			AddParticleToPool(TransformedStartingEmitters[0], Directions[0]);
			AddParticleToPool(TransformedStartingEmitters[1], Directions[1]);
			AddParticleToPool(TransformedStartingEmitters[2], Directions[2]);
			AddParticleToPool(TransformedStartingEmitters[3], Directions[3]);
			AddParticleToPool(TransformedStartingEmitters[4], Directions[4]);
			AddParticleToPool(TransformedStartingEmitters[5], Directions[5]);
			AddParticleToPool(TransformedStartingEmitters[6], Directions[6]);
			AddParticleToPool(TransformedStartingEmitters[7], Directions[7]);
		}

		// ---------------------------------------------------------------

		void Update(vml::views::View* view, float dt)
		{
			// compute spherical coordiantes matrix

			ComputeMatrix();

			// get matrix pointer

			float* m = glm::value_ptr(M);

			// transform first ring points

			TransformedStartingEmitters[0].x = m[0] * StartingEmitters[0].x + m[4] * StartingEmitters[0].y + m[ 8] * StartingEmitters[0].z + m[12];
			TransformedStartingEmitters[0].y = m[1] * StartingEmitters[0].x + m[5] * StartingEmitters[0].y + m[ 9] * StartingEmitters[0].z + m[13];
			TransformedStartingEmitters[0].z = m[2] * StartingEmitters[0].x + m[6] * StartingEmitters[0].y + m[10] * StartingEmitters[0].z + m[14];
			TransformedStartingEmitters[1].x = m[0] * StartingEmitters[1].x + m[4] * StartingEmitters[1].y + m[ 8] * StartingEmitters[1].z + m[12];
			TransformedStartingEmitters[1].y = m[1] * StartingEmitters[1].x + m[5] * StartingEmitters[1].y + m[ 9] * StartingEmitters[1].z + m[13];
			TransformedStartingEmitters[1].z = m[2] * StartingEmitters[1].x + m[6] * StartingEmitters[1].y + m[10] * StartingEmitters[1].z + m[14];
			TransformedStartingEmitters[2].x = m[0] * StartingEmitters[2].x + m[4] * StartingEmitters[2].y + m[ 8] * StartingEmitters[2].z + m[12];
			TransformedStartingEmitters[2].y = m[1] * StartingEmitters[2].x + m[5] * StartingEmitters[2].y + m[ 9] * StartingEmitters[2].z + m[13];
			TransformedStartingEmitters[2].z = m[2] * StartingEmitters[2].x + m[6] * StartingEmitters[2].y + m[10] * StartingEmitters[2].z + m[14];
			TransformedStartingEmitters[3].x = m[0] * StartingEmitters[3].x + m[4] * StartingEmitters[3].y + m[ 8] * StartingEmitters[3].z + m[12];
			TransformedStartingEmitters[3].y = m[1] * StartingEmitters[3].x + m[5] * StartingEmitters[3].y + m[ 9] * StartingEmitters[3].z + m[13];
			TransformedStartingEmitters[3].z = m[2] * StartingEmitters[3].x + m[6] * StartingEmitters[3].y + m[10] * StartingEmitters[3].z + m[14];
			TransformedStartingEmitters[4].x = m[0] * StartingEmitters[4].x + m[4] * StartingEmitters[4].y + m[ 8] * StartingEmitters[4].z + m[12];
			TransformedStartingEmitters[4].y = m[1] * StartingEmitters[4].x + m[5] * StartingEmitters[4].y + m[ 9] * StartingEmitters[4].z + m[13];
			TransformedStartingEmitters[4].z = m[2] * StartingEmitters[4].x + m[6] * StartingEmitters[4].y + m[10] * StartingEmitters[4].z + m[14];
			TransformedStartingEmitters[5].x = m[0] * StartingEmitters[5].x + m[4] * StartingEmitters[5].y + m[ 8] * StartingEmitters[5].z + m[12];
			TransformedStartingEmitters[5].y = m[1] * StartingEmitters[5].x + m[5] * StartingEmitters[5].y + m[ 9] * StartingEmitters[5].z + m[13];
			TransformedStartingEmitters[5].z = m[2] * StartingEmitters[5].x + m[6] * StartingEmitters[5].y + m[10] * StartingEmitters[5].z + m[14];
			TransformedStartingEmitters[6].x = m[0] * StartingEmitters[6].x + m[4] * StartingEmitters[6].y + m[ 8] * StartingEmitters[6].z + m[12];
			TransformedStartingEmitters[6].y = m[1] * StartingEmitters[6].x + m[5] * StartingEmitters[6].y + m[ 9] * StartingEmitters[6].z + m[13];
			TransformedStartingEmitters[6].z = m[2] * StartingEmitters[6].x + m[6] * StartingEmitters[6].y + m[10] * StartingEmitters[6].z + m[14];
			TransformedStartingEmitters[7].x = m[0] * StartingEmitters[7].x + m[4] * StartingEmitters[7].y + m[ 8] * StartingEmitters[7].z + m[12];
			TransformedStartingEmitters[7].y = m[1] * StartingEmitters[7].x + m[5] * StartingEmitters[7].y + m[ 9] * StartingEmitters[7].z + m[13];
			TransformedStartingEmitters[7].z = m[2] * StartingEmitters[7].x + m[6] * StartingEmitters[7].y + m[10] * StartingEmitters[7].z + m[14];

			// transform second ring points

			TransformedStartingEmitters[ 8].x = m[0] * StartingEmitters[ 8].x + m[4] * StartingEmitters[ 8].y + m[ 8] * StartingEmitters[ 8].z + m[12];
			TransformedStartingEmitters[ 8].y = m[1] * StartingEmitters[ 8].x + m[5] * StartingEmitters[ 8].y + m[ 9] * StartingEmitters[ 8].z + m[13];
			TransformedStartingEmitters[ 8].z = m[2] * StartingEmitters[ 8].x + m[6] * StartingEmitters[ 8].y + m[10] * StartingEmitters[ 8].z + m[14];
			TransformedStartingEmitters[ 9].x = m[0] * StartingEmitters[ 9].x + m[4] * StartingEmitters[ 9].y + m[ 8] * StartingEmitters[ 9].z + m[12];
			TransformedStartingEmitters[ 9].y = m[1] * StartingEmitters[ 9].x + m[5] * StartingEmitters[ 9].y + m[ 9] * StartingEmitters[ 9].z + m[13];
			TransformedStartingEmitters[ 9].z = m[2] * StartingEmitters[ 9].x + m[6] * StartingEmitters[ 9].y + m[10] * StartingEmitters[ 9].z + m[14];
			TransformedStartingEmitters[10].x = m[0] * StartingEmitters[10].x + m[4] * StartingEmitters[10].y + m[ 8] * StartingEmitters[10].z + m[12];
			TransformedStartingEmitters[10].y = m[1] * StartingEmitters[10].x + m[5] * StartingEmitters[10].y + m[ 9] * StartingEmitters[10].z + m[13];
			TransformedStartingEmitters[10].z = m[2] * StartingEmitters[10].x + m[6] * StartingEmitters[10].y + m[10] * StartingEmitters[10].z + m[14];
			TransformedStartingEmitters[11].x = m[0] * StartingEmitters[11].x + m[4] * StartingEmitters[11].y + m[ 8] * StartingEmitters[11].z + m[12];
			TransformedStartingEmitters[11].y = m[1] * StartingEmitters[11].x + m[5] * StartingEmitters[11].y + m[ 9] * StartingEmitters[11].z + m[13];
			TransformedStartingEmitters[11].z = m[2] * StartingEmitters[11].x + m[6] * StartingEmitters[11].y + m[10] * StartingEmitters[11].z + m[14];
			TransformedStartingEmitters[12].x = m[0] * StartingEmitters[12].x + m[4] * StartingEmitters[12].y + m[ 8] * StartingEmitters[12].z + m[12];
			TransformedStartingEmitters[12].y = m[1] * StartingEmitters[12].x + m[5] * StartingEmitters[12].y + m[ 9] * StartingEmitters[12].z + m[13];
			TransformedStartingEmitters[12].z = m[2] * StartingEmitters[12].x + m[6] * StartingEmitters[12].y + m[10] * StartingEmitters[12].z + m[14];
			TransformedStartingEmitters[13].x = m[0] * StartingEmitters[13].x + m[4] * StartingEmitters[13].y + m[ 8] * StartingEmitters[13].z + m[12];
			TransformedStartingEmitters[13].y = m[1] * StartingEmitters[13].x + m[5] * StartingEmitters[13].y + m[ 9] * StartingEmitters[13].z + m[13];
			TransformedStartingEmitters[13].z = m[2] * StartingEmitters[13].x + m[6] * StartingEmitters[13].y + m[10] * StartingEmitters[13].z + m[14];
			TransformedStartingEmitters[14].x = m[0] * StartingEmitters[14].x + m[4] * StartingEmitters[14].y + m[ 8] * StartingEmitters[14].z + m[12];
			TransformedStartingEmitters[14].y = m[1] * StartingEmitters[14].x + m[5] * StartingEmitters[14].y + m[ 9] * StartingEmitters[14].z + m[13];
			TransformedStartingEmitters[14].z = m[2] * StartingEmitters[14].x + m[6] * StartingEmitters[14].y + m[10] * StartingEmitters[14].z + m[14];
			TransformedStartingEmitters[15].x = m[0] * StartingEmitters[15].x + m[4] * StartingEmitters[15].y + m[ 8] * StartingEmitters[15].z + m[12];
			TransformedStartingEmitters[15].y = m[1] * StartingEmitters[15].x + m[5] * StartingEmitters[15].y + m[ 9] * StartingEmitters[15].z + m[13];
			TransformedStartingEmitters[15].z = m[2] * StartingEmitters[15].x + m[6] * StartingEmitters[15].y + m[10] * StartingEmitters[15].z + m[14];

			// compèute direction vectors

			Directions[0] = glm::normalize(TransformedStartingEmitters[8] - TransformedStartingEmitters[0]);
			Directions[1] = glm::normalize(TransformedStartingEmitters[9] - TransformedStartingEmitters[1]);
			Directions[2] = glm::normalize(TransformedStartingEmitters[10] - TransformedStartingEmitters[2]);
			Directions[3] = glm::normalize(TransformedStartingEmitters[11] - TransformedStartingEmitters[3]);
			Directions[4] = glm::normalize(TransformedStartingEmitters[12] - TransformedStartingEmitters[4]);
			Directions[5] = glm::normalize(TransformedStartingEmitters[13] - TransformedStartingEmitters[5]);
			Directions[6] = glm::normalize(TransformedStartingEmitters[14] - TransformedStartingEmitters[6]);
			Directions[7] = glm::normalize(TransformedStartingEmitters[15] - TransformedStartingEmitters[7]);

			// update particles

			ParticleNodeBase::Update(view, dt);
		}

		// ---------------------------------------------------------------

		void Draw(vml::views::View* view)
		{
			if (!IsVisible())
				return;

			// draw emitters
			
			for (size_t i = 0; i < 16; ++i)
				vml::DebugRender->DrawPoint(view, TransformedStartingEmitters[i], vml::colors::Yellow);

			for (size_t i = 0; i < 48 / 3; ++i)
			{
				size_t j = i * 3;

				vml::DebugRender->DrawTriangle( view,
												TransformedStartingEmitters[Indices[j    ]],
												TransformedStartingEmitters[Indices[j + 1]],
												TransformedStartingEmitters[Indices[j + 2]], vml::colors::White, true);
			}

		}

		// ---------------------------------------------------------------
		// ctor / dtor

		CilindricalEmitters8(const std::string& screenname,
							 const float startingradius,
							 const float endingradius,
							 const float height,
							 const glm::vec3& position,
							 const glm::vec3& positiondeviation,
							 const float velocity,
							 const glm::vec3& velocitydeviation,
							 const glm::vec3& accelleration,
							 const glm::vec3& accellerationdeviation,
							 const float angle,
							 const float angledeviation,
							 const float angularvelocity,
							 const float angularvelocitydeviation,
							 const float angularaccelleration,
							 const float angularaccellerationdeviation,
							 const glm::vec2& billboardinitialparticlescale,
							 const glm::vec2& billboardfinalscale,
							 const glm::vec2& quadinitialparticlescale,
							 const glm::vec2& quadfinalscale,
							 const float spawningdelay,
							 const float initialdelay,
							 const float lifespan,
							 const int maxparticlescount,
							 const int maxparticlespoolsize,
							 const uint32 flags,
							 const uint32 drawmode,
							 const uint32 rotationmode,
							 const glm::vec4& startcolor,
							 const glm::vec4& endcolor) : ParticleNodeBase( screenname,
																			"Cilindrical ( 8 points ) Emitter",
																			position,
																			positiondeviation,
																			velocity,
																			glm::vec3(0, 1, 0),
																			velocitydeviation,
																			accelleration,
																			accellerationdeviation,
																			angle, angledeviation,
																			angularvelocity,
																			angularvelocitydeviation,
																			angularaccelleration,
																			angularaccellerationdeviation,
																			billboardinitialparticlescale,
																			billboardfinalscale,
																			quadinitialparticlescale,
																			quadfinalscale,
																			spawningdelay,
																			initialdelay,
																			lifespan,
																			maxparticlescount,
																			maxparticlespoolsize,
																			flags,
																			drawmode,
																			rotationmode,
																			startcolor,
																			endcolor)
		{
			Height                = height;
			StartingRadius        = startingradius;
			FinalRadius           = endingradius;
			InitialStartingRadius = startingradius;
			InitialFinalRadius    = endingradius;

			// initialize arrays

			memset(&TransformedStartingEmitters[0], 0, 16 * sizeof(glm::vec3));
			memset(&Directions[0], 0, 8 * sizeof(glm::vec3));

			// compute radii

			ComputeRadii();
		}

		~CilindricalEmitters8()
		{
		}

};

