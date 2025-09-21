#pragma once


  /////////////////////////////////////////////////////////////////////////////////////////////////////
  // convex polyogn clipper

namespace vml
{
	namespace geo2d
	{

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// falsg for the convex polygon ray clipper

		static const unsigned int   POLYGONCLIPPERFLAG_GREATER			= 0;
		static const unsigned int	POLYGONCLIPPERFLAG_GREATER_OR_EQUAL = 1;
		static const unsigned int	POLYGONCLIPPERFLAG_LESSER			= 2;
		static const unsigned int	POLYGONCLIPPERFLAG_LESSER_OR_EQUAL	= 3;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// convex polygon clipper against ray

		static vml::geo2d::Polygon ConvexPolygonClipper(const vml::geo2d::Polygon &source, const glm::vec2& a, const glm::vec2& b, const glm::vec2 &N, const int comp= POLYGONCLIPPERFLAG_GREATER)
		{

			float c1, c2;
			float ua, ub;
			float numa, numb;
			float invdenom, denom;
			float x1, y1, x2, y2;
			size_t i, j, pts;
			int flag;

			vml::geo2d::Polygon clipped;

			const std::vector<glm::vec2> &sVertex = source.GetVertexArray();

			pts = source.GetVertexCount();

			clipped.Begin();

			for (i = 0; i<pts; ++i)
			{

				j = (i + 1) % pts;

				x1 = sVertex[i].x;
				y1 = sVertex[i].y;
				x2 = sVertex[j].x;
				y2 = sVertex[j].y;

				c1 = (x1 - a[0])*N[0] + (y1 - a[1])*N[1];
				c2 = (x2 - a[0])*N[0] + (y2 - a[1])*N[1];

				flag = 0;

				switch (comp)
				{
					case POLYGONCLIPPERFLAG_GREATER:			flag = (c1 >  0) << 0 | (c2 >  0) << 1;	break;
					case POLYGONCLIPPERFLAG_GREATER_OR_EQUAL:	flag = (c1 >= 0) << 0 | (c2 >= 0) << 1; break;
					case POLYGONCLIPPERFLAG_LESSER:				flag = (c1 <  0) << 0 | (c2 <  0) << 1;	break;
					case POLYGONCLIPPERFLAG_LESSER_OR_EQUAL:	flag = (c1 <= 0) << 0 | (c2 <= 0) << 1; break;
				}

				// line is out

			//	if (flag == 0)
			//	{
			//	}

				// line is intersecting coming in

				if (flag == 1)
				{

					float pi[2] = { 0,0 };

					denom = ((a[1] - b[1])*(x2 - x1)) - ((a[0] - b[0])*(y2 - y1));
					numa = ((a[0] - b[0])*(y1 - b[1])) - ((a[1] - b[1])*(x1 - b[0]));
					numb = ((x2 - x1)*(y1 - b[1])) - ((y2 - y1)*(x1 - b[0]));

					invdenom = 1.0f / denom;
					ua = numa * invdenom;
					ub = numb * invdenom;

					// Get the intersection point.

					pi[0] = x1 + ua * (x2 - x1);
					pi[1] = y1 + ua * (y2 - y1);

					clipped.AddVertex(sVertex[i]);

					clipped.AddVertex(glm::vec2(pi[0], pi[1]));

				}

				// line is intersecting coming out

				if (flag == 2)
				{

					float pi[2] = { 0,0 };

					denom = ((a[1] - b[1])*(x2 - x1)) - ((a[0] - b[0])*(y2 - y1));
					numa = ((a[0] - b[0])*(y1 - b[1])) - ((a[1] - b[1])*(x1 - b[0]));
					numb = ((x2 - x1)*(y1 - b[1])) - ((y2 - y1)*(x1 - b[0]));

					invdenom = 1.0f / denom;
					ua = numa * invdenom;
					ub = numb * invdenom;

					// Get the intersection point.

					pi[0] = x1 + ua * (x2 - x1);
					pi[1] = y1 + ua * (y2 - y1);

					clipped.AddVertex(glm::vec2(pi[0], pi[1]));

				}

				// line is in

				if (flag == 3)
				{
					clipped.AddVertex(sVertex[i]);
				}

			}

			clipped.End();

			return clipped;

		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// convex polygon clipper against convex polygon
		// note that this funcion works only if poylgon source and target are both convex
		// else you will get unwnated results, see the function isconvex in the polygon class the check for polygon convexity

		static vml::geo2d::Polygon ConvexPolygonClipper(const vml::geo2d::Polygon &pt, const vml::geo2d::Polygon &pb)
		{
			vml::geo2d::Polygon clipped = pt;

			size_t pts = pb.GetVertexCount();

			for (size_t i = 0; i < pts; ++i)
			{
				size_t j = (i + 1) % pts;

				clipped = vml::geo2d::ConvexPolygonClipper(clipped, pb.GetVertex(i), pb.GetVertex(j), pb.GetNormal(i), vml::geo2d::POLYGONCLIPPERFLAG_LESSER_OR_EQUAL);
			}

			return clipped;
		}
				
		/////////////////////////////////////////////////////////////////////////////
		// intersection between bounding boxes flags

		static const unsigned int POLYGON_DISJOINT			  = 0;
		static const unsigned int POLYGON_FIRST_INSIDE_SECOND = 1;
		static const unsigned int POLYGON_SECOND_INSIDE_FIRST = 2;
		static const unsigned int POLYGON_INTERSECTED		  = 3;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// convex polygon clipper against polygon return also the inclusion flags
		// note that this funcion works only if poylgon source and target are both convex
		// else you will get unwnated results, see the function isconvex in the polygon class the check for polygon convexity

		static unsigned int ConvexPolygonClipper(const vml::geo2d::Polygon &pa, const vml::geo2d::Polygon &pb, vml::geo2d::Polygon &clipped,const float &eps=vml::math::EPSILON)
		{
			unsigned int result = vml::geo2d::intersections::AABBoxVsAABBox(pa.GetBoundingBoxMin(), pa.GetBoundingBoxMax(),
																		    pb.GetBoundingBoxMin(), pb.GetBoundingBoxMax());

			if (result == vml::geo2d::Results::DOES_INTERSECT)
			{
				clipped.Clear();
				return POLYGON_DISJOINT;
			}

			clipped = pa;

			size_t pts = pb.GetVertexCount();

			for (size_t i = 0; i < pts; ++i)
			{
				size_t j = (i + 1) % pts;

				clipped = vml::geo2d::ConvexPolygonClipper(clipped, pb.GetVertex(i), pb.GetVertex(j), pb.GetNormal(i), vml::geo2d::POLYGONCLIPPERFLAG_LESSER_OR_EQUAL);
			}

			if (clipped.GetArea() == 0.0f)
			{
				clipped.Clear();
				return POLYGON_DISJOINT;
			}

			if (fabs(pa.GetArea() - clipped.GetArea() <eps))
				return  POLYGON_FIRST_INSIDE_SECOND;

			if (fabs(pb.GetArea() - clipped.GetArea() <eps))
				return POLYGON_SECOND_INSIDE_FIRST;

			return POLYGON_INTERSECTED;
		}
		
	} // end of geo2d

} // end of namespace
