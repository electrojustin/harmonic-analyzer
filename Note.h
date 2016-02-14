#pragma once

namespace mike
{

struct Note {
	Note() {};
	Note(int argNote, int argMode);

	bool Equals(Note note);
	bool StrictEquals(Note note);


	//note letter (0-6->A-G)
	int note;
	//note quality (flat (-1), natrual(0), sharp(1))
	int mode;


};

Note::Note(int argNote, int argMode)
{
	if (argNote < 8 && argNote >= 0)
		this->note = argNote;
	else
		this->note = -1;

	if (argMode >= -1 && argMode <= 1)
		this->mode = argMode;
}

bool Note::Equals(Note note) {
	if (this->note == note.note)
		return true;
	else
		return false;
}

bool Note::StrictEquals(Note note) {
	if (this->note == note.note && this->mode == note.mode)
		return true;
	else
		return false;
}
}
