#pragma once
#include "../Object/Model.h"

namespace P6 {
	class ForceGenerator
	{
	public:
		virtual void UpdateForce(Model* p, float time) {
			p->AddForce(MyVector(0, 0, 0));
		}
	};
}
