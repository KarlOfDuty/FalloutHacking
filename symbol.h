#ifndef SYMBOL_H
#define SYMBOL_H
#include<string>
#include"character.h"
using namespace std;
class Symbol : public Character
{
private:
	string symbol;
	bool used;
	bool bonus;
public:
	Symbol();
	Symbol(const string symbol, const int bonus);
	Symbol(const string symbol, const int used, const bool bonus);
	~Symbol();
	void use();
	bool isUsed()const;
	void setBonus(bool bonus);
	bool getBonus()const;
	string getString()const;
};
#endif
