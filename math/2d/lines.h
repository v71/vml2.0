#pragma once

//////////////////////////////////////////////////////
// line 2d class

namespace vml
{
	namespace geo2d
	{

		// -------------------------------------------------------------------------------------------------
		// line struct

		struct Line
		{
			glm::vec2 P;
			glm::vec2 Q;
			Line() : P(glm::vec2(0, 0)), Q(glm::vec2(0, 0)) {}
			Line(const glm::vec2& p, const glm::vec2& q) : P(p), Q(q) {}
			Line(const float px, const float py, const float qx, const float qy) : P(px, py), Q(qx, qy) {}
		};

		// -------------------------------------------------------------------------------------------------
		// const predicates

		const Line NullLine =  Line(0,  0,  0,  0);
		const Line UpLine    = Line(0,  0,  0,  1);
		const Line DownLine  = Line(0,  0,  0, -1);
		const Line LeftLine  = Line(0,  0,  1,  0);
		const Line RightLine = Line(0,  0, -1,  0);

	} // end of geo2d namespace

} // end of vml namepsace



