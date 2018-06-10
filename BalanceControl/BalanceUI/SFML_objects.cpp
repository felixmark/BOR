#include "stdafx.h"
#include "SFML_objects.h"
#include <stdexcept>
#include <exception>


SFML_objects::SFML_objects()
{ /* No constructor needed */ }

std::vector<sf::Shape> SFML_objects::get_shapes(int width, int height) const {
	std::vector<sf::Shape> shapes;

	const int center_x = -width / 2;
	const int center_y = -height / 2;
	const float ball_radius = 20;
	const int wippenlaenge = 200;
	const int magnif_factor = 4;
	const float rockerbase_radius = (cos(1.178097)*((wippenlaenge / 2) * magnif_factor)) / 2;
	const std::string fontname = "OpenSans-Regular.ttf";
	const std::string image_failed = "failed.png";

	// Ball
	sf::CircleShape circle{ ball_radius };

	// Rocker
	sf::RectangleShape bar{ sf::Vector2f(wippenlaenge * magnif_factor, 20) };
	bar.setFillColor(sf::Color(0, 150, 200));
	bar.setOrigin(center_x + wippenlaenge / 2 * magnif_factor, center_y - ball_radius * 2);

	// Rocker Base
	sf::CircleShape rockerbase_top{ rockerbase_radius };
	rockerbase_top.setFillColor(sf::Color(0, 50, 130));
	rockerbase_top.setOrigin(center_x + rockerbase_radius, center_y - ball_radius * 3);
	rockerbase_top.setPointCount(3);

	sf::RectangleShape rockerbase_bottom{ sf::Vector2f(160, 53) };
	rockerbase_bottom.setFillColor(sf::Color(0, 50, 130));
	rockerbase_bottom.setOrigin(center_x + 80, center_y - 160);

	// Ground
	sf::RectangleShape ground{ sf::Vector2f(width, height) };
	ground.setFillColor(sf::Color(40, 150, 0));
	ground.setOutlineThickness(20);
	ground.setOutlineColor(sf::Color(80, 180, 0));
	ground.setOrigin(0, center_y - ball_radius * 3 - rockerbase_radius * 2 - 20);

	// Sky
	sf::RectangleShape sky{ sf::Vector2f(width, height) };
	sky.setFillColor(sf::Color(30, 30, 30));

	// Text
	sf::Text text1;
	sf::Font font;
	if (!font.loadFromFile(fontname)) {
		throw new std::runtime_error("Font \""+fontname+"\" not found.");
	}
	text1.setFont(font);
	text1.setStyle(sf::Text::Regular);
	text1.setPosition(0, 0);
	text1.setFillColor(sf::Color(255, 255, 255));
	text1.setCharacterSize(30);

	sf::Text text2(text1);
	text2.setPosition(0, 40);
	sf::Text text3(text1);
	text3.setPosition(0, 80);
	sf::Text text4(text1);
	text4.setPosition(0, 120);

	// Images
	sf::Image failed;
	failed.loadFromFile(image_failed);
	sf::Texture texture;
	texture.loadFromImage(failed);
	sf::Sprite sprite_failed;
	sprite_failed.setTexture(texture, true);
	sprite_failed.setOrigin(sf::Vector2f(300.0, 200.0));
	sprite_failed.setPosition(-center_x, -center_y);


	shapes.push_back(circle);
	shapes.push_back(bar);
	shapes.push_back(rockerbase_top);
	shapes.push_back(rockerbase_bottom);
	shapes.push_back(ground);
	shapes.push_back(sky);
	return shapes;
}


std::vector<sf::Text> SFML_objects::get_texts(int width, int height) const {
	std::vector<sf::Text> texts;

	const int center_x = -width / 2;
	const int center_y = -height / 2;
	const float ball_radius = 20;
	const int wippenlaenge = 200;
	const int magnif_factor = 4;
	const float rockerbase_radius = (cos(1.178097)*((wippenlaenge / 2) * magnif_factor)) / 2;
	const std::string fontname = "OpenSans-Regular.ttf";
	const std::string image_failed = "failed.png";

	// Text
	sf::Text text_time;
	sf::Font font;
	if (!font.loadFromFile(fontname)) {
		throw new std::runtime_error("Font \"" + fontname + "\" not found.");
	}
	text_time.setFont(font);
	text_time.setStyle(sf::Text::Regular);
	text_time.setPosition(0, 0);
	text_time.setFillColor(sf::Color(255, 255, 255));
	text_time.setCharacterSize(30);

	sf::Text text2(text_time);
	text2.setPosition(0, 40);
	sf::Text text3(text_time);
	text3.setPosition(0, 80);
	sf::Text text4(text_time);
	text4.setPosition(0, 120);

	// Images
	sf::Image failed;
	failed.loadFromFile(image_failed);
	sf::Texture texture;
	texture.loadFromImage(failed);
	sf::Sprite sprite_failed;
	sprite_failed.setTexture(texture, true);
	sprite_failed.setOrigin(sf::Vector2f(300.0, 200.0));
	sprite_failed.setPosition(-center_x, -center_y);

	texts.push_back(text_time);
	texts.push_back(text2);
	texts.push_back(text3);
	texts.push_back(text4);

	return texts;
}