// HarmonicAnalyzer.cpp : Defines the entry point for the console application.
#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <string>
#include "math.h"
using namespace std;
using namespace mike;

#include "Note.h"
#include "NoteBank.h"
typedef vector<NoteBank> Channel;
#include "Score.h"
#include "ChordMap.h"
#include "Analyzer.h"


int main()
{


	Channel inst1;
	vector<int> inNotes = { 1, 3, 5 };
	vector<int> inModes = { 0, 0, 0 };
	NoteBank nb1 = NoteBank(inNotes, inModes, 0, 2);
	Channel c1(1, nb1);
	nb1.setRange(3, 6);
	c1.push_back(nb1);

	inNotes.erase(inNotes.begin() + 1);
	inModes.erase(inModes.begin() + 1);
	NoteBank nb2 = NoteBank(inNotes, inModes, 0, 1);
	Channel c2(1, nb2);
	nb2.setRange(2, 3);
	c2.push_back(nb2);
	nb2.setRange(4, 5);
	c2.push_back(nb2);

	inNotes.push_back(2);
	inNotes.push_back(5);
	inModes.push_back(0);
	inModes.push_back(0);
	NoteBank nb3 = NoteBank(inNotes, inModes, 0, 3);
	Channel c3(1, nb3);
	inNotes.erase(inNotes.begin() + 2, inNotes.begin() + 4);
	inModes.erase(inModes.begin() + 2, inModes.begin() + 4);
	nb3 = NoteBank(inNotes, inModes, 4, 5);
	c3.push_back(nb3);



	Score score;
	score.addInstrument(c1);
	score.addInstrument(c2);
	score.addInstrument(c3);
	score.setLength(5);

	Analyzer::LoopOverScore(score);

	return 0;
}

