#define INT_MAX 4000000000
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <string>
#include <fstream>
#include "math.h"
using namespace std;
#include "Note.h"
#include "NoteBank.h"
typedef vector<mike::NoteBank> Channel;
#include "parsednote.h"
#include "Score.h"
#include "ChordMap.h"
#include "Analyzer.h"
#include "measure.h"
#include "guitar_pro.h"
#include "track.h"
#include "beat.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace mike;

const map <int, float> CHROMOTODIATONIC{
	{1, 1},
	{2, 1.5},
	{3, 2},
	{4, 3},
	{5, 3.5},
	{6, 4},
	{7, 4.5},
	{8, 5},
	{9, 6},
	{10, 6.5},
	{11, 7},
	{12, 7.5}
};

#define readString {\
	temp = *(uint32_t*)currPos;\
	currPos += 4;\
	currPos += temp;\
}

int main(int argc, char** argv)
{
	/*Measure* measure;
	Track* track;*/
	Beat* beat;
	size_t fileSize;
	char* fileBuf;
	char* currPos;
	uint32_t temp;
	uint32_t numMeasures;
	uint32_t numTracks;
	int i;
	argv[1] = "blah.gp5";
	FILE* fp = fopen(argv[1], "r");
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fileBuf = (char*)malloc(fileSize+1000);
	fseek(fp, 0, SEEK_SET);
	fread(fileBuf, 1, fileSize, fp);
	currPos = fileBuf;

	//I saw this pattern in the hex dump
	//I dunno why this works or how consistently
	while (*(uint32_t*)currPos != 0xFFFFFFFF)
		currPos++;

	while (*(uint32_t*)currPos != 0)
		currPos++;

	currPos += 4;
	pSI(numMeasures);
	pSI(numTracks);
	currPos += 3;

	//More fuckin' magic
	for (i = 0; i < numTracks; i++)
	{
		while (*(uint32_t*)currPos != 0xFF0A0908)
			currPos++;
		currPos += 9;
		readString;
		readString;
		currPos += 7;
		readString;
		readString;
		currPos += 5;
	}

	uint64_t currStep;
	uint64_t globalStart = 0;
	uint64_t globalEnd = 0;
	int j;
	char* val;
	std::vector<mike::NoteBank> channel;
	mike::Score score;

	printf("%d\n", numMeasures);
	printf("%d\n", sizeof (char));

	for (i = 0; i < numMeasures; i++)
	{
		currStep = 0;
		while (currStep < 64)
		{
			mike::NoteBank tempBank;
			std::vector<mike::Note> tempNotes;
			size_t tempSize = &fileBuf [fileSize] - currPos;
			beat = new Beat(currPos, fileBuf + fileSize - currPos);
			beat->parse();
			currStep += pow(2, 6 - (beat->beatDuration + 2));
			globalEnd += pow(2, 6 - (beat->beatDuration + 2));
			for (j = 0; j < beat->notes.size(); j++)
			{
				val = beat->notes[j]->val;
				if (val[1] == '#')
				{
					printf("%s", val);
					mike::Note tempNote(val[0] - 'A' + 1, 1);

					tempNotes.push_back(tempNote);
				}
				else
				{
					printf("%s", val);
					mike::Note tempNote(val[0] - 'A' + 1, 0);
					tempNotes.push_back(tempNote);
				}
			}
			//beat->addText("hello", 5);
			printf("\n");
			tempBank.addNotes(tempNotes);
			if(tempBank.getNotes().size() > 0)
				mike::Analyzer::DetermineChord(tempBank);
			channel.push_back(tempBank);
			globalStart = globalEnd;
			currPos += beat->getSize();
			delete beat;
		}

		currPos += 15;
	}

	score.addInstrument(channel);
	//fwrite(fileBuf, 1, fileSize + 1000, fp);
	fclose(fp);

	//mike::Analyzer::LoopOverScore (score);
	
	/*string filename = "Test.mscx";
	ifstream f(filename);
	if (!f)
	{
		cerr << "Could not open file " << filename << endl;
		return 1;
	}

	// Read the entire file into memory
	vector<string> file;
	string t;
	while (getline(f, t))
	file.push_back(t + '\n');
	f.close();

	//get track begin/end
	vector<int> trackIndexes;
	for (int i = 0; i < file.size(); i++) {
		if (file.at(i).find("<Staff id=") != string::npos)
			trackIndexes.push_back(i);
	}
	trackIndexes.erase(trackIndexes.begin(), trackIndexes.begin() + trackIndexes.size() / 2);
	trackIndexes.push_back(file.size());

	//load notes
	Score score;
	for (int i = 1; i < trackIndexes.size(); i++) {
		Channel instrument;
		bool trackBreak = false;
		for (int j = trackIndexes.at(0); j < trackIndexes.at(i); j++)
		{
			if (trackBreak)
				break;
			if (file.at(i).find("<Chord>")) {
				//find duration type of chord
				while (file.at(j).find("<durationType>") == string::npos)
					j++;
				if (file.at(j).find("<durationType>") == string::npos)
					j++;

				//find notes
				NoteBank bank;
				while (file.at(j).find("<durationType>") == string::npos)
					j++;

				bool hitNextChord = false;
				while (file.at(j).find("<Chord>") == string::npos)
				{
					if (hitNextChord)
						break;
					#define line file.at(j)
					cout << line;
					int numIndex = line.find("<durationType>") + 14;
					int endNumIndex = 0;
					endNumIndex = line.rfind("<");
					string duration = line.substr(numIndex, endNumIndex - numIndex);
					cout << duration;

					//move to note
					while (line.find("<pitch>") == string::npos || j == trackIndexes.size() - 1) {
						j++;
					}
					if (j == trackIndexes.size() - 1) {
						trackBreak = true;
						break;
					}

					NoteBank bank;
					while (!hitNextChord) {
						if (line.find("<pitch>") == string::npos)
							j++;
						int numBegin = line.find("<pitch>") + 7;
						int numEnd = line.rfind("<");
						string numString = line.substr(numBegin, numEnd - numBegin);
						int number = stoi(numString);
						number = number % 12;
						if (number >= 9)
							number -= 8;
						else
							number += 4;

						j++;
						float intNote = CHROMOTODIATONIC.at(number);
						Note note;
						note.note = floor(intNote);
						if (intNote - floor(intNote) > 0)
							note.mode = 1;
						else
							note.mode = 0;
						bank.addNote(note);

						j++;
						while (line.find("<Chord>") == string::npos && line.find("<pitch>") == string::npos) {
							j++;
						}
						if (line.find("<Chord>") != string::npos) {
							hitNextChord = true;
						}
					}
						j++;
					//instrument.push_back(bank);
				}
				instrument.push_back(bank);
			} //end of chord
		}
		score.addInstrument(instrument);
	}//end of track
	
	int terminate;
	cin >> t;*/
	return 0;
}
