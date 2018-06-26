#pragma once
#include <memory>
#include <SFML\Graphics.hpp>
#include <chrono>
#include "../BalanceLib/Global_definitions.h"
#include "../BalanceLib/Rocker.h"
#include "../BalanceLib/PID_controller.h"

class UI_handler {
private:
	const double ball_radius_ = 20;
	const int rocker_length_ = 200;
	const int magnif_factor_ = 4;
	const double rockerbase_radius_ = (cos(1.178097)*((rocker_length_ / 2) * magnif_factor_)) / 2;
	const int sim_time_ = 20;
	int center_x_;
	int center_y_;
	int width_;
	int height_;
	bool key_r_was_pressed_;
	bool key_l_was_pressed_;
	std::chrono::time_point<std::chrono::steady_clock> start_time_;


	sf::CircleShape ball_;
	sf::RectangleShape bar_;
	sf::CircleShape rockerbase_top_;
	sf::RectangleShape rockerbase_bottom_;
	sf::RectangleShape ground_;
	sf::RectangleShape background_;
	sf::Text text_time_;
	sf::Text text_position_;
	sf::Text text_angle_;
	sf::Text text_ballspeed_;
	sf::Text text_info_;
	sf::Sprite sprite_failed_;
	sf::Transform transform_;
	sf::Texture texture_;

	Rocker rocker_;
	PID_controller controller_;
	unsigned long time_;

public:
	UI_handler(sf::RenderWindow & window, sf::Font& font, sf::Image& image_failed);
	void clear(sf::RenderWindow & window);
	void step(sf::RenderWindow & window);
	void draw(sf::RenderWindow & window);
	void update(sf::RenderWindow & window);
};
