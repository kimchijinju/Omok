#include <cstring>

#include "NetLib/ILog.h"
#include "NetLib/TcpNetwork.h"
#include "User.h"
#include "UserManager.h"
#include "Room.h"
#include "RoomManager.h"
#include "PacketProcess.h"

using LOG_TYPE = NServerNetLib::LOG_TYPE;
using ServerConfig = NServerNetLib::ServerConfig;

	
PacketProcess::PacketProcess() {}
PacketProcess::~PacketProcess() {}

void PacketProcess::Init(TcpNet* pNetwork, UserManager* pUserMgr, RoomManager* pLobbyMgr, ServerConfig* pConfig, ILog* pLogger)
{
	m_pRefLogger = pLogger;
	m_pRefNetwork = pNetwork;
	m_pRefUserMgr = pUserMgr;
	m_pRefRoomMgr = pLobbyMgr;
}
	
void PacketProcess::Process(PacketInfo packetInfo)
{
	using netLibPacketId = NServerNetLib::PACKET_ID;
	using commonPacketId = PACKET_ID;

	auto packetId = packetInfo.PacketId;
	switch (packetId)
	{
	case (int)netLibPacketId::NTF_SYS_CONNECT_SESSION:
		NtfSysConnctSession(packetInfo);
		break;
	case (int)netLibPacketId::NTF_SYS_CLOSE_SESSION:
		NtfSysCloseSession(packetInfo);
		break;
	case (int)commonPacketId::PK_LOGIN_IN_REQ:
		Login(packetInfo);
		break;
	case (int)commonPacketId::PK_ENTER_ROOM_REQ:
		EnterRoom(packetInfo);
		break;
	case (int)commonPacketId::PK_LEAVE_ROOM_REQ:
		LeaveRoom(packetInfo);
		break;
	case (int)commonPacketId::PK_CHAT_ROOM_REQ:
		ChatRoom(packetInfo);
		break;
	}
	
}


ERROR_CODE PacketProcess::NtfSysConnctSession(PacketInfo packetInfo)
{
	m_pRefLogger->Write(LOG_TYPE::L_INFO, "%s | NtfSysConnctSession. sessionIndex(%d)", __FUNCTION__, packetInfo.SessionIndex);

	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::NtfSysCloseSession(PacketInfo packetInfo)
{
	auto pUser = std::get<1>(m_pRefUserMgr->GetUser(packetInfo.SessionIndex));

	if (pUser) {		
		m_pRefUserMgr->RemoveUser(packetInfo.SessionIndex);		
	}
			
	m_pRefLogger->Write(LOG_TYPE::L_INFO, "%s | NtfSysCloseSesson. sessionIndex(%d)", __FUNCTION__, packetInfo.SessionIndex);
	return ERROR_CODE::NONE;
}


ERROR_CODE PacketProcess::Login(PacketInfo packetInfo)
{
	// 패스워드는 무조건 pass 해준다.
	// ID 중복이라면 에러 처리한다.
	PktLogInRes resPkt;
	auto reqPkt = (PktLogInReq*)packetInfo.pRefData;

	auto addRet = m_pRefUserMgr->AddUser(packetInfo.SessionIndex, reqPkt->szID);

	if (addRet != ERROR_CODE::NONE) {
		resPkt.SetError(addRet);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_LOGIN_IN_RES, sizeof(resPkt), (char*)&resPkt, addRet);
	}

	if (addRet == ERROR_CODE::USER_IN_ROOM)
	{
		m_pRefLogger->Write(LOG_TYPE::L_INFO, "dup");

	}
		
	resPkt.SetError(addRet);
	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_LOGIN_IN_RES, sizeof(resPkt), (char*)&resPkt);
	return ERROR_CODE::NONE;
}

//TODO: 방 입장
ERROR_CODE PacketProcess::EnterRoom(PacketInfo packetInfo)
{
	PktEnterRoomRes resPkt;	
	auto reqPkt = (PktEnterRoomReq*)packetInfo.pRefData;
	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);
	
	if (errorCode != ERROR_CODE::NONE) 
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::USER_NOT_LOGIN);
	}

	if (!user->IsCurDomainInLogIn())
	{
		resPkt.SetError(ERROR_CODE::USER_NOT_LOGIN);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::USER_NOT_LOGIN);
	}

	if(user->IsCurDomainInRoom())
	{
		resPkt.SetError(ERROR_CODE::USER_IN_ROOM);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::USER_IN_ROOM);
	}
	
	if (reqPkt->RoomNumber >= m_pRefRoomMgr->MaxRoomCount())
	{
		resPkt.SetError(ERROR_CODE::ROOM_INVALID_INDEX);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::ROOM_INVALID_INDEX);
	}

	Room* room;

	if (reqPkt->RoomNumber == -1)
		room = m_pRefRoomMgr->SearchEmptyRoom();
	else
		room = m_pRefRoomMgr->GetRoom(reqPkt->RoomNumber);

	room->EnterUser(user);
	user->EnterRoom(reqPkt->RoomNumber);

	room->NotifyEnterUser(user);

	resPkt.SetError(ERROR_CODE::NONE);
	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(resPkt), (char*)&resPkt);
	return ERROR_CODE::NONE;
}

//TODO: 방 나가기
ERROR_CODE PacketProcess::LeaveRoom(PacketInfo packetInfo)
{
	PktLeaveRoomRes resPkt;
	
	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

	if (errorCode != ERROR_CODE::NONE)
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_LEAVE_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::USER_NOT_LOGIN);
	}

	if (!user->IsCurDomainInRoom())
	{
		resPkt.SetError(ERROR_CODE::USER_NOT_IN_ROOM);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_LEAVE_ROOM_RES, sizeof(resPkt), (char*) &resPkt, ERROR_CODE::USER_NOT_IN_ROOM);
	}

	Room* room = m_pRefRoomMgr->GetRoom(user->GetRoomIndex());

	user->ExitRoom();
	room->ExitUser(user);

	room->NotifyLeaveUser(user);

	resPkt.SetError(ERROR_CODE::NONE);
	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_LEAVE_ROOM_RES, sizeof(resPkt), (char*)&resPkt);

	return ERROR_CODE::NONE;
}


//TODO: 방 채팅
ERROR_CODE PacketProcess::ChatRoom(PacketInfo packetInfo)
{
	PktChatRoomRes resPkt;
	auto reqPkt = (PktChatRoomReq*)packetInfo.pRefData;

	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);
	
	if (errorCode != ERROR_CODE::NONE)
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_CHAT_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::USER_NOT_LOGIN);
	}

	if (!user->IsCurDomainInRoom())
	{
		resPkt.SetError(ERROR_CODE::USER_NOT_IN_ROOM);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_CHAT_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::USER_NOT_IN_ROOM);
	}

	Room* room = m_pRefRoomMgr->GetRoom(user->GetRoomIndex());
	
	wchar_t msg[256];
	wcscpy_s(msg, reqPkt->Msg);
	m_pRefLogger->Write(LOG_TYPE::L_INFO, "%s", reqPkt->Msg);
	room->NotifyChat(reqPkt->Msg, user->GetID());

	resPkt.SetError(ERROR_CODE::NONE);
	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_CHAT_ROOM_RES, sizeof(resPkt), (char*)&resPkt);

	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::SendError(int sessionIndex, short packetID, int size, char* data, ERROR_CODE errorCode)
{
	m_pRefNetwork->SendData(sessionIndex, packetID, size, data);
	return errorCode;
}