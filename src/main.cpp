#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

	sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "GScript", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
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


		window.clear();
		

		window.display();
	}

    return 0;
}
