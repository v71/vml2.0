#pragma once

class CilindricalEmitters16 : public ParticleNodeBase
{
	private:

		inline constexpr static unsigned int Indices[] = {
															 0, 16,  1,
															 1, 16, 17,
															 1, 17,  2,
															 2, 17, 18,
															 2, 18,  3,
															 3, 18, 19,
															 3, 19,  4,
															 4, 19, 20,
															 4, 20,  5,
															 5, 20, 21,
															 5, 21,  6,
															 6, 21, 22,
															 6, 22,  7,
															 7, 22, 23,
															 7, 23,  8,
															 8, 23, 24,
															 8, 24,  9,
															 9, 24, 25,
															 9, 25, 10,
															10, 25, 26,
															10, 26, 11,
															11, 26, 27,
															11, 27, 12,
															12, 27, 28,
															12, 28, 13,
															13, 28, 29,
															13, 29, 14,
															14, 29, 30,
															14, 30, 15,
															15, 30, 31,
															31,  0, 15,
															16,  0, 31
		};

		glm::vec3 StartingEmitters[32];
		glm::vec3 TransformedStartingEmitters[32];
		glm::vec3 Directions[16];
		float	  Height;
		float	  InitialStartingRadius;
		float	  InitialFinalRadius;
		float	  StartingRadius;
		float	  FinalRadius;

	public:

		// ---------------------------------------------------------------
		// setters

		void SetStartingRadius(float val)
		{
			StartingRadius = val;
		}

		void SetFinalRadius(float val)
		{
			FinalRadius = val;
		}

		void ResetStartingRadius()
		{
			StartingRadius = InitialStartingRadius;
		}

		void ResetFinalgRadius()
		{
			StartingRadius = InitialStartingRadius;
		}

		void Reset()
		{
			StartingRadius = InitialStartingRadius;
			ParticleNodeBase::Reset();
		}

		// ---------------------------------------------------------------
		// getters

		float GetStartingRadius() const
		{
			return StartingRadius;
		}

		float GetFinalRadius() const
		{
			return FinalRadius;
		}

		int GetEmittersCount() const
		{
			return 16;
		}

		const glm::vec3* GetTransformedStartingEmitters()
		{
			return TransformedStartingEmitters;
		}

		const unsigned int* GetIndices()
		{
			return Indices;
		}

		// ---------------------------------------------------------------
		//

		void ComputeRadii()
		{
			float deg;
			float rad;
			float dr;

			// compute angle increment

			dr = 360.0f / (float)16;

			// first ring

			deg = 0;

			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 0] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 1] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 2] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 3] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 4] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 5] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 6] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 7] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 8] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[ 9] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[10] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[11] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[12] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[13] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[14] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[15] = glm::vec3(StartingRadius * cos(rad), 0, StartingRadius * sin(rad)); deg += dr;

			// second ring

			deg = 0;

			rad = deg * vml::math::DEGTORAD; StartingEmitters[16] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[17] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[18] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[19] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[20] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[21] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[22] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[23] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[24] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[25] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[26] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[27] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[28] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[29] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[30] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;
			rad = deg * vml::math::DEGTORAD; StartingEmitters[31] = glm::vec3(FinalRadius * cos(rad), Height, FinalRadius * sin(rad)); deg += dr;

		}

		// ---------------------------------------------------------------
		// add new particle to the pool

		void Add()
		{
			AddParticleToPool(TransformedStartingEmitters[ 0], Directions[ 0]);
			AddParticleToPool(TransformedStartingEmitters[ 1], Directions[ 1]);
			AddParticleToPool(TransformedStartingEmitters[ 2], Directions[ 2]);
			AddParticleToPool(TransformedStartingEmitters[ 3], Directions[ 3]);
			AddParticleToPool(TransformedStartingEmitters[ 4], Directions[ 4]);
			AddParticleToPool(TransformedStartingEmitters[ 5], Directions[ 5]);
			AddParticleToPool(TransformedStartingEmitters[ 6], Directions[ 6]);
			AddParticleToPool(TransformedStartingEmitters[ 7], Directions[ 7]);
			AddParticleToPool(TransformedStartingEmitters[ 8], Directions[ 8]);
			AddParticleToPool(TransformedStartingEmitters[ 9], Directions[ 9]);
			AddParticleToPool(TransformedStartingEmitters[10], Directions[10]);
			AddParticleToPool(TransformedStartingEmitters[11], Directions[11]);
			AddParticleToPool(TransformedStartingEmitters[12], Directions[12]);
			AddParticleToPool(TransformedStartingEmitters[13], Directions[13]);
			AddParticleToPool(TransformedStartingEmitters[14], Directions[14]);
			AddParticleToPool(TransformedStartingEmitters[15], Directions[15]);
		}

		// ---------------------------------------------------------------
		//

		void Update(vml::views::View* view, float dt)
		{
			// compute spherical coordiantes matrix

			ComputeMatrix();

			// get matrix pointer

			float* m = glm::value_ptr(M);

			// transform fist ring points

			TransformedStartingEmitters[ 0].x = m[0] * StartingEmitters[ 0].x + m[4] * StartingEmitters[ 0].y + m[ 8] * StartingEmitters[ 0].z + m[12];
			TransformedStartingEmitters[ 0].y = m[1] * StartingEmitters[ 0].x + m[5] * StartingEmitters[ 0].y + m[ 9] * StartingEmitters[ 0].z + m[13];
			TransformedStartingEmitters[ 0].z = m[2] * StartingEmitters[ 0].x + m[6] * StartingEmitters[ 0].y + m[10] * StartingEmitters[ 0].z + m[14];
			TransformedStartingEmitters[ 1].x = m[0] * StartingEmitters[ 1].x + m[4] * StartingEmitters[ 1].y + m[ 8] * StartingEmitters[ 1].z + m[12];
			TransformedStartingEmitters[ 1].y = m[1] * StartingEmitters[ 1].x + m[5] * StartingEmitters[ 1].y + m[ 9] * StartingEmitters[ 1].z + m[13];
			TransformedStartingEmitters[ 1].z = m[2] * StartingEmitters[ 1].x + m[6] * StartingEmitters[ 1].y + m[10] * StartingEmitters[ 1].z + m[14];
			TransformedStartingEmitters[ 2].x = m[0] * StartingEmitters[ 2].x + m[4] * StartingEmitters[ 2].y + m[ 8] * StartingEmitters[ 2].z + m[12];
			TransformedStartingEmitters[ 2].y = m[1] * StartingEmitters[ 2].x + m[5] * StartingEmitters[ 2].y + m[ 9] * StartingEmitters[ 2].z + m[13];
			TransformedStartingEmitters[ 2].z = m[2] * StartingEmitters[ 2].x + m[6] * StartingEmitters[ 2].y + m[10] * StartingEmitters[ 2].z + m[14];
			TransformedStartingEmitters[ 3].x = m[0] * StartingEmitters[ 3].x + m[4] * StartingEmitters[ 3].y + m[ 8] * StartingEmitters[ 3].z + m[12];
			TransformedStartingEmitters[ 3].y = m[1] * StartingEmitters[ 3].x + m[5] * StartingEmitters[ 3].y + m[ 9] * StartingEmitters[ 3].z + m[13];
			TransformedStartingEmitters[ 3].z = m[2] * StartingEmitters[ 3].x + m[6] * StartingEmitters[ 3].y + m[10] * StartingEmitters[ 3].z + m[14];
			TransformedStartingEmitters[ 4].x = m[0] * StartingEmitters[ 4].x + m[4] * StartingEmitters[ 4].y + m[ 8] * StartingEmitters[ 4].z + m[12];
			TransformedStartingEmitters[ 4].y = m[1] * StartingEmitters[ 4].x + m[5] * StartingEmitters[ 4].y + m[ 9] * StartingEmitters[ 4].z + m[13];
			TransformedStartingEmitters[ 4].z = m[2] * StartingEmitters[ 4].x + m[6] * StartingEmitters[ 4].y + m[10] * StartingEmitters[ 4].z + m[14];
			TransformedStartingEmitters[ 5].x = m[0] * StartingEmitters[ 5].x + m[4] * StartingEmitters[ 5].y + m[ 8] * StartingEmitters[ 5].z + m[12];
			TransformedStartingEmitters[ 5].y = m[1] * StartingEmitters[ 5].x + m[5] * StartingEmitters[ 5].y + m[ 9] * StartingEmitters[ 5].z + m[13];
			TransformedStartingEmitters[ 5].z = m[2] * StartingEmitters[ 5].x + m[6] * StartingEmitters[ 5].y + m[10] * StartingEmitters[ 5].z + m[14];
			TransformedStartingEmitters[ 6].x = m[0] * StartingEmitters[ 6].x + m[4] * StartingEmitters[ 6].y + m[ 8] * StartingEmitters[ 6].z + m[12];
			TransformedStartingEmitters[ 6].y = m[1] * StartingEmitters[ 6].x + m[5] * StartingEmitters[ 6].y + m[ 9] * StartingEmitters[ 6].z + m[13];
			TransformedStartingEmitters[ 6].z = m[2] * StartingEmitters[ 6].x + m[6] * StartingEmitters[ 6].y + m[10] * StartingEmitters[ 6].z + m[14];
			TransformedStartingEmitters[ 7].x = m[0] * StartingEmitters[ 7].x + m[4] * StartingEmitters[ 7].y + m[ 8] * StartingEmitters[ 7].z + m[12];
			TransformedStartingEmitters[ 7].y = m[1] * StartingEmitters[ 7].x + m[5] * StartingEmitters[ 7].y + m[ 9] * StartingEmitters[ 7].z + m[13];
			TransformedStartingEmitters[ 7].z = m[2] * StartingEmitters[ 7].x + m[6] * StartingEmitters[ 7].y + m[10] * StartingEmitters[ 7].z + m[14];
			TransformedStartingEmitters[ 8].x = m[0] * StartingEmitters[ 8].x + m[4] * StartingEmitters[ 8].y + m[ 8] * StartingEmitters[ 8].z + m[12];
			TransformedStartingEmitters[ 8].y = m[1] * StartingEmitters[ 8].x + m[5] * StartingEmitters[ 8].y + m[ 9] * StartingEmitters[ 8].z + m[13];
			TransformedStartingEmitters[ 8].z = m[2] * StartingEmitters[ 8].x + m[6] * StartingEmitters[ 8].y + m[10] * StartingEmitters[ 8].z + m[14];
			TransformedStartingEmitters[ 9].x = m[0] * StartingEmitters[ 9].x + m[4] * StartingEmitters[ 9].y + m[ 8] * StartingEmitters[ 9].z + m[12];
			TransformedStartingEmitters[ 9].y = m[1] * StartingEmitters[ 9].x + m[5] * StartingEmitters[ 9].y + m[ 9] * StartingEmitters[ 9].z + m[13];
			TransformedStartingEmitters[ 9].z = m[2] * StartingEmitters[ 9].x + m[6] * StartingEmitters[ 9].y + m[10] * StartingEmitters[ 9].z + m[14];
			TransformedStartingEmitters[10].x = m[0] * StartingEmitters[10].x + m[4] * StartingEmitters[10].y + m[ 8] * StartingEmitters[10].z + m[12];
			TransformedStartingEmitters[10].y = m[1] * StartingEmitters[10].x + m[5] * StartingEmitters[10].y + m[ 9] * StartingEmitters[10].z + m[13];
			TransformedStartingEmitters[10].z = m[2] * StartingEmitters[10].x + m[6] * StartingEmitters[10].y + m[10] * StartingEmitters[10].z + m[14];
			TransformedStartingEmitters[11].x = m[0] * StartingEmitters[11].x + m[4] * StartingEmitters[11].y + m[ 8] * StartingEmitters[11].z + m[12];
			TransformedStartingEmitters[11].y = m[1] * StartingEmitters[11].x + m[5] * StartingEmitters[11].y + m[ 9] * StartingEmitters[11].z + m[13];
			TransformedStartingEmitters[11].z = m[2] * StartingEmitters[11].x + m[6] * StartingEmitters[11].y + m[10] * StartingEmitters[11].z + m[14];
			TransformedStartingEmitters[12].x = m[0] * StartingEmitters[12].x + m[4] * StartingEmitters[12].y + m[ 8] * StartingEmitters[12].z + m[12];
			TransformedStartingEmitters[12].y = m[1] * StartingEmitters[12].x + m[5] * StartingEmitters[12].y + m[ 9] * StartingEmitters[12].z + m[13];
			TransformedStartingEmitters[12].z = m[2] * StartingEmitters[12].x + m[6] * StartingEmitters[12].y + m[10] * StartingEmitters[12].z + m[14];
			TransformedStartingEmitters[13].x = m[0] * StartingEmitters[13].x + m[4] * StartingEmitters[13].y + m[ 8] * StartingEmitters[13].z + m[12];
			TransformedStartingEmitters[13].y = m[1] * StartingEmitters[13].x + m[5] * StartingEmitters[13].y + m[ 9] * StartingEmitters[13].z + m[13];
			TransformedStartingEmitters[13].z = m[2] * StartingEmitters[13].x + m[6] * StartingEmitters[13].y + m[10] * StartingEmitters[13].z + m[14];
			TransformedStartingEmitters[14].x = m[0] * StartingEmitters[14].x + m[4] * StartingEmitters[14].y + m[ 8] * StartingEmitters[14].z + m[12];
			TransformedStartingEmitters[14].y = m[1] * StartingEmitters[14].x + m[5] * StartingEmitters[14].y + m[ 9] * StartingEmitters[14].z + m[13];
			TransformedStartingEmitters[14].z = m[2] * StartingEmitters[14].x + m[6] * StartingEmitters[14].y + m[10] * StartingEmitters[14].z + m[14];
			TransformedStartingEmitters[15].x = m[0] * StartingEmitters[15].x + m[4] * StartingEmitters[15].y + m[ 8] * StartingEmitters[15].z + m[12];
			TransformedStartingEmitters[15].y = m[1] * StartingEmitters[15].x + m[5] * StartingEmitters[15].y + m[ 9] * StartingEmitters[15].z + m[13];
			TransformedStartingEmitters[15].z = m[2] * StartingEmitters[15].x + m[6] * StartingEmitters[15].y + m[10] * StartingEmitters[15].z + m[14];

			// transform second ring points

			TransformedStartingEmitters[16].x = m[0] * StartingEmitters[16].x + m[4] * StartingEmitters[16].y + m[ 8] * StartingEmitters[16].z + m[12];
			TransformedStartingEmitters[16].y = m[1] * StartingEmitters[16].x + m[5] * StartingEmitters[16].y + m[ 9] * StartingEmitters[16].z + m[13];
			TransformedStartingEmitters[16].z = m[2] * StartingEmitters[16].x + m[6] * StartingEmitters[16].y + m[10] * StartingEmitters[16].z + m[14];
			TransformedStartingEmitters[17].x = m[0] * StartingEmitters[17].x + m[4] * StartingEmitters[17].y + m[ 8] * StartingEmitters[17].z + m[12];
			TransformedStartingEmitters[17].y = m[1] * StartingEmitters[17].x + m[5] * StartingEmitters[17].y + m[ 9] * StartingEmitters[17].z + m[13];
			TransformedStartingEmitters[17].z = m[2] * StartingEmitters[17].x + m[6] * StartingEmitters[17].y + m[10] * StartingEmitters[17].z + m[14];
			TransformedStartingEmitters[18].x = m[0] * StartingEmitters[18].x + m[4] * StartingEmitters[18].y + m[ 8] * StartingEmitters[18].z + m[12];
			TransformedStartingEmitters[18].y = m[1] * StartingEmitters[18].x + m[5] * StartingEmitters[18].y + m[ 9] * StartingEmitters[18].z + m[13];
			TransformedStartingEmitters[18].z = m[2] * StartingEmitters[18].x + m[6] * StartingEmitters[18].y + m[10] * StartingEmitters[18].z + m[14];
			TransformedStartingEmitters[19].x = m[0] * StartingEmitters[19].x + m[4] * StartingEmitters[19].y + m[ 8] * StartingEmitters[19].z + m[12];
			TransformedStartingEmitters[19].y = m[1] * StartingEmitters[19].x + m[5] * StartingEmitters[19].y + m[ 9] * StartingEmitters[19].z + m[13];
			TransformedStartingEmitters[19].z = m[2] * StartingEmitters[19].x + m[6] * StartingEmitters[19].y + m[10] * StartingEmitters[19].z + m[14];
			TransformedStartingEmitters[20].x = m[0] * StartingEmitters[20].x + m[4] * StartingEmitters[20].y + m[ 8] * StartingEmitters[20].z + m[12];
			TransformedStartingEmitters[20].y = m[1] * StartingEmitters[20].x + m[5] * StartingEmitters[20].y + m[ 9] * StartingEmitters[20].z + m[13];
			TransformedStartingEmitters[20].z = m[2] * StartingEmitters[20].x + m[6] * StartingEmitters[20].y + m[10] * StartingEmitters[20].z + m[14];
			TransformedStartingEmitters[21].x = m[0] * StartingEmitters[21].x + m[4] * StartingEmitters[21].y + m[ 8] * StartingEmitters[21].z + m[12];
			TransformedStartingEmitters[21].y = m[1] * StartingEmitters[21].x + m[5] * StartingEmitters[21].y + m[ 9] * StartingEmitters[21].z + m[13];
			TransformedStartingEmitters[21].z = m[2] * StartingEmitters[21].x + m[6] * StartingEmitters[21].y + m[10] * StartingEmitters[21].z + m[14];
			TransformedStartingEmitters[22].x = m[0] * StartingEmitters[22].x + m[4] * StartingEmitters[22].y + m[ 8] * StartingEmitters[22].z + m[12];
			TransformedStartingEmitters[22].y = m[1] * StartingEmitters[22].x + m[5] * StartingEmitters[22].y + m[ 9] * StartingEmitters[22].z + m[13];
			TransformedStartingEmitters[22].z = m[2] * StartingEmitters[22].x + m[6] * StartingEmitters[22].y + m[10] * StartingEmitters[22].z + m[14];
			TransformedStartingEmitters[23].x = m[0] * StartingEmitters[23].x + m[4] * StartingEmitters[23].y + m[ 8] * StartingEmitters[23].z + m[12];
			TransformedStartingEmitters[23].y = m[1] * StartingEmitters[23].x + m[5] * StartingEmitters[23].y + m[ 9] * StartingEmitters[23].z + m[13];
			TransformedStartingEmitters[23].z = m[2] * StartingEmitters[23].x + m[6] * StartingEmitters[23].y + m[10] * StartingEmitters[23].z + m[14];
			TransformedStartingEmitters[24].x = m[0] * StartingEmitters[24].x + m[4] * StartingEmitters[24].y + m[ 8] * StartingEmitters[24].z + m[12];
			TransformedStartingEmitters[24].y = m[1] * StartingEmitters[24].x + m[5] * StartingEmitters[24].y + m[ 9] * StartingEmitters[24].z + m[13];
			TransformedStartingEmitters[24].z = m[2] * StartingEmitters[24].x + m[6] * StartingEmitters[24].y + m[10] * StartingEmitters[24].z + m[14];
			TransformedStartingEmitters[25].x = m[0] * StartingEmitters[25].x + m[4] * StartingEmitters[25].y + m[ 8] * StartingEmitters[25].z + m[12];
			TransformedStartingEmitters[25].y = m[1] * StartingEmitters[25].x + m[5] * StartingEmitters[25].y + m[ 9] * StartingEmitters[25].z + m[13];
			TransformedStartingEmitters[25].z = m[2] * StartingEmitters[25].x + m[6] * StartingEmitters[25].y + m[10] * StartingEmitters[25].z + m[14];
			TransformedStartingEmitters[26].x = m[0] * StartingEmitters[26].x + m[4] * StartingEmitters[26].y + m[ 8] * StartingEmitters[26].z + m[12];
			TransformedStartingEmitters[26].y = m[1] * StartingEmitters[26].x + m[5] * StartingEmitters[26].y + m[ 9] * StartingEmitters[26].z + m[13];
			TransformedStartingEmitters[26].z = m[2] * StartingEmitters[26].x + m[6] * StartingEmitters[26].y + m[10] * StartingEmitters[26].z + m[14];
			TransformedStartingEmitters[27].x = m[0] * StartingEmitters[27].x + m[4] * StartingEmitters[27].y + m[ 8] * StartingEmitters[27].z + m[12];
			TransformedStartingEmitters[27].y = m[1] * StartingEmitters[27].x + m[5] * StartingEmitters[27].y + m[ 9] * StartingEmitters[27].z + m[13];
			TransformedStartingEmitters[27].z = m[2] * StartingEmitters[27].x + m[6] * StartingEmitters[27].y + m[10] * StartingEmitters[27].z + m[14];
			TransformedStartingEmitters[28].x = m[0] * StartingEmitters[28].x + m[4] * StartingEmitters[28].y + m[ 8] * StartingEmitters[28].z + m[12];
			TransformedStartingEmitters[28].y = m[1] * StartingEmitters[28].x + m[5] * StartingEmitters[28].y + m[ 9] * StartingEmitters[28].z + m[13];
			TransformedStartingEmitters[28].z = m[2] * StartingEmitters[28].x + m[6] * StartingEmitters[28].y + m[10] * StartingEmitters[28].z + m[14];
			TransformedStartingEmitters[29].x = m[0] * StartingEmitters[29].x + m[4] * StartingEmitters[29].y + m[ 8] * StartingEmitters[29].z + m[12];
			TransformedStartingEmitters[29].y = m[1] * StartingEmitters[29].x + m[5] * StartingEmitters[29].y + m[ 9] * StartingEmitters[29].z + m[13];
			TransformedStartingEmitters[29].z = m[2] * StartingEmitters[29].x + m[6] * StartingEmitters[29].y + m[10] * StartingEmitters[29].z + m[14];
			TransformedStartingEmitters[30].x = m[0] * StartingEmitters[30].x + m[4] * StartingEmitters[30].y + m[ 8] * StartingEmitters[30].z + m[12];
			TransformedStartingEmitters[30].y = m[1] * StartingEmitters[30].x + m[5] * StartingEmitters[30].y + m[ 9] * StartingEmitters[30].z + m[13];
			TransformedStartingEmitters[30].z = m[2] * StartingEmitters[30].x + m[6] * StartingEmitters[30].y + m[10] * StartingEmitters[30].z + m[14];
			TransformedStartingEmitters[31].x = m[0] * StartingEmitters[31].x + m[4] * StartingEmitters[31].y + m[ 8] * StartingEmitters[31].z + m[12];
			TransformedStartingEmitters[31].y = m[1] * StartingEmitters[31].x + m[5] * StartingEmitters[31].y + m[ 9] * StartingEmitters[31].z + m[13];
			TransformedStartingEmitters[31].z = m[2] * StartingEmitters[31].x + m[6] * StartingEmitters[31].y + m[10] * StartingEmitters[31].z + m[14];

			// compèute direction vectors

			Directions[ 0] = glm::normalize(TransformedStartingEmitters[16] - TransformedStartingEmitters[ 0]);
			Directions[ 1] = glm::normalize(TransformedStartingEmitters[17] - TransformedStartingEmitters[ 1]);
			Directions[ 2] = glm::normalize(TransformedStartingEmitters[18] - TransformedStartingEmitters[ 2]);
			Directions[ 3] = glm::normalize(TransformedStartingEmitters[19] - TransformedStartingEmitters[ 3]);
			Directions[ 4] = glm::normalize(TransformedStartingEmitters[20] - TransformedStartingEmitters[ 4]);
			Directions[ 5] = glm::normalize(TransformedStartingEmitters[21] - TransformedStartingEmitters[ 5]);
			Directions[ 6] = glm::normalize(TransformedStartingEmitters[22] - TransformedStartingEmitters[ 6]);
			Directions[ 7] = glm::normalize(TransformedStartingEmitters[23] - TransformedStartingEmitters[ 7]);
			Directions[ 8] = glm::normalize(TransformedStartingEmitters[24] - TransformedStartingEmitters[ 8]);
			Directions[ 9] = glm::normalize(TransformedStartingEmitters[25] - TransformedStartingEmitters[ 9]);
			Directions[10] = glm::normalize(TransformedStartingEmitters[26] - TransformedStartingEmitters[10]);
			Directions[11] = glm::normalize(TransformedStartingEmitters[27] - TransformedStartingEmitters[11]);
			Directions[12] = glm::normalize(TransformedStartingEmitters[28] - TransformedStartingEmitters[12]);
			Directions[13] = glm::normalize(TransformedStartingEmitters[29] - TransformedStartingEmitters[13]);
			Directions[14] = glm::normalize(TransformedStartingEmitters[30] - TransformedStartingEmitters[14]);
			Directions[15] = glm::normalize(TransformedStartingEmitters[31] - TransformedStartingEmitters[15]);

			// update particles

			ParticleNodeBase::Update(view, dt);
		}

		// ---------------------------------------------------------------
		//

		void Draw(vml::views::View* view)
		{
			if (!IsVisible())
				return;
			
			// draw emitters
			
			for (size_t i = 0; i < 32; ++i)
				vml::DebugRender->DrawPoint(view, TransformedStartingEmitters[i], vml::colors::Yellow);

			for (size_t i = 0; i < 96 / 3; ++i)
			{
				size_t j = i * 3;

				vml::DebugRender->DrawTriangle(view,
												TransformedStartingEmitters[Indices[j    ]],
												TransformedStartingEmitters[Indices[j + 1]],
												TransformedStartingEmitters[Indices[j + 2]], vml::colors::White, true);
			}
			
		}

		// ---------------------------------------------------------------
		// ctor / dtor

		CilindricalEmitters16(const std::string& screenname,
							  const float startingradius,
							  const float endingradius,
							  const float height,
							  const glm::vec3& position,
							  const glm::vec3& positiondeviation,
							  const float velocity,
							  const glm::vec3& velocitydeviation,
							  const glm::vec3& accelleration,
							  const glm::vec3& accellerationdeviation,
							  const float angle,
							  const float angledeviation,
							  const float angularvelocity,
							  const float angularvelocitydeviation,
							  const float angularaccelleration,
							  const float angularaccellerationdeviation,
							  const glm::vec2& billboardinitialparticlescale,
							  const glm::vec2& billboardfinalscale,
					 		  const glm::vec2& quadinitialparticlescale,
					 		  const glm::vec2& quadfinalscale,
							  const float spawningdelay,
							  const float initialdelay,
							  const float lifespan,
							  const int maxparticlescount,
							  const int maxparticlespoolsize,
							  const uint32 flags,
							  const uint32 drawmode,
							  const uint32 rotationmode,
							  const glm::vec4& startcolor,
							  const glm::vec4& endcolor) : ParticleNodeBase(screenname,
																			  "Cilindrical ( 16 points ) Emitter",
																			  position,
																			  positiondeviation,
																			  velocity,
																			  glm::vec3(0, 1, 0),
																			  velocitydeviation,
																			  accelleration,
																			  accellerationdeviation,
																			  angle, angledeviation,
																			  angularvelocity,
																			  angularvelocitydeviation,
																			  angularaccelleration,
																			  angularaccellerationdeviation,
																			  billboardinitialparticlescale,
																			  billboardfinalscale,
																			  quadinitialparticlescale,
																			  quadfinalscale,
																			  spawningdelay,
																			  initialdelay,
																			  lifespan,
																			  maxparticlescount,
																			  maxparticlespoolsize,
																			  flags,
																			  drawmode,
																			  rotationmode,
																			  startcolor,
																			  endcolor)
		{
			Height				  = height;
			StartingRadius		  = startingradius;
			FinalRadius			  = endingradius;
			InitialStartingRadius = startingradius;
			InitialFinalRadius	  = endingradius;

			// initialize arrays

			memset(&TransformedStartingEmitters[0], 0, 32 * sizeof(glm::vec3));
			memset(&StartingEmitters[0], 0, 32 * sizeof(glm::vec3));
			memset(&Directions[0], 0, 16 * sizeof(glm::vec3));

			// compute radii

			ComputeRadii();

		}

		~CilindricalEmitters16()
		{
		}

};
