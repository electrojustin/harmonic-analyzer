#pragma once

namespace mike
{

const int THIRDS[] = { 1, 3, 5, 7, 2, 4, 6, 1, 3, 5, 7, 2, 4, 6 };
const int THRIDSSIZE = 14;


//Class Body*******************************************************
class Analyzer {
public:
	static void DetermineChord(NoteBank chord);
	static Note DetermineRoot(NoteBank chord);
	static string DetermineQuality(NoteBank chord, Note rootNote);
	static void LoopOverScore(Score score);
	static NoteBank RemoveDuplicates(NoteBank chord);
	static NoteBank RemoveStrictDuplicates(NoteBank chord);
	static bool ContainsNote(vector<Note>, Note target);
	static bool StrictContainsNote(vector<Note>, Note target);
	static int FindNextInstanceOf(int bottomCutoff, Note note); //used explicitly for third determination
	static int maxElement(vector<int> data);
	static int mapToChromatic(Note note);
};

//Function Definitions**********************************************
int Analyzer::mapToChromatic(Note note) {
	map<int, float> chromaticMap;
	chromaticMap[1] = 1;
	chromaticMap[2] = 3;
	chromaticMap[3] = 4;
	chromaticMap[4] = 6;
	chromaticMap[5] = 8;
	chromaticMap[6] = 9;
	chromaticMap[7] = 11;

	return chromaticMap.at(note.note) + note.mode;
}
bool Analyzer::ContainsNote(vector<Note> notes, Note target) {
	for (int j = 0; j < notes.size(); j++) {
		if (notes.at(j).Equals(target))
			return true;
	}
	return false;
}

bool Analyzer::StrictContainsNote(vector<Note> notes, Note target) {
	for (int j = 0; j < notes.size(); j++) {
		if (notes.at(j).StrictEquals(target))
			return true;
	}
	return false;
}


NoteBank Analyzer::RemoveDuplicates(NoteBank bank) {
	vector<Note> notes = bank.getNotes();
	vector<Note> chord;

	for (int i = 0; i < notes.size(); i++) {
		if (!ContainsNote(chord, notes.at(i)))
			chord.push_back(notes.at(i));
	}

	return NoteBank(chord);
}

NoteBank Analyzer::RemoveStrictDuplicates(NoteBank bank) {
	vector<Note> notes = bank.getNotes();
	vector<Note> chord;

	for (int i = 0; i < notes.size(); i++) {
		if (!StrictContainsNote(chord, notes.at(i)))
			chord.push_back(notes.at(i));
	}

	return NoteBank(chord);
}

int Analyzer::FindNextInstanceOf(int bottomCutoff, Note note) {

	int start = 0;
	for (int i = bottomCutoff; i < THRIDSSIZE; i++) {
		if (THIRDS[i] == note.note) {
			start = i;
			break;
		}
	}

	for (int i = start; i < THRIDSSIZE; i++) {
		if (THIRDS[i] == note.note) {
			return i;
		}
	}

	cout << "Find Next Instance failed";
	return -1;
}

int Analyzer::maxElement(vector<int> data) {
	int max = -1;
	for (int i = 0; i < data.size(); i++) {
		if (data.at(i) > max)
			max = data.at(i);
	}

	return max;
}

//determines root index in terms of passed in notes
Note Analyzer::DetermineRoot(NoteBank chord) {

	int complexity = 15;
	Note rootChoice;

	for (int i = 0; i < chord.getNotes().size(); i++) {
		vector<Note> otherNotes = chord.getNotes();

		//find location of root note in THIRDS
		int rootIndex = FindNextInstanceOf(0, otherNotes.at(i)); //to be used for later calculation

																 //find next locations of other notes
		otherNotes.erase(otherNotes.begin() + i);
		vector<int> indexes;
		for (int j = 0; j < otherNotes.size(); j++) {
			indexes.push_back(FindNextInstanceOf(rootIndex, otherNotes.at(j)));
		}

		//check if complexity of this root choice is less than any others, if so, use this as root
		int thisComplexity = maxElement(indexes) - rootIndex;
		if (thisComplexity < complexity) {
			complexity = thisComplexity;
			rootChoice = chord.getNotes().at(i);
		}
	}

	return rootChoice;
}

string Analyzer::DetermineQuality(NoteBank chord, Note rootNote) {
	string quality = "";
	vector<Note> notes = chord.getNotes();
	int root = FindNextInstanceOf(0, rootNote);

	vector<int> indexes;
	//int x = 2;
	int thirdsRootIndex = FindNextInstanceOf(0, rootNote);
	indexes.push_back(thirdsRootIndex);

	for (int j = 0; j < notes.size(); j++) {
		if(!notes.at(j).Equals(rootNote))
			indexes.push_back(FindNextInstanceOf(thirdsRootIndex, notes.at(j)));
	}

	//order notes in note array
	sort(indexes.begin(), indexes.end());
	vector<int> indexNotes;
	for (int i = 0; i < indexes.size(); i++) {
		indexNotes.push_back(THIRDS[indexes.at(i)]);
	}

	vector<Note> stackedNotes;
	for (int i = 0; i < indexNotes.size(); i++) {
		for (int j = 0; j <notes.size(); j++) {
			if (indexNotes.at(i) == notes.at(j).note) {
				stackedNotes.push_back(notes.at(j));
				break;
			}
		}
	}

	//map stackedNotes to chromatic equivalent
	vector<int> chromaticNotes;
	for (int i = 0; i < stackedNotes.size(); i++) {
		Note note = stackedNotes.at(i);
		chromaticNotes.push_back(mapToChromatic(note));
		//flip potential extensions up an octave
		if (i > 0 && chromaticNotes.at(i) < chromaticNotes.at(i-1))
			chromaticNotes.at(i) += 12;
	}

	//calculate intervals between each note
	vector<int> intervals;
	for (int i = 0; i < chromaticNotes.size() - 1; i++) {
		intervals.push_back(chromaticNotes.at(i + 1) - chromaticNotes.at(i));
	}

	//determine chord quality based on intervals
	int key = 0;
	int treeEnd = 0;
	int extensionPoint = 0;
	if (intervals.size() > 3) {
		treeEnd = 3;
		extensionPoint = 4;
	}
	else
		treeEnd = intervals.size();

	for (int i = 0; i < intervals.size(); i++) {
		int interval = intervals.at(i)*pow(10, (intervals.size() - i) - 1);
		if (intervals.at(i) >= 2 && intervals.at(i) <= 4)
			key += interval;
		else {
			extensionPoint = i;
			key /= (10 * (intervals.size()-i));
		}
	}

	quality = IntervalTree.at(key);

	//algorithmically determin extensions
	if (extensionPoint != 0) {
		int interval = 0;
		for (int j = 0; j < extensionPoint; j++) {
			interval += intervals.at(j);
		}
		
		for (int i = extensionPoint; i < intervals.size(); i++) {
			interval += intervals.at(i);
			if (floor(key / (10 * i)) == 3)
				quality.append(MinorExtensionTree.at(interval));
			else
				quality.append(MajorExtensionTree.at(interval));
		}
	}


	return quality;
}

void Analyzer::DetermineChord(NoteBank chord) {
	Note root = DetermineRoot(RemoveDuplicates(chord));
	string quality = DetermineQuality(RemoveStrictDuplicates(chord), root);
	cout << "quality: " << quality << "\n";

	return;
}

void Analyzer::LoopOverScore(Score score) {


	//create and index for each instrument channel starting at 0
	vector<int> markers(score.getNumInstruments(), 0);

	//loop through music score, stopping at each point where all channels stay constant
	bool endNotReached = true;
	int globalTracker[] = { 0, 0 }; //priority channel, index in that channel
	while (endNotReached) {
		NoteBank chord;
		NoteBank activeBank = score.getInstrument(globalTracker[0]).at(globalTracker[1]);

		//find earliest end time latest begin time for notebanks
		int minEnd = INT_MAX;
		int minEndInst = globalTracker[0];
		for (int m = 0; m < markers.size(); m++) {
			Channel instrument = score.getInstrument(m);
			int nbIndex = markers.at(m);

			if (instrument.at(nbIndex).getEndTime() < minEnd) {
				minEnd = instrument.at(nbIndex).getEndTime();
				minEndInst = m;
			}
		}

		//add all current notes to current chord
		for (int i = 0; i < score.getNumInstruments(); i++) {
			Channel instrument = score.getInstrument(i);
			int nbIndex = markers.at(i); //notebank index for given channel

			if (instrument.at(nbIndex).getEndTime() >= minEnd && instrument.at(nbIndex).getStartTime() <= activeBank.getStartTime())
				chord.addNotes(instrument.at(nbIndex).getNotes());
		}

		//Analyze chord
		DetermineChord(chord);

		//increment each markers with end before minBegin
		for (int m = 0; m < markers.size(); m++) {
			if (score.getInstrument(m).at(markers.at(m)).getEndTime() <= minEnd)
				markers.at(m)++;
		}

		globalTracker[0] = minEndInst;
		globalTracker[1] = markers.at(minEndInst);

		if (activeBank.getEndTime() >= score.getLength())
			endNotReached = false;

	}

}
}
