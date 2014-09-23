// Wall.h 
// Moving Background objects

#ifndef WALL_H
#define WALL_H

#include <Ogre.h>

using namespace Ogre;

class Wall {
	public:
		Wall(SceneManager *sceneManagerPtr);
		~Wall();
		void addToScene();
		void moveWall(Real time);

	private:
		SceneManager *sceneManagerPtr;
		SceneNode *nodePtr;
		int speed;
		Vector3 direction;

		// private util functions
		void hitShip();
		void checkMaterial();
};

#endif
