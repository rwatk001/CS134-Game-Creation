// Obstacle.h 
// obstacle call definitions
// obstacle represents the the object to avoid/enemy in-game
// protrayed as a launched fire ball or falling block of ice

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <Ogre.h>
//#include <OgreAL.h> //sound plugin

using namespace Ogre;

class Ship;

class Obstacle {
	public:
		Obstacle(SceneManager *sceneManagerPtr);
		~Obstacle();
		void addToScene();
		void moveObs(Real time);
		void reset();
		

	private:
		SceneManager *sceneManagerPtr;
		SceneNode *nodePtr;
		int speed, radius;
		Vector3 direction;

		// private util functions
		void spawnObs();
		void reverseVericalDirection();
		void hitShip();
};

#endif
