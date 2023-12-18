#pragma once
class color {
public:
	color() { r = 1; g = 1; b = 1; }
	color(float R, float G, float B) { r = R / 255; g = G / 255; b = B / 255; }
	~color() {}
	float r; float g; float b;

	bool operator==(const color& other)
	{
		return (other.r == r && other.g == g && other.b == b);
	}
};