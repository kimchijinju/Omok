#pragma once
#include "User.h"
#include "Packet.h"
#include <memory>

class Game
{
public :
	Game(User* first, User* second);
	virtual ~Game();
	PktPutALGameRoomNtf PutAL(int x, int y, std::string userID);
	bool CheckOmok(int x, int y);
	
private :
	enum STONE : short { NONE, BLACK, WHITE};
	STONE Omok[19][19];
	bool turn = 0;  // 0 = ∞À¿∫ µπ, 1 = »Úµπ
	User* black;
	User* white;
};