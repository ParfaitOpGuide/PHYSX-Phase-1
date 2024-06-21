#include "GravityForceGenerator.h"

using namespace P6;

void GravityForceGenerator::UpdateForce(Model* particle, float time)
{
	if (particle->Mass <= 0) return;

	MyVector force = Gravity * particle->Mass;
	particle->AddForce(force);
}