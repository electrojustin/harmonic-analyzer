#include "track.h"
#include <vector>

Track::Track (char* start)
{
	this->start = start;
	currPos = start;
}

void Track::parse (void)
{
	int i;
	pSB (header);
	name = currPos;
	currPos += 40;
	pSI (numStrings);
	for (i = 0; i < numStrings; i ++)
	{
		tuneTable.push_back (*(struct Gp5StringTune*)currPos);
		currPos += sizeof (struct Gp5StringTune);
	}

	pSI (port);
	pSI (channel);
	pSI (effectChannel);
	pSI (numFrets);
	pSI (capoHeight);
	color = *(struct Gp5Color*)currPos;
	currPos += sizeof (struct Gp5Color);

	size = currPos - start;
}

size_t Track::getSize (void)
{
	return size;
}
