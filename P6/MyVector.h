#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace P6 {
	class MyVector
	{
	public:
		float x, y, z;

		MyVector() : x(0), y(0), z(0) {}
		MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}
        MyVector(glm::vec3 v) : x(v.x), y(v.y), z(v.z){}

		/* sample operator changes
		MyVector operator+ (const MyVector v) {
			return MyVector(this->x + v.x, this->y + v.y, this->z + v.z);
		}

		void operator+= (const MyVector v) {
			this->x += v.x; this->y += v.y; this->z += v.z;
		}*/

		float Magnitude();
		MyVector Direction();

        MyVector operator+ (const int n);
        MyVector operator+ (const float f);
        MyVector operator+= (const int num);
        MyVector operator+= (const float f);

        MyVector operator+ (const MyVector v);
        MyVector operator+= (const MyVector v);

        MyVector operator- (const int n);
        MyVector operator- (const float f);
        MyVector operator-= (const int num);
        MyVector operator-= (const float f);

        MyVector operator- (const MyVector v);
        MyVector operator-= (const MyVector v);

        //scalar multiplication
        MyVector operator* (const int n);
        MyVector operator* (const float f);
        MyVector operator*= (const int num);
        MyVector operator*= (const float f);

        //component product
        MyVector operator* (const MyVector v);
        MyVector operator*= (const MyVector v);

        MyVector operator/ (const int n);
        MyVector operator/ (const float f);
        MyVector operator/= (const int num);
        MyVector operator/= (const float f);

        MyVector operator/ (const MyVector v);
        MyVector operator/= (const MyVector v);

        operator glm::vec3() const;

		MyVector Add(MyVector b);
		MyVector Subtract(MyVector b);

		MyVector ScalarMult(float b);
		MyVector Component(MyVector b);

		float ScalarProd(MyVector b);
		MyVector Cross(MyVector b);

        MyVector Normalize();
	};
}