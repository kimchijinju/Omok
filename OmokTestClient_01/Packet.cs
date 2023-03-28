using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace csharp_test_client
{
    struct PacketData
    {
        public Int16 DataSize;
        public Int16 PacketID;
        public SByte Type;
        public byte[] BodyData;
    }

    public class PacketDump
    {
        public static string Bytes(byte[] byteArr)
        {
            StringBuilder sb = new StringBuilder("[");
            for (int i = 0; i < byteArr.Length; ++i)
            {
                sb.Append(byteArr[i] + " ");
            }
            sb.Append("]");
            return sb.ToString();
        }
    }
    

    public class ErrorNtfPacket
    {
        public ERROR_CODE Error;

        public bool FromBytes(byte[] bodyData)
        {
            Error = (ERROR_CODE)BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }
    

    public class LoginReqPacket
    {
        byte[] UserID = new byte[PacketDef.MAX_USER_ID_BYTE_LENGTH];
        byte[] UserPW = new byte[PacketDef.MAX_USER_PW_BYTE_LENGTH];

        public void SetValue(string userID, string userPW)
        {
            Encoding.UTF8.GetBytes(userID).CopyTo(UserID, 0);
            Encoding.UTF8.GetBytes(userPW).CopyTo(UserPW, 0);
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(UserID);
            dataSource.AddRange(UserPW);
            return dataSource.ToArray();
        }
    }

    public class LoginResPacket
    {
        public Int16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }


    public class RoomEnterReqPacket
    {
        int RoomNumber;
        public void SetValue(int roomNumber)
        {
            RoomNumber = roomNumber;
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes(RoomNumber));
            return dataSource.ToArray();
        }
    }

    public class RoomEnterResPacket
    {
        public Int16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class RoomUserListNtfPacket
    {
        public int UserCount = 0;
        public List<string> UserIDList = new List<string>();

        public bool FromBytes(byte[] bodyData)
        {
            var readPos = 0;
            var userCount = (SByte)bodyData[readPos];
            ++readPos;

            for (int i = 0; i < userCount; ++i)
            {
                var idlen = PacketDef.MAX_USER_ID_BYTE_LENGTH;

                var id = Encoding.UTF8.GetString(bodyData, readPos, idlen);
                readPos += PacketDef.MAX_USER_ID_BYTE_LENGTH;

                UserIDList.Add(id);
            }

            UserCount = userCount;
            return true;
        }
    }

    public class RoomNewUserNtfPacket
    {
        public string UserID;

        public bool FromBytes(byte[] bodyData)
        {

            UserID = Encoding.UTF8.GetString(bodyData, 0, PacketDef.MAX_USER_ID_BYTE_LENGTH);
            UserID.Trim();

            return true;
        }
    }


    public class RoomChatReqPacket
    {
        byte[] Msg = new byte[PacketDef.MAX_ROOM_CHAT_MSG_SIZE];

        public void SetValue(string message)
        {
            var msg = Encoding.UTF8.GetBytes(message);
            Buffer.BlockCopy(msg, 0, Msg, 0, msg.Length);
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(Msg);
            return dataSource.ToArray();
        }
    }

    public class RoomChatResPacket
    {
        public Int16 Result;
        
        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class RoomChatNtfPacket
    {
        public string UserID;
        public string Message;

        public bool FromBytes(byte[] bodyData)
        {
            var readPos = 0;

            var idLen = bodyData[readPos];
            ++readPos;

            UserID = Encoding.UTF8.GetString(bodyData, readPos, idLen);
            readPos += PacketDef.MAX_USER_ID_BYTE_LENGTH;

            var msgLen = BitConverter.ToInt16(bodyData, readPos);
            readPos += 2;

            Message = Encoding.UTF8.GetString(bodyData, readPos, msgLen);

            return true;
        }
    }

    public class RoomLeaveResPacket
    {
        public Int16 Result;
        
        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class RoomLeaveUserNtfPacket
    {
        public string UserID;

        public bool FromBytes(byte[] bodyData)
        {
            UserID = Encoding.UTF8.GetString(bodyData, 0, PacketDef.MAX_USER_ID_BYTE_LENGTH);
            return true;
        }
    }

    public class ReadyGameRoomRes
    {
        public Int16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }
    
    public class CancelReadyGameRoomRes
    {
        public Int16 Result;

        public bool FromBytes(byte[] bodyData)
        {
            Result = BitConverter.ToInt16(bodyData, 0);
            return true;
        }
    }

    public class StartGameRoomNtfPacket
    {
        public string TurnUserID;

        public bool FromBytes(byte[] bodyData)
        {
            TurnUserID = Encoding.UTF8.GetString(bodyData, 0, PacketDef.MAX_USER_ID_BYTE_LENGTH);
            return true;
        }
    }

    public class PutALGameRoomReq
    {
        int x;
        int y;

        public void SetValue(int x, int y)
        {
            this.x = x;
            this.y = y;
        }

        public byte[] ToBytes()
        {
            List<byte> dataSource = new List<byte>();
            dataSource.AddRange(BitConverter.GetBytes(x));
            dataSource.AddRange(BitConverter.GetBytes(y));
            return dataSource.ToArray();
        }
    }

    public class PutALNtf
    {
        public short XPos;
        public short YPos;
        public short Color;

        public bool FromBytes(byte[] bodyData)
        {
            XPos = BitConverter.ToInt16(bodyData, 0);
            YPos = BitConverter.ToInt16(bodyData, 2);
            Color = BitConverter.ToInt16(bodyData, 4);
            return true;
        }

    }

    public class EndGameNtf
    {
        public string WinUserID;

        public bool FromBytes(byte[] bodyData)
        {
            WinUserID = Encoding.UTF8.GetString(bodyData, 0, PacketDef.MAX_USER_ID_BYTE_LENGTH);
            return true;
        }
    }

    public class PingRequest
    {
        public Int16 PingNum;

        public byte[] ToBytes()
        {
            return BitConverter.GetBytes(PingNum);
        }

    }
}
