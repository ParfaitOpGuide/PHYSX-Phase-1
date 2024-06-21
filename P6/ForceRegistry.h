#pragma once
#include <list>
#include "../Object/Model.h"
#include "ForceGenerator.h"

namespace P6 {
	class ForceRegistry
	{

	public:
		void Add(Model* particle, ForceGenerator* generator);
		void Remove(Model* particle, ForceGenerator* generator);
		void Clear();

		void UpdateForces(float time);

	protected:
		struct ParticleForceRegistry {
			Model* particle;
			ForceGenerator* generator;
		};

		std::list<ParticleForceRegistry> Registry;
	};
}

