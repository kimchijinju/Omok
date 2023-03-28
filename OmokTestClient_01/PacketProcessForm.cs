using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace csharp_test_client
{
    public partial class 
        mainForm
    {
        Dictionary<PACKET_ID, Action<byte[]>> PacketFuncDic = new Dictionary<PACKET_ID, Action<byte[]>>();

        void SetPacketHandler()
        {
            //PacketFuncDic.Add(PACKET_ID.PACKET_ID_ERROR_NTF, PacketProcess_ErrorNotify);
            PacketFuncDic.Add(PACKET_ID.LOGIN_RES, PacketProcess_LoginResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_ENTER_RES, PacketProcess_RoomEnterResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_USER_LIST_NTF, PacketProcess_RoomUserListNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_NEW_USER_NTF, PacketProcess_RoomNewUserNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_RES, PacketProcess_RoomLeaveResponse);
            PacketFuncDic.Add(PACKET_ID.ROOM_LEAVE_USER_NTF, PacketProcess_RoomLeaveUserNotify);
            PacketFuncDic.Add(PACKET_ID.ROOM_CHAT_RES, PacketProcess_RoomChatResponse);            
            PacketFuncDic.Add(PACKET_ID.ROOM_CHAT_NOTIFY, PacketProcess_RoomChatNotify);
            PacketFuncDic.Add(PACKET_ID.PK_READY_GAME_ROOM_RES, PacketProcess_ReadyGameResponse);
            PacketFuncDic.Add(PACKET_ID.PK_CANCEL_READY_GAME_ROOM_RES, PacketProcess_CancelReadyGameResponse);
            PacketFuncDic.Add(PACKET_ID.PK_START_GAME_ROOM_NTF, PacketProcess_GameStart);
            PacketFuncDic.Add(PACKET_ID.PK_PUT_AL_ROOM_NTF, PacketProcess_PutALNtf);
            PacketFuncDic.Add(PACKET_ID.PK_END_GAME_ROOM_NTF, PacketProcess_EndGameNtf);
            //PacketFuncDic.Add(PACKET_ID.PACKET_ID_ROOM_RELAY_NTF, PacketProcess_RoomRelayNotify);
        }

        void PacketProcess(PacketData packet)
        {
            var packetType = (PACKET_ID)packet.PacketID;
            //DevLog.Write("Packet Error:  PacketID:{packet.PacketID.ToString()},  Error: {(ERROR_CODE)packet.Result}");
            //DevLog.Write("RawPacket: " + packet.PacketID.ToString() + ", " + PacketDump.Bytes(packet.BodyData));

            if (PacketFuncDic.ContainsKey(packetType))
            {
                PacketFuncDic[packetType](packet.BodyData);
            }
            else
            {
                DevLog.Write("Unknown Packet Id: " + packet.PacketID.ToString());
            }         
        }

        void PacketProcess_ErrorNotify(byte[] bodyData)
        {
            var notifyPkt = new ErrorNtfPacket();
            notifyPkt.FromBytes(bodyData);

            DevLog.Write($"에러 통보 받음:  {notifyPkt.Error}");
        }


        void PacketProcess_LoginResponse(byte[] bodyData)
        {
            var responsePkt = new LoginResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"로그인 결과:  {(ERROR_CODE)responsePkt.Result}");
        }


        void PacketProcess_RoomEnterResponse(byte[] bodyData)
        {
            var responsePkt = new RoomEnterResPacket();
            responsePkt.FromBytes(bodyData);

            DevLog.Write($"방 입장 결과:  {(ERROR_CODE)responsePkt.Result}");
            if ((ERROR_CODE)responsePkt.Result == 0)
            {
                btn_GameStart.Enabled = true;
            }    
        }

        void PacketProcess_RoomUserListNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomUserListNtfPacket();
            notifyPkt.FromBytes(bodyData);

            for (int i = 0; i < notifyPkt.UserCount; ++i)
            {
                AddRoomUserList(notifyPkt.UserIDList[i]);
            }

            DevLog.Write($"방의 기존 유저 리스트 받음");
        }

        void PacketProcess_RoomNewUserNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomNewUserNtfPacket();
            notifyPkt.FromBytes(bodyData);

            AddRoomUserList(notifyPkt.UserID);
            
            DevLog.Write($"방에 새로 들어온 유저 받음");
        }


        void PacketProcess_RoomLeaveResponse(byte[] bodyData)
        {
            var responsePkt = new RoomLeaveResPacket();
            responsePkt.FromBytes(bodyData);

            RoomUserListClear();

            DevLog.Write($"방 나가기 결과:  {(ERROR_CODE)responsePkt.Result}");
            if ((ERROR_CODE)responsePkt.Result == 0)
            {
                btn_GameStart.Enabled = false;
            }
        }

        void PacketProcess_RoomLeaveUserNotify(byte[] bodyData)
        {
            var notifyPkt = new RoomLeaveUserNtfPacket();
            notifyPkt.FromBytes(bodyData);

            RemoveRoomUserList(notifyPkt.UserID);

            DevLog.Write($"방에서 나간 유저 받음");
        }


        void PacketProcess_RoomChatResponse(byte[] bodyData)
        {
            var responsePkt = new RoomChatResPacket();
            responsePkt.FromBytes(bodyData);

            var errorCode = (ERROR_CODE)responsePkt.Result;
            var msg = $"방 채팅 요청 결과:  {(ERROR_CODE)responsePkt.Result}";
            if (errorCode == ERROR_CODE.ERROR_NONE)
            {
                DevLog.Write(msg, LOG_LEVEL.ERROR);
            }
            else
            {
                DevLog.Write("FAIL");
                //AddRoomChatMessageList(, msg);
            }
        }
          

        void PacketProcess_RoomChatNotify(byte[] bodyData)
        {
            var responsePkt = new RoomChatNtfPacket();
            responsePkt.FromBytes(bodyData);

            AddRoomChatMessageList(responsePkt.UserID, responsePkt.Message);
        }

        void AddRoomChatMessageList(string userID, string msgssage)
        {
            var msg = $"{userID}:  {msgssage}";

            if (listBoxRoomChatMsg.Items.Count > 512)
            {
                listBoxRoomChatMsg.Items.Clear();
            }

            listBoxRoomChatMsg.Items.Add(msg);
            listBoxRoomChatMsg.SelectedIndex = listBoxRoomChatMsg.Items.Count - 1;
        }

        void PacketProcess_ReadyGameResponse(byte[] bodyData)
        {
            var resPkt = new ReadyGameRoomRes();
            resPkt.FromBytes(bodyData);

            DevLog.Write($"준비 결과 : {(ERROR_CODE)resPkt.Result}");
            //레디 표시 로직
        }

        void PacketProcess_CancelReadyGameResponse(byte[] bodyData)
        {
            var resPkt = new CancelReadyGameRoomRes();
            resPkt.FromBytes(bodyData);

            DevLog.Write($"준비 취소 결과 : {(ERROR_CODE)resPkt.Result}");
            //레디 취소 로직
        }

        void PacketProcess_GameStart(byte[] bodyData)
        {
            var ntfPkt = new StartGameRoomNtfPacket();
            ntfPkt.FromBytes(bodyData);

            DevLog.Write($"시작 턴 유저 : {ntfPkt.TurnUserID}");

            InitButtonStatus(false);

        }

        void PacketProcess_PutALNtf(byte[] bodyData)
        {
            var ntfPkt = new PutALNtf();
            ntfPkt.FromBytes(bodyData);
            int x = ntfPkt.XPos;
            int y = ntfPkt.YPos;
            //Omok[x,y] = (STONE) ntfPkt.Color;
            // 바둑판[x,y] 에 돌을 그린다
            Rectangle r = new Rectangle(
              margin + gridSize * x - stoneSize / 2,
              margin + gridSize * y - stoneSize / 2,
              stoneSize, stoneSize);

            // 검은돌 차례
            if (ntfPkt.Color == (SByte) STONE.BLACK)
            {
                g.FillEllipse(bBrush, r);
            }
            else if(ntfPkt.Color == (SByte) STONE.WHITE)
            {
                g.FillEllipse(wBrush, r);
            }
        }

        void PacketProcess_EndGameNtf(byte[] bodyData)
        {
            var ntfPkt = new EndGameNtf();

            ntfPkt.FromBytes(bodyData);
            System.Windows.Forms.MessageBox.Show($"{ntfPkt.WinUserID} is Winner! ");
            InitButtonStatus(true);
            OmokBoard.Refresh();
            
        }

        void RoomUserListClear()
        {
            listBoxRoomUserList.Items.Clear();
            listBoxRoomChatMsg.Items.Clear();
        }
    }

}
