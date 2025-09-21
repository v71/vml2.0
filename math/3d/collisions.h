#pragma once

///////////////////////////////////////////////////////////////////////
// mtv

namespace vml
{
	namespace geo3d
	{
		namespace collisions
		{

			class MTV
			{
				public:

					glm::vec3 Normal        = glm::vec3(0, 0, 0);
					glm::vec3 P0	        = glm::vec3(0, 0, 0);
					glm::vec3 P1	        = glm::vec3(0, 0, 0);
					glm::vec3 P2	        = glm::vec3(0, 0, 0);
					float     Distance		= 0.f;
					bool	  IsValid       = false;
					glm::vec3 ContactPoint  = glm::vec3(0, 0, 0);
					glm::vec3 ContactNormal = glm::vec3(0, 0, 0);

					// --------------------------------------------------
					// ctor / dtor

					MTV()
					{}

					~MTV()
					{}

			};

		}
	}
}

///////////////////////////////////////////////////////////////////////
// ellipsod collider

namespace vml
{
	namespace geo3d
	{
		namespace collisions
		{

			class EllipsoidCollider
			{
					// ----------------------------------------------

					glm::vec3 Up;
					glm::vec3 Right;
					glm::vec3 Forward;
					glm::vec3 Position;
					glm::vec3 Radii;
					glm::mat4 Matrix;
					glm::vec3 BMin;
					glm::vec3 BMax;
					glm::vec3 RelativePosition;
					float	  RestitutionFactor;

				public:

					// ----------------------------------------------

					const glm::mat4& GetMatrix()		const { return Matrix; }
					const glm::vec3& GetForwardVector() const {	return Forward;	}
					const glm::vec3& GetRightVector()   const { return Right; }
					const glm::vec3& GetUpVector()      const { return Up; }
					const glm::vec3& GetPosition()      const { return Position; }
					const glm::vec3& GetRadii()         const {	return Radii; }
					const glm::vec3& GetMin()	        const { return BMin; }
					const glm::vec3& GetMax()	        const { return BMax; }
					float GetRestitutionFactor()		const { return RestitutionFactor; }

					// ----------------------------------------------

					void Transform(const float* sourcematrix, const glm::vec3& scaling)
					{
						float* mptr = glm::value_ptr(Matrix);

						float Invparentscale[3] = { Radii.x / scaling.x,
													Radii.y / scaling.y,
													Radii.z / scaling.z };

						// scale parent matrix

						mptr[ 0] = sourcematrix[ 0] * Invparentscale[0];
						mptr[ 1] = sourcematrix[ 1] * Invparentscale[0];
						mptr[ 2] = sourcematrix[ 2] * Invparentscale[0];

						mptr[ 4] = sourcematrix[ 4] * Invparentscale[1];
						mptr[ 5] = sourcematrix[ 5] * Invparentscale[1];
						mptr[ 6] = sourcematrix[ 6] * Invparentscale[1];

						mptr[ 8] = sourcematrix[ 8] * Invparentscale[2];
						mptr[ 9] = sourcematrix[ 9] * Invparentscale[2];
						mptr[10] = sourcematrix[10] * Invparentscale[2];

				//		mptr[12] = sourcematrix[12];
				//		mptr[13] = sourcematrix[13];
				//		mptr[14] = sourcematrix[14];

						mptr[12] = sourcematrix[0] * RelativePosition.x + sourcematrix[4] * RelativePosition.y + sourcematrix[ 8] * RelativePosition.z + sourcematrix[12];
						mptr[13] = sourcematrix[1] * RelativePosition.x + sourcematrix[5] * RelativePosition.y + sourcematrix[ 9] * RelativePosition.z + sourcematrix[13];
						mptr[14] = sourcematrix[2] * RelativePosition.x + sourcematrix[6] * RelativePosition.y + sourcematrix[10] * RelativePosition.z + sourcematrix[14];
	
						// get position

						Position.x = mptr[12];
						Position.y = mptr[13];
						Position.z = mptr[14];

						// compute ellipsoid bounding box
						//	We begin by noting our transformation matrix, which we値l call M.
						// We値l also need the transpose of M, which we値l call MT.
						// Next, we define a sphere S using a 4x4 matrix :

						// [r 0 0 0]
						// [0 r 0 0]
						// [0 0 r 0]
						// [0 0 0 - 1]
						
						// where r is the radius of the sphere.So for a unit diameter sphere, r = .5. ( for some reason this code works with r=4 )
						// Once we have built S, we値l take its inverse, which we値l call SI.
						//	We now calculate a new 4x4 matrix R = M * SI * MT.R should be symmetric when we池e done, 
						// //such that R = transpose(R).We値l assign R痴 indices the following names :

						// R = [r11 r12 r13 r14]
						//	   [r12 r22 r23 r24]
						//	   [r13 r23 r23 r24]
						//	   [r14 r24 r24 r24]
						//
						//	   Using R, we can now get our bounds :

						// zmax = (r23 + sqrt(pow(r23, 2) - (r33 * r22))) / r33;
						// zmin = (r23 - sqrt(pow(r23, 2) - (r33 * r22))) / r33;
						// ymax = (r13 + sqrt(pow(r13, 2) - (r33 * r11))) / r33;
						// ymin = (r13 - sqrt(pow(r13, 2) - (r33 * r11))) / r33;
						// xmax = (r03 + sqrt(pow(r03, 2) - (r33 * r00))) / r33;
						// xmin = (r03 - sqrt(pow(r03, 2) - (r33 * r00))) / r33;

						// compute the bounding box using the above formulas
						
						// glm::mat4 S{ 4,0,0,0,
						//	 		    0,4,0,0,
						//			    0,0,4,0,
						//			    0,0,0,-1 };

						// glm::mat4 M = { mptr[0],mptr[1],mptr[ 2],mptr[ 3],
						//				   mptr[4],mptr[5],mptr[ 6],mptr[ 7],
						//				   mptr[8],mptr[9],mptr[10],mptr[11],
						//				   0,0,0,1	};

						// glm::mat4 MT = glm::transpose(M);
						// glm::mat4 SI = glm::inverse(S);
						// glm::mat4 R = M * MT * SI;

						// float zmax = (R[2][3] + sqrt(pow(R[2][3], 2) - (R[3][3] * R[2][2]))) / R[3][3];
						// float zmin = (R[2][3] - sqrt(pow(R[2][3], 2) - (R[3][3] * R[2][2]))) / R[3][3];
						// float ymax = (R[1][3] + sqrt(pow(R[1][3], 2) - (R[3][3] * R[1][1]))) / R[3][3];
						// float ymin = (R[1][3] - sqrt(pow(R[1][3], 2) - (R[3][3] * R[1][1]))) / R[3][3];
						// float xmax = (R[0][3] + sqrt(pow(R[0][3], 2) - (R[3][3] * R[0][0]))) / R[3][3];
						// float xmin = (R[0][3] - sqrt(pow(R[0][3], 2) - (R[3][3] * R[0][0]))) / R[3][3];
						
						// RA0 = glm::vec3(xmin, ymin, zmin) + Position;
						// RA1 = glm::vec3(xmax, ymin, zmin) + Position;
						// RA2 = glm::vec3(xmax, ymax, zmin) + Position;
						// RA3 = glm::vec3(xmin, ymax, zmin) + Position;

						// RB0 = glm::vec3(xmin, ymin, zmax) + Position;
						// RB1 = glm::vec3(xmax, ymin, zmax) + Position;
						// RB2 = glm::vec3(xmax, ymax, zmax) + Position;
						// RB3 = glm::vec3(xmin, ymax, zmax) + Position;
					
						// optimized version

						float r = 4;
						float det = 1.0f / r;

						float s00 =  (mptr[0] * mptr[0] + mptr[4] * mptr[4] + mptr[ 8] * mptr[ 8]) * det;
						float s11 =  (mptr[1] * mptr[1] + mptr[5] * mptr[5] + mptr[ 9] * mptr[ 9]) * det;
						float s22 =  (mptr[2] * mptr[2] + mptr[6] * mptr[6] + mptr[10] * mptr[10]) * det;
						float s33 = -(mptr[3] * mptr[3] + mptr[7] * mptr[7] + mptr[11] * mptr[11] + 1);
						float s03 = -(mptr[0] * mptr[3] + mptr[4] * mptr[7] + mptr[ 8] * mptr[11]);
						float s13 = -(mptr[1] * mptr[3] + mptr[5] * mptr[7] + mptr[ 9] * mptr[11]);
						float s23 = -(mptr[2] * mptr[3] + mptr[6] * mptr[7] + mptr[10] * mptr[11]);

						float invs33 = 1.0f / s33;

						BMin.x = (s03 + sqrt(s03 * s03 - s33 * s00)) * invs33 + Position.x;
						BMin.y = (s13 + sqrt(s13 * s13 - s33 * s11)) * invs33 + Position.y;
						BMin.z = (s23 + sqrt(s23 * s23 - s33 * s22)) * invs33 + Position.z;

						BMax.x = (s03 - sqrt(s03 * s03 - s33 * s00)) * invs33 + Position.x;
						BMax.y = (s13 - sqrt(s13 * s13 - s33 * s11)) * invs33 + Position.y;
						BMax.z = (s23 - sqrt(s23 * s23 - s33 * s22)) * invs33 + Position.z;
					
						// compute direction vectors

						float denum;

						Right.x = mptr[0];
						Right.y = mptr[1];
						Right.z = mptr[2];
						denum = 1.0f / sqrtf(Right.x * Right.x + Right.y * Right.y + Right.z * Right.z);
						Right.x *= denum;
						Right.y *= denum;
						Right.z *= denum;

						Up.x = mptr[4];
						Up.y = mptr[5];
						Up.z = mptr[6];
						denum = 1.0f / sqrtf(Up.x * Up.x + Up.y * Up.y + Up.z * Up.z);
						Up.x *= denum;
						Up.y *= denum;
						Up.z *= denum;

						Forward.x = mptr[8];
						Forward.y = mptr[9];
						Forward.z = mptr[10];
						denum = 1.0f / sqrtf(Forward.x * Forward.x + Forward.y * Forward.y + Forward.z * Forward.z);
						Forward.x *= denum;
						Forward.y *= denum;
						Forward.z *= denum;

					}

					// ----------------------------------------------
					// ctor / dtor

					EllipsoidCollider()
					{
						Radii	 = glm::vec3(1, 1, 1);
						Right    = glm::vec3(1, 0, 0);
						Up       = glm::vec3(0, 1, 0);
						Forward  = glm::vec3(0, 0, 1);
						Position = glm::vec3(0, 0, 0);
						BMin	 = glm::vec3(0, 0, 0);
						BMax	 = glm::vec3(0, 0, 0);
						RestitutionFactor=1.0f;
						RelativePosition = glm::vec3(0, 0, 0);
						Matrix   = glm::mat4(1, 0, 0, 0,
											 0, 1, 0, 0,
											 0, 0, 1, 0,
											 0, 0, 0, 1);
					}

					EllipsoidCollider(const glm::vec3& radii,const glm::vec3 &relpos,const float restitutionfactor=1.0f)
					{
						Radii    = radii;
						Right    = glm::vec3(1, 0, 1);
						Up       = glm::vec3(0, 1, 0);
						Forward  = glm::vec3(0, 0, 1);
						Position = glm::vec3(0, 0, 0);
						BMin	 = glm::vec3(0, 0, 0);
						BMax	 = glm::vec3(0, 0, 0);
						RestitutionFactor = restitutionfactor;
						RelativePosition = relpos;
						Matrix   = glm::mat4(1, 0, 0, 0,
											 0, 1, 0, 0,
											 0, 0, 1, 0,
											 0, 0, 0, 1);
					}

					~EllipsoidCollider()
					{
					}

			};

		}	//end of namespace collisions

	}	// end of geo3d namespace

}	// end of vml namespace

