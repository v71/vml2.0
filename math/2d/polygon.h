#pragma once

////////////////////////////////////////////////////////////////////////////////////
// enums for polygon2d class 

#include <map>

namespace vml
{

		///////////////////////////////////////////////////////////////////////////////////////
		// 2d geometric utilities

		namespace geo2d
		{

				////////////////////////////////////////////////////////////////////////////////////////
				// 2d polygon class
			
				class Polygon
				{
					
					private:

						unsigned int			   Flags;					// polygon flags
						float				  	   Length;					// polygon lenght
						float					   Area;					// polygon area
						float					   Width;					// polygon width
						float					   Height;					// polygon height
						float					   Radius;					// radius
						glm::vec2				   BoundingBoxMin;			// bouding box min coordinate
						glm::vec2				   BoundingBoxMax;			// bouding box max coordinate
						glm::vec2				   LeftExtremalPoint;		// left extremal point
						glm::vec2				   RightExtremalPoint;		// right extremal point
						glm::vec2				   UpperExtremalPoint;		// upper extremal point
						glm::vec2				   LowerExtremalPoint;		// lower extremal point
						glm::vec2				   Center;					// polygon center
						std::vector<glm::vec2>	   Normals;					// points holding polygon normals
						std::vector<glm::vec2>	   Points;					// points holding polygon coordinates
						
						// ---------------------------------------------------------------------
						// compute bounding box

						void ComputeBoundingBox()
						{
							BoundingBoxMin = glm::vec2( FLT_MAX,  FLT_MAX);
							BoundingBoxMax = glm::vec2(-FLT_MAX, -FLT_MAX);

							for (size_t i = 0; i < Points.size(); ++i)
							{
								if (Points[i].x < BoundingBoxMin.x) BoundingBoxMin.x = Points[i].x;
								if (Points[i].y < BoundingBoxMin.y) BoundingBoxMin.y = Points[i].y;
								if (Points[i].x > BoundingBoxMax.x) BoundingBoxMax.x = Points[i].x;
								if (Points[i].y > BoundingBoxMax.y) BoundingBoxMax.y = Points[i].y;
							}
							
							// get bounding box extensions

							Width  = BoundingBoxMax.x - BoundingBoxMin.x;
							Height = BoundingBoxMax.y - BoundingBoxMin.y;
						}

						// ---------------------------------------------------------------------
						// compute radius

						void ComputeRadius()
						{
							Radius=-FLT_MAX;

							for ( size_t i=0; i<Points.size(); ++i )
							{
								float d=Points[i].x*Points[i].x+Points[i].y*Points[i].y;
								if ( d>=Radius ) Radius=d;	
							}

							Radius=sqrtf( Radius);
						}
				
						// ---------------------------------------------------------------------
						// compute center

						void ComputeCenter()
						{
							Center = glm::vec2(0, 0);
							for (size_t i = 0; i < Points.size(); ++i)
								Center += Points[i];
							Center /= (float)(Points.size());
						}

						// ---------------------------------------------------------------------
						// compute polygon extremal points

						void ComputeExtremalPoints()
						{
							size_t minindex;

							// leftmost extremal point

							minindex = 0;
							for (size_t i = 0; i < Points.size(); ++i)
								if (Points[minindex].x > Points[i].x) minindex = i;
							LeftExtremalPoint = Points[minindex];

							// rightmost extremal point

							minindex = 0;
							for (size_t i = 0; i < Points.size(); ++i)
								if (Points[minindex].x < Points[i].x) minindex = i;
							RightExtremalPoint = Points[minindex];

							// topmost extremal point

							minindex = 0;
							for (size_t i = 0; i < Points.size(); ++i)
								if (Points[minindex].y < Points[i].y) minindex = i;
							UpperExtremalPoint = Points[minindex];

							// lowermost extremal point

							minindex = 0;
							for (size_t i = 0; i < Points.size(); ++i)
								if (Points[minindex].y > Points[i].y) minindex = i;
							LowerExtremalPoint = Points[minindex];
						}
	
						// ---------------------------------------------------------------------
						// inverts orientation

						void Invert()
						{

							// invert polygon vertices

							std::vector<glm::vec2> InvPoints;

							size_t n = Points.size();

							for (size_t i = 0; i < n; ++i)
								InvPoints.push_back(Points[n - i - 1]);

							Points = InvPoints;

							// toggle clockwise or counterclockwise bit

							vml::bits32::Toggle(Flags, CLOCKWISE);

						}

				public:

						// ---------------------------------------------------------------------
						// compute length

						bool ComputeLength(float eps=vml::math::EPSILON)
						{
							Length = 0.0f;

							size_t n = Points.size();

							for (size_t i = 0; i < n; ++i)
							{
								Length += glm::distance(Points[(i + 1) % n], Points[i]);
							}

							// if lenght is non zero, then set bit and return true 

							if (Length > 0)
							{
								vml::bits32::SetToFalse(Flags, ZEROLENGHT);
								return false;
							}

							// if lenght is zero we have a bad input , set bit and exit

							if (-eps < Length && Length < eps)
								vml::bits32::SetToTrue(Flags, ZEROLENGHT);
						
							return true;
						}
						
						// ---------------------------------------------------------------------
						// compute area

						bool ComputeArea(float eps = vml::math::EPSILON)
						{
							glm::vec2 p,q;

							Area = 0.0f;

							size_t n = Points.size();

							for (size_t i = 0; i < n; ++i)
							{
								p = Points[i];
								q = Points[(i + 1) % n];
								Area += p.x * q.y - q.x * p.y;
							}

							Area *= 0.5f;

							// since y coordinates increase themselves going down, i consider positive area
							// to be clockwise, in a geometric sense, actually its the contrary

							if (Area > 0)
							{
								vml::bits32::SetToTrue(Flags, CLOCKWISE);
								return true;
							}

							if (Area < 0)
							{
								vml::bits32::SetToFalse(Flags, CLOCKWISE);
								return false;
							}

							// if area is zero we have a bad input

							if (-eps < Area && Area < eps)
								vml::bits32::SetToTrue(Flags, ZEROAREA);

							// else get area absolute value

							Area = fabs(Area);
						
							return false;
						}
						
						// ---------------------------------------------------------------------
						// compute normals

						void ComputeNormals()
						{
							size_t n = Points.size();
							Normals.clear();
							for (size_t i = 0; i<n; ++i)
							{
								size_t j = (i + 1) % n;
								glm::vec2 ns = glm::normalize(Points[j]- Points[i]);
								Normals.emplace_back(glm::vec2(ns.y,-ns.x));
							}
						}
						
						// ---------------------------------------------------------------------
						// polygon metrics computations bitflags

						static unsigned int const CLOCKWISE = vml::bits32::BIT0;					// if polygon is clockwise the bit is set to 1 , else is set to 0

						// ---------------------------------------------------------------------
						// polygon topology computations bitflags  

						static unsigned int const CONVEX = vml::bits32::BIT2;						// if the polygon is convex the bit si set to 1 else is set to 0
						static unsigned int const SIMPLE = vml::bits32::BIT3;						// if polygon is simple( non self intersecting ) the bit si set to 1 else is set to 0

						// ---------------------------------------------------------------------
						// polygon errors bitflags, normally this signals a bad input from the user

						static unsigned int const ZEROAREA   = vml::bits32::BIT15;					// polygon has zero area
						static unsigned int const ZEROLENGHT = vml::bits32::BIT16;					// polygon has zero length
						static unsigned int const DUPLICATES = vml::bits32::BIT17;					// polygon has duplicated points

						// ---------------------------------------------------------------------
						// polygon metrics computations bitflags

						static unsigned int const CENTER = vml::bits32::BIT3;						// flag for recentering polygon
						
						// ---------------------------------------------------------------------
						//inits the polygon 

						void Begin()
						{
							Clear();
						}

						// ---------------------------------------------------------------------
						// adds a segment to this polygon if the ppolygon has been initialized
						// with a zero array length , see begin function for details

						void AddVertex(const glm::vec2 &p)
						{
							Points.emplace_back(p);
						}

						// ---------------------------------------------------------------------
						// adds a segment to this polygon if the ppolygon has been initialized
						// with a zero array length , see begin function for details

						void AddVertex(const std::vector<glm::vec2> &p)
						{
							Points=p;
						}
						
						// ---------------------------------------------------------------------
						// finalize polygon ,compute metrica nad topology , if happens that the 
						// polygon is not visibile you'll want to check for errors becasue, probably
						// user might have given a bad input for the polygon

						void End(unsigned int preprocessflags=0 )
						{
							// validate polygon

							if (Points.size() < 3)
							{
								Clear();
								return;	
							}

							// compute polygon metrics and topology

							ComputeCenter();					// compute center
							
							// if center flag is selected , then centre object at 
							// local coordinates, so the polygon is at 0,0

							if (vml::bits32::Get(preprocessflags, CENTER))
							{
								for (size_t i = 0; i < Points.size(); ++i)
									Points[i] -= Center;
							}

							ComputeRadius();					// compute radius
							ComputeBoundingBox();				// compute enclosing bounding box
							ComputeLength();					// compute length
							ComputeArea();						// compute area
							ComputeExtremalPoints();			// compute extremal points 
							ComputeNormals();					// compute normals

							// if we find that the polygon is counterclockwise, we 
							// force the polygon to be walked in a clockwise direction
							
							if (IsCounterClockWise())
							{
								Invert();
								//	vml::CMessage::Trace(L"inverting\n");
							}
							
						}
										
						// ---------------------------------------------------------------------
						// check for bad input

						const std::string CheckErrors() const
						{
							std::string errorstring = "Ok";
							if (IsNotSimple())   errorstring = "Polygon is self intersecting";
							if (HasZeroArea())   errorstring = "Polygon has zero area";
							if (HasZeroLength()) errorstring = "Polygon has zero length";
							if (IsHillFormed())  errorstring = "Polygon has less than 3 vertices";
							if (HasDuplicates()) errorstring = "Polygon has duplicated vertices";
							return errorstring;
						}
						
						// ---------------------------------------------------------------------
						// resets the polygon structure

						void Clear()
						{
							Points.clear();
							Normals.clear();
							BoundingBoxMin     = glm::vec2(0, 0);
							BoundingBoxMax     = glm::vec2(0, 0);
							LeftExtremalPoint  = glm::vec2(0, 0);
							RightExtremalPoint = glm::vec2(0, 0);
							UpperExtremalPoint = glm::vec2(0, 0);
							LowerExtremalPoint = glm::vec2(0, 0);
							Center             = glm::vec2(0, 0);
							Length             = 0.0f;
							Area               = 0.0f;
							Flags              = 0;
							Width              = 0.0f;
							Height             = 0.0f;
						}
						
						// ---------------------------------------------------------------------
						// getting functions

						size_t						  GetVertexCount()            const { return Points.size(); }
						size_t						  GetSegmentsCount()          const { return Points.size(); }
						unsigned int				  GetFlags()                  const { return Flags; }
						const glm::vec2              &GetVertex(size_t pos)       const { return Points[pos]; }
						const glm::vec2              &GetNormal(size_t pos)       const { return Normals[pos]; }
						const glm::vec2              &GetSegmentA(size_t pos)     const { return Points[pos]; }
						const glm::vec2              &GetSegmentB(size_t pos)     const { return Points[(pos + 1) % Points.size()]; }
						size_t						  NextSegmentOf(size_t pos)   const { return (pos + 1) % Points.size(); }
						size_t						  PrevSegmentOf(size_t pos)   const { return (pos - 1 < 0) ? Points.size() - 1 : pos - 1; }
						float						  GetLength()                 const { return Length; }
						float						  GetArea()                   const { return Area; }
						float						  GetWidth()                  const { return Width; }
						float						  GetHeight()                 const { return Height; }
						float						  GetRadius()                 const { return Radius; }
						const glm::vec2				 &GetCenter()                 const { return Center; }
						const glm::vec2				 &GetBoundingBoxMin()         const { return BoundingBoxMin; }
						const glm::vec2				 &GetBoundingBoxMax()         const { return BoundingBoxMax; }
						const glm::vec2				 &GetLeftExtremalPoint()      const { return LeftExtremalPoint; }
						const glm::vec2				 &GetRightExtremalPoint()     const { return RightExtremalPoint; }
						const glm::vec2				 &GetUpperExtremalPoint()     const { return UpperExtremalPoint; }
						const glm::vec2				 &GetLowermExtremalPoint()    const { return LowerExtremalPoint; }
						const std::vector<glm::vec2> &GetVertexArray()            const { return Points; }
						const glm::vec2 &operator[](size_t pos)                   const { return Points[pos]; }
						
						// ---------------------------------------------------------------------
						// get topology infos
						
						bool IsClockWise() const { return vml::bits32::Get(Flags, CLOCKWISE) == 1; }
						bool IsCounterClockWise() const { return vml::bits32::Get(Flags, CLOCKWISE) == 0; }
						bool IsConvex() const { return vml::bits32::Get(Flags, CONVEX) == 1; }
						bool IsConcave() const { return vml::bits32::Get(Flags, CONVEX) == 0; }
						bool IsHillFormed() const { return Points.size() < 3; }
						
						// ---------------------------------------------------------------------
						// get error infos
						
						bool IsSimple() const { return vml::bits32::Get(Flags, SIMPLE) == 1; }
						bool IsNotSimple() const { return vml::bits32::Get(Flags, SIMPLE) == 0; }
						bool HasZeroArea() const { return vml::bits32::Get(Flags, ZEROAREA) == 1; }
						bool HasZeroLength() const { return vml::bits32::Get(Flags, ZEROLENGHT) == 1; }
						bool HasDuplicates() const { return vml::bits32::Get(Flags, DUPLICATES) == 1; }
						
						// ---------------------------------------------------------------------
						//sets the polygon orientation to clockwise
						
						void SetCW()
						{
							// if polygon is already clockwise, just exit

							if (IsClockWise()) return;

							// else invert 

							Invert();
						}

						// ---------------------------------------------------------------------
						//sets the polygon orientation to clockwise

						void SetCCW()
						{
							// if polygon is already counter clockwise, just exit

							if (IsCounterClockWise()) return;

							// else invert 

							Invert();
						}
						
						// ---------------------------------------------------------------------
						// centers polygon at origin

						void Centre()
						{
							Center = glm::vec2(0, 0);
							for (size_t i = 0; i < Points.size(); ++i)
								Center += Points[i];
							Center /= (float)(Points.size());
							for (size_t i = 0; i < Points.size(); ++i)
								Points[i] -= Center;
							Center = glm::vec2(0, 0);			// center is now at zero
							ComputeBoundingBox();				// compute enclosing bounding box
							ComputeExtremalPoints();			// compute extremal points 
						}
						
						// ---------------------------------------------------------------------
						// copy constructor

						Polygon(const Polygon &polygon)
						{
							
							Points				= polygon.Points;
							Normals				= polygon.Normals;
							BoundingBoxMin		= polygon.BoundingBoxMin;
							BoundingBoxMax		= polygon.BoundingBoxMax;
							LeftExtremalPoint	= polygon.LeftExtremalPoint;
							RightExtremalPoint	= polygon.RightExtremalPoint;
							UpperExtremalPoint	= polygon.UpperExtremalPoint;
							LowerExtremalPoint	= polygon.LowerExtremalPoint;
							Center				= polygon.Center;
							Length				= polygon.Length;
							Area				= polygon.Area;
							Flags				= polygon.Flags;
							Width				= polygon.Width;
							Height				= polygon.Height;
							Radius				= polygon.Radius;
							
						}
												
						// ---------------------------------------------------------------------
						// assignment operator

						Polygon& operator=(const Polygon &polygon)
						{
							if (this == &polygon) return *this;
							Points				= polygon.Points;
							Normals				= polygon.Normals;
							BoundingBoxMin		= polygon.BoundingBoxMin;
							BoundingBoxMax		= polygon.BoundingBoxMax;
							LeftExtremalPoint	= polygon.LeftExtremalPoint;
							RightExtremalPoint	= polygon.RightExtremalPoint;
							UpperExtremalPoint	= polygon.UpperExtremalPoint;
							LowerExtremalPoint	= polygon.LowerExtremalPoint;
							Center				= polygon.Center;
							Length				= polygon.Length;
							Area				= polygon.Area;
							Flags				= polygon.Flags;
							Width				= polygon.Width;
							Height				= polygon.Height;
							Radius				= polygon.Radius;
							return *this;
						}
						
						// ---------------------------------------------------------------------
						// ctor / dtor

						Polygon()
						{
							Flags  = 0;
							Length = 0.0f;
							Area   = 0.0f;
							Width  = 0.0f;
							Height = 0.0f;
							Radius = 0.0f;
							BoundingBoxMin = glm::vec2( FLT_MAX,  FLT_MAX);
							BoundingBoxMax = glm::vec2(-FLT_MAX, -FLT_MAX);
							Center = glm::vec2(0, 0);
							LeftExtremalPoint = glm::vec2(0, 0);
							RightExtremalPoint = glm::vec2(0, 0);
							UpperExtremalPoint = glm::vec2(0, 0);
							LowerExtremalPoint = glm::vec2(0, 0);
							
						}

						Polygon(const std::vector<glm::vec2> &p,unsigned int flags=0)
						{
							
							Clear();
							Points = p;
							End(flags);
							
						}

						~Polygon()
						{
						}

				};

	} // end of namespace geo2d

} // end of namespace vml


////////////////////////////////////////////////////////////////////////////////////
// polygon2d utilities

namespace vml
{

	namespace geo2d
	{
		namespace polygons
		{

			/////////////////////////////////////////////////////////////////
			// sanitize polygon, eliminates consequents segmetns
			// given a tolerance angle
			// 10 % of angle difference as default paramenter

			static vml::geo2d::Polygon Sanitize(const vml::geo2d::Polygon& polygon, float eps = 0.01f)
			{
				// set epsilon in radians

				eps *= vml::math::PI / 180.0f;

				size_t n = int(polygon.GetSegmentsCount());

				int cur = 0;
				int next = 1;

				vml::geo2d::Polygon newpolygon;

				newpolygon.Begin();

				// get first and last segments slope

				glm::vec2 curdiff = polygon.GetVertex(0) - polygon.GetVertex(1);
				glm::vec2 nextdiff = polygon.GetVertex(n - 1) - polygon.GetVertex(0);

				// compute slope difference

				float delta = vml::math::angles::Atan2(curdiff.y, curdiff.x, vml::math::EPSILON) -
					vml::math::angles::Atan2(nextdiff.y, nextdiff.x, vml::math::EPSILON);

				// add if value is in delta range

				if (delta <= -eps || delta >= eps)
					newpolygon.AddVertex(polygon.GetVertex(0));

				// iterate for all other segmetns

				for (size_t i = 0; i < n - 1; ++i)
				{

					// get current and next segments slope

					curdiff = polygon.GetVertex(cur) - polygon.GetVertex(((size_t)cur + 1) % n);
					nextdiff = polygon.GetVertex(next) - polygon.GetVertex(((size_t)next + 1) % n);

					// compute slope difference

					delta = vml::math::angles::Atan2(curdiff.y, curdiff.x, vml::math::EPSILON) -
							vml::math::angles::Atan2(nextdiff.y, nextdiff.x, vml::math::EPSILON);

					// add if value is in delta range

					if (delta <= -eps || delta >= eps)
						newpolygon.AddVertex(polygon.GetVertex(next));

					// get next segments

					cur++;
					next = cur + 1;

				}

				// finalize polygon

				newpolygon.End();

				// return newly createdpolygon

				return newpolygon;
			}

			//--------------------------------------------------------------------------
			// transform polygon , rotation, scaling and translation

			static vml::geo2d::Polygon Transform(const vml::geo2d::Polygon& source, const float angle, const glm::vec2& scale, const glm::vec2& pos)
			{
				vml::geo2d::Polygon dest;
				glm::vec2 c = source.GetCenter();
				float rad = angle * vml::math::DEGTORAD;
				float sinx = sin(rad);
				float cosx = cos(rad);
				float a11 = scale.x * cosx;
				float a12 = scale.x * sinx;
				float a21 = scale.y * sinx;
				float a22 = scale.y * cosx;
				float tx = c.x - c.x * a11 + c.y * a21 + pos.x;
				float ty = c.y - c.y * a22 - c.x * a12 + pos.y;
				dest.Begin();
				for (size_t i = 0; i < source.GetVertexCount(); ++i)
				{
					float px = source.GetVertex(int(i)).x;
					float py = source.GetVertex(int(i)).y;
					dest.AddVertex(glm::vec2(px * a11 - py * a21 + tx, px * a12 + py * a22 + ty));
				}
				dest.End();
				return dest;
			}

			// ---------------------------------------------------------------------
			// determine if polygon is convex or concave

			bool CheckForConvexity(const vml::geo2d::Polygon& poly)
			{
				size_t n = poly.GetVertexCount();

				if (n < 4)
					return true;

				bool sign = false;

				for (size_t i = 0; i < n; i++)
				{
					float dx1 = poly.GetVertex((i + 2) % n).x - poly.GetVertex((i + 1) % n).x;
					float dy1 = poly.GetVertex((i + 2) % n).y - poly.GetVertex((i + 1) % n).y;
					float dx2 = poly.GetVertex((i)).x - poly.GetVertex((i + 1) % n).x;
					float dy2 = poly.GetVertex((i)).y - poly.GetVertex((i + 1) % n).y;
					float zcrossproduct = dx1 * dy2 - dy1 * dx2;

					if (i == 0)
					{
						sign = zcrossproduct > 0;
					}
					else
					{
						if (sign != (zcrossproduct > 0))
							return false;
					}
				}

				return true;
			}

			// ---------------------------------------------------------------------
			// determine if polygon has duplicated vertices

			bool CheckForDuplicatedVertices(const vml::geo2d::Polygon& poly)
			{
				// structure for multimap insertion

				struct vec2cmp
				{
					bool operator()(const glm::vec2& lhs, const glm::vec2& rhs) const
					{
						return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
					}
				};

				// declare a multimap

				std::multimap< glm::vec2, size_t, vec2cmp > Map;

				// define types for multimap

				typedef std::multimap<glm::vec2, size_t, vec2cmp> CollectionType;
				typedef std::pair<CollectionType::iterator, CollectionType::iterator> RangeType;

				// map each point to a 2d hash index

				for (size_t i = 0; i < poly.GetVertexCount(); ++i)
					Map.insert(std::pair<glm::vec2, size_t>(poly.GetVertex(i), i));

				// if for every point the count is greater than 1 , then that
				// point is duplicated , thus we set the bit flag and exit

				for (size_t i = 0; i < poly.GetVertexCount(); ++i)
					if (Map.count(poly.GetVertex(i)) > 1)
						return true;

				return false;
			}

			//--------------------------------------------------------------------------
			// mirror polygon respect x axis

			static vml::geo2d::Polygon MirrorX(const vml::geo2d::Polygon& poly)
			{
				vml::geo2d::Polygon dest;
				const glm::vec2 c = poly.GetCenter();
				dest.Begin();
				for (size_t i = 0; i < poly.GetVertexCount(); ++i)
				{
					float px = -poly.GetVertex(int(i)).x + c.x + c.x;
					float py = poly.GetVertex(int(i)).y;
					dest.AddVertex(glm::vec2(px, py));
				}
				dest.End();
				return dest;
			}

			//--------------------------------------------------------------------------
			// mirror polygon respect y axis

			static vml::geo2d::Polygon MirrorY(const vml::geo2d::Polygon& poly)
			{
				vml::geo2d::Polygon dest;
				glm::vec2 c = poly.GetCenter();
				dest.Begin();
				for (size_t i = 0; i < poly.GetVertexCount(); ++i)
				{
					float px = poly.GetVertex(int(i)).x;
					float py = -poly.GetVertex(int(i)).y + c.y + c.y;
					dest.AddVertex(glm::vec2(px, py));
				}
				dest.End();
				return dest;
			}

			//--------------------------------------------------------------------------
			// create a cube polygon

			static vml::geo2d::Polygon CreateRectangle()
			{
				vml::geo2d::Polygon dest;
				dest.AddVertex(glm::vec2(-1,  1));
				dest.AddVertex(glm::vec2( 1,  1));
				dest.AddVertex(glm::vec2( 1, -1));
				dest.AddVertex(glm::vec2(-1, -1));
				dest.End();
				return dest;
			}

			//--------------------------------------------------------------------------
			// create a trianlge polygon

			static vml::geo2d::Polygon CreateTriangle()
			{
				vml::geo2d::Polygon dest;
				dest.AddVertex(glm::vec2(-1,  1));
				dest.AddVertex(glm::vec2( 1,  1));
				dest.AddVertex(glm::vec2( 1, -1));
				dest.End();
				return dest;
			}

			//--------------------------------------------------------------------------
			// create an hexagon polygon

			static vml::geo2d::Polygon CreateCircle(int n)
			{
				vml::geo2d::Polygon dest;
				dest.Begin();
				for (int i = 0; i < n; i++)
				{
					float a = 2.0f * vml::math::PI * (i / (float)n);
					dest.AddVertex(glm::vec2(cos(a), sin(a)));
				}
				dest.End();
				return dest;
			}
		
		} // end of namespace polygons

	} // end of namespace geo2d

} // end of namespace vml
