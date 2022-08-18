#include <algorithm>
#include <cstring>
#include <vector>
#include <wchar.h>

#include "NetLib/ILog.h"
#include "NetLib/TcpNetwork.h"
#include "Packet.h"
#include "ErrorCode.h"
#include "User.h"
#include "Room.h"

using LOG_TYPE = NServerNetLib::LOG_TYPE;


Room::Room() {}

Room::~Room()
{
}
	
void Room::Init(const short index, const short maxUserCount)
{
	m_Index = index;
	m_MaxUserCount = maxUserCount;
}

void Room::SetNetwork(TcpNet* pNetwork, ILog* pLogger)
{
	m_pRefLogger = pLogger;
	m_pRefNetwork = pNetwork;
}

void Room::Clear()
{
	m_PlayingGame = false;
	m_UserList.clear();
}

void Room::EnterUser(User* user)
{
	m_UserList.push_back(user);
}

void Room::ExitUser(User* user)
{
	m_UserList.erase(std::find(m_UserList.begin(), m_UserList.end(), user));
}

void Room::NotifyEnterUser(User* user)
{
	PktNewUserEnterRoomNtf ntfPkt;

	strcpy_s(ntfPkt.UserID,user->GetID().c_str());

	SendNotify((short)PACKET_ID::PK_NEW_USER_ENTER_ROOM_NTF, sizeof(ntfPkt), (char*)&ntfPkt);
}

void Room::GetRoomUserList(User* user)
{
	PktRoomUserListNtf ntfPkt;

	ntfPkt.UserCount = 0;
	
	if (m_UserList.size() >= 1)
	{
		strcpy_s(ntfPkt.UserID1, m_UserList[0]->GetID().c_str());
		ntfPkt.UserCount += 1;
	}
	
	if (m_UserList.size() >= 2)
	{
		strcpy_s(ntfPkt.UserID2, m_UserList[1]->GetID().c_str());
		ntfPkt.UserCount += 1;
	}
	m_pRefNetwork->SendData(user->GetSessionIndex(), (short)PACKET_ID::PK_ROOM_USER_LIST_NTF, sizeof(ntfPkt), (char*)&ntfPkt);
}

void Room::NotifyChat(char* msg, std::string UserID, int userSessionIndex)
{
	PktChatRoomNtf ntfPkt;

	ntfPkt.IDlen = UserID.size();
	strcpy(ntfPkt.UserID, UserID.c_str());

	ntfPkt.Msglen = strlen(msg);
	strcpy(ntfPkt.Msg, msg);

	m_pRefLogger->Write(LOG_TYPE::L_DEBUG, "msg : %s", ntfPkt.Msg);

	SendNotify((short)PACKET_ID::PK_CHAT_ROOM_NTF, sizeof(ntfPkt), (char*)&ntfPkt);	
}

void Room::NotifyLeaveUser(User* user)
{
	PktUserLeaveRoomNtf ntfPkt;

	strcpy_s(ntfPkt.UserID, user->GetID().c_str());

	SendNotify((short)PACKET_ID::PK_USER_LEAVE_ROOM_NTF, sizeof(ntfPkt), (char *)&ntfPkt);
}

void Room::SendNotify(short packetID, unsigned long long packetSize, char* ntfPkt)
{
	for (User* roomUser : m_UserList)
	{
		int roomUserIndex = roomUser->GetSessionIndex();
		
		m_pRefNetwork->SendData(roomUserIndex, packetID, packetSize, ntfPkt);
	}
}

User* Room::GetUser(int index)
{
	return m_UserList[index];
}

bool Room::AllUserReady()
{
	if (m_UserList.size() != 2)
	{
		return false;
	}

	for (User* user : m_UserList)
	{
		if (!user->IsReady())
			return false;
	}
	return true;
}

void Room::GameStart()
{
	m_PlayingGame = true;
}