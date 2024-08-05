#ifndef CHARACTER_H
#define CHARACTER_H
#include<string>
using namespace std;
class Character
{
public:
	Character();
	virtual ~Character();
	virtual string getString()const = 0;
};

#endif