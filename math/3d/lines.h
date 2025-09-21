#pragma once

/////////////////////////////////////////////////////////////////////////////
// flags for spheres

namespace vml
{
	namespace geo3d
	{
			class Line
			{
					glm::vec3 P1;
					glm::vec3 P2;
					glm::vec3 Dir;
					glm::vec3 DirCosines;	// direction cosines
					glm::vec3 Normal;
					glm::vec3 Slope;		// slope of line
					float     Length;		// lenght for this segment

				public:

					//---------------------------------------------------------------
					//query functions

					const glm::vec3 &GetP1()			const { return P1; }
					const glm::vec3 &GetP2()			const { return P2; }
					const glm::vec2 &GetNormal()		const { return Normal; }
					const glm::vec3 &GetDir()			const { return Dir; }
					const glm::vec3 &GetDirCosines()    const { return DirCosines; }
					const glm::vec3 &GetSlope()			const { return Slope; }
					float GetLength()					const { return Length; }

					// ------------------------------------------------------------------
					// setting 

					void Set(const glm::vec3 &p1, const glm::vec3 &p2)
					{
						P1 = p1;
						P2 = p2;
						Dir = P2 - P1;
						
						Length = glm::length(Dir);
						if (Length > -vml::math::EPSILON && Length < vml::math::EPSILON) Length = vml::math::EPSILON;
						
						float invlenght = 1.0f / Length;
						
						// compute dir cosines

						DirCosines.x = Dir.x * invlenght ;
						DirCosines.y = Dir.y * invlenght;
						DirCosines.z = Dir.z * invlenght;
						
						// compute slopes

						Slope.x = vml::math::angles::Atan2(DirCosines.x, -DirCosines.y, vml::math::EPSILON) / vml::math::PI * 180.0f + 180.0f;
						Slope.y = vml::math::angles::Atan2(DirCosines.x, -DirCosines.z, vml::math::EPSILON) / vml::math::PI * 180.0f + 180.0f;
						Slope.z = vml::math::angles::Atan2(DirCosines.y, -DirCosines.z, vml::math::EPSILON) / vml::math::PI * 180.0f + 180.0f;
						
						// compute normal

						float min = fabs(Dir.x);
						
						glm::vec3 cardinalAxis(1, 0, 0);
						
						if (fabs(Dir.y) < min)
						{
							min = fabs(Dir.y);
							cardinalAxis = glm::vec3(0, 1, 0);
						}
						if (fabs(Dir.z) < min)
						{
							cardinalAxis = glm::vec3(0, 0, 1);
						}
						
						Normal=glm::cross(Dir, cardinalAxis);
					}

					// ------------------------------------------------------------------
					// ctor / dtor

					Line(const glm::vec3 &p1, const glm::vec3 &p2)
					{
						Set(p1, p2);
					}

					Line()
					{
						P1			= glm::vec3(0, 0, 0);
						P2			= glm::vec3(0, 0, 0);
						Dir			= glm::vec3(0, 0, 0);
						Slope		= glm::vec3(0, 0, 0);
						DirCosines	= glm::vec3(0, 0, 0);
						Slope		= glm::vec3(0, 0, 0);
						Normal	    = glm::vec3(0, 0, 0);
						Length		= 0;
					}

					~Line()
					{}
			};

	}	// end of geo3d namespace

}	// end of vml namespace

