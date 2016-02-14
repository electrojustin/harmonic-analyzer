#include <stdint.h>
#include "guitar_pro.h"
#include "beat_effect.h"
#include "note.h"
#include <vector>
#include <cstdlib>

#pragma once

class Beat
{
	public:
	size_t size;
	size_t restFileSize;
	char* start;
	char* currPos;
	char* potTextLoc;
	uint8_t header;
	uint8_t status;
	int8_t beatDuration;
	uint32_t nTuplet;
	struct Gp5Chord diagram;
	int textLen;
	char* text;
	BeatEffect* effects;
	struct Gp5MixTable mixTable;
	std::vector <Note*> notes;

	Beat (char* start, size_t fileSize);
	~Beat ();
	void parse (void);
	void parseTuple (void);
	void parseChord (void);
	void parseText (void);
	void parseEffects (void);
	void parseMixTable (void);
	void addText(char* toAdd, uint8_t len);
	size_t getSize (void);
};

Beat::Beat(char* start, size_t restFileSize)
{
	this->start = start;
	currPos = start;
	this->restFileSize = restFileSize;
}

Beat::~Beat()
{
	dC(effects);
}

void Beat::parse(void)
{
	int stringNum;
	uint8_t strings;
	Note* note;

	pSB(header);
	pS(int8_t, beatDuration);
	condParse(5, parseTuple);
	condParse(1, parseChord);
	potTextLoc = currPos;
	condParse(2, parseText);
	condParse(3, parseEffects);
	condParse(4, parseMixTable);

	pSB(strings);
	for (stringNum = 0; stringNum < 7; stringNum++)
	{
		if (strings >> (7 - stringNum) & 1)
		{
			note = new Note(currPos);
			note->parse(stringNum);
			notes.push_back(note);
			currPos += note->getSize();
		}
	}

	currPos += 2;

	size = currPos - start;
}

void Beat::parseTuple(void)
{
	pSI(nTuplet);
}

void Beat::parseChord(void)
{
	pS(struct Gp5Chord, diagram);
}

void Beat::parseText(void)
{
	pSI(textLen);
	currPos++;
	text = currPos;
	currPos += textLen - 1;
}

void Beat::parseEffects(void)
{
	pC(effects, BeatEffect);
}

void Beat::parseMixTable(void)
{
	pS(struct Gp5MixTable, mixTable);
}

void Beat::addText(char* toAdd, uint8_t len)
{
	uint8_t newHeader;
	size_t tempSize = start + restFileSize - potTextLoc;
	newHeader = *(uint8_t*)start;
	newHeader |= 0x04;
	*(uint8_t*)start = newHeader;
	char* temp = (char*)malloc(tempSize);
	memcpy(temp, potTextLoc, tempSize);
	*(uint32_t*)potTextLoc = (uint32_t)len+1;
	potTextLoc += 4;
	*(uint32_t*)potTextLoc = len;
	potTextLoc += 1;
	memcpy(potTextLoc, toAdd, len);
	potTextLoc += len;
	memcpy(potTextLoc, temp, tempSize);
}

size_t Beat::getSize(void)
{
	return size;
}
