// Game.cpp
// Game class member-function definitions
// Main Drive/Engine/Manager for the game loop

#include <sstream>
#include <stdexcept>

#include <OgreStringConverter.h>
#include <OgreTimer.h>
#include <OgreFontManager.h>

#include "Game.h"
#include "Ship.h"
#include "Obstacle.h"
#include "Wall.h"
#include "Coin.h"

using namespace std;
using namespace Ogre;

//Timer
Timer gameTime = Timer::Timer();

int Game::player1Score = 0;
int Game::player2Score = 0;
bool Game::wait = false;
bool Game::pause = false;
bool Game::reset = false;
int Game::screenHeight;
int Game::screenWidth;
int Game::gTime = 0;
int Game::level = 1;
Vector3 Game::shipMove = Vector3(0,0,0);

// directions to move the ship
const Vector3 DOWN = Vector3(0,-100,0);
const Vector3 UP = Vector3(0,100,0);
const Vector3 LEFT = Vector3(-100,0,0);
const Vector3 RIGHT = Vector3(100,0,0);

Game::Game() {
	rootPtr = new Root();

	if (!(rootPtr->showConfigDialog()))
		throw runtime_error("User Cancelled Ogre Setup Dialog Box.");

	//get pointer to Render Window
	windowPtr = rootPtr->initialise(true, "Game" );

	// create SceneManger
	sceneManagerPtr = rootPtr->createSceneManager(ST_GENERIC);

	// create Camera
	cameraPtr = sceneManagerPtr->createCamera("GameCamera");
	cameraPtr->setPosition(Vector3(0,0,200));
	cameraPtr->lookAt(Vector3(0,0,0));
	cameraPtr->setNearClipDistance(5);
	cameraPtr->setFarClipDistance(1000);

	// create the ViewPort
	viewportPtr = windowPtr->addViewport(cameraPtr);
	viewportPtr->setBackgroundColour(ColourValue(0.55,0.14,0.14));
	screenWidth = viewportPtr->getActualWidth();
	screenHeight = viewportPtr->getActualHeight();

	// set camera aspect ratio
	cameraPtr->setAspectRatio(Real(viewportPtr->getActualWidth()) / (viewportPtr->getActualHeight()));

	// set ambient light
	sceneManagerPtr->setAmbientLight(ColourValue(0.75, 0.75, 0.75));
	// set Light
	Light *lightPtr = sceneManagerPtr->createLight("Light");
	lightPtr->setPosition(0,0,50);

	unsigned long hWnd; // window handler
	windowPtr->getCustomAttribute("WINDOW", &hWnd);
	OIS::ParamList paramList;

	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	// create InputManger
	inputManagerPtr = OIS::InputManager::createInputSystem(paramList);
	keyboardPtr = static_cast < OIS::Keyboard* > (inputManagerPtr->createInputObject(OIS::OISKeyboard, true));
	keyboardPtr->setEventCallback(this);

	rootPtr->addFrameListener(this);

	// load resources
	ResourceGroupManager::getSingleton().addResourceLocation("../../resources", "FileSystem");
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// load fonts explicitly after resources are loaded (esp. SdkTray.zip) since they will not be loaded on start up from the resource manager
	ResourceManager::ResourceMapIterator iter = FontManager::getSingleton().getResourceIterator();
	while (iter.hasMoreElements()) { iter.getNext()->load(); }

	quit = false;
	time = 0;
}

Game::~Game() {
	// free dynamically allocated memory for keyboard
	inputManagerPtr->destroyInputObject(keyboardPtr);
	OIS::InputManager::destroyInputSystem(inputManagerPtr);

	delete rootPtr;
	rootPtr = 0;

	delete obsPtr;
	obsPtr = 0;

	delete shipPtr;
	shipPtr = 0;

	delete wallPtr;
	wallPtr = 0;
}

void Game::createScene() {
	if (level == 1) {
		// Start Screen
		Overlay *gameStartOverlayPtr = OverlayManager::getSingleton().getByName("GameStartOverlay");
		gameStartOverlayPtr->show();

		Overlay *scoreOverlayPtr = OverlayManager::getSingleton().getByName("Score");
		scoreOverlayPtr->show();

		// make game objects
		obsPtr = new Obstacle(sceneManagerPtr);
		obsPtr->addToScene();
		shipPtr = new Ship(sceneManagerPtr, "Ship", -90);
		shipPtr->addToScene();
		wallPtr = new Wall(sceneManagerPtr);
		wallPtr->addToScene();
		coinPtr = new Coin(sceneManagerPtr);
		coinPtr->addToScene();
	}
	pause = true;
}

void Game::run() {
	createScene();
	rootPtr->startRendering();
}

void Game::gameOver () {
	pause = true;
	Overlay *gameStartOverlayPtr = OverlayManager::getSingleton().getByName("GameOverOverlay");
	gameStartOverlayPtr->show();
	reset = true;
}

void Game::resetGame() {
	level = 1;
	obsPtr->reset();
	shipPtr->reset();
	coinPtr->reset();
	player1Score = 0;
	gTime = 0;
	gameTime.reset();
	viewportPtr->setBackgroundColour(ColourValue(0.55,0.14,0.14));
	Overlay *gameStartOverlayPtr = OverlayManager::getSingleton().getByName("GameOverOverlay");
	gameStartOverlayPtr->hide();
}

void Game::nextLevel() {
	level = 2;
	obsPtr->reset();
	shipPtr->reset();
	coinPtr->reset();
	gTime = 0;
	gameTime.reset();
	pause = true;
	Overlay *nextLevelPtr = OverlayManager::getSingleton().getByName("Level2Overlay");
	nextLevelPtr->show();
	viewportPtr->setBackgroundColour(ColourValue(0.75,0.75,0.93));
}

void Game::updateTimer() {
	int temp = gameTime.getMilliseconds() / 1000;
	if (pause) {
		temp = 0;
		gameTime.reset();
	}
	if (temp >= 1) {
		gTime ++;
		temp = 0;
		gameTime.reset();
	}

	// Time limit reached so switch levels
	if (gTime >= CHANGE_TIME && level == 2) {
		gameOver();
	}
	else if (gTime >= CHANGE_TIME) {
		nextLevel();
	}
}

void Game::updateScore(Players player) {
	player1Score++;
	updateScoreText();
}

// update score text
void Game::updateScoreText() {
	ostringstream scoreText;
//	updateTimer();
	scoreText << "Time: " << gTime;

	// get the right player's TextArea
	OverlayElement *textElementPtr = OverlayManager::getSingletonPtr()->getOverlayElement("right");
	textElementPtr->setCaption(scoreText.str());

	scoreText.str("");
	scoreText << "Score: " << player1Score;

	// get the left player's TextArea
	textElementPtr = OverlayManager::getSingletonPtr()->getOverlayElement("left");
	textElementPtr->setCaption(scoreText.str());
}

// respond to the user keyboard input
bool Game::keyPressed(const OIS::KeyEvent &keyEventRef) {
	// if the game is not paused
	if (!pause) {
		switch (keyEventRef.key) {
			case OIS::KC_ESCAPE:
				quit = true;
				break;
			case OIS::KC_W:
				shipMove = UP;
				break;
			case OIS::KC_S:
				shipMove = DOWN;
				break;
			case OIS::KC_A:
				shipMove = LEFT;
				break;
			case OIS::KC_D:
				shipMove = RIGHT;
				break;
			case OIS::KC_P:
				pause = true;
				Overlay *pauseOverlayPtr = OverlayManager::getSingleton().getByName("PauseOverlay");
				pauseOverlayPtr->show();
				break;
		}
	} else if (reset) {
		if (keyEventRef.key == OIS::KC_RETURN) {
			resetGame();
			pause = false;
			reset = false;
			Overlay *nextLevelPtr = OverlayManager::getSingleton().getByName("Level2Overlay");
			nextLevelPtr->hide();
		}
	} else { // game is paused
		if (keyEventRef.key == OIS::KC_RETURN) {
			pause = false;
			Overlay *pauseOverlayPtr = OverlayManager::getSingleton().getByName("PauseOverlay");
			pauseOverlayPtr->hide();
			Overlay *gameStartOverlayPtr = OverlayManager::getSingleton().getByName("GameStartOverlay");
			gameStartOverlayPtr->hide();
			Overlay *nextLevelPtr = OverlayManager::getSingleton().getByName("Level2Overlay");
			nextLevelPtr->hide();
		}
	}
	return true;
}

bool Game::keyReleased(const OIS::KeyEvent &keyEventRef) {
	shipMove = Vector3(0,0,0);
	return true;
}

bool Game::frameEnded(const FrameEvent &frameEvent) {
	return !quit;
}

// process game logic, return true if the next frame should be rendered
bool Game::frameStarted(const FrameEvent &frameEvent) {
	keyboardPtr->capture();
	// if game is not paused then obstacle moves
	if (!wait && !pause) {
		shipPtr->moveShip(shipMove, frameEvent.timeSinceLastFrame);
		obsPtr->moveObs(frameEvent.timeSinceLastFrame);
		wallPtr->moveWall(frameEvent.timeSinceLastFrame);
		coinPtr->moveCoin(frameEvent.timeSinceLastFrame);
		updateScoreText();
		updateTimer();
	}
	else if (wait) {
		if (time < 4)
			time += frameEvent.timeSinceLastFrame;
	}
	else {
		wait = false;
		time = 0;
	}

	return !quit;
}