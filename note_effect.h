#include <stdint.h>
#include <cstdlib>
#include "guitar_pro.h"
#include "bend.h"

#pragma once

class NoteEffect
{
	public:
	size_t size;
	char* start;
	char* currPos;
	uint8_t header;
	uint8_t header2;
	Bend* bend;
	Gp5GraceNote graceNote;
	uint8_t tremolo;
	uint8_t slide;
	uint8_t harmonics;
	struct Gp5Trill trill;
	
	NoteEffect (char* start);
	~NoteEffect ();
	void parse (void);
	void parseBend (void);
	void parseTremolo (void);
	void parseGraceNote (void);
	void parseSlide (void);
	void parseHarmonics (void);
	void parseTrill (void);
	size_t getSize (void);
};

NoteEffect::NoteEffect(char* start)
{
	this->start = start;
	currPos = start;
	bend = NULL;
}

NoteEffect::~NoteEffect()
{
	if (bend)
		delete bend;
}

void NoteEffect::parse(void)
{
	pSB(header);
	pSB(header2);

	condParse(0, parseBend);
	condParse(4, parseGraceNote);
	condParse2(2, parseTremolo);
	condParse2(3, parseSlide);
	condParse2(4, parseHarmonics);
	condParse2(5, parseTrill);

	size = currPos - start;
}

void NoteEffect::parseBend(void)
{
	bend = new Bend(currPos);
	bend->parse();
	currPos += bend->getSize();
}

void NoteEffect::parseGraceNote(void)
{
	graceNote = *(struct Gp5GraceNote*)currPos;
	currPos += sizeof(struct Gp5GraceNote);
}

void NoteEffect::parseTremolo(void)
{
	pSB(tremolo);
}

void NoteEffect::parseSlide(void)
{
	pSB(slide);
}

void NoteEffect::parseHarmonics(void)
{
	pSB(harmonics);
}

void NoteEffect::parseTrill(void)
{
	trill = *(struct Gp5Trill*)currPos;
	currPos += sizeof(struct Gp5Trill);
}

size_t NoteEffect::getSize(void)
{
	return size;
}
