// Ship.cpp

#include "Ship.h"
using namespace Ogre;

Ship::Ship(SceneManager *ptr, String shipName, int positionX) {
	sceneManagerPtr = ptr;
	name = shipName;
	x = positionX;
}

Ship::~Ship() {}

void Ship::addToScene() {
	Entity *entityPtr = sceneManagerPtr->createEntity(name, "cube.mesh");
	entityPtr->setMaterialName("Ship");
	nodePtr = sceneManagerPtr->getRootSceneNode()->createChildSceneNode(name);
	nodePtr->attachObject(entityPtr);
	nodePtr->setScale(0.2,0.1,0.1);
	nodePtr->setPosition(x,0,0);
}

void Ship::moveShip(const Vector3 &direction, Real time) {
	nodePtr->translate((direction * time));
	if (nodePtr->getPosition().y > 80.0)
		nodePtr->setPosition(nodePtr->getPosition().x,80.0,0);
	else if (nodePtr->getPosition().y < -80.0)
		nodePtr->setPosition(nodePtr->getPosition().x,-80.0,0);
	else if (nodePtr->getPosition().x > 140.0)
		nodePtr->setPosition(140.0,nodePtr->getPosition().y,0);
	else if (nodePtr->getPosition().x < -140.0)
		nodePtr->setPosition(-140.0,nodePtr->getPosition().y,0);
}

void Ship::reset() {
	nodePtr->setPosition(x,0,0);
}