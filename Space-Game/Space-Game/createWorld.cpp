#include "createWorld.h"

const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;

GameWorld::GameWorld()
	: m_player(nullptr)
{
	m_level = 0;
	m_points = 0;
	m_lives = 3;
	m_cols = 0;
	m_rows = 0;
	m_alive = true;
	m_levelComplete = false;
	m_exit = false;
	m_hit = false;
}

void GameWorld::levelBeginMessage(string level)
{
	//Open the specified level. If the file for the level does not exist, it is assumed that the player has won.
	ifstream inFile;
	string fileName = "Levels\\Level" + level + ".txt";
	inFile.open(fileName);

	//Level file cannot load so player has won
	if (inFile.fail())
	{
		system("cls");
		cout << "Congratulations on winning!!!" << endl << "You had " << m_points << " points." << endl;
		exit(1);
	}

	system("cls");
	cout << "Prepare for Level " << level << "." << endl;
	cout << "Press 'SPACE' to continue or 'x' to exit." << endl;

	bool continueGame = false;

	while (!continueGame && !m_exit)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case 32:
				continueGame = true;
				break;
			case 'X':
			case 'x':
				m_exit = true;
				break;
			default:
				break;
			}
		}
	}
}

void GameWorld::hitMessage()
{
	system("cls");

	if (m_lives > 0) 
	{
		cout << "Ouch! You've been hit by the enemy." << endl;
		cout << "You have " << m_lives << " lives remaining." << endl;
		cout << "Press 'c' to continue, or 'x' to exit." << endl;

		bool continueGame = false;

		while (!continueGame && !m_exit) 
		{
			if (_kbhit())
			{
				switch (_getch())
				{
				case 'C':
				case 'c':
					//recreates the boardgame
					continueGame = true;
					m_hit = false;
					m_alive = true;
					break;
				case 'x':
				case 'X':
					//exits the game
					m_exit = true;
					break;
				default:
					break;
				}
			}
		}
	}
	else 
	{
		cout << "Ouch! You lost the game chief." << endl;
		cout << "The enemies have won and taken over your planet!!" << endl;
		cout << "Press 'x' to exit the program." << endl;

		while (!m_exit)
		{
			if (_kbhit())
			{
				switch (_getch())
				{
				case 'x':
				case 'X':
					//exits the game
					m_exit = true;
					break;
				default:
					break;
				}
			}
		}
	}
}

void GameWorld::levelCompleteMessage(string level)
//Display this message after completing a level
{
	system("cls");
	cout << "Congratulations, you beat Level " << level << "." << endl;
	cout << "You get another life because you beat the level!!!" << endl;
	cout << "Press 'c' to continue." << endl;

	//Delete the player and reset it
	delete m_player;
	m_player = nullptr;

	m_lives++;
	bool continueGame = false;

	while (!continueGame && !m_exit)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'C':
			case 'c':
				continueGame = true;
				break;
			default:
				break;
			}
		}
	}
}

void GameWorld::createBoard(string level)
//Creates the initial game board
{
	//Open the specified level. If the file for the level does not exist, it is assumed that the player has won.
	ifstream inFile;
	string fileName = "Levels\\Level" + level + ".txt";
	inFile.open(fileName);

	//Level file cannot load so player has won
	if (inFile.fail())
	{
		cerr << "No such file exists";
		exit(1);
	}
	m_level = stoi(level);

	string line;

	//Ensure that all variables of type vector are cleared, the player is set to alive status, and the level is not yet complete
	cleanUp();
	m_alive = true;
	m_levelComplete = false;

	//Parse through the file and enter in the characters into a game board variable
	while (getline(inFile, line))
	{
		vector<char> temp;

		for (int x = 0; x < line.length(); x++)
		{
			temp.push_back(line[x]);
		}

		m_board.push_back(temp);
	}

	//Retrieve the rows and cols for the specified game board
	m_rows = m_board.size();
	m_cols = m_board[0].size();

	//Parse through the game board and create the characters based on specified characters
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			if (m_board[row][col] == 'x')
			{
				BasicEnemy* newEnemy = new BasicEnemy(row, col);
				m_Benemies.push_back(newEnemy);
			}
			else if (m_board[row][col] == 'v')
			{
				ShootingEnemy* newSEnemy = new ShootingEnemy(row, col);
				m_Senemies.push_back(newSEnemy);
			}
			else if (m_board[row][col] == '^')
			{
				m_player = new Player(row, col);
			}
		}
	}
};

void GameWorld::updateBoard()
//Redraws the game board with any modifications to the player and/or enemies
{
	if (m_alive && !m_levelComplete)
	{
		//Clears screen
		system("cls");

		//Clear the board as displayed in the vector
		for (int row = 0; row < m_rows; row++)
		{
			for (int col = 0; col < m_cols; col++)
			{
				if (m_board[row][col] != '#')
				{
					m_board[row][col] = ' ';
				}
			}
		}

		//Update Player position
		m_board[m_player->getRow()][m_player->getCol()] = '^';

		//Update Bullet position before enemy position so that the enemy positions overlay the bullet
		for (int x = 0; x < m_Bullets.size(); x++)
		{
			m_board[m_Bullets[x]->getRow()][m_Bullets[x]->getCol()] = '|';
		}

		//Update Enemy positions
		for (int x = 0; x < m_Benemies.size(); x++)
		{
			m_board[m_Benemies[x]->getRow()][m_Benemies[x]->getCol()] = 'x';
		}

		for (int x = 0; x < m_Senemies.size(); x++)
		{
			m_board[m_Senemies[x]->getRow()][m_Senemies[x]->getCol()] = 'v';
		}

		//Print header
		string header = "Level: " + to_string(m_level) + " Lives: " + to_string(m_lives) + " Points: " + to_string(m_points);
		cout << header << endl;

		//Updates the game board based on the player movement and the patterned enemy movement
		for (int row = 0; row < m_rows; row++)
		{
			for (int col = 0; col < m_cols; col++)
			{
				cout << m_board[row][col];
			}
			cout << endl;
		}
	}
}

void GameWorld::movePlayer()
//Moves the player based on user input
{
	//If a keyboard input is registered, perform a particular action
	if (_kbhit())
	{
		Bullet* playerBullet = new Bullet(m_player->getRow() - 1, m_player->getCol(), true);
		switch (_getch())
		{
		case KEY_LEFT:
		case 'A':
		case 'a':
			//Move the player left
			if (m_player->getCol() > 1)
			{
				m_player->moveLeft();
			}
			break;
		case KEY_RIGHT:
		case 'D':
		case 'd':
			//Move the player right
			if (m_player->getCol() < m_cols - 2)
			{
				m_player->moveRight();
			}
			break;
		case 32:
			//Shoot a bullet
			m_Bullets.push_back(playerBullet);
			break;
		case 'X':
		case 'x':
			//Exit the game
			m_exit = true;
			break;
		default:
			//Prevent memory leak
			delete playerBullet;
			break;
		}
	}
}

void GameWorld::moveEnemy(enemDir& enemyDirection)
//Moves the enemy units as a group
{
	//Determine the minimum and maximum position of the enemies as observing them as a group to determine their possible movement trajectory
	int maxCol = -1;
	int minCol = m_cols;

	for (int x = 0; x < m_Benemies.size(); x++)
	{
		maxCol = max(maxCol, m_Benemies[x]->getCol());
		minCol = min(minCol, m_Benemies[x]->getCol());
	}

	for (int x = 0; x < m_Senemies.size(); x++)
	{
		maxCol = max(maxCol, m_Senemies[x]->getCol());
		minCol = min(minCol, m_Senemies[x]->getCol());
	}

	//Move the enemy units to the right if allowed to do so as defined by the game borders
	if (enemyDirection == RIGHT)
	{
		if (maxCol < m_cols - 2)
		{
			moveEnemiesRight();
		}
		else
		{
			moveEnemiesLeft();
			enemyDirection = LEFT;
		}
	}

	//Move the enemy units to the left if allowed to do so as defined by the game borders
	else
	{
		if (minCol > 1)
		{
			moveEnemiesLeft();
		}
		else
		{
			moveEnemiesRight();
			enemyDirection = RIGHT;
		}
	}
}

void GameWorld::moveEnemiesLeft()
//Move each enemy a column to the left
{
	for (int x = 0; x < m_Benemies.size(); x++)
	{
		m_Benemies[x]->setCol(m_Benemies[x]->getCol() - 1);
	}
	for (int x = 0; x < m_Senemies.size(); x++)
	{
		m_Senemies[x]->setCol(m_Senemies[x]->getCol() - 1);
	}
}

void GameWorld::moveEnemiesRight()
//Move each enemy a column to the right
{
	for (int x = 0; x < m_Benemies.size(); x++)
	{
		m_Benemies[x]->setCol(m_Benemies[x]->getCol() + 1);
	}
	for (int x = 0; x < m_Senemies.size(); x++)
	{
		m_Senemies[x]->setCol(m_Senemies[x]->getCol() + 1);
	}
}

void GameWorld::enemyShoot()
{
	//Generate a distribution of random numbers
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 100);

	//Give each Shooting Enemy a 1% chance to shoot at each iteration
	for (int x = 0; x < m_Senemies.size(); x++)
	{
		if (dis(gen) == 1)
		{
			Bullet* enemyBullet = new Bullet(m_Senemies[x]->getRow() + 1, m_Senemies[x]->getCol(), false);
			m_Bullets.push_back(enemyBullet);
		}
	}
}

void GameWorld::moveBullet()
//Moves the bullet in the proper direction
{
	//If the bullet is from the player, it should move up. Otherwise, it should move down
	int x = 0;

	while (x < m_Bullets.size() && m_alive)
	{
		bool collision = false;

		//Move bullet up if player bullet
		if (m_Bullets[x]->isPlayerBullet())
		{
			m_Bullets[x]->moveBullet(m_Bullets[x]->getRow() - 1);

			//Delete bullet from the vector if it goes out of bounds
			if (m_Bullets[x]->getRow() == 0)
			{
				delete m_Bullets[x];
				m_Bullets.erase(m_Bullets.begin() + x);
			}
			else
			{

				//Determine if a bullet collides with an enemy
				if (!collision)
				{
					for (int y = 0; y < m_Benemies.size(); y++)
					{
						collision = checkCollision(m_Bullets[x], m_Benemies[y]->getRow(), m_Benemies[y]->getCol());

						//If there is a collision, update the point count and delete the particular enemy and bullet from the vectors
						if (collision)
						{
							m_points += 100;
							delete m_Benemies[y];
							m_Benemies.erase(m_Benemies.begin() + y);
							delete m_Bullets[x];
							m_Bullets.erase(m_Bullets.begin() + x);

							break;
						}
					}
				}
				if (!collision)
				{
					for (int y = 0; y < m_Senemies.size(); y++)
					{
						collision = checkCollision(m_Bullets[x], m_Senemies[y]->getRow(), m_Senemies[y]->getCol());

						if (collision)
						{
							m_points += 250;
							delete m_Senemies[y];
							m_Senemies.erase(m_Senemies.begin() + y);
							delete m_Bullets[x];
							m_Bullets.erase(m_Bullets.begin() + x);

							break;
						}
					}
				}

			}
		}

		//Move bullet down if an enemy bullet
		else
		{
			m_Bullets[x]->moveBullet(m_Bullets[x]->getRow() + 1);

			//Delete bullet from the vector if it goes out of bounds
			if (m_Bullets[x]->getRow() == m_rows - 1)
			{
				collision = true;
				delete m_Bullets[x];
				m_Bullets.erase(m_Bullets.begin() + x);
			}

			//Determine if bullet collides with the player
			else
			{
				collision = checkCollision(m_Bullets[x], m_player->getRow(), m_player->getCol());
				if (collision)
				{
					//Set player to dead and clean up the player
					m_alive = false;
					delete m_player;
					m_player = nullptr;
					m_hit = true;
					cleanUp();
					m_lives--;
				}
			}
		}

		if (!collision)
		{
			x++;
		}
	}
}

bool GameWorld::checkCollision(Bullet* bullet, int objectRow, int objectCol)
//Check for a collision if the rows and cols of two objects are the same
{
	if (bullet->getCol() == objectCol && bullet->getRow() == objectRow)
	{
		return true;
	}

	return false;
}

bool GameWorld::isHit()
{
	return m_hit;
}

bool GameWorld::isExit()
//Return the exit status
{
	return m_exit;
}

int GameWorld::getLives()
//Return the number of lives left
{
	return m_lives;
}

bool GameWorld::isAlive()
//Return if the player is alive
{
	return m_alive;
}

void GameWorld::checkIfComplete()
//Determine if the level is complete if there are no more enemies left and the player is still alive
{
	if (m_Benemies.size() == 0 && m_Senemies.size() == 0 && m_alive)
	{
		m_levelComplete = true;
	}
}

bool GameWorld::isLevelComplete()
//Return if the level is complete
{
	return m_levelComplete;
}

void GameWorld::cleanUp()
//Clean up the game board from death
{
	//Properly delete the enemies and bullets and clear their associated vectors
	for (int x = 0; x < m_Benemies.size(); x++)
	{
		delete m_Benemies[x];
	}
	for (int x = 0; x < m_Senemies.size(); x++)
	{
		delete m_Senemies[x];
	}
	for (int x = 0; x < m_Bullets.size(); x++)
	{
		delete m_Bullets[x];
	}

	m_board.clear();
	m_Benemies.clear();
	m_Senemies.clear();
	m_Bullets.clear();
}
