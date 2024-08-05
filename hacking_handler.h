#pragma once
#include "character.h"
#include "symbol.h"
#include "word.h"
#include <string>
#include "file_manager.h"

class HackingHandler
{
public:
    HackingHandler();
    ~HackingHandler();
    void startRound();
    int checkWord(int wordPos);
    int checkSymbol(int symbolPos, int row);
    void save();
    void load();
    void removeRandomWord();


    Character** getElements();
    int getWordLength() const;
    int getAmountOfWords() const;
    int getRowLength() const;
    int getNumberOfRows() const;
    string getCorrectWord() const;
    int getAttemptsRemaining() const;
    int* getAttempts();
    int getArrayLength() const;
    int getAttemptNumber() const;
    string getSaveName() const;
    int getPoints() const;
    int getRemovedWords() const;

    void setElements(Character** elements);
    void setWordLength(int wordLength);
    void setAmountOfWords(int amountOfWords);
    void setRowLength(int rowLength);
    void setNumberOfRows(int numberOfRows);
    void setCorrectWord(string correctWord);
    void setAttemptsRemaining(int attemptsRemaining);
    void setAttempts(int* attempts);
    void setArrayLength(int arrayLength);
    void setAttemptNumber(int attemptNumber);
    void setSaveName(string saveName);
    void setPoints(int points);
    void setRemovedWords(int removedWords);

private:
	const string EIGHTWORDS[20] =
    {
		"ABRIDGED",
		"ACCEPTED",
		"ACCURACY",
		"AFFINITY",
		"ANCIENTS",
		"APPENDIX",
		"AUTONOMY",
		"BROADEST",
		"BUILDING",
		"BROTHERS",
		"CABBAGES",
		"CARRYING",
		"CENSORED",
		"CLEVERLY",
		"COLLIDED",
		"COLONELS",
		"DEVIATED",
		"DIFFUSED",
		"DIRECTOR",
		"DUNGEONS"
    };

	const string SYMBOLS[30] =
    {
		".",",","/","\\","?","|","_","-","+","%",
		"$","*",":","!","@","\"","'","#","=",";",
		"`","^","{","}","[","]","(",")","<",">"
    };

	Character** elements { nullptr };
	int wordLength { 0 };
	int amountOfWords { 0 };
	int rowLength { 0 };
	int numberOfRows { 0 };
	string correctWord {};
	int attemptsRemaining { 0 };
	string saveName {};
	int* attempts { nullptr };
	int arrayLength { 0 };
	int attemptNumber { 0 };
	int points { 0 };
	int removedWords { 0 };

	bool placeWord(int minPos, int maxPos);
};