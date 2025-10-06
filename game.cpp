/** -------------------------------------------------------------
* @file    game.cpp
* @author  Jacqui Bouchard
* @version 1.0
*
* CS162-01 - Assignment 3.2
*   This file contains a simple game processing loop that gets
*	user input, updates SpaceObjects, and draws them on the
*	screen.
*
* ------------------------------------------------------------ */

#include <SFML/Graphics.hpp>
#include <iostream>
#include "constants.h"
#include "spaceObject.h"

//===============================================================
int main()
{
	//generate random seed for asteroid creation ----------------
	srand(42);

	//create graphics window ------------------------------------
	sf::RenderWindow window(
		sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
		"Polygons!",
		sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	//create SpaceObjects ---------------------------------------
	SpaceObject* asteroids[MAX_ASTEROIDS] = { nullptr };
	SpaceObject* ship = new SpaceObject(SHIP, 2.0, { 200.0,200.0 },
		{ 0.0,0.0 }, 0.0, 0);

	SpaceObject* photons[MAX_PHOTONS] = { nullptr };

	//dynamically create 5 new asteroids ------------------------
	for (int x = 0; x < 5; x++) {
		asteroids[x] = new SpaceObject;
		asteroids[x]->getRandomLocation();
		asteroids[x]->getRandomVelocity();
	}

	//game loop -------------------------------------------------
	while (window.isOpen())
	{
		//handle user input (events and keyboard keys being 
		//pressed)
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			//launch torpedos if spacebar is pressed-------------
			if (event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Space) {

				int p = 0;
				while (p < MAX_PHOTONS && photons[p] != nullptr)
					p++;

				if (p < MAX_PHOTONS) {
					photons[p] = new SpaceObject(PHOTON_TORPEDO,
						2.0, ship->getLocation(),
						ship->getVelocity(), ship->getAngle(), 0);

					photons[p]->applyThrust(5.0);
				}
			}
		} //while window.pollEvent

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			ship->changeAngle(-5.0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			ship->changeAngle(5.0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			ship->applyThrust(0.02);
		}

		//update game objects -----------------------------------
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			if (asteroids[i] != nullptr) {
				asteroids[i]->updatePosition();
			}
		}

		ship->updatePosition();

		for (int p = 0; p < MAX_PHOTONS; p++) {
			if (photons[p] != nullptr) {
				photons[p]->updatePosition();

				//make photon disappear after a few seconds -----
				if (photons[p]->getTimesDrawn() > PHOTON_LIFESPAN) {
					delete photons[p];
					photons[p] = 0;
				}
			}
		}

		//determine if ship and asteroid have intersected -------
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			if (asteroids[i] != nullptr) {
				if (objectsIntersect(ship, asteroids[i])) {
					ship->explode();
				}
			}
		}

		//determine if asteroid and photon have intersected -----
		for (int n = 0; n < MAX_PHOTONS; n++) {
			for (int b = 0; b < MAX_ASTEROIDS; b++) {
				if (photons[n] != nullptr && asteroids[b] != nullptr) {

					if (objectsIntersect(photons[n], asteroids[b])) {
						delete photons[n];
						photons[n] = nullptr;

						//make the original asteroid split in two
						//used constant for original asteroid size
						//because all asteroids are first constructed
						//at the same size.  
						if (asteroids[b]->getRadius() > (0.25 * ORIGINAL_ASTEROID_SIZE)) {

							int count = 0;
							int a = 0;

							//allocate memory to two new asteroids
							//each should be half the size of their parent
							while (count < 2) {
								if (a < MAX_ASTEROIDS && asteroids[a] == nullptr) {
									asteroids[a] = new SpaceObject(ASTEROID, asteroids[b]->getRadius() * 0.5,
														asteroids[b]->getLocation(), { -1, -1 }, 0, 0);
									asteroids[a]->getRandomVelocity();

									count++;
								}
								a++;
							}
							
						} //make asteroid split if radius in range

						//delete the original asteroid
						delete asteroids[b];
						asteroids[b] = nullptr;

					} //if objectsIntersect
				}
			}
		} 


		//draw new frame ----------------------------------------
		window.clear();

		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			if (asteroids[i] != nullptr) {
				asteroids[i]->draw(window);
			}
		}

		ship->draw(window);

		for (int p = 0; p < MAX_PHOTONS; p++) {
			if (photons[p] != nullptr) {
				photons[p]->draw(window);
			}
		}

		window.display();
	}

	return 0;
}
