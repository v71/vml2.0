#pragma once


class SphericalParticleNodeLv1 : public ParticleNodeBase
{
	private:

		// mesh data

		inline constexpr static glm::vec3 Emitters[] = {
															glm::vec3( 0.000000 , -1.000000 ,  0.000000),
															glm::vec3( 0.723607 , -0.447220 ,  0.525725),
															glm::vec3(-0.276388 , -0.447220 ,  0.850649),
															glm::vec3(-0.894426 , -0.447216 ,  0.000000),
															glm::vec3(-0.276388 , -0.447220 , -0.850649),
															glm::vec3( 0.723607 , -0.447220 , -0.525725),
															glm::vec3( 0.276388 ,  0.447220 ,  0.850649),
															glm::vec3(-0.723607 ,  0.447220 ,  0.525725),
															glm::vec3(-0.723607 ,  0.447220 , -0.525725),
															glm::vec3( 0.276388 ,  0.447220 , -0.850649),
															glm::vec3( 0.894426 ,  0.447216 ,  0.000000),
															glm::vec3( 0.000000 ,  1.000000 ,  0.000000),
															glm::vec3(-0.162456 , -0.850654 ,  0.499995),
															glm::vec3( 0.425323 , -0.850654 ,  0.309011),
															glm::vec3( 0.262869 , -0.525738 ,  0.809012),
															glm::vec3( 0.850648 , -0.525736 ,  0.000000),
															glm::vec3( 0.425323 , -0.850654 , -0.309011),
															glm::vec3(-0.525730 , -0.850652 ,  0.000000),
															glm::vec3(-0.688189 , -0.525736 ,  0.499997),
															glm::vec3(-0.162456 , -0.850654 , -0.499995),
															glm::vec3(-0.688189 , -0.525736 , -0.499997),
															glm::vec3( 0.262869 , -0.525738 , -0.809012),
															glm::vec3( 0.951058 ,  0.000000 ,  0.309013),
															glm::vec3( 0.951058 ,  0.000000 , -0.309013),
															glm::vec3( 0.000000 ,  0.000000 ,  1.000000),
															glm::vec3( 0.587786 ,  0.000000 ,  0.809017),
															glm::vec3(-0.951058 ,  0.000000 ,  0.309013),
															glm::vec3(-0.587786 ,  0.000000 ,  0.809017),
															glm::vec3(-0.587786 ,  0.000000 , -0.809017),
															glm::vec3(-0.951058 ,  0.000000 , -0.309013),
															glm::vec3( 0.587786 ,  0.000000 , -0.809017),
															glm::vec3( 0.000000 ,  0.000000 , -1.000000),
															glm::vec3( 0.688189 ,  0.525736 ,  0.499997),
															glm::vec3(-0.262869 ,  0.525738 ,  0.809012),
															glm::vec3(-0.850648 ,  0.525736 ,  0.000000),
															glm::vec3(-0.262869 ,  0.525738 , -0.809012),
															glm::vec3( 0.688189 ,  0.525736 , -0.499997),
															glm::vec3( 0.162456 ,  0.850654 ,  0.499995),
															glm::vec3( 0.525730 ,  0.850652 ,  0.000000),
															glm::vec3(-0.425323 ,  0.850654 ,  0.309011),
															glm::vec3(-0.425323 ,  0.850654 , -0.309011),
															glm::vec3( 0.162456 ,  0.850654 , -0.499995)
		};

		inline constexpr static unsigned int Indices[] = {
															 0 , 13 , 12 ,
															 1 , 13 , 15 ,
															 0 , 12 , 17 ,
															 0 , 17 , 19 ,
															 0 , 19 , 16 ,
															 1 , 15 , 22 ,
															 2 , 14 , 24 ,
															 3 , 18 , 26 ,
															 4 , 20 , 28 ,
															 5 , 21 , 30 ,
															 1 , 22 , 25 ,
															 2 , 24 , 27 ,
															 3 , 26 , 29 ,
															 4 , 28 , 31 ,
															 5 , 30 , 23 ,
															 6 , 32 , 37 ,
															 7 , 33 , 39 ,
															 8 , 34 , 40 ,
															 9 , 35 , 41 ,
															10 , 36 , 38 ,
															38 , 41 , 11 ,
															38 , 36 , 41 ,
															36 ,  9 , 41 ,
															41 , 40 , 11 ,
															41 , 35 , 40 ,
															35 ,  8 , 40 ,
															40 , 39 , 11 ,
															40 , 34 , 39 ,
															34 ,  7 , 39 ,
															39 , 37 , 11 ,
															39 , 33 , 37 ,
															33 ,  6 , 37 ,
															37 , 38 , 11 ,
															37 , 32 , 38 ,
															32 , 10 , 38 ,
															23 , 36 , 10 ,
															23 , 30 , 36 ,
															30 ,  9 , 36 ,
															31 , 35 ,  9 ,
															31 , 28 , 35 ,
															28 ,  8 , 35 ,
															29 , 34 ,  8 ,
															29 , 26 , 34 ,
															26 ,  7 , 34 ,
															27 , 33 ,  7 ,
															27 , 24 , 33 ,
															24 ,  6 , 33 ,
															25 , 32 ,  6 ,
															25 , 22 , 32 ,
															22 , 10 , 32 ,
															30 , 31 ,  9 ,
															30 , 21 , 31 ,
															21 ,  4 , 31 ,
															28 , 29 ,  8 ,
															28 , 20 , 29 ,
															20 ,  3 , 29 ,
															26 , 27 ,  7 ,
															26 , 18 , 27 ,
															18 ,  2 , 27 ,
															24 , 25 ,  6 ,
															24 , 14 , 25 ,
															14 ,  1 , 25 ,
															22 , 23 , 10 ,
															22 , 15 , 23 ,
															15 ,  5 , 23 ,
															16 , 21 ,  5 ,
															16 , 19 , 21 ,
															19 ,  4 , 21 ,
															19 , 20 ,  4 ,
															19 , 17 , 20 ,
															17 ,  3 , 20 ,
															17 , 18 ,  3 ,
															17 , 12 , 18 ,
															12 ,  2 , 18 ,
															15 , 16 ,  5 ,
															15 , 13 , 16 ,
															13 ,  0 , 16 ,
															12 , 14 ,  2 ,
															12 , 13 , 14 ,
															13 ,  1 , 14
		};

		// private data

		glm::vec3  TransformedEmitters[42];
		glm::vec3  Directions[42];
		float	   InitialStartingRadius;
		float	   StartingRadius;

	public:

		// ---------------------------------------------------------------

		void Reset()
		{
			StartingRadius = InitialStartingRadius;
			ParticleNodeBase::Reset();
		}

		// ---------------------------------------------------------------
		// add new particle to the pool

		void Add()
		{
			for (size_t i = 0; i < 42; ++i)
				AddParticleToPool(TransformedEmitters[i], Directions[i]);
		}

		// ---------------------------------------------------------------

		void SetStartingRadius(float val)
		{
			StartingRadius = val;
		}

		int GetIcoLevel() const
		{
			return 1;
		}

		int GetEmittersCount() const
		{
			return 42;
		}

		int GetIndicesCount() const
		{
			return 240;
		}

		const glm::vec3* GetTransformedStartingEmitters()
		{
			return TransformedEmitters;
		}

		const unsigned int* GetIndices()
		{
			return Indices;
		}

		// ---------------------------------------------------------------

		float GetStartingRadius() const
		{
			return StartingRadius;
		}

		// ---------------------------------------------------------------

		void Update(vml::views::View* view, float dt)
		{
			// compute matrix 

			ComputeMatrix();

			// transform emitters

			const float* m = glm::value_ptr(M);

			// transform base emitters

			for (size_t i = 0; i < 42; ++i)
			{
				TransformedEmitters[i].x = m[ 0] * Emitters[i].x * StartingRadius +
										   m[ 4] * Emitters[i].y * StartingRadius +
										   m[ 8] * Emitters[i].z * StartingRadius + m[12];
				TransformedEmitters[i].y = m[ 1] * Emitters[i].x * StartingRadius +
										   m[ 5] * Emitters[i].y * StartingRadius +
										   m[ 9] * Emitters[i].z * StartingRadius + m[13];
				TransformedEmitters[i].z = m[ 2] * Emitters[i].x * StartingRadius +
										   m[ 6] * Emitters[i].y * StartingRadius +
										   m[10] * Emitters[i].z * StartingRadius + m[14];
			}

			// compute directions

			for (size_t i = 0; i < 42; ++i)
				Directions[i] = glm::normalize(TransformedEmitters[i] - Position);

			// update

			ParticleNodeBase::Update(view, dt);
		}

		// ---------------------------------------------------------------

		void Draw(vml::views::View* view)
		{
			if (!IsVisible())
				return;
			
			// draw emitters

			for (size_t i = 0; i < 42; ++i)
				vml::DebugRender->DrawPoint(view, TransformedEmitters[i], vml::colors::Yellow);

			for (size_t i = 0; i < 240 / 3; ++i)
			{
				size_t j = i * 3;

				vml::DebugRender->DrawTriangle( view,
												TransformedEmitters[Indices[j    ]],
												TransformedEmitters[Indices[j + 1]],
												TransformedEmitters[Indices[j + 2]], vml::colors::White, true );
			}
		

		}

		// ---------------------------------------------------------------
		// ctor / dtor

		SphericalParticleNodeLv1(const std::string& screenname,
								 const float startingradius,
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
								 const glm::vec2 &billboardinitialparticlescale,
								 const glm::vec2 &billboardfinalscale,
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
																				"IcoSphere ( ico1 ) emitter",
																				position,
																				positiondeviation,
																				velocity,
																				glm::vec3(0, 0, 0),
																				velocitydeviation,
																				accelleration,
																				accellerationdeviation,
																				angle,
																				angledeviation,
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
			// radii

			StartingRadius = startingradius;
			InitialStartingRadius = startingradius;

			// initialize arrays

			memset(&TransformedEmitters[0], 0, 42 * sizeof(glm::vec3));
			memset(&Directions[0], 0, 42 * sizeof(glm::vec3));
		}

		~SphericalParticleNodeLv1()
		{
		}

};
