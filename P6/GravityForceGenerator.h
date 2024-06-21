#pragma once
#include "ForceGenerator.h"
#include "../Object/Model.h"

namespace P6 {
	class GravityForceGenerator : public ForceGenerator
	{
	private:
		MyVector Gravity = MyVector(0, -9.8f, 0);

	public:
		GravityForceGenerator(const MyVector gravity) : Gravity(gravity) {};
		void UpdateForce(Model* particle, float time) override;
	};
}

