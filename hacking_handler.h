#ifndef HACKINGHANDLER_H
#define HACKINGHANDLER_H
#include"character.h"
#include"symbol.h"
#include"word.h"
#include<string>
#include"file_manager.h"
using namespace std;
class HackingHandler
{
private:
	const string EIGHTWORDS[20] = { 
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
		"DUNGEONS" };
	const string SYMBOLS[30] = { 
		".",",","/","\\","?","|","_","-","+","%",
		"$","*",":","!","@","\"","'","#","=",";",
		"`","^","{","}","[","]","(",")","<",">" };
	Character* *elements;
	int wordLength;
	int amountOfWords;
	int rowLength;
	int numberOfRows;
	string correctWord;
	int attemptsRemaining;
	string saveName;
	int *attempts;
	int arrayLength;
	int attemptNumber;
	int points;
	int removedWords;
	bool placeWord(const int minPos, const int maxPos);
public:
	HackingHandler();
	~HackingHandler();
	void startRound();
	int checkWord(const int wordPos);
	int checkSymbol(const int symbolPos,const int row);
	void save();
	void load();
	void removeRandomWord();

	Character** getElements();
	int getWordLength()const;
	int getAmountOfWords()const;
	int getRowLength()const;
	int getNumberOfRows()const;
	string getCorrectWord()const;
	int getAttemptsRemaining()const;
	int* getAttempts();
	int getArrayLength()const;
	int getAttemptNumber()const;
	string getSaveName()const;
	int getPoints()const;
	int getRemovedWords()const;

	void setElements(Character* *elements);
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
};
#endif