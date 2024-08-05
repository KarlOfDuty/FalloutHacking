#ifndef WORD_H
#define WORD_H
#include<string>
#include"Character.h"
using namespace std;
class Word : public Character
{
private:
	string word;
	int pos;
	bool removed;
public:
	Word(const string word);
	Word(const string word, const int pos, const bool removed);
	~Word();
	int compareWord(const string &correctWord)const;
	void remove();
	void setPos(const int pos);
	string getString()const;
	int getPos()const;
	bool isRemoved()const;
};
#endif
