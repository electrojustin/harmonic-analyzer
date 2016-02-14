#pragma once

namespace mike
{

class Score {

private:
	vector<Channel> instruments;
	int numInstruments;
	int length;

public:
	Score() {};

	vector<Channel> getInstruments() { return this->instruments; }
	Channel getInstrument(int index) { return this->instruments.at(index); }
	void addInstrument(Channel instrument) { this->instruments.push_back(instrument); this->numInstruments++; }
	int getNumInstruments() { return numInstruments; }
	int getLength() { return this->length; }
	void setLength(int argLength) { this->length = argLength; }

};
}
