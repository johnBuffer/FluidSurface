#include <SFML/Graphics.hpp>
#include <iostream>
#include "domain.hpp"


int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "GScript", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	Domain domain(80, 20, 10.5f);
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				const float amount = 150.0f;
				sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
				const uint64_t col = mouse_position.x / domain.width;
				domain.columns[col].height += amount;
				std::cout << domain.columns[col].height << std::endl;
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
			}
			else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Space) {
				}
				else if (event.key.code == sf::Keyboard::Enter) {
				}
				else if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}

		domain.update(0.008f);

		window.clear();
		
		float x = 0.0f;
		for (const Column& c : domain.columns) {
			sf::RectangleShape r(sf::Vector2f(domain.width, c.height));
			r.setFillColor(sf::Color::Blue);
			r.setOrigin(0.0f, c.height);
			r.setPosition(x, WinHeight);
			window.draw(r);

			x += domain.width;
		}

		window.display();
	}

    return 0;
}
