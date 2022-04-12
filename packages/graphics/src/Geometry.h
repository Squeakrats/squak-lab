#pragma once
#include <vector>

class Geometry {
public:
	std::vector<float> positions;

	Geometry() : positions() {};
	Geometry(std::vector<float>&& positions) : positions(positions) {};
};