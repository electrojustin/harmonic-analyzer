#include <stdint.h>
#include <vector>
#include <cstdlib>
#include "guitar_pro.h"

#pragma once

class Track
{
	public:
	size_t size;
	char* start;
	char* currPos;
	uint8_t header;
	char* name;
	uint32_t numStrings;
	std::vector <struct Gp5StringTune> tuneTable;
	uint32_t port;
	uint32_t channel;
	uint32_t effectChannel;
	uint32_t numFrets;
	uint32_t capoHeight;
	struct Gp5Color color;

	Track (char* start);
	void parse (void);
	size_t getSize (void);
};
