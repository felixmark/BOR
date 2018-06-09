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
	Rocker rocker(90, 30, 0);
	rocker.set_angle(-rocker.get_angle());
	auto last_sim = system_clock::now().time_since_epoch();
	auto last_switch = system_clock::now().time_since_epoch();

	// Create the main window
	sf::ContextSettings settings;
	//settings.antialiasingLevel = 8;
	const int height = 900;
	const int width = 1600;
	sf::RenderWindow window(sf::VideoMode(width, height), "Balance", sf::Style::Default, settings);

	window.setFramerateLimit(60);
	int x = -width/2;
	const int y = -height/2;
	const float radius = 20;
	
	const int wippenlaenge = 200;
	const int magnif_factor = 4;
	long time = 0;

	const float radius_mitte = (cos(1.178097)*((wippenlaenge / 2) * magnif_factor))/2;

	sf::CircleShape circle{ radius };
	sf::RectangleShape bar{ sf::Vector2f(wippenlaenge * magnif_factor, 20) };
	bar.setFillColor(sf::Color(0, 80, 180));
	bar.setOrigin(x+ wippenlaenge/2 * magnif_factor, y-radius*2);
	sf::CircleShape mitte{ radius_mitte };
	mitte.setFillColor(sf::Color(0, 50, 130));
	mitte.setOrigin(x+radius_mitte, y - radius * 3);
	sf::RectangleShape floor{ sf::Vector2f(width, 20) };
	floor.setFillColor(sf::Color(80, 180, 0));
	floor.setOrigin(0, y - radius * 3 - radius_mitte * 2);
	sf::RectangleShape floor2{ sf::Vector2f(width, height) };
	floor2.setFillColor(sf::Color(40, 150, 0));
	floor2.setOrigin(0, y - radius * 3 - radius_mitte * 2 - 20);
	sf::RectangleShape background{ sf::Vector2f(width, height) };
	background.setFillColor(sf::Color(190, 240, 255));

	sf::Text text1;
	sf::Font font;
	if (!font.loadFromFile("OpenSans-Regular.ttf")) {
		cout << "Font not found." << endl;
		return 1;
	}
	text1.setFont(font);
	text1.setStyle(sf::Text::Regular);
	text1.setPosition(0,0);
	text1.setFillColor(sf::Color(0, 0, 0));
	text1.setCharacterSize(30);
	sf::Text text2(text1);
	text2.setPosition(0, 50);
	sf::Text text3(text1);
	text3.setPosition(0, 100);
	sf::Text text4(text1);
	text4.setPosition(0, 150);

	sf::Image failed;
	failed.loadFromFile("failed.png");
	sf::Texture texture;
	texture.loadFromImage(failed);
	sf::Sprite sprite_failed;
	sprite_failed.setTexture(texture, true);
	sprite_failed.setOrigin(sf::Vector2f(300.0, 200.0));
	sprite_failed.setPosition(-x,-y);

	// Start the game loop
	while (window.isOpen()) {
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		// Clear screen
		window.clear();

		//if (duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - duration_cast<microseconds>(last_switch).count()
		//	>= 1000 * 1000) {
		//	rocker.set_angle(-rocker.get_angle());
		//	last_switch = system_clock::now().time_since_epoch();
		//}

		//long ms_passed = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - duration_cast<milliseconds>(last_sim).count();
		//for (long cnt = 0; cnt < ms_passed; ++cnt) { rocker.step(); }
		//last_sim = system_clock::now().time_since_epoch();

		int sim_time = 20;
		time = time + sim_time;
		if (time >= 1000) {
			rocker.set_angle(-rocker.get_angle());
			time = 0;
		}
		for (long cnt = 0; cnt < sim_time; ++cnt) { rocker.step(); }

		circle.setOrigin(
			x+radius-(cos(rad(rocker.get_angle()))*rocker.get_position()*magnif_factor),
			y+(sin(rad(rocker.get_angle()))*rocker.get_position()*magnif_factor)
		);
		sf::Transform transform;
		transform.rotate(
			-rocker.get_angle(), 
			{ static_cast<float>(-x), static_cast<float>(-y) }
		);
		text1.setString("Simulated time: " + to_string(rocker.get_time_elapsed()));
		text2.setString("Position: "+to_string(rocker.get_position()));
		text3.setString("Angle: " + to_string(rocker.get_angle()));
		text4.setString("Ball speed: " + to_string(rocker.get_ball_speed()));

		// Draw screen
		window.draw(background);
		window.draw(floor);
		window.draw(floor2);
		window.draw(circle);
		window.draw(mitte);
		window.draw(bar, transform);
		window.draw(text1);
		window.draw(text2);
		window.draw(text3);
		window.draw(text4);

		// Update the window
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

