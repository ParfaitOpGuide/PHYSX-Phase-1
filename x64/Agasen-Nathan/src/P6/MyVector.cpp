#include "MyVector.h"
#include <math.h>

using namespace P6;

float MyVector::Magnitude() {
	float mag = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	return mag;
}

MyVector MyVector::Direction() {
	float c1, c2, c3;
	c1 = this->x * this->Magnitude();
	c2 = this->y * this->Magnitude();
	c3 = this->z * this->Magnitude();

	return { c1,c2,c3 };
}

MyVector MyVector::operator+ (const int n) { return MyVector(this->x + n, this->y + n, this->z + n); }
MyVector MyVector::operator+ (const float f) { return MyVector(this->x + f, this->y + f, this->z + f); }
MyVector MyVector::operator+= (const int num) { return MyVector(this->x += num, this->y += num, this->z += num); }
MyVector MyVector::operator+= (const float f) { return MyVector(this->x += f, this->y += f, this->z += f); }

MyVector MyVector::operator+ (const MyVector v) { return MyVector(this->x + v.x, this->y + v.y, this->z + v.z); }
MyVector MyVector::operator+= (const MyVector v) { return MyVector(this->x += v.x, this->y += v.y, this->z += v.z); }

MyVector MyVector::operator- (const int n) { return MyVector(this->x - n, this->y - n, this->z - n); }
MyVector MyVector::operator- (const float f) { return MyVector(this->x - f, this->y - f, this->z - f); }
MyVector MyVector::operator-= (const int num) { return MyVector(this->x -= num, this->y -= num, this->z -= num); }
MyVector MyVector::operator-= (const float f) { return MyVector(this->x -= f, this->y -= f, this->z -= f); }

MyVector MyVector::operator- (const MyVector v) { return MyVector(this->x - v.x, this->y - v.y, this->z - v.z); }
MyVector MyVector::operator-= (const MyVector v) { return MyVector(this->x -= v.x, this->y -= v.y, this->z -= v.z); }

//scalar multiplication
MyVector MyVector::operator* (const int n) { return MyVector(this->x * n, this->y * n, this->z * n); }
MyVector MyVector::operator* (const float f) { return MyVector(this->x * f, this->y * f, this->z * f); }
MyVector MyVector::operator*= (const int num) { return MyVector(this->x *= num, this->y *= num, this->z *= num); }
MyVector MyVector::operator*= (const float f) { return MyVector(this->x *= f, this->y *= f, this->z *= f); }

//component product
MyVector MyVector::operator* (const MyVector v) { return MyVector(this->x * v.x, this->y * v.y, this->z * v.z); }
MyVector MyVector::operator*= (const MyVector v) { return MyVector(this->x *= v.x, this->y *= v.y, this->z *= v.z); }

MyVector MyVector::operator/ (const int n) { return MyVector(this->x / n, this->y / n, this->z / n); }
MyVector MyVector::operator/ (const float f) { return MyVector(this->x / f, this->y / f, this->z / f); }
MyVector MyVector::operator/= (const int num) { return MyVector(this->x /= num, this->y /= num, this->z /= num); }
MyVector MyVector::operator/= (const float f) { return MyVector(this->x /= f, this->y /= f, this->z /= f); }

MyVector MyVector::operator/ (const MyVector v) { return MyVector(this->x / v.x, this->y / v.y, this->z / v.z); }
MyVector MyVector::operator/= (const MyVector v) { return MyVector(this->x /= v.x, this->y /= v.y, this->z /= v.z); }

MyVector::operator glm::vec3() const { return glm::vec3(x, y, z); }

MyVector MyVector::Add(MyVector b) {
	this->x += b.x;
	this->y += b.y;
	this->z += b.z;
	return { this->x,this->y,this->z };
}

MyVector MyVector::Subtract(MyVector b) {
	this->x -= b.x;
	this->y -= b.y;
	this->z -= b.z;
	return { this->x,this->y,this->z };
}

MyVector MyVector::ScalarMult(float b) {
	this->x *= b;
	this->y *= b;
	this->z *= b;

	return { this->x,this->y,this->z };
}

MyVector MyVector::Component(MyVector b) {
	float c1, c2, c3;
	c1 = (this->x * b.x);
	c2 = (this->y * b.y);
	c3 = (this->z * b.z);
	return { c1,c2,c3 };
}

float MyVector::ScalarProd(MyVector b) {
	float dot = (this->x * b.x) + (this->y * b.y) + (this->z * b.z);
	return dot;
}

MyVector MyVector::Cross(MyVector b) {
	float c1, c2, c3;
	c1 = (this->y * b.z) - (this->z * b.y);
	c2 = (this->z * b.x) - (this->x * b.z);
	c3 = (this->x * b.y) - (this->y * b.x);

	return { c1,c2,c3 };
}