// Game.h
// Game class definition (represents a game of pong).

//#pragma once // fix for Intellisence warnings

#ifndef GAME_H
#define GAME_H

#include <stdlib.h>

#include <Ogre.h> //Ogre class definition
#include <OIS/OISEvents.h> // OISEvents class definition 
#include <OIS/OISInputManager.h> // OISInputManger class definition
#include <OIS/OISKeyboard.h>
using namespace Ogre;

class Obstacle;
class Ship;
class Wall;
class Coin;

enum Players { PLAYER1, PLAYER2 };

const int CHANGE_TIME = 30;

class Game : public FrameListener, public OIS::KeyListener {
	public:
		Game();
		~Game();
		void run();

		bool keyPressed (const OIS::KeyEvent &keyEventRef);
		bool keyReleased (const OIS::KeyEvent &keyEventRef);

		virtual bool frameStarted(const FrameEvent &frameEvent);
		virtual bool frameEnded(const FrameEvent &frameEvent);
		static void updateScore(Players player);
		static void updateScoreText ();
		static void gameOver ();
		void resetGame();
		void nextLevel();

		void updateTimer ();

		static int screenHeight;
		static int screenWidth;

		static int level;
		
		static Vector3 shipMove;

		static bool pause, reset;

	private:
		void createScene ();

		//Ogre Objects
		Root *rootPtr;
		SceneManager *sceneManagerPtr;
		RenderWindow *windowPtr;
		Viewport *viewportPtr;
		Camera *cameraPtr;

		// OIS input objects
		OIS::InputManager *inputManagerPtr;
		OIS::Keyboard *keyboardPtr;

		// Game Objects
		Obstacle *obsPtr;
		Ship *shipPtr;
		Wall *wallPtr;
		Coin *coinPtr;

		// variables to control game states
		bool quit;
		Real time;
		static bool wait;

		static int player1Score;
		static int player2Score;
		static int gTime;
};

#endif


		