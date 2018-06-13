#include "stdafx.h"
#include "UIHandler.h"
#include <SFML\Graphics.hpp>
#include "../BalanceLib/Rocker.h"
#include <chrono>
#include <thread>

using namespace std;


UI_handler::UI_handler(sf::RenderWindow & window, sf::Font& font, sf::Image& image_failed) {
	setup(window, font, image_failed);
}

void UI_handler::clear(sf::RenderWindow & window) {
	window.clear();
}

void UI_handler::step(sf::RenderWindow & window) {
	//vvvvvvvvvvvvvv Replace with PID controller vvvvvvvvvvvvvv
	//if (time++ % 50 == 0) {
	//	rocker.set_angle(-rocker.get_angle());
	//}
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	double current_position = rocker.get_position();
	controller.set_process_value(current_position);
	for (long cnt = 0; cnt < sim_time; ++cnt) {
		rocker.step();
		controller.step();
	}
	double angle = -controller.get_control_value();
	rocker.set_angle(angle);


	//===================== SET BALL POSITION, ROCKER ROTATION AND TEXT =====================
	circle.setOrigin(
		center_x + ball_radius - (
			cos(grad_to_rad(rocker.get_angle())) * 
			rocker.get_position()*magnif_factor
		),
		center_y + (
			sin(grad_to_rad(rocker.get_angle())) * 
			rocker.get_position()*magnif_factor
		)
	);
	transform = sf::Transform();
	transform.rotate(
		-rocker.get_angle(),
		{ static_cast<float>(-center_x), static_cast<float>(-center_y) }
	);
	text_time.setString("Simulated time: " + to_string(rocker.get_time_elapsed()) + " s");
	text_position.setString("Position: " + to_string(rocker.get_position()) + " cm");
	text_angle.setString("Angle: " + to_string(rocker.get_angle()) + " °");
	text_ballspeed.setString("Ball speed: " + to_string(rocker.get_ball_speed()) + " m/s");
}

void UI_handler::draw(sf::RenderWindow & window) {
	window.draw(background);
	window.draw(ground);
	window.draw(circle);
	window.draw(rockerbase_top);
	window.draw(rockerbase_bottom);
	window.draw(bar, transform);
	window.draw(text_time);
	window.draw(text_position);
	window.draw(text_angle);
	window.draw(text_ballspeed);
}

void UI_handler::update(sf::RenderWindow & window) {
	if (!rocker.is_ball_on_rocker()) {
		window.draw(sprite_failed);
		window.display();
		this_thread::sleep_for(chrono::seconds(3));
		window.close();
	} else {
		window.display();
	}
}

void UI_handler::setup(sf::RenderWindow & window, sf::Font& font, sf::Image& image_failed) {
	width = window.getView().getSize().x;
	height = window.getView().getSize().y;
	center_x = -window.getView().getSize().x / 2;
	center_y = -window.getView().getSize().y / 2;

	//===================== VISUAL SFML OBJECTS =====================
	// Ball
	circle = sf::CircleShape( ball_radius );

	// Rocker
	bar = sf::RectangleShape( sf::Vector2f(wippenlaenge * magnif_factor, 20) );
	bar.setFillColor(sf::Color(0, 150, 200));
	bar.setOrigin(center_x + wippenlaenge / 2 * magnif_factor, center_y - ball_radius * 2);

	// Rocker Base
	rockerbase_top = sf::CircleShape( rockerbase_radius );
	rockerbase_top.setFillColor(sf::Color(0, 50, 130));
	rockerbase_top.setOrigin(center_x + rockerbase_radius, center_y - ball_radius * 3);
	rockerbase_top.setPointCount(3);

	rockerbase_bottom = sf::RectangleShape( sf::Vector2f(160, 53) );
	rockerbase_bottom.setFillColor(sf::Color(0, 50, 130));
	rockerbase_bottom.setOrigin(center_x + 80, center_y - 160);

	// Ground
	ground = sf::RectangleShape( sf::Vector2f(width, height) );
	ground.setFillColor(sf::Color(40, 150, 0));
	ground.setOutlineThickness(20);
	ground.setOutlineColor(sf::Color(80, 180, 0));
	ground.setOrigin(0, center_y - ball_radius * 3 - rockerbase_radius * 2 - 20);

	// Sky
	background = sf::RectangleShape( sf::Vector2f(width, height) );
	background.setFillColor(sf::Color(30, 30, 30));

	// Text
	text_time = sf::Text();
	text_time.setFont(font);
	text_time.setStyle(sf::Text::Regular);
	text_time.setPosition(0, 0);
	text_time.setFillColor(sf::Color(255, 255, 255));
	text_time.setCharacterSize(30);

	text_position = sf::Text(text_time);
	text_position.setPosition(0, 40);
	text_angle = sf::Text(text_time);
	text_angle.setPosition(0, 80);
	text_ballspeed = sf::Text(text_time);
	text_ballspeed.setPosition(0, 120);

	// Images
	texture = sf::Texture();
	texture.loadFromImage(image_failed);
	sprite_failed = sf::Sprite();
	sprite_failed.setTexture(texture, true);
	sprite_failed.setOrigin(sf::Vector2f(300.0, 200.0));
	sprite_failed.setPosition(-center_x, -center_y);


	//===================== SIMULATION =====================
	rocker = Rocker(30, 10, 0);
	controller = PID_controller(0.1, 0.5, 0.2, - 180.0 / 8, 180.0 / 8);
	controller.set_limit_output(true);
	time = 0;
}
