#include "measure.h"

Measure::Measure (char* start)
{
	this->start = start;
	currPos = start;
}

void Measure::parse (void)
{
	pSB (header);

	condParse (0, parseNumer);
	condParse (1, parseDenom);
	isRepeat = (header >> 2) & 1;
	currPos ++;
	condParse (3, parseRepeats);
	condParse (4, parseEnds);
	condParse (5, parseMarker);
	condParse (6, parseTonality);

	doubleBar = (header >> 7) & 1;
	currPos ++;

	size = (size_t)(currPos - start);
}

void Measure::parseNumer (void)
{
	pSB (numerSig);
}

void Measure::parseDenom (void)
{
	pSB (denomSig);
}

void Measure::parseRepeats (void)
{
	pSB (numRepeats);
}

void Measure::parseEnds (void)
{
	pSB (numEnds);
}

void Measure::parseMarker (void)
{
	marker.incLen = *(uint32_t*)currPos;
	currPos += 4;
	marker.name = currPos;
	currPos += marker.incLen;
	marker.color = *(Gp5Color*)currPos;
	currPos += sizeof (Gp5Color);
}

void Measure::parseTonality (void)
{
	tonality = *(int8_t*)currPos;
	currPos ++;
}

size_t Measure::getSize (void)
{
	return size;
}
