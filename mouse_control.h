#ifndef MOUSECONTROL_H
#define MOUSECONTROL_H
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include"hacking_handler.h"
using namespace sf;

class MouseControl
{
private:
	static bool mouseClick;
public:
	static void drawMousePointer(RenderWindow &window);
	static int mouseDetection(const RenderWindow &window, const HackingHandler &hackingHandler);
	static void drawMouseHighlighting(RenderWindow &window, HackingHandler &hackingHandler, const Font font);
	static int mainMenuButtonDetection(const RenderWindow &window);
	static int loadMenuButtonDetection(const RenderWindow &window);
	static int newGameMenuButtonDetection(const RenderWindow &window);
	static bool isMouseClicked();
	static void setMouseClick(const bool mouseClick);
};
#endif