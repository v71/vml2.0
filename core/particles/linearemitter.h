#pragma once


class LinearParticleNode : public ParticleNodeBase
{
	private:

		// ---------------------------------------------------------------
		// add new particle

		void Add()
		{
			AddParticleToPool(Position, Forward);
		}

	public:

		// ---------------------------------------------------------------
		// update particles

		void Update(vml::views::View* view, float dt)
		{
			ComputeMatrix();
			ParticleNodeBase::Update(view, dt);
		}

		// ---------------------------------------------------------------
		// getters

		int GetEmittersCount() const
		{
			return 1;
		}

		// ---------------------------------------------------------------
		// reset particle positions

		void Reset(const bool fullreset=false)
		{
			ParticleNodeBase::Reset(fullreset);
		}

		// ---------------------------------------------------------------
		// debug drawing

		void Draw(vml::views::View* view)
		{
			if (!IsVisible())
				return;

			vml::DebugRender->DrawArrow(view, Position, Position + Forward * 15.0f, vml::colors::Red, 1, 2);

		}

		// ---------------------------------------------------------------
		// ctor / dtor

		LinearParticleNode(const std::string& screenname,
						   const glm::vec3& position, 
						   const glm::vec3& positiondeviation,
						   const float meanvelocity,
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
						   const glm::vec2& quadparticlescale,
						   const glm::vec2& quadfinalscale,
						   const float spawningdelay,
						   const float initialdelay,
						   const float lifespan,
						   const int maxparticlescount,
						   const int maxparticlespoolsize,
						   const uint32 flags,
						   const uint32 drawmode,
						   const uint32 rotationmode,
						   const glm::vec4 &startcolor,
						   const glm::vec4 &endcolor) : ParticleNodeBase( screenname,
																		  "Linear Emitter",
																		  position, 
																		  positiondeviation,
																		  meanvelocity,
																		  glm::vec3(0, 1, 0), 
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
																		  quadparticlescale,
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
																		  endcolor )
		{
		}

		~LinearParticleNode()
		{
		}

};
