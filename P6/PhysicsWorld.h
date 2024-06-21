#pragma once
#include <list>
#include "../Object/Model.h"
#include <chrono>
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

namespace P6 {
	class PhysicsWorld
	{

	public:
		ForceRegistry forceRegistry;

		std::list<Model*> Particles;
		std::list<std::string> placements = { "1st", "2nd", "3rd", "4th" };
		
		void GenerateList(Model* cloneSource);

		void AddParticle(Model* toAdd);
		void Update(float time, bool paused);
		

		bool AtCenter(Model* p);

	private:
		void UpdateParticleList();
		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8f, 0));
	};
}

