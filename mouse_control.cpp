#include "mouse_control.h"
bool MouseControl::mouseClick = false;

void MouseControl::drawMousePointer(RenderWindow &window)
{
	//Draws the mouse pointer
	Texture pointerTexture;
	pointerTexture.loadFromFile("cursor.png");
	Sprite mousePointer(pointerTexture);
	Vector2<float> mousePos = static_cast<Vector2f>(Mouse::getPosition(window));
	mousePointer.setPosition(mousePos - Vector2f(10, 10));
	window.draw(mousePointer);
}

int MouseControl::mouseDetection(const RenderWindow &window, const HackingHandler &hackingHandler)
{
	//Returns the position in the elements array of the object that the mouse is hovering over

	Vector2<float> mousePos = static_cast<Vector2f>(Mouse::getPosition(window));
	//First column - Detection for symbol highlighting
	for (int i = 0; i < hackingHandler.getNumberOfRows(); i++)
	{
		for (int j = 0; j < hackingHandler.getRowLength(); j++)
		{
			//Checking if the mouse pointer is within the area of a symbol
			if (mousePos.x > j * 15 + 170 && mousePos.x < j * 15 + 15 + 170 &&
				mousePos.y > i * 26 + 185 + 5 && mousePos.y < i * 26 + 26 + 185 + 5)
			{
				return i * hackingHandler.getRowLength() + j;
			}
		}
	}
	//Second column
	for (int i = 0; i < hackingHandler.getNumberOfRows(); i++)
	{
		for (int j = 0; j < hackingHandler.getRowLength(); j++)
		{
			//Checking if the mouse pointer is within the area of a symbol
			if (mousePos.x > j * 15 + 470 && mousePos.x < j * 15 + 15 + 470 &&
				mousePos.y > i * 26 + 185 + 5 && mousePos.y < i * 26 + 26 + 185 + 5)
			{
				//RectangleShape rect(Vector2f(15, 26));
				////Draws a square with a few offsets
				//rect.setPosition(Vector2f(j * 15 + 470, i * 26 + 185 + 5));
				//rect.setFillColor(Color(100, 100, 100));
				//window.draw(rect);
				//Adds the last column as well as the position in this one to get the correct position in the array
				return hackingHandler.getRowLength() * hackingHandler.getNumberOfRows() + i * hackingHandler.getRowLength() + j;
			}
		}
	}
	return -1;
}

void MouseControl::drawMouseHighlighting(RenderWindow &window, HackingHandler &hackingHandler, const Font font)
{
	//Draws mouse highlighting of symbols and words in the game
	Character **elements = hackingHandler.getElements();
	Symbol *currentSymbol;
	Word *currentWord;
	int mousePos = mouseDetection(window,hackingHandler);
	int currentPos = 0;
	int foundPos = 0;
	bool done = false;
	//Even though this only interacts with the objects the mouse is hovering over, the structure of rows and columns still needs to be built with these for-loops
	//First column
	for (int y = 0; y < hackingHandler.getNumberOfRows() && done == false && mousePos < (hackingHandler.getArrayLength() / 2); y++)
	{
		//y is the position of a row, x is the position of a character within a row
		for (int x = 0; x < hackingHandler.getRowLength() && done == false; x++)
		{
			//Finds the closest symbol on the same row that mirrors this one and highlights the area between them if one exists
			if ((currentSymbol = dynamic_cast<Symbol*>(elements[currentPos])) && mousePos == currentPos)
			{
				//Checks if this symbol has already been used, disabling the more advanced highlighting
				if (currentSymbol->isUsed() == false)
				{
					//Checks if there is a corresponding symbol on the same row as this one, 
					//as the highlighting would then stretch from this symbol to the other one
					foundPos = hackingHandler.checkSymbol(currentPos,(hackingHandler.getRowLength() - x));
					if (foundPos != -1)
					{
						//Highlights this symbol, the found symbol and all in between them
						for (int i = 0; i < (foundPos - currentPos + 1); i++)
						{
							//Inverts the color of the character by drawing the new colored one over the old one
							Text character(elements[currentPos + i]->getString(), font);
							character.setCharacterSize(25);
							character.setPosition(Vector2f((x + i) * 15 + 170.5, y * 26 + 185.5));
							character.setFillColor(Color(0, 30, 0));

							//Draws a rectangle with a few offsets
							RectangleShape rect(Vector2f(15, 26));
							rect.setPosition(Vector2f((x + i) * 15 + 170.5, y * 26 + 185.5 + 5));
							rect.setFillColor(Color(30, 220, 30));

							window.draw(rect);
							window.draw(character);
						}
						done = true;
					}
					else
					{
						//Inverts the color of the character by drawing the new colored one over the old one
						Text character(elements[currentPos]->getString(), font);
						character.setCharacterSize(25);
						character.setPosition(Vector2f(x * 15 + 170.5, y * 26 + 185.5));
						character.setFillColor(Color(0, 30, 0));

						//Draws a rectangle with a few offsets
						RectangleShape rect(Vector2f(15, 26));
						rect.setPosition(Vector2f(x * 15 + 170.5, y * 26 + 185.5 + 5));
						rect.setFillColor(Color(30, 220, 30));

						window.draw(rect);
						window.draw(character);
						done = true;
					}
				}
				else
				{
					//Inverts the color of the character by drawing the new colored one over the old one
					Text character(elements[currentPos]->getString(), font);
					character.setCharacterSize(25);
					character.setPosition(Vector2f(x * 15 + 170.5, y * 26 + 185.5));
					character.setFillColor(Color(0, 30, 0));

					//Draws a rectangle with a few offsets
					RectangleShape rect(Vector2f(15, 26));
					rect.setPosition(Vector2f(x * 15 + 170.5, y * 26 + 185.5 + 5));
					rect.setFillColor(Color(30, 220, 30));

					window.draw(rect);
					window.draw(character);
					done = true;
				}

			}
			//Highlights an entire word regardless of which letter is moused over
			else if ((currentWord = dynamic_cast<Word*>(elements[currentPos])) && mousePos == currentPos)
			{
				if (currentWord->isRemoved())
				{
					//Inverts the color of the character by drawing the new colored one over the old one
					Text character(".", font);
					character.setCharacterSize(25);
					character.setPosition(Vector2f(x * 15 + 170.5, y * 26 + 185.5));
					character.setFillColor(Color(0, 30, 0));

					//Draws a rectangle with a few offsets
					RectangleShape rect(Vector2f(15, 26));
					rect.setPosition(Vector2f(x * 15 + 170.5, y * 26 + 185.5 + 5));
					rect.setFillColor(Color(30, 220, 30));

					window.draw(rect);
					window.draw(character);
					done = true;
				}
				else
				{
					//Indicates which letter of the word the mouse is hovering over relative to the whole word
					int localPos = currentWord->getPos();
					//Skips back to the first letter in the word relative to the entire array of objects
					int firstPos = currentPos - localPos;

					//As the for-loops will not be used anymore, their x and y variables can be directly edited

					//If the word is split between two rows and the mouse is hovering over the lower row
					if (x <= localPos)
					{
						int lettersOnSecondRow = hackingHandler.getWordLength() - (localPos - x);
						//The part of the word on the lower row is drawn first
						for (int i = 0; i < lettersOnSecondRow && i < hackingHandler.getWordLength(); i++)
						{
							//Highlights the character by drawing the new colored character over the old one, and draws a rectangle behind it
							Text character(elements[firstPos]->getString()[(hackingHandler.getWordLength() - lettersOnSecondRow) + i], font);
							RectangleShape rect(Vector2f(15, 26));

							character.setCharacterSize(25);

							rect.setPosition(Vector2f(i * 15 + 170.5, y * 26 + 185.5 + 5));
							character.setPosition(Vector2f(i * 15 + 170.5, y * 26 + 185.5));

							rect.setFillColor(Color(30, 220, 30));
							character.setFillColor(Color(0, 30, 0));

							window.draw(rect);
							window.draw(character);
						}

						//Moves x and y to the first letter of the word, and also makes localPos the number of letters on the upper row
						localPos = localPos - x;
						x = hackingHandler.getRowLength();
						x = x - localPos;
						y--;

						//The upper row is drawn
						for (int i = 0; i < localPos && i < hackingHandler.getWordLength(); i++)
						{
							//Highlights the character by drawing the new colored character over the old one, and draws a rectangle behind it
							Text character(elements[firstPos]->getString()[i], font);
							RectangleShape rect(Vector2f(15, 26));

							character.setCharacterSize(25);

							rect.setPosition(Vector2f((x + i) * 15 + 170.5, y * 26 + 185.5 + 5));
							character.setPosition(Vector2f((x + i) * 15 + 170.5, y * 26 + 185.5));

							rect.setFillColor(Color(30, 220, 30));
							character.setFillColor(Color(0, 30, 0));

							window.draw(rect);
							window.draw(character);
						}
					}
					//If the mouse is split between two rows and the mouse is hovering over the upper row, or the word isn't split at all
					else
					{
						for (int i = 0; i < (hackingHandler.getRowLength() - x + localPos) && i < hackingHandler.getWordLength(); i++)
						{
							//Inverts the color of the character by drawing the new colored one over the old one
							Text character(elements[firstPos + i]->getString()[i], font);
							character.setCharacterSize(25);
							character.setPosition(Vector2f(((x - localPos) + i) * 15 + 170.5, y * 26 + 185.5));
							character.setFillColor(Color(0, 30, 0));
							//Draws a rectangle with a few offsets
							RectangleShape rect(Vector2f(15, 26));
							rect.setPosition(Vector2f(((x - localPos) + i) * 15 + 170.5, y * 26 + 185.5 + 5));
							rect.setFillColor(Color(30, 220, 30));
							window.draw(rect);
							window.draw(character);
						}
						int lettersOnSecondRow = hackingHandler.getWordLength() - (hackingHandler.getRowLength() - (x - localPos));
						//Moves down to the next row and if the word continues, highlights the rest of it
						x = 0;
						y++;
						//If the word is split between the two columns, the second half will need different coordinates than usual
						if (y >= hackingHandler.getNumberOfRows())
						{
							for (int i = 0; i < lettersOnSecondRow; i++)
							{
								//Highlights the character by drawing the new colored character over the old one, and draws a rectangle behind it
								Text character(elements[firstPos + i]->getString()[(hackingHandler.getWordLength() - lettersOnSecondRow) + i], font);
								RectangleShape rect(Vector2f(15, 26));
							
								character.setCharacterSize(25);

								rect.setPosition(Vector2f(i * 15 + 470.5, 185.5 + 5));
								character.setPosition(Vector2f(i * 15 + 470.5, 185.5));

								rect.setFillColor(Color(30, 220, 30));
								character.setFillColor(Color(0, 30, 0));

								window.draw(rect);
								window.draw(character);
							}
						}
						//Otherwise, as long as the word is split at all, the second half is drawn as usual
						else
						{
							for (int i = 0; i < lettersOnSecondRow; i++)
							{
								//Highlights the character by drawing the new colored character over the old one, and draws a rectangle behind it
								Text character(elements[firstPos + i]->getString()[(hackingHandler.getWordLength() - lettersOnSecondRow) + i], font);
								RectangleShape rect(Vector2f(15, 26));

								character.setCharacterSize(25);

								rect.setPosition(Vector2f(i * 15 + 170.5, y * 26 + 185.5 + 5));
								character.setPosition(Vector2f(i * 15 + 170.5, y * 26 + 185.5));

								rect.setFillColor(Color(30, 220, 30));
								character.setFillColor(Color(0, 30, 0));

								window.draw(rect);
								window.draw(character);
							}
						}
					}
					done = true;
				}
			}
			currentPos++;
		}
	}
	currentPos = hackingHandler.getArrayLength() / 2;
	//Second column - works the same as the first one
	for (int y = 0; y < hackingHandler.getNumberOfRows() && done == false && mousePos >= (hackingHandler.getArrayLength() / 2); y++)
	{
		for (int x = 0; x < hackingHandler.getRowLength() && done == false; x++)
		{
			if ((currentSymbol = dynamic_cast<Symbol*>(elements[currentPos])) && mousePos == currentPos)
			{
				if (currentSymbol->isUsed() == false)
				{
					foundPos = hackingHandler.checkSymbol(currentPos, (hackingHandler.getRowLength() - x));
					if (foundPos != -1)
					{
						for (int i = 0; i < (foundPos - currentPos + 1); i++)
						{
							Text character(elements[currentPos + i]->getString(), font);
							character.setCharacterSize(25);
							character.setPosition(Vector2f((x + i) * 15 + 470.5, y * 26 + 185.5));
							character.setFillColor(Color(0, 30, 0));

							RectangleShape rect(Vector2f(15, 26));
							rect.setPosition(Vector2f((x + i) * 15 + 470.5, y * 26 + 185.5 + 5));
							rect.setFillColor(Color(30, 220, 30));

							window.draw(rect);
							window.draw(character);
						}
						done = true;
					}
					else
					{
						Text character(elements[currentPos]->getString(), font);
						character.setCharacterSize(25);
						character.setPosition(Vector2f(x * 15 + 470.5, y * 26 + 185.5));
						character.setFillColor(Color(0, 30, 0));

						RectangleShape rect(Vector2f(15, 26));
						rect.setPosition(Vector2f(x * 15 + 470.5, y * 26 + 185.5 + 5));
						rect.setFillColor(Color(30, 220, 30));

						window.draw(rect);
						window.draw(character);
						done = true;
					}
				}
				else
				{
					Text character(elements[currentPos]->getString(), font);
					character.setCharacterSize(25);
					character.setPosition(Vector2f(x * 15 + 470.5, y * 26 + 185.5));
					character.setFillColor(Color(0, 30, 0));

					RectangleShape rect(Vector2f(15, 26));
					rect.setPosition(Vector2f(x * 15 + 470.5, y * 26 + 185.5 + 5));
					rect.setFillColor(Color(30, 220, 30));

					window.draw(rect);
					window.draw(character);
					done = true;
				}

			}
			else if ((currentWord = dynamic_cast<Word*>(elements[currentPos])) && mousePos == currentPos)
			{
				if (currentWord->isRemoved())
				{
					Text character(".", font);
					character.setCharacterSize(25);
					character.setPosition(Vector2f(x * 15 + 470.5, y * 26 + 185.5));
					character.setFillColor(Color(0, 30, 0));

					RectangleShape rect(Vector2f(15, 26));
					rect.setPosition(Vector2f(x * 15 + 470.5, y * 26 + 185.5 + 5));
					rect.setFillColor(Color(30, 220, 30));

					window.draw(rect);
					window.draw(character);
					done = true;
				}
				else
				{
					int localPos = currentWord->getPos();
					int firstPos = currentPos - localPos;

					if (x <= localPos)
					{
						int lettersOnSecondRow = hackingHandler.getWordLength() - (localPos - x);
						for (int i = 0; i < lettersOnSecondRow && i < hackingHandler.getWordLength(); i++)
						{
							Text character(elements[firstPos]->getString()[(hackingHandler.getWordLength() - lettersOnSecondRow) + i], font);
							RectangleShape rect(Vector2f(15, 26));

							character.setCharacterSize(25);

							rect.setPosition(Vector2f(i * 15 + 470.5, y * 26 + 185.5 + 5));
							character.setPosition(Vector2f(i * 15 + 470.5, y * 26 + 185.5));

							rect.setFillColor(Color(30, 220, 30));
							character.setFillColor(Color(0, 30, 0));

							window.draw(rect);
							window.draw(character);
						}

						localPos = localPos - x;
						x = hackingHandler.getRowLength();
						x = x - localPos;
						y--;
						if (y <= 0)
						{
							for (int i = 0; i < localPos && i < hackingHandler.getWordLength(); i++)
							{
								Text character(elements[firstPos]->getString()[i], font);
								RectangleShape rect(Vector2f(15, 26));

								character.setCharacterSize(25);

								rect.setPosition(Vector2f((x + i) * 15 + 170.5, (hackingHandler.getNumberOfRows() - 1) * 26 + 185.5 + 5));
								character.setPosition(Vector2f((x + i) * 15 + 170.5, (hackingHandler.getNumberOfRows() - 1) * 26 + 185.5));

								rect.setFillColor(Color(30, 220, 30));
								character.setFillColor(Color(0, 30, 0));

								window.draw(rect);
								window.draw(character);
							}
						}
						else
						{
							for (int i = 0; i < localPos && i < hackingHandler.getWordLength(); i++)
							{
								Text character(elements[firstPos]->getString()[i], font);
								RectangleShape rect(Vector2f(15, 26));

								character.setCharacterSize(25);

								rect.setPosition(Vector2f((x + i) * 15 + 470.5, y * 26 + 185.5 + 5));
								character.setPosition(Vector2f((x + i) * 15 + 470.5, y * 26 + 185.5));

								rect.setFillColor(Color(30, 220, 30));
								character.setFillColor(Color(0, 30, 0));

								window.draw(rect);
								window.draw(character);
							}
						}

					}
					else
					{
						for (int i = 0; i < (hackingHandler.getRowLength() - x + localPos) && i < hackingHandler.getWordLength(); i++)
						{
							//Inverts the color of the character by drawing the new colored one over the old one
							Text character(elements[firstPos + i]->getString()[i], font);
							character.setCharacterSize(25);
							character.setPosition(Vector2f(((x - localPos) + i) * 15 + 470.5, y * 26 + 185.5));
							character.setFillColor(Color(0, 30, 0));
							//Draws a rectangle with a few offsets
							RectangleShape rect(Vector2f(15, 26));
							rect.setPosition(Vector2f(((x - localPos) + i) * 15 + 470.5, y * 26 + 185.5 + 5));
							rect.setFillColor(Color(30, 220, 30));
							window.draw(rect);
							window.draw(character);
						}
						int lettersOnSecondRow = hackingHandler.getWordLength() - (hackingHandler.getRowLength() - (x - localPos));
						x = 0;
						y++;
						for (int i = 0; i < lettersOnSecondRow; i++)
						{
							Text character(elements[firstPos + i]->getString()[(hackingHandler.getWordLength() - lettersOnSecondRow) + i], font);
							RectangleShape rect(Vector2f(15, 26));

							character.setCharacterSize(25);

							rect.setPosition(Vector2f(i * 15 + 470.5, y * 26 + 185.5 + 5));
							character.setPosition(Vector2f(i * 15 + 470.5, y * 26 + 185.5));

							rect.setFillColor(Color(30, 220, 30));
							character.setFillColor(Color(0, 30, 0));

							window.draw(rect);
							window.draw(character);
						}
					}
					done = true;
				}

			}
			currentPos++;
		}
	}
}

int MouseControl::mainMenuButtonDetection(const RenderWindow &window)
{
	//Returns a different int depending on which menu button the mouse is hovering over
	//For the main menu
	Vector2i mousePos = Mouse::getPosition(window);
	if (mousePos.x > 512 - 150 && mousePos.y > 200 - 25 && mousePos.x < 512 - 150 + 300 && mousePos.y < 200 - 25 + 50)
	{
		return 1;
	}
	else if (mousePos.x > 512 - 150 && mousePos.y > 300 - 25 && mousePos.x < 512 - 150 + 300 && mousePos.y < 300 - 25 + 50)
	{
		return 2;
	}
	else if (mousePos.x > 512 - 150 && mousePos.y > 400 - 25 && mousePos.x < 512 - 150 + 300 && mousePos.y < 400 - 25 + 50)
	{
		return 3;
	}
	return 0;
}

int MouseControl::loadMenuButtonDetection(const RenderWindow &window)
{
	//Returns a different int depending on which menu button the mouse is hovering over
	//For the load game menu
	Vector2i mousePos = Mouse::getPosition(window);
	if (mousePos.x > 70 && mousePos.y > 510 && mousePos.x < 70 + 300 && mousePos.y < 510 + 50)
	{
		return -4;
	}
	else if (mousePos.x > 70 && mousePos.y > 580 && mousePos.x < 70 + 300 && mousePos.y < 580 + 50)
	{
		return -3;
	}
	else if (mousePos.x > 70 && mousePos.y > 650 && mousePos.x < 70 + 300 && mousePos.y < 650 + 50)
	{
		return -2;
	}
	for (int i = 0; i < FileManager::getNumberOfSaves(); i++)
	{
		if (mousePos.x > +392, mousePos.y > i * 25 + 70, mousePos.x < 952, mousePos.y < i * 25 + 95)
		{
			return i;
		}
	}
	return -1;
}

int MouseControl::newGameMenuButtonDetection(const RenderWindow &window)
{
	//Returns a different int depending on which menu button the mouse is hovering over
	//For the New Game menu and the end game menu
	Vector2i mousePos = Mouse::getPosition(window);
	if (mousePos.x > 282 && mousePos.y > 315 && mousePos.x < 282 + 200 && mousePos.y < 315 + 50)
	{
		return 1;
	}
	else if (mousePos.x > 540 && mousePos.y > 315 && mousePos.x < 540 + 200 && mousePos.y < 315 + 50)
	{
		return 2;
	}
	return 0;
}

bool MouseControl::isMouseClicked()
{
	return MouseControl::mouseClick;
}

void MouseControl::setMouseClick(const bool mouseClick)
{
	MouseControl::mouseClick = mouseClick;
}