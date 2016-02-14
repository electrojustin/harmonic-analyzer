#include <stdint.h>
#include <vector>
#include <cstdlib>
#include "guitar_pro.h"

#pragma once

class Bend
{
	public:
	size_t size;
	char* start;
	char* currPos;
	uint8_t type;
	uint32_t val;
	uint32_t numPoints;
	std::vector <struct Gp5Point> points;

	Bend (char* start);
	void parse (void);
	size_t getSize (void);
};

Bend::Bend(char* start)
{
	this->start = start;
	currPos = start;
}

void Bend::parse(void)
{
	int i;

	pSB(type);
	pSI(val);
	pSI(numPoints);

	for (i = 0; i < numPoints; i++)
	{
		points.push_back(*(struct Gp5Point*)currPos);
		currPos += sizeof(struct Gp5Point);
	}

	size = currPos - start;
}

size_t Bend::getSize(void)
{
	return size;
}