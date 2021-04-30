#include <iostream>
#include "Game.h"


int main() {

	//init srand (random number)
	std::srand(static_cast<unsigned>(time(NULL)));

	//init game engine
	Game game;
	

	//Game loop
	while (game.running()) {
		if (game.getGameState()==0) {
			game.start();
		}
		else if (game.getGameState() == 1) {
			//Update
			game.update();
			
			//Render
			game.render();
			
			
		}
		else if (game.getGameState() == 2) {
		
		}

	}
	
	//end of application
	return 0;
}