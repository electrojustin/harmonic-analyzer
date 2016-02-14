#include "guitar_pro.h"
#include "bend.h"
#include <stdint.h>
#include <cstdlib>

#pragma once

class BeatEffect
{
	public:
	size_t size;
	char* start;
	char* currPos;
	uint8_t header;
	uint8_t header2;
	uint8_t tps;
	Bend* tremolo;
	uint8_t upstroke;
	uint8_t downstroke;
	uint8_t pickstroke;

	BeatEffect (char* start);
	~BeatEffect ();
	void parse (void);
	void parseTPS (void);
	void parseTremolo (void);
	void parseStroke (void);
	void parsePickstroke (void);
	size_t getSize (void);
};

BeatEffect::BeatEffect(char* start)
{
	this->start = start;
	currPos = start;
	tremolo = NULL;
}

BeatEffect::~BeatEffect()
{
	if (tremolo)
		delete tremolo;
}

void BeatEffect::parse(void)
{
	pSB(header);
	pSB(header2);

	condParse(5, parseTPS);
	condParse2(2, parseTremolo);
	condParse(6, parseStroke);
	condParse2(1, parsePickstroke);

	size = currPos - start;
}

void BeatEffect::parseTPS(void)
{
	pSB(tps);
}

void BeatEffect::parseTremolo(void)
{
	tremolo = new Bend(currPos);
	tremolo->parse();
	currPos += tremolo->getSize();
}

void BeatEffect::parseStroke(void)
{
	pSB(upstroke);
	pSB(downstroke);
}

void BeatEffect::parsePickstroke(void)
{
	pSB(pickstroke);
}

size_t BeatEffect::getSize(void)
{
	return size;
}
