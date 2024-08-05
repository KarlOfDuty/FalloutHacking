#include "HackingHandler.h"
#include<ctime>
HackingHandler::HackingHandler()
{
	srand((unsigned int)time(0));
	amountOfWords = 10;
	saveName = "";
	points = 0;
	startRound();
}

HackingHandler::~HackingHandler()
{
	//Deleting arrays
	delete[] attempts;
	for (int i = 0; i < arrayLength; i++)
	{
		delete elements[i];
	}
	delete[] elements;
}

bool HackingHandler::placeWord(const int minPos, const int maxPos)
{
	//Checks if there is room in the array to place a new word at a specific position by looking from minPos to maxPos
	Word *testWord;
	bool temp = true;
	if (minPos < maxPos)
	{
		temp = placeWord(minPos + 1, maxPos);
	}
	if (temp == false || (testWord = dynamic_cast<Word*>(elements[minPos])))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void HackingHandler::startRound()
{
	//Starts a new round
	wordLength = 8;
	rowLength = 12;
	numberOfRows = 20; //double in total as of the 2 columns
	arrayLength = rowLength * numberOfRows * 2;
	attemptsRemaining = 4;
	attemptNumber = 0;
	removedWords = 0;
	//Deletes the arrays if they exist
	if (elements != nullptr)
	{
		delete[] attempts;
		for (int i = 0; i < arrayLength; i++)
		{
			delete elements[i];
		}
		delete[] elements;
	}
	//Recreates the arrays
	elements = new Character*[arrayLength];
	attempts = new int[arrayLength];
	for (int i = 0; i < arrayLength; i++)
	{
		attempts[i] = 0;
		elements[i] = new Symbol(SYMBOLS[rand()%30],rand()%10);
	}
	
	//Creates an array of unique words to be added to the elements array
	string *randWords = new string[amountOfWords];
	for (int i = 0; i < amountOfWords; i++)
	{
		randWords[i] = EIGHTWORDS[rand() % 20];
		bool duplicate = false;
		for (int j = 0; j < i; j++)
		{
			if (randWords[i] == randWords[j])
			{
				duplicate = true;
			}
		}
		if (duplicate == true)
		{
			i--;
		}
	}
	int temp = 0;
	//Places the words randomly in the list with at least one symbol between them
	while (temp < amountOfWords)
	{
		int tempPos = rand() % arrayLength;

		int minPos = tempPos - (wordLength + 1);
		int maxPos = tempPos + (wordLength + 1);
		if (minPos < 0)
		{
			minPos = 0;
		}
		if (maxPos < arrayLength)
		{
			if (placeWord(minPos,maxPos))
			{
				for (int i = 0; i < wordLength; i++)
				{
					//Creates one Word object for every letter as the "elements" array contains one letter/symbol per position.
					//As every Word object only represents one of it's letters, they also get a position integer indicating which letter should be drawn.
					elements[tempPos + i] = new Word(randWords[temp]);
					dynamic_cast<Word*>(elements[tempPos + i])->setPos(i);
				}
				temp++;
			}
		}
	}
	correctWord = randWords[rand() % amountOfWords];
	delete[] randWords;
}

int HackingHandler::checkWord(const int wordPos)
{
	//Checks how many letters are correct in this word
	int result = dynamic_cast<Word*>(elements[wordPos])->compareWord(correctWord);
	return result;
}

int HackingHandler::checkSymbol(const int symbolPos, const int row)
{
	//row is the number of elements after the current symbol on the same row, not the entire row
	Symbol *thisSymbol;
	Symbol *compareSymbol;
	Word *tempWord;
	//Checks if there is a corresponding symbol on the same row as this one
	if ((thisSymbol = dynamic_cast<Symbol*>(elements[symbolPos])) && row > 0)
	{
		if (thisSymbol->getString() == "{")
		{
			for (int i = 1; i < row; i++)
			{
				if ((compareSymbol = dynamic_cast<Symbol*>(elements[symbolPos + i])) && compareSymbol->getString() == "}")
				{
					//Returns the position of the corresponding symbol
					return symbolPos + i;
				}
				else if (tempWord = dynamic_cast<Word*>(elements[symbolPos + i]))
				{
					return -1;
				}
			}
		}
		else if (thisSymbol->getString() == "<")
		{
			for (int i = 1; i < row; i++)
			{
				if ((compareSymbol = dynamic_cast<Symbol*>(elements[symbolPos + i])) && compareSymbol->getString() == ">")
				{
					//Returns the position of the corresponding symbol
					return symbolPos + i;
				}
				else if (tempWord = dynamic_cast<Word*>(elements[symbolPos + i]))
				{
					return -1;
				}
			}
		}
		else if (thisSymbol->getString() == "[")
		{
			for (int i = 1; i < row; i++)
			{
				if ((compareSymbol = dynamic_cast<Symbol*>(elements[symbolPos + i])) && compareSymbol->getString() == "]")
				{
					//Returns the position of the corresponding symbol
					return symbolPos + i;
				}
				else if (tempWord = dynamic_cast<Word*>(elements[symbolPos + i]))
				{
					return -1;
				}
			}
		}
		else if (thisSymbol->getString() == "(")
		{
			for (int i = 1; i < row; i++)
			{
				if ((compareSymbol = dynamic_cast<Symbol*>(elements[symbolPos + i])) && compareSymbol->getString() == ")")
				{
					//Returns the position of the corresponding symbol
					return symbolPos + i;
				}
				else if (tempWord = dynamic_cast<Word*>(elements[symbolPos + i]))
				{
					return -1;
				}
			}
		}
	}
	return -1;
}

void HackingHandler::save()
{
	//Saves the game
	FileManager::saveGame(saveName, *(this));
	FileManager::saveList(saveName, *(this));
}

void HackingHandler::load()
{
	//Loads a game
	FileManager::loadGame(saveName,*(this));
}

void HackingHandler::removeRandomWord()
{
	//Removes a random word
	Word *tempWord;
	bool done = false;
	if (removedWords < amountOfWords - 1)
	{
		//Continues until a word is removed
		while (done == false)
		{
			for (int i = 0; i < arrayLength && done == false; i++)
			{
				if (tempWord = dynamic_cast<Word*>(elements[i]))
				{
					//If the random number is 0, the word is removed
					if (rand() % amountOfWords == 0 && tempWord->getString() != correctWord && tempWord->isRemoved() == false)
					{
						for (int j = i; j < i + wordLength; j++)
						{
							dynamic_cast<Word*>(elements[j])->remove();
						}
						done = true;
					}
					else
					{
						i = i + wordLength;
					}
				}
			}
		}
		removedWords++;
	}
}

//Get things
Character** HackingHandler::getElements()
{
	return elements;
}
int HackingHandler::getWordLength()const
{
	return wordLength;
}
int HackingHandler::getAmountOfWords()const
{
	return amountOfWords;
}
int HackingHandler::getRowLength()const
{
	return rowLength;
}
int HackingHandler::getNumberOfRows()const
{
	return numberOfRows;
}
string HackingHandler::getCorrectWord()const
{
	return correctWord;
}
int HackingHandler::getAttemptsRemaining()const
{
	return attemptsRemaining;
}
int *HackingHandler::getAttempts()
{
	return attempts;
}
int HackingHandler::getArrayLength()const
{
	return arrayLength;
}
int HackingHandler::getAttemptNumber()const
{
	return attemptNumber;
}
string HackingHandler::getSaveName()const
{
	return saveName;
}
int HackingHandler::getPoints() const
{
	return points;
}
int HackingHandler::getRemovedWords() const
{
	return removedWords;
}

//Set things
void HackingHandler::setElements(Character* *elements)
{
	//The object class calling this function is responsible for destroying the last array
	this->elements = elements;
}
void HackingHandler::setWordLength(const int wordLength)
{
	this->wordLength = wordLength;
}
void HackingHandler::setAmountOfWords(const int amountOfWords)
{
	this->amountOfWords = amountOfWords;
}
void HackingHandler::setRowLength(const int rowLength)
{
	this->rowLength = rowLength;
}
void HackingHandler::setNumberOfRows(const int numberOfRows)
{
	this->numberOfRows = numberOfRows;
}
void HackingHandler::setCorrectWord(const string correctWord)
{
	this->correctWord = correctWord;
}
void HackingHandler::setAttemptsRemaining(const int attemptsRemaining)
{
	this->attemptsRemaining = attemptsRemaining;
}
void HackingHandler::setAttempts(int *attempts)
{
	//The object class calling this function is responsible for destroying the last array
	this->attempts = attempts;
}
void HackingHandler::setArrayLength(const int arrayLength)
{
	this->arrayLength = arrayLength;
}
void HackingHandler::setAttemptNumber(const int attemptNumber)
{
	this->attemptNumber = attemptNumber;
}
void HackingHandler::setSaveName(const string saveName)
{
	this->saveName = saveName;
}
void HackingHandler::setPoints(const int points)
{
	this->points = points;
}
void HackingHandler::setRemovedWords(int removedWords)
{
	this->removedWords = removedWords;
}
