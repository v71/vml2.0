#pragma once

namespace fa2040
{

	//////////////////////////////////////////////////////////////////
	// player helicopter

	class CleoCopter  : public vml::objects::Object3d_2
	{

		private:
			
			vml::objects::Model3d_2*  BodyModel;
			vml::objects::Model3d_2*  CanopyModel;
			vml::objects::Model3d_2*  JetsModel;
			vml::objects::Model3d_2*  RotorModel;
			vml::objects::Model3d_2*  TailRotorModel;
			vml::objects::Model3d_2*  BladesModel;
			vml::objects::Model3d_2*  RadarModel;
			vml::objects::Model3d_2*  LandingGearModel;
			vml::objects::Model3d_2*  MissileModel[16];
			vml::objects::Model3d_2*  GunRotorModel;
			vml::objects::Model3d_2*  GunMountModel;
			vml::objects::Model3d_2*  GunPivotModel;
			vml::objects::Model3d_2*  GunModel;

			int State;
			int LandingGearState;

			float Yaw;
			float Roll;
			float Pitch;

			float TurnSpeed;			// yaw turn speed
			float RollAngleLimit;		// pitch angle limit when turining
			float ClimbAngleLimit;		// max inclination when levelling up or down
			float RollDampening;		// return to 0 pitch rate
			float RollSpeed;			// pitch decrease when reaching left or right turining limits

			float PrevYaw;				// store previous angular value used in bumping
			float PrevPitch;			// store previous angular value used in bumping
			bool DampingIsOver;			// check when harmonic damping is nearly finished
			float DampingEps;			// eps to check if damping is coming to a rest, the lower, the latter the sate will be set to true			
			float BoostFactorA;
			float BoostFactorB;
			float LandingGearSpeed;
			float BumpSpeed;

			int GunTurretState;
			float GunTurretSpeed;
			glm::vec3 GunTarget;

			bool Boost;

			float T;

			float BowLimit;
			float BowTau;
			float ClimbSpeed;

			bool UpKeyPressed;
			bool DownKeyPressed;
			bool LeftKeyPressed;
			bool RightKeyPressed;
			bool LandingGearKeyPressed;
			bool FireKeyPressed;
			bool LeftShiftKeyPressed;
			bool RightShiftKeyPressed;
			bool TabKeyPressed;
			bool LeftCtrlKeyPressed;
			bool RightCtrlKeyPressed;

			glm::vec3 DirectionVector;		// driection vector
			float	  ForceMagnitude;		// resistive viscous force
			float	  RbForceMagnitude;		// resistive viscous force
			glm::vec3 RbForceDirection;		// resisitive force direction
			glm::vec3 Velocity;				// velocity
			glm::vec3 Accelleration;		// accelleration
			float	  Mass;					// Newtonian Mass
			float	  InvMass;				// inverse of mass

			int LastHorzDirectionState;
			int LastVertDirectionState;
			int DirectionState;

			vml::geo2d::PathFinder* PathFinder;
			int						NavMeshCellId;		// id of cell in the navmesh the navmesh is segmented into 'islands'
														// this value indicates in which of these islands the 
														// cleocopter belongs to see pathfinder for more info
											
			// -------------------------------------------------------------------------
			//

			static const unsigned int NEUTRAL    = 0;
			static const unsigned int LEFT_POSE  = 1;
			static const unsigned int RIGHT_POSE = 2;
			static const unsigned int BUMP_RIGHT = 3;
			static const unsigned int BUMP_LEFT  = 4;
			static const unsigned int BUMP_DOWN  = 5;
			static const unsigned int BUMP_UP    = 6;

			// -------------------------------------------------------------------------
			//

			void ComputeBumpRight()
			{
				BumpSpeed = 1.5f;

				static int init = 0;

				if (State == BUMP_RIGHT && !init)
				{
					PrevYaw = Yaw;
					init = 1;
				}

				if (init)
				{

					Yaw += TurnSpeed * BumpSpeed;

					if (fabs(Yaw - PrevYaw) > 60.0f)
					{
						State = NEUTRAL;
						T     = 0.0f;
						init  = 0;
					}

					Roll += RollSpeed * BumpSpeed;

					if (Roll >  RollAngleLimit * 1.5f)
						Roll =  RollAngleLimit * 1.5f;
				}
			
			}

			// -------------------------------------------------------------------------
			//

			void ComputeBumpLeft()
			{
				BumpSpeed = 1.5f;

				static int init = 0;

				if (State == BUMP_LEFT && !init)
				{
					PrevYaw = Yaw;
					init = 1;
				}

				if (init)
				{

					Yaw -= TurnSpeed * BumpSpeed;

					if (fabs(Yaw - PrevYaw) > 60.0f)
					{
						State = NEUTRAL;
						T     = 0.0f;
						init  = 0;
					}

					Roll -= RollSpeed * BumpSpeed;

					if (Roll < -RollAngleLimit * 1.5f)
						Roll = -RollAngleLimit * 1.5f;
				}
			}

			// -------------------------------------------------------------------------
			//

			void ComputeBumpDown()
			{

				BumpSpeed = 1.5f;

				static int init = 0;

				if (State == BUMP_DOWN && !init)
				{
					PrevPitch = Pitch;
					init = 1;
				}

				if (init)
				{

					Pitch -= ClimbSpeed * BumpSpeed * 2.0f;

					if (fabs(Pitch - PrevPitch) > ClimbAngleLimit)
					{
						State = NEUTRAL;
						T = 0.0f;
						init = 0;
					}

				}

			}

			// -------------------------------------------------------------------------
			//

			void ComputeBumpUp()
			{

				BumpSpeed =1.5f;

				static int init = 0;

				if (State == BUMP_UP && !init)
				{
					PrevPitch = Pitch;
					init = 1;
				}

				if (init)
				{

					Pitch += ClimbSpeed * BumpSpeed *2.0f;

					if (fabs(Pitch - PrevPitch) > ClimbAngleLimit)
					{
						State = NEUTRAL;
						T = 0.0f;
						init = 0;
					}
				}

			}

			// -------------------------------------------------------------------------
			//

			void LandingGearControl()
			{

				if (LandingGearState == 0)
				{
					if (LandingGearKeyPressed)
					{
						LandingGearState = 1;
					}
				}

				if (LandingGearState == 1)
				{
					// ATTENZIONE QUI ALLA DIVISIONE per 16 PERCHE' IL MODELLO E' SCALATO di 16

					if (LandingGearModel->GetPosY() > -4.0f / 16.0f)
						LandingGearModel->MoveY(-LandingGearSpeed);
					else
						LandingGearState = 2;
				}

				if (LandingGearState == 2)
				{
					if (LandingGearKeyPressed)
					{
						LandingGearState = 3;
					}
				}

				if (LandingGearState == 3)
				{
					// ATTENZIONE QUI ALLA DIVISIONE per 16 PERCHE' IL MODELLO E' SCALATO di 16

					if (LandingGearModel->GetPosY() < -0.8f / 16.0f)
						LandingGearModel->MoveY(LandingGearSpeed);
					else
						LandingGearState = 0;
				}
			}

			// ---------------------------------------------------------------
			//

			void BumpControl()
			{
				if (State == BUMP_RIGHT) ComputeBumpRight();
				if (State == BUMP_LEFT)  ComputeBumpLeft();
				if (State == BUMP_DOWN)  ComputeBumpDown();
				if (State == BUMP_UP)	 ComputeBumpUp();
			}

			// ---------------------------------------------------------------
			//

			void GunTurretControl()
			{
				// gun aims to target

				if (GunTurretState == 1)
				{

					// orient gunrotor towrads tagert

					float df = glm::dot(GunTarget - GunRotorModel->GetTransformedPosition(), GunRotorModel->GetRightVector());

					if (df < 0)
						GunRotorModel->AddAngleY(-GunTurretSpeed);
					else
						GunRotorModel->AddAngleY(GunTurretSpeed);

					// orient gin pivot to target

					glm::vec3 v = glm::normalize(GunPivotModel->GetTransformedPosition() - GunTarget);

					df = glm::dot(v, GunPivotModel->GetUpVector());

					if (df < 0)
						GunPivotModel->AddAngleX(-GunTurretSpeed);
					else
						GunPivotModel->AddAngleX(GunTurretSpeed);

					if (GunPivotModel->GetAngles().x < 0)
						GunPivotModel->SetAngleX(0);

				}

				// gun resets to default state

				if (GunTurretState == 0)
				{	
					float gy = GunRotorModel->GetAngles().y;
					float gx = GunPivotModel->GetAngles().x;
		
					if (gy > 0)
						GunRotorModel->AddAngleY(-GunTurretSpeed);
					else if (gy <0)
						GunRotorModel->AddAngleY(GunTurretSpeed);
					else
						GunRotorModel->SetAngleY(0);

					if (gx >0)
						GunPivotModel->AddAngleX(-GunTurretSpeed);
					else if (gx <0)
						GunPivotModel->AddAngleX(GunTurretSpeed);
					else
						GunPivotModel->SetAngleX(0);
				
				}

			}

			// -------------------------------------------------------------------------
			//

			void MoveRight()
			{
				if ( LeftShiftKeyPressed || RightShiftKeyPressed)
					Yaw -= TurnSpeed * BoostFactorA * 2.0f;
			
				if (!LeftShiftKeyPressed || !RightShiftKeyPressed)
					Yaw -= TurnSpeed * BoostFactorA;

				if (Yaw < 0.0f)
				{
					Yaw = 0.0f;

					Roll += RollSpeed;

					if (Roll > 0.0f)
					{
						Roll = 0.0f;

						if (DampingIsOver)
							State = RIGHT_POSE;
					}
				}
				else
				{
					Roll -= RollSpeed*2;

				//	if (Roll < -RollAngleLimit)
				//		Roll = -RollAngleLimit;
				}

			}

			// -------------------------------------------------------------------------
			//

			void MoveLeft()
			{
				if (LeftShiftKeyPressed || RightShiftKeyPressed)
					Yaw += TurnSpeed * 2.0f*BoostFactorA;

				if (!LeftShiftKeyPressed || !RightShiftKeyPressed)
					Yaw += TurnSpeed * BoostFactorA;

				if (Yaw > 180.0f)
				{
					Yaw = 180.0f;

					Roll -= RollSpeed;

					if (Roll < 0.0f)
					{
						Roll = 0.0f;

						if (DampingIsOver)
							State = LEFT_POSE;
					}
				}
				else
				{
					Roll += RollSpeed*2;

				//	if (Roll > RollAngleLimit)
				//		Roll = RollAngleLimit;
				}

			}

		public:
			
			// --------------------------------------------------------
			//
						
			static const unsigned int KEY_UP		    =  0;
			static const unsigned int KEY_DOWN		    =  1;
			static const unsigned int KEY_LEFT		    =  2;
			static const unsigned int KEY_RIGHT		    =  3;
			static const unsigned int KEY_L			    =  4;
			static const unsigned int KEY_Q			    =  5;
			static const unsigned int KEY_LEFT_SHIFT    =  6;
			static const unsigned int KEY_RIGHT_SHIFT   =  7;
			static const unsigned int KEY_TAB		    =  8;
			static const unsigned int KEY_LEFT_CONTROL  =  9;
			static const unsigned int KEY_RIGHT_CONTROL = 10;

			// -------------------------------------------------------------------------
			//
			
			const std::string GetStateString()
			{
				switch (State)
				{
					case NEUTRAL    : return "Neutral";			    break;
					case LEFT_POSE  : return "Left Limit Reached";	break;
					case RIGHT_POSE : return "Right Limit Reached"; break;
					case BUMP_RIGHT : return "Bump Right";			break;
					case BUMP_LEFT  : return "Bump Left";			break;
					case BUMP_DOWN  : return "Bump Down";			break;
					case BUMP_UP    : return "Bump Up";			    break;
				}
				return "Unknown State";
			}

			//------------------------------------------------------------------
			// physics getters

			float GetMass()						   const { return Mass; }
			float GetForceMagnitude()			   const { return ForceMagnitude; }
			float GetRbForceMagnitude()            const { return RbForceMagnitude; }
			const glm::vec3& GetRbForceDirection() const { return RbForceDirection; }
			const glm::vec3& GetAccelleration()	   const { return Accelleration; }
			const glm::vec3& GetVelocity()		   const { return Velocity; }
			bool IsKeyUpPressed()				   const { return UpKeyPressed; }
			bool IsKeyDownPressed()				   const { return DownKeyPressed; }
			bool IsKeyRightPressed()			   const { return RightKeyPressed; }
			bool IsKeyLeftPressed()				   const { return LeftKeyPressed; }

			//------------------------------------------------------------------
			// physics setters

			void SetMass(float mass) { Mass = mass; }
			void SetVelocity(const glm::vec3& velocity) { Velocity = velocity; }
			void AddVelocity(const glm::vec3& dv) { Velocity += dv*BoostFactorB; }

			// -------------------------------------------------------------------------
			//

			void BindKey(GLFWwindow* glwindow)
			{
			
				UpKeyPressed          = false;
				DownKeyPressed        = false;
				RightKeyPressed       = false;
				LeftKeyPressed        = false;
				LandingGearKeyPressed = false; 
				FireKeyPressed        = false;
				LeftShiftKeyPressed   = false;
				RightShiftKeyPressed  = false; 
				TabKeyPressed         = false;
				LeftCtrlKeyPressed    = false; 
				RightCtrlKeyPressed   = false; 

				if (glfwGetKey(glwindow, GLFW_KEY_UP))    { UpKeyPressed    = true; }
				if (glfwGetKey(glwindow, GLFW_KEY_DOWN))  { DownKeyPressed  = true; }
				if (glfwGetKey(glwindow, GLFW_KEY_LEFT))  { LeftKeyPressed  = true; }
				if (glfwGetKey(glwindow, GLFW_KEY_RIGHT)) { RightKeyPressed = true; }

				if (glfwGetKey(glwindow, GLFW_KEY_L))			  { LandingGearKeyPressed = true; }
				if (glfwGetKey(glwindow, GLFW_KEY_Q))             { FireKeyPressed		  = true;}
				if (glfwGetKey(glwindow, GLFW_KEY_LEFT_SHIFT))	  { LeftShiftKeyPressed   = true; }
				if (glfwGetKey(glwindow, GLFW_KEY_RIGHT_SHIFT))   { RightShiftKeyPressed  = true; }
				if (glfwGetKey(glwindow, GLFW_KEY_TAB))			  { TabKeyPressed		  = true; }
				if (glfwGetKey(glwindow, GLFW_KEY_LEFT_CONTROL))  { LeftCtrlKeyPressed    = true;}
				if (glfwGetKey(glwindow, GLFW_KEY_RIGHT_CONTROL)) { RightCtrlKeyPressed   = true; }

			}

			// -------------------------------------------------------------------------
			//

			void Controller(vml::views::View* view)
			{

				float f = 16.0f;

				TurnSpeed        = 0.2f * f;
				ClimbSpeed       = 0.1f * f;
				RollDampening    = 0.9f;
				RollSpeed        = 0.2f * f;
				BowTau           = 100.0f;
				LandingGearSpeed = 0.005f;
				GunTurretSpeed   = 0.00125f * f;

				// check if damping is over

				DampingIsOver = Pitch > -DampingEps && Pitch < DampingEps;

				// get direction vector

				float dx = GetForwardVector().x;

				// turn left or right even if both keys are pressed
				// store previous state for both keys

				if (RightKeyPressed && !LeftKeyPressed) LastHorzDirectionState = 0;
				if (!RightKeyPressed && LeftKeyPressed) LastHorzDirectionState = 1;

				if (RightKeyPressed && LeftKeyPressed)
				{
					if (LastHorzDirectionState == 0)
					{
						RightKeyPressed = 0;
						LeftKeyPressed = 1;
					}

					if (LastHorzDirectionState == 1)
					{
						RightKeyPressed = 1;
						LeftKeyPressed = 0;
					}
				}

				// turn up or down even if both keys are pressed
				// store previous state for both keys

				if (UpKeyPressed && !DownKeyPressed) LastVertDirectionState = 0;
				if (!UpKeyPressed && DownKeyPressed) LastVertDirectionState = 1;

				if (UpKeyPressed && DownKeyPressed)
				{
					if (LastVertDirectionState == 0)
					{
						UpKeyPressed = 0;
						DownKeyPressed = 1;
					}

					if (LastVertDirectionState == 1)
					{
						UpKeyPressed = 1;
						DownKeyPressed = 0;
					}
				}

				// if tab is pressed and shift is pressed as well
				// then the boost procedure is executed
				// regardless the shiftkey being pressed

				if (TabKeyPressed && LeftShiftKeyPressed )
				{
					LeftShiftKeyPressed = false;
				}

				// boost control , this must happen right

				if (TabKeyPressed && !LeftShiftKeyPressed)
				{
					BoostFactorA = 2.0f;
					BoostFactorB = 4.0f;

					if (dx > 0)
					{
						RightKeyPressed = 1;
						LeftKeyPressed = 0;
					}
					else
					{
						RightKeyPressed = 0;
						LeftKeyPressed = 1;
					}
				}
				else
				{
					BoostFactorA = 1.0f;
					BoostFactorB = 1.0f;
				}

				// set initial direction 

				DirectionVector.x = 0;
				DirectionVector.y = 0;
				DirectionVector.z = 0;

				if (LeftKeyPressed)  DirectionVector.x -= 1;
				if (RightKeyPressed) DirectionVector.x += 1;
				if (DownKeyPressed)  DirectionVector.y -= 1;
				if (UpKeyPressed)    DirectionVector.y += 1;

				// if state is 0 then control 
				// for right and left turn

				if (State == NEUTRAL)
				{
					// right rudder control

					if (!LeftKeyPressed && RightKeyPressed)
					{
						MoveRight();
					}

					// left rudder control

					if (!RightKeyPressed && LeftKeyPressed)
					{
						MoveLeft();
					}

					// Pitch control , Climbing Up

					if (UpKeyPressed && !DownKeyPressed)
					{
						Pitch += ClimbSpeed;

						if (Pitch > ClimbAngleLimit)
							Pitch = ClimbAngleLimit;
					}

					// Pitch control , Climbing Down

					if (!UpKeyPressed && DownKeyPressed)
					{
						Pitch -= ClimbSpeed;

						if (Pitch < -ClimbAngleLimit)
							Pitch = -ClimbAngleLimit;
					}

				}

				// right direction

				if (State == RIGHT_POSE)
				{
					// if right key is pressed , then perform a forward bow

					if (RightKeyPressed)
					{
						Pitch = BowLimit - BowLimit * exp(-T * BoostFactorB / BowTau);

						T += 1.0f;
					}

					// if right key is released, reset forward bow timer

					if (!RightKeyPressed)
					{
						State = NEUTRAL;
						T = 0.0f;
					}

					// if these keys are pressed, go back to neutral state

					if (RightKeyPressed && UpKeyPressed)
					{
						State = NEUTRAL;
						T = 0.0f;
					}

					if (RightKeyPressed && DownKeyPressed)
					{
						State = NEUTRAL;
						T = 0.0f;
					}

				}

				// left direction

				if (State == LEFT_POSE)
				{

					// if left key is pressed , then perform a forward bow

					if (LeftKeyPressed)
					{
						Pitch = BowLimit - BowLimit * exp(-T * BoostFactorB / BowTau);

						T += 1.0f;
					}

					// if left key is relese, reset forward bow timer

					if (!LeftKeyPressed)
					{
						State = NEUTRAL;
						T = 0.0f;
					}

					// if these keys are pressed, go back to neutral state

					if (LeftKeyPressed && UpKeyPressed)
					{
						State = NEUTRAL;
						T = 0.0f;
					}

					if (LeftKeyPressed && DownKeyPressed)
					{
						State = NEUTRAL;
						T = 0.0f;
					}

				}

				// damp accumulated rool and pitch angles

				Roll  *= RollDampening;
				Pitch *= RollDampening;

				// bump controller

				BumpControl();

				// landing gear controller

				LandingGearControl();

				// gun turret conroller

				GunTurretControl();

				// compute forces

				ComputeForces();

			}

			// -------------------------------------------------------------------
			//

			glm::ivec2 GetNavMeshCell()
			{
				if (PathFinder) 
				{
					glm::ivec2 ci = PathFinder->GetCellIndicesFromPoint(GetBodyModel()->GetPosition());
					if (PathFinder->GetCellIdFromIndices(ci.x, ci.y) == -1)
					{
						// NavMeshCellId its the isle in the navmesh bitmap where originally the cleocopter started
						return PathFinder->FindNearestCellToAnotherCell(ci.x, ci.y, NavMeshCellId);
					}
				}
				return glm::ivec2(-1,-1);
			}

			// -------------------------------------------------------------------
			//
			
			void Bump(const vml::octree::ObjectMTV &mtv) 
			{
				if (mtv.Normal.x > 0) State = BUMP_LEFT;
				if (mtv.Normal.x < 0) State = BUMP_RIGHT;
				if (mtv.Normal.y > 0) State = BUMP_UP;
				if (mtv.Normal.y < 0) State = BUMP_DOWN;
			}

			// -------------------------------------------------------------------
			//

			void ComputeForces()
			{

				ForceMagnitude	   = 25;
				RbForceMagnitude   = 2.5f;
				
				glm::vec3 totalacc = glm::vec3(0, 0, 0);
							
				// viscous direction is the opposite of velocity

				RbForceDirection = -Velocity;
				
				// if left shoft key is not pressed then compute accelleration due to forces
				// if leftkey is pressed perform a rapid in place turn 

				if (!LeftShiftKeyPressed)
				{

					bool keypressed = UpKeyPressed || DownKeyPressed || LeftKeyPressed || RightKeyPressed;

					// normalize direction vector this is due to the fact that direction vector 
					// is the result of different sum of base vectors (1,0,0), (0,1,0), (0,0,1) 
					// and negative directions as well

					float denum = DirectionVector.x * DirectionVector.x + DirectionVector.y * DirectionVector.y + DirectionVector.z * DirectionVector.z;

					// this never happens
					
				//	if (denum > -vml::math::EPSILON && denum < vml::math::EPSILON)
				//	{
				//		DirectionVector.x = 0;
				//		DirectionVector.y = 0;
				//		DirectionVector.z = 0;
				//	}
				//	else
					{
						denum = 1.0f / denum;

						DirectionVector.x *= denum;
						DirectionVector.y *= denum;
						DirectionVector.z *= denum;
					}

					// detect keypress

					if (keypressed)
					{
						// compute accelleration due to propulsion force and viscous force

						totalacc = (DirectionVector * ForceMagnitude * BoostFactorB + RbForceMagnitude * RbForceDirection) * InvMass;
					}
					else
					{
						// comptue decelleration due to viscous force

						totalacc = RbForceMagnitude * RbForceDirection * InvMass;
					}

				}
				else
				{
					// comptue decelleration due to viscous force
				
					totalacc = RbForceMagnitude * RbForceDirection * InvMass;
				}

				// integrate

				float dt = 1.0f / 60.0f;

				Velocity += totalacc * dt ;

				// apply changes to root model

				GetBodyModel()->Move(Velocity * dt);

				BodyModel->SetAngles(glm::vec3(Roll, Pitch, Yaw));

			}

			// -------------------------------------------------------------------
			//
			
			void ComputeGunHitPoint(glm::vec3 &guntip, glm::vec3 &gundir)
			{
				gundir = GunModel->GetForwardVector();
				guntip = GunModel->GetTransformedPosition() + 0.2f * GunModel->GetForwardVector() + 0.02f * GunModel->GetUpVector();
			}

			// -------------------------------------------------------------------
			//
			
			void ActivateGunTurret()
			{
				switch(GunTurretState)
				{
					case  0: GunTurretState = 1; return; break;
					case  1: GunTurretState = 0; return; break;
				}
			}

			void SetGunTurretTarget(const glm::vec3& tgt)
			{
				GunTarget = tgt;
			}

			// -------------------------------------------------------------------
			//

			vml::objects::Model3d_2* GetBodyModel() const { return BodyModel; }
			vml::objects::Model3d_2* GetBladesModel() const { return BladesModel; }
			vml::objects::Model3d_2* GetLandingGearModel() const { return LandingGearModel; }
			int GetNavMeshCellId() const { return NavMeshCellId; }

			// -------------------------------------------------------------------
			//

			void DrawRefrenceSystem(vml::views::View* view)
			{
				vml::DebugRender->DrawObjectRefSystem(view, this, 20);
			}

			void DrawUpVector(vml::views::View* view)
			{
				vml::DebugRender->DrawObjectUpVector(view, this, 20);
			}

			void DrawRightVector(vml::views::View* view)
			{
				vml::DebugRender->DrawObjectRightVector(view, this, 20);
			}

			void DrawForwardVector(vml::views::View* view) 
			{
				vml::DebugRender->DrawObjectForwardVector(view, this, 20);
			}

			void DrawDirectionVector(vml::views::View* view) const
			{
				vml::DebugRender->DrawArrow(view, GetTransformedRootPosition(), GetTransformedRootPosition() + 25.0f * DirectionVector, vml::colors::Yellow);
			}
			
			// -------------------------------------------------------------------
			// Object creation

			void CreateObject(const std::string& sourcepath, const glm::vec3 &pos, const glm::vec3& rot, const glm::vec3& scaling)
			{
				// default rotation mode

				int rotmode = vml::objects::Model3d_2::EULER;

				// add obejcts

				AddModel("Body",
						 sourcepath + "body.3df",
						 glm::vec3(0, 0, 0),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
				//		 rotmode);
						vml::objects::Model3d_2::POLAR);
				
				AddModel("Canopy",
						 sourcepath + "canopy.3df",
						 glm::vec3(0.062, 0.426, 10.183),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);
				
				AddModel("Jets",
						 sourcepath + "jets.3df",
						 glm::vec3(-0.008f, 0.496, 2.033f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Rotor",
						 sourcepath + "main_rotor_shaft.3df",
						 glm::vec3(0.1f, 2.2f, 5.86f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);
								
				AddModel("Radar",
						 sourcepath + "radar.3df",
						 glm::vec3(0.1f, 3.3f, 5.86f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);
				
				AddModel("TailRotor",
						 sourcepath + "tail_rotor.3df",
						 glm::vec3(0.1f, -1.3f, -11.26f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);
				
				AddModel("Blades",
						 sourcepath + "mainrotor.3df",
						 glm::vec3(0.0f, 0.33f, 0.0f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Stand",
						 sourcepath + "stand.3df",
						 glm::vec3(0.05f, -3.8f, 4.02f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("GunRotor",
						 sourcepath + "gunrotor.3df",
						 glm::vec3(0.12f, -2.24f, 9.02011f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("GunMount",
						 sourcepath + "gunmount.3df",
						 glm::vec3(-0.12f, -0.84f, 0),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("GunPivot",
						 sourcepath + "gunpivot.3df",
						 glm::vec3(-0.04f, 0, 0.2f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Gun",
						 sourcepath + "gun.3df",
						 glm::vec3(0.04f, -0.24f, 1.52f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				// first missle group

				AddModel("Missile0",
						 sourcepath + "missile.3df",
						 glm::vec3(4.44, -3.32, 0.8),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile1",
						 sourcepath + "missile.3df",
						 glm::vec3(4.44, -2.79, 0.8),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile2",
						 sourcepath + "missile.3df",
						 glm::vec3(3.94, -2.79, 0.8),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile3",
						 sourcepath + "missile.3df",
						 glm::vec3(3.94, -3.32, 0.8),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);
				
				// second missile group

				AddModel("Missile4",
						 sourcepath + "missile.3df",
						 glm::vec3(2.85, -2.88, 2.5),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile5",
						 sourcepath + "missile.3df",
						 glm::vec3(2.85, -2.35, 2.5),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile6",
						 sourcepath + "missile.3df",
						 glm::vec3(2.31, -2.35, 2.5),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile7",
						 sourcepath + "missile.3df",
						 glm::vec3(2.31, -2.88, 2.5),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				// third missile group
				
				AddModel("Missile8",
						 sourcepath + "missile.3df",
						 glm::vec3(-2.31, -2.88, 2.5),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile9",
						 sourcepath + "missile.3df",
						 glm::vec3(-2.31, -2.35, 2.5),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile10",
						 sourcepath + "missile.3df",
						 glm::vec3(-2.85, -2.35, 2.5),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile11",
						 sourcepath + "missile.3df",
						 glm::vec3(-2.85, -2.88, 2.5),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);
				
				// forth missile group

				AddModel("Missile12",
						 sourcepath + "missile.3df",
						 glm::vec3(-3.94, -3.32, 0.8),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile13",
						 sourcepath + "missile.3df",
						 glm::vec3(-3.94, -2.79, 0.8),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile14",
						 sourcepath + "missile.3df",
						 glm::vec3(-4.44, -2.79, 0.8),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);

				AddModel("Missile15",
						 sourcepath + "missile.3df",
						 glm::vec3(-4.44, -3.32, 0.8),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 rotmode);
						
				// link models

				Link("Body" , "Canopy");
				Link("Body" , "Jets");
				Link("Body" , "Rotor");

				Link("Body"     , "GunRotor");
				Link("GunRotor" , "GunMount");
				Link("GunMount" , "GunPivot");
				Link("GunPivot" , "Gun");

				Link("Body"  , "TailRotor");
				Link("Rotor" , "Blades");
				Link("Body"  , "Radar");
				Link("Body"  , "Stand");
				
				Link("Body" , "Missile0");
				Link("Body" , "Missile1");
				Link("Body" , "Missile2");
				Link("Body" , "Missile3");

				Link("Body" , "Missile4");
				Link("Body" , "Missile5");
				Link("Body" , "Missile6");
				Link("Body" , "Missile7");

				Link("Body" , "Missile8");
				Link("Body" , "Missile9");
				Link("Body" , "Missile10");
				Link("Body" , "Missile11");

				Link("Body" , "Missile12");
				Link("Body" , "Missile13");
				Link("Body" , "Missile14");
				Link("Body" , "Missile15");
				
				// finalize object

				Finalize(pos, rot, scaling);

				// get models pointers
				
				BodyModel        = GetModelAt("cleocopter\\body");
				CanopyModel      = GetModelAt("cleocopter\\canopy");
				JetsModel        = GetModelAt("cleocopter\\jets");
				RotorModel		 = GetModelAt("cleocopter\\rotor");
				TailRotorModel   = GetModelAt("cleocopter\\tailrotor");
				BladesModel      = GetModelAt("cleocopter\\blades");
				RadarModel       = GetModelAt("cleocopter\\radar");
				LandingGearModel = GetModelAt("cleocopter\\stand");
				GunRotorModel	 = GetModelAt("cleocopter\\gunrotor");
				GunPivotModel	 = GetModelAt("cleocopter\\gunpivot");
				GunModel		 = GetModelAt("cleocopter\\gun");
				MissileModel[ 0] = GetModelAt("cleocopter\\missile0");
				MissileModel[ 1] = GetModelAt("cleocopter\\missile1");
				MissileModel[ 2] = GetModelAt("cleocopter\\missile2");
				MissileModel[ 3] = GetModelAt("cleocopter\\missile3");
				MissileModel[ 4] = GetModelAt("cleocopter\\missile4");
				MissileModel[ 5] = GetModelAt("cleocopter\\missile5");
				MissileModel[ 6] = GetModelAt("cleocopter\\missile6");
				MissileModel[ 7] = GetModelAt("cleocopter\\missile7");
				MissileModel[ 8] = GetModelAt("cleocopter\\missile8");
				MissileModel[ 9] = GetModelAt("cleocopter\\missile9");
				MissileModel[10] = GetModelAt("cleocopter\\missile10");
				MissileModel[11] = GetModelAt("cleocopter\\missile11");
				MissileModel[12] = GetModelAt("cleocopter\\missile12");
				MissileModel[13] = GetModelAt("cleocopter\\missile13");
				MissileModel[14] = GetModelAt("cleocopter\\missile14");
				MissileModel[15] = GetModelAt("cleocopter\\missile15");
				
				// copter starts with landing gear retracted

				LandingGearState = 1;
				LandingGearState = 0;
				LandingGearModel->MoveY(3*scaling.y);
				
				// add colliders

				BodyModel->AddEllipsoidCollider(glm::vec3(10, 5, 30)* scaling,glm::vec3(0,-20,10)*scaling);
				BladesModel->AddEllipsoidCollider(glm::vec3(30, 2, 30)*scaling);
				LandingGearModel->AddEllipsoidCollider(glm::vec3(2, 2, 8)* scaling, glm::vec3(0, -18, -4)* scaling);
			}

			// ---------------------------------------------------
			// ctor / dtor

			CleoCopter(vml::geo2d::PathFinder* pathfinder, const std::string& sourcepath,const std::string& screenname,vml::scenes::Scene* scene, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling) : vml::objects::Object3d_2(screenname)
			{
				
				BodyModel        = nullptr;
				CanopyModel      = nullptr;
				JetsModel        = nullptr;
				RotorModel       = nullptr;
				TailRotorModel   = nullptr;
				BladesModel      = nullptr;
				RadarModel       = nullptr;
				LandingGearModel = nullptr;
				GunRotorModel    = nullptr;
				GunMountModel    = nullptr;
				GunPivotModel    = nullptr;
				GunModel         = nullptr;

				MissileModel[ 0] = nullptr;
				MissileModel[ 1] = nullptr;
				MissileModel[ 2] = nullptr;
				MissileModel[ 3] = nullptr;
				MissileModel[ 4] = nullptr;
				MissileModel[ 5] = nullptr;
				MissileModel[ 6] = nullptr;
				MissileModel[ 7] = nullptr;
				MissileModel[ 8] = nullptr;
				MissileModel[ 9] = nullptr;
				MissileModel[10] = nullptr;
				MissileModel[11] = nullptr;
				MissileModel[12] = nullptr;
				MissileModel[13] = nullptr;
				MissileModel[14] = nullptr;
				MissileModel[15] = nullptr;

				UpKeyPressed          = false;
				DownKeyPressed        = false;
				LeftKeyPressed        = false;
				RightKeyPressed       = false;
				LandingGearKeyPressed = false;
				FireKeyPressed        = false;
				LeftShiftKeyPressed   = false;
				RightShiftKeyPressed  = false;
				TabKeyPressed         = false;
				LeftCtrlKeyPressed    = false;
				RightCtrlKeyPressed   = false;

				Boost = false;

				Yaw       = 0.0f;
				Roll      = 0.0f;
				Pitch     = 0.0f;
				PrevYaw   = 0.0f;
				PrevPitch = 0.0f;

				TurnSpeed		 =  0.25f;
				ClimbSpeed		 =  0.09f;
				RollDampening	 =  0.995f;					// return to 0 roll speed rate
				RollSpeed		 =  0.1f;
				BumpSpeed		 =  0.05f;

				RollAngleLimit   =  45.0f;					// maximum pitch angle
				ClimbAngleLimit  =  25.0f;					// Angle limit when climbing up or down

				BowLimit         = -15.0f;
				BowTau			 =  1.0f;
				T                =  0.0f;

				State                  = NEUTRAL;
				LastHorzDirectionState = 0;
				LastVertDirectionState = 0;
				LandingGearState       = 0;
				GunTurretState	       = 0;
				DirectionState		   = 0;
				DampingIsOver          = 0;
			
				DampingEps       = 0.01f;
				BoostFactorA     = 1.0f;
				BoostFactorB     = 1.0f;
				LandingGearSpeed = 0.005f;
				GunTurretSpeed   = 0.00125f;
				
				ForceMagnitude	  = 1.5f;								
				RbForceMagnitude  = 1.0f;								
				Mass			  = 1.0f;
				InvMass			  = 1.0f / Mass;
				RbForceDirection  = glm::vec3(0, 0, 0);
				Accelleration	  = glm::vec3(0, 0, 0);
				Velocity		  = glm::vec3(0, 0, 0);
				DirectionVector   = glm::vec3(1, 0, 0);

				NavMeshCellId	  =-1;
				PathFinder	      = nullptr;

				// create object

				CreateObject(sourcepath, pos, rot, scaling);
				
				// * Important *//
				// get copter position to retrieve navmesh cell index 
				// for the pathfiunder in the case due to navmesh segmentation
				// the player could intercept a non traversable cell 
				// this interrupting the chase function for drones
				// each isolated region in the navmesh bitmap has a value
				// assigned, we need to know in which of these region 
				// the cleocopter is
				
				if (pathfinder)
				{
					PathFinder = pathfinder;
					glm::ivec2 ci = PathFinder->GetCellIndicesFromPoint(pos);
					NavMeshCellId = PathFinder->GetCellIdFromIndices(ci.x, ci.y);
				}
				else
				{
					vml::os::Message::Error("CleoCopter :" ,"pathfinder pointer is null");
				}

			}

			~CleoCopter()
			{
			}

	};


}
