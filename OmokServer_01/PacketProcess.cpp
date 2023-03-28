#include <cstring>

#include "NetLib/ILog.h"
#include "NetLib/TcpNetwork.h"
#include "User.h"
#include "UserManager.h"
#include "Room.h"
#include "RoomManager.h"
#include "PacketProcess.h"
#include "Game.h"

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
	case (int)commonPacketId::PK_READY_GAME_ROOM_REQ:
		Ready(packetInfo);
		break;
	case (int)commonPacketId::PK_CANCEL_READY_GAME_ROOM_REQ:
		CancelReady(packetInfo);
		break;
	case (int)commonPacketId::PK_START_GAME_ROOM_NTF:
		GameStart(packetInfo);
		break;
	case (int)commonPacketId::PK_PUT_AL_ROOM_REQ:
		PutAL(packetInfo);
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
		
	resPkt.SetError(addRet);
	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_LOGIN_IN_RES, sizeof(resPkt), (char*)&resPkt);
	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::EnterRoom(PacketInfo packetInfo)
{
	PktEnterRoomRes resPkt;

	auto reqPkt = (PktEnterRoomReq*)packetInfo.pRefData;
	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

	if (errorCode != ERROR_CODE::NONE) 
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);
	}

	if (!user->IsCurDomainInLogIn())
	{
		resPkt.SetError(ERROR_CODE::USER_NOT_LOGIN);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::USER_NOT_LOGIN);
	}

	if (user->IsCurDomainInRoom())
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
	{
		room = m_pRefRoomMgr->SearchEmptyRoom();
	}
	else
	{
		room = m_pRefRoomMgr->GetRoom(reqPkt->RoomNumber);
	}

	if (room->FullRoom())
	{
		resPkt.SetError(ERROR_CODE::ROOM_INVALID_INDEX);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::ROOM_INVALID_INDEX);
	}
	
	room->NotifyEnterUser(user);

	room->EnterUser(user);
	user->EnterRoom(reqPkt->RoomNumber);

	room->GetRoomUserList(user);

	resPkt.SetError(ERROR_CODE::NONE);
	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(resPkt), (char*)&resPkt);
	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::LeaveRoom(PacketInfo packetInfo)
{
	PktLeaveRoomRes resPkt;
	
	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

	if (errorCode != ERROR_CODE::NONE)
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_LEAVE_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);
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


ERROR_CODE PacketProcess::ChatRoom(PacketInfo packetInfo)
{
	PktChatRoomRes resPkt;
	auto reqPkt = (PktChatRoomReq*)packetInfo.pRefData;
	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);
	
	if (errorCode != ERROR_CODE::NONE)
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_CHAT_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);
	}

	Room* room = m_pRefRoomMgr->GetRoom(user->GetRoomIndex());
	
	if (room == nullptr)
	{
		resPkt.SetError(ERROR_CODE::USER_NOT_IN_ROOM);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_CHAT_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::USER_NOT_IN_ROOM);
	}

	room->NotifyChat(reqPkt->Msg, user->GetID(), user->GetSessionIndex());

	resPkt.SetError(ERROR_CODE::NONE);
	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_CHAT_ROOM_RES, sizeof(resPkt), (char*)&resPkt);

	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::Ready(PacketInfo packetInfo)
{
	PktReadyGameRoomRes resPkt;
	
	auto reqPkt = (PktReadyGameRoomReq*)packetInfo.pRefData;
	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);
	
	if (errorCode != ERROR_CODE::NONE)
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_READY_GAME_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);
	}

	if (user->IsCurDomainInLogIn())
	{
		resPkt.SetError(ERROR_CODE::USER_NOT_IN_ROOM);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_READY_GAME_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::USER_NOT_IN_ROOM);;
	}

	user->Ready();

	resPkt.SetError(ERROR_CODE::NONE);
	return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_READY_GAME_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::NONE);;
}

ERROR_CODE PacketProcess::CancelReady(PacketInfo packetInfo)
{
	PktCancelReadyGameRoomRes resPkt;

	auto reqPkt = (PktReadyGameRoomReq*)packetInfo.pRefData;
	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);
	
	if (errorCode != ERROR_CODE::NONE)
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_CANCEL_READY_GAME_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);
	}

	if (user->IsReady())
	{
		user->CancelReady();
		resPkt.SetError(ERROR_CODE::NONE);
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_CANCEL_READY_GAME_ROOM_RES, sizeof(resPkt), (char*)&resPkt);
		return ERROR_CODE::NONE;
	}

	resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
	return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_CANCEL_READY_GAME_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);

}

ERROR_CODE PacketProcess::GameStart(PacketInfo packetInfo)
{
	PktStartGameRoomNtf ntfPkt;

	auto reqPkt = (PktStartGameRoomNtf*)packetInfo.pRefData;
	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

	Room* room = m_pRefRoomMgr->GetRoom(user->GetRoomIndex());

	if (room->AllUserReady())
	{
		User* black = room->GetUser(0);
		User* white = room->GetUser(1);
		room->GameStart(black, white);
		strcpy(ntfPkt.TurnUserID, black->GetID().c_str());
		room->SendNotify((short)PACKET_ID::PK_START_GAME_ROOM_NTF, sizeof(ntfPkt), (char*)&ntfPkt);
		return ERROR_CODE::NONE;
	}

	return ERROR_CODE::UNASSIGNED_ERROR;
}

ERROR_CODE PacketProcess::PutAL(PacketInfo packetInfo)
{
	PktPutALGameRoomRes resPkt;

	auto reqPkt = (PktPutALGameRoomReq*)packetInfo.pRefData;
	
	auto [errorCode, user] = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);

	if (errorCode != ERROR_CODE::NONE)
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_PUT_AL_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);
	}

	if (!user->IsPlaying())
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_PUT_AL_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);
	}
	
	Room* room = m_pRefRoomMgr->GetRoom(user->GetRoomIndex());
	Game* game = room->GetGame();

	m_pRefLogger->Write(LOG_TYPE::L_DEBUG, "x : %d y : %d", reqPkt->XPos, reqPkt->YPos);

	if (game == nullptr)
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_PUT_AL_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);
	}

	int x = reqPkt->XPos;
	int y = reqPkt->YPos;

	PktPutALGameRoomNtf ntfPkt = game->PutAL(x, y, user->GetID());

	if (ntfPkt.color == -1)
	{
		resPkt.SetError(ERROR_CODE::UNASSIGNED_ERROR);
		return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_PUT_AL_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::UNASSIGNED_ERROR);
	}

	room->SendNotify((short)PACKET_ID::PK_PUT_AL_ROOM_NTF, sizeof(ntfPkt), (char*)&ntfPkt);

	if (game->CheckOmok(x, y))
	{
		PktEndGameRoomNtf ntfPkt;
		strcpy(ntfPkt.WinUserID, user->GetID().c_str());
		room->SendNotify((short)PACKET_ID::PK_END_GAME_ROOM_NTF, sizeof(ntfPkt), (char*)&ntfPkt);
	}

	return SendError(packetInfo.SessionIndex, (short)PACKET_ID::PK_PUT_AL_ROOM_RES, sizeof(resPkt), (char*)&resPkt, ERROR_CODE::NONE);
}

ERROR_CODE PacketProcess::SendError(int sessionIndex, short packetID, int size, char* data, ERROR_CODE errorCode)
{
	m_pRefNetwork->SendData(sessionIndex, packetID, size, data);
	return errorCode;
}