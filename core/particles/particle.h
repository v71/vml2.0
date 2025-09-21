#pragma once

////////////////////////////////////////////////////////////////////////////////////////
// Partcile class

class Particle
{
	private:

		glm::vec3 Position;								// particle position
		glm::vec3 Forward;								// direction vector used when QUADS flag is enabled
		glm::vec3 Velocity;								// particle velocity
		glm::vec3 Accelleration;						// particle accelleration
		float	  Angle;								// angle
		float	  AngularVelocity;						// angular velocity
		float	  AngularAccelleration;					// angular accelleration

		glm::vec3 InitialPosition;						// initial position
		glm::vec3 InitialVelocity;						// initial velocity
		glm::vec3 InitialAccelleration;					// initial accelleration
		float	  InitialAngle;							// initial angle
		float	  InitialAngularVelocity;				// initial angular velocity
		float	  InitialAngularAccelleration;			// initial angular accelleration

		glm::vec3 PositionDeviation;					// position deviation		
		glm::vec3 VelocityDeviation;					// velocityt deviation
		glm::vec3 AccellerationDeviation;				// accelleration deviation
		float	  AngleDeviation;						// angle deviation
		float	  AngularVelocityDeviation;				// angular speed deviation
		float	  AngularAccellerationDeviation;		// angular accelleration deviation

		glm::vec4 Color;
		glm::vec4 InitialColor;
		glm::vec4 FinalColor;

		glm::vec2 BillBoardScale;
		glm::vec2 BillBoardInitialScale;
		glm::vec2 BillBoardFinalScale;

		glm::vec2 QuadScale;
		glm::vec2 QuadInitialScale;
		glm::vec2 QuadFinalScale;

		glm::vec3 AABBoxMax;
		glm::vec3 AABBoxMin;
		glm::vec3 HalfDiagonal;

		glm::vec3 R0, R1, R2, R3;
		glm::vec2 U0, U1, U2, U3;

		float     T;
		float	  LifeSpan;
		float	  InitialCurrentTime;

		int		  Id;
		uint32_t  Alive;
		uint32_t  DrawMode;
		uint32_t  RotationMode;

		// ---------------------------------------------------------------
		//

		float Randf(float dMin)
		{
			return dMin * (((float)rand() / (RAND_MAX)) * 2 - 1);
		}

		// ---------------------------------------------------------------
		//

		void ComputeParticleGeometry(vml::views::View* view)
		{
			glm::vec3 up    = view->GetUp();
			glm::vec3 right = view->GetRight();

			float cx = cos(Angle);
			float sx = sin(Angle);

			// view aligned rectangle

			// original code

			//	glm::vec3 a = Position - right * scalex + up * scaley;
			//	glm::vec3 b = Position + right * scalex + up * scaley;
			//	glm::vec3 c = Position + right * scalex - up * scaley;
			//	glm::vec3 d = Position - right * scalex - up * scaley;
			//	glm::vec3 o = (a + b + c + d) / 4.0f;

			//	glm::vec2 p0(-scalex,  scaley);
			//	glm::vec2 p1( scalex,  scaley);
			//	glm::vec2 p2( scalex, -scaley);
			//	glm::vec2 p3(-scalex, -scaley);

			//	glm::vec2 q0(p0.x * cx - p0.y * sx, p0.x * sx + p0.y * cx);
			//	glm::vec2 q1(p1.x * cx - p1.y * sx, p1.x * sx + p1.y * cx);
			//	glm::vec2 q2(p2.x * cx - p2.y * sx, p2.x * sx + p2.y * cx);
			//	glm::vec2 q3(p3.x * cx - p3.y * sx, p3.x * sx + p3.y * cx);

			//	glm::vec3 r0 = o + right * q0.x + up * q0.y;
			//	glm::vec3 r1 = o + right * q1.x + up * q1.y;
			//	glm::vec3 r2 = o + right * q2.x + up * q2.y;
			//	glm::vec3 r3 = o + right * q3.x + up * q3.y;

			// optimized version

			float ax = BillBoardScale.x * cx;
			float ay = BillBoardScale.x * sx;
			float bx = BillBoardScale.y * cx;
			float by = BillBoardScale.y * sx;

			glm::vec3 raxis0 = right * (ax + by);
			glm::vec3 raxis1 = right * (ax - by);
			glm::vec3 up0    = up    * (ay + bx);
			glm::vec3 up1    = up    * (ay - bx);

			R0 = Position - raxis0 - up1;
			R1 = Position + raxis1 + up0;
			R2 = Position + raxis0 + up1;
			R3 = Position - raxis1 - up0;

			// compute particle bounding box

			AABBoxMin.x = R0.x;
			AABBoxMin.y = R0.y;
			AABBoxMin.z = R0.z;

			if (R1.x < AABBoxMin.x)AABBoxMin.x = R1.x;
			if (R2.x < AABBoxMin.x)AABBoxMin.x = R2.x;
			if (R3.x < AABBoxMin.x)AABBoxMin.x = R3.x;
			if (R1.y < AABBoxMin.y)AABBoxMin.y = R1.y;
			if (R2.y < AABBoxMin.y)AABBoxMin.y = R2.y;
			if (R3.y < AABBoxMin.y)AABBoxMin.y = R3.y;
			if (R1.z < AABBoxMin.z)AABBoxMin.z = R1.z;
			if (R2.z < AABBoxMin.z)AABBoxMin.z = R2.z;
			if (R3.z < AABBoxMin.z)AABBoxMin.z = R3.z;

			AABBoxMax.x = R0.x;
			AABBoxMax.y = R0.y;
			AABBoxMax.z = R0.z;

			if (R1.x > AABBoxMax.x)AABBoxMax.x = R1.x;
			if (R2.x > AABBoxMax.x)AABBoxMax.x = R2.x;
			if (R3.x > AABBoxMax.x)AABBoxMax.x = R3.x;
			if (R1.y > AABBoxMax.y)AABBoxMax.y = R1.y;
			if (R2.y > AABBoxMax.y)AABBoxMax.y = R2.y;
			if (R3.y > AABBoxMax.y)AABBoxMax.y = R3.y;
			if (R1.z > AABBoxMax.z)AABBoxMax.z = R1.z;
			if (R2.z > AABBoxMax.z)AABBoxMax.z = R2.z;
			if (R3.z > AABBoxMax.z)AABBoxMax.z = R3.z;
			
			// conpute diagonal

			HalfDiagonal= raxis0 + up1;

		}

		// ---------------------------------------------------------------
		// compute geometry when the quads flag is active
		// computes reference system using the up vector 
		// crosse the forward vector

		void ComputeParticleGeometryForQuads(vml::views::View* view, const float dt)
		{
			glm::vec3 up    = view->GetUp();
			glm::vec3 right = view->GetRight();

			// compute direction vector

			Forward = glm::normalize(Velocity * dt);

			glm::vec3 u(0, 1, 0);
			glm::vec3 R = glm::cross(Forward, u);
			
			// compute quad dimensions

			float quadWidth  = QuadScale.x;
			float quadHeight = QuadScale.y;

			R0 = Position - quadWidth * R + quadHeight * Forward;
			R1 = Position + quadWidth * R + quadHeight * Forward;
			R2 = Position + quadWidth * R - quadHeight * Forward;
			R3 = Position - quadWidth * R - quadHeight * Forward;

			// compute particle bounding box

			AABBoxMin.x = R0.x;
			AABBoxMin.y = R0.y;
			AABBoxMin.z = R0.z;

			if (R1.x < AABBoxMin.x)AABBoxMin.x = R1.x;
			if (R2.x < AABBoxMin.x)AABBoxMin.x = R2.x;
			if (R3.x < AABBoxMin.x)AABBoxMin.x = R3.x;
			if (R1.y < AABBoxMin.y)AABBoxMin.y = R1.y;
			if (R2.y < AABBoxMin.y)AABBoxMin.y = R2.y;
			if (R3.y < AABBoxMin.y)AABBoxMin.y = R3.y;
			if (R1.z < AABBoxMin.z)AABBoxMin.z = R1.z;
			if (R2.z < AABBoxMin.z)AABBoxMin.z = R2.z;
			if (R3.z < AABBoxMin.z)AABBoxMin.z = R3.z;

			AABBoxMax.x = R0.x;
			AABBoxMax.y = R0.y;
			AABBoxMax.z = R0.z;

			if (R1.x > AABBoxMax.x)AABBoxMax.x = R1.x;
			if (R2.x > AABBoxMax.x)AABBoxMax.x = R2.x;
			if (R3.x > AABBoxMax.x)AABBoxMax.x = R3.x;
			if (R1.y > AABBoxMax.y)AABBoxMax.y = R1.y;
			if (R2.y > AABBoxMax.y)AABBoxMax.y = R2.y;
			if (R3.y > AABBoxMax.y)AABBoxMax.y = R3.y;
			if (R1.z > AABBoxMax.z)AABBoxMax.z = R1.z;
			if (R2.z > AABBoxMax.z)AABBoxMax.z = R2.z;
			if (R3.z > AABBoxMax.z)AABBoxMax.z = R3.z;
		}

	public:

		// ---------------------------------------------------------------
		// global flags

		static const uint32_t DEAD  = 0;
		static const uint32_t ALIVE = 1;

		// drawing mode flags

		static const uint32 WIRE_BILLBOARDS					  = vml::bits32::BIT0;
		static const uint32 SOLID_BILLBOARDS				  = vml::bits32::BIT1;
		static const uint32 TEXTURED_BILLBOARDS				  = vml::bits32::BIT2;
		static const uint32 TEXTURED_ALPHA_BILLBOARDS		  = vml::bits32::BIT3;
		static const uint32 TEXTURED_ALPHA_COLORED_BILLBOARDS = vml::bits32::BIT4;
			
		static const uint32 WIRE_QUADS						  = vml::bits32::BIT5;
		static const uint32 SOLID_QUADS						  = vml::bits32::BIT6;
		static const uint32 TEXTURED_QUADS					  = vml::bits32::BIT7;
		static const uint32 TEXTURED_ALPHA_QUADS			  = vml::bits32::BIT8;
		static const uint32 TEXTURED_ALPHA_COLORED_QUADS	  = vml::bits32::BIT9;

		static const uint32 EULER_MATRIX					  = vml::bits32::BIT16;
		static const uint32 QUATERNION_MATRIX				  = vml::bits32::BIT17;

		// ---------------------------------------------------------------
		//

		void Update(vml::views::View* view, float dt)
		{
			// if particle is type of wire, solid, billboards

			if (vml::bits32::Get(DrawMode , WIRE_BILLBOARDS)					||
				vml::bits32::Get(DrawMode , SOLID_BILLBOARDS)					||
				vml::bits32::Get(DrawMode , TEXTURED_BILLBOARDS)				||
				vml::bits32::Get(DrawMode , TEXTURED_ALPHA_BILLBOARDS)			||
				vml::bits32::Get(DrawMode , TEXTURED_ALPHA_COLORED_BILLBOARDS))
			{
				// if lifespan has expired, particle is consdidered dead

				T = (glfwGetTime() - InitialCurrentTime) / LifeSpan;

				if ( T > 1)
					Alive = DEAD;

				// integrate position

				Velocity += Accelleration * dt;
				Position += Velocity * dt;

				// integrate angle

				AngularVelocity += AngularAccelleration * dt;
				Angle += AngularVelocity * dt;

				// scaling

				BillBoardScale = BillBoardInitialScale + T * (BillBoardFinalScale - BillBoardInitialScale);
				
				// color

				Color = InitialColor + T * (FinalColor - InitialColor);

				// if scales go negative then the particle is compressed
				// along and axis and thus ios considered dead

				if (BillBoardScale.x < 0 || BillBoardScale.y < 0)
					Alive = DEAD;

				// compute bounding box

				ComputeParticleGeometry(view);
			}

			// if particle is type of quads

			if (vml::bits32::Get(DrawMode, WIRE_QUADS)						|| 
			    vml::bits32::Get(DrawMode, SOLID_QUADS)						|| 
				vml::bits32::Get(DrawMode, TEXTURED_QUADS)					||
				vml::bits32::Get(DrawMode, TEXTURED_ALPHA_QUADS)			||
				vml::bits32::Get(DrawMode, TEXTURED_ALPHA_COLORED_QUADS))
			{
				// if lifespan has expired, particle is consdidered dead

				T = (glfwGetTime() - InitialCurrentTime) / LifeSpan;

				if ( T > 1 )
					Alive = DEAD;

				// integrate position

				Velocity += Accelleration * dt;
				Position += Velocity * dt;

				// scaling
				
				QuadScale = QuadInitialScale + T * (QuadFinalScale - QuadInitialScale);

				// color

				Color = InitialColor + T * (FinalColor - InitialColor);

				// if scales go negative then the particle is compressed
				// along and axis and thus ios considered dead

				if (QuadScale.x < 0 || QuadScale.y < 0)
					Alive = DEAD;

				// compute geometry and particle bounding box

				ComputeParticleGeometryForQuads(view, dt);

			}

		}

		// ---------------------------------------------------------------
		//

		void Reset()
		{
			InitialCurrentTime   = glfwGetTime();
		
			// reset physics attributes

			Position			 = InitialPosition             + PositionDeviation;
			Velocity			 = InitialVelocity             + VelocityDeviation;
			Accelleration		 = InitialAccelleration        + AccellerationDeviation;
			Angle				 = InitialAngle                + AngleDeviation;
			AngularVelocity      = InitialAngularVelocity      + AngularVelocityDeviation;
			AngularAccelleration = InitialAngularAccelleration + AngularAccellerationDeviation;

			// convert ot radians

			AngularVelocity		 *= vml::math::DEGTORAD;
			AngularAccelleration *= vml::math::DEGTORAD;
			
			// reset geometry

			BillBoardScale	  = BillBoardInitialScale;

			// reset quad dimensions

			QuadScale = QuadInitialScale;

			// reset color

			Color	  = InitialColor;
			
			// reseet bounding box

			AABBoxMax = glm::vec3(0, 0, 0);
			AABBoxMin = glm::vec3(0, 0, 0);

			// reset business logic

			T		  = 0.0f;
			Alive	  = DEAD;
		}

		// ---------------------------------------------------------------
		//

		void SetParms(const int id,
					  const glm::vec3& initialposition, 
					  const glm::vec3& positiondeviation,
					  const glm::vec3& initialvelocity, 
					  const glm::vec3& velocitydeviation,
					  const glm::vec3& initialaccelleration, 
					  const glm::vec3& accellerationdeviation,
					  const float initialangle, 
					  const float angledeviation,
					  const float initialangularvelocity, 
					  const float angularvelocitydeviation,
					  const float initialangularaccelleration, 
					  const float angularaccellerationdeviation,
					  const glm::vec2& billboardinitialscale, 
					  const glm::vec2& billboardfinalscale,
					  const glm::vec2& quadinitialscale,
					  const glm::vec2& quadfinalscale,
					  const float lifespan,
					  const uint32_t alive,
					  const uint32 drawmode,
					  const uint32 rotationmode,
					  const glm::vec4 &initialcolor,
					  const glm::vec4 &finalcolor)
		{
			// original parameters

			InitialPosition              = initialposition;
			InitialVelocity              = initialvelocity;
			InitialAccelleration         = initialaccelleration;
			InitialAngle                 = initialangle;
			InitialAngularVelocity       = initialangularvelocity;
			InitialAngularAccelleration  = initialangularaccelleration;

			// linear parameters deviation

			PositionDeviation	   = glm::vec3(Randf(positiondeviation.x), Randf(positiondeviation.y), Randf(positiondeviation.z));
			VelocityDeviation	   = glm::vec3(Randf(velocitydeviation.x), Randf(velocitydeviation.y), Randf(velocitydeviation.z));
			AccellerationDeviation = glm::vec3(Randf(accellerationdeviation.x), Randf(accellerationdeviation.y), Randf(accellerationdeviation.z));

			// angluar parameter deviation

			AngleDeviation                = Randf(angledeviation);
			AngularVelocityDeviation      = Randf(angularvelocitydeviation);
			AngularAccellerationDeviation = Randf(angularaccellerationdeviation);

			// initial values

			Position	         = InitialPosition		       + PositionDeviation;
			Velocity	         = InitialVelocity		       + VelocityDeviation;
			Accelleration        = InitialAccelleration        + AccellerationDeviation;
			Angle		         = InitialAngle		           + AngleDeviation;
			AngularVelocity      = InitialAngularVelocity      + AngularVelocityDeviation;
			AngularAccelleration = InitialAngularAccelleration + AngularAccellerationDeviation;

			AngularVelocity		 *= vml::math::DEGTORAD;
			AngularAccelleration *= vml::math::DEGTORAD;

			// geometric parameters

			BillBoardScale        = billboardinitialscale;
			BillBoardInitialScale = billboardinitialscale;
			BillBoardFinalScale   = billboardfinalscale;

			// quad dimensions

			QuadScale			  = quadinitialscale;
			QuadInitialScale	  = quadinitialscale;
			QuadFinalScale		  = quadfinalscale;

			// bounding box

			AABBoxMax = glm::vec3(0, 0, 0);
			AABBoxMin = glm::vec3(0, 0, 0);
			
			// diagonal 

			HalfDiagonal = glm::vec3(0, 0, 0);

			// particle transformed positions

			R0 = glm::vec3(0, 0, 0);
			R1 = glm::vec3(0, 0, 0);
			R2 = glm::vec3(0, 0, 0);
			R3 = glm::vec3(0, 0, 0);

			// particle uv coordinates

			U0 = glm::vec2(0, 0);
			U1 = glm::vec2(1, 0);
			U2 = glm::vec2(1, 1);
			U3 = glm::vec2(0, 1);

			// intgegration parameters

			T                  = 0.0f;
			LifeSpan           = lifespan;
			InitialCurrentTime = glfwGetTime();

			// business parameters
			
			Alive		 = alive;
			Id			 = id;
			DrawMode	 = drawmode;
			RotationMode = QUATERNION_MATRIX;

			// color

			Color		 = initialcolor;
			InitialColor = initialcolor;
			FinalColor   = finalcolor;

		}

		// ---------------------------------------------------------------
		// getters

		const glm::vec3& GetPosition()			   const { return Position; }
		const glm::vec3& GetLinearVelocity()	   const { return Velocity; }
		const glm::vec3& GetLinearAccelleration()  const { return Accelleration; }
		const glm::vec2& GetBillBoardScale()	   const { return BillBoardScale; }
		const glm::vec4& GetColor()			       const { return Color; }
		uint32_t		 GetDrawMode()		       const { return DrawMode; }
		float			 GetAngle()				   const { return Angle; }
		float			 GetAngularVelocity()      const { return AngularVelocity; }
		float			 GetAngularAccelleration() const { return AngularAccelleration; }
		float			 GetQuadWidth()			   const { return QuadScale.x; }
		float			 GetQuadHeight()		   const { return QuadScale.y; }

		float			 GetT()						const { return T; }

		bool			 IsAlive()					const { return Alive == ALIVE; }
		bool			 IsDead()					const { return Alive == DEAD; }

		const glm::vec3& GetAABBoxMin()				const { return AABBoxMin; }
		const glm::vec3& GetAABBoxMax()				const { return AABBoxMax; }
		const glm::vec3& GetHalfDiagonal()		    const { return HalfDiagonal; }

		const glm::vec3& GetR0()					const { return R0; }
		const glm::vec3& GetR1()					const { return R1; }
		const glm::vec3& GetR2()					const { return R2; }
		const glm::vec3& GetR3()					const { return R3; }

		const glm::vec2& GetU0()					const { return U0; }
		const glm::vec2& GetU1()					const { return U1; }
		const glm::vec2& GetU2()					const { return U2; }
		const glm::vec2& GetU3()					const { return U3; }

		// ---------------------------------------------------------------
		// setters

		void ChangeLinearAccelleration(const glm::vec3& da) { Accelleration += da; }
		void ChangeLinearVelocity(const glm::vec3 &dv) { Velocity += dv; }
		void ChangeLinearPosition(const glm::vec3& dp) { Position += dp; }
		void SetLinearPosition(const glm::vec3& p) { Position = p; }
		void SetLinearVelocity(const glm::vec3& v) { Velocity = v; }
		void SetLinearAccelleration(const glm::vec3& a) { Accelleration = a; }
		void ChangeAngularAccelleration(const float da) { AngularAccelleration = da; }
		void ChangeAngularVelocity(const float dw) { AngularVelocity = dw; }
		void ChangeAngle(const float ds) { Angle += ds; }
		void SetAngularAccelleration(const float a) { AngularAccelleration = a; }
		void SetAngularVelocity(const float w) { AngularVelocity = w; }
		void SetAngle(const float s) { Angle = s; }
		
		// ---------------------------------------------------------------
		// ctor / dtor

		Particle()
		{
			SetParms( -1,
					  glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
					  glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
					  glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
					  0, 0,
					  0, 0,
					  0, 0,
					  glm::vec2(0, 0), glm::vec2(0, 0),
					  glm::vec2(0, 0), glm::vec2(0, 0),
					  0,
					  DEAD,
					  WIRE_BILLBOARDS,
				 	  QUATERNION_MATRIX,
					  glm::vec4(1, 1, 1, 1),
					  glm::vec4(1, 1, 1, 1) );
			}
		
		~Particle()
		{}

};

////////////////////////////////////////////////////////////////////////////////////////
// Base class for particle node , each effect is composed by nodes

class ParticleNodeBase
{
	protected:

		std::string				ScreenName;							// name of this node

		int						Size;								// number of particles
		int						MaxSize;							// max number of particels allowed
		int						MaxPoolSize;						// max size of particle buffer 

		uint32					PreferencesFlags;					// flags
		bool					Done;								// is the animation ended  *only active when loop is off* 
		bool					FirstEmission;						// first emission of particles

		Particle*				Pool;								// particle buffer 
		float*					ParticlePositionBuffer;				// vao positon buffer
		float*					ParticleUVBuffer;					// vao uv buffer
		float*					ParticleColorBuffer;				// vao positon buffer
		int						ParticleBufferSize;					// size of vao buffer

		glm::mat4				M;									// model matrix

		glm::vec3				Position;							// node position
		glm::vec3				InitialPosition;					// node initial position , used to reset position
		glm::vec3				PositionDeviation;					// position deviation from original node position

		float					MeanVelocity;
		glm::vec3				Velocity;							// velocity applied to all particles
		glm::vec3				InitialVelocity;					// node vleocity , used to reset velocity
		glm::vec3				VelocityDeviation;					// velocity deviation from original node velocity

		glm::vec3				Accelleration;						// accelleration
		glm::vec3				InitialAccelleration;				// init accelleration
		glm::vec3				AccellerationDeviation;				// acelleration deviation

		float					Angle;
		float					InitialAngle;
		float					AngleDeviation;

		float					AngularVelocity;
		float					InitialAngularVelocity;
		float					AngularVelocityDeviation;

		float					AngularAccelleration;
		float					InitialAngularAccelleration;
		float					AngularAccellerationDeviation;

		glm::vec2				BillBoardScale;
		glm::vec2				BillBoardFinalScale;
		glm::vec2				BillBoardInitialScale;

		glm::vec2				QuadScale;
		glm::vec2				QuadFinalScale;
		glm::vec2				QuadInitialScale;

		glm::vec4				Color;
		glm::vec4				FinalColor;
		glm::vec4				InitialColor;

		bool					RestartTimer;
		bool					InitialRestartTimer;
		float					CurrentTime;
		float					InitialCurrentTime;
		float					SpawningDelay;
		float				 	InitialDelay;
		float					LifeSpan;
		float					Ticks;

		glm::vec3				AABBoxMax;
		glm::vec3				AABBoxMin;
		glm::vec3				AABBoxCenter;
		float					Radius;

		glm::vec3				Up;
		glm::vec3				Right;
		glm::vec3				Forward;

		float					Theta;
		float					Phi;

		float					Yaw;
		float					Pitch;
		float					Roll;

		uint32					DrawMode;
		uint32					RotationMode;
		std::string				TypeString;

		int						BlendMode;

		vml::textures::Texture* Texture;

		// ---------------------------------------------------------------

		virtual void Add() = 0;

		// ---------------------------------------------------------------

		bool WaitForInitialDelay()
		{
			if (InitialRestartTimer)
			{
				InitialCurrentTime = glfwGetTime();
				InitialRestartTimer = false;
			}

			if (glfwGetTime() - InitialCurrentTime > InitialDelay)
			{
				return true;
			}

			return false;
		}

		// ---------------------------------------------------------------

		void IntervalSpawning()
		{
			// if loop is set to true

			if (vml::bits32::Get(PreferencesFlags, LOOP))
			{
				if (RestartTimer)
				{
					CurrentTime = glfwGetTime();
					RestartTimer = false;

					// add particles using a pure virtual function

					Add();
				}

				if (glfwGetTime() - CurrentTime > SpawningDelay)
				{
					// std::cout << glfwGetTime() - CurrentTime << " " << SpawningDelay << std::endl;

					RestartTimer = true;
				}
		
				//	Color = StartColor + ds * (EndColor - StartColor);

			}
			else
			{

				// if loop is not set to true

				if (Done)
					return;

				if (RestartTimer)
				{
					CurrentTime = glfwGetTime();
					RestartTimer = false;

					// add particles using a pure virtual function

					Add();

					if (FirstEmission)
					{
						FirstEmission = false;
						Ticks = 1.0f;			// start with 1 because this is the first particle emission
					}
				}
				
				if (glfwGetTime() - CurrentTime > SpawningDelay)
				{
					// if loop is not set, then check if 
					// all of the particles have been spawned
					// if so, then set flags to done
					// note that the particle animation will 
					// be complteted

					//	std::cout << Size << " : " << LifeSpan / SpawningDelay << " " << Ticks << std::endl;

					// if Size is greather than MaxSize then animation is done
					// else the number of emission events is compared to 
					// the ratio between lifespan and spawningdelay

					if (Size >= MaxSize)
						Done = true;

					if ( fabs(Ticks-LifeSpan/SpawningDelay) < vml::math::EPSILON)
						Done = true;

					RestartTimer = true;

					Ticks += 1.0f;

				}

			}

		}

		// ---------------------------------------------------------------

		void RemoveUnused()
		{
			int i = 0;

			while (i < Size)
			{
				if (Pool[i].IsDead())
				{
					// remove dead particle swapoping
					// the current particle with the
					// last in the pool according
					// to current pool size

					std::swap(Pool[i], Pool[Size - 1]);

					// original element swap

					//Particle temp;
					//Particle temp = Pool[i];
					//Pool[i] = Pool[Size - 1];
					//Pool[Size - 1] = temp;

					// slowest test version

					//	for (int j = i; j < Size - 1; ++j)
					//		Pool[j] = Pool[j + 1];

					Size--;
				}
				else
				{
					i++;
				}
			}
		}

		// ---------------------------------------------------------------
		// compute node bounding box

		void ComputeMetrics()
		{
			// compèute bounding box

			AABBoxMin = glm::vec3( FLT_MAX,  FLT_MAX,  FLT_MAX);
			AABBoxMax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

			for (size_t i = 0; i < Size; ++i)
			{
				Particle* p = &Pool[i];

				if (p->GetAABBoxMin().x < AABBoxMin.x) AABBoxMin.x = p->GetAABBoxMin().x;
				if (p->GetAABBoxMin().y < AABBoxMin.y) AABBoxMin.y = p->GetAABBoxMin().y;
				if (p->GetAABBoxMin().z < AABBoxMin.z) AABBoxMin.z = p->GetAABBoxMin().z;
				if (p->GetAABBoxMax().x > AABBoxMax.x) AABBoxMax.x = p->GetAABBoxMax().x;
				if (p->GetAABBoxMax().y > AABBoxMax.y) AABBoxMax.y = p->GetAABBoxMax().y;
				if (p->GetAABBoxMax().z > AABBoxMax.z) AABBoxMax.z = p->GetAABBoxMax().z;
			}

			// compute bounding sphere

			float dx = AABBoxMax.x - AABBoxMin.x;
			float dy = AABBoxMax.y - AABBoxMin.y;
			float dz = AABBoxMax.z - AABBoxMin.z;

			Radius = sqrtf(dx * dx + dy * dy + dz * dz) * 0.5f;

			AABBoxCenter.x = (AABBoxMax.x + AABBoxMin.x) * 0.5f;
			AABBoxCenter.y = (AABBoxMax.y + AABBoxMin.y) * 0.5f;
			AABBoxCenter.z = (AABBoxMax.z + AABBoxMin.z) * 0.5f;

		}

		// ---------------------------------------------------------------
		//

		void ComputeSphericalMatrix()
		{
			// avoid special case where forward vector adn up has same directxion

			float phi = (Phi + vml::math::EPSILON) * vml::math::DEGTORAD;
			float theta = Theta * vml::math::DEGTORAD;

			// compute spherical coordinates

			float sphi   = sin(phi);
			float cphi   = cos(phi);
			float stheta = sin(theta);
			float ctheta = cos(theta);

			// up vector

			glm::vec3 u(0, 1, 0);

			// compute forward vector in spherical coordinates

			Forward = glm::normalize(glm::vec3(sphi * ctheta, cphi, -sphi * stheta));

			// compute up and right vectors to create anotrhonormal basis

			//	if (fabs(u.x - Forward.x) < vml::math::EPSILON &&
			//		fabs(u.y - Forward.y) < vml::math::EPSILON &&
			//		fabs(u.z - Forward.z) < vml::math::EPSILON)
			//	{
			//		u.x = vml::math::EPSILON;
			//		u.y = 1.0f + vml::math::EPSILON;
			//		u.z = vml::math::EPSILON;
			//	}

			Up    = glm::normalize(glm::cross(Forward, u));
			Right = glm::normalize(glm::cross(Up, Forward));

			// transform points

			float* m = glm::value_ptr(M);

			m[ 0] = Up[0];
			m[ 1] = Up[1];
			m[ 2] = Up[2];
			m[ 4] = Forward[0];
			m[ 5] = Forward[1];
			m[ 6] = Forward[2];
			m[ 8] = Right[0];
			m[ 9] = Right[1];
			m[10] = Right[2];
			m[12] = Position.x;
			m[13] = Position.y;
			m[14] = Position.z;
		
		}

		// -----------------------------------------------------------------------
		// Compute euler matrix

		void ComputeEulerMatrix()
		{
			// get angles

			float yaw   = Yaw   * vml::math::DEGTORAD;
			float pitch = Pitch * vml::math::DEGTORAD;
			float roll  = Roll  * vml::math::DEGTORAD;

			// clamp angles

			if (yaw < 0.0) yaw = fmod(yaw, vml::math::PIMUL2) + vml::math::PIMUL2;
				else if (yaw > vml::math::PI) yaw = fmod(yaw, vml::math::PIMUL2);

			if (pitch < 0.0) pitch = fmod(pitch, vml::math::PIMUL2) + vml::math::PIMUL2;
				else if (pitch > vml::math::PI) pitch = fmod(pitch, vml::math::PIMUL2);

			if (roll < 0.0) roll = fmod(roll, vml::math::PIMUL2) + vml::math::PIMUL2;
				else if (roll > vml::math::PI) roll = fmod(roll, vml::math::PIMUL2);

			// cache trigvalues

			float sx = sin(yaw);
			float sy = sin(pitch);
			float sz = sin(roll);
			float cx = cos(yaw);
			float cy = cos(pitch);
			float cz = cos(roll);

			// get parent matrix pointer

			float* matrix = glm::value_ptr(M);

			matrix[ 0] =  cy * cz;
			matrix[ 1] =  sx * sy * cz + cx * sz;
			matrix[ 2] = -cx * sy * cz + sx * sz;
			matrix[ 3] =  0;
			matrix[ 4] = -cy * sz;
			matrix[ 5] = -sx * sy * sz + cx * cz;
			matrix[ 6] =  cx * sy * sz + sx * cz;
			matrix[ 7] =  0;
			matrix[ 8] =  sy;
			matrix[ 9] = -sx * cy;
			matrix[10] =  cx * cy;
			matrix[11] =  0;
			matrix[12] =  Position.x;
			matrix[13] =  Position.y;
			matrix[14] =  Position.z;
			matrix[15] =  1;

		}

	public:

		// ---------------------------------------------------------------
		// global flags

		static const uint32 LOOP                 = vml::bits32::BIT0;
		static const uint32 VISIBLE              = vml::bits32::BIT1;
		
		static const uint32 DRAW_BOUNDING_BOX    = vml::bits32::BIT16;
		static const uint32 DRAW_BOUNDING_SPHERE = vml::bits32::BIT17;
		static const uint32 DRAW_REF_SYSTEM      = vml::bits32::BIT18;

		// ---------------------------------------------------------------
		// update

		virtual void Update(vml::views::View* view, float dt)
		{
			// if interval spawning is 0 , then spawn particles
			// according to spawing delay

			if (fabs(InitialDelay) < vml::math::EPSILON)
			{
				IntervalSpawning();
			}
			else
			{
				// wait for time to initiate emission

				if (WaitForInitialDelay())
				{
					IntervalSpawning();
				}
			}

			// update particles

			for (size_t i = 0; i < Size; ++i)
			{
				Pool[i].Update(view, dt);
			}

			// compute node bounding box

			ComputeMetrics();

			// remove particles when their lifespan has been exhausted
			// partcile Size is updated as well

			RemoveUnused();

			// compute particle buffer 

			ParticleBufferSize = 0;

			for (size_t i = 0; i < Size; ++i)
			{
			
				const Particle* p = &Pool[i];

				// positions

				const glm::vec3& r0 = p->GetR0();
				const glm::vec3& r1 = p->GetR1();
				const glm::vec3& r2 = p->GetR2();
				const glm::vec3& r3 = p->GetR3();

				int j = ParticleBufferSize * 16;

				ParticlePositionBuffer[j    ] = r0.x;
				ParticlePositionBuffer[j + 1] = r0.y;
				ParticlePositionBuffer[j + 2] = r0.z;
				ParticlePositionBuffer[j + 3] = 1.0f;

				ParticlePositionBuffer[j + 4] = r1.x;
				ParticlePositionBuffer[j + 5] = r1.y;
				ParticlePositionBuffer[j + 6] = r1.z;
				ParticlePositionBuffer[j + 7] = 1.0f;

				ParticlePositionBuffer[j +  8] = r2.x;
				ParticlePositionBuffer[j +  9] = r2.y;
				ParticlePositionBuffer[j + 10] = r2.z;
				ParticlePositionBuffer[j + 11] = 1.0f;

				ParticlePositionBuffer[j + 12] = r3.x;
				ParticlePositionBuffer[j + 13] = r3.y;
				ParticlePositionBuffer[j + 14] = r3.z;
				ParticlePositionBuffer[j + 15] = 1.0f;

				// color

				const glm::vec4& col = p->GetColor();

				ParticleColorBuffer[j     ] = col.x;
				ParticleColorBuffer[j +  1] = col.y;
				ParticleColorBuffer[j +  2] = col.z;
				ParticleColorBuffer[j +  3] = col.w;

				ParticleColorBuffer[j +  4] = col.x;
				ParticleColorBuffer[j +  5] = col.y;
				ParticleColorBuffer[j +  6] = col.z;
				ParticleColorBuffer[j +  7] = col.w;

				ParticleColorBuffer[j +  8] = col.x;
				ParticleColorBuffer[j +  9] = col.y;
				ParticleColorBuffer[j + 10] = col.z;
				ParticleColorBuffer[j + 11] = col.w;

				ParticleColorBuffer[j + 12] = col.x;
				ParticleColorBuffer[j + 13] = col.y;
				ParticleColorBuffer[j + 14] = col.z;
				ParticleColorBuffer[j + 15] = col.w;

				// uv coordinates

				int k = ParticleBufferSize * 8;

				const glm::vec2& u0 = p->GetU0();
				const glm::vec2& u1 = p->GetU1();
				const glm::vec2& u2 = p->GetU2();
				const glm::vec2& u3 = p->GetU3();

				ParticleUVBuffer[k    ] = u0.x;
				ParticleUVBuffer[k + 1] = u0.y;
				ParticleUVBuffer[k + 2] = u1.x;
				ParticleUVBuffer[k + 3] = u1.y;
				ParticleUVBuffer[k + 4] = u2.x;
				ParticleUVBuffer[k + 5] = u2.y;
				ParticleUVBuffer[k + 6] = u3.x;
				ParticleUVBuffer[k + 7] = u3.y;
							
				// increse counter 

				ParticleBufferSize++;

			}
		
		}

		// ---------------------------------------------------------------
		// add a single particle to the particle pool
		// * you must give the direction vector *

		void AddParticleToPool(const glm::vec3& transformedemitterposition, const glm::vec3& direction)
		{
			if (Size < MaxSize)
			{
				Pool[Size].SetParms(Size,
									transformedemitterposition,
									PositionDeviation,
									direction * MeanVelocity,
									VelocityDeviation,
									Accelleration, AccellerationDeviation,
									Angle, AngleDeviation,
									AngularVelocity, AngularVelocityDeviation,
									AngularAccelleration, AngularAccellerationDeviation,
									BillBoardInitialScale, BillBoardFinalScale,
									QuadInitialScale,QuadFinalScale,
								    LifeSpan,
									Particle::ALIVE,
									DrawMode,
									RotationMode,
									InitialColor,
								    FinalColor);

				// new particle added from particles pool

				Size++;
			}
			// else
			{
				//	std::cout << "max partricle count exceeded" << std::endl;
			}
		}

		// ---------------------------------------------------------------
		// 

		void ComputeMatrix()
		{
			if (RotationMode == Particle::EULER_MATRIX)
				ComputeEulerMatrix();
			else if (RotationMode == Particle::QUATERNION_MATRIX)
				ComputeSphericalMatrix();
		}

		// ---------------------------------------------------------------
		// reset

		virtual void Reset(bool fullreset=false)
		{
			if (fullreset)
			{
				Position			 = InitialPosition;
				Velocity			 = InitialVelocity;
				Accelleration		 = InitialAccelleration;
				Angle				 = InitialAngle;
				AngularVelocity		 = InitialAngularVelocity;
				AngularAccelleration = InitialAngularAccelleration;
				BillBoardScale		 = BillBoardInitialScale;
				QuadScale			 = QuadInitialScale;
				Color				 = InitialColor;
				Theta				 = 0.0f;
				Phi					 = 0.0f;
				Yaw					 = 0.0f;
				Pitch				 = 0.0f;
				Roll				 = 0.0f;
			}

			Done                 = false;
			RestartTimer         = true;
			InitialRestartTimer  = true;
			FirstEmission        = true;
			CurrentTime          = 0.0f;
			InitialCurrentTime   = 0.0f;
			Size                 = 0;
			ParticleBufferSize   = 0;
			Ticks                = 1.0f;

			// reset each particle

			for (size_t i = 0; i < MaxSize; ++i)
				Pool[i].Reset();
		}

		// ---------------------------------------------------------------
		// getters

		const std::string&	   GetScreenName()				      const { return ScreenName; }
		bool				   IsVisible()					      const { return vml::bits32::Get(PreferencesFlags, VISIBLE) == 1; }
		bool				   IsLoop()						      const { return vml::bits32::Get(PreferencesFlags, LOOP) == 1; }
		bool				   IsRotationEuler()				  const { return RotationMode == Particle::EULER_MATRIX; }
		bool				   IsRotationQuaternion()			  const { return RotationMode == Particle::QUATERNION_MATRIX; }
		bool				   IsDone()						      const { return Done && Size == 0; }
		const glm::vec3&	   GetAABBoxMin()					  const { return AABBoxMin; }
		const glm::vec3&	   GetAABBoxMax()					  const { return AABBoxMax; }
		const glm::vec3&	   GetAABBoxCenter()				  const { return AABBoxCenter; }
		float				   GetRadius()						  const { return Radius; }
		size_t				   GetParticlePositionBufferSize()    const { return size_t(MaxPoolSize) * 4; }
		size_t				   GetParticlesCount()			      const { return Size; }
		size_t				   GetMaxParticlesCount()		      const { return MaxSize; }
		size_t				   GetMaxParticlesPoolSize()	      const { return MaxPoolSize; }
		Particle*			   GetParticleAt(const size_t i)	  const { return &Pool[i]; }
		const glm::vec3&	   GetPosition()					  const { return Position; }
		const glm::vec3&   	   GetVelocity()					  const { return Velocity; }
		const glm::vec3&	   GetAccelleration()				  const { return Accelleration; }
		const glm::vec3&	   GetPositionDeviation()			  const { return PositionDeviation; }
		const glm::vec3&	   GetVelocityDeviation()			  const { return VelocityDeviation; }
		const glm::vec3&	   GetAccellerationDeviation()	      const { return AccellerationDeviation; }
		float				   GetMeanVelocity()				  const { return MeanVelocity; }
		float				   GetAngle()					      const { return Angle; }
		float				   GetTheta()						  const { return Theta; }
		float				   GetPhi()							  const { return Phi; }
		float				   GetYaw()							  const { return Yaw; }
		float				   GetPicth()						  const { return Pitch; }
		float				   GetRoll()						  const { return Roll; }
		float				   GetAngularVelocity()			      const { return AngularVelocity; }
		float				   GetAngularAccelleration()	      const { return AngularAccelleration; }
		float				   GetAngleDeviation()				  const { return AngleDeviation; }
		float				   GetAngularVelocityDeviation()	  const { return AngularVelocityDeviation; }
		float				   GetAngularAccellerationDeviation() const { return AngularAccellerationDeviation; }
		float				   GetSpawningDelay()				  const { return SpawningDelay; }
		float				   GetInitialDelay()				  const { return InitialDelay; }
		float				   GetLifeSpan()					  const { return LifeSpan; }
		const glm::vec2&	   GetBillBoardScale()				  const { return BillBoardScale; }
		const glm::vec2&	   GetInitialBillBoardScale()		  const { return BillBoardInitialScale; }
		const glm::vec2&	   GetFinalBillBoardScale()			  const { return BillBoardFinalScale; }
		const glm::vec2&	   GetQuadScale()					  const { return QuadScale; }
		const glm::vec2&	   GetInitialQuadScale()			  const { return QuadInitialScale; }
		const glm::vec2&	   GetFinalQuadScale()				  const { return QuadFinalScale; }
		const glm::vec3&	   GetUpVector()					  const { return Up; }
		const glm::vec3&	   GetRightVector()					  const { return Right; }
		const glm::vec3&	   GetForwardVector()				  const { return Forward; }
		uint32				   GetDrawMode()					  const { return DrawMode; }
		bool				   IsBoundingBoxVisible()			  const { return vml::bits32::Get(PreferencesFlags, DRAW_BOUNDING_BOX) == 1; }
		bool				   IsBoundingSphereVisible()		  const { return vml::bits32::Get(PreferencesFlags, DRAW_BOUNDING_SPHERE) == 1; }
		bool				   IsRefSystemVisible()				  const { return vml::bits32::Get(PreferencesFlags, DRAW_REF_SYSTEM) == 1; }
		float				   GetBoundingBoxVolume()			  const { return (AABBoxMax.x - AABBoxMin.x) * (AABBoxMax.y - AABBoxMin.y) * (AABBoxMax.z - AABBoxMin.z); }
		float				   GetBoundingSphereVolume()		  const { return 4.0f * vml::math::PI * Radius / 3.0f; }
		bool				   IsDrawModeBillboards()			  const { return DrawMode == Particle::WIRE_BILLBOARDS || 
																			     DrawMode == Particle::SOLID_BILLBOARDS || 
																			     DrawMode == Particle::TEXTURED_BILLBOARDS ||
																				 DrawMode == Particle::TEXTURED_ALPHA_BILLBOARDS ||
																				 DrawMode == Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS; }
		bool				   IsDrawModeQuads()				  const { return DrawMode == Particle::WIRE_QUADS || 
																			     DrawMode == Particle::SOLID_QUADS || 
																				 DrawMode == Particle::TEXTURED_QUADS ||
																				 DrawMode == Particle::TEXTURED_ALPHA_QUADS ||
																				 DrawMode == Particle::TEXTURED_ALPHA_COLORED_QUADS; }
		bool				    HasTexture()					  const { return Texture != nullptr; }
		const float*		    GetParticlePositionBuffer()		  const { return ParticlePositionBuffer; }
		const float*		    GetParticleUVBuffer()			  const { return ParticleUVBuffer; }
		const float*		    GetParticleColorBuffer()		  const { return ParticleColorBuffer; }
		const int			    GetParticleBufferSize()			  const { return ParticleBufferSize; }
		bool				    IsDrawModeWire()				  const { return DrawMode == Particle::WIRE_BILLBOARDS || DrawMode == Particle::WIRE_QUADS; }
		bool				    IsDrawModeSolid()				  const { return DrawMode == Particle::SOLID_BILLBOARDS || DrawMode == Particle::SOLID_QUADS; }
		bool				    IsDrawModeTextured()			  const { return DrawMode == Particle::TEXTURED_BILLBOARDS || DrawMode == Particle::TEXTURED_QUADS; }
		bool				    IsDrawModeAlphaTextured()		  const { return DrawMode == Particle::TEXTURED_ALPHA_BILLBOARDS || DrawMode == Particle::TEXTURED_ALPHA_QUADS; }
		bool				    IsDrawModeAlphaColorTextured()	  const { return DrawMode == Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS || DrawMode == Particle::TEXTURED_ALPHA_COLORED_QUADS; }
		const std::string&		GetTypeString()					  const { return TypeString; }
		vml::textures::Texture* GetTexture()					  const { return Texture; }
		int						GetBlendMode()					  const { return BlendMode; }

		// ---------------------------------------------------------------
		// setters

		void SetBoundingBoxVisibleToTrue()						{ vml::bits32::SetToTrue(PreferencesFlags, DRAW_BOUNDING_BOX); }
		void SetBoundingBoxVisibleToFalse()						{ vml::bits32::SetToFalse(PreferencesFlags, DRAW_BOUNDING_BOX); }
		void SetRefSystemVisibleToTrue()						{ vml::bits32::SetToTrue(PreferencesFlags, DRAW_REF_SYSTEM); }
		void SetRefSystemVisibleToFalse()						{ vml::bits32::SetToFalse(PreferencesFlags, DRAW_REF_SYSTEM); }
		void SetBoundingSphereVisibleToTrue()					{ vml::bits32::SetToTrue(PreferencesFlags, DRAW_BOUNDING_SPHERE); }
		void SetBoundingSphereVisibleToFalse()					{ vml::bits32::SetToFalse(PreferencesFlags, DRAW_BOUNDING_SPHERE); }

		void SetLoopOn()									    { vml::bits32::SetToTrue(PreferencesFlags, LOOP); }

		void SetLoopOff()									    { vml::bits32::SetToFalse(PreferencesFlags, LOOP); }
		void SetVisibileOn()								    { vml::bits32::SetToTrue(PreferencesFlags, VISIBLE); }
		void SetVisibileOff()								    { vml::bits32::SetToFalse(PreferencesFlags, VISIBLE); }

		void SetPosition(const glm::vec3& dp)					{ Position = dp; }
		void SetPositionDeviation(const glm::vec3& dp)			{ PositionDeviation = dp; }

		void SetMeanVelocity(float vel)							{ MeanVelocity = vel; }
		void SetVelocity(const glm::vec3& dv)					{ Velocity = dv; }
		void SetVelocityDeviation(const glm::vec3& dv)			{ VelocityDeviation = dv; }

		void SetAccelleration(const glm::vec3& da)			    { Accelleration = da; }
		void SetAccellerationDeviation(const glm::vec3& da)		{ AccellerationDeviation = da; }

		void SetAngle(const float angle)						{ Angle = angle; }
		void SetAngleDeviation(const float da)					{ AngleDeviation = da; }

		void SetAngularVelocity(const float omega)				{ AngularVelocity = omega; }

		void SetAngularVelocityDeviation(const float domega)	{ AngularVelocityDeviation = domega; }
		void SetAngularAccelleration(const float alpha)			{ AngularAccelleration = alpha; }
		void SetAngularAccellerationDeviation(const float dacc) { AngularAccellerationDeviation = dacc; }

		void SetInitialBillBoardScale(const glm::vec2& sc)		{ BillBoardInitialScale = sc; }
		void SetFinalBillBoardScale(const glm::vec2& sc)		{ BillBoardFinalScale = sc; }

		void SetInitialQuadScale(const glm::vec2& sc)			{ QuadInitialScale = sc; }
		void SetFinalQuadScale(const glm::vec2& sc)				{ QuadFinalScale = sc; }

		void SetTheta(const float val)							{ Theta = val; }
		void SetPhi(const float val)							{ Phi = val; }
		void SetYaw(const float val)							{ Yaw = val; }
		void SetPitch(const float val)						    { Pitch = val; }
		void SetRoll(const float val)							{ Roll = val; }
	
		// ---------------------------------------------------------------
		//

		void SetQuads()
		{
			if (DrawMode == Particle::WIRE_BILLBOARDS)				     DrawMode = Particle::WIRE_QUADS;
			if (DrawMode == Particle::SOLID_BILLBOARDS)					 DrawMode = Particle::SOLID_QUADS;
			if (DrawMode == Particle::TEXTURED_BILLBOARDS)				 DrawMode = Particle::TEXTURED_QUADS;
			if (DrawMode == Particle::TEXTURED_ALPHA_BILLBOARDS)		 DrawMode = Particle::TEXTURED_ALPHA_QUADS;
			if (DrawMode == Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS) DrawMode = Particle::TEXTURED_ALPHA_COLORED_QUADS;
		}

		// ---------------------------------------------------------------
		//

		void SetBillboards()
		{
			if (DrawMode == Particle::WIRE_QUADS)					DrawMode = Particle::WIRE_BILLBOARDS;
			if (DrawMode == Particle::SOLID_QUADS)					DrawMode = Particle::SOLID_BILLBOARDS;
			if (DrawMode == Particle::TEXTURED_QUADS)				DrawMode = Particle::TEXTURED_BILLBOARDS;
			if (DrawMode == Particle::TEXTURED_ALPHA_QUADS)			DrawMode = Particle::TEXTURED_ALPHA_BILLBOARDS;
			if (DrawMode == Particle::TEXTURED_ALPHA_COLORED_QUADS) DrawMode = Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS;
		}

		// ---------------------------------------------------------------
		//

		void SetWire()
		{
			if (DrawMode == Particle::WIRE_QUADS)						 DrawMode = Particle::WIRE_QUADS;
			if (DrawMode == Particle::WIRE_BILLBOARDS)					 DrawMode = Particle::WIRE_BILLBOARDS;
			if (DrawMode == Particle::SOLID_BILLBOARDS)					 DrawMode = Particle::WIRE_BILLBOARDS;
			if (DrawMode == Particle::SOLID_QUADS)						 DrawMode = Particle::WIRE_QUADS;
			if (DrawMode == Particle::TEXTURED_BILLBOARDS)				 DrawMode = Particle::WIRE_BILLBOARDS;
			if (DrawMode == Particle::TEXTURED_QUADS)					 DrawMode = Particle::WIRE_QUADS;
			if (DrawMode == Particle::TEXTURED_ALPHA_BILLBOARDS)		 DrawMode = Particle::WIRE_BILLBOARDS;
			if (DrawMode == Particle::TEXTURED_ALPHA_QUADS)			     DrawMode = Particle::WIRE_QUADS;
			if (DrawMode == Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS) DrawMode = Particle::WIRE_BILLBOARDS;
			if (DrawMode == Particle::TEXTURED_ALPHA_COLORED_QUADS)	     DrawMode = Particle::WIRE_QUADS;
		}

		// ---------------------------------------------------------------
		//

		void SetSolid()
		{
			if (DrawMode == Particle::SOLID_BILLBOARDS)					  DrawMode = Particle::SOLID_BILLBOARDS;
			if (DrawMode == Particle::SOLID_QUADS)						  DrawMode = Particle::SOLID_QUADS;
			if (DrawMode == Particle::WIRE_BILLBOARDS)				      DrawMode = Particle::SOLID_BILLBOARDS;
			if (DrawMode == Particle::TEXTURED_BILLBOARDS)				  DrawMode = Particle::SOLID_BILLBOARDS;
			if (DrawMode == Particle::TEXTURED_ALPHA_BILLBOARDS)		  DrawMode = Particle::SOLID_BILLBOARDS;
			if (DrawMode == Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS)  DrawMode = Particle::SOLID_BILLBOARDS;
			if (DrawMode == Particle::WIRE_QUADS)						  DrawMode = Particle::SOLID_QUADS;
			if (DrawMode == Particle::TEXTURED_QUADS)					  DrawMode = Particle::SOLID_QUADS;
			if (DrawMode == Particle::TEXTURED_ALPHA_QUADS)				  DrawMode = Particle::SOLID_QUADS;
			if (DrawMode == Particle::TEXTURED_ALPHA_COLORED_QUADS)	      DrawMode = Particle::SOLID_QUADS;
		}

		// ---------------------------------------------------------------
		//

		void SetDrawModeTextured()
		{
			if ( DrawMode == Particle::WIRE_QUADS  || 
				 DrawMode == Particle::SOLID_QUADS || 
				 DrawMode == Particle::TEXTURED_QUADS ||
				 DrawMode == Particle::TEXTURED_ALPHA_QUADS ||
				 DrawMode == Particle::TEXTURED_ALPHA_COLORED_QUADS ) DrawMode = Particle::TEXTURED_QUADS;
		
			if ( DrawMode == Particle::WIRE_BILLBOARDS || 
				 DrawMode == Particle::SOLID_BILLBOARDS ||
				 DrawMode == Particle::TEXTURED_BILLBOARDS ||
				 DrawMode == Particle::TEXTURED_ALPHA_BILLBOARDS ||
				 DrawMode == Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS ) DrawMode = Particle::TEXTURED_BILLBOARDS;
		}

		// ---------------------------------------------------------------
		//

		void SetDrawModeTexturedAlpha()
		{
			if (DrawMode == Particle::WIRE_QUADS ||
				DrawMode == Particle::SOLID_QUADS ||
				DrawMode == Particle::TEXTURED_QUADS ||
				DrawMode == Particle::TEXTURED_ALPHA_QUADS ||
				DrawMode == Particle::TEXTURED_ALPHA_COLORED_QUADS) DrawMode = Particle::TEXTURED_ALPHA_QUADS;

			if (DrawMode == Particle::WIRE_BILLBOARDS ||
				DrawMode == Particle::SOLID_BILLBOARDS ||
				DrawMode == Particle::TEXTURED_BILLBOARDS ||
				DrawMode == Particle::TEXTURED_ALPHA_BILLBOARDS ||
				DrawMode == Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS) DrawMode = Particle::TEXTURED_ALPHA_BILLBOARDS;
		}

		// ---------------------------------------------------------------
		//

		void SetDrawModeTexturedAlphaColored()
		{
			if (DrawMode == Particle::WIRE_QUADS ||
				DrawMode == Particle::SOLID_QUADS ||
				DrawMode == Particle::TEXTURED_QUADS ||
				DrawMode == Particle::TEXTURED_ALPHA_QUADS ||
				DrawMode == Particle::TEXTURED_ALPHA_COLORED_QUADS) DrawMode = Particle::TEXTURED_ALPHA_COLORED_QUADS;

			if (DrawMode == Particle::WIRE_BILLBOARDS ||
				DrawMode == Particle::SOLID_BILLBOARDS ||
				DrawMode == Particle::TEXTURED_BILLBOARDS ||
				DrawMode == Particle::TEXTURED_ALPHA_BILLBOARDS ||
				DrawMode == Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS) DrawMode = Particle::TEXTURED_ALPHA_COLORED_BILLBOARDS;
		}

		// ---------------------------------------------------------------
		//

		void SetSpawningDelay(float val)
		{
			if (val < 0)  val = 0.01f;
			if (val > LifeSpan) val = LifeSpan;
			SpawningDelay = val;
		}

		// ---------------------------------------------------------------
		//

		void SetLifeSpan(float val)
		{
			if (val < 0.01f) val = 0.01f;
			if (val < SpawningDelay) val = SpawningDelay;
			LifeSpan = val;
		}

		// ---------------------------------------------------------------
		//

		void SetInitialDelay(float val)
		{
			if (val < 0) val = 0.01f;
			InitialDelay = val;
		}

		// ---------------------------------------------------------------
		// 

		void SetTexture(const vml::textures::Texture* texture)
		{
			if (!texture)
				vml::os::Message::Error("Particle ","Texture pointer cannot be null");
			Texture = const_cast<vml::textures::Texture*> (texture);
		}

		// ---------------------------------------------------------------
		// 

		void SetBlendMode(int blendmode) 
		{
			if ( blendmode != vml::debugrendering::AlphaTexturedQuad::ADDICTIVE_BLENDING_NO_ALPHA &&
				 blendmode != vml::debugrendering::AlphaTexturedQuad::ADDICTIVE_BLENDING &&
				 blendmode != vml::debugrendering::AlphaTexturedQuad::TRANSPARENCY )
					vml::os::Message::Error("Particle "," Unknown blend mode");
			BlendMode = blendmode; 
		}

		// ---------------------------------------------------------------
		//

		virtual void Draw(vml::views::View* view) = 0;

		// ---------------------------------------------------------------
		// ctor / dtor

		ParticleNodeBase(const std::string& screenname,
						 const std::string &typestring,
						 const glm::vec3& position,
						 const glm::vec3& positiondeviation,
						 const float meanvelocity,
						 const glm::vec3& velocity,
						 const glm::vec3& velocitydeviation,
						 const glm::vec3& accelleration,
						 const glm::vec3& accellerationdeviation,
						 const float angle,
						 const float angledeviation,
						 const float angularvelocity,
						 const float angularvelocitydeviation,
						 const float angularaccelleration,
						 const float angularaccellerationdeviation,
						 const glm::vec2 billboardinitialparticlescale,
						 const glm::vec2 billboardifinalparticlescale,
						 const glm::vec2 quadinitialparticlescale,
						 const glm::vec2 quadfinalparticlescale,
						 const float spawningdelay,
						 const float initialdelay,
						 const float lifespan,
						 const int maxparticlescount,
						 const int maxparticlespoolsize,
						 const uint32 flags,
						 const uint32 drawmode,
						 const uint32 rotationmode,
						 const glm::vec4 &initialcolor,
						 const glm::vec4& finalcolor)
		{
			if (maxparticlespoolsize <= 0)
				vml::os::Message::Error("Pool : ", "poolsize must be positive");
			if (maxparticlescount <= 0)
				vml::os::Message::Error("Pool : ", "particles count must be positive");
			if (maxparticlescount >= maxparticlespoolsize)
				vml::os::Message::Error("Pool : ", "particles must be less than poolsize");
			if ( typestring.size()==0)
				vml::os::Message::Error("Pool : ", "particles must have a typestring");

			// namme of this node

			ScreenName = screenname;
			TypeString = typestring;

			// position 

			Position          = position;
			InitialPosition   = position;
			PositionDeviation = positiondeviation;

			// velocity

			MeanVelocity	  = meanvelocity;
			Velocity		  = velocity;
			InitialVelocity   = velocity;
			VelocityDeviation = velocitydeviation;

			// accelleration

			Accelleration          = accelleration;
			InitialAccelleration   = accelleration;
			AccellerationDeviation = accellerationdeviation;

			// angle

			Angle          = angle;
			InitialAngle   = angle;
			AngleDeviation = angledeviation;

			// anguilar velocity

			AngularVelocity          = angularvelocity;
			InitialAngularVelocity   = angularvelocity;
			AngularVelocityDeviation = angularvelocitydeviation;

			// angular accelleration

			AngularAccelleration          = angularaccelleration;
			InitialAngularAccelleration   = angularaccelleration;
			AngularAccellerationDeviation = angularaccellerationdeviation;

			// particles size

			BillBoardScale        = billboardinitialparticlescale;
			BillBoardInitialScale = billboardinitialparticlescale;
			BillBoardFinalScale   = billboardifinalparticlescale;

			QuadScale			  = quadinitialparticlescale;
			QuadFinalScale		  = quadfinalparticlescale;
			QuadInitialScale	  = quadinitialparticlescale;

			// flags

			PreferencesFlags = flags;
			Done             = false;

			// timing

			RestartTimer        = true;
			InitialRestartTimer = true;
			CurrentTime         = 0.0f;
			InitialCurrentTime  = 0.0f;
			SpawningDelay       = spawningdelay;
			InitialDelay		= initialdelay;
			LifeSpan			= lifespan;
			Ticks				= 1.0f;

			// data buffer

			MaxSize		  = maxparticlescount;
			MaxPoolSize   = maxparticlespoolsize;
			Size		  = 0;
			FirstEmission = true;
			DrawMode	  = drawmode;
			RotationMode  = rotationmode;

			Pool				   = new Particle[MaxPoolSize];
			ParticlePositionBuffer = new float[MaxPoolSize * 16];
			ParticleColorBuffer	   = new float[MaxPoolSize * 16];
			ParticleUVBuffer	   = new float[MaxPoolSize * 8];
			ParticleBufferSize	   = 0;
				
			// geometry

			AABBoxMax	 = glm::vec3(0, 0, 0);
			AABBoxMin	 = glm::vec3(0, 0, 0);
			AABBoxCenter = glm::vec3(0, 0, 0);
			Radius		 = 0.0f;
			Right		 = glm::vec3(1, 0, 0);
			Up			 = glm::vec3(0, 1, 0);
			Forward		 = glm::vec3(0, 0, 1);
			M			 = glm::mat4(0);

			Theta		 = 0.0f;
			Phi			 = 0.0f;

			Yaw			 = 0.0f;
			Pitch		 = 0.0f;
			Roll		 = 0.0f;

			InitialColor = initialcolor;
			FinalColor	 = finalcolor;
			Color		 = initialcolor;

			BlendMode    = vml::debugrendering::AlphaTexturedQuad::ADDICTIVE_BLENDING_NO_ALPHA;

			Texture		 = nullptr;
		}

		virtual	~ParticleNodeBase()
		{
			vml::SafeDeleteArray(Pool);
			vml::SafeDelete(ParticlePositionBuffer);
			vml::SafeDelete(ParticleUVBuffer);
			vml::SafeDelete(ParticleColorBuffer);
		}

};
