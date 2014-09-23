// Wall.cpp
// Wall class member-function definitions

#include "Wall.h"
#include "Obstacle.h"
#include "Ship.h"
#include "Game.h"
using namespace Ogre;

Wall::Wall(SceneManager *ptr) {
	sceneManagerPtr = ptr;
	speed = 100;
	direction = Vector3(-1,0,0);
}

Wall::~Wall() {}

void Wall::addToScene() {
	Entity *entityPtr;
	if (Game::level == 1) {
		entityPtr = sceneManagerPtr->createEntity("WallBottom1", "cube.mesh");
		entityPtr->setMaterialName("firewall");

		// create the SceneNode for the bottom wall
		nodePtr = sceneManagerPtr->getRootSceneNode()->createChildSceneNode("WallBottom1");
		nodePtr->attachObject(entityPtr);
		nodePtr->setPosition(0,-80,0);
		nodePtr->setScale(3,0.08,0.1);

		entityPtr = sceneManagerPtr->createEntity("WallBottom2", "cube.mesh");
		entityPtr->setMaterialName("firewall");

		// create the SceneNode for the bottom wall
		nodePtr = sceneManagerPtr->getRootSceneNode()->createChildSceneNode("WallBottom2");
		nodePtr->attachObject(entityPtr);
		nodePtr->setPosition(300,-80,0);
		nodePtr->setScale(3,0.3,0.1);
	} else {
		entityPtr = sceneManagerPtr->createEntity("WallTop", "cube.mesh");
		entityPtr->setMaterialName("wall");

		// create the SceneNode for the top wall
		nodePtr = sceneManagerPtr->getRootSceneNode()->createChildSceneNode("WallTop");
		nodePtr->attachObject(entityPtr);
		nodePtr->setPosition(0,80,0);
		nodePtr->setScale(1.95,0.05,0.1);
	}
}

void Wall::moveWall(Real time) {
	nodePtr = sceneManagerPtr->getSceneNode("WallBottom1");
	nodePtr->translate((direction * (speed * time)));
	Vector3 position = nodePtr->getPosition();

	Vector3 leftPosition(-150,0,0);
	Vector3 rightPosition(150,0,0);

	const int WALL_WIDTH = 150;

	// check if wall hits the left side
	if (position.x <= leftPosition.x - WALL_WIDTH) {
		nodePtr->setPosition(150*2,-80,0);
	}

	nodePtr = sceneManagerPtr->getSceneNode("WallBottom2");
	nodePtr->translate((direction * (speed * time)));
	position = nodePtr->getPosition();

	// check if wall hits the left side
	if (position.x <= leftPosition.x - WALL_WIDTH) {
		nodePtr->setPosition(150*2,-80,0);
	}

	checkMaterial();
    hitShip();
}

void Wall::checkMaterial() {
	if (Game::level == 2) {
		Entity *entityPtr = static_cast<Entity*>(sceneManagerPtr->getEntity("WallBottom1"));
		entityPtr->setMaterialName("icecube");
		entityPtr = static_cast<Entity*>(sceneManagerPtr->getEntity("WallBottom2"));
		entityPtr->setMaterialName("icecube");
	} else {
		Entity *entityPtr = static_cast<Entity*>(sceneManagerPtr->getEntity("WallBottom1"));
		entityPtr->setMaterialName("firewall");
		entityPtr = static_cast<Entity*>(sceneManagerPtr->getEntity("WallBottom2"));
		entityPtr->setMaterialName("firewall");
	}
}

void Wall::hitShip() {
	Vector3 shipPosition = sceneManagerPtr->getSceneNode("Ship")->getPosition();
	nodePtr = sceneManagerPtr->getSceneNode("WallBottom1");
	Vector3 wall1Position = nodePtr->getPosition();
	nodePtr = sceneManagerPtr->getSceneNode("WallBottom2");
	Vector3 wall2Position = nodePtr->getPosition();

	const int SHIP_WIDTH = 3;
	const int SHIP_HEIGHT = 2;
	const int WALL_WIDTH = 150;
	const int WALL1_HEIGHT = 2;
	const int WALL2_HEIGHT = 18;

	if ((wall1Position.x - WALL_WIDTH) < (shipPosition.x + (SHIP_WIDTH / 2)) &&
		(wall1Position.x + WALL_WIDTH) > (shipPosition.x - (SHIP_WIDTH / 2)) &&
		(wall1Position.y - WALL1_HEIGHT) < (shipPosition.y + (SHIP_HEIGHT / 2)) && 
		(wall1Position.y + WALL1_HEIGHT) > (shipPosition.y - (SHIP_HEIGHT / 2)) ) {
		
		Game::gameOver();
	}
	else if  ((wall2Position.x - WALL_WIDTH) < (shipPosition.x + (SHIP_WIDTH / 2)) &&
		(wall2Position.x + WALL_WIDTH) > (shipPosition.x - (SHIP_WIDTH / 2)) &&
		(wall2Position.y - WALL2_HEIGHT) < (shipPosition.y + (SHIP_HEIGHT / 2)) && 
		(wall2Position.y + WALL2_HEIGHT) > (shipPosition.y - (SHIP_HEIGHT / 2)) ) {

		Game::gameOver();
	}
}