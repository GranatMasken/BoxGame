#pragma once

#include<fstream>
#include<iostream>
#include<vector>
#include<ctime>
#include<string>



#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


/*

	Class that acts as the game engine
	Wrapper class.

*/


class Game
{
private:

	//Variables

	//window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Game logic
	int gamestate;
	bool gameover;
	int points;
	int lives;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	float speed;
	int highscore;
	std::string highscoreString;

	//Assets
	//font
	sf::Font font;
	//text
	sf::Text textPoints;
	sf::Text textLives;
	sf::Text textStart;
	sf::Text textHighscore;
	//music
	sf::Music music0;
	sf::Music music1;



	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	//private functions
	void initVariables();
	void initWindow();
	void initEnemies();

	//Highscore
	void loadHighscore();
	void saveHighscore();

public:
	//constructors / destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	void gameOver();
	int getGameState();


	//functions
	void spawnEnemy();

	//player input
	void pollEvents();
	void updateMousePositions();

	//update functions
	void updateEnemies();
	void update();

	//render functions
	void renderEnemies();
	void render(); 
	void renderText();

	//Other
	void addPoints();
	void changeLife(int x);
	void start();

};

