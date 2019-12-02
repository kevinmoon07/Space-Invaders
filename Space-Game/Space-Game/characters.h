#ifndef CHARACTERS_H_
#define CHARACTERS_H_

class Bullet
{
public:
	Bullet(int row, int col, bool playerBullet);

	int getRow();
	int getCol();
	bool isPlayerBullet();
	void moveBullet(int row);

private:
	int m_row;
	int m_col;
	bool m_playerBullet;
};

class Player
{
public:
	Player(int row, int col);

	int getRow();
	int getCol();
	void moveRight();
	void moveLeft();

private:
	int m_row;
	int m_col;
	bool m_alive;
};

class BasicEnemy
{
public:
	BasicEnemy(int row, int col);
	
	int getRow();
	int getCol();
	void setCol(int col);

private:
	int m_row;
	int m_col;
};

class ShootingEnemy
{
public:
	ShootingEnemy(int row, int col);

	int getRow();
	int getCol();
	void setCol(int col);

private:
	int m_row;
	int m_col;
};

#endif