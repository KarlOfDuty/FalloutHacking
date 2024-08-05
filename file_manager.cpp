#include "file_manager.h"
#include "hacking_handler.h"
int FileManager::maxNumberOfSaves = 25;
//These get filled whenever loadList is called
int FileManager::numberOfSaves = 0;
int* FileManager::points = nullptr;
string* FileManager::saveNames = nullptr;
void FileManager::saveGame(const string filePath, HackingHandler &hackingHandler)
{
	//Saves the game to a file
	ofstream saveFile;
	saveFile.open(filePath);
	saveFile << hackingHandler.getWordLength() << endl;
	saveFile << hackingHandler.getAmountOfWords() << endl;
	saveFile << hackingHandler.getRowLength() << endl;
	saveFile << hackingHandler.getNumberOfRows() << endl;
	saveFile << hackingHandler.getCorrectWord() << endl;
	saveFile << hackingHandler.getAttemptsRemaining() << endl;
	saveFile << hackingHandler.getArrayLength() << endl;
	saveFile << hackingHandler.getAttemptNumber() << endl;
	saveFile << hackingHandler.getSaveName() << endl;
	saveFile << hackingHandler.getPoints() << endl;
	saveFile << hackingHandler.getRemovedWords() << endl;
	//Saving the arrays
	Character **elements = hackingHandler.getElements();
	Symbol *tempSymbol;
	Word *tempWord;
	for (int i = 0; i < hackingHandler.getArrayLength(); i++)
	{
		if (tempSymbol = dynamic_cast<Symbol*>(hackingHandler.getElements()[i]))
		{
			saveFile << "symbol" << endl;
			saveFile << tempSymbol->getString() << endl;
			saveFile << tempSymbol->isUsed() << endl;
			saveFile << tempSymbol->getBonus() << endl;
		}
		else if (tempWord = dynamic_cast<Word*>(hackingHandler.getElements()[i]))
		{
			saveFile << "word" << endl;
			saveFile << tempWord->getString() << endl;
			saveFile << tempWord->getPos() << endl;
			saveFile << tempWord->isRemoved() << endl;
		}
	}
	int *attempts = hackingHandler.getAttempts();
	for (int i = 0; i < hackingHandler.getArrayLength(); i++)
	{
		saveFile << attempts[i] << endl;
	}
	saveFile.close();
}

void FileManager::loadGame(const string filePath, HackingHandler &hackingHandler)
{
	//Loads a game from a file
	ifstream saveFile;
	//Deleteing existing arrays
	Character* *elements = hackingHandler.getElements();
	for (int i = 0; i < hackingHandler.getArrayLength(); i++)
	{
		delete elements[i];
	}
	delete[] elements;
	int *attempts = hackingHandler.getAttempts();
	delete[] attempts;

	int tempInt = 0;
	string tempString = "";
	saveFile.open(filePath);
	//Loading variables
	saveFile >> tempInt; hackingHandler.setWordLength(tempInt);
	saveFile >> tempInt; hackingHandler.setAmountOfWords(tempInt);
	saveFile >> tempInt; hackingHandler.setRowLength(tempInt);
	saveFile >> tempInt; hackingHandler.setNumberOfRows(tempInt);
	saveFile >> tempString; hackingHandler.setCorrectWord(tempString);
	saveFile >> tempInt; hackingHandler.setAttemptsRemaining(tempInt);
	saveFile >> tempInt; hackingHandler.setArrayLength(tempInt);
	saveFile >> tempInt; hackingHandler.setAttemptNumber(tempInt);
	saveFile >> tempString;  hackingHandler.setSaveName(tempString);
	saveFile >> tempInt; hackingHandler.setPoints(tempInt);
	saveFile >> tempInt; hackingHandler.setRemovedWords(tempInt);
	//Creating and filling arrays
	string word = "";
	int pos = 0;
	bool removed = false;
	string symbol = "";
	bool used;
	bool bonus;
	elements = new Character*[hackingHandler.getArrayLength()];
	hackingHandler.setElements(elements);
	for (int i = 0; i < hackingHandler.getArrayLength(); i++)
	{
		saveFile >> tempString;
		if (tempString == "word")
		{
			saveFile >> word;
			saveFile >> pos;
			saveFile >> removed;
			elements[i] = new Word(word, pos, removed);
		}
		else if (tempString == "symbol")
		{
			saveFile >> symbol;
			saveFile >> used;
			saveFile >> bonus;
			elements[i] = new Symbol(symbol, used);
		}
	}
	attempts = new int[hackingHandler.getArrayLength()];
	hackingHandler.setAttempts(attempts);
	for (int i = 0; i < hackingHandler.getArrayLength(); i++)
	{
		saveFile >> attempts[i];
	}
	saveFile.close();
}

void FileManager::saveList(const string filePath, HackingHandler &hackingHandler)
{
	//Loads the existing file list
	loadList();
	//Checks if this save file already exists
	int foundPos = -1;
	for (int i = 0; i < numberOfSaves; i++)
	{
		if (saveNames[i] == filePath)
		{
			foundPos = i;
		}
	}
	ofstream saveList;
	saveList.open("savelist.txt");
	//If this save already exists, it's entry is just overwritten
	if (foundPos > -1)
	{
		saveList << numberOfSaves << endl;
		for (int i = 0; i < numberOfSaves; i++)
		{
			if (i == foundPos)
			{
				saveList << filePath << endl;
				saveList << hackingHandler.getPoints() << endl;
			}
			else
			{
				saveList << saveNames[i] << endl;
				saveList << points[i] << endl;
			}
		}
	}
	//Otherwise it's added to the end of the list
	else
	{
		saveNames[numberOfSaves] = filePath;
		points[numberOfSaves] = hackingHandler.getPoints();
		numberOfSaves++;
		saveList << numberOfSaves << endl;
		for (int i = 0; i < numberOfSaves; i++)
		{
			saveList << saveNames[i] << endl;
			saveList << points[i] << endl;
		}

	}
	saveList.close();
}

void FileManager::loadList()
{
	//Loads the file list
	ifstream saveList;
	saveList.open("savelist.txt");
	saveList >> numberOfSaves;

	delete[] saveNames;
	delete[] points;
	saveNames = new string[numberOfSaves + 1];
	points = new int[numberOfSaves + 1];

	if (numberOfSaves > 0)
	{
		for (int i = 0; i < numberOfSaves; i++)
		{
			saveList >> saveNames[i];
			saveList >> points[i];
		}
	}
	saveList.close();
}

void FileManager::deleteSave(const int save, HackingHandler &hackingHandler)
{
	//Loads the list of all files
	loadList();
	ofstream saveList;
	saveList.open("savelist.txt");
	//Decreases the number of files in the list
	numberOfSaves--;
	saveList << numberOfSaves << endl;
	for (int i = 0; i < numberOfSaves + 1; i++)
	{
		if (i != save)
		{
			//Writes all saves except the deleted one back to the savelist file
			saveList << saveNames[i] << endl;
			saveList << points[i] << endl;
		}
		else
		{
			//Deletes the actual savefile
			remove(saveNames[save].c_str());
		}
	}
	//Loads the new list without the deleted file
	loadList();
}

//Get things
int FileManager::getNumberOfSaves()
{
	return numberOfSaves;
}
int* FileManager::getPoints()
{
	loadList();
	return points;
}
string* FileManager::getSaveNames()
{
	loadList();
	return saveNames;
}
int FileManager::getMaxNumberOfSaves()
{
	return maxNumberOfSaves;
}