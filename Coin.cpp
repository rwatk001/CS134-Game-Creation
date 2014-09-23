// Coin.cpp
// Coin class member-function definitions

#include "Coin.h"
#include "Obstacle.h"
#include "Ship.h"
#include "Game.h"
using namespace Ogre;

Coin::Coin(SceneManager *ptr) {
	sceneManagerPtr = ptr;
	speed = 150;
	direction = Vector3(-1,0,0);
}

Coin::~Coin() {}

void Coin::addToScene() {
	// create entity and attach it to a node in the scene
	Entity *entityPtr = sceneManagerPtr->createEntity("Coin","sphere.mesh");
	entityPtr->setMaterialName("coin");
	//entityPtr->setNormaliseNormals(true);
	nodePtr = sceneManagerPtr->getRootSceneNode()->createChildSceneNode("Coin");
	nodePtr->attachObject(entityPtr);
	nodePtr->setScale(0.05,0.05,0.05);
	spawnCoin();
}

void Coin::spawnCoin() {
	int newPos = rand() % 100 + 1;
	int neg = rand() % 2 + 1;
	if (neg == 1)
		nodePtr->setPosition(150,newPos,0);
	else
		nodePtr->setPosition(150,-newPos,0);
}

void Coin::moveCoin(Real time) {
	nodePtr->translate((direction * (speed * time)));
	Vector3 position = nodePtr->getPosition();

	Vector3 leftPosition(-150,0,0);

	// check if ball hits the left side
	if ((position.x - RADIUS) <= leftPosition.x) {
		spawnCoin();
	}
    hitShip();
}

void Coin::hitShip() {
	Vector3 shipPosition = sceneManagerPtr->getSceneNode("Ship")->getPosition();
	Vector3 coinPosition = nodePtr->getPosition();

	const int PADDLE_WIDTH = 3;
	const int PADDLE_HEIGHT = 2;

	// is the coin is in range of the ship
	if ((coinPosition.x - RADIUS) < (shipPosition.x + (PADDLE_WIDTH / 2)) &&
		(coinPosition.x + RADIUS) > (shipPosition.x - (PADDLE_WIDTH / 2)) &&
		(coinPosition.y - RADIUS) < (shipPosition.y + (PADDLE_HEIGHT / 2)) && 
		(coinPosition.y + RADIUS) > (shipPosition.y + (PADDLE_HEIGHT / 2)) ) {
		
		Game::updateScore(PLAYER1);
		spawnCoin();
	}
}

void Coin::reset () {
	spawnCoin();
}