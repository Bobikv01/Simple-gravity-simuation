#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "ObjectCreator.h"

static void Merge(int indexObj1, int indexObj2)
{
	sf::Vector2<double> collisionPoint({
		(objects[indexObj1].pos.x + objects[indexObj2].pos.x) / 2,
		(objects[indexObj1].pos.y + objects[indexObj2].pos.y) / 2,
		});

	double newMass = objects[indexObj1].mass + objects[indexObj2].mass;
	sf::Vector2<double> newVelocity = (objects[indexObj1].velocity * objects[indexObj1].mass + objects[indexObj2].velocity * objects[indexObj2].mass) / newMass;

	Object newObj(
		objects[indexObj1].name + " + " + objects[indexObj2].name,
		collisionPoint,
		newVelocity,
		newMass
	);

	int firstIndex = indexObj1 < indexObj2 ? indexObj1 : indexObj2;
	int secondIndex = indexObj1 + indexObj2 - firstIndex;

	objects.erase(objects.begin() + firstIndex);
	objects.erase(objects.begin() + secondIndex - 1);

	objects.push_back(newObj);

	sf::CircleShape newShape(newObj.radius);

	sf::Color newColor = sf::Color({
		(uint8_t)((shapes[indexObj1].getFillColor().r + shapes[indexObj2].getFillColor().r) / 2),
		(uint8_t)((shapes[indexObj1].getFillColor().g + shapes[indexObj2].getFillColor().g) / 2),
		(uint8_t)((shapes[indexObj1].getFillColor().b + shapes[indexObj2].getFillColor().b) / 2)
	});
	newShape.setFillColor(newColor);

	shapes.erase(shapes.begin() + firstIndex);
	shapes.erase(shapes.begin() + secondIndex - 1);

	shapes.push_back(newShape);

	std::vector<sf::Vertex> newTrail = std::vector<sf::Vertex>();
	
	oldTrail.push_back(trail[firstIndex]);
	trail.erase(trail.begin() + firstIndex);
	oldTrail.push_back(trail[secondIndex - 1]);
	trail.erase(trail.begin() + secondIndex - 1);

	trail.push_back(newTrail);
}

static void Physics(float dt)
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].Move(dt, i);
		for (int j = 0; j < objects.size(); j++)
		{
			if (i != j)
			{
				if ((objects[i].getPosCentered() - objects[j].getPosCentered()).length() < (objects[i].radius + objects[j].radius) / 2)
				{
					std::cout << "Merge " << i << " + " << j << "\n";
					Merge(i, j);
					break;
				}
			}
		}
	}
}

static void Debug()
{
	std::cout << "Printing info about all objects:\n";
	for (int i = 0; i < objects.size(); i++)
	{
		std::cout << "[" << i << "] " << objects[i].name << ":" << "\n"
			<< " - mass: " << objects[i].mass << "\n"
			<< " - radius: " << objects[i].radius << "\n"
			<< " - position: " << objects[i].pos.x << " ; " << objects[i].pos.x << "\n"
			<< " - velocity: " << objects[i].velocity.x << " ; " << objects[i].velocity.x << "\n"
			<< " - color: " << (int)shapes[i].getFillColor().r << " , " << (int)shapes[i].getFillColor().g << " , " << (int)shapes[i].getFillColor().b << "\n"
			<< "\n";	
	}
	std::cout << "input index of object to go to([0] - [" << objects.size() - 1 << "], any other key to continue simulation):\n";
	char choose = ' ';
	std::cin >> choose;
	if (choose - '0' >= 0 && choose - '0' <= objects.size() - 1)
	{
		viewCenter = { (float)objects[choose - '0'].pos.x, (float)objects[choose - '0'].pos.y };
	}
}

std::string ConverToDate(unsigned long long frame)
{
	unsigned long long milliseconds = 0, seconds = 0, minutes = 0, hours = 0, days = 0, years;
	seconds = frame % 60;
	minutes = frame / 60 % 60;
	hours = frame / 3600 % 24;
	days = frame / 3600 / 24 % 365;
	years = frame / 3600 / 24 / 365;
	return std::to_string(years) + ":" + std::to_string(days) + ":" + std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
}

int main()
{
	unsigned long long frameCount = 0;
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "SFML");
	sf::View view(viewCenter, { WINDOW_WIDTH, WINDOW_HEIGHT });
	view.zoom(viewZoom);

	ObjectCreator objectCreator = ObjectCreator();
	objectCreator.SetObjects();

	sf::Clock clock2;
	double accumulator = 0.0f;
	double secondsPassed = 0.0f;
	const double targetDt = 100; // Фиксированный шаг физики (100 Гц)

	sf::Font font("fonts/arial.ttf");
	sf::Text text(font);

	text.setString("Hello");
	text.setCharacterSize(24);
	text.setPosition({ 0, 0 });
	text.setScale({ viewZoom, viewZoom });


	while (window.isOpen())
	{
		sf::Time elapsed = clock2.restart();
		// Накапливаем реальное время, умноженное на коэффициент
		accumulator += elapsed.asSeconds() * timeMultiplier;
		secondsPassed += elapsed.asSeconds() * timeMultiplier;

		// --- EVENT MAGIC ---
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
			// Handle mouse wheel scrolling
			else if (const auto* mouse = event->getIf<sf::Event::MouseWheelScrolled>())
			{
				float viewDelta = mouse->delta == -1 ? 1.1f : 0.9f;
				view.zoom(viewDelta);
				viewZoom *= viewDelta;

				std::cout << "Vertical wheel moved. Delta: " << viewZoom << std::endl;
				// Also available: event.mouseWheelScroll.x and event.mouseWheelScroll.y 
				// for the mouse position at the time of the event.
			}

			// catch the resize events
			if (const auto* resized = event->getIf<sf::Event::Resized>())
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea({ 0, 0 }, sf::Vector2f(resized->size));
				view.setSize(visibleArea.size);
				view.zoom(viewZoom);
				view.setCenter(visibleArea.getCenter() + viewCenter * viewZoom);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			viewCenter += { -viewSpeed * viewZoom, 0 };
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			viewCenter += { viewSpeed * viewZoom, 0 };
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			viewCenter += { 0, -viewSpeed * viewZoom };
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			viewCenter += { 0, viewSpeed * viewZoom };
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			Debug();
		}

		// Пример изменения множителя клавишами (опционально)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal))
		{
			timeMultiplier *= 1.02f;
			std::cout << "timeMultiplier + : " << timeMultiplier << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen))
		{
			timeMultiplier *= 0.98f;
			std::cout << "timeMultiplier - : " << timeMultiplier << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
		{
			timeMultiplier = 1000;
			std::cout << "timeMultiplier = : " << timeMultiplier << std::endl;
		}

		view.setCenter(viewCenter);

		// --- FIXED PHYSICS LOOP ---
		// Если accumulator велик (например, из-за ускорения), 
		// цикл выполнится несколько раз за один кадр отрисовки.
		while (accumulator >= targetDt)
		{
			Physics(targetDt);
			accumulator -= targetDt;
		}

		for (int i = 0; i < objects.size(); i++)
		{
			shapes[i].setPosition({ (float)objects[i].pos.x, (float)objects[i].pos.y });
		}

		if (frameCount % 30 == 0)
		{
			// trail system
			for (int i = 0; i < objects.size(); i++)
			{
				sf::Vertex vertex = sf::Vertex();
				vertex.position = { (float)objects[i].getPosCentered().x, (float)objects[i].getPosCentered().y };
				sf::Color trailColor = sf::Color({
					(uint8_t)(shapes[i].getFillColor().r),
					(uint8_t)(shapes[i].getFillColor().g),
					(uint8_t)(shapes[i].getFillColor().b),
					(uint8_t)(127)
					});
				vertex.color = trailColor;

				trail[i].push_back(vertex);

				// Limit the number of points (alternative to time-based fading)
				if (trail[i].size() > 10000) // Example limit
				{
					trail[i].erase(trail[i].begin());
				}
			}

			// old trails
			for (int i = 0; i < oldTrail.size(); i++)
			{
				// Limit the number of points (alternative to time-based fading)
				if (oldTrail[i].size() > 10000) // Example limit
				{
					oldTrail[i].erase(oldTrail[i].begin());
				}
			}
		}

		text.setScale({ viewZoom, viewZoom });
		text.setPosition({ -WINDOW_WIDTH * viewZoom / 2 + viewCenter.x, -WINDOW_HEIGHT * viewZoom / 2 + viewCenter.y });
		
		std::string frames = "time " + ConverToDate((unsigned long long)secondsPassed);
		frames += "\ntime multi: " + std::to_string(timeMultiplier);
		frames += "\ndelta time: " + std::to_string(targetDt);


		text.setString(frames);

		// --- DRAWING ---

		window.clear();
		window.setView(view);
		
		for (int i = 0; i < objects.size(); i++)
		{
			if (!trail[i].empty())
			{
				window.draw(&trail[i][0], trail[i].size(), sf::PrimitiveType::LineStrip);
			}
		}
		for (int i = 0; i < oldTrail.size(); i++)
		{
			if (!oldTrail[i].empty())
			{
				window.draw(&oldTrail[i][0], oldTrail[i].size(), sf::PrimitiveType::LineStrip);
			}
		}

		for (int i = 0; i < objects.size(); i++)
		{
			window.draw(shapes[i]);
		}

		window.draw(text);
		window.display();

		frameCount++;
	}
}





