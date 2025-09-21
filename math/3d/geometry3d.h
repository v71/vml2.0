#pragma once

namespace vml
{
	namespace geo3d
	{

		struct Results
		{
			// bit flags

			static const unsigned int DOES_INTERSECT	  = vml::bits32::BIT0;
			static const unsigned int DOES_NOT_INTERSECT  = vml::bits32::BIT1;
			static const unsigned int COLLINEAR			  = vml::bits32::BIT2;
			static const unsigned int PARALLEL			  = vml::bits32::BIT3;
			static const unsigned int INSIDE			  = vml::bits32::BIT4;
			static const unsigned int OUTSIDE			  = vml::bits32::BIT5;
			static const unsigned int SECOND_INSIDE_FIRST = vml::bits32::BIT6;
			static const unsigned int FIRST_INSIDE_SECOND = vml::bits32::BIT7;
		
			// get bit desciptors

			static const std::string ToString(const unsigned int bitvalue)
			{
				switch (bitvalue)
				{

					case vml::bits32::BIT0 : return "Do Not Intersect";		break;
					case vml::bits32::BIT1 : return "Do Intersect";			break;
					case vml::bits32::BIT2 : return "Collinear";			break;
					case vml::bits32::BIT3 : return "Parallel";			    break;
					case vml::bits32::BIT4 : return "Inside";				break;
					case vml::bits32::BIT5 : return "Outside";				break;
					case vml::bits32::BIT6 : return "Second Inside First";  break;
					case vml::bits32::BIT7 : return "First Inside Second";  break;
					case vml::bits32::BIT8 : return "Nodesc08";			    break;
					case vml::bits32::BIT9 : return "NoDesc09";		        break;
					case vml::bits32::BIT10: return "NoDesc10";			    break;
					case vml::bits32::BIT11: return "NoDesc11";				break;
					case vml::bits32::BIT12: return "NoDesc12";				break;
					case vml::bits32::BIT13: return "NoDesc13";			    break;
					case vml::bits32::BIT14: return "NoDesc14";		        break;
					case vml::bits32::BIT15: return "NoDesc15";				break;
					case vml::bits32::BIT16: return "NoDesc16";				break;
					case vml::bits32::BIT17: return "NoDesc17";				break;
					case vml::bits32::BIT18: return "NoDesc18";				break;
					case vml::bits32::BIT19: return "NoDesc19";				break;
					case vml::bits32::BIT20: return "NoDesc20";				break;
					case vml::bits32::BIT21: return "NoDesc21";		        break;
					case vml::bits32::BIT22: return "NoDesc22";		        break;
					case vml::bits32::BIT23: return "NoDesc23";		        break;
					case vml::bits32::BIT24: return "NoDesc24";			    break;
					case vml::bits32::BIT25: return "NoDesc25";			    break;
					case vml::bits32::BIT26: return "NoDesc26";			    break;
					case vml::bits32::BIT27: return "NoDesc27";			    break;
					case vml::bits32::BIT28: return "NoDesc28";			    break;
					case vml::bits32::BIT29: return "NoDesc29";			    break;
					case vml::bits32::BIT30: return "NoDesc30";			    break;
					case vml::bits32::BIT31: return "NoDesc31";			    break;

				}

				return "Undefined BitValue";
			}

		};

	} // end of geo2d namespace

} // end of vml namepsace

///////////////////////////////////////////////////////////////////////////////////////
// 3d geometry predicsates

#include <vml2.0\math\3d\points.h>
#include <vml2.0\math\3d\lines.h>
#include <vml2.0\math\3d\triangles.h>
#include <vml2.0\math\3d\aabbox.h>
#include <vml2.0\math\3d\sphere.h>
#include <vml2.0/math/3d/plane.h>

///////////////////////////////////////////////////////////////////////////////////////
// 3d geometry algorithms

#include <vml2.0\math\3d\metrics.h>
#include <vml2.0\math\3d\aaboxintersections.h>
#include <vml2.0\math\3d\distances.h>
#include <vml2.0\math\3d\trianglesintersections.h>
#include <vml2.0\math\3d\triangledistances.h>
#include <vml2.0\math\3d\triangleclipper.h>
#include <vml2.0\math\3d\sphereintersections.h>
#include <vml2.0\math\3d\lineintersections.h>
#include <vml2.0/math/3d/collisions.h>
#include <vml2.0/math/3d/trianglecollisions.h>
#include <vml2.0/math/3d/spherecollisions.h>



