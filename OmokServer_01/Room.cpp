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
	m_IsUsed = false;
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

	strcpy_s(ntfPkt.UserID, user->GetID().c_str());
	ntfPkt.UserIDSize = (unsigned char)user->GetID().size();

	for (User* roomUser : m_UserList)
	{
		int userSession = roomUser->GetSessionIndex();
		m_pRefNetwork->SendData(userSession, (short)PACKET_ID::PK_NEW_USER_ENTER_ROOM_NTF, sizeof(ntfPkt), (char*)&ntfPkt);
	}
}
#include <iostream>
void Room::NotifyChat(wchar_t* msg, std::string UserID)
{
	PktChatRoomNtf ntfPkt;

	strcpy(ntfPkt.UserID, UserID.c_str());
	ntfPkt.UserIDSize = strlen(ntfPkt.UserID);

	wcscpy_s(ntfPkt.Msg, msg);
	ntfPkt.MsgSize = wcslen(ntfPkt.Msg);
	std::cout << ntfPkt.Msg << std::endl;
	for (User* roomUser : m_UserList)
	{
		int userSession = roomUser->GetSessionIndex();
		m_pRefNetwork->SendData(userSession, (short)PACKET_ID::PK_CHAT_ROOM_NTF, sizeof(ntfPkt), (char*)&ntfPkt);
	}
}

void Room::NotifyLeaveUser(User* user)
{
	PktUserLeaveRoomNtf ntfPkt;

	strcpy_s(ntfPkt.UserID, user->GetID().c_str());
	ntfPkt.UserIDSize = user->GetID().size();

	for (User* roomUser : m_UserList)
	{
		m_pRefNetwork->SendData(roomUser->GetSessionIndex(), (short)PACKET_ID::PK_USER_LEAVE_ROOM_NTF, sizeof(ntfPkt), (char*)&ntfPkt);
	}
}
User* Room::GetUser(int index)
{
	return m_UserList[index];
}