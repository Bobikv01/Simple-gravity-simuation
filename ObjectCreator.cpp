#pragma once
#include "ObjectCreator.h"

void ObjectCreator::SetObjects()
{
	objects.clear();

	// Change generation algorithm
	//CreateObjectsCircle(32, 5000, { 0, 0 });
	//CreateObjectsCircleWithStar(32, 20000, { 0, 0 }, 300000, 500, { 0, 0 });
	SolarSystem();

	SetShapes();
}


void ObjectCreator::SetShapes()
{
	shapes.clear();
	for (int i = 0; i < objects.size(); i++)
	{
		shapes.push_back(sf::CircleShape(objects[i].radius));
		shapes[i].setFillColor(sf::Color{ (uint8_t)(rand() % 255), (uint8_t)(rand() % 255), (uint8_t)(rand() % 255) });
	}

	SetTrails();
}

void ObjectCreator::SetTrails()
{
	trail.clear();

	for (int i = 0; i < objects.size(); i++)
	{
		trail.push_back(std::vector<sf::Vertex>());
	}
}

void ObjectCreator::CreateObjectsCircle(int count, double radius, sf::Vector2<double> center)
{
	sf::Vector2<double> rotation({ 0, 1 });
	sf::Angle angle = sf::radians(6.28318530718f / count);
	sf::Vector2<double> position = radius * rotation + center;

	for (int i = 0; i < count; i++)
	{
		sf::Vector2<double> vel({ rotation.perpendicular().x / 10, rotation.perpendicular().y / 10 });
		double mass = 50 + rand() % 10000;
		objects.push_back(Object("planet " + i, radius * rotation, vel, mass));

		rotation = rotation.rotatedBy(angle);
	}
}


void ObjectCreator::CreateObjectsCircleWithStar(int count, double radius, sf::Vector2<double> center, double starMass, double starRadius, sf::Vector2<double> starVelocity)
{
	sf::Vector2<double> rotation({ 0, 1 });
	sf::Angle angle = sf::radians(6.28318530718f / count);
	sf::Vector2<double> position = radius * rotation + center;

	objects.push_back(Object("sun", { 0 - starRadius, 0 - starRadius }, starVelocity, starMass));

	for (int i = 0; i < count; i++)
	{
		sf::Vector2<double> vel({ rotation.perpendicular().x / 10, rotation.perpendicular().y / 10 });

		float mass = 50 + rand() % 10000;
		objects.push_back(Object("planet " + i, radius * rotation, starVelocity + vel, mass));

		rotation = rotation.rotatedBy(angle);
	}
}

void ObjectCreator::CreateObjectsCircleWithManyStars(int count, double radius, sf::Vector2<double> center, double starMass, double starRadius, sf::Vector2<double> starVelocity, int starCount)
{
	sf::Vector2<double> rotation({ 0, 1 });
	sf::Angle angle = sf::radians(6.28318530718f / count);
	sf::Vector2<double> position = radius * rotation + center;

	for (int i = 0; i < starCount; i++)
	{
		sf::Vector2<double> vel({ rotation.perpendicular().x / 50, rotation.perpendicular().y / 50 });
		objects.push_back(Object("star " + i, radius / 5 * rotation, starVelocity + vel, starMass + rand() % (int)(starMass / 100)));

		rotation = rotation.rotatedBy(angle);

	}

	for (int i = 0; i < count; i++)
	{
		sf::Vector2<double> vel({ rotation.perpendicular().x / 10, rotation.perpendicular().y / 10 });
		objects.push_back(Object("planet " + i, radius * rotation, vel, 100 + rand() % 900));

		rotation = rotation.rotatedBy(angle);
	}
}

void ObjectCreator::SolarSystem()
{
	// Sun
	objects.push_back(Object("Sun", { -696340000, -696340000 }, 696340000, {0, 0}, 1.989e+30));

	// Mercury
	objects.push_back(Object("Mercury", { 46001009000 - 2439700, -2439700 }, 2439700, { 0, 59000 }, 3.33022e+23));
	// Venus
	objects.push_back(Object("Venus", { 107476259000 - 6051800, -6051800 }, 6051800, { 0, 36259 }, 4.8675e+24));
	// Earth
	objects.push_back(Object("Earth", { 147098450000 - 6378137, -6378137 }, 6378137, { 0, 30290 }, 5.9726e+24));
	// Moon
	objects.push_back(Object("Moon", { 147098450000 + 362600000 - 1737400, -1737400 }, 1737400, { 0, 30290 + 1080 }, 7.346e+22));
	// Mars
	objects.push_back(Object("Mars", { 206655000000 - 3396200, -3396200 }, 3396200, { 0, 26500 }, 6.4171e+23));

	// Ceres
	objects.push_back(Object("Ceres", { 381028000000 - 481500, -481500 }, 481500, { 0, 18900 }, 9.393e+20));
	// Vesta
	objects.push_back(Object("Vesta", { 321766000000 - 262700, -262700 }, 262700, { 0, 20600 }, 2.59076e+20));
	// Pallas
	objects.push_back(Object("Pallas", { 319008000000 - 481500, -481500 }, 256000, { 0, 20300 }, 2.11e+20));
	// Hygea
	objects.push_back(Object("Hygea", { 416646000000 - 217000, -217000 }, 217000, { 0, 18907 }, 8.32e+20));

	// Jupiter
	objects.push_back(Object("Jupiter", { 7.405736e11 - 71492000, -71492000 }, 71492000, { 0, 13720 }, 1.8986e+27));
	// Saturn
	objects.push_back(Object("Saturn", { 1353572956000 - 60268000, -60268000 }, 60268000, { 0, 9990 }, 5.6846e+26));
	// Uranus
	objects.push_back(Object("Uranus", { 2748938461000 - 25559000, -25559000 }, 25559000, { 0, 7110 }, 8.6813e+25));
	// Neptune
	objects.push_back(Object("Neptune", { 4452940833000 - 24341000, -24341000 }, 24341000, { 0, 5500 }, 102409e+25));
}