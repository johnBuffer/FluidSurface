#include <SFML/Graphics.hpp>
#include <iostream>
#include "domain.hpp"


int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	constexpr uint32_t WinWidth = 1920;
	constexpr uint32_t WinHeight = 1080;

	sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "FluidSurface", sf::Style::Fullscreen, settings);
	window.setVerticalSyncEnabled(true);

	Domain domain(192, 11, 800.0f);

	//domain.objects.emplace_back();
	//domain.objects.back().radius = 150.0f;

	const float normalizer = 1.0f / sqrt(2.0f);
	const sf::Vector2f sun_direction(-1.0f * normalizer, 1.0f * normalizer);
	
	while (window.isOpen())
	{
		sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				float amount = 200.0f;
				if (event.mouseButton.button == sf::Mouse::Right) {
					amount *= -1.0f;
				}
				const uint64_t col = mouse_position.x / domain.width;
				const int32_t width = 30;
				const uint32_t wave_width = 2 * width + 1;
				const float PI = 3.14159f;
				for (int32_t i(-width); i <= width; ++i) {
					int32_t index = col + i;
					if (index >= 0 && index < domain.columns.size()) {
						const float ratio = i / float(wave_width);
						domain.columns[index].height += amount * std::pow(cos(ratio * PI), 3.0f);
						domain.columns[index].height += 0.025f * amount * std::pow(cos(4.0f * ratio * PI), 3.0f);
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
		
		const uint64_t count = domain.columns.size() - 1;
		sf::VertexArray va(sf::Quads, 4 * count);
		for (uint32_t i(0); i < count; ++i) {
			uint32_t  index = 4 * i;
			const Column& c = domain.columns[i];
			const Column& next_c = domain.columns[i+1];
			va[index + 0].position = sf::Vector2f(i * domain.width, WinHeight);
			va[index + 1].position = sf::Vector2f(i * domain.width, WinHeight - c.height);
			va[index + 2].position = sf::Vector2f((i + 1) * domain.width, WinHeight - next_c.height);
			va[index + 3].position = sf::Vector2f((i + 1) * domain.width, WinHeight);

			va[index + 0].color = sf::Color::Blue;
			va[index + 1].color = sf::Color::Blue;
			va[index + 2].color = sf::Color::Blue;
			va[index + 3].color = sf::Color::Blue;
		}
		window.draw(va);

		const float width = 20.0f;
		sf::VertexArray va_spec(sf::TriangleStrip, 2 * (count + 1));
		va_spec[0].position = sf::Vector2f(0.0f, WinHeight - domain.columns[0].height);
		va_spec[0].color = sf::Color::Blue;
		va_spec[1].position = sf::Vector2f(0.0f, WinHeight - domain.columns[0].height + width);
		va_spec[1].color = sf::Color::Blue;
		for (uint32_t i(1); i < count+1; ++i) {
			uint32_t index = 2*i;
			const Column& c = domain.columns[i];

			va_spec[index + 0].position = sf::Vector2f(i * domain.width, WinHeight - c.height);
			const sf::Vector2f v = va_spec[index].position - va_spec[index - 2].position;
			const float length = sqrt(v.x*v.x + v.y*v.y);
			const sf::Vector2f normal(-v.y / length, v.x / length);
			const float dot = std::pow(std::max(0.0f, normal.x * sun_direction.x + normal.y * sun_direction.y), 4.0f);
			va_spec[index + 1].position = va_spec[index + 0].position + normal * width * dot;

			va_spec[index + 0].color = sf::Color(dot * 255, dot * 255, 255);
			va_spec[index + 1].color = sf::Color::Blue;
		}
		window.draw(va_spec);

		for (const Object& o : domain.objects) {
			sf::CircleShape circle(o.radius);
			circle.setOrigin(o.radius, o.radius);
			circle.setPosition(o.x, o.y);
			circle.setFillColor(sf::Color(255, 255, 0, 128));
			window.draw(circle);
		}

		window.display();
	}

    return 0;
}
