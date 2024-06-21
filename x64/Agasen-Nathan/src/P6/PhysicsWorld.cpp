#include "PhysicsWorld.h"

using namespace P6;

void PhysicsWorld::AddParticle(Model* toAdd) {
	this->Particles.push_back(toAdd);
	this->ParticleDraw = Particles;
}

void PhysicsWorld::Update(float time) {
	UpdateParticleList();

	for (std::list<Model*>::iterator p = Particles.begin(); p != Particles.end(); p++)
	{
		(*p)->Update(time);

		if (AtCenter(*p)) {
			(*p)->Destroy(placements.front(), start_time);
			placements.pop_front();
		}
	}
}

void PhysicsWorld::UpdateParticleList() {
	Particles.remove_if(
		[](Model* p) {
			return p->IsDestroyed();
		}
	);
}

bool PhysicsWorld::AtCenter(Model* p) {
	if (p->z > -1 && p->z < 1)
		return true;
	else return false;
}