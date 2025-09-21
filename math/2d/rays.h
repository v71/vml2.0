#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ray

namespace vml
{
	namespace geo2d
	{
	
		// -------------------------------------------------------------------------------------------------
		// class handling rays in 2d space

		struct Ray
		{
			glm::vec2 P;	// origin
			glm::vec2 Dir;	// direction vector
			Ray() : P(glm::vec2(0,0)),Dir(glm::vec2(0,0)){}
			Ray(const glm::vec2 &p, const glm::vec2& dir) : P(p),Dir(dir){}
			Ray(const float px, const float py, const float dx, const float dy) : P(px, py), Dir(dx, dy) {}
		};

		// -------------------------------------------------------------------------------------------------
		// const predicates

		const Ray NullRay  = Ray(0, 0,  0,  0);
		const Ray UpRay    = Ray(0, 0,  0,  1);
		const Ray DownRay  = Ray(0, 0,  0, -1);
		const Ray LeftRay  = Ray(0, 0,  1,  0);
		const Ray RightRay = Ray(0, 0, -1,  0);

	} // end of geo2d namespace

} // end of vml namepsace

