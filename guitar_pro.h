#include <stdint.h>

#pragma once

#define condParse(bitNum, parseFunc) {\
	if ((header >> bitNum) & 1)\
		parseFunc ();\
}

#define condParse2(bitNum, parseFunc) {\
	if ((header2 >> bitNum) & 1)\
		parseFunc ();\
}

//Parse Simple Byte
#define pSB(name) {\
	name = *(uint8_t*)currPos;\
	currPos ++;\
}

//Parse Simple Int
#define pSI(name) {\
	name = *(uint32_t*)currPos;\
	currPos += 4;\
}

//Generic parse simple
#define pS(type, name) {\
	name = *(type*)currPos;\
	currPos += sizeof (type);\
}

//Parse complex
#define pC(name, type) {\
	name = new type (currPos);\
	name->parse();\
	currPos += name->getSize();\
}

//Destroy complex
#define dC(name) {\
	if (name)\
		delete name;\
}

struct Gp5Color
{
	uint8_t white;
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};

struct Gp5TablatureHeader
{
	uint32_t size;
	char* data;
};

//Lyric header is arbitrary size, parse over it

struct Gp5MidiChannel
{
	uint32_t instrument;
	uint8_t volume;
	uint8_t balance;
	uint8_t chorus;
	uint8_t reverb;
	uint8_t phaser;
	uint8_t tremolo;
	uint8_t blanks [2];
};

struct Gp5InfoHeader
{
	uint32_t tempo;
	uint8_t key;
	uint8_t octave;
	struct Gp5MidiChannel channels [16];
	uint32_t numMeasures;
	uint32_t numTracks;
};

struct Gp5Marker
{
	uint32_t incLen;
	char* name;
	struct Gp5Color color;
};

struct Gp5StringTune
{
	uint32_t tune [7];
};

struct Gp5Chord
{
	uint8_t header;
	uint8_t sharp;
	uint8_t blanks [3];
	uint8_t root;
	uint8_t type;
	uint8_t net;
	uint32_t bass;
	uint32_t da;
	uint8_t add;
	char name [20];
	uint8_t blanks2 [2];
	uint8_t fifth;
	uint8_t ninth;
	uint8_t eleventh;
	uint32_t baseFret;
	int32_t frets [7];
	uint8_t numBarres;
	uint8_t barreFret [5];
	uint8_t barreStart [5];
	uint8_t barreEnd [5];
	uint8_t omissions [7];
	uint8_t blank3;
	int8_t fingering [7];
	uint8_t showDiagFingering;
};

struct Gp5GraceNote
{
	uint8_t fret;
	uint8_t dynamic;
	uint8_t transition;
	uint8_t duration;
};

struct Gp5Point
{
	uint32_t absTimePos;
	uint32_t vertPos;
	uint8_t vibrato;
};

struct Gp5MixTable
{
	int8_t instrument;
	int8_t vol;
	int8_t pan;
	int8_t chorus;
	int8_t reverb;
	int8_t phaser;
	int8_t tremolo;
	int32_t tempo;
	uint8_t volChange;
	uint8_t panChange;
	uint8_t chorusChange;
	uint8_t reverbChange;
	uint8_t phaserChange;
	uint8_t tremoloChange;
	uint8_t tempoChange;
	uint8_t precises;
};

struct Gp5Trill
{
	uint8_t fret;
	uint8_t period;
};

struct Gp5NoteDuration
{
	uint8_t time;
	uint8_t nTuplet;
};
