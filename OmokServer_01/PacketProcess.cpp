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

	//TODO: 유저가 방에 들어간 상태이면 방에서 빼고, 방에 아직 유저가 있다면 있는 유저에게 PK_USER_LEAVE_ROOM_NTF을 보낸다

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
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&resPkt);
		return addRet;
	}
		
	resPkt.ErrorCode = (short)addRet;
	m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_LOGIN_IN_RES, sizeof(PktLogInRes), (char*)&resPkt);

	return ERROR_CODE::NONE;
}

//TODO: 방 입장
ERROR_CODE PacketProcess::EnterRoom(PacketInfo packetInfo)
{
	PktEnterRoomRes resPkt;
	auto reqPkt = (PktEnterRoomReq*)packetInfo.pRefData;
	// 에러체크: 이미 방에 들어가 있는지 확인한다
	User* user = std::get<1>(m_pRefUserMgr->GetUser(packetInfo.SessionIndex));
	if(user->IsCurDomainInRoom())
	{
		m_pRefLogger->Write(LOG_TYPE::L_INFO, "in room error");
		resPkt.SetError(ERROR_CODE::USER_IN_ROOM);
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(PktEnterRoomRes), (char*)&resPkt);
		return ERROR_CODE::USER_IN_ROOM;
	}
	// 에러체크: 방 번호가 서버가 생성한 방번호 보다 큰지 확인한다
	if (reqPkt->RoomNumber >= m_pRefRoomMgr->MaxRoomCount())
	{
		m_pRefLogger->Write(LOG_TYPE::L_INFO, "index error ");
		resPkt.SetError(ERROR_CODE::ROOM_INVALID_INDEX);
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_ENTER_ROOM_RES, sizeof(PktEnterRoomRes), (char*)&resPkt);
		return ERROR_CODE::ROOM_INVALID_INDEX;
	}
	m_pRefLogger->Write(LOG_TYPE::L_INFO, "room index : %d", reqPkt->RoomNumber);
	/* 방번호의 방 객체를 얻는다.

	방에 들어간 인원이 이미 다 찬 상태이면 실패이다.

	원하는 방번호를 가진 방에 입장시킨다. */
	Room* room;

	if (reqPkt->RoomNumber == -1)
	{
		room = m_pRefRoomMgr->SearchEmptyRoom();
	}
	else
	{
		room = m_pRefRoomMgr->GetRoom(reqPkt->RoomNumber);
	}
	room->EnterUser(user);

	/*
	  위에서 성공한 경우
 	  현재 방 인원수가 2명 이상이면 이미 다른 사람이 있는 방에 입장한 것이므로 기존 사람들에게
	  PK_NEW_USER_ENTER_ROOM_NTF 패킷을 보낸다.	 

	  유저 상태를 방 입장 상태로 바꾸고, 방번호를 저장한다
	*/
	if (room->GetUserCount() >= 2)
	{
		for (int i = 0; i < room->GetUserCount() - 1; i++) 
		{
			m_pRefLogger->Write(LOG_TYPE::L_INFO, "in Loop");
			auto userSession = room->GetUser(i)->GetSessionIndex();
			resPkt.SetError(ERROR_CODE::NONE);
			m_pRefNetwork->SendData(userSession, (short)PACKET_ID::PK_NEW_USER_ENTER_ROOM_NTF, sizeof(PktEnterRoomRes), (char*)&resPkt);
		}
	}
	user->EnterRoom(reqPkt->RoomNumber);

	return ERROR_CODE::NONE;
}

//TODO: 방 나가기
ERROR_CODE PacketProcess::LeaveRoom(PacketInfo packetInfo)
{
	PktLeaveRoomRes resPkt;
	
	User* user = std::get<1>(m_pRefUserMgr->GetUser(packetInfo.SessionIndex));
	// 유저가 방에 들어온 상태가 맞는지 확인한다
	if (!user->IsCurDomainInRoom())
	{
		resPkt.SetError(ERROR_CODE::USER_IN_ROOM);
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::PK_LEAVE_ROOM_RES, sizeof(PktLeaveRoomRes), (char*)&resPkt);
		return ERROR_CODE::USER_IN_ROOM;
	}
	// 유저가 들어간 방의 객체를 가져온다
	Room* room = m_pRefRoomMgr->GetRoom(user->GetRoomIndex());
	// 방에서 유저를 빼고, 유저 상태도 변경한다.
	room->ExitUser(user);
	user->ExitRoom();
	// 아직 방에 다른 유저가 있다면 PK_USER_LEAVE_ROOM_NTF을 보낸다
	for (int i = 0; i < room->GetUserCount(); i++)
	{
		int userSession = room->GetUser(i)->GetSessionIndex();
		m_pRefNetwork->SendData(userSession, (short)PACKET_ID::PK_USER_LEAVE_ROOM_NTF, sizeof(PktLeaveRoomRes), &resPkt);
	}
	return ERROR_CODE::NONE;
}

//TODO: 방 채팅
ERROR_CODE PacketProcess::ChatRoom(PacketInfo packetInfo)
{
	PktChatRoomRes resPkt;
	auto reqPkt = (PktChatRoomReq*)packetInfo.pRefData;

	// 유저가 방에 들어온 상태가 맞는지 확인한다
	// 유저가 들어간 방의 객체를 가져온다

	// 채팅 메시지에 문제가 없다면 요청이 성공함을 알린다

	// 방의 모든 유저에게 PK_CHAT_ROOM_NTF을 보낸다
	return ERROR_CODE::NONE;
}
