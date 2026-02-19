#include "object.h"
#include "parameters.h"

sf::Vector2<double> Object::getPosCentered()
{
	return sf::Vector2<double>(pos.x + radius, pos.y + radius);
}

void Object::MoveOld(double dt)
{
	pos += velocity * dt;
}

void Object::AccelerateToOld(Object obj, double dt)
{
	sf::Vector2<double> force = CalculateGravitationalForceTo(obj);
	velocity += force * dt;
}

double Object::CalculateDistanceTo(Object obj)
{
	sf::Vector2<double> dpos(obj.getPosCentered() - this->getPosCentered());
	return dpos.length();
}

double Object::CalculateDistanceSquaredTo(Object obj)
{
	sf::Vector2<double> dpos(obj.getPosCentered() - this->getPosCentered());
	return dpos.lengthSquared();
}

sf::Vector2<double> Object::CalculateGravitationalForceTo(Object obj)
{
	double distanceSquared = this->CalculateDistanceSquaredTo(obj);
	double force = 0;
	if (distanceSquared >= 2)
		force = GRAVITY_CONST * (obj.mass / distanceSquared);

	sf::Vector2<double> dir = (obj.getPosCentered() - this->getPosCentered()).normalized();

	return dir * force;
}

sf::Vector2<double> Object::AccelerateTo(double dt, int i)
{
	sf::Vector2<double> newAcc = { 0, 0 };

	for (int j = 0; j < objects.size(); j++)
	{
		if (i != j)
			newAcc += CalculateGravitationalForceTo(objects[j]);
	}

	return newAcc;
}

void Object::Move(double dt, int i)
{
	sf::Vector2<double> newPos = pos + velocity * dt + acceleration * (dt * dt * 0.5);
	sf::Vector2<double> newAcc = AccelerateTo(dt, i);
	sf::Vector2<double> newVel = velocity + (acceleration + newAcc) * (dt * 0.5);

	pos = newPos;
	velocity = newVel;
	acceleration = newAcc;
}