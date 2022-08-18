using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace csharp_test_client
{
    class PacketDef
    {
        public const Int16 PACKET_HEADER_SIZE = 5;
        public const int MAX_USER_ID_BYTE_LENGTH = 16;
        public const int MAX_USER_PW_BYTE_LENGTH = 16;
        public const int MAX_ROOM_CHAT_MSG_SIZE = 256;
    }

    public enum PACKET_ID : ushort
    {
        // 로그인
        LOGIN_REQ = 21,
        LOGIN_RES = 22,

        LOBBY_LIST_REQ = 26,
        LOBBY_LIST_RES = 27,

        LOBBY_ENTER_REQ = 31,
        LOBBY_ENTER_RES = 32,

        LOBBY_LEAVE_REQ = 46,
        LOBBY_LEAVE_RES = 47,
        
        ROOM_ENTER_REQ = 61,
        ROOM_ENTER_RES = 62,        
        ROOM_NEW_USER_NTF = 63,
        ROOM_USER_LIST_NTF = 64,

        ROOM_LEAVE_REQ = 66,
        ROOM_LEAVE_RES = 67,
        ROOM_LEAVE_USER_NTF = 68,

        ROOM_CHAT_REQ = 76,
        ROOM_CHAT_RES = 77,
        ROOM_CHAT_NOTIFY = 78,

        PK_READY_GAME_ROOM_REQ = 81,
        PK_READY_GAME_ROOM_RES = 82,
        PK_READY_GAME_ROOM_NTF = 83,

        PK_CANCEL_READY_GAME_ROOM_REQ = 86,
        PK_CANCEL_READY_GAME_ROOM_RES = 87,
        PK_CANCEL_READY_GAME_ROOM_NTF = 88,

        PK_START_GAME_ROOM_NTF = 91,

        PK_PUT_AL_ROOM_REQ = 96,
        PK_PUT_AL_ROOM_RES = 97,
        PK_END_GAME_ROOM_NTF = 98,

    }


    public enum ERROR_CODE : Int16
    {
        ERROR_NONE = 0,



        ERROR_CODE_USER_MGR_INVALID_USER_UNIQUEID = 112,

        ERROR_CODE_PUBLIC_CHANNEL_IN_USER = 114,

        ERROR_CODE_PUBLIC_CHANNEL_INVALIDE_NUMBER = 115,
    }
}
