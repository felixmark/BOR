// BalanceUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include "../BalanceLib/Rocker.h"

using namespace std;
using namespace chrono;

double rad(double grad) {
	return (PI * grad) / 180;
}

int main() {
	//===================== SETUP =====================
	sf::ContextSettings settings;
	//settings.antialiasingLevel = 8;
	const int height = 900;
	const int width = 1600;

	sf::RenderWindow window(sf::VideoMode(width, height), "BOR", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	const int center_x = -width / 2;
	const int center_y = -height / 2;
	const float ball_radius = 20;
	const int wippenlaenge = 200;
	const int magnif_factor = 4;
	const float rockerbase_radius = (cos(1.178097)*((wippenlaenge / 2) * magnif_factor)) / 2;


	//===================== VISUAL SFML OBJECTS =====================
	// Ball
	sf::CircleShape circle{ ball_radius };

	// Rocker
	sf::RectangleShape bar{ sf::Vector2f(wippenlaenge * magnif_factor, 20) };
	bar.setFillColor(sf::Color(0, 150, 200));
	bar.setOrigin(center_x+ wippenlaenge/2 * magnif_factor, center_y-ball_radius*2);
	
	// Rocker Base
	sf::CircleShape rockerbase_top{ rockerbase_radius };
	rockerbase_top.setFillColor(sf::Color(0, 50, 130));
	rockerbase_top.setOrigin(center_x+rockerbase_radius, center_y - ball_radius * 3);
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
	sf::RectangleShape background{ sf::Vector2f(width, height) };
	background.setFillColor(sf::Color(30, 30, 30));

	// Text
	sf::Text text1;
	sf::Font font;
	if (!font.loadFromFile("OpenSans-Regular.ttf")) {
		cout << "Font not found." << endl;
		return 1;
	}
	text1.setFont(font);
	text1.setStyle(sf::Text::Regular);
	text1.setPosition(0,0);
	text1.setFillColor(sf::Color(255,255,255));
	text1.setCharacterSize(30);

	sf::Text text2(text1);
	text2.setPosition(0, 40);
	sf::Text text3(text1);
	text3.setPosition(0, 80);
	sf::Text text4(text1);
	text4.setPosition(0, 120);

	// Images
	sf::Image failed;
	failed.loadFromFile("failed.png");
	sf::Texture texture;
	texture.loadFromImage(failed);
	sf::Sprite sprite_failed;
	sprite_failed.setTexture(texture, true);
	sprite_failed.setOrigin(sf::Vector2f(300.0, 200.0));
	sprite_failed.setPosition(-center_x,-center_y);


	//===================== SIMULATION =====================
	Rocker rocker(90, 30, 0);
	cout << rocker;

	long time = 0;
	//auto last_sim = system_clock::now().time_since_epoch();
	//auto last_switch = system_clock::now().time_since_epoch();




	//===================== START BALANCING THE BALL =====================
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		window.clear();


		//===================== SIMULATE =====================
		int sim_time = 20;
		
		//vvvvvvvvvvvvvv Replace with PID controller vvvvvvvvvvvvvv
		if (time++ % 50 == 0) {
			rocker.set_angle(-rocker.get_angle());
		}
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		for (long cnt = 0; cnt < sim_time; ++cnt) { rocker.step(); }


		//===================== SET BALL POSITION, ROCKER ROTATION AND TEXT =====================
		circle.setOrigin(
			center_x+ball_radius-(cos(rad(rocker.get_angle()))*rocker.get_position()*magnif_factor),
			center_y+(sin(rad(rocker.get_angle()))*rocker.get_position()*magnif_factor)
		);
		sf::Transform transform;
		transform.rotate(
			-rocker.get_angle(), 
			{ static_cast<float>(-center_x), static_cast<float>(-center_y) }
		);
		text1.setString("Simulated time: " + to_string(rocker.get_time_elapsed())+" s");
		text2.setString("Position: "+to_string(rocker.get_position())+" cm");
		text3.setString("Angle: " + to_string(rocker.get_angle())+" °");
		text4.setString("Ball speed: " + to_string(rocker.get_ball_speed())+" m/s");


		//===================== DRAW EVERYTHING =====================
		window.draw(background);
		window.draw(ground);
		window.draw(circle);
		window.draw(rockerbase_top);
		window.draw(rockerbase_bottom);
		window.draw(bar, transform);
		window.draw(text1);
		window.draw(text2);
		window.draw(text3);
		window.draw(text4);


		//===================== UPDATE THE WINDOW =====================
		if (!rocker.is_ball_on_rocker()) {
			window.draw(sprite_failed);
			window.display();
			std::this_thread::sleep_for(std::chrono::seconds(3));
			window.close();
		} else {
			window.display();
		}
	}
	return EXIT_SUCCESS;
}

