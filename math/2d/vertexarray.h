#pragma once

namespace vml
{
	namespace geo2d
	{

		/////////////////////////////////////////////////////

		class VertexArray
		{

			private:

					std::vector<glm::vec2> Points;
					glm::vec2			   BoundingBoxMin;			// bouding box min coordinate
					glm::vec2			   BoundingBoxMax;			// bouding box max coordinate
					glm::vec2			   Center;					// polygon center
					glm::vec2			   LeftExtremalPoint;		// left extremal point
					glm::vec2			   RightExtremalPoint;		// right extremal point
					glm::vec2			   UpperExtremalPoint;		// upper extremal point
					glm::vec2			   LowerExtremalPoint;		// lower extremal point
					float				   Width;					// polygon width
					float				   Height;					// polygon height
					float				   Radius;

					// -------------------------------------------------------------------------
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

					// -------------------------------------------------------------------------
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

					// -------------------------------------------------------------------------
					// compute center

					void ComputeCenter()
					{
						Center = glm::vec2(0, 0);
						for (size_t i = 0; i < Points.size(); ++i)
							Center += Points[i];
						Center /= (float)(Points.size());
					}

					// -------------------------------------------------------------------------
					// compute radius

					void ComputeRadius()
					{
						Radius = 0.0f;
						for (size_t i = 0; i < Points.size(); ++i)
						{
							float px = Points[i].x - Center.x;
							float py = Points[i].y - Center.y;
							float dist = px*px + py*py;
							if (dist > Radius) Radius = dist;
						}
						Radius = sqrtf(Radius);
					}

			public:

					// -------------------------------------------------------------------------

					void Begin()
					{
						Clear();
					}

					// -------------------------------------------------------------------------

					void Clear()
					{
						Points.clear();
						BoundingBoxMin = glm::vec2(0, 0);
						BoundingBoxMax = glm::vec2(0, 0);
						LeftExtremalPoint = glm::vec2(0, 0);
						RightExtremalPoint = glm::vec2(0, 0);
						UpperExtremalPoint = glm::vec2(0, 0);
						LowerExtremalPoint = glm::vec2(0, 0);
						Center = glm::vec2(0, 0);
						Width = 0.0f;
						Height = 0.0f;
						Radius = 0.0f;
					}

					// -------------------------------------------------------------------------

					void Add(const glm::vec2 &p)
					{
						Points.emplace_back(p);
					}

					// -------------------------------------------------------------------------

					void Add(float x, float y)
					{
						Points.emplace_back(glm::vec2(x, y));
					}

					// -------------------------------------------------------------------------
			
					void Assign(const std::vector<glm::vec2> &p)
					{
						Clear();
						for (size_t i = 0; i < p.size(); ++i)
							Points.emplace_back(p[i]);
						End();
					}
					
					// -------------------------------------------------------------------------

					void Erase(size_t pos)
					{
						Points.erase(Points.begin() + pos);
					
						// recompute metrics

						End();
					}

					// -------------------------------------------------------------------------

					void End()
					{
						ComputeCenter();					// compute center
						ComputeBoundingBox();				// compute enclosing bounding box
						ComputeRadius();					// compute radius
						ComputeExtremalPoints();			// compute extremal points 
					}

					// ------------------------------------------------------------------------------
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
						ComputeRadius();					// compute radius
						ComputeExtremalPoints();			// compute extremal points 
					}

					// -----------------------------------------------------------------------------
					// getters

					glm::vec2& operator[](size_t pos) { return Points[pos]; }
					const glm::vec2& operator[](size_t pos) const{ return Points[pos]; }
					size_t Size() const { return Points.size(); }
					float GetWidth() const { return Width; }
					float GetHeight() const { return Height; }
					float GetRadius() const { return Radius; }
					const glm::vec2 &GetCenter() const { return Center; }
					const glm::vec2 &GetBoundingBoxMin() const { return BoundingBoxMin; }
					const glm::vec2 &GetBoundingBoxMax() const { return BoundingBoxMax; }
					const glm::vec2 &GetLeftExtremalPoint() const { return LeftExtremalPoint; }
					const glm::vec2 &GetRightExtremalPoint() const { return RightExtremalPoint; }
					const glm::vec2 &GetUpperExtremalPoint() const { return UpperExtremalPoint; }
					const glm::vec2 &GetLowermExtremalPoint() const { return LowerExtremalPoint; }
					const std::vector<glm::vec2> &GetPointsArray() const { return Points; }
					std::vector<glm::vec2>& GetPointsArray() { return Points; }

					// -------------------------------------------------------------------------------

					VertexArray& operator*=(const float s)
					{
						float cx = Center.x - Center.x*s;
						float cy = Center.y - Center.y*s;
						for (size_t i = 0; i < Points.size(); ++i)
						{
							Points[i].x *= s;
							Points[i].y *= s;
							Points[i].x += cx;
							Points[i].y += cy;
						}
						End();
						return *this;
					}

					VertexArray& operator/=(const float s)
					{
						float cx = Center.x - Center.x*s;
						float cy = Center.y - Center.y*s;
						for (size_t i = 0; i < Points.size(); ++i)
						{
							Points[i].x /= s;
							Points[i].y /= s;
							Points[i].x += cx;
							Points[i].y += cy;
						}
						End();
						return *this;
					}

					VertexArray& operator+=(const float s)
					{
						for (size_t i = 0; i < Points.size(); ++i)
						{
							Points[i].x += s;
							Points[i].y += s;
						}
						End();
						return *this;
					}

					VertexArray& operator-=(const float s)
					{
						for (size_t i = 0; i < Points.size(); ++i)
						{
							Points[i].x -= s;
							Points[i].y -= s;
						}
						End();
						return *this;
					}

					// -------------------------------------------------------------------------------

					VertexArray& operator*=(const glm::vec2 &s)
					{
						float cx = Center.x - Center.x*s.x;
						float cy = Center.y - Center.y*s.y;
						for (size_t i = 0; i < Points.size(); ++i)
						{
							Points[i].x *= s.x;
							Points[i].y *= s.y;
							Points[i].x += cx;
							Points[i].y += cy;
						}
						End();
						return *this;
					}

					VertexArray& operator/=(const glm::vec2 &s)
					{
						float cx = Center.x - Center.x*s.x;
						float cy = Center.y - Center.y*s.y;
						for (size_t i = 0; i < Points.size(); ++i)
						{
							Points[i].x /= s.x;
							Points[i].y /= s.y;
							Points[i].x += cx;
							Points[i].y += cy;
						}
						End();
						return *this;
					}

					VertexArray& operator+=(const glm::vec2 &s)
					{
						for (size_t i = 0; i < Points.size(); ++i)
						{
							Points[i].x += s.x;
							Points[i].y += s.y;
						}
						End();
						return *this;
					}

					VertexArray& operator-=(const glm::vec2 &s)
					{
						for (size_t i = 0; i < Points.size(); ++i)
						{
							Points[i].x -= s.x;
							Points[i].y -= s.y;
						}
						End();
						return *this;
					}

					// -------------------------------------------------------------------------------

					bool operator== (const VertexArray& a) const
					{
						if (a.Size() != Points.size())
							vml::os::Message::Error("VertexArray : ","Sizes must match");
						for (size_t i = 0; i < Points.size(); ++i)
							if (a.Points[i] != Points[i]) return false;
						return true;
					}

					bool operator!= (const VertexArray& a) const
					{
						if (a.Size() != Points.size())
							vml::os::Message::Error("VertexArray : ", "Sizes must match");
						for (size_t i = 0; i < Points.size(); ++i)
							if (a.Points[i] == Points[i]) return false;
						return true;
					}

					// -------------------------------------------------------------------------------

					friend bool operator==(const VertexArray& a, const VertexArray& b);
					friend bool operator!=(const VertexArray& a, const VertexArray& b);
					friend VertexArray operator*(const VertexArray& a, const float s);
					friend VertexArray operator*(const float s, const VertexArray& a);
					friend VertexArray operator*(const VertexArray& a, const VertexArray& b);
					friend VertexArray operator/(const float s, const VertexArray& a);
					friend VertexArray operator/(const VertexArray& a, const float s);
					friend VertexArray operator+(const VertexArray& a, const VertexArray& b);
					friend VertexArray operator-(const VertexArray& a, const VertexArray& b);
					friend VertexArray operator-(const VertexArray& a);
					friend VertexArray operator+(const VertexArray& a);

					// -------------------------------------------------------------------------------
					// casting to pointer

					operator const std::vector<glm::vec2> &() 
					{
						return (const std::vector<glm::vec2>&)Points;
					}

					// -------------------------------------------------------------------------------
					// assignment operator

					VertexArray& operator=(const std::vector<glm::vec2> &points)
					{
						Points.clear();
						for (size_t i = 0; i < points.size(); ++i)
							Points.emplace_back(points[i]);
						End();
						return *this;
					}

					VertexArray& operator=(const VertexArray &other)
					{
						Points.clear();
						for (size_t i = 0; i < other.Size(); ++i)
							Points.emplace_back(other.Points[i]);
						End();
						return *this;
					}

					VertexArray(const VertexArray& other)
					{
						Points.clear();
						for (size_t i = 0; i < other.Size(); ++i)
							Points.emplace_back(other.Points[i]);
						End();
					}

					// -------------------------------------------------------------------------------
					// ctor / dtor

					VertexArray()
					{
						Clear();
					}

					~VertexArray()
					{}

		};

		bool operator==(const VertexArray& a, const VertexArray& b)
		{
			if (a.Size() != b.Size())
				vml::os::Message::Error("VertexArray : ", "Sizes must match");
			for (size_t i = 0; i < a.Size(); ++i)
				if (a.Points[i] != b.Points[i]) return false;
			return true;
		}

		bool operator!=(const VertexArray& a, const VertexArray& b)
		{
			if (a.Size() != b.Size())
				vml::os::Message::Error("VertexArray : ", "Sizes must match");
			for (size_t i = 0; i < a.Size(); ++i)
				if (a.Points[i] == b.Points[i]) return false;
			return true;
		}

		VertexArray operator*(const VertexArray& a, const float s)
		{
			VertexArray v;
			v.Begin();
			for (size_t i = 0; i < a.Size(); ++i)
				v.Add(a.Points[i] * s);
			v.End();
			return v;
		}

		VertexArray operator*(const float s, const VertexArray& a)
		{
			VertexArray v;
			v.Begin();
			for (size_t i = 0; i < a.Size(); ++i)
				v.Add(a.Points[i] * s);
			v.End();
			return v;
		}

		VertexArray operator*(const VertexArray& a, const VertexArray& b)
		{
			if (a.Size() != b.Size())
				vml::os::Message::Error("VertexArray : ", "Sizes must match");
			VertexArray v;
			v.Begin();
			for (size_t i = 0; i < a.Size(); ++i)
				v.Add(a.Points[i] * b.Points[i]);
			v.End();
			return v;
		}

		VertexArray operator/(const float s, const VertexArray& a)
		{
			VertexArray v;
			v.Begin();
			for (size_t i = 0; i < a.Size(); ++i)
				v.Add(s / a.Points[i]);
			v.End();
			return v;
		}

		VertexArray operator/(const VertexArray& a, const float s)
		{
			VertexArray v;
			v.Begin();
			for (size_t i = 0; i < a.Size(); ++i)
				v.Add(a.Points[i] / s);
			v.End();
			return v;
		}

		VertexArray operator+(const VertexArray& a, const VertexArray& b)
		{
			if (a.Size() != b.Size())
				vml::os::Message::Error("VertexArray : ", "Sizes must match");
			VertexArray v;
			v.Begin();
			for (size_t i = 0; i < a.Size(); ++i)
				v.Add(a.Points[i] + b.Points[i]);
			v.End();
			return v;
		}

		VertexArray operator-(const VertexArray& a, const VertexArray& b)
		{
			if (a.Size() != b.Size())
				vml::os::Message::Error("VertexArray : ", "Sizes must match");
			VertexArray v;
			v.Begin();
			for (size_t i = 0; i < a.Size(); ++i)
				v.Add(a.Points[i] - b.Points[i]);
			v.End();
			return v;
		}

		VertexArray operator-(const VertexArray& a)
		{
			VertexArray v;
			v.Begin();
			for (size_t i = 0; i < a.Size(); ++i)
				v.Add(-a.Points[i]);
			v.End();
			return v;
		}

		VertexArray operator+(const VertexArray& a)
		{
			VertexArray v;
			v.Begin();
			for (size_t i = 0; i < a.Size(); ++i)
				v.Add(+a.Points[i]);
			v.End();
			return v;
		}

	} // end of geo2d namespace

} // end of vml namepsace
