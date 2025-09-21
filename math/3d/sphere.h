#pragma once

/////////////////////////////////////////////////////////////////////////////
// flags for spheres

namespace vml
{
	namespace geo3d
	{
		namespace spheres
		{

			////////////////////////////////////////////////////////////////////////////////////
			// sphere class

			class Sphere
			{

				private:

					// ------------------------------------------------------------------
					// private data 

					glm::vec3 P0;
					float     Radius;
					float     Diameter;

				public:

					// ------------------------------------------------------------------
					// public function

					// ------------------------------------------------------------------
					// sets parameters for plane

					void Set(const glm::vec3 &p0, const float radius)
					{
						P0       = p0;
						Radius   = radius;
						Diameter = 2.0f*Radius;
					}

					// ------------------------------------------------------------------
					// getters

					const glm::vec3 &GetP0() const { return P0; }
					float GetRadius() const { return Radius; }
					float GetDiameter() const { return Diameter; }

					// ------------------------------------------------------------------
					// ctor / dtor

					Sphere()
					{
						P0       = glm::vec3(0, 0, 0);
						Radius   = 0.0f;
						Diameter = 0.0f;
					}

					Sphere(const glm::vec3 &p0, const float radius)
					{
						Set(p0, radius);
					}

					~Sphere()
					{}

			};

		} // end of spheres namespace

	}	// end of geo3d namespace

}	// end of vml namespace
