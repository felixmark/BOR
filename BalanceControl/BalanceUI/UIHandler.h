#pragma once
#include <memory>
#include <SFML\Graphics.hpp>
#include <chrono>
#include "../BalanceLib/Global_definitions.h"
#include "../BalanceLib/Rocker.h"
#include "../BalanceLib/PID_controller.h"

class UI_handler {
private:
	const double ball_radius = 20;
	const int wippenlaenge = 200;
	const int magnif_factor = 4;
	const double rockerbase_radius = (cos(1.178097)*((wippenlaenge / 2) * magnif_factor)) / 2;
	const int sim_time = 20;
	int center_x;
	int center_y;
	int width;
	int height;
	bool key_r_was_pressed_;
	bool key_l_was_pressed_;
	std::chrono::time_point<std::chrono::steady_clock> start_time_;


	sf::CircleShape circle;
	sf::RectangleShape bar;
	sf::CircleShape rockerbase_top;
	sf::RectangleShape rockerbase_bottom;
	sf::RectangleShape ground;
	sf::RectangleShape background;
	sf::Text text_time;
	sf::Text text_position;
	sf::Text text_angle;
	sf::Text text_ballspeed;
	sf::Sprite sprite_failed;
	sf::Transform transform;
	sf::Texture texture;

	Rocker rocker_;
	PID_controller controller;
	unsigned long time;

	void setup(sf::RenderWindow & window, sf::Font& font, sf::Image& image_failed);

public:
	UI_handler(sf::RenderWindow & window, sf::Font& font, sf::Image& image_failed);
	void clear(sf::RenderWindow & window);
	void step(sf::RenderWindow & window);
	void draw(sf::RenderWindow & window);
	void update(sf::RenderWindow & window);
};
