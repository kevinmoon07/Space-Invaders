#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#include "characters.h"

#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>

#include <cstdlib>
#include <random>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

enum enemDir
{
	LEFT,
	RIGHT
};

class GameWorld
{
public:
	GameWorld();

	void levelBeginMessage(string level);
	void hitMessage();
	void levelCompleteMessage(string level);
	void createBoard(string fileName);
	void updateBoard();

	void movePlayer();
	void moveEnemy(enemDir& enemyDirection);
	void moveEnemiesLeft();
	void moveEnemiesRight();
	void enemyShoot();
	void moveBullet();
	bool checkCollision(Bullet* bullet, int objectRow, int objectCol);
	
	bool isHit();
	bool isExit();
	int getLives();
	bool isAlive();
	void checkIfComplete();
	bool isLevelComplete();
	void cleanUp();

private:
	int m_points;
	int m_lives;
	int m_cols;
	int m_rows;
	int m_level;
	bool m_alive;
	bool m_hit; // checks to see if the player has been hit
	bool m_levelComplete;
	bool m_exit;
	
	vector<vector<char>> m_board;

	Player* m_player;
	vector<BasicEnemy*> m_Benemies;
	vector<ShootingEnemy*> m_Senemies;
	vector<Bullet*> m_Bullets;
};


#endif