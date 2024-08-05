#include "Word.h"

Word::Word(const string word)
{
	this->word = word;
	this->removed = false;
	this->pos = 0;
}

Word::Word(const string word, const int pos, const bool removed)
{
	//Constructor used when attributes are copied from a save file
	this->word = word;
	this->pos = pos;
	this->removed = removed;
}

Word::~Word()
{

}

int Word::compareWord(const string &correctWord)const
{
	//Returns the number of characters are correct and in the correct position
	int result = 0;
	for (int i = 0; i < word.length(); i++)
	{
		if (this->word[i] == correctWord[i])
		{
			result++;
		}
	}
	return result;
}

void Word::remove()
{
	this->removed = true;
}

void Word::setPos(const int pos)
{
	this->pos = pos;
}

string Word::getString()const
{
	return this->word;
}
int Word::getPos()const
{
	return this->pos;
}

bool Word::isRemoved()const
{
	return removed;
}
