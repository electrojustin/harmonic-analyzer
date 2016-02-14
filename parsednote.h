#include "guitar_pro.h"
#include <stdint.h>
#include <cstdlib>
#include "note_effect.h"

#pragma once

class Note
{
	public:
	uint8_t header;
	size_t size;
	char* start;
	char* currPos;
	uint16_t type;
	Gp5NoteDuration duration;
	uint8_t dynamic;
	uint8_t fretNum;
	int8_t fingerRight;
	int8_t fingerLeft;
	NoteEffect* effects;
	char val [2];

	Note (char* start);
	~Note ();
	void parse (int stringNum);
	void parseType (void);
	void parseDuration (void);
	void parseDynamic (void);
	void parseFret (void);
	void parseFingering (void);
	void parseEffects (void);
	size_t getSize (void);
};

Note::Note(char* start)
{
	this->start = start;
	currPos = start;
}

Note::~Note()
{
	dC(effects);
}

void Note::parse(int stringNum)
{
	const char* guitarLookup[6][16] = { { "E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G" },
	{ "B", "C", "C#", "D", "D#", "E", "F", "A#", "G", "G#", "A", "A#", "B", "C", "C#", "D" },
	{ "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#" },
	{ "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#", "E", "F" },
	{ "A", "A#" ,"B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C" },
	{ "E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G" } };

	pSB(header);

	condParse(5, parseType);
	condParse(0, parseDuration);
	condParse(4, parseDynamic);
	//condParse (5, parseFret);
	condParse(7, parseFingering);

	if ((header >> 3) & 1)
		pC(effects, NoteEffect);

	memcpy(val, guitarLookup[stringNum - 1][*(uint8_t*)currPos], 2);
	currPos += 2;

	size = currPos - start;
}

void Note::parseType(void)
{
	pS(uint8_t, type);
}

void Note::parseDuration(void)
{
	pS(struct Gp5NoteDuration, duration);
}

void Note::parseDynamic(void)
{
	pSB(dynamic);
}

void Note::parseFret(void)
{
	pSB(fretNum);
}

void Note::parseFingering(void)
{
	pS(int8_t, fingerLeft);
	pS(int8_t, fingerRight);
}

size_t Note::getSize(void)
{
	return size;
}
