#include "Game.h"
#include "User.h"
#include "Packet.h"
#include <memory>
#include <iostream>
Game::Game(User* black, User* white)
{
	for (int i = 0; i < 19; i++)
	{
		std::memset(Omok[i], STONE::NONE, sizeof(STONE) * 19);
	}
	this->black = black;
	this->white = white;
}

Game::~Game()
{

}

PktPutALGameRoomNtf Game::PutAL(int x, int y, std::string userID)
{
	PktPutALGameRoomNtf ntfPkt;
	
	ntfPkt.color = STONE::NONE;
	if (Omok[y][x] != STONE::NONE)
	{
		ntfPkt.color = -1;
		return ntfPkt;
	}
	if (turn == false && userID == black->GetID()) // °ËÀºµ¹
	{
		ntfPkt.color = Omok[y][x] = STONE::BLACK;
		turn = true;
	}
	else if (turn == true && userID == white->GetID())
	{
		ntfPkt.color = Omok[y][x] = STONE::WHITE;
		turn = false;
	}

	ntfPkt.XPos = x;
	ntfPkt.YPos = y;

	return ntfPkt;
}

bool Game::CheckOmok(int x, int y)
{
	int count = 1;

	for (int i = x + 1; i < 19; i++)
	{
		if (Omok[y][x] == Omok[y][i])
			count += 1;
		else
			break;
	}
	for (int i = x - 1; i >= 0; i--)
	{
		if (Omok[y][x] == Omok[y][i])
			count += 1;
		else
			break;
	}

	if (count == 5)
		return true;

	count = 1;

	for (int i = y + 1; i < 19; i++)
	{
		if (Omok[y][x] == Omok[i][x])
			count += 1;
		else
			break;
	}
	
	for (int i = y - 1; i >= 0; i--)
	{
		if (Omok[y][x] == Omok[i][x])
			count += 1;
		else
			break;
	}

	if (count == 5)
		return true;

	count = 1;

	for (int i = x + 1, j = y - 1; i <= 18 && j >= 0; i++, j--)
	{
		if (Omok[y][x] == Omok[j][i])
			count += 1;
		else
			break;
	}
	for (int i = x - 1, j = y + 1; i >= 0 && j <= 18; i--, j++)
	{
		if (Omok[y][x] == Omok[j][i])
			count += 1;
		else
			break;
	}

	if (count == 5)
		return true;

	count = 1;

	for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--)
	{
		if (Omok[y][x] == Omok[j][i])
			count += 1;
		else
			break;
	}

	for (int i = x + 1, j = y + 1; i <= 18 && j <= 18; i++, j++)
	{
		if (Omok[y][x] == Omok[j][i])
			count += 1;
		else
			break;
	}
	
	if (count == 5)
		return true;

	return false;
}