// Paddle.h

#ifndef SHIP_H
#define SHIP_H

#include <Ogre.h>
using namespace Ogre;

class Ship {
	public:
		Ship(SceneManager *sceneManagerPtr, String shipName, int positionX);
		~Ship();
		void addToScene();
		void moveShip(const Vector3 &direction, Real time);
		void reset();

	private:
		SceneManager *sceneManagerPtr;
		SceneNode *nodePtr;
		String name;
		int x;
};

#endif
