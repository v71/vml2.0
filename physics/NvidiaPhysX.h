#pragma once

#include <vml2.0/libs/physx/include/PxPhysicsAPI.h>

namespace vml
{

	using namespace physx;

	const unsigned int MAX_NUM_ACTOR_SHAPES = 128;

	/*
	class ContactReportCallback: public PxSimulationEventCallback
	{
		void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)	{ PX_UNUSED(constraints); PX_UNUSED(count); }
		void onWake(PxActor** actors, PxU32 count)							{ PX_UNUSED(actors); PX_UNUSED(count); }
		void onSleep(PxActor** actors, PxU32 count)							{ PX_UNUSED(actors); PX_UNUSED(count); }
		void onTrigger(PxTriggerPair* pairs, PxU32 count)					{ PX_UNUSED(pairs); PX_UNUSED(count); }
		void onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}
		void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) 
		{
			std::vector<PxContactPairPoint> contactPoints;

			PxTransform spherePose(PxIdentity);
			PxU32 nextPairIndex = 0xffffffff;

			PxContactPairExtraDataIterator iter(pairHeader.extraDataStream, pairHeader.extraDataStreamSize);
			bool hasItemSet = iter.nextItemSet();
			if (hasItemSet)
				nextPairIndex = iter.contactPairIndex;

			for(PxU32 i=0; i < nbPairs; i++)
			{
				//
				// Get the pose of the dynamic object at time of impact.
				//
				if (nextPairIndex == i)
				{
					if (pairHeader.actors[0]->is<PxRigidDynamic>())
						spherePose = iter.eventPose->globalPose[0];
					else
						spherePose = iter.eventPose->globalPose[1];

					gContactSphereActorPositions.push_back(spherePose.p);

					hasItemSet = iter.nextItemSet();
					if (hasItemSet)
						nextPairIndex = iter.contactPairIndex;
				}

				//
				// Get the contact points for the pair.
				//
				const PxContactPair& cPair = pairs[i];
				if (cPair.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_CCD))
				{
					PxU32 contactCount = cPair.contactCount;
					contactPoints.resize(contactCount);
					cPair.extractContacts(&contactPoints[0], contactCount);

					for(PxU32 j=0; j < contactCount; j++)
					{
						gContactPositions.push_back(contactPoints[j].position);
						gContactImpulses.push_back(contactPoints[j].impulse);
					}
				}
			}
		}
	};

	ContactReportCallback gContactReportCallback;

	*/
	// -----------------------------------------------------------------

	class NvidiaPhysXEngine
	{
		private:

			// -----------------------------------------------------------------

			class TriggerRender
			{
				public:
					virtual	bool	isTrigger(physx::PxShape*)	const = 0;
			};

			// -----------------------------------------------------------------

			enum CCDAlgorithm
			{
				LINEAR_CCD,			// Uses linear CCD algorithm 
				SPECULATIVE_CCD,	// Uses speculative/angular CCD algorithm
				FULL_CCD,			// Uses linear & angular CCD at the same time
				RAYCAST_CCD,		// Uses raycast CCD algorithm 
				NO_CCD,				// Switch to NO_CCD to see the sphere go through the box stack without CCD.
				CCD_COUNT			// Number of CCD algorithms used in this snippet
			};

			// -----------------------------------------------------------------

			static PxFilterFlags ccdFilterShader(
													PxFilterObjectAttributes attributes0,
													PxFilterData filterData0,
													PxFilterObjectAttributes attributes1,
													PxFilterData filterData1,
													PxPairFlags& pairFlags,
													const void* constantBlock,
													PxU32 constantBlockSize)
			{
				PX_UNUSED(attributes0);
				PX_UNUSED(filterData0);
				PX_UNUSED(attributes1);
				PX_UNUSED(filterData1);
				PX_UNUSED(constantBlock);
				PX_UNUSED(constantBlockSize);

				pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT | PxPairFlag::eDETECT_CCD_CONTACT;

				return PxFilterFlags();
				
			/*
				pairFlags = PxPairFlag::eCONTACT_DEFAULT
					 	  | PxPairFlag::eDETECT_CCD_CONTACT
				 		  | PxPairFlag::eNOTIFY_TOUCH_CCD
						  | PxPairFlag::eNOTIFY_TOUCH_FOUND
						  | PxPairFlag::eNOTIFY_CONTACT_POINTS
						  | PxPairFlag::eCONTACT_EVENT_POSE;
	
				return PxFilterFlag::eDEFAULT;
			*/			
			}

			// -----------------------------------------------------------------

			void InitPhysics()
			{
				gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
				if (!gFoundation) throw("PxCreateFoundation failed!");

				gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);

				PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
				sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
				gDispatcher = PxDefaultCpuDispatcherCreate(2);
				sceneDesc.cpuDispatcher = gDispatcher;
				sceneDesc.filterShader = PxDefaultSimulationFilterShader;

				enableLinearCCD = false;
				enableSpeculativeCCD = false;

				const CCDAlgorithm ccd = CCDAlgorithm::LINEAR_CCD;

				if (ccd == LINEAR_CCD)
				{
					enableLinearCCD = true;
					sceneDesc.filterShader = ccdFilterShader;
					sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;

					gScene = gPhysics->createScene(sceneDesc);

					std::cout << "Using linear CCD" << std::endl;
				}
				else if (ccd == SPECULATIVE_CCD)
				{
					enableSpeculativeCCD = true;

					gScene = gPhysics->createScene(sceneDesc);

					std::cout << "Using speculative/angular CCD" << std::endl;

				}
				else if (ccd == FULL_CCD)
				{
					enableLinearCCD = true;
					enableSpeculativeCCD = true;
					sceneDesc.filterShader = ccdFilterShader;
					sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;

					gScene = gPhysics->createScene(sceneDesc);

					std::cout << "Using full CCD" << std::endl;
				}
				else if (ccd == NO_CCD)
				{
					gScene = gPhysics->createScene(sceneDesc);

					std::cout << "Using no CCD" << std::endl;
				}

				gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

				//

		//		PxReal stackZ=10.0f;
		//		PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
		//		gScene->addActor(*groundPlane);
	//			for (PxU32 i = 0; i < 5; i++)
	//				CreateStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 1.0f);
	//			CreateDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));

	//			PxRigidActor* cube=CreateDynamic(0.5f);

			//	std::cout << "Using linear CCD" << std::endl;
				std::cout << "Initting physx" << std::endl;
		
			
			}

		public:

			PxDefaultAllocator		gAllocator;
			PxDefaultErrorCallback	gErrorCallback;
			PxFoundation*			gFoundation;
			PxPhysics*				gPhysics;
			PxDefaultCpuDispatcher* gDispatcher;
			PxScene*				gScene;
			PxMaterial*				gMaterial;

			bool					enableLinearCCD;
			bool					enableSpeculativeCCD;

			std::vector<PxRigidActor*> Actors;


		public:

			// -----------------------------------------------------------------
			
			PxRigidDynamic* CreateDynamic(const PxVec3 &halfExtent)
			{
				PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent.x, halfExtent.y, halfExtent.z), *gMaterial);
				PxTransform localTm(PxVec3(0, 0, 0));
				PxRigidDynamic* body = gPhysics->createRigidDynamic(localTm);
				if (shape)
					body->attachShape(*shape);
				gScene->addActor(*body);
				if (enableLinearCCD)
					body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
				if (enableSpeculativeCCD)
					body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);
				shape->release();
				Actors.emplace_back(body);
				return body;
			}

			// -----------------------------------------------------------------
			// Setup common cooking params

			void setupCommonCookingParams(PxCookingParams& params, bool skipMeshCleanup, bool skipEdgeData)
			{
				// we suppress the triangle mesh remap table computation to gain some speed, as we will not need it
				// in this snippet
	
				params.suppressTriangleMeshRemapTable = true;

				// If DISABLE_CLEAN_MESH is set, the mesh is not cleaned during the cooking. The input mesh must be valid.
				// The following conditions are true for a valid triangle mesh :
				//  1. There are no duplicate vertices(within specified vertexWeldTolerance.See PxCookingParams::meshWeldTolerance)
				//  2. There are no large triangles(within specified PxTolerancesScale.)
				// It is recommended to run a separate validation check in debug/checked builds, see below.

				if (!skipMeshCleanup)
					params.meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);
				else
					params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;

				// If eDISABLE_ACTIVE_EDGES_PRECOMPUTE is set, the cooking does not compute the active (convex) edges, and instead
				// marks all edges as active. This makes cooking faster but can slow down contact generation. This flag may change
				// the collision behavior, as all edges of the triangle mesh will now be considered active.
				if (!skipEdgeData)
					params.meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE);
				else
					params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
			}

			// -----------------------------------------------------------------
			
			void CreateBVHFromMesh(const vml::meshes::Mesh3d* mesh)
			{
				if (!mesh)
					return;

				// get data from mesh

				PxU32 numVertices  = mesh->GetVertexCount();
				PxU32 numTriangles = mesh->GetSurfaceCount();

				PxVec3* vertices = new PxVec3[numVertices];
				PxU32* indices   = new PxU32[numTriangles * 3];

				const std::vector<float>& v        = mesh->GetVertexArray();
				const std::vector<unsigned int>& s = mesh->GetSurfaceIndices();

				PxU32 currentIdx;

				currentIdx = 0;

				for (size_t i = 0; i < numVertices; ++i)
				{
					size_t j = i * 4;

					vertices[currentIdx++]= PxVec3(v[j], v[j + 1], v[j + 2]);
				}

				currentIdx=0;

				for (size_t i = 0; i < numTriangles; ++i)
				{
					size_t j = i * 3;

					indices[currentIdx++] = s[j    ];
					indices[currentIdx++] = s[j + 1];
					indices[currentIdx++] = s[j + 2];
				}
			
				// configure bvh

				bool skipMeshCleanup      = false;
				bool skipEdgeData	      = false;
				bool cookingPerformance   = true;
				bool meshSizePerfTradeoff = false;

				createBV33TriangleMesh(numVertices, vertices, 
					                   numTriangles, indices, 
					                   skipMeshCleanup, 
									   skipEdgeData, 
									   cookingPerformance, 
									   meshSizePerfTradeoff);

				// release memory

				vml::SafeDeleteArray(vertices);
				vml::SafeDeleteArray(indices);
			}

			// -----------------------------------------------------------------
			// Creates a triangle mesh using BVH33 midphase with different settings.

			void createBV33TriangleMesh(PxU32 numVertices, const PxVec3* vertices, PxU32 numTriangles, const PxU32* indices,
										bool skipMeshCleanup, bool skipEdgeData, bool cookingPerformance, bool meshSizePerfTradeoff)
			{

				PxTriangleMeshDesc meshDesc;

				meshDesc.points.count     = numVertices;
				meshDesc.points.data      = vertices;
				meshDesc.points.stride    = sizeof(PxVec3);
				meshDesc.triangles.count  = numTriangles;
				meshDesc.triangles.data   = indices;
				meshDesc.triangles.stride = 3 * sizeof(PxU32);

				// check validity of mesDesc data

				PX_ASSERT(meshDesc.isValid());

				PxTolerancesScale scale;
				PxCookingParams params(scale);

				// Create BVH33 midphase

				params.midphaseDesc = PxMeshMidPhase::eBVH33;

				// setup common cooking params
				setupCommonCookingParams(params, skipMeshCleanup, skipEdgeData);

				// The COOKING_PERFORMANCE flag for BVH33 midphase enables a fast cooking path at the expense of somewhat lower quality BVH construction.

				if (cookingPerformance)
					params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eCOOKING_PERFORMANCE;
				else
					params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;

				// If meshSizePerfTradeoff is set to true, smaller mesh cooked mesh is produced. The mesh size/performance trade-off
				// is controlled by setting the meshSizePerformanceTradeOff from 0.0f (smaller mesh) to 1.0f (larger mesh).
				// else will use the default value
		
				if (meshSizePerfTradeoff)
					params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 0.0f;
				else
					params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 0.55f;

				//	#if defined(PX_CHECKED) || defined(PX_DEBUG)

				// If DISABLE_CLEAN_MESH is set, the mesh is not cleaned during the cooking.
				// We should check the validity of provided triangles in debug/checked builds though.

				if (skipMeshCleanup)
					PX_ASSERT(PxValidateTriangleMesh(params, meshDesc));

				//	#endif // DEBUG

				PxTriangleMesh* triMesh = NULL;
				PxU32 meshSize = 0;
				bool result;

				// The cooked mesh may either be saved to a stream for later loading, or inserted directly into PxPhysics.

				PxDefaultMemoryOutputStream outBuffer;
				
				result=PxCookTriangleMesh(params, meshDesc, outBuffer);
				
				if (!result)
					vml::os::Message::Error("PhysX : ","Error cooking mesh");

				PxDefaultMemoryInputData stream(outBuffer.getData(), outBuffer.getSize());
				triMesh = gPhysics->createTriangleMesh(stream);
				meshSize = outBuffer.getSize();

				// add triangle mesh to physx pipeline

				PxTransform transform(PxVec3(0.0f, 0.0f, 0.0f));
				PxRigidStatic* rigidStatic = gPhysics->createRigidStatic(transform);
			
				if (!rigidStatic)
				{	vml::os::Message::Error("PhysX : ","Error creating static object");
					return;	}
				
				// create shape to attach to actor

				PxShape* shape = gPhysics->createShape(PxTriangleMeshGeometry(triMesh), *gPhysics->createMaterial(0.5f, 0.5f, 0.1f));

				if (!shape)
				{	vml::os::Message::Error("PhysX : ","Error creating shape");
					return;	}

				if (shape)
					rigidStatic->attachShape(*shape);
				
				// cook the bvh
				
				PxBVH* bvh = PxRigidActorExt::createBVHFromActor(*gPhysics, *rigidStatic);

				if (!bvh)
				{	vml::os::Message::Error("PhysX : ","Error creating bvh");
					return;	}

				// add the actor to the scene and provide the bvh structure
	
				result=gScene->addActor(*rigidStatic,bvh);

				if (!result)
				{	vml::os::Message::Error("PhysX : ","Error creating actor for bvh");
					return;	}

				// bvh can be released at this point, the precomputed BVH structure was copied to the SDK pruners.
				
				bvh->release();

				// release shape

				shape->release();

				// add actor to actors list
			
				Actors.emplace_back(rigidStatic);

				// reòease triangle mesh

				triMesh->release();

				// Print the elapsed time for comparison

				std::cout << "\t Create triangle mesh with " << numTriangles << " triangles: \n";
				cookingPerformance ? std::cout << "\t\t Cooking performance on\n" : std::cout << "\t\t Cooking performance off\n";
				!skipEdgeData ? std::cout << "\t\t Precompute edge data on\n" : std::cout << "\t\t Precompute edge data off\n";
				!skipMeshCleanup ? std::cout << "\t\t Mesh cleanup on\n" : std::cout << "\t\t Mesh cleanup off\n";
				std::cout << "\t\t Mesh size/performance trade-off: " << double(params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff) << "\n";
				std::cout << "\t Mesh size: " << meshSize << "\n";

				std::cout << std::endl;

			}

			// -----------------------------------------------------------------
			
			void Clear()
			{
				std::cout << "Cleaning physx" << std::endl;

				for (size_t i = 0; i < Actors.size(); i++) 
				{
					gScene->removeActor(*Actors[i]);
					Actors[i]->release();

					std::cout << "removeing actor " << i << std::endl;
				}

				Actors.clear();
				
			}

			// -----------------------------------------------------------------

			void StepPhysics()
			{
				gScene->simulate(1.0f / 60.0f);
				gScene->fetchResults(true);
			}

			// -----------------------------------------------------------------

			void ReleaseAll()
			{
				if (gScene) PX_RELEASE(gScene);
				if (gDispatcher) PX_RELEASE(gDispatcher);
				if (gPhysics) PX_RELEASE(gPhysics);
				if (gFoundation) PX_RELEASE(gFoundation);

				std::cout << "Closing physx" << std::endl;
			}

			// -----------------------------------------------------------------

			void RenderCallback(vml::views::View* view)
			{

				//stepPhysics();

				PxScene* scene;

				PxGetPhysics().getScenes(&scene, 1);

				PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);

			//	std::cout << nbActors << std::endl;

				if (nbActors)
				{
					std::vector<PxRigidActor*> actors(nbActors);

					scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);

					RenderActors(view, &actors[0], static_cast<PxU32>(actors.size()), true);
				}

			}

			// -----------------------------------------------------------------

			void RenderGeometry(vml::views::View* view, const PxGeometry& geom, const float* matrix)
			{

				switch (geom.getType())
				{
					case physx::PxGeometryType::eBOX:
					{
						const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);

						glm::mat4 mymatrix;

						float* mptr = glm::value_ptr(mymatrix);

						float scx = boxGeom.halfExtents.x * 2;
						float scy = boxGeom.halfExtents.y * 2;
						float scz = boxGeom.halfExtents.z * 2;

						// scale parent matrix

						mptr[0] = matrix[0] * scx;
						mptr[1] = matrix[1] * scx;
						mptr[2] = matrix[2] * scx;
						mptr[3] = matrix[3] * scx;

						mptr[4] = matrix[4] * scy;
						mptr[5] = matrix[5] * scy;
						mptr[6] = matrix[6] * scy;
						mptr[7] = matrix[7] * scy;

						mptr[8] = matrix[8] * scz;
						mptr[9] = matrix[9] * scz;
						mptr[10] = matrix[10] * scz;
						mptr[11] = matrix[11] * scz;

						mptr[12] = matrix[12];
						mptr[13] = matrix[13];
						mptr[14] = matrix[14];
						mptr[15] = matrix[15];

						vml::DebugRender->DrawWireCube(view, mymatrix, vml::colors::Yellow);

					}

					break;

					case physx::PxGeometryType::eSPHERE:
					{
						const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(geom);

						glm::mat4 mymatrix;

						float* mptr = glm::value_ptr(mymatrix);

						float scx = sphereGeom.radius * 2;
						float scy = sphereGeom.radius * 2;
						float scz = sphereGeom.radius * 2;

						// scale parent matrix

						mptr[0] = matrix[0] * scx;
						mptr[1] = matrix[1] * scx;
						mptr[2] = matrix[2] * scx;
						mptr[3] = matrix[3] * scx;

						mptr[4] = matrix[4] * scy;
						mptr[5] = matrix[5] * scy;
						mptr[6] = matrix[6] * scy;
						mptr[7] = matrix[7] * scy;

						mptr[8] = matrix[8] * scz;
						mptr[9] = matrix[9] * scz;
						mptr[10] = matrix[10] * scz;
						mptr[11] = matrix[11] * scz;

						mptr[12] = matrix[12];
						mptr[13] = matrix[13];
						mptr[14] = matrix[14];
						mptr[15] = matrix[15];

						vml::DebugRender->DrawSphere(view, mymatrix, vml::colors::Red);
					}
					break;

					case physx::PxGeometryType::eCAPSULE:
					{
						std::cout << "tianglemesh 4" << std::endl;

						/*
						const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(geom);
						const PxF32 radius = capsuleGeom.radius;
						const PxF32 halfHeight = capsuleGeom.halfHeight;

						//Sphere
						glPushMatrix();
						glTranslatef(halfHeight, 0.0f, 0.0f);
						glScalef(radius, radius, radius);
				//		glutSolidSphere(1, 10, 10);
						glPopMatrix();

						//Sphere
						glPushMatrix();
						glTranslatef(-halfHeight, 0.0f, 0.0f);
						glScalef(radius, radius, radius);
					//	glutSolidSphere(1, 10, 10);
						glPopMatrix();

						//Cylinder
						glPushMatrix();
						glTranslatef(-halfHeight, 0.0f, 0.0f);
						glScalef(2.0f * halfHeight, radius, radius);
						glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
						glEnableClientState(GL_VERTEX_ARRAY);
						glEnableClientState(GL_NORMAL_ARRAY);
						glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), gCylinderData);
						glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), gCylinderData + 3);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 13 * 2);
						glDisableClientState(GL_VERTEX_ARRAY);
						glDisableClientState(GL_NORMAL_ARRAY);
						glPopMatrix();
					*/
					}
					break;

					case physx::PxGeometryType::eCONVEXMESH:
					{
						std::cout << "tianglemesh 3" << std::endl;

						/*
						const PxConvexMeshGeometry& convexGeom = static_cast<const PxConvexMeshGeometry&>(geom);

						//Compute triangles for each polygon.
						const PxVec3& scale = convexGeom.scale.scale;
						PxConvexMesh* mesh = convexGeom.convexMesh;
						const PxU32 nbPolys = mesh->getNbPolygons();
						const PxU8* polygons = mesh->getIndexBuffer();
						const PxVec3* verts = mesh->getVertices();
						PxU32 nbVerts = mesh->getNbVertices();
						PX_UNUSED(nbVerts);

						prepareVertexBuffer();

						PxU32 numTotalTriangles = 0;
						for (PxU32 i = 0; i < nbPolys; i++)
						{
							PxHullPolygon data;
							mesh->getPolygonData(i, data);

							const PxU32 nbTris = PxU32(data.mNbVerts - 2);
							const PxU8 vref0 = polygons[data.mIndexBase + 0];
							PX_ASSERT(vref0 < nbVerts);
							for (PxU32 j = 0; j < nbTris; j++)
							{
								const PxU32 vref1 = polygons[data.mIndexBase + 0 + j + 1];
								const PxU32 vref2 = polygons[data.mIndexBase + 0 + j + 2];

								//generate face normal:
								PxVec3 e0 = verts[vref1] - verts[vref0];
								PxVec3 e1 = verts[vref2] - verts[vref0];

								PX_ASSERT(vref1 < nbVerts);
								PX_ASSERT(vref2 < nbVerts);

								PxVec3 fnormal = e0.cross(e1);
								fnormal.normalize();

								pushVertex(verts[vref0], verts[vref1], verts[vref2], fnormal);
								numTotalTriangles++;
							}
						}
						glPushMatrix();
						glScalef(scale.x, scale.y, scale.z);
						glEnableClientState(GL_NORMAL_ARRAY);
						glEnableClientState(GL_VERTEX_ARRAY);
						const PxVec3* vertexBuffer = getVertexBuffer();
						glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), vertexBuffer);
						glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), vertexBuffer + 1);
						glDrawArrays(GL_TRIANGLES, 0, int(numTotalTriangles * 3));
						glPopMatrix();
					*/
					}
					break;

					case physx::PxGeometryType::eTRIANGLEMESH:
					{
												
						const PxTriangleMeshGeometry& triGeom = static_cast<const PxTriangleMeshGeometry&>(geom);

						const PxTriangleMesh& mesh = *triGeom.triangleMesh;
						const PxVec3 scale = triGeom.scale.scale;

						const PxU32 triangleCount = mesh.getNbTriangles();
						const PxU32 has16BitIndices = mesh.getTriangleMeshFlags() & PxTriangleMeshFlag::e16_BIT_INDICES;
						const void* indexBuffer = mesh.getTriangles();

						const PxVec3* vertices = mesh.getVertices();
						
						const PxU32* intIndices = reinterpret_cast<const PxU32*>(indexBuffer);
						const PxU16* shortIndices = reinterpret_cast<const PxU16*>(indexBuffer);
						
						PxU32 numTotalTriangles = 0;
						
						for (PxU32 i = 0; i < triangleCount; ++i)
						{
							
							PxU32 vref0, vref1, vref2;
							
							if (has16BitIndices)
							{
								vref0 = *shortIndices++;
								vref1 = *shortIndices++;
								vref2 = *shortIndices++;
							}
							else
							{
								vref0 = *intIndices++;
								vref1 = *intIndices++;
								vref2 = *intIndices++;
							}

							const PxVec3& v0 = vertices[vref0];
							const PxVec3& v1 = vertices[vref1];
							const PxVec3& v2 = vertices[vref2];

							vml::DebugRender->DrawTriangle(view, 
														   glm::vec3(v0.x, v0.y, v0.z),
														   glm::vec3(v1.x, v1.y, v1.z),
														   glm::vec3(v2.x, v2.y, v2.z), 
														   vml::colors::Grey,
														   true, true);

						}

					}

					break;

					case physx::PxGeometryType::eTETRAHEDRONMESH:
					{
						std::cout << "tianglemesh 1" << std::endl;

						/*
							const int tetFaces[4][3] = { {0,2,1}, {0,1,3}, {0,3,2}, {1,2,3} };

							const PxTetrahedronMeshGeometry& tetGeom = static_cast<const PxTetrahedronMeshGeometry&>(geom);

							const PxTetrahedronMesh& mesh = *tetGeom.tetrahedronMesh;

							//Get the deformed vertices
							const PxVec3* vertices = mesh.getVertices();
							const PxU32 tetCount = mesh.getNbTetrahedrons();
							const PxU32 has16BitIndices = mesh.getTetrahedronMeshFlags() & PxTetrahedronMeshFlag::e16_BIT_INDICES;
							const void* indexBuffer = mesh.getTetrahedrons();

							prepareVertexBuffer();

							const PxU32* intIndices = reinterpret_cast<const PxU32*>(indexBuffer);
							const PxU16* shortIndices = reinterpret_cast<const PxU16*>(indexBuffer);
							PxU32 numTotalTriangles = 0;
							for (PxU32 i = 0; i < tetCount; ++i)
							{
								PxU32 vref[4];
								if (has16BitIndices)
								{
									vref[0] = *shortIndices++;
									vref[1] = *shortIndices++;
									vref[2] = *shortIndices++;
									vref[3] = *shortIndices++;
								}
								else
								{
									vref[0] = *intIndices++;
									vref[1] = *intIndices++;
									vref[2] = *intIndices++;
									vref[3] = *intIndices++;
								}

								for (PxU32 j = 0; j < 4; ++j)
								{
									const PxVec3& v0 = vertices[vref[tetFaces[j][0]]];
									const PxVec3& v1 = vertices[vref[tetFaces[j][1]]];
									const PxVec3& v2 = vertices[vref[tetFaces[j][2]]];

									PxVec3 fnormal = (v1 - v0).cross(v2 - v0);
									fnormal.normalize();

									pushVertex(v0, v1, v2, fnormal);
									numTotalTriangles++;
								}
							}
							glPushMatrix();
							glScalef(1.0f, 1.0f, 1.0f);
							glEnableClientState(GL_NORMAL_ARRAY);
							glEnableClientState(GL_VERTEX_ARRAY);
							const PxVec3* vertexBuffer = getVertexBuffer();
							glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), vertexBuffer);
							glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), vertexBuffer + 1);
							glDrawArrays(GL_TRIANGLES, 0, int(numTotalTriangles * 3));
							glDisableClientState(GL_VERTEX_ARRAY);
							glDisableClientState(GL_NORMAL_ARRAY);
							glPopMatrix();
						*/
					}
					break;

					default:
				
					break;
				
				}
			}

			// -----------------------------------------------------------------

			void RenderActors(vml::views::View* view,
							  physx::PxRigidActor** actors,
							  const physx::PxU32 numActors,
							  bool shadows = false,
							  const physx::PxVec3& color = physx::PxVec3(0.0f, 0.75f, 0.0f),
							  TriggerRender* cb = NULL,
							  bool changeColorForSleepingActors = true)
			{

				PxShape* shapes[MAX_NUM_ACTOR_SHAPES];

				for (PxU32 i = 0; i < numActors; i++)
				{

					const PxU32 nbShapes = actors[i]->getNbShapes();

					PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);

					actors[i]->getShapes(shapes, nbShapes);

					bool sleeping;

					if (changeColorForSleepingActors)
						sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;
					else
						sleeping = false;

					for (PxU32 j = 0; j < nbShapes; j++)
					{

						const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
						const PxGeometry& geom = shapes[j]->getGeometry();
						const bool isTrigger = cb ? cb->isTrigger(shapes[j]) : shapes[j]->getFlags() & PxShapeFlag::eTRIGGER_SHAPE;

						if (isTrigger)
						{
							//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						}

						// render object

						//glPushMatrix();
						//glMultMatrixf(&shapePose.column0.x);

						if (sleeping)
						{
							//	const PxVec3 darkColor = color * 0.25f;
							//	glColor4f(darkColor.x, darkColor.y, darkColor.z, 1.0f);
						}
						else
						{
							//	glColor4f(color.x, color.y, color.z, 1.0f);
						}

						RenderGeometry(view, geom, &shapePose.column0.x);

					}

				}

			}

			
			// -----------------------------------------------------------------
			// ctor / dtor

			NvidiaPhysXEngine()
			{
				gFoundation = nullptr;
				gPhysics    = nullptr;
				gDispatcher = nullptr;
				gScene      = nullptr;
				gMaterial   = nullptr;

				enableLinearCCD      = false;
				enableSpeculativeCCD = false;

				InitPhysics();

			}

			~NvidiaPhysXEngine()
			{
				ReleaseAll();
			}

	};
}
