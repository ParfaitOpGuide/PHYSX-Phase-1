#include "PhysicsWorld.h"


using namespace P6;

void PhysicsWorld::GenerateList(Model* cloneSource) {
	int count;
	std::cout << "Amount of particles: ";
	std::cin >> count;

	std::cout << "Particles do their best now and are preparing. Please watch warmly";

	for (int i = 0; i < count; i++) {
		Model* p = new Model(*cloneSource);
		(*p).Acceleration = P6::MyVector(0, 0, 0);
		(*p).Mass = 1;
		(*p).lifetime = ((rand() % (100 - 10 + 1)) + 10) / 10.0; // generates a float from 1.0 to 10.0 by diving a rand int 100 by float 10.0
		(*p).AddForce(P6::MyVector((rand() % 7000 - 3500), ((rand() % (7400 - 1000 + 1)) + 1000), (rand() % 7000 - 3500)));
		(*p).color = { (rand() % 11) /10.0 ,(rand() % 11) / 10.0,(rand() % 11) / 10.0 }; // generates a float from 0 to 1 by diving a rand int 10 by float 10.0
		(*p).scale = (rand() % (20 - 4 + 1) + 4);
		this->AddParticle(p);
	}
}

void PhysicsWorld::AddParticle(Model* toAdd) {
	this->Particles.push_back(toAdd);

	forceRegistry.Add(toAdd, &Gravity);
}

void PhysicsWorld::Update(float time, bool paused) {

	if (!paused) {
		UpdateParticleList();

		forceRegistry.UpdateForces(time);

		for (std::list<Model*>::iterator p = Particles.begin(); p != Particles.end(); p++)
		{
			(*p)->Update(time);

			if ((*p)->currTime >= (*p)->lifetime) {
				(*p)->Destroy();

			}
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