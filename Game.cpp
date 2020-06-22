#include "Game.h"

//Private functions
void Game::initializeVariables()
{
	this->window = nullptr;

	//Game logic
	this->points = 0;
	this->health = 20;
	this->endGame = false;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
}

void Game::initializeWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new RenderWindow(this->videoMode, "Tile Destroyer", Style::Titlebar | Style::Close);

	this->window->setFramerateLimit(144);
}

void Game::initializeEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(Vector2f(100.f, 100.f));
	this->enemy.setScale(Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(Color::Green);
}

void Game::initializeFonts()
{
	if (!this->font.loadFromFile("Fonts/Retro Gaming.ttf"))
	{
		cout << "ERROR: Failed to load font!" << endl;
	}
}

void Game::initializeText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(Color::White);
	this->uiText.setString("none");

	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(60);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setString("Game Over!");
	this->gameOverText.setPosition(
		this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f);
}

//Constructor 
Game::Game()
{
	this->initializeVariables();
	this->initializeWindow();
	this->initializeFonts();
	this->initializeText();
	this->initializeEnemies();
}

//Destructor
Game::~Game()
{
	delete this->window;
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

//Functions
void Game::spawnEnemy()
{
	/*
	   @return void

	   Spawns enemies and sets their type and colors. Spawns positions randomly.
		- Set random type (diff)
		- Sets a random position
		- Sets a random color
		- Adds enemy to the vector
	*/

	this->enemy.setPosition(
		static_cast<float> (rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	//Ranomize enemy type
	int type = rand() % 6;

	switch (type)
	{
	case 0:
		this->enemy.setSize(Vector2f(20.f, 20.f));
		this->enemy.setFillColor(Color::Red);
		break;

	case 1:
		this->enemy.setSize(Vector2f(40.f, 40.f));
		this->enemy.setFillColor(Color::Blue);
		break;

	case 2:
		this->enemy.setSize(Vector2f(60.f, 60.f));
		this->enemy.setFillColor(Color::Cyan);
		break;

	case 3:
		this->enemy.setSize(Vector2f(80.f, 80.f));
		this->enemy.setFillColor(Color::Magenta);
		break;

	case 4:
		this->enemy.setSize(Vector2f(100.f, 100.f));
		this->enemy.setFillColor(Color::Green);
		break;

	default:
		this->enemy.setSize(Vector2f(120.f, 120.f));
		this->enemy.setFillColor(Color::Yellow);
		break;
	}

	//Spawn the enemy
	this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case Event::Closed:
			this->window->close();
			break;

		case Event::KeyPressed:
			if (this->event.key.code == Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePositions()
{
	/*
	@return void

	Update the mouse positions
		- Mouse position relative to window(Vector2i)
	*/

	this->mousePosWindow = Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
	/*
		@return void

		Update the enemy spawn timer and spawn enemies
		when the total amount of enemies is smaller than the max.
		Moves the enemies down the screen
		Removes the enemies at the edge of the screen
	*/

	//Updates the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			//Spawn the enemy and reset the timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			//Increases enemy spawn timer
			this->enemySpawnTimer += 1.f;
	}

	//Moving and updating enemies
	for (int i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].move(0.f, 1.f);

		//If the enemy is past the bottom of the screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;

			cout << "Health: " << this->health << endl;
		}
	}


	//Check if clicked upon
	if (Mouse::isButtonPressed(Mouse::Left))
	{

		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;

			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					//Plays sound when enemy is clicked
					if (this->buffer.loadFromFile("Sounds/Explosion.wav"))
					{
						this->sound.setBuffer(this->buffer);
						this->sound.play();
					}
					else
						cout << "ERROR: Failed to load sound file!" << endl;

					//Gain points
					if (this->enemies[i].getFillColor() == Color::Red)
						this->points += 10;

					else if (this->enemies[i].getFillColor() == Color::Blue)
						this->points += 8;

					else if (this->enemies[i].getFillColor() == Color::Cyan)
						this->points += 6;

					else if (this->enemies[i].getFillColor() == Color::Magenta)
						this->points += 4;

					else if (this->enemies[i].getFillColor() == Color::Green)
						this->points += 2;
					else
						this->points += 1;

					cout << "Points: " << this->points << endl;

					//Delete the enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		this->mouseHeld = false;
	}
}

void Game::updateText()
{
	//Displays updated points and health
	stringstream ss;

	ss << "Points: " << this->points;
	ss << "\n" << "Health: " << this->health;

	this->uiText.setString(ss.str());
}

void Game::renderText(RenderTarget& target)
{
	//Renders text to screen
	target.draw(this->uiText);
}

void Game::renderEnemies(RenderTarget& target)
{
	//Renders enemies to screen
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::update()
{
	this->pollEvents();

	if (!this->endGame)
	{
		this->updateMousePositions();

		this->updateText();

		this->updateEnemies();
	}

	//Determines when game is over
	if (this->health <= 0)
		this->endGame = true;
}

void Game::render()
{
	/*
		@Return void

		- Clear old frame
		- Render Objects
		- Display frame in window
		Renders the game objects
	*/

	this->window->clear();

	//Draw game objects
	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	if (this->health <= 0)
		this->window->draw(this->gameOverText);

	this->window->display();
}
