/** -------------------------------------------------------------
* @file    spaceObject.cpp
* @author  Jacqui Bouchard
* @version 1.0
*
* CS162-01 - Assignment 3.2
*   This file fully defines each function documented in
*	spaceObject.h and implements the class data type. 
*
* ------------------------------------------------------------ */

#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include "spaceObject.h"
#include "constants.h"

// -------------------------------------------------------------
SpaceObject::SpaceObject()
{
	type = ASTEROID;
	radius = 15.0;
	location = { 20.0,20.0 };
	velocity = { 1.0,1.0 };
	angleDeg = 45.0;
	timesDrawn = 0;

}

// -------------------------------------------------------------
SpaceObject::SpaceObject(SpaceObjType type, double radius, 
	Point location, Point velocity, double angle, int timesDrawn)
{
	if (type >= MIN_ENUM && type <= MAX_ENUM) {
		this->type = type;
	}
	else {
		type = ASTEROID;
	}

	if (!setRadius(radius)) {
		this->radius = 5.0;
	}

	setLocation(location.x, location.y);
	
	if (!setVelocity(velocity.x, velocity.y)) {
		setVelocity(this->velocity.x, this->velocity.y);
	}

	setAngle(angle);

	this->timesDrawn = 0;
}

// -------------------------------------------------------------
//                      MUTATORS

// -------------------------------------------------------------
bool SpaceObject::setRadius(double radius)
{
	if (radius < 0 || radius > (SCREEN_HEIGHT / 2.0)) {
		std::cout << "*Radius is invalid. Ignore change.*\n\n";

		return false;
	}
	else {
		this->radius = radius;
		return true;
		assert(this->radius > 0 && radius < (SCREEN_HEIGHT / 2.0));
	}
}

// -------------------------------------------------------------
void SpaceObject::setLocation(double x, double y)
{	
	while (x > SCREEN_WIDTH) {
		x -= SCREEN_WIDTH;
	}
	while (x < 0) {
		x += SCREEN_WIDTH;
	}
	location.x = x;
	assert(this->location.x >= 0
		&& this->location.x <= SCREEN_WIDTH);

	while (y > SCREEN_HEIGHT) {
		y -= SCREEN_HEIGHT;
	}
	while (y < 0) {
		y += SCREEN_HEIGHT;
	}
	location.y = y;
	assert(this->location.y >= 0 
		&& this->location.y <= SCREEN_HEIGHT);

}

// -------------------------------------------------------------
bool SpaceObject::setVelocity(double velocityX, double velocityY)
{
	if (velocityX > 200 || velocityY > 200) {	// 200 is too fast! 
												// Can be negative 
												// to go down/left
		std::cout << "*Velocity is invalid*\n\n";
		return false;
	}
	else {
		velocity.x = velocityX;
		velocity.y = velocityY;
		assert(this->velocity.x < 200 && this->velocity.y < 200);

		return true;
	}
}

// -------------------------------------------------------------
void SpaceObject::setAngle(double angDeg)
{
	angleDeg = angDeg; // no invalid values so that
					   // user has freewill to rotate
					   // in any way they'd like
}

// -------------------------------------------------------------
void SpaceObject::changeAngle(double deltaDeg)
{
	if (type == SHIP) {
		setAngle(angleDeg + deltaDeg);
	}
}

//--------------------------------------------------------------
//                          ACCESSORS

// -------------------------------------------------------------
double SpaceObject::getRadius() const
{
	return radius;
}

// -------------------------------------------------------------
Point SpaceObject::getLocation() const
{
	return Point(location);
}

// -------------------------------------------------------------
Point SpaceObject::getVelocity() const
{
	return Point(velocity);
}

// -------------------------------------------------------------
double SpaceObject::getAngle() const
{
	return angleDeg;
}

// -------------------------------------------------------------
int SpaceObject::getTimesDrawn() const
{
	return timesDrawn;
}

//--------------------------------------------------------------
//                             OTHERS

// -------------------------------------------------------------
void SpaceObject::updatePosition()
{
	setLocation(location.x + velocity.x, location.y + velocity.y);
}

// -------------------------------------------------------------
void SpaceObject::dumpData() const
{
	std::cout << "Space object: ";

	if (type == 0) {
		std::cout << "Ship\n";
	}
	else if (type == 1) {
		std::cout << "Asteroid\n";
	} 
	else {
		std::cout << "Photon Torpedo";
	}

	std::cout << "Object location: (" << location.x << ", "
		<< location.y << ")\n";
	std::cout << "Object velocity: (" << velocity.x << ", "
		<< velocity.y << ") in pixels/frame\n";
	std::cout << "Angle of object: " << angleDeg << " degrees\n";
	std::cout << "Radius of object: " << radius << "\n\n";

}

// -------------------------------------------------------------
void SpaceObject::applyThrust(double engineThrust) {
	if (type == SHIP || type == PHOTON_TORPEDO) {

		double forceX = cos((angleDeg - 90) * PI / 180) 
						* engineThrust;
		double forceY = sin((angleDeg - 90) * PI / 180) 
						* engineThrust;
		// Note; the - 90 is there because an angle of 0 
		// is usually pointing to the right and we want it 
		// to be pointing up.

		velocity.x = velocity.x + forceX;
		velocity.y = velocity.y + forceY;
	}
}

// -------------------------------------------------------------
//						DRAWING FUNCTIONS

// -------------------------------------------------------------
void SpaceObject::draw(sf::RenderWindow& win) {
	if (type == SHIP) {
		drawShip(win);
	}
	
	if (type == SHIP_EXPLODING) {
		drawExplodingShip(win);
	} 
	
	if (type == ASTEROID || type == PHOTON_TORPEDO) {
		drawAsteroid(win);
	}
	
	if (type == SHIP_GONE) {	// redraw ship
		type = SHIP;
		setLocation(200, 200);
	}

	timesDrawn++;
}

// -------------------------------------------------------------
void SpaceObject::drawAsteroid(sf::RenderWindow& win) {
	//Configure a graphics object to be used for drawing our object.
	//This code draws a simple pentagon
	int points = 5;
	sf::CircleShape shape(radius, points); //radius from our SpaceObject

	sf::Vector2f midpoint(radius, radius);
	shape.setOrigin(midpoint);

	shape.setFillColor(sf::Color(0, 0, 0));
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color(255, 255, 255));

	//apply our object position to the graphics object and draw it
	shape.setPosition(location.x, location.y);
	shape.setRotation(angleDeg);

	win.draw(shape);
}

// -------------------------------------------------------------
void SpaceObject::drawShip(sf::RenderWindow& win) {
	// draw ship
	sf::ConvexShape shipShape;
	shipShape.setPointCount(3);
	shipShape.setPoint(0, sf::Vector2f(10, 0));
	shipShape.setPoint(1, sf::Vector2f(0, 25));
	shipShape.setPoint(2, sf::Vector2f(20, 25));

	sf::Vector2f midpoint(10, 15);
	shipShape.setOrigin(midpoint);

	shipShape.setFillColor(sf::Color(0, 0, 0));
	shipShape.setOutlineThickness(1);
	shipShape.setOutlineColor(sf::Color(255, 255, 255));

	shipShape.setPosition(location.x, location.y);
	shipShape.setRotation(angleDeg);
	win.draw(shipShape);
}

// -------------------------------------------------------------
void SpaceObject::drawExplodingShip(sf::RenderWindow& win) {
	int points = 360;
	sf::CircleShape explodeShape(radius, points); //radius from 
												//our SpaceObject
	sf::Vector2f midpoint(radius, radius);
	explodeShape.setOrigin(midpoint);

	explodeShape.setFillColor(sf::Color(250, 175, 0));
	explodeShape.setOutlineThickness(1);
	explodeShape.setOutlineColor(sf::Color(255, 0, 0));

	//apply our object position to the graphics object and draw it
	explodeShape.setPosition(location.x, location.y);
	explodeShape.setRotation(angleDeg);

	if (radius <= MAX_EXPLODE_SIZE) {
		win.draw(explodeShape);
		radius += 3;
	} 
	else {
		type = SHIP_GONE;
		radius = 2.0;
	}
}

// -------------------------------------------------------------
//					RANDOM GENERATORS

// -------------------------------------------------------------
Point SpaceObject::getRandomLocation() {
	int rangeX = SCREEN_WIDTH - 0 + 1;	 //high-low+1
	int rangeY = SCREEN_HEIGHT - 0 + 1;

	location.x = rand() % rangeX;
	location.y = rand() % rangeY;

	return location;
}

// -------------------------------------------------------------
Point SpaceObject::getRandomVelocity() {
	double low = -1.0;
	double high = 1.0;
	
	if (high < low) {
		int temp = high;
		high = low;
		low = temp;
	}

	int  range = (high - low + .001) * 1000;

	velocity.x = (rand() % range / static_cast<double>(1000)) + low;
	velocity.y = (rand() % range / static_cast<double>(1000)) + low;

	return velocity;
}

// -------------------------------------------------------------
//                         EXPLOSION FUNCTIONS

// -------------------------------------------------------------
bool objectsIntersect(SpaceObject* object1, SpaceObject* object2) {
	Point position1 = object1->getLocation();
	Point position2 = object2->getLocation();

	int sum = object1->getRadius() + object2->getRadius();
	int distance = sqrt(pow(position2.x - position1.x, 2) + 
		pow(position2.y - position1.y, 2));

	if (distance <= sum) {
		return true;
	}
	else {
		return false;
	}
}

// -------------------------------------------------------------
void SpaceObject::explode() {
	type = SHIP_EXPLODING;

	velocity.x = 0;
	velocity.y = 0;
}