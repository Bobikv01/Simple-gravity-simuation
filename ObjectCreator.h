#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "object.h"
#include "parameters.h"

class ObjectCreator
{
public:
	void SetShapes();

	void SetObjects();

	void SetTrails();

	void CreateObjectsCircle(int count, double radius, sf::Vector2<double> center);

	void CreateObjectsCircleWithStar(int count, double radius, sf::Vector2<double> center, double starMass, double starRadius, sf::Vector2<double> starVelocity);

	void CreateObjectsCircleWithManyStars(int count, double radius, sf::Vector2<double> center, double starMass, double starRadius, sf::Vector2<double> starVelocity, int starCount);

	void SolarSystem();
	void SolarSystemNew();
};

