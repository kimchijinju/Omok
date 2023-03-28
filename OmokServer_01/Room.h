#pragma once

#include <vector>
#include <string>
#include <memory>

#include "User.h"
#include "Game.h"

namespace NServerNetLib { class ITcpNetwork; }
namespace NServerNetLib { class ILog; }
	
using TcpNet = NServerNetLib::ITcpNetwork;
using ILog = NServerNetLib::ILog;

class Room
{
public:
	Room();
	virtual ~Room();

	void Init(const short index, const short maxUserCount);
	
	void SetNetwork(TcpNet* pNetwork, ILog* pLogger);

	void Clear();
		
	void EnterUser(User* user);

	void ExitUser(User* user);

	void NotifyEnterUser(User* user);

	void NotifyChat(char* msg, std::string UserID, int userSessionIndex);

	void NotifyLeaveUser(User* user);
	
	void GetRoomUserList(User* user);

	void SendNotify(short packetID, unsigned long long packetSize, char* ntfPkt);

	short GetIndex() { return m_Index; }

	User* GetUser(int index);

	bool FullRoom();

	bool IsPlayingGame() { return m_PlayingGame; }
		
	short MaxUserCount() { return m_MaxUserCount; }

	short GetUserCount() { return (short)m_UserList.size(); }

	bool AllUserReady();

	Game* GetGame() { return m_pGame; }

	void GameStart(User* black, User* white);
		
private:
	ILog* m_pRefLogger;
	TcpNet* m_pRefNetwork;

	short m_Index = -1;
	short m_MaxUserCount;
		
	bool m_PlayingGame = false;
	std::vector<User*> m_UserList;

	Game* m_pGame = nullptr;
};
