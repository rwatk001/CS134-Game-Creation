// Obstacle.cpp
// obstacle class member-function definitions
#include <iostream>
#include "Obstacle.h"
#include "Ship.h"
#include "Game.h"
using namespace Ogre;

Obstacle::Obstacle(SceneManager *ptr) {
	sceneManagerPtr = ptr;

	speed = 150;
	radius = 13;
	if (Game::level == 1)
		direction = Vector3(-0.5,1,0);
	else
		direction = Vector3(-0.5,-2,0);
}

Obstacle::~Obstacle() {}

void Obstacle::addToScene() {
	// create entity and attach it to a node in the scene
	Entity *entityPtr;
	//Level 1 Objects
	entityPtr = sceneManagerPtr->createEntity("Ball","sphere.mesh");
	entityPtr->setMaterialName("fireball");
	nodePtr = sceneManagerPtr->getRootSceneNode()->createChildSceneNode("Ball");
	nodePtr->attachObject(entityPtr);
	nodePtr->setScale(0.1,0.1,0.1);
		
	//Level 2 Objects
	entityPtr = sceneManagerPtr->createEntity("Cube","cube.mesh");
	entityPtr->setMaterialName("icecube");
	nodePtr = sceneManagerPtr->getRootSceneNode()->createChildSceneNode("Cube");
	nodePtr->attachObject(entityPtr);
	nodePtr->setScale(0.3,0.3,0.3);
	nodePtr->setPosition(0,150,0);
	
	spawnObs();
}

void Obstacle::moveObs(Real time) {
	nodePtr->translate((direction * (speed * time)));
	Vector3 position = nodePtr->getPosition();

	Vector3 topPosition(0,100,0);
	Vector3 bottomPosition(0,-150,0);
	Vector3 leftPosition(-170,0,0);
	Vector3 rightPosition(200,0,0);

	const int WALL_WIDTH = 5;

	if (Game::level == 1) {
		// check if Obstacle hits the left side
		if ((position.x - radius) <= leftPosition.x + (WALL_WIDTH/2)) {
			spawnObs();
		}
		// check if Obstacle hits the right side
		else if ((position.x + radius) >= rightPosition.x - (WALL_WIDTH/2)) {
			spawnObs();
		}
		// check if Obstacle hits the bottom side
		else if ((position.y - radius) <= bottomPosition.y + (WALL_WIDTH / 2) && direction.y < 0) {
			spawnObs();
			reverseVericalDirection();
		}
		// check if Obstacle hits the top side
		else if ((position.y + radius) >= topPosition.y - (WALL_WIDTH/2) && direction.y > 0) {
			reverseVericalDirection();
		}
		else {
				direction.y -= 0.0015;
		}
	} else {
		// check if Obstacle hits the left side
		if ((position.x - radius) <= leftPosition.x + (WALL_WIDTH/2)) {
			spawnObs();
		}
		// check if Obstacle hits the right side
		else if ((position.x + radius) >= rightPosition.x - (WALL_WIDTH/2)) {
			spawnObs();
		}
	}

    hitShip();
}

void Obstacle::spawnObs() {
	int newPos;
	if (Game::level == 1) {
		nodePtr = sceneManagerPtr->getSceneNode("Ball");
		direction = Vector3(-0.5,1,0);
		newPos = rand() % 200 + 50;
		nodePtr->setPosition(newPos,-100,0);
	} else {
		nodePtr = sceneManagerPtr->getSceneNode("Cube");
		direction = Vector3(-0.5,-1,0);
		newPos = rand() % 100 + 1;
		nodePtr->setPosition(newPos,100,0);
		int neg = rand() % 2 + 1;
		if (neg == 1)
			nodePtr->setPosition(newPos,100,0);
		else
			nodePtr->setPosition(-newPos,100,0);
	}
}

void Obstacle::reverseVericalDirection() {
	direction *= Vector3(1,-1,1);
}

void Obstacle::hitShip() {
	Vector3 shipPosition = sceneManagerPtr->getSceneNode("Ship")->getPosition();
	Vector3 obstaclePosition = nodePtr->getPosition();

	const int PADDLE_WIDTH = 3;
	const int PADDLE_HEIGHT = 2;

	if ((obstaclePosition.x - radius) < (shipPosition.x + (PADDLE_WIDTH / 2)) &&
		(obstaclePosition.x + radius) > (shipPosition.x - (PADDLE_WIDTH / 2)) &&
		(obstaclePosition.y - radius) < (shipPosition.y + (PADDLE_HEIGHT / 2)) && 
		(obstaclePosition.y + radius) > (shipPosition.y - (PADDLE_HEIGHT / 2)) ) {
		
		Game::gameOver();
	}

}

void Obstacle::reset() {
	nodePtr = sceneManagerPtr->getSceneNode("Ball");
	nodePtr->setPosition(0,-100,0);
	nodePtr = sceneManagerPtr->getSceneNode("Cube");
	nodePtr->setPosition(0,150,0);
	if (Game::level == 1)
		radius = 13;
	else
		radius = 18;
	spawnObs();
}