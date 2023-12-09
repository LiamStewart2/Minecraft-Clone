#pragma once
class Vector3
{
public:
	Vector3(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	Vector3() { x = 0; y = 0; z = 0; }

	float x; float y; float z;
};