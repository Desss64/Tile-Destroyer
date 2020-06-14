#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

using namespace sf;
using namespace std;

/*
    Class that acts as the game engine.
*/

class Game
{
private:
	//Private functions
	void initializeVariables();
	void initializeWindow();
	void initializeEnemies();
	void initializeFonts();
	void initializeText();

	//Variables
	//Window
	RenderWindow* window;
	Event event;
	VideoMode videoMode;

	//Mouse positions
	Vector2i mousePosWindow;
	Vector2f mousePosView;

	//Resources
	Font font;

	//Text
	Text uiText;
	Text gameOverText;

	//Sound
	SoundBuffer buffer;
	Sound sound;

	//Game logic
	bool endGame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;

	//Game objects
	vector<RectangleShape> enemies;
	RectangleShape enemy;

public:
	//Constructors
	Game();
	~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	void spawnEnemy();

	void pollEvents();
	void updateMousePositions();
	void updateEnemies();
	void updateText();
	void update();

	void renderText(RenderTarget& target);
	void renderEnemies(RenderTarget& target);
	void render();

};

