#pragma once
#include <list>
#include "../Object/Model.h"
#include <chrono>

namespace P6 {
	class PhysicsWorld
	{

	public:

		std::list<Model*> Particles;
		std::list<Model*> ParticleDraw;
		std::list<std::string> placements = { "1st", "2nd", "3rd", "4th" };
		std::chrono::steady_clock::time_point start_time;
		

		void AddParticle(Model* toAdd);
		void Update(float time);

		void UpdateParticleList();

		bool AtCenter(Model* p);
	};
}

