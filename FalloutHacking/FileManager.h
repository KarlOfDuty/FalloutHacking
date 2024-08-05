#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include<string>
#include<fstream>
using namespace std;
class HackingHandler;
static class FileManager
{
private:
	static string *saveNames;
	static int *points;
	static int numberOfSaves;
	static int maxNumberOfSaves;
public:
	static void saveGame(const string filePath, HackingHandler &hackingHandler);
	static void loadGame(const string filePath, HackingHandler &hackingHandler);
	static void saveList(const string filePath, HackingHandler &hackingHandler);
	static void loadList();
	static void deleteSave(const int save, HackingHandler &hackingHandler);
	static int getNumberOfSaves();
	static int* getPoints();
	static string* getSaveNames();
	static int getMaxNumberOfSaves();
};
#endif