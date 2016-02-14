#include <stdint.h>
#include <cstdlib>
#include "guitar_pro.h"

#pragma once

class Measure
{
	public:
	char* start;
	char* currPos;
	uint8_t header;
	size_t size;
	uint8_t numerSig;
	uint8_t denomSig;
	uint8_t numRepeats;
	uint8_t numEnds;
	struct Gp5Marker marker;
	int8_t tonality;
	bool doubleBar;
	bool isRepeat;

	Measure (char* start);
	void parse (void);
	void parseTonality (void);
	void parseMarker (void);
	void parseEnds (void);
	void parseRepeats (void);
	void parseDenom (void);
	void parseNumer (void);

	size_t getSize (void);
};
