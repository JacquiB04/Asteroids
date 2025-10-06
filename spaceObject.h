/** -------------------------------------------------------------
* @file    spaceObject.h
* @author  Jacqui Bouchard
* @version 1.0
*
* CS162-01 - Assignment 3.2
*   This file holds the SpaceObject Class, struct, SpaceObjType,
*	and other functions' definitions.
*
* ------------------------------------------------------------ */

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include <SFML/Graphics.hpp>

struct Point {
	double x;
	double y;
};

enum SpaceObjType { SHIP, ASTEROID, PHOTON_TORPEDO, SHIP_EXPLODING, SHIP_GONE };


class SpaceObject {

public:

	//-----------------------------------------------------------
	/**
	*	Initializes ALL member variables:
	*		- Type set to ASTEROID
	*		- Radius set to 20
	*		- Other variables given reasonable values
	*/
	SpaceObject();


	//-----------------------------------------------------------
	/**
	*	Initializes all member variables to given values
	*		- If values are invalid, use reasonable default values
	*
	*	@param type - space object type
	*	@param radius - radius of type
	*	@param location - location of type (x and y coordinates)
	*	@param velocity - current velocity in pixels/frame
	*	@param angle - angle of type
	*	@param timesDrawn - times SpaceObject has been drawn
	*/
	SpaceObject(SpaceObjType type, double radius,
		Point location, Point velocity, double angle, int timesDrawn);


	//-----------------------------------------------------------
	//                          MUTATORS


	//-----------------------------------------------------------
	/**
	*	Radius Mutator: validates values given and sets associated 
	*					member variables
	*
	*	@param radius - radius of type
	*	@return false - if given value is negative or
	*			  > 1/2 screen height
	*/
	bool setRadius(double radius);


	//-----------------------------------------------------------
	/**
	*	Location Mutator: validates value given and sets associated
	*					  member values. If values outside of 
	*					  range, it adjusts them.
	*
	*	@param x - x-coordinate of location
	*	@param y - y-coordinate of location
	*/
	void setLocation(double x, double y);


	//-----------------------------------------------------------
	/**
	*	Velocity Mutator - validates values given and sets 
	*					   associated member variables
	*
	*	@param velocityX - number of pixels moving in the x 
						   direction
	*	@param velocityY - number of pixels moving in the y 
						   direction
	*	@return true - if values are valid in the specified range
	*/
	bool setVelocity(double velocityX, double velocityY);


	//-----------------------------------------------------------
	/**
	*	Angle Mutator - validates values given and sets 
	*					associated member variables
	*
	*	@param angDeg - degree of the angle
	*/
	void setAngle(double angDeg);


	//-----------------------------------------------------------
	/**
	*	Changes angle by given amount if type is SHIP
	*
	*	@param deltaDeg - amount of change in degrees
	*/
	void changeAngle(double deltaDeg);


	//-----------------------------------------------------------
	//                          ACCESSORS


	//-----------------------------------------------------------
	/**
	*	Radius Accessor - accesses and returns copy of the radius
	*
	*	@return radius - radius of the type
	*/
	double getRadius() const;


	//-----------------------------------------------------------
	/**
	*	Location Accessor - accesses and returns copy of the 
	*						location
	*
	*	@return location - x- and y-coordinates
	*/
	Point getLocation() const;


	//-----------------------------------------------------------
	/**
	*	Velocity accessor - accesses and returns copy of the 
	*						velocity
	*
	*	@return velocityX - x-value of velocity
	*	@return velocityY - y-value of velocity
	*/
	Point getVelocity() const;


	//-----------------------------------------------------------
	/**
	*	Angle Accessor - accesses and returns copy of angle
	*
	*	@return angDeg - degree of the angle
	*/
	double getAngle() const;


	//-----------------------------------------------------------
	/**
	*	Accesses and returns copy of the timesDrawn variable
	* 
	*	@return timesDrawn - number of times the SpaceObject has 
	*						 been drawn
	*/
	int getTimesDrawn() const;


	//-----------------------------------------------------------
	//                             OTHERS


	//-----------------------------------------------------------
	/**
	*	Updates location based on current velocity. Calls
	*	setLocation() to pass and validate new computed values.
	*
	*/
	void updatePosition();


	//-----------------------------------------------------------
	/**
	*	Prints data of objects for testing
	* 
	*/
	void dumpData() const;


	/** ---------------------------------------------------------
	*   Draws the spaceobject on the given window
	*
	* 	@param win - the window on which we値l draw the space 
	*				 object
	---------------------------------------------------------- */
	void draw(sf::RenderWindow& win);


	/** ----------------------------------------------------------
	*	Simulates firing the engine. Changes the current velocity 
	*	based on the angle the SpaceObject is facing
	* 
	*	@param engineThrust - amount of thrust to be applied
	* 
	----------------------------------------------------------- */
	void applyThrust(double engineThrust);


	//-----------------------------------------------------------
	/**
	*	Changes the SpaceObjType to SHIP_EXPLODING and velocity
	*	to 0.
	*/
	void explode();


	//-----------------------------------------------------------
	//					RANDOM GENERATORS

	//-----------------------------------------------------------
	/**
	*	Generates random x and y values for a location
	* 
	*	@return location - random x- and y-values between 0 and  
	*					   a screen constant for an object's 
	*					   location
	*/
	Point getRandomLocation();


	//-----------------------------------------------------------
	/**
	*	Generates random velocity values
	* 
	*	@return velocity - random x- and y- values  between -1 
	*					   and 1 for an object's velocity
	*/
	Point getRandomVelocity();


private:

	//-----------------------------------------------------------
	/**
	*   Draws the spaceobject on the given window
	*
	* 	@param win - the window on which we値l draw the asteroid
	*/
	void drawAsteroid(sf::RenderWindow& win);


	//-----------------------------------------------------------
	/**
	*   Draws the spaceobject on the given window as a ship
	*
	* 	@param win - the window on which we値l draw the ship
	*/
	void drawShip(sf::RenderWindow& win);


	//-----------------------------------------------------------
	/**
	*   Draws exploding ship on the given window
	*
	* 	@param win - the window on which we値l draw the ship
	*/
	void drawExplodingShip(sf::RenderWindow& win);
	

	//-----------------------------------------------------------
	//				PRIVATE MEMBER VARIABLES

	SpaceObjType type;	  //type of object
	Point location;		  //current location (x,y)
	Point velocity;		  //current velocity (in pixels/frame)
	double angleDeg;	  //angle object is facing (in degrees)
	double radius;		  //gross radius of object (for collision 
						  //detection)
	int timesDrawn;		  // times a SpaceObject has been drawn
};


//-----------------------------------------------------------
//					OUTSIDE THE CLASS 


//-----------------------------------------------------------
/**
*	Tests whether two objects intersect by seeing if their
*	radii overlap.
* 
*	@param object1 - first SpaceObject pointer to be tested
*	@param object2 - second SpaceObject pointer to be tested
* 
*	@return true - if the distance between the objects'
*				   positions is less than or equal to the sum 
*				   of their radii (they're overlapping).
*/
bool objectsIntersect(SpaceObject* object1, SpaceObject* object2);

#endif