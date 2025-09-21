#pragma once

//////////////////////////////////////////////////////
// circles

namespace vml
{
	namespace geo2d
	{

		// -------------------------------------------------------------------------------------------------
		// circle struct

		struct Circle
		{
			glm::vec2 P;
			float R;
			Circle() : P(glm::vec2(0, 0)), R(0) {}
			Circle(const glm::vec2& p, const float radius) : P(glm::vec2(p)), R(radius) {}
			Circle(const float px, const float py, const float radius) : P(px, py), R(radius) {}
		};

	} // end of geo2d namespace

} // end of vml namepsace


/*
		// -------------------------------------------------------------------------------------------------
		// Given point p, return the point q on or in AABB b that is closest to p
		// For each coordinate axis, if the point coordinate value is
		// outside box, clamp it to the box, else keep it as is

		 bool CircleVsBoxCollision(SDL_Renderer* rend,Collider *circle, Collider *box)
		 {

			 glm::vec2 p0 = box->Position;
			 glm::vec2 c0 = circle->Position;
			 glm::vec2 q0 = c0 - p0;

			 float* m = box->M;

			 // circle position transformed in box
			 // coordinate system and orientation

			 glm::vec2 q( q0.x * m[0] + q0.y * m[1] + p0.x,
						  q0.x * m[2] + q0.y * m[3] + p0.y );

			 // poit is in box local system , which now is an
			 // aabb , so we can perform a simpler test

			 float w1 = p0.x - box->Width  * 0.5f;
			 float w2 = p0.x + box->Width  * 0.5f;
			 float h1 = p0.y - box->Height * 0.5f;
			 float h2 = p0.y + box->Height * 0.5f;

			 if (q.x < w1) q.x = w1;
			 if (q.x > w2) q.x = w2;
			 if (q.y < h1) q.y = h1;
			 if (q.y > h2) q.y = h2;

			 // unmap point into worl dspace

			 q0 = q - p0;

			 glm::vec2 t( q0.x * m[0] + q0.y * m[2] + p0.x,
						  q0.x * m[1] + q0.y * m[3] + p0.y );

			 glm::vec2 n = glm::normalize(t-c0);
			 glm::vec2 s = c0 + circle->Radius * n;

			 DrawLine(rend, t, circle->Position, 255, 255, 255);
			 DrawPoint(rend, t, 255, 0, 0);
			 DrawPoint(rend, s, 0, 255, 0);

			return true;

		 }


*/