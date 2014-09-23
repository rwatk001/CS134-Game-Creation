// Coin.h 
// Coin call definitions (represents the collectable to increase score)

#ifndef COIN_H
#define COIN_H

#include <Ogre.h>

using namespace Ogre;

const int RADIUS = 10;

class Coin {
	public:
		Coin(SceneManager *sceneManagerPtr);
		~Coin();
		void addToScene();
		void moveCoin(Real time);
		void reset();

	private:
		SceneManager *sceneManagerPtr;
		SceneNode *nodePtr;
		int speed;
		Vector3 direction;

		// private util functions
		void spawnCoin();
		void hitShip();
};

#endif