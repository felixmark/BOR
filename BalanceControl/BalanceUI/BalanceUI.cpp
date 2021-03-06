// BalanceUI.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include "../BalanceLib/Rocker.h"
#include "UIHandler.h"

using namespace std;
using namespace chrono;

int main() {
	//===================== SETUP =====================
	const int height = 900;
	const int width = 1600;
	sf::ContextSettings settings;
	//settings.antialiasingLevel = 8;

	// Font
	sf::Font font;
	if (!font.loadFromFile("OpenSans-Regular.ttf")) {
		throw runtime_error("Font not found.");
	}

	// Image
	sf::Image image_failed;
	if (!image_failed.loadFromFile("failed.png")) {
		throw runtime_error("Image not found.");
	}

	sf::RenderWindow window(sf::VideoMode(width, height), "BOR", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	UI_handler ui_handler(window,font,image_failed);

	//===================== START BALANCING THE BALL =====================
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		ui_handler.clear(window);
		ui_handler.step(window);
		ui_handler.draw(window);
		ui_handler.update(window);
	}
	return EXIT_SUCCESS;
}