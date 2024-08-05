#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<string>
#include<ctime>
#include"HackingHandler.h"
#include"MouseControl.h"
#include"FileManager.h"
using namespace sf;
int mainMenu(RenderWindow &window, HackingHandler &hackingHandler, const Font &font);
int loadMenu(RenderWindow &window, HackingHandler &hackingHandler, int &choosenSave, const Font &font);
void drawNewGameMenu(RenderWindow &window, string allTextEntered, const Font &font);
void drawEndGameMenu(RenderWindow &window, HackingHandler &hackingHandler, const Font &font);
void drawStaticObjects(RenderWindow &window, HackingHandler &hackingHandler, const Font &font);
void drawAttempts(RenderWindow &window, int attemptsLeft);
void drawSymbols(RenderWindow &window, HackingHandler &hackingHandler, const Font &font);
void drawMainMenu(RenderWindow &window, const Font &font);
void drawLoadMenu(RenderWindow &window, int choosenSave, const Font &font);
void drawOutput(RenderWindow &window, HackingHandler &hackingHandler, const Font &font);
int main()
{
	//Variables//
	RenderWindow window(VideoMode(1024, 768), "Fallout Hacking");
	window.setMouseCursorVisible(false);

	View fixed = window.getView();
	window.setFramerateLimit(60);

	Font font;
	font.loadFromFile("fixedsys.ttf");

	RectangleShape background(Vector2f(1024, 768));
	background.setFillColor(Color(20, 20, 20));

	RectangleShape centreBackground(Vector2f(924, 668));
	centreBackground.setPosition(Vector2f(50.5, 50.5));
	centreBackground.setFillColor(Color(0, 30, 0));

	HackingHandler hackingHandler;

	int choice = 0;
	string allTextEntered = "";
	int choosenSave = -1;
	bool victory = false;
	//////////////
	while (window.isOpen())
	{	
		window.clear();
		window.setView(fixed);
		window.draw(background);
		window.draw(centreBackground);
		Event event;
		string textEntered = "";
		bool backSpace = false;
		bool returnPressed = false;
		MouseControl::setMouseClick(false);
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				//Detects the left mouse key
				MouseControl::setMouseClick(true);
			}
			else if (event.type == event.TextEntered)
			{
				//Detects typing
				if ((event.text.unicode < 123 && event.text.unicode > 96) || (event.text.unicode < 91 && event.text.unicode > 64) || (event.text.unicode < 58 && event.text.unicode > 47) || event.text.unicode == 32)
				{
					textEntered = event.text.unicode;
				}
				else if (event.text.unicode == 8)
				{
					backSpace = true;
				}
			}
			else if (event.type == event.KeyReleased)
			{
				//Detects the enter key
				if (event.key.code == Keyboard::Return)
				{
					returnPressed = true;
				}
				//Detects the Escape key and exits
				else if (event.key.code == Keyboard::Escape)
				{
					if (hackingHandler.getSaveName() != "" && choice == 1)
					{
						hackingHandler.save();
					}
					else if (choice == 0)
					{
						window.close();
					}
					choice = 0;
					hackingHandler.setSaveName("");
				}
			}
		}
		//Go to main menu
		if (choice == 0)
		{
			choice = mainMenu(window, hackingHandler, font);
		}
		//Go to game
		else if (choice == 1)
		{
			//New game menu
			if (hackingHandler.getSaveName() == "")
			{
				//Typing
				if (textEntered != "" && allTextEntered.length() < 32)
				{
					allTextEntered.append(textEntered);
					textEntered = "";
				}
				//Pressing backspace, erasing a character
				else if (backSpace == true && allTextEntered.length() > 0)
				{
					allTextEntered.pop_back();
					backSpace = false;
				}
				//Clicking back
				else if (MouseControl::newGameMenuButtonDetection(window) == 1 && MouseControl::isMouseClicked() == true)
				{
					MouseControl::setMouseClick(false);
					choice = 0;
				}
				//Clicking on the start button
				else if (MouseControl::newGameMenuButtonDetection(window) == 2 && MouseControl::isMouseClicked() == true && allTextEntered.length() > 0 && FileManager::getNumberOfSaves() < FileManager::getMaxNumberOfSaves())
				{
					MouseControl::setMouseClick(false);
					hackingHandler.setSaveName(allTextEntered + ".save");
					hackingHandler.startRound();
					hackingHandler.save();
					returnPressed = false;
				}
				//Pressing enter, starting
				else if (returnPressed == true && allTextEntered.length() > 0 && FileManager::getNumberOfSaves() < FileManager::getMaxNumberOfSaves())
				{
					hackingHandler.setSaveName(allTextEntered + ".save");
					hackingHandler.startRound();
					hackingHandler.save();
					returnPressed = false;
				}
				drawNewGameMenu(window, allTextEntered, font);
			}
			//Game start
			else
			{	
				//Drawing objects used in the game
				drawStaticObjects(window, hackingHandler,font);
				drawAttempts(window,hackingHandler.getAttemptsRemaining());
				drawSymbols(window, hackingHandler, font);
				drawOutput(window, hackingHandler, font);
				//Game playing
				if (victory == false && hackingHandler.getAttemptsRemaining() > 0)
				{
					Word *tempWord;
					Symbol *tempSymbol;
					Character* *elements = hackingHandler.getElements();
					int *attempts = hackingHandler.getAttempts();
					for (int i = 0; i < hackingHandler.getArrayLength(); i++)
					{
						if (MouseControl::isMouseClicked() == true && MouseControl::mouseDetection(window,hackingHandler) == i)
						{
							if (tempSymbol = dynamic_cast<Symbol*>(elements[i]))
							{
								if (hackingHandler.checkSymbol(i, hackingHandler.getRowLength() - (i % hackingHandler.getRowLength())) > - 1 && tempSymbol->isUsed() == false)
								{
									tempSymbol->use();
									//The attemptNumber is increased by 1
									hackingHandler.setAttemptNumber(hackingHandler.getAttemptNumber() + 1);
									//The attempts array is filled with 0 in every position at the start of each round
									//Then, for example, if it's the player's 3rd attempt and they click on the symbol in the 25th position of the elements array,
									//this will place a 3 int the 25th position of this array, so everything can be drawn in order even after loading.
									attempts[i] = hackingHandler.getAttemptNumber();
									if (tempSymbol->getBonus() == 0)
									{
										hackingHandler.setAttemptsRemaining(4);
									}
									else
									{
										hackingHandler.removeRandomWord();
									}
								}
							}
							else if (tempWord = dynamic_cast<Word*>(elements[i]))
							{
								//Checks if the word is the correct one
								if (hackingHandler.checkWord(i) == hackingHandler.getWordLength())
								{
									victory = true;
								}
								//If it's wrong it's added to the attempts array
								else if (tempWord->isRemoved() == false)
								{
									hackingHandler.setAttemptsRemaining(hackingHandler.getAttemptsRemaining() - 1);
									hackingHandler.setAttemptNumber(hackingHandler.getAttemptNumber() + 1);
									attempts[i] = hackingHandler.getAttemptNumber();
								}
							}
							MouseControl::setMouseClick(false);
						}
					}
					MouseControl::drawMouseHighlighting(window,hackingHandler,font);
				}
				//Next round menu
				else
				{
					drawEndGameMenu(window, hackingHandler, font);
					//Back to main menu
					if (MouseControl::newGameMenuButtonDetection(window) == 1 && MouseControl::isMouseClicked() == true)
					{
						victory = false;
						hackingHandler.setPoints(hackingHandler.getPoints() + hackingHandler.getAttemptsRemaining());
						//If the amount of words haven't reached 15 yet it's increased by 1
						if (hackingHandler.getAmountOfWords() < 15)
						{
							hackingHandler.setAmountOfWords(hackingHandler.getAmountOfWords() + 1);
						}
						hackingHandler.startRound();
						hackingHandler.save();
						hackingHandler.setSaveName("");
						choice = 0;
					}
					//Goes to next round
					else if (MouseControl::newGameMenuButtonDetection(window) == 2 && MouseControl::isMouseClicked() == true)
					{
						victory = false;
						hackingHandler.setPoints(hackingHandler.getPoints() + hackingHandler.getAttemptsRemaining());
						//If the amount of words haven't reached 15 yet it's increased by 1
						if (hackingHandler.getAmountOfWords() < 15)
						{
							hackingHandler.setAmountOfWords(hackingHandler.getAmountOfWords() + 1);
						}
						hackingHandler.startRound();
						hackingHandler.save();
					}
				}
			}
		}
		//Go to load menu
		else if (choice == 2)
		{
			choice = loadMenu(window, hackingHandler, choosenSave, font);
		}
		//Exit game
		else if (choice == 3)
		{
			window.close();
		}
		MouseControl::drawMousePointer(window);
		window.display();
	}
	return 0;
}
int mainMenu(RenderWindow &window,HackingHandler &hackingHandler, const Font &font)
{
	//Manages the main menu
	int choice = 0;
	drawMainMenu(window, font);
	//Switches to other menus based on mouse clicking
	if (MouseControl::isMouseClicked() == true)
	{
		if (MouseControl::mainMenuButtonDetection(window) == 1)
		{
			choice = 1;
			MouseControl::setMouseClick(false);
		}
		else if (MouseControl::mainMenuButtonDetection(window) == 2)
		{
			choice = 2;
			MouseControl::setMouseClick(false);
		}
		else if (MouseControl::mainMenuButtonDetection(window) == 3)
		{
			choice = 3;
			MouseControl::setMouseClick(false);
		}
	}
	return choice;
}
int loadMenu(RenderWindow &window, HackingHandler &hackingHandler, int &choosenSave, const Font &font)
{
	//Manages the load menu
	int choice = 2;
	drawLoadMenu(window, choosenSave, font);
	FileManager::loadList();
	//Acts according to where the mouse clicks
	if (MouseControl::isMouseClicked() == true)
	{
		//Back button
		if (MouseControl::loadMenuButtonDetection(window) == -2)
		{
			choice = 0;
			MouseControl::setMouseClick(false);
		}
		//Delete button
		else if (MouseControl::loadMenuButtonDetection(window) == -3 && choosenSave > -1)
		{
			MouseControl::setMouseClick(false);
			FileManager::deleteSave(choosenSave,hackingHandler);
		}
		//Load button
		else if(MouseControl::loadMenuButtonDetection(window) == -4 && choosenSave > - 1)
		{
			choice = 1;
			MouseControl::setMouseClick(false);
			FileManager::loadGame(FileManager::getSaveNames()[choosenSave], hackingHandler);
		}
		for (int i = 0; i < FileManager::getNumberOfSaves(); i++)
		{
			if (MouseControl::loadMenuButtonDetection(window) == i)
			{
				choosenSave = i;
			}
			MouseControl::setMouseClick(false);
		}
	}
	return choice;
}
void drawEndGameMenu(RenderWindow &window, HackingHandler &hackingHandler, const Font &font)
{
	//Draws the end game menu
	RectangleShape endGameWindow(Vector2f(500, 200));
	endGameWindow.setPosition(262, 184);
	endGameWindow.setFillColor(Color(5, 50, 5));
	Text endGameWindowText("Points Earned:" + to_string(hackingHandler.getAttemptsRemaining()), font);
	endGameWindowText.setPosition(340, 230);
	endGameWindowText.setCharacterSize(40);
	endGameWindowText.setFillColor(Color(30, 220, 30));

	RectangleShape endGameWindowTop(Vector2f(500, 25));
	endGameWindowTop.setPosition(262, 184);
	endGameWindowTop.setFillColor(Color(5, 130, 5));
	Text endGameWindowTopText("Game Over", font);
	endGameWindowTopText.setPosition(440, 178);
	endGameWindowTopText.setCharacterSize(24);
	endGameWindowTopText.setFillColor(Color(0, 30, 0));

	RectangleShape menuButton(Vector2f(200, 50));
	menuButton.setPosition(282, 315);
	Text menuText("Menu", font);
	menuText.setCharacterSize(30);
	menuText.setPosition(350, 320);

	RectangleShape playButton(Vector2f(200, 50));
	playButton.setPosition(540, 315);
	Text playText("Next Round", font);
	playText.setCharacterSize(30);
	playText.setPosition(555, 320);
	//The buttons are in the same place as on the new game menu, so it uses the same function
	if (MouseControl::newGameMenuButtonDetection(window) == 1)
	{
		menuButton.setFillColor(Color(30, 220, 30));
		menuText.setFillColor(Color(5, 30, 5));
	}
	else
	{
		menuButton.setFillColor(Color(5, 120, 5));
		menuText.setFillColor(Color(30, 220, 30));
	}
	if (MouseControl::newGameMenuButtonDetection(window) == 2)
	{
		playButton.setFillColor(Color(30, 220, 30));
		playText.setFillColor(Color(5, 30, 5));
	}
	else
	{
		playButton.setFillColor(Color(5, 120, 5));
		playText.setFillColor(Color(30, 220, 30));
	}

	window.draw(endGameWindow);
	window.draw(endGameWindowTop);
	window.draw(endGameWindowTopText);
	window.draw(endGameWindowText);
	window.draw(menuButton);
	window.draw(menuText);
	window.draw(playButton);
	window.draw(playText);
}
void drawNewGameMenu(RenderWindow &window, const string allTextEntered, const Font &font)
{
	//Draws the new game menu
	Text userInput(allTextEntered + ".save", font);
	userInput.setCharacterSize(20);
	userInput.setPosition(290, 250);
	userInput.setFillColor(Color(30, 220, 30));

	RectangleShape textBox(Vector2f(460, 30));
	textBox.setPosition(282, 250);
	textBox.setFillColor(Color(5, 80, 5));

	RectangleShape newGameWindow(Vector2f(500, 200));
	newGameWindow.setPosition(262, 184);
	newGameWindow.setFillColor(Color(5, 50, 5));

	RectangleShape newGameWindowTop(Vector2f(500, 25));
	newGameWindowTop.setPosition(262, 184);
	newGameWindowTop.setFillColor(Color(5, 130, 5));
	Text newGameWindowTopText("Enter new save name",font);
	newGameWindowTopText.setPosition(390, 178);
	newGameWindowTopText.setCharacterSize(24);
	newGameWindowTopText.setFillColor(Color(0, 30, 0));

	RectangleShape backButton(Vector2f(200, 50));
	backButton.setPosition(282, 315);
	Text backText("Back", font);
	backText.setCharacterSize(30);
	backText.setPosition(350, 320);

	RectangleShape startButton(Vector2f(200, 50));
	startButton.setPosition(540, 315);
	Text startText("Start", font);
	startText.setCharacterSize(30);
	startText.setPosition(600, 320);
	//Draws button highlighting
	if (MouseControl::newGameMenuButtonDetection(window) == 1)
	{
		backButton.setFillColor(Color(30, 220, 30));
		backText.setFillColor(Color(5, 30, 5));
	}
	else
	{
		backButton.setFillColor(Color(5, 120, 5));
		backText.setFillColor(Color(30, 220, 30));
	}
	if (MouseControl::newGameMenuButtonDetection(window) == 2 && FileManager::getNumberOfSaves() < FileManager::getMaxNumberOfSaves())
	{
		startButton.setFillColor(Color(30, 220, 30));
		startText.setFillColor(Color(5, 30, 5));
	}
	//If the save list is full the button is dark
	else if(FileManager::getNumberOfSaves() >= FileManager::getMaxNumberOfSaves())
	{
		startButton.setFillColor(Color(5, 100, 5));
		startText.setFillColor(Color(5, 30, 5));
	}
	else
	{
		startButton.setFillColor(Color(5, 120, 5));
		startText.setFillColor(Color(30, 220, 30));
	}

	window.draw(newGameWindow);
	window.draw(textBox);
	window.draw(newGameWindowTop);
	window.draw(newGameWindowTopText);
	window.draw(userInput);
	window.draw(backButton);
	window.draw(backText);
	window.draw(startButton);
	window.draw(startText);
}
void drawStaticObjects(RenderWindow &window, HackingHandler &hackingHandler, const Font &font)
{
	//Draws static text on the screen and the points
	Text text("Welcome to ROBCO Industries (TM) Termlink", font);
	text.setCharacterSize(25);
	text.setFillColor(Color(30, 220, 30));
	text.setPosition(70.5, 60.5);

	Text text2("Password Required", font);
	text2.setCharacterSize(25);
	text2.setFillColor(Color(30, 220, 30));
	text2.setPosition(70.5, 105.5);

	Text text3("Attempts Remaining:", font);
	text3.setCharacterSize(25);
	text3.setFillColor(Color(30, 220, 30));
	text3.setPosition(70.5, 150.5);
	
	Text points("Points: " + to_string(hackingHandler.getPoints()), font);
	points.setCharacterSize(25);
	points.setFillColor(Color(30, 220, 30));
	points.setPosition(700.5,185.5);

	//The static rows of numbers and letters next to the ones generated for the game
	Text staticColumn("",font);
	staticColumn.setCharacterSize(25);
	staticColumn.setFillColor(Color(30, 220, 30));
	staticColumn.setPosition(70.5, 185.5); //Position for the first row

	const string COLUMN1[20] = {
		"0x1540","0x154C","0x1558","0x1564","0x1570","0x157C","0x1588","0x1594","0x15A0","0x15AC","0x15BB","0x15C4","0x15D0","0x15DC","0x15E8","0x15F4","0x15JY","0x15M3","0x15T7","0x15YN"};
	const string COLUMN2[20] = {
		"0x1600","0x160C","0x1618","0x1624","0x1630","0x163C","0x1648","0x1654","0x1660","0x166C","0x1678","0x1684","0x1690","0x169C","0x16A8","0x16B4","0x16E3","0x16H6","0x16K2","0x16P2"};

	window.draw(text);
	window.draw(text2);
	window.draw(text3);
	window.draw(points);
	for (int i = 0; i < hackingHandler.getNumberOfRows(); i++)
	{
		staticColumn.setString(COLUMN1[i]);
		window.draw(staticColumn);
		staticColumn.move(0,26);
	}
	staticColumn.move(300,0);
	for (int i = hackingHandler.getNumberOfRows() - 1; i >= 0; i--)
	{
		staticColumn.move(0,-26);
		staticColumn.setString(COLUMN2[i]);
		window.draw(staticColumn);
	}
}
void drawAttempts(RenderWindow &window, int attemptsLeft)
{
	//Draws the number of attempts left as a number of rectangles
	RectangleShape rect(Vector2f(12, 20));
	rect.setPosition(Vector2f(350.5, 159.5));
	rect.setFillColor(Color(30, 220, 30));

	for (int i = 0; i < attemptsLeft; i++)
	{
		window.draw(rect);
		rect.move(17,0);
	}
}
void drawSymbols(RenderWindow &window, HackingHandler &hackingHandler, const Font &font)
{
	//Draws the symbols and words from the elements array in hackingHandler
	Text symbol("", font);
	Character **elements = hackingHandler.getElements();
	symbol.setCharacterSize(25);
	symbol.setFillColor(Color(30, 220, 30));
	symbol.setPosition(170.5, 185.5);
	Word *tempPtr;
	int pos = 0;
	
	//Draws first column
	for (int y = 0; y < hackingHandler.getNumberOfRows(); y++)
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
		for (int x = 0; x < hackingHandler.getRowLength(); x++)
		{
			//Checks if this object is a symbol or a word
			if (tempPtr = dynamic_cast<Word*>(elements[pos]))
			{
				if (tempPtr->isRemoved())
				{
					//If the word is removed it is drawn as a dot
					symbol.setString(".");
				}
				else
				{
					//The current letter of the word is drawn using the getPos() function of the word to find which letter of the word should be drawn
					symbol.setString(tempPtr->getString()[tempPtr->getPos()]);
				}
			}
			else
			{
				//Symbol is drawn
				symbol.setString(elements[pos]->getString()[0]);
			}
			window.draw(symbol);
			pos++;
			symbol.move(15, 0);
		}
		symbol.move(-15 * hackingHandler.getRowLength(),26);
	}
	//Moves on to the next column and does the same as above
	symbol.move(300,-26 * hackingHandler.getNumberOfRows());
	for (int y = hackingHandler.getNumberOfRows(); y > 0; y--)
	{
		for (int x = 0; x < hackingHandler.getRowLength(); x++)
		{
			if (tempPtr = dynamic_cast<Word*>(elements[pos]))
			{
				if (tempPtr->isRemoved())
				{
					symbol.setString(".");
				}
				else
				{
					symbol.setString(tempPtr->getString()[tempPtr->getPos()]);
				}
			}
			else
			{
				symbol.setString(elements[pos]->getString()[0]);
			}
			window.draw(symbol);
			pos++;
			symbol.move(15, 0);
		}
		symbol.move(-15 * hackingHandler.getRowLength(), 26);
	}
}
void drawMainMenu(RenderWindow &window, const Font &font)
{
	//Draw main menu
	RectangleShape playButton(Vector2f(300, 50));
	playButton.setPosition(Vector2f(512 - 150, 200 - 25));

	Text playText("New Game", font);
	playText.setCharacterSize(40);
	playText.setPosition(Vector2f(430, 170));

	RectangleShape loadButton(Vector2f(300, 50));
	loadButton.setPosition(Vector2f(512 - 150, 300 - 25));

	Text loadText("Load", font);
	loadText.setCharacterSize(40);
	loadText.setPosition(Vector2f(470, 270));

	RectangleShape exitButton(Vector2f(300, 50));
	exitButton.setPosition(Vector2f(512 - 150, 400 - 25));

	Text exitText("Exit", font);
	exitText.setCharacterSize(40);
	exitText.setPosition(Vector2f(470, 370));

	//Highlighting of playButton
	if (MouseControl::mainMenuButtonDetection(window) == 1)
	{
		playButton.setFillColor(Color(30, 220, 30));
		playText.setFillColor(Color(0,30,0));
	}
	else
	{
		playButton.setFillColor(Color(5,50,5));
		playText.setFillColor(Color(30, 220, 30));
	}
	//Highlighting of loadButton
	if (MouseControl::mainMenuButtonDetection(window) == 2)
	{
		loadButton.setFillColor(Color(30, 220, 30));
		loadText.setFillColor(Color(0, 30, 0));
	}
	else
	{
		loadButton.setFillColor(Color(5, 50, 5));
		loadText.setFillColor(Color(30, 220, 30));
	}
	//Highlighting of exitButton
	if (MouseControl::mainMenuButtonDetection(window) == 3)
	{
		exitButton.setFillColor(Color(30, 220, 30));
		exitText.setFillColor(Color(0, 30, 0));
	}
	else
	{
		exitButton.setFillColor(Color(5, 50, 5));
		exitText.setFillColor(Color(30, 220, 30));
	}
	//Draw everything
	window.draw(playButton);
	window.draw(playText);
	window.draw(loadButton);
	window.draw(loadText);
	window.draw(exitButton);
	window.draw(exitText);
}
void drawLoadMenu(RenderWindow &window, int choosenSave, const Font &font)
{
	//Draws the load menu
	RectangleShape textField(Vector2f(560, 630));
	textField.setPosition(Vector2f(392, 70));
	textField.setFillColor(Color(5, 80, 5));

	RectangleShape loadButton(Vector2f(300, 50));
	loadButton.setPosition(Vector2f(70, 510));
	Text loadText("Load", font);
	loadText.setCharacterSize(40);
	loadText.setPosition(Vector2f(180, 505));

	RectangleShape deleteButton(Vector2f(300, 50));
	deleteButton.setPosition(Vector2f(70, 580));
	Text deleteText("Delete", font);
	deleteText.setCharacterSize(40);
	deleteText.setPosition(Vector2f(160, 575));

	RectangleShape backButton(Vector2f(300, 50));
	backButton.setPosition(Vector2f(70, 650));
	Text backText("Back", font);
	backText.setCharacterSize(40);
	backText.setPosition(Vector2f(180, 645));

	RectangleShape listItem(Vector2f(560,25));
	listItem.setPosition(Vector2f(392,70));
	Text listText("", font);
	listText.setCharacterSize(22);
	listText.setPosition(Vector2f(400,66));

	//Highlighting of backButton
	if (MouseControl::loadMenuButtonDetection(window) == - 2)
	{
		backButton.setFillColor(Color(30, 220, 30));
		backText.setFillColor(Color(5, 30, 5));
	}
	else
	{
		backButton.setFillColor(Color(5, 50, 5));
		backText.setFillColor(Color(30, 220, 30));
	}
	//Highlighting of deleteButton
	if (MouseControl::loadMenuButtonDetection(window) == - 3)
	{
		deleteButton.setFillColor(Color(30, 220, 30));
		deleteText.setFillColor(Color(5, 30, 5));
	}
	else
	{
		deleteButton.setFillColor(Color(5, 50, 5));
		deleteText.setFillColor(Color(30, 220, 30));
	}
	//Highlighting of loadButton
	if (MouseControl::loadMenuButtonDetection(window) == -4)
	{
		loadButton.setFillColor(Color(30, 220, 30));
		loadText.setFillColor(Color(5, 30, 5));
	}
	else
	{
		loadButton.setFillColor(Color(5, 50, 5));
		loadText.setFillColor(Color(30, 220, 30));
	}
	//Draw everything
	window.draw(textField);
	window.draw(loadButton);
	window.draw(loadText);
	window.draw(deleteButton);
	window.draw(deleteText);
	window.draw(backButton);
	window.draw(backText);
	//Draws a list of all save files
	for (int i = 0; i < FileManager::getNumberOfSaves() && i <= FileManager::getMaxNumberOfSaves(); i++)
	{
		//Highlighting of saves
		if (MouseControl::loadMenuButtonDetection(window) == i ||choosenSave == i)
		{
			listItem.setFillColor(Color(30, 220, 30));
			listText.setFillColor(Color(5, 30, 5));
		}
		else
		{
			listItem.setFillColor(Color(5, 50, 5));
			listText.setFillColor(Color(30, 220, 30));
		}
		listText.setString(FileManager::getSaveNames()[i]);
		window.draw(listItem);
		window.draw(listText);
		listItem.move(0, 25);
		listText.move(0, 25);
	}
}
void drawOutput(RenderWindow &window, HackingHandler &hackingHandler, const Font &font)
{
	//Draws the output for when the player clicks on a symbol or word
	int *attempts = hackingHandler.getAttempts();
	Word *tempWord;
	Symbol *tempSymbol;
	int characterSize = 25;
	double x = 700.5;
	double y = 675.5;
	//0 for symbol and 1 for word
	bool symbolOrWord = 0;
	string stringForDraw = "";
	string likenessNumber;
	bool bonus = 0;

	Text hoverWord("", font);
	hoverWord.setCharacterSize(characterSize);
	hoverWord.setPosition(x, y);
	hoverWord.setFillColor(Color(30, 220, 30));

	Text stringOutput(">", font);
	stringOutput.setCharacterSize(characterSize);
	stringOutput.setPosition(x,y - 75);
	stringOutput.setFillColor(Color(30, 220, 30));

	Text entryDenied(">Entry Denied.", font);
	entryDenied.setCharacterSize(characterSize);
	entryDenied.setFillColor(Color(30, 220, 30));
	entryDenied.setPosition(x, y - 50);

	Text likeness(">Likeness=", font);
	likeness.setCharacterSize(characterSize);
	likeness.setFillColor(Color(30, 220, 30));
	likeness.setPosition(x, y - 25);

	Text dudRemoved(">Dud Removed.", font);
	dudRemoved.setCharacterSize(characterSize);
	dudRemoved.setFillColor(Color(30, 220, 30));
	dudRemoved.setPosition(x, y - 25);

	Text attemptsRestored(">Attempts Restored", font);
	attemptsRestored.setCharacterSize(characterSize);
	attemptsRestored.setFillColor(Color(30, 220, 30));
	attemptsRestored.setPosition(x, y -25);

	//If the player is hovering over a word or symbol, it's displayed below the rest of the output
	if (MouseControl::mouseDetection(window, hackingHandler) > -1)
	{
		hoverWord.setString(">" + hackingHandler.getElements()[MouseControl::mouseDetection(window, hackingHandler)]->getString());
		if (tempWord = dynamic_cast<Word *>(hackingHandler.getElements()[MouseControl::mouseDetection(window, hackingHandler)]))
		{
			if (tempWord->isRemoved() == true)
			{
				hoverWord.setString("> .");
			}
		}
		else if (tempSymbol = dynamic_cast<Symbol*>(hackingHandler.getElements()[MouseControl::mouseDetection(window, hackingHandler)]))
		{
			string temp = "";
			int tempInt = hackingHandler.checkSymbol(MouseControl::mouseDetection(window, hackingHandler), hackingHandler.getRowLength() - (MouseControl::mouseDetection(window, hackingHandler) % hackingHandler.getRowLength()));
			if (tempInt < 1)
			{
				hoverWord.setString(">" + hackingHandler.getElements()[MouseControl::mouseDetection(window, hackingHandler)]->getString());
			}
			else
			{
				for (int i = MouseControl::mouseDetection(window, hackingHandler); i <= tempInt; i++)
				{
					temp.append(hackingHandler.getElements()[i]->getString());
				}
				hoverWord.setString(">" + temp);
			}
		}
	}
	window.draw(hoverWord);
	//Finds the last 6 attempts for drawing in the output window
	for (int attemptNumber = hackingHandler.getAttemptNumber(); attemptNumber > hackingHandler.getAttemptNumber() - 6 && attemptNumber > 0; attemptNumber--)
	{
		for (int i = 0; i < hackingHandler.getArrayLength(); i++)
		{
			if (attempts[i] == attemptNumber)
			{
				//Gets the neccecary attributes for drawing the output for when the player either clicks a symbol or word
				if (tempSymbol = dynamic_cast<Symbol*>(hackingHandler.getElements()[i]))
				{
					stringForDraw = "";
					for (int j = i; j <= hackingHandler.checkSymbol(i, hackingHandler.getRowLength() - (i % hackingHandler.getRowLength())); j++)
					{
						stringForDraw.append(hackingHandler.getElements()[j]->getString());
					}
					bonus = tempSymbol->getBonus();
					symbolOrWord = 0;
				}
				else if (tempWord = dynamic_cast<Word*>(hackingHandler.getElements()[i]))
				{
					stringForDraw = tempWord->getString();
					likenessNumber = to_string(hackingHandler.checkWord(i));
					symbolOrWord = 1;
				}
			}
		}
		if (symbolOrWord == 0)
		{
			//As the symbol output is two rows instead of three, like the word output is, the stringOutput is moved down once before it's drawn
			stringOutput.move(0,25);
			stringOutput.setString(">\"" + stringForDraw + "\"");
			//Depending on what happens when the player clicks the symbol, the output is different
			if (bonus == 0)
			{
				window.draw(attemptsRestored);
			}
			else
			{
				window.draw(dudRemoved);
			}
			window.draw(stringOutput);
			stringOutput.move(0, -75);
			dudRemoved.move(0, -50);
			entryDenied.move(0, -50);
			likeness.move(0, -50);
			attemptsRestored.move(0, -50);
		}
		else
		{
			//Draws the word, entry denied and how many letters were correct
			stringOutput.setString(">" + stringForDraw);
			likeness.setString(">Likeness=" + likenessNumber);
			window.draw(stringOutput);
			window.draw(entryDenied);
			window.draw(likeness);			
			stringOutput.move(0, -75);
			dudRemoved.move(0, -75);
			entryDenied.move(0, -75);
			likeness.move(0, -75);
			attemptsRestored.move(0, -75);
		}
	}
}