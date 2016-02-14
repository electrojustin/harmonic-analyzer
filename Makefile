all: main bend.o measure.o track.o note_effect.o beat_effect.o note.o beat.o
main: main.o bend.o measure.o track.o note_effect.o beat_effect.o note.o beat.o
	g++ -g bend.o measure.o track.o note_effect.o beat_effect.o note.o beat.o main.o -o main -std=gnu++11
main.o: main.cpp measure.h guitar_pro.h track.h beat.h note.h Analyzer.h Note.h NoteBank.h Score.h ChordMap.h 
	g++ -g -c main.cpp -std=gnu++11
bend.o: bend.cpp bend.h guitar_pro.h
	g++ -g -c bend.cpp
measure.o: measure.cpp measure.h guitar_pro.h
	g++ -g -c measure.cpp
track.o: track.cpp track.h guitar_pro.h
	g++ -g -c track.cpp
note_effect.o: note_effect.cpp note_effect.h guitar_pro.h bend.o
	g++ -g -c note_effect.cpp
beat_effect.o: beat_effect.cpp beat_effect.h guitar_pro.h bend.o
	g++ -g -c beat_effect.cpp
note.o: note.cpp note.h guitar_pro.h note_effect.o
	g++ -g -c note.cpp
beat.o: beat.cpp beat.h guitar_pro.h note.o beat_effect.o
	g++ -g -c beat.cpp
clean:
	rm *.o
