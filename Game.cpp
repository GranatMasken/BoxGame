#include "Game.h"
//private functions
void Game::initVariables()
{
	//Points
	this->gameover = false;
	this->points = 0;

	//Lives
	this->lives = 3;

	//wierd ass pointer shit no entiendo
	this->window = nullptr;

	//Game logic
	this->gamestate = 0;
	this->points = 0;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10;
	this->speed = 2;

	//Font
	if (!this->font.loadFromFile("Assets/Fonts/KarmaFuture.ttf")) {
		std::cout << "Error loading font" << std::endl;
		system("pause");
	}
	//text
	this->textPoints.setFont(font);
	this->textPoints.setString("Points: " + std::to_string(points) );
	this->textPoints.setCharacterSize(24);
	this->textPoints.setFillColor(sf::Color::White);
	//text that display lives
	this->textLives.setFont(font);
	this->textLives.setString("Life: " + std::to_string(lives));
	this->textLives.setCharacterSize(24);
	this->textLives.setFillColor(sf::Color::White);
	//text that displays start button
	this->textStart.setFont(font);
	this->textStart.setString("START");
	this->textStart.setCharacterSize(75);
	this->textStart.setFillColor(sf::Color::White);
	this->textStart.setPosition(400 - this->textStart.getGlobalBounds().width / 2, 100);
	//text that displays highscore
	this->textHighscore.setFont(font);
	this->textHighscore.setString("Highscore: " + highscoreString);
	this->textHighscore.setCharacterSize(24);
	this->textHighscore.setFillColor(sf::Color::White);

	//init music
	if (!music0.openFromFile("Assets/Music/GameOver.ogg")) {
		std::cout << "Error loading music" << std::endl;
		system("pause");
	}
	if (!music1.openFromFile("Assets/Music/IntergalacticOdyssey.ogg")) {
		std::cout << "Error loading music" << std::endl;
		system("pause");
	}
	music0.setVolume(5.f);
	music1.setVolume(1.f);
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Gem", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(50.f, 50.f));
	this->enemy.setFillColor(sf::Color::Cyan);
	this->enemy.setOutlineColor(sf::Color::Black);
	this->enemy.setOutlineThickness(2.f);


}

void Game::loadHighscore()
{
	std::ifstream highscoreFile("Assets/Saves/highscore.txt");
	while (std::getline(highscoreFile, highscoreString)) {
		highscore = std::stoi(highscoreString);
		std::cout << std::to_string(highscore);
	}
}

void Game::saveHighscore()
{
	if (points > highscore) {

		std::ofstream highscoreFile("Assets/Saves/highscore.txt");
		highscoreFile << std::to_string(points);
		highscoreFile.close();
		
	}
}


//constructors
Game::Game() {
	this->loadHighscore();
	this->initVariables();
	this->initWindow();
	this->initEnemies();
	
}
Game::~Game() {
	delete this->window;
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

void Game::gameOver()
{
	music1.setLoop(false);
	music1.stop();
	music0.play();
	music0.setLoop(true);
}

int Game::getGameState()
{
	return this->gamestate;
}



//functions

void Game::spawnEnemy()
{
	/*
		Spawns enemies and sets their color and position.
		-random pos
		-random color
		-random size;
		-adds enemy to vector
	*/

	//setpos
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);
	//setcolor
	this->enemy.setFillColor(sf::Color(rand()%255,rand()%255,rand()%255,255));
	//setsize
	this->enemy.setSize(sf::Vector2f(rand()% 50 + 25, rand() % 50 + 25));


	//spawn the enemy
	this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->ev)) {

		//Switch case för vilken typ av event
		switch (this->ev.type) {

		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape) {
				this->window->close();
				break;
			}

		}

	}
}

void Game::updateMousePositions()
{
	/*
		void

		updates mouse positions:
			-relative to window
			-Vector2i
	*/
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
	/*
		updates enemy spawn timer and spawns enemies
		when total amount is smaller than max.
		Moves enemies.
		Removes enemies at edge of screen.
	
	*/


	//updating timer for enemy spawning
	if(this->enemies.size() < this->maxEnemies)
		if (this->enemySpawnTimer >= enemySpawnTimerMax) {
			//spawn enemy & reset timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;

	/*
		for loop that cycles through all the enemies and moves each one.
		And checks if any is being clicked upon
		checks if enemies are at bottom
		deletes clicked enemies
	*/
	for (int i = 0; i < this->enemies.size(); i++) {

		bool deleted = false;

		//move enemy
		this->enemies[i].move(0.f, this->speed);

		//Check if clicked upon
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			//check if the enemy touches the mouse
			if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
				deleted = true;

				//Gain points
				this->addPoints();
				
			}
		}
		//check if enemy is at bottom
		if (this->enemies[i].getPosition().y + enemies[i].getSize().y > this->window->getSize().y) {
			deleted = true;
			this->changeLife(-1);
		}
		//final delete
		if(deleted)
			this->enemies.erase(this->enemies.begin() + i);

	}
	

}

void Game::update()
{
	this->pollEvents();

	this->updateMousePositions();

	this->updateEnemies();

}

void Game::renderEnemies()
{
	//rendering all enemies
	for (auto& e : this->enemies) {
		this->window->draw(e);
	}
}

void Game::render()
{
	/*
		Renders the game objects:
		-clear old frame
		-render new objects
		-display new frame
	*/


	//clear previous frame
	this->window->clear(sf::Color(100,100,100,255));
	//draw game objects
	this->renderEnemies();
	this->renderText();

	//finnished drawing
	this->window->display();
}

void Game::renderText()
{
	if (gameover != true) {
		this->textPoints.setString("Points: " + std::to_string(points));
		this->window->draw(textPoints);
		this->textLives.setString("Life: " + std::to_string(lives));
		this->textLives.setPosition(0.f, 25.f);
		this->window->draw(textLives);
	}
	else {
		this->textLives.setString("GAME OVER, score: " + std::to_string(points));
		if (points > highscore) this->textLives.setString("GAME OVER, score: " + std::to_string(points) + " \n New Highscore!!");
		this->textLives.setCharacterSize(50);
		this->textLives.setPosition(this->window->getSize().x / 2 - this-> textLives.getGlobalBounds().width /2, this->window->getSize().y / 2 - this->textLives.getGlobalBounds().height);
		this->window->draw(textLives);

		this->textHighscore.setString("Highscore: " + std::to_string(highscore));
		this->window->draw(textHighscore);
	}
}


//functioner som lägger till poäng och tar bort liv, kollar ifall liven är under  0
void Game::changeLife(int x)
{
	this->lives += x;
	if (lives == 0) {
		saveHighscore();
		gameover = true;
		gameOver();
	}
}

void Game::addPoints()
{
	if(!gameover)
	this->points += 1;

	//ökar speeden
	this->speed += 0.05;

	std::cout << std::to_string(this->speed) << std::endl;
	
}
//start screeen
void Game::start()
{
	music0.play();
	music0.setLoop(true);
	while (gamestate == 0) {
		//poll events for closing
		this->pollEvents();
		this->updateMousePositions();

		//clear previous frame
		this->window->clear(sf::Color(100, 100, 100, 255));
		//draw game objects
		this->window->draw(textStart);
		this->window->draw(textHighscore);

		//finnished drawing
		this->window->display();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (this->textStart.getGlobalBounds().contains(this->mousePosView)) {

				gamestate = 1;

				//changing music
				music0.setLoop(false);
				music0.stop();
				music1.play();
				music1.setLoop(true);

			}
		}

	}
}

