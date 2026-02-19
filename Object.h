#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <cmath>
#include <string>

class Object
{
public:
	sf::Vector2<double> pos;
	sf::Vector2<double> velocity;
	sf::Vector2<double> acceleration;
	double mass;
	double radius;
	std::string name;

	Object(std::string name, sf::Vector2<double> pos, sf::Vector2<double> velocity, double mass)
	{
		this->name = name;
		this->pos = pos;
		this->velocity = velocity;
		this->mass = mass;

		radius = 2 * sqrt(mass / 3.141f);
	}

	Object(std::string name, sf::Vector2<double> pos, double radius, sf::Vector2<double> velocity, double mass)
	{
		this->name = name;
		this->pos = pos;
		this->radius = radius;
		this->velocity = velocity;
		this->mass = mass;
	}

	sf::Vector2<double> getPosCentered();

	void MoveOld(double dt);

	double CalculateDistanceTo(Object obj);
	double CalculateDistanceSquaredTo(Object obj);

	sf::Vector2<double> CalculateGravitationalForceTo(Object obj);
	
	void AccelerateToOld(Object obj, double dt);

	sf::Vector2<double> AccelerateTo(double dt, int i);

	void Move(double dt, int i);
};