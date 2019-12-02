#include "characters.h"

Bullet::Bullet(int row, int col, bool playerBullet)
{
	m_row = row;
	m_col = col;
	m_playerBullet = playerBullet;
}

int Bullet::getRow()
{
	return m_row;
}

int Bullet::getCol()
{
	return m_col;
}

bool Bullet::isPlayerBullet()
{
	return m_playerBullet;
}

void Bullet::moveBullet(int row)
{
	m_row = row;
}

Player::Player(int row, int col)
{
	m_row = row;
	m_col = col;
}

int Player::getRow()
{
	return m_row;
}

int Player::getCol()
{
 	return m_col;
}

void Player::moveLeft()
{
	m_col--;
}

void Player::moveRight()
{
	m_col++;
}

BasicEnemy::BasicEnemy(int row, int col)
{
	m_row = row;
	m_col = col;
}

int BasicEnemy::getRow()
{
	return m_row;
}

int BasicEnemy::getCol()
{
	return m_col;
}

void BasicEnemy::setCol(int col)
{
	m_col = col;
}

ShootingEnemy::ShootingEnemy(int row, int col)
{
	m_row = row;
	m_col = col;
}

int ShootingEnemy::getRow()
{
	return m_row;
}

int ShootingEnemy::getCol()
{
	return m_col;
}

void ShootingEnemy::setCol(int col)
{
	m_col = col;
}
