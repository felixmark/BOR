#include "stdafx.h"
#include "UIHandler.h"
#include <SFML\Graphics.hpp>
#include "../BalanceLib/Rocker.h"
#include <chrono>
#include <thread>

using namespace std;

void UI_handler::clear(sf::RenderWindow & window) {
	window.clear();
}

void UI_handler::step(sf::RenderWindow & window) {
	// Read Left and Right Key on Keyboard
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && key_r_was_pressed_ == false) {
		key_r_was_pressed_ = true;
		start_time_ = chrono::steady_clock::now();
	} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && key_r_was_pressed_) {
		int elapsed_ms = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time_).count();
		rocker_.push_ball(0.001*elapsed_ms);
		key_r_was_pressed_ = false;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && key_l_was_pressed_ == false) {
		key_l_was_pressed_ = true;
		start_time_ = chrono::steady_clock::now();
	} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && key_l_was_pressed_) {
		int elapsed_ms = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time_).count();
		rocker_.push_ball(-0.001*elapsed_ms);
		key_l_was_pressed_ = false;
	}


	// Simulate
	for (long cnt = 0; cnt < sim_time_; ++cnt) {
		double current_position = rocker_.get_position();
		controller_.set_process_value(current_position);
		rocker_.step();
		controller_.step();
		double angle = -controller_.get_control_value();
		rocker_.set_angle(angle);
	}


	//===================== SET BALL POSITION, ROCKER ROTATION AND TEXT =====================
	ball_.setOrigin(
		center_x_ + ball_radius_ - (
			cos(grad_to_rad(rocker_.get_angle())) * 
			rocker_.get_position()*magnif_factor_
		),
		center_y_ + (
			sin(grad_to_rad(rocker_.get_angle())) * 
			rocker_.get_position()*magnif_factor_
		)
	);
	transform_ = sf::Transform();
	transform_.rotate(
		-rocker_.get_angle(),
		{ static_cast<float>(-center_x_), static_cast<float>(-center_y_) }
	);
	text_time_.setString("Simulated time: " + to_string(rocker_.get_time_elapsed()) + " s");
	text_position_.setString("Position: " + to_string(rocker_.get_position()) + " cm");
	text_angle_.setString("Angle: " + to_string(rocker_.get_angle()) + " °");
	text_ballspeed_.setString("Ball speed: " + to_string(rocker_.get_ball_speed()) + " m/s");
}

void UI_handler::draw(sf::RenderWindow & window) {
	window.draw(background_);
	window.draw(ground_);
	window.draw(ball_);
	window.draw(rockerbase_top_);
	window.draw(rockerbase_bottom_);
	window.draw(bar_, transform_);
	window.draw(text_time_);
	window.draw(text_position_);
	window.draw(text_angle_);
	window.draw(text_ballspeed_);
	window.draw(text_info_);
}

void UI_handler::update(sf::RenderWindow & window) {
	if (!rocker_.is_ball_on_rocker()) {
		window.draw(sprite_failed_);
		window.display();
		this_thread::sleep_for(chrono::seconds(3));
		window.close();
	} else {
		window.display();
	}
}

UI_handler::UI_handler(sf::RenderWindow & window, sf::Font& font, sf::Image& image_failed) {
	width_ = window.getView().getSize().x;
	height_ = window.getView().getSize().y;
	center_x_ = -window.getView().getSize().x / 2;
	center_y_ = -window.getView().getSize().y / 2;
	key_r_was_pressed_ = false;
	key_l_was_pressed_ = false;
	start_time_ = chrono::steady_clock::now();


	//===================== VISUAL SFML OBJECTS =====================
	// Ball
	ball_ = sf::CircleShape( ball_radius_ );

	// Rocker
	bar_ = sf::RectangleShape( sf::Vector2f(rocker_length_ * magnif_factor_, 20) );
	bar_.setFillColor(sf::Color(0, 150, 200));
	bar_.setOrigin(center_x_ + rocker_length_ / 2 * magnif_factor_, center_y_ - ball_radius_ * 2);

	// Rocker Base
	rockerbase_top_ = sf::CircleShape( rockerbase_radius_ );
	rockerbase_top_.setFillColor(sf::Color(0, 50, 130));
	rockerbase_top_.setOrigin(center_x_ + rockerbase_radius_, center_y_ - ball_radius_ * 3);
	rockerbase_top_.setPointCount(3);

	rockerbase_bottom_ = sf::RectangleShape( sf::Vector2f(160, 53) );
	rockerbase_bottom_.setFillColor(sf::Color(0, 50, 130));
	rockerbase_bottom_.setOrigin(center_x_ + 80, center_y_ - 160);

	// Ground
	ground_ = sf::RectangleShape( sf::Vector2f(width_, height_) );
	ground_.setFillColor(sf::Color(40, 150, 0));
	ground_.setOutlineThickness(20);
	ground_.setOutlineColor(sf::Color(80, 180, 0));
	ground_.setOrigin(0, center_y_ - ball_radius_ * 3 - rockerbase_radius_ * 2 - 20);

	// Sky
	background_ = sf::RectangleShape( sf::Vector2f(width_, height_) );
	background_.setFillColor(sf::Color(30, 30, 30));

	// Text
	text_time_ = sf::Text();
	text_time_.setFont(font);
	text_time_.setStyle(sf::Text::Regular);
	text_time_.setPosition(0, 0);
	text_time_.setFillColor(sf::Color(255, 255, 255));
	text_time_.setCharacterSize(30);

	text_position_ = sf::Text(text_time_);
	text_position_.setPosition(0, 40);
	text_angle_ = sf::Text(text_time_);
	text_angle_.setPosition(0, 80);
	text_ballspeed_ = sf::Text(text_time_);
	text_ballspeed_.setPosition(0, 120);
	text_info_ = sf::Text(text_time_);
	text_info_.setString("Press left or right to push the ball.");
	sf::FloatRect textRect = text_info_.getLocalBounds();
	text_info_.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text_info_.setPosition(-center_x_, height_-100);

	// Images
	texture_ = sf::Texture();
	texture_.loadFromImage(image_failed);
	sprite_failed_ = sf::Sprite();
	sprite_failed_.setTexture(texture_, true);
	sprite_failed_.setOrigin(sf::Vector2f(300.0, 200.0));
	sprite_failed_.setPosition(-center_x_, -center_y_);


	//===================== SIMULATION =====================
	rocker_ = Rocker(0,25,0);
	controller_ = PID_controller(0.2, 0, 0.08, - 180.0 / 8, 180.0 / 8);
	time_ = 0;
}
