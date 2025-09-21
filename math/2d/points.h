#pragma once

////////////////////////////////////////////////////////////////////////////////////
// point class

namespace vml
{
	namespace geo2d
	{
		// -------------------------------------------------------------------------------------------------
		// const predicates

		const glm::vec2 NullVec  = glm::vec2( 0,  0);
		const glm::vec2 UnaryVec = glm::vec2( 1,  1);
		const glm::vec2 UpVec    = glm::vec2( 0,  1);
		const glm::vec2 DownVec  = glm::vec2( 0, -1);
		const glm::vec2 LeftVec  = glm::vec2( 1,  0);
		const glm::vec2 RightVec = glm::vec2(-1,  0);

	}	// end of geo3d namespace

}	// end of vml namespace
