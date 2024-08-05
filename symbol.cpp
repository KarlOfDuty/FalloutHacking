#include "symbol.h"

Symbol::Symbol()
{
	this->symbol = "";
	this->used = true;
}

Symbol::Symbol(const string symbol, const int bonus)
{
	this->symbol = symbol;
	//If the symbol cannot be used it will be set as if it's already used
	if (symbol != "{" && symbol != "(" && symbol != "[" && symbol != "<")
	{
		this->used = true;
	}
	else
	{
		this->used = false;
	}
	this->bonus = bonus;
}

Symbol::Symbol(const string symbol, const int used, const bool bonus)
{
	//Constructor used when attributes are copied from a savefile
	this->symbol = symbol;
	this->used = used;
	this->bonus = bonus;
}

Symbol::~Symbol()
{

}

void Symbol::use()
{
	this->used = true;
}

bool Symbol::isUsed()const
{
	return this->used;
}

void Symbol::setBonus(const bool bonus)
{
	this->bonus = bonus;
}

bool Symbol::getBonus() const
{
	return bonus;
}

string Symbol::getString()const
{
	return this->symbol;
}
