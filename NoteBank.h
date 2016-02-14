#pragma once

namespace mike
{
using namespace mike;

class NoteBank {

private:
	std::vector<Note> notes;
	int startTime;
	int endTime;

public:
	NoteBank() {};
	NoteBank(vector<Note> argNotes);
	NoteBank(vector<int> letter, vector<int> mode);//creates noteBank using letter-mode
	NoteBank(vector<int> letter, vector<int> mode, int start, int end);

	vector<Note> getNotes() { return this->notes; }
	void setNotes(vector<Note> argNotes) { this->notes = argNotes; }
	void addNotes(vector<Note> argNotes) { for (int n = 0; n < argNotes.size(); n++) { this->notes.push_back(argNotes.at(n)); } }
	void addNote(Note argNote) { this->notes.push_back(argNote); }
	int getStartTime() { return startTime; }
	void setStartTime(int start) { this->startTime = start; }
	int getEndTime() { return endTime; }
	void setEndTime() { this->endTime = endTime; }
	void setRange(int start, int end) { this->startTime = start; this->endTime = end; }
};

NoteBank::NoteBank(vector<Note> argNotes) {
	this->notes = argNotes;
}

NoteBank::NoteBank(vector<int> letter, vector<int> mode) {
	if (letter.size() != mode.size()) {
		cout << "Notebank letter-mode constructor failed";
		return;
	}

	vector<Note> genNotes;
	for (int i = 0; i < letter.size(); i++)
		genNotes.push_back(Note(letter.at(i), mode.at(i)));

	this->notes = genNotes;
}

NoteBank::NoteBank(vector<int> letter, vector<int> mode, int start, int end) {
	if (letter.size() != mode.size()) {
		cout << "Notebank letter-mode constructor failed";
		return;
	}

	vector<Note> genNotes;
	for (int i = 0; i < letter.size(); i++)
		genNotes.push_back(Note(letter.at(i), mode.at(i)));

	this->notes = genNotes;

	this->startTime = start;
	this->endTime = end;
}
}
