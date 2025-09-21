#pragma once

/////////////////////////////////////

#include <vml2.0/math/3d/collisions.h>

namespace vml
{
	namespace objects
	{

		class Model3d_2
		{
				
			private:

				Model3d_2								   *Parent;							// Parent object
				std::vector<Model3d_2*>						Child;							// children objects
				std::string									ScreenName;						// Object ScreenName
				std::string									FullPathScreenName;				// Screenname based on model hierarchy
				unsigned int								InternalFlags;					// Bitfield used to store internal flags
				unsigned int								PreferencesFlags;				// Bitfield used to store preferences flags
				unsigned int 								CullingFlags;					// Bitfield used to store internal flags
				glm::vec3									Position;						// position
				glm::vec3									Rotation;						// euler rotation angles
				glm::vec3									Scaling;						// scaling factor
				glm::vec3									OriginalRotation;				// old rotation position , used for deltas
				glm::vec3									OriginalScaling;				// old scaling position , used for deltas
				glm::vec3									OriginalPosition;				// old vector position , used for deltas
				std::vector<vml::meshes::Mesh3d*>			Meshes;							// Mesh pointer
				vml::geo3d::collisions::EllipsoidCollider*	EllipsoidCollider;				// Colliders
				int											CurrentMesh;					// Currentmesh indx
				vml::geo3d::AABBox							AABoundingBox;					// Bounding box
				vml::geo3d::AOBBox							AOBoundingBox;					// Bounding box
				float										Radius;							// Radius
				glm::quat									Quaternion;						// Quaternion 
				glm::vec3									Right;							// Right direction vector
				glm::vec3									Forward;						// Forward direction vector
				glm::vec3									Up;								// Up direction vector
				glm::mat4									M;								// model matrix
				glm::mat4									MV;								// model * view matrix
				glm::mat4									MVP;							// model * view * projection matrix
				glm::mat4									NV;								// viewnormal matrix
				bool										Visible;

				// ---------------------------------------------------------------
				// remove mesh if uniqe , if mesh is still in use, the
				// resource manager waits until no more models use it.

				void ReleaseAll()
				{
					// release attached mesh(es) trhough the meshstore

					for (size_t i = 0; i < Meshes.size(); ++i)
						vml::MeshStore->UnLoad(Meshes[i]->GetResourceFileName());
					
					// clear meshes array

					Meshes.clear();

					// release colliders

					vml::SafeDelete(EllipsoidCollider);

				}

			public:

				// ---------------------------------------------------------------
				// specify rotation transformation

				static const unsigned int EULER		  = vml::bits32::BIT0;
				static const unsigned int QUATERNIONS = vml::bits32::BIT1;
				static const unsigned int POLAR		  = vml::bits32::BIT2;
				static const unsigned int CUSTOM	  = vml::bits32::BIT3;

				// ---------------------------------------------------------------
				// specify rendering modes

				static const unsigned int WIREFRAME = vml::bits32::BIT16;

				// -------------------------------------------------------------
				// transform objet to view

				void TransformToView(vml::views::View* View)
				{
					// get View and projection matrices

					const glm::mat4& p = View->GetProjection();
					const glm::mat4& v = View->GetView();

					// concatenate v and p matrices

					MV  = v * M;
					MVP = p * MV;

					// floowing code is this transformation sequence 
					// using glm  marix lib

					//model->NV  = glm::mat3(glm::transpose(glm::inverse(model->MV)));

					// get normal matrix pointer

					float *normalviewmatrix = glm::value_ptr(NV);

					// get model * view matrix

					float *modelviewmatrix = glm::value_ptr(MV);

					// normal matrix is computed as the inverse transpose
					// of the model view matrix, this causes the 
					// first 3x3 order for this matrix is meant to be divided 
					// by each scaling factor along correspondig axes, so 
					// we need to unskew normal matrix

					float determinant = + modelviewmatrix[0] * (modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[9])
									    - modelviewmatrix[1] * (modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[6] * modelviewmatrix[8])
									    + modelviewmatrix[2] * (modelviewmatrix[4] * modelviewmatrix[ 9] - modelviewmatrix[5] * modelviewmatrix[8]);

					if (determinant > -vml::math::EPSILON && determinant < vml::math::EPSILON) determinant = 1.0f / vml::math::EPSILON;
						else determinant = 1.0f / determinant;

					normalviewmatrix[0] = +(modelviewmatrix[5] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[6]) * determinant;
					normalviewmatrix[1] = -(modelviewmatrix[4] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[6]) * determinant;
					normalviewmatrix[2] = +(modelviewmatrix[4] * modelviewmatrix[ 9] - modelviewmatrix[8] * modelviewmatrix[5]) * determinant;
					normalviewmatrix[3] = -(modelviewmatrix[1] * modelviewmatrix[10] - modelviewmatrix[9] * modelviewmatrix[2]) * determinant;
					normalviewmatrix[4] = +(modelviewmatrix[0] * modelviewmatrix[10] - modelviewmatrix[8] * modelviewmatrix[2]) * determinant;
					normalviewmatrix[5] = -(modelviewmatrix[0] * modelviewmatrix[ 9] - modelviewmatrix[8] * modelviewmatrix[1]) * determinant;
					normalviewmatrix[6] = +(modelviewmatrix[1] * modelviewmatrix[ 6] - modelviewmatrix[5] * modelviewmatrix[2]) * determinant;
					normalviewmatrix[7] = -(modelviewmatrix[0] * modelviewmatrix[ 6] - modelviewmatrix[4] * modelviewmatrix[2]) * determinant;
					normalviewmatrix[8] = +(modelviewmatrix[0] * modelviewmatrix[ 5] - modelviewmatrix[4] * modelviewmatrix[1]) * determinant;

				}

				// -----------------------------------------------------------------------
				// transform model bounding box

				void TransformBoundingBoxes()
				{
					// compute oriented bounding box
					// the bounding box is computed transforming
					// models' mesh's bounding box by the model's
					// M matrix , which is the T * R * S matrix
					// see aabbox.h
					// 
					// cache model's M matrix

					const float* m = glm::value_ptr(M);

					// get's model's mesh bounding box coordinates

					AOBoundingBox.Set(GetCurrentMesh()->GetBoundingBox().GetMin(), GetCurrentMesh()->GetBoundingBox().GetMax(), m);

					// compute axis aligned bounding box

					AABoundingBox.Set(AOBoundingBox.GetMin(), AOBoundingBox.GetMax());

					// compute bounding sphere

					Radius = sqrtf(AABoundingBox.GetExtents().x * AABoundingBox.GetExtents().x +
							 	   AABoundingBox.GetExtents().y * AABoundingBox.GetExtents().y +
								   AABoundingBox.GetExtents().z * AABoundingBox.GetExtents().z)*0.5f;

					// compute direction vectors
					// the oriented bounding box is centered at root's center

					Right   = glm::normalize(glm::vec3(m[0], m[1], m[ 2]));
					Up      = glm::normalize(glm::vec3(m[4], m[5], m[ 6]));
					Forward = glm::normalize(glm::vec3(m[8], m[9], m[10]));
				}

				//-----------------------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Model3d_2(Model3d_2& model) = delete;

				//-----------------------------------------------------------------------------------
				// overload operator is private,
				// no copies allowed since classes
				// are referenced

				void operator=(const Model3d_2& model) = delete;

				// --------------------------------------------------------------------------------
				// getters

				const std::string						  &GetScreenName()			const { return ScreenName; }
				const std::string						  &GetFullPathScreenName()	const { return FullPathScreenName; }
				unsigned int							   GetInternalFlags()		const { return InternalFlags; }
				unsigned int							   GetPreferencesFlags()	const { return PreferencesFlags; }
				Model3d_2								  *GetParent()				const { return Parent; }							
				size_t									   GetChildCount()			const { return Child.size(); }					
				vml::geo3d::collisions::EllipsoidCollider *GetEllipsoidCollider()	const { return EllipsoidCollider; }

				// --------------------------------------------------------------------------
				// metrics getters

				const vml::geo3d::AABBox &GetAABoundingBox()		const { return AABoundingBox; }					
				const vml::geo3d::AOBBox &GetAOBoundingBox()		const {	return AOBoundingBox; }
				float					  GetRadius()				const {	return Radius; }							 
				const glm::vec3			 &GetRightVector()			const { return Right; }
				const glm::vec3			 &GetForwardVector()		const {	return Forward; }
				const glm::vec3			 &GetUpVector()				const { return Up; }
				glm::vec3				  GetTransformedPosition()  const { const float* m = glm::value_ptr(M); return glm::vec3(m[12], m[13], m[14]); }
				bool					  IsVisbile()				const { return Visible; }
				unsigned int			  GetCullingFlags()			const { return CullingFlags; }
				unsigned int			  IsInFrustum()				const { return CullingFlags != vml::views::frustum::OUTSIDE; }
				bool					  IsWire()					const { return vml::bits32::Get(PreferencesFlags, WIREFRAME); }
				bool					  IsSolid()					const { return !vml::bits32::Get(PreferencesFlags, WIREFRAME); }

				// --------------------------------------------------------------------------
				// hierarchy getters

				Model3d_2				*GetChild(size_t pos)		 const { return *(Child.begin() + pos); }
				vml::meshes::Mesh3d	    *GetCurrentMesh()			 const { return Meshes[CurrentMesh]; }
				vml::meshes::Mesh3d		*GetMeshAt(const size_t pos) const { return Meshes[pos]; }
				size_t					 GetMeshesCount()			 const { return Meshes.size(); }

				// Gets Child model given screenname

				Model3d_2* GetChild(const std::string& childname) const
				{
					if (childname.empty())
						vml::os::Message::Error("ObjectManager : ", "Model3d : Null name is not allowed");
					for (size_t i = 0; i < Child.size(); ++i)
						if (Child[i]->GetScreenName() == childname) return Child[i];
					return nullptr;
				}

				// --------------------------------------------------------------------------
				// matrix getters

				float* GetMptr()		   { return glm::value_ptr(M); }
				float* GetNVptr()		   { return glm::value_ptr(NV); }
				float* GetMVptr()		   { return glm::value_ptr(MV); }
				float* GetMVPptr()		   { return glm::value_ptr(MVP); }
				glm::quat& GetQuaternion() { return Quaternion; }

				const glm::mat4& GetM()   const { return M; }
				const glm::mat4& GetMV()  const { return MV; }
				const glm::mat4& GetMVP() const { return MVP; }
				const glm::mat4& GetNV()  const { return NV; }

				// rotation / postion / scaling getters

				const glm::vec3& GetPosition() const { return Position; }			
				const glm::vec3& GetAngles()   const { return Rotation; }				
				const glm::vec3& GetScaling()  const { return Scaling; }				

				float GetPosX() const { return Position.x; }
				float GetPosY() const { return Position.y; }
				float GetPosZ() const { return Position.z; }

				// -----------------------------------------------------------------------
				// Get rotation mode

				int GetRotationMode() const
				{
					if (vml::bits32::Get(PreferencesFlags, QUATERNIONS)) return QUATERNIONS;
					if (vml::bits32::Get(PreferencesFlags, EULER))		 return EULER;
					if (vml::bits32::Get(PreferencesFlags, POLAR))		 return POLAR;
					if (vml::bits32::Get(PreferencesFlags, CUSTOM))		 return CUSTOM;
					return -1;
				}

				// ----------------------------------------------------------------------------------------
				// Setters

				void SetParent(Model3d_2* parent)								  { Parent = parent; }
				void ClearChild(Model3d_2* child)								  { Child.clear(); }
				void AddChild(Model3d_2* child)									  { Child.emplace_back(child); }
				void SetFullPathScreenName(const std::string& fullpathscreenname) { FullPathScreenName = fullpathscreenname; }
				void SetVisible(bool visible)									  { Visible = visible; }
				void SetCullingFlags(unsigned int cullingflags)					  { CullingFlags = cullingflags; }
				
				void SetRotationMode(int mode)
				{
					if (mode != EULER && mode != QUATERNIONS && mode != POLAR && mode != CUSTOM)
						vml::os::Message::Error("ObjectManager : ", "unacceptable rotation mode");
					vml::bits32::SetToTrue(PreferencesFlags, mode);
				}

				// --------------------------------------------------------------------------------

				void SetPosition(const glm::vec3& pos)		    { Position  = pos; }									// Set model position given a vector
				void SetOriginalPosition(const glm::vec3& pos)  { OriginalPosition = pos; }								// Set model position given a vector
				void Move(const glm::vec3& disp)			    { Position += disp; }									// Move object to position 
				void MoveX(const float px)					    { Position.x += px; }									// Move object to position 
				void MoveY(const float py)					    { Position.y += py; }									// Move object to position 
				void MoveZ(const float pz)					    { Position.z += pz; }									// Move object to position 
				void SetPosX(const float px)				    { Position.x = px; }									// Set model position given a vector
				void SetPosY(const float py)				    { Position.y = py; }									// Set model position given a vector
				void SetPosZ(const float pz)				    { Position.z = pz; }									// Set model position given a vector

				void SetAngles(const glm::vec3& angles)			{ Rotation  = angles * vml::math::DEGTORAD; }			// Rotation angles
				void AddAngles(const glm::vec3& angles)			{ Rotation += angles * vml::math::DEGTORAD; }			// Add rotation angles
				void SetOriginalAngles(const glm::vec3& angles) { OriginalRotation = angles * vml::math::DEGTORAD; }	// Rotation angles
				void SetAngleX(const float ax)					{ Rotation.x = ax; }
				void SetAngleY(const float ay)					{ Rotation.y = ay; }
				void SetAngleZ(const float az)					{ Rotation.z = az; }
				void AddAngleX(const float ax)				    { Rotation.x += ax; }
				void AddAngleY(const float ay)				    { Rotation.y += ay; }
				void AddAngleZ(const float az)				    { Rotation.z += az; }

				void SetScale(const glm::vec3& scale)		    { Scaling = scale; }										// Scale model by a scaling vector
				void SetOriginalScale(const glm::vec3& scale)   { OriginalScaling = scale; }								// Scale model by a scaling vector
				void SetScaleX(const float sx)				    { Scaling.x = sx; }
				void SetScaleY(const float sy)				    { Scaling.y = sy; }
				void SetScaleZ(const float sz)				    { Scaling.z = sz; }
				void AddScaleX(const float sx)				    { Scaling.x *= sx; }
				void AddScaleY(const float sy)					{ Scaling.y *= sy; }
				void AddScaleZ(const float sz)					{ Scaling.z *= sz; }

				// --------------------------------------------------------------------------------
				
				void ResetScaling()	  {	Scaling  = OriginalScaling;  }													// Resets model scale
				void ResetPosition()  { Position = OriginalPosition; }													// Reset model position
				void ResetRotation()  {	Rotation = OriginalRotation; }													// Reset angle rotation with initial values

				// reset all transofrmations

				void ResetTransformations()																
				{
					Position = OriginalPosition;
					Rotation = OriginalRotation;
					Scaling  = OriginalScaling;
				}

				// -----------------------------------------------------------------------
				// Compute euler matrix

				void ComputeEulerMatrix()
				{
					float scx = Scaling.x;
					float scy = Scaling.y;
					float scz = Scaling.z;

					// get angles

					float alpha = Rotation.x;
					float phi   = Rotation.y;
					float theta = Rotation.z;

					// clamp angles

					if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

					if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

					if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
						else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

					// cache trigvalues

					float sx = sin(alpha);
					float sy = sin(phi);
					float sz = sin(theta);
					float cx = cos(alpha);
					float cy = cos(phi);
					float cz = cos(theta);

					// get parent matrix pointer

					float* matrix = glm::value_ptr(M);

					matrix[ 0] = ( cy * cz               ) * scx;
					matrix[ 1] = ( sx * sy * cz + cx * sz) * scx;
					matrix[ 2] = (-cx * sy * cz + sx * sz) * scx;
					matrix[ 3] = 0;
					matrix[ 4] = (-cy * sz               ) * scy;
					matrix[ 5] = (-sx * sy * sz + cx * cz) * scy;
					matrix[ 6] = ( cx * sy * sz + sx * cz) * scy;
					matrix[ 7] = 0;
					matrix[ 8] = ( sy					 ) * scz;
					matrix[ 9] = (-sx * cy				 ) * scz;
					matrix[10] = ( cx * cy				 ) * scz;
					matrix[11] = 0;
					matrix[12] = Position.x;
					matrix[13] = Position.y;
					matrix[14] = Position.z;
					matrix[15] = 1;
				}

				// -----------------------------------------------------------------------
				// compute quaternion matrix

				void ComputeQuaternionMatrix()
				{
					float scx = Scaling.x;
					float scy = Scaling.y;
					float scz = Scaling.z;

					glm::mat4 R = glm::toMat4(Quaternion);

					//	M = T * R * S ;

					float* matrix = glm::value_ptr(M);

					float* r = glm::value_ptr(R);

					matrix[ 0] = r[ 0] * scx;
					matrix[ 1] = r[ 1] * scx;
					matrix[ 2] = r[ 2] * scx;
					matrix[ 3] = 0;

					matrix[ 4] = r[ 4] * scy;
					matrix[ 5] = r[ 5] * scy;
					matrix[ 6] = r[ 6] * scy;
					matrix[ 7] = 0;

					matrix[ 8] = r[ 8] * scz;
					matrix[ 9] = r[ 9] * scz;
					matrix[10] = r[10] * scz;
					matrix[11] = 0;
					matrix[12] = Position.x;
					matrix[13] = Position.y;
					matrix[14] = Position.z;
					matrix[15] = 1;

				}

				// ------------------------------------------------------------------
				// Compute quaternion matrix

				void ComputePolarMatrix()
				{
					// cache scaling factors

					float scx = Scaling.x;
					float scy = Scaling.y;
					float scz = Scaling.z;

					// get angles

					float alpha = Rotation.x;
					float phi   = Rotation.y;
					float theta = Rotation.z;

					if (alpha < 0.0) alpha = fmod(alpha, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (alpha > vml::math::PI) alpha = fmod(alpha, vml::math::PIMUL2);

					if (phi < 0.0) phi = fmod(phi, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (phi > vml::math::PI) phi = fmod(phi, vml::math::PIMUL2);

					if (theta < 0.0) theta = fmod(theta, vml::math::PIMUL2) + vml::math::PIMUL2;
					else if (theta > vml::math::PI) theta = fmod(theta, vml::math::PIMUL2);

					float cx = cosf(phi);
					float sx = sinf(phi);
					float cy = cosf(theta);
					float sy = sinf(theta);
				
					// polar coordinates
					// direction axis in spherical coordinates

					glm::vec3 axis(cx * cy, sx, cx * sy);

					// create quaternion around this axis

					glm::quat Quaternion = glm::angleAxis(alpha, axis);

					glm::mat4 Q = glm::toMat4(Quaternion);

					float* QMatrix = glm::value_ptr(Q);

					float f0  = QMatrix[0];
					float f1  = QMatrix[1];
					float f2  = QMatrix[2];
				//  float f3  = QMatrix[3];		// f3 is 0
					float f4  = QMatrix[4];
					float f5  = QMatrix[5];
					float f6  = QMatrix[6]; 
				//  float f7  = QMatrix[7];		// f7 is 0
					float f8  = QMatrix[8];
					float f9  = QMatrix[9];
					float f10 = QMatrix[10];
				//  float f11 = QMatrix[11];	// f11 is 0

					// assumes axis is already normalised

				//	glm::vec3 up(0, 1, 0);
				//	glm::vec3 p(cx * cy, sx, cx * sy);
				//	glm::vec3 s(glm::normalize(glm::cross(p, up)));
				//	glm::vec3 u(glm::cross(s, p));
				//	glm::mat4 O( p.x , p.y , p.z , 0.0f ,
				//			 	 u.x , u.y , u.z , 0.0f ,
				//		 		 s.x , s.y , s.z , 0.0f ,
				//				 0   ,   0 ,   0 , 1.0f );

					// hardcoded version of the above code

					float r0  =  cx * cy;
					float r1  =  sx;
					float r2  =  cx * sy;
					float r4  = -cy * sx;
					float r5  =  cx;
					float r6  = -sx * sy;
					float r8  = -sy;
					float r10 =  cy;

					// get parent matrix pointer

					float *matrix = glm::value_ptr(M);
				
					matrix[ 0] = -(r8 * f0 + r10 *  f8) * scz;
					matrix[ 1] = -(r8 * f1 + r10 *  f9) * scz;
					matrix[ 2] = -(r8 * f2 + r10 * f10) * scz;
					matrix[ 3] =  0;
					matrix[ 4] =  (r4 * f0 + r5 * f4 + r6 *  f8) * scy;
					matrix[ 5] =  (r4 * f1 + r5 * f5 + r6 *  f9) * scy;
					matrix[ 6] =  (r4 * f2 + r5 * f6 + r6 * f10) * scy;
					matrix[ 7] =  0;
					matrix[ 8] =  (r0 * f0 + r1 * f4 + r2 *  f8) * scx;
					matrix[ 9] =  (r0 * f1 + r1 * f5 + r2 *  f9) * scx;
					matrix[10] =  (r0 * f2 + r1 * f6 + r2 * f10) * scx;
					matrix[11] =  0;
					matrix[12] =  Position.x;
					matrix[13] =  Position.y;
					matrix[14] =  Position.z;
					matrix[15] =  1;

				}

				// -------------------------------------------------------------
				// add collider to model

				void AddEllipsoidCollider(const glm::vec3& radii,const glm::vec3 &relpos=glm::vec3(0,0,0),const float restfactor=1.0f)
				{
					EllipsoidCollider = new vml::geo3d::collisions::EllipsoidCollider(radii,relpos,restfactor);
				}

				// ------------------------------------------------------------------
				// ctor / dtor

				Model3d_2( const std::string& screenname,
						   const std::string& filename,
						   const glm::vec3& pos = vml::geo3d::NullVec,
						   const glm::vec3& angles = vml::geo3d::NullVec,
						   const glm::vec3& scaling = vml::geo3d::UnaryVec,
						   const uint32 preferencesflags = EULER )
				{
					if (screenname.empty())
						vml::os::Message::Error("ObjectManager : ", "Cannot assign empty screenname");

					if (filename.empty())
						vml::os::Message::Error("ObjectManager : ", "NUll mesh resource filename is not allowed fro model ' ", ScreenName.c_str()," '");

					// init member data

					ScreenName		  = vml::strings::StringUtils::TrimAndLower(screenname);
					Parent            = nullptr;
					InternalFlags     = vml::InternalFlags::INITTED | vml::InternalFlags::FINALIZED;
					PreferencesFlags  = preferencesflags;
					CullingFlags	  = 0;
					CurrentMesh       = 0;
					EllipsoidCollider = nullptr;

					// set model data

					Position	     = pos;
					Rotation		 = angles * vml::math::DEGTORAD;
					Scaling		     = scaling;
					OriginalPosition = Position;
					OriginalRotation = Rotation;
					OriginalScaling  = Scaling;

					Quaternion		 = glm::quat(0,0,0,0);		

					if (fabs(Scaling.x) < vml::math::EPSILON ||
						fabs(Scaling.y) < vml::math::EPSILON ||
						fabs(Scaling.z) < vml::math::EPSILON)
							vml::os::Message::Error("ObjectManager : ", "Scale cannot bu null for model ' ", ScreenName.c_str(), " '");

					// if rotation flags are not set, provide a way
					// to use euler rotation computation for matrix computations

					bool euler       = vml::bits32::Get(PreferencesFlags, EULER);
					bool quaternions = vml::bits32::Get(PreferencesFlags, QUATERNIONS);
					bool polar       = vml::bits32::Get(PreferencesFlags, POLAR);
					bool custom      = vml::bits32::Get(PreferencesFlags, CUSTOM);

					// rendering flags

					bool wire		 = vml::bits32::Get(PreferencesFlags, WIREFRAME);

					// if angles flag is not euler, quaternion or polar, 
					// then default to euler

					if (!((euler && !quaternions && !polar && !custom) ||
						 (!euler &&  quaternions && !polar && !custom) ||
						 (!euler && !quaternions &&  polar && !custom) ||
						 (!euler && !quaternions && !polar &&  custom)))
								vml::os::Message::Error("ObjectManager : ", "Choose only one rotation mode for model ' ", ScreenName.c_str()," '");
									
					// uploads the requested mesh

					Meshes.emplace_back(vml::MeshStore->Load(filename));

					// init matrices

					M   = glm::mat4(0);
					MV  = glm::mat4(0);
					MVP = glm::mat4(0);
					NV  = glm::mat3(0);

					// init direction vector

					Right   = glm::vec3(1, 0, 0);
					Up      = glm::vec3(0, 1, 0);
					Forward = glm::vec3(0, 0, 1);

					// initialise metrics

					AOBoundingBox = vml::geo3d::AOBBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					AABoundingBox = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					Radius		  = 0.0f;

					Visible = true;
				}

				~Model3d_2()
				{
					ReleaseAll();
				}

		};

		//////////////////////////////////////////////////////////////////////////////////////////////

		class Object3d_2
		{
			
			private:

				//------------------------------------------------------------------
				// private data
				
				std::string				  FileName;				// filename for this object includes full path
				std::string				  ScreenName;			// screenname for this object
				Model3d_2			    **Stack;				// stacked breadth-first hierarchyical transformation
				int						  StackCounter;			// Stack counter
				vml::geo3d::AABBox	      AABoundingBox;		// Bounding box
				vml::geo3d::AOBBox	      AOBoundingBox;		// Bounding box
				float					  Radius;				// radius for this object
				glm::vec3				  Right;				// Right direction vector
				glm::vec3				  Forward;				// Forward direction vector
				glm::vec3				  Up;					// Up direction vector
				int						  CurrentModel;			// current model
				unsigned int			  InternalFlags;		// bitflag used for flagging
				unsigned int	 		  CullingFlags;			// Bitfield used to store internal flags
				std::vector<Model3d_2*>	  Models;				// models building up the object

				//------------------------------------------------------------------
				// release all items
				
				void ReleaseAll()
				{
					// meshes are released through the resource manager

					 for ( size_t i=0; i<Models.size(); ++i )
						vml::SafeDelete ( Models[i] );

					// delete models array

					 Models.clear();

					// delete stack

					 vml::SafeDelete(Stack);
						
				}
				
				// -------------------------------------------------------------
				// compute stack base on hierarchy between object's models
				
				void ComputeStack()
				{
		
					// allocate space for stack, the stack is used for 
					// hierarchical matrix transformation
			
					vml::SafeDelete(Stack);
						
					size_t n = Models.size();

					Stack= new vml::objects::Model3d_2 *[ n ];
						
					// initial stack counter position

					StackCounter = 1;

					// root model is always at 0 position

					Stack[0] = Models[0];
						
					// breadth first stack

					if ( n>1 )
					{
						for (size_t sp = 0; sp < StackCounter; ++sp)
						{
							for (size_t j = 0; j < Stack[sp]->GetChildCount(); ++j)
							{
								Stack[StackCounter++] = Stack[sp]->GetChild(j);
							}
						}
					}

					//Log.Out("Object", "Connected", ScreenName, vml::strings::StringFormat::Text("Connected {0} models", sc));

					if (StackCounter !=Models.size() )
						vml::os::Message::Error("Object Instance ' ", ScreenName.c_str()," ' : All models must be connected");

					// create full path string for each model
			
					for (size_t i = 0; i < StackCounter; ++i)
					{
						if (i == 0)
						{
							Stack[0]->SetFullPathScreenName("root\\" + Stack[0]->GetScreenName());
						}
						else
						{
							Stack[i]->SetFullPathScreenName(Stack[i]->GetParent()->GetFullPathScreenName() + "\\" + vml::strings::SplitPath::GetTitle(Stack[i]->GetScreenName()));
						}
					}

					//	for (size_t i = 0; i < StackCounter; ++i)
					//	{
					//		if ( Stack[i]->GetParent())
					//			std::cout << Stack[i]->GetFullPathScreenName() << " Parent : " << Stack[i]->GetParent()->GetFullPathScreenName() << std::endl;
					//		else
					//			std::cout << Stack[i]->GetFullPathScreenName() << " Parent : Root" << std::endl;
					//	}

				}

				// -----------------------------------------------------------------------
				// transform compound bounding box

				void TransformBoundingBoxes()
				{
						
					// compute bounding box for the entire object
					// basically iterate trhough all the object's models
					// and finds the lowest and farthest vertices for
					// each models' axis aligned bounding box

					glm::vec3 alignedboundingboxmin( FLT_MAX,  FLT_MAX,  FLT_MAX);
					glm::vec3 alignedboundingboxmax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					for (size_t i = 0; i < Models.size(); ++i)
					{

						const vml::geo3d::AABBox& bbox = Stack[i]->GetAABoundingBox();

						if (bbox.GetMin().x < alignedboundingboxmin.x) alignedboundingboxmin.x = bbox.GetMin().x;
						if (bbox.GetMin().y < alignedboundingboxmin.y) alignedboundingboxmin.y = bbox.GetMin().y;
						if (bbox.GetMin().z < alignedboundingboxmin.z) alignedboundingboxmin.z = bbox.GetMin().z;

						if (bbox.GetMax().x > alignedboundingboxmax.x) alignedboundingboxmax.x = bbox.GetMax().x;
						if (bbox.GetMax().y > alignedboundingboxmax.y) alignedboundingboxmax.y = bbox.GetMax().y;
						if (bbox.GetMax().z > alignedboundingboxmax.z) alignedboundingboxmax.z = bbox.GetMax().z;

					}

					// compute axis aligned bounding box

					AABoundingBox.Set(alignedboundingboxmin, alignedboundingboxmax);

					// compute bounding sphere

					Radius = sqrtf( AABoundingBox.GetExtents().x * AABoundingBox.GetExtents().x +
									AABoundingBox.GetExtents().y * AABoundingBox.GetExtents().y +
									AABoundingBox.GetExtents().z * AABoundingBox.GetExtents().z)*0.5f;

					// direction vectors are taken from the root model
					// the oriented bounding box is centered at root's center
					// 0-1 x axis
					// 0-3 y axis
					// 0-4 z axis

					Right   = Models[0]->GetRightVector();
					Up      = Models[0]->GetUpVector();
					Forward = Models[0]->GetForwardVector();

					// gets oriented bounding box

					float dist;
					glm::vec3 p0, p1, p2, p3, p4, p5, p6, p7;

					float sizemx =  FLT_MAX;
					float sizeMx = -FLT_MAX;
					float sizemy =  FLT_MAX;
					float sizeMy = -FLT_MAX;
					float sizemz =  FLT_MAX;
					float sizeMz = -FLT_MAX;

					const glm::vec3 &center = AABoundingBox.GetCenter();

					for (size_t i = 0; i < Models.size(); ++i)
					{
							
						const vml::geo3d::AOBBox& aobbox = Models[i]->GetAOBoundingBox();
							
						p0 = aobbox[0] - center;
						p1 = aobbox[1] - center;
						p2 = aobbox[2] - center;
						p3 = aobbox[3] - center;
						p4 = aobbox[4] - center;
						p5 = aobbox[5] - center;
						p6 = aobbox[6] - center;
						p7 = aobbox[7] - center;
							
						dist = glm::dot(Right, p0); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p1); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p2); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p3); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p4); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p5); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p6); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;
						dist = glm::dot(Right, p7); if (dist < sizemx) sizemx = dist; if (dist > sizeMx) sizeMx = dist;

						dist = glm::dot(Up, p0); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p1); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p2); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p3); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p4); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p5); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p6); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;
						dist = glm::dot(Up, p7); if (dist < sizemy) sizemy = dist; if (dist > sizeMy) sizeMy = dist;

						dist = glm::dot(Forward, p0); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p1); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p2); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p3); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p4); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p5); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p6); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
						dist = glm::dot(Forward, p7); if (dist < sizemz) sizemz = dist; if (dist > sizeMz) sizeMz = dist;
							
					}
						
					// create oriented bounding box
					// minimum is p0 + p1 + p2 + center, 
					// maximum is p3 + p4 + p5 + center

					p0 = sizemx * Right;
					p1 = sizemy * Up;
					p2 = sizemz * Forward;
					p3 = sizeMx * Right;
					p4 = sizeMy * Up;
					p5 = sizeMz * Forward;

					AOBoundingBox.Set(glm::vec3(p0 + p1 + p2 + center),
									  glm::vec3(p3 + p1 + p2 + center),
									  glm::vec3(p3 + p4 + p2 + center),
									  glm::vec3(p0 + p4 + p2 + center),
									  glm::vec3(p0 + p1 + p5 + center),
									  glm::vec3(p3 + p1 + p5 + center),
									  glm::vec3(p3 + p4 + p5 + center),
									  glm::vec3(p0 + p4 + p5 + center));

				}

			public :

				//-----------------------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Object3d_2(Object3d_2& object) = delete;

				//-----------------------------------------------------------------------------------
				// overload operator is private,
				// no copies allowed since classes
				// are referenced

				void operator=(const Object3d_2& model) = delete;

				//-------------------------------------------------------------------------
				// adds a model to the current object
		
				Model3d_2 *AddModel(const std::string &screenname,
								    const std::string &filename,
								    const glm::vec3 &pos,
								    const glm::vec3 &rot,
								    const glm::vec3 &scale,
								    const unsigned int flags)
				{

					//create resource screenname string as :  Object/Model

					std::string modelscreenname = ScreenName + "\\" + vml::strings::StringUtils::TrimAndLower(screenname);

					// check if we already have this screenname taken
					
					for (size_t i = 0; i<Models.size(); ++i)
					  if ( modelscreenname==Models[i]->GetScreenName() )
							vml::os::Message::Error("ObjectManager : ", "Object3d Instance ' ", ScreenName.c_str(), " ' : ScreenName must be unique");

					//Log.Out("Object", ScreenName.c_str(), vml::strings::StringFormat::Text("Adding new model to object '{0}'", filename.c_str()).c_str());

					// create a new model

					vml::objects::Model3d_2 *newmodel= new vml::objects::Model3d_2( modelscreenname,filename,pos,rot,scale,flags );

					// store this model into object's model array

					Models.emplace_back ( newmodel );

					return newmodel;
								
				}
						
				// -------------------------------------------------------------
				// link parts togheter
								
				void Link( const std::string &parent, const std::string &child )
				{
					if ( parent.empty() )
						vml::os::Message::Error("ObjectManager : ","Object3d Instance ' ", ScreenName.c_str()," ' : Source Name cannot be null" );

					if ( child.empty() )
						vml::os::Message::Error("ObjectManager : ", "Object3d Instance ' ", ScreenName.c_str(), " ' : Dest Name cannot be null" );
								
					// find root node

					std::string parentname = ScreenName + "\\" + vml::strings::StringUtils::TrimAndLower(parent);

					vml::objects::Model3d_2* parentmodel = GetModelAt(parentname);

					if (parentmodel)
					{
						// find child node

						std::string childname = ScreenName + "\\" + vml::strings::StringUtils::TrimAndLower(child);

						vml::objects::Model3d_2* childmodel = GetModelAt(childname);

						if (childmodel)
						{
							// check if same nodes are linked

							if (parentmodel == childmodel)
								vml::os::Message::Error("ObjectManager : ","Object3d Instance ' ", ScreenName.c_str()," ' : Cannot link same nodes" );

							// check for cyclic connections, this is a fatal error

							for (size_t i = 0; i < childmodel->GetChildCount(); ++i)
								if ( childmodel->GetChild(i)==parentmodel)
									vml::os::Message::Error("ObjectManager : ","Object3d Instance ' ", ScreenName.c_str()," ' : cyclic connections aren't allowed from ' ",
										childmodel->GetChild(i)->GetScreenName().c_str()," ' to ' " , parentname.c_str(), " '");

							// link child to parent and parent to child

							childmodel->SetParent(parentmodel);
							parentmodel->AddChild(childmodel);

		//						std::cout << "Linking : " << childmodel->GetScreenName() << " To " << parentmodel->GetScreenName() << std::endl;

						}
						else
						{
							vml::os::Message::Error("ObjectManager : ","Object3d Instance ' " , ScreenName.c_str()," ' : Cannot find child model ' ", childname.c_str(), " '");
						}

					}
					else
					{
						vml::os::Message::Error("ObjectManager : ","Object3d Instance '", ScreenName.c_str() , " ' : Cannot find parent model ' ", parentname.c_str(), " '");
					}

				}
				
				//-----------------------------------------------------------------
				// scale object

				void Scale(const glm::vec3& scaling)
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");

					if (fabs(scaling.x) < vml::math::EPSILON ||
						fabs(scaling.x) < vml::math::EPSILON ||
						fabs(scaling.x) < vml::math::EPSILON)
							vml::os::Message::Error("ObjectManager : ", "Object scaling factor is too small");

					// scale the entire model according to scaling factor

					for (size_t i = 0; i < Models.size(); ++i)
					{
						// scale model

						Models[i]->SetScale(Models[i]->GetScaling() * scaling);
						Models[i]->SetOriginalScale(Models[i]->GetScaling());

						// scale position

						Models[i]->SetPosition(Models[i]->GetPosition() * scaling);
						Models[i]->SetOriginalPosition(Models[i]->GetPosition());
					}

					// unscale root position

					Models[0]->SetPosition(Models[0]->GetPosition() / scaling);
					Models[0]->SetOriginalPosition(Models[0]->GetPosition());
				}

				// -------------------------------------------------------------
				// finalize object

				Object3d_2* Finalize( const glm::vec3 &pos=glm::vec3(0,0,0),
									  const glm::vec3 &rot=glm::vec3(0,0,0),
									  const glm::vec3 &scaling=glm::vec3(1,1,1) )
				{
								
					if (Models.size() == 0)
						vml::os::Message::Error("ObjectManager : ", "Object3d instance : Object ' ", ScreenName.c_str()," ' has no models attached");

					// compute stack for transformation and rendering

					ComputeStack();
				
					//	Log.Out("Object", ScreenName.c_str(), "Finalized");

					// set finalized flag to true

					vml::bits32::SetToTrue(InternalFlags, vml::InternalFlags::FINALIZED);

					// position root model at given postion and angles

					GetRootModel()->SetPosition(pos);
					GetRootModel()->SetAngles(rot);

					// scale the entire model according to scaling factor
					
					Scale(scaling);
					
					// kickstart trasnformation pipeline

					Transform();
						
					// set current model as root model

					CurrentModel = 0;

					return this;
								
				}
							
				// -------------------------------------------------------------
				// transform objet to view

				void TransformToView(vml::views::View *view)
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");

					for (size_t i = 0; i < Models.size(); ++i)
						Models[i]->TransformToView(view);
				}
							
				// -------------------------------------------------------------
				// transform pipeline for object
				// non recursive breadth-first tree traversal

				void Transform()
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ","Object is not finalized");
						
					vml::objects::Model3d_2* root   = nullptr;
					vml::objects::Model3d_2* parent = nullptr;
					vml::objects::Model3d_2* child  = nullptr;
								
					float *destchildmatrix   = nullptr;
					float *childmatrix		 = nullptr;
					float *parentmatrix		 = nullptr;
					float  parentscale[3]    = { 0 };
					float  invparentscale[3] = { 0 };

					// get root model

					root = Stack[0];

					// if model needs to be transformed
					// we perform the M matrix computation
					// else we use the M matrix from the previous
					// frame, which left the matrix itself unchanged

					// get parent matrix pointer

					parentmatrix = root->GetMptr();

					//	vml::os::Trace("%d\n", root->GetRotationMode());

					switch (root->GetRotationMode())
					{
						case vml::objects::Model3d_2::EULER		  :	root->ComputeEulerMatrix();		 break;
						case vml::objects::Model3d_2::QUATERNIONS :	root->ComputeQuaternionMatrix(); break;
						case vml::objects::Model3d_2::POLAR		  : root->ComputePolarMatrix();		 break;
						case vml::objects::Model3d_2::CUSTOM	  :									 break;
					}

					// compute bounding boxes and oriented vectors

					root->TransformBoundingBoxes();
							
					// transform graph using the breadth-first stack computed previously

					for (size_t i = 1; i < Models.size(); ++i)
					{
								
						// get parent

						parent = Stack[i]->GetParent();

						// get child

						child = Stack[i];
								
						// if model needs to be transformed
						// we perform the M matrix computation
						// else we use the M matrix from the previous
						// frame, which left the matrix itself unchanged

						switch (child->GetRotationMode())
						{
							case vml::objects::Model3d_2::EULER       :	child->ComputeEulerMatrix();	  break;
							case vml::objects::Model3d_2::QUATERNIONS :	child->ComputeQuaternionMatrix(); break;
							case vml::objects::Model3d_2::POLAR		  :	child->ComputePolarMatrix();	  break;
							case vml::objects::Model3d_2::CUSTOM	  :								      break;
						}

						// rescale parent matrix , this is necessary
						// becasue of child-parent matrix multiplication
						// actually this performs a multiplicatiuon 
						// between the parent model matrix and the
						// inverse of its own scaling matrix

						// cache scaling factors

						parentscale[0] = parent->GetScaling().x;
						parentscale[1] = parent->GetScaling().y;
						parentscale[2] = parent->GetScaling().z;

						invparentscale[0] = 1.0f / parentscale[0];
						invparentscale[1] = 1.0f / parentscale[1];
						invparentscale[2] = 1.0f / parentscale[2];
								
						// get parent matrix pointer

						parentmatrix = parent->GetMptr();

						// scale parent matrix
								
						parentmatrix[ 0] *= invparentscale[0];
						parentmatrix[ 1] *= invparentscale[0];
						parentmatrix[ 2] *= invparentscale[0];
						parentmatrix[ 4] *= invparentscale[1];
						parentmatrix[ 5] *= invparentscale[1];
						parentmatrix[ 6] *= invparentscale[1];
						parentmatrix[ 8] *= invparentscale[2];
						parentmatrix[ 9] *= invparentscale[2];
						parentmatrix[10] *= invparentscale[2];

						// concatenate parent and child matrices

						childmatrix = child->GetMptr();
							
						// save child matrix to a temporary matrix

						float t[16] = { childmatrix[ 0] , childmatrix[ 1] , childmatrix[ 2] , childmatrix[ 3],
										childmatrix[ 4] , childmatrix[ 5] , childmatrix[ 6] , childmatrix[ 7],
										childmatrix[ 8] , childmatrix[ 9] , childmatrix[10] , childmatrix[11],
										childmatrix[12] , childmatrix[13] , childmatrix[14] , childmatrix[15] };
														
						//get access to child matrix pointer 

						destchildmatrix = child->GetMptr();

						// compute matrix multiplication

						destchildmatrix[ 0] = t[ 0] * parentmatrix[0] + t[ 1] * parentmatrix[4] + t[ 2] * parentmatrix[ 8] + t[ 3] * parentmatrix[12];
						destchildmatrix[ 1] = t[ 0] * parentmatrix[1] + t[ 1] * parentmatrix[5] + t[ 2] * parentmatrix[ 9] + t[ 3] * parentmatrix[13];
						destchildmatrix[ 2] = t[ 0] * parentmatrix[2] + t[ 1] * parentmatrix[6] + t[ 2] * parentmatrix[10] + t[ 3] * parentmatrix[14];
						destchildmatrix[ 3] = t[ 0] * parentmatrix[3] + t[ 1] * parentmatrix[7] + t[ 2] * parentmatrix[11] + t[ 3] * parentmatrix[15];
						destchildmatrix[ 4] = t[ 4] * parentmatrix[0] + t[ 5] * parentmatrix[4] + t[ 6] * parentmatrix[ 8] + t[ 7] * parentmatrix[12];
						destchildmatrix[ 5] = t[ 4] * parentmatrix[1] + t[ 5] * parentmatrix[5] + t[ 6] * parentmatrix[ 9] + t[ 7] * parentmatrix[13];
						destchildmatrix[ 6] = t[ 4] * parentmatrix[2] + t[ 5] * parentmatrix[6] + t[ 6] * parentmatrix[10] + t[ 7] * parentmatrix[14];
						destchildmatrix[ 7] = t[ 4] * parentmatrix[3] + t[ 5] * parentmatrix[7] + t[ 6] * parentmatrix[11] + t[ 7] * parentmatrix[15];
						destchildmatrix[ 8] = t[ 8] * parentmatrix[0] + t[ 9] * parentmatrix[4] + t[10] * parentmatrix[ 8] + t[11] * parentmatrix[12];
						destchildmatrix[ 9] = t[ 8] * parentmatrix[1] + t[ 9] * parentmatrix[5] + t[10] * parentmatrix[ 9] + t[11] * parentmatrix[13];
						destchildmatrix[10] = t[ 8] * parentmatrix[2] + t[ 9] * parentmatrix[6] + t[10] * parentmatrix[10] + t[11] * parentmatrix[14];
						destchildmatrix[11] = t[ 8] * parentmatrix[3] + t[ 9] * parentmatrix[7] + t[10] * parentmatrix[11] + t[11] * parentmatrix[15];
						destchildmatrix[12] = t[12] * parentmatrix[0] + t[13] * parentmatrix[4] + t[14] * parentmatrix[ 8] + t[15] * parentmatrix[12];
						destchildmatrix[13] = t[12] * parentmatrix[1] + t[13] * parentmatrix[5] + t[14] * parentmatrix[ 9] + t[15] * parentmatrix[13];
						destchildmatrix[14] = t[12] * parentmatrix[2] + t[13] * parentmatrix[6] + t[14] * parentmatrix[10] + t[15] * parentmatrix[14];
						destchildmatrix[15] = t[12] * parentmatrix[3] + t[13] * parentmatrix[7] + t[14] * parentmatrix[11] + t[15] * parentmatrix[15];

						// this is the equvalent of this
						//child->M = parent->M * child->M;

						// compute bounding boxes and oriented vectors

						child->TransformBoundingBoxes();

						// rescale parent matrix by its own scaling matrix

						parentmatrix[ 0] *= parentscale[0];
						parentmatrix[ 1] *= parentscale[0];
						parentmatrix[ 2] *= parentscale[0];
						parentmatrix[ 4] *= parentscale[1];
						parentmatrix[ 5] *= parentscale[1];
						parentmatrix[ 6] *= parentscale[1];
						parentmatrix[ 8] *= parentscale[2];
						parentmatrix[ 9] *= parentscale[2];
						parentmatrix[10] *= parentscale[2];

					}

					// compute compound bounding box

					TransformBoundingBoxes();

					// transform colliders

					for (size_t i = 0; i < Models.size(); ++i)
					{

						vml::geo3d::collisions::EllipsoidCollider* ec = Models[i]->GetEllipsoidCollider();

						if (ec)
						{
							ec->Transform(Models[i]->GetMptr(), Models[i]->GetScaling());
						}
						
					}

				}

				//------------------------------------------------------------------
				// cull compound bounding box

				void Cull(vml::views::View* view)
				{

					switch (vml::views::frustum::TestAABBox(view->GetFrustumPlanes(), 
						                                    AABoundingBox.GetCenter(), 
						                                    AABoundingBox.GetHalfExtents()))
					{

						case vml::views::frustum::OUTSIDE :
							
								// set culling flag to outside, and all of its component models

								CullingFlags = vml::views::frustum::OUTSIDE;

								for (size_t i = 0; i < Models.size(); ++i)
									Models[i]->SetCullingFlags(vml::views::frustum::OUTSIDE);

						break;

						case vml::views::frustum::INSIDE :

								// set culling flag to inside, and all of its component models

								CullingFlags = vml::views::frustum::INSIDE;

								for (size_t i = 0; i < Models.size(); ++i)
									Models[i]->SetCullingFlags(vml::views::frustum::INSIDE);

						break;

						case vml::views::frustum::INTERSECTED :

								// set culling flag to intersected, all of its component models
								// must be checkd against frustum to determine which is inside,outside or intersected

								CullingFlags = vml::views::frustum::INTERSECTED;

								// ottimizzare , si potrebbe scrivere questo 
								// 
								//	 Models[i]->SetCullingFlags(vml::views::frustum::TestAABBox(view->GetFrustumPlanes(),
								//																Models[i]->GetAABoundingBox().GetCenter(),
								//																Models[i]->GetAABoundingBox().GetHalfExtents()))
								
								for (size_t i = 0; i < Models.size(); ++i)
								{
									switch (vml::views::frustum::TestAABBox(view->GetFrustumPlanes(),
																			Models[i]->GetAABoundingBox().GetCenter(),
																			Models[i]->GetAABoundingBox().GetHalfExtents()))
									{
										case vml::views::frustum::OUTSIDE	  : Models[i]->SetCullingFlags(vml::views::frustum::OUTSIDE);	  break;
										case vml::views::frustum::INSIDE	  : Models[i]->SetCullingFlags(vml::views::frustum::INSIDE);	  break;
										case vml::views::frustum::INTERSECTED : Models[i]->SetCullingFlags(vml::views::frustum::INTERSECTED); break;
									}
								}

						break;

					}

				}

				//------------------------------------------------------------------
				// returns if object is finalized

				bool IsFinalized() const
				{
					return vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED);
				}

				//------------------------------------------------------------------

				const std::string& GetFileName() const
				{
					return FileName;
				}

				//------------------------------------------------------------------

				size_t GetModelsCount() const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return Models.size();
				}
				
				//------------------------------------------------------------------
				// gets model by position

				Model3d_2* GetModelAt(size_t pos) const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ","Object is not finalized");
					return Models[pos];
				}

				const std::string& GetScreenName() const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return ScreenName;
				}

				unsigned int GetInternalFlags() const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return InternalFlags;
				}
		
				unsigned int GetCullingFlags() const 
				{ 
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return CullingFlags; 
				}

				unsigned int IsInFrustum() const
				{
					return CullingFlags != vml::views::frustum::OUTSIDE;
				}

				float GetRadius() const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					return Radius;
				}

				//------------------------------------------------------------------
				// gets model by screen name

				Model3d_2* GetModelAt(const std::string& fullpathscreenname) const
				{
					for (size_t i = 0; i < Models.size(); ++i)
						if (fullpathscreenname == Models[i]->GetScreenName())
							return Models[i];
					return nullptr;
				}
	
				//------------------------------------------------------------------
				// gets root model, which is always the first model
				// position of the entire model is related to the root model

				Model3d_2* GetRootModel() const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ","Object is not finalized");
					return Models[0];
				}
				
				// Get axis aligned BoundingBox

				const vml::geo3d::AABBox& GetAABoundingBox() const
				{
					return AABoundingBox;
				}

				// Get axis aligned BoundingBox

				const vml::geo3d::AOBBox& GetAOBoundingBox() const
				{
					return AOBoundingBox;
				}

				// Get Forward vector

				const glm::vec3& GetForwardVector() const
				{
					return Forward;
				}

				// Get Right vector

				const glm::vec3& GetRightVector() const
				{
					return Right;
				}

				// Get Up vector

				const glm::vec3& GetUpVector() const
				{
					return Up;
				}

				// get position from model mattrix

				glm::vec3 GetTransformedRootPosition() const
				{
					const float* m = Models[0]->GetMptr();
					return glm::vec3(m[12], m[13], m[14]);
				}

				//-------------------------------------------------------------
				// get current model

				Model3d_2 *GetCurrentModel() const
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ","Object is not finalized");
					
					return Models[CurrentModel];
				}

				//-------------------------------------------------------------
				// increases currentmodel index and get model's pointer

				Model3d_2* GetNextModel() 
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
					
					CurrentModel++;
					int modelscount = (int)Models.size() - 1;
					if (CurrentModel > modelscount) CurrentModel = modelscount;
					return Models[CurrentModel];
				}

				//-------------------------------------------------------------
				// decreases currentmodel index and get model's pointer

				Model3d_2* GetPrevModel()
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");
				
					CurrentModel--;
					if (CurrentModel <= 0) CurrentModel = 0;
					return Models[CurrentModel];
				}

				//-------------------------------------------------------------
				// set current model

				vml::objects::Model3d_2* SetCurrentModel(size_t pos) 
				{
					if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::FINALIZED))
						vml::os::Message::Error("ObjectManager : ", "Object is not finalized");

					for (CurrentModel= 0; CurrentModel < Models.size(); ++CurrentModel)
						if (CurrentModel ==pos)
							return Models[(size_t)CurrentModel];

					vml::os::Message::Error("ObjectManager : ", "Cannot find Object at pos %d",pos);
				}

				//-------------------------------------------------------------
				// specialize in your derived class
		
				virtual void Controller(vml::views::View *view)
				{

				}

			public:

				// -------------------------------------------------------------------
				// compute the bounding box including bounding box colliders

				vml::geo3d::AABBox GetExpandedBoundingBox() const
				{
					float minx = GetAABoundingBox().GetMin().x;
					float miny = GetAABoundingBox().GetMin().y;
					float minz = GetAABoundingBox().GetMin().z;
					float maxx = GetAABoundingBox().GetMax().x;
					float maxy = GetAABoundingBox().GetMax().y;
					float maxz = GetAABoundingBox().GetMax().z;

					for (size_t i = 0; i < Models.size(); ++i)
					{

						vml::geo3d::collisions::EllipsoidCollider* ec = Models[i]->GetEllipsoidCollider();

						if (ec)
						{
							if (ec->GetMin().x < minx) minx = ec->GetMin().x;
							if (ec->GetMin().y < miny) miny = ec->GetMin().y;
							if (ec->GetMin().z < minz) minz = ec->GetMin().z;

							if (ec->GetMax().x > maxx) maxx = ec->GetMax().x;
							if (ec->GetMax().y > maxy) maxy = ec->GetMax().y;
							if (ec->GetMax().z > maxz) maxz = ec->GetMax().z;

						}
					}

					return vml::geo3d::AABBox(glm::vec3(minx, miny, minz), glm::vec3(maxx, maxy, maxz));
				}

				// ------------------------------------------------------------------
				// ctor / dtor

				Object3d_2(const std::string &screenname)
				{
					FileName		 = "";
					ScreenName       = vml::strings::StringUtils::TrimAndLower(screenname);
					InternalFlags    = vml::InternalFlags::INITTED;
					CullingFlags	 = 0;
					StackCounter     = 0;
					Stack            = nullptr;
					CurrentModel	 =-1;

					// init direction vector

					Right	= glm::vec3(1, 0, 0);
					Up		= glm::vec3(0, 1, 0);
					Forward = glm::vec3(0, 0, 1);

					// initialise metrics

					AABoundingBox = vml::geo3d::AABBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					AOBoundingBox = vml::geo3d::AOBBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
					Radius		  = 0;

				//		Log.Out("Object", "Creating new object", ScreenName.c_str());
				}

				virtual ~Object3d_2()
				{
					ReleaseAll();
					std::cout << "Releasing " << ScreenName << std::endl;
	//				Log.Out("Object", "Releasing instance of Object3d", ScreenName.c_str());
				}

		};

		//////////////////////////////////////////////////////////////////////////////////////////////

		class ObjectManager_2
		{

			private:

				std::vector<Object3d_2*>  Objects;				// Objects array	

				// ----------------------------------------------------
				// release memory

				void ReleaseAll()
				{
					// release object array

					for (auto it = Objects.begin(); it != Objects.end(); ++it)
						vml::SafeDelete(*it);

					Objects.clear();
				}
				
			public:

				// ----------------------------------------------------
				// Add object

				void AddObject(Object3d_2 *object)
				{
					// check if object pointer is null

					if (object)
					{
						if (!object->IsFinalized())
							vml::os::Message::Error("Object Manager : ","Object is not finalized");

						// duplicates names for object aren't allowed

						for (size_t i = 0; i < Objects.size(); ++i)
						{
							if (Objects[i]->GetScreenName() == object->GetScreenName())
							{
								vml::os::Message::Error("Object Manager : ","Duplicated names for objects are not allowed");
							}
						}

						// if we get here, we can store the object in the pointer vector

						Objects.emplace_back(object);

					}
					else
					{
						vml::os::Message::Error("Object Manager : ","Object pointer is null");
					}

				}
				
				// -----------------------------------------------------------------
				// retrieves objects given its name

				Object3d_2* GetObjectAt(const size_t pos) const
				{
					return Objects[pos];
				}

				// -----------------------------------------------------------------

				size_t GetObjectsCount() const
				{
					return Objects.size();
				}

				// ------------------------------------------------------------------
				// tranform object 

				void TransformObject(vml::views::View* view,vml::objects::Object3d_2* object)
				{

					// transform objects

					object->Transform();

					// cull objects

					object->Cull(view);

					// if object is in frustum, transform view objects

					if (object->GetCullingFlags() != vml::views::frustum::OUTSIDE)
					{
						object->TransformToView(view);
					}

				}

				// ----------------------------------------------------
				// call all objects controllers
				
				void CallObjectsController(vml::views::View* view)
				{
					for (size_t i = 0; i < Objects.size(); ++i)
					{
						Objects[i]->Controller(view);
					}
				}

				// ----------------------------------------------------
				// tranform objects 

				void TransformPipeline(vml::views::View* view)
				{
					for (size_t i = 0; i < Objects.size(); ++i)
					{
						TransformObject(view,Objects[i]);
					}
				}

				// -----------------------------------------------------------------
				// removes an object

				void RemoveObject(int pos)
				{
					if (pos<0 || pos>Objects.size())
						vml::os::Message::Error("Object Manager : ","Object index is out of range");

					// release object memenory 

					vml::SafeDelete(Objects[pos]);

					// remove from array

					Objects.erase(Objects.begin() + pos);
				}
				
				// -----------------------------------------------------------------
				// remove all objects 

				void RemoveAllObjects()
				{
					ReleaseAll();

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Object Manager", "Resetting Object Manager", "Done");
				}
				
				// ----------------------------------------------------
				// ctor / dtor

				ObjectManager_2()
				{
					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Object Manager", "Initting Object Manager", "Done");
				}

				~ObjectManager_2()
				{
					ReleaseAll();

					vml::Logger::GetInstance()->Out(vml::Logger::GetInstance()->GetCurrentDate(), "Object Manager", "Releasing Object Manager", "Done");
				}

		};

	}	// end of objects namespace

} // end of namespace vml


// ---------------------------------------------------------------
// default vectors

namespace vml
{
	namespace objects
	{

		static glm::vec3 ZeroPos = glm::vec3(0, 0, 0);
		static glm::vec3 ZeroRot = glm::vec3(0, 0, 0);
		static glm::vec3 UnaryScale = glm::vec3(1, 1, 1);

	}	// end of objects namespace

} // end of namespace vml

// example of adding aon obejct

//	void AddCube(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling = glm::vec3(1, 1, 1))
//	{
//		if (!vml::bits32::Get(InternalFlags, vml::InternalFlags::INITTED))
//			vml::os::Error("Scene is not initted");
//
//		// default rotation mode
//
//		int rotmode = vml::objects::Model3d_2::EULER;
//
//		// add obejcts
//
//		// generate screenname automatically
//
//		vml::objects::Object3d_2* currentobject;
//
//		// create an integer 'name' for this object
//
//		currentobject = new vml::objects::Object3d_2(vml::strings::StringConverter::GetUUID());
//
//		// add model to object
//
//		currentobject->AddModel("Root",
//								vml::Session::GetInstance()->GetMainPath() + "\\content\\opengl\\meshes\\cube.3df",
//								glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1),
//								rotmode);
//
//		// finalize object
//
//		currentobject->Finalize(pos, rot, scaling);
//
//		// add this shit
//
//		ObjectManager->AddObject(currentobject);
//
//	}
