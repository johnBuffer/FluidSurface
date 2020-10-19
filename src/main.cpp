#include <SFML/Graphics.hpp>
#include <iostream>
#include "domain.hpp"


int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "FluidSurface", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	Domain domain(200, 8, 70.0f);
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				const float amount = 200.0f;
				sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
				const uint64_t col = mouse_position.x / domain.width;
				const int32_t width = 20;
				const uint32_t wave_width = 2 * width + 1;
				const float PI = 3.14159f;
				for (int32_t i(-width); i <= width; ++i) {
					int32_t index = col + i;
					if (index >= 0 && index < domain.columns.size()) {
						const float ratio = i / float(wave_width);
						domain.columns[index].height += 0.05f * amount * std::pow(cos(2.0f * ratio * PI), 8.0f) + 0.95f * amount * std::pow(cos(ratio * PI), 2.0f);
					}
				}
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
