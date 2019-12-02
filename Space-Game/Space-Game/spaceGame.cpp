#include "createWorld.h"

bool gameOver = false;

int main()
{
	//Create a new GameWorld object
	GameWorld* world = new GameWorld();

	//Initialize the level, timer for how often enemies move, and the start direction 
	int level = 1;
	int moveEnemyTimer = 5;
	int timer = 0;
	enemDir enemDirection = RIGHT;
	world->createBoard(to_string(level));
	world->levelBeginMessage(to_string(level));
		
	//While the player is alive and hasn't completed the level nor tried to exit
	while (world->getLives() > 0 && !world->isExit())
	{
		world->updateBoard();

		//Move the player
		world->movePlayer();
		//Move the enemy if the timer has reached the enemy timer
		if (timer == moveEnemyTimer)
		{
			world->moveEnemy(enemDirection);
			timer = 0;
		}
		//Give a chance for the enemy to shoot
		world->enemyShoot();
		//Move all current bullets
		world->moveBullet();
		//Check if the level is complete
		world->checkIfComplete();

		//Update the game board if viable
		if (world->isHit()) 
		{
			world->hitMessage();
			if (world->isAlive()) 
			{
				world->createBoard(to_string(level));
				enemDirection = RIGHT;
				timer = 0;
			}
		}

		//If the player has beat the level, update the level
		else if (world->isLevelComplete())
		{
			world->levelCompleteMessage(to_string(level));
			level++;		
			world->levelBeginMessage(to_string(level));
			world->createBoard(to_string(level));
			enemDirection = RIGHT;
			timer = 0;
		}

		timer++;
	}

	return 1;
}

