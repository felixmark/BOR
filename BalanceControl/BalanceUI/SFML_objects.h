#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
class SFML_objects
{
public:
	SFML_objects();
	std::vector<sf::Shape> get_shapes(int width, int height) const;
	std::vector<sf::Text> get_texts(int width, int height) const;
};

