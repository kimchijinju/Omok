﻿namespace csharp_test_client
{
    partial class mainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnDisconnect = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.checkBoxLocalHostIP = new System.Windows.Forms.CheckBox();
            this.textBoxIP = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.labelStatus = new System.Windows.Forms.Label();
            this.listBoxLog = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxUserID = new System.Windows.Forms.TextBox();
            this.textBoxUserPW = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.Room = new System.Windows.Forms.GroupBox();
            this.btn_GameStart = new System.Windows.Forms.Button();
            this.btn_Cancel = new System.Windows.Forms.Button();
            this.btn_Ready = new System.Windows.Forms.Button();
            this.btnRoomChat = new System.Windows.Forms.Button();
            this.textBoxRoomSendMsg = new System.Windows.Forms.TextBox();
            this.listBoxRoomChatMsg = new System.Windows.Forms.ListBox();
            this.label4 = new System.Windows.Forms.Label();
            this.listBoxRoomUserList = new System.Windows.Forms.ListBox();
            this.btn_RoomLeave = new System.Windows.Forms.Button();
            this.btn_RoomEnter = new System.Windows.Forms.Button();
            this.textBoxRoomNumber = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.OmokBoard = new System.Windows.Forms.Panel();
            this.groupBox5.SuspendLayout();
            this.Room.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btnDisconnect.Location = new System.Drawing.Point(430, 49);
            this.btnDisconnect.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(82, 35);
            this.btnDisconnect.TabIndex = 29;
            this.btnDisconnect.Text = "접속끊기";
            this.btnDisconnect.UseVisualStyleBackColor = true;
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btnConnect.Location = new System.Drawing.Point(430, 13);
            this.btnConnect.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(82, 34);
            this.btnConnect.TabIndex = 28;
            this.btnConnect.Text = "접속하기";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.textBoxPort);
            this.groupBox5.Controls.Add(this.label10);
            this.groupBox5.Controls.Add(this.checkBoxLocalHostIP);
            this.groupBox5.Controls.Add(this.textBoxIP);
            this.groupBox5.Controls.Add(this.label9);
            this.groupBox5.Location = new System.Drawing.Point(12, 15);
            this.groupBox5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBox5.Size = new System.Drawing.Size(403, 65);
            this.groupBox5.TabIndex = 27;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Socket 더미 클라이언트 설정";
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(225, 25);
            this.textBoxPort.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxPort.MaxLength = 6;
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(51, 23);
            this.textBoxPort.TabIndex = 18;
            this.textBoxPort.Text = "11021";
            this.textBoxPort.WordWrap = false;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(163, 30);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(62, 15);
            this.label10.TabIndex = 17;
            this.label10.Text = "포트 번호:";
            // 
            // checkBoxLocalHostIP
            // 
            this.checkBoxLocalHostIP.AutoSize = true;
            this.checkBoxLocalHostIP.Checked = true;
            this.checkBoxLocalHostIP.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLocalHostIP.Location = new System.Drawing.Point(285, 30);
            this.checkBoxLocalHostIP.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.checkBoxLocalHostIP.Name = "checkBoxLocalHostIP";
            this.checkBoxLocalHostIP.Size = new System.Drawing.Size(102, 19);
            this.checkBoxLocalHostIP.TabIndex = 15;
            this.checkBoxLocalHostIP.Text = "localhost 사용";
            this.checkBoxLocalHostIP.UseVisualStyleBackColor = true;
            // 
            // textBoxIP
            // 
            this.textBoxIP.Location = new System.Drawing.Point(68, 24);
            this.textBoxIP.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxIP.MaxLength = 15;
            this.textBoxIP.Name = "textBoxIP";
            this.textBoxIP.Size = new System.Drawing.Size(87, 23);
            this.textBoxIP.TabIndex = 11;
            this.textBoxIP.Text = "0.0.0.0";
            this.textBoxIP.WordWrap = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 29);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(62, 15);
            this.label9.TabIndex = 10;
            this.label9.Text = "서버 주소:";
            // 
            // labelStatus
            // 
            this.labelStatus.AutoSize = true;
            this.labelStatus.Location = new System.Drawing.Point(12, 632);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(112, 15);
            this.labelStatus.TabIndex = 40;
            this.labelStatus.Text = "서버 접속 상태: ???";
            // 
            // listBoxLog
            // 
            this.listBoxLog.FormattingEnabled = true;
            this.listBoxLog.HorizontalScrollbar = true;
            this.listBoxLog.ItemHeight = 15;
            this.listBoxLog.Location = new System.Drawing.Point(13, 458);
            this.listBoxLog.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.listBoxLog.Name = "listBoxLog";
            this.listBoxLog.Size = new System.Drawing.Size(499, 154);
            this.listBoxLog.TabIndex = 41;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(30, 100);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 15);
            this.label1.TabIndex = 42;
            this.label1.Text = "UserID:";
            // 
            // textBoxUserID
            // 
            this.textBoxUserID.Location = new System.Drawing.Point(79, 97);
            this.textBoxUserID.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxUserID.MaxLength = 6;
            this.textBoxUserID.Name = "textBoxUserID";
            this.textBoxUserID.Size = new System.Drawing.Size(87, 23);
            this.textBoxUserID.TabIndex = 43;
            this.textBoxUserID.Text = "jacking75";
            this.textBoxUserID.WordWrap = false;
            // 
            // textBoxUserPW
            // 
            this.textBoxUserPW.Location = new System.Drawing.Point(237, 97);
            this.textBoxUserPW.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxUserPW.MaxLength = 6;
            this.textBoxUserPW.Name = "textBoxUserPW";
            this.textBoxUserPW.Size = new System.Drawing.Size(87, 23);
            this.textBoxUserPW.TabIndex = 45;
            this.textBoxUserPW.Text = "jacking75";
            this.textBoxUserPW.WordWrap = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(178, 101);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 15);
            this.label2.TabIndex = 44;
            this.label2.Text = "PassWD:";
            // 
            // button2
            // 
            this.button2.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.button2.Location = new System.Drawing.Point(358, 92);
            this.button2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(145, 32);
            this.button2.TabIndex = 46;
            this.button2.Text = "Login";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // Room
            // 
            this.Room.Controls.Add(this.btn_GameStart);
            this.Room.Controls.Add(this.btn_Cancel);
            this.Room.Controls.Add(this.btn_Ready);
            this.Room.Controls.Add(this.btnRoomChat);
            this.Room.Controls.Add(this.textBoxRoomSendMsg);
            this.Room.Controls.Add(this.listBoxRoomChatMsg);
            this.Room.Controls.Add(this.label4);
            this.Room.Controls.Add(this.listBoxRoomUserList);
            this.Room.Controls.Add(this.btn_RoomLeave);
            this.Room.Controls.Add(this.btn_RoomEnter);
            this.Room.Controls.Add(this.textBoxRoomNumber);
            this.Room.Controls.Add(this.label3);
            this.Room.Location = new System.Drawing.Point(12, 140);
            this.Room.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Room.Name = "Room";
            this.Room.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Room.Size = new System.Drawing.Size(500, 312);
            this.Room.TabIndex = 47;
            this.Room.TabStop = false;
            this.Room.Text = "Room";
            // 
            // btn_GameStart
            // 
            this.btn_GameStart.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btn_GameStart.Location = new System.Drawing.Point(363, 43);
            this.btn_GameStart.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btn_GameStart.Name = "btn_GameStart";
            this.btn_GameStart.Size = new System.Drawing.Size(119, 23);
            this.btn_GameStart.TabIndex = 56;
            this.btn_GameStart.Text = "Game Start";
            this.btn_GameStart.UseVisualStyleBackColor = true;
            this.btn_GameStart.Click += new System.EventHandler(this.button_GameStart_Click);
            // 
            // btn_Cancel
            // 
            this.btn_Cancel.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btn_Cancel.Location = new System.Drawing.Point(422, 12);
            this.btn_Cancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btn_Cancel.Name = "btn_Cancel";
            this.btn_Cancel.Size = new System.Drawing.Size(60, 23);
            this.btn_Cancel.TabIndex = 55;
            this.btn_Cancel.Text = "cancel";
            this.btn_Cancel.UseVisualStyleBackColor = true;
            this.btn_Cancel.Click += new System.EventHandler(this.button_Cancel_Click);
            // 
            // btn_Ready
            // 
            this.btn_Ready.AutoEllipsis = true;
            this.btn_Ready.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btn_Ready.Location = new System.Drawing.Point(363, 12);
            this.btn_Ready.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btn_Ready.Name = "btn_Ready";
            this.btn_Ready.Size = new System.Drawing.Size(53, 23);
            this.btn_Ready.TabIndex = 54;
            this.btn_Ready.Text = "Ready";
            this.btn_Ready.UseVisualStyleBackColor = true;
            this.btn_Ready.Click += new System.EventHandler(this.button_Ready_Click);
            // 
            // btnRoomChat
            // 
            this.btnRoomChat.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btnRoomChat.Location = new System.Drawing.Point(437, 265);
            this.btnRoomChat.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnRoomChat.Name = "btnRoomChat";
            this.btnRoomChat.Size = new System.Drawing.Size(50, 32);
            this.btnRoomChat.TabIndex = 53;
            this.btnRoomChat.Text = "chat";
            this.btnRoomChat.UseVisualStyleBackColor = true;
            this.btnRoomChat.Click += new System.EventHandler(this.btnRoomChat_Click);
            // 
            // textBoxRoomSendMsg
            // 
            this.textBoxRoomSendMsg.Location = new System.Drawing.Point(13, 269);
            this.textBoxRoomSendMsg.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxRoomSendMsg.MaxLength = 32;
            this.textBoxRoomSendMsg.Name = "textBoxRoomSendMsg";
            this.textBoxRoomSendMsg.Size = new System.Drawing.Size(419, 23);
            this.textBoxRoomSendMsg.TabIndex = 52;
            this.textBoxRoomSendMsg.Text = "test1";
            this.textBoxRoomSendMsg.WordWrap = false;
            // 
            // listBoxRoomChatMsg
            // 
            this.listBoxRoomChatMsg.FormattingEnabled = true;
            this.listBoxRoomChatMsg.ItemHeight = 15;
            this.listBoxRoomChatMsg.Location = new System.Drawing.Point(144, 81);
            this.listBoxRoomChatMsg.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listBoxRoomChatMsg.Name = "listBoxRoomChatMsg";
            this.listBoxRoomChatMsg.Size = new System.Drawing.Size(343, 169);
            this.listBoxRoomChatMsg.TabIndex = 51;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(10, 64);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(55, 15);
            this.label4.TabIndex = 50;
            this.label4.Text = "User List:";
            // 
            // listBoxRoomUserList
            // 
            this.listBoxRoomUserList.FormattingEnabled = true;
            this.listBoxRoomUserList.ItemHeight = 15;
            this.listBoxRoomUserList.Location = new System.Drawing.Point(13, 82);
            this.listBoxRoomUserList.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listBoxRoomUserList.Name = "listBoxRoomUserList";
            this.listBoxRoomUserList.Size = new System.Drawing.Size(123, 169);
            this.listBoxRoomUserList.TabIndex = 49;
            // 
            // btn_RoomLeave
            // 
            this.btn_RoomLeave.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btn_RoomLeave.Location = new System.Drawing.Point(225, 22);
            this.btn_RoomLeave.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btn_RoomLeave.Name = "btn_RoomLeave";
            this.btn_RoomLeave.Size = new System.Drawing.Size(70, 32);
            this.btn_RoomLeave.TabIndex = 48;
            this.btn_RoomLeave.Text = "Leave";
            this.btn_RoomLeave.UseVisualStyleBackColor = true;
            this.btn_RoomLeave.Click += new System.EventHandler(this.btn_RoomLeave_Click);
            // 
            // btn_RoomEnter
            // 
            this.btn_RoomEnter.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.btn_RoomEnter.Location = new System.Drawing.Point(150, 22);
            this.btn_RoomEnter.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btn_RoomEnter.Name = "btn_RoomEnter";
            this.btn_RoomEnter.Size = new System.Drawing.Size(69, 32);
            this.btn_RoomEnter.TabIndex = 47;
            this.btn_RoomEnter.Text = "Enter";
            this.btn_RoomEnter.UseVisualStyleBackColor = true;
            this.btn_RoomEnter.Click += new System.EventHandler(this.btn_RoomEnter_Click);
            // 
            // textBoxRoomNumber
            // 
            this.textBoxRoomNumber.Location = new System.Drawing.Point(101, 28);
            this.textBoxRoomNumber.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxRoomNumber.MaxLength = 6;
            this.textBoxRoomNumber.Name = "textBoxRoomNumber";
            this.textBoxRoomNumber.Size = new System.Drawing.Size(38, 23);
            this.textBoxRoomNumber.TabIndex = 44;
            this.textBoxRoomNumber.Text = "0";
            this.textBoxRoomNumber.WordWrap = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(5, 31);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(90, 15);
            this.label3.TabIndex = 43;
            this.label3.Text = "Room Number:";
            // 
            // OmokBoard
            // 
            this.OmokBoard.Location = new System.Drawing.Point(528, 27);
            this.OmokBoard.Name = "OmokBoard";
            this.OmokBoard.Size = new System.Drawing.Size(595, 593);
            this.OmokBoard.TabIndex = 48;
            this.OmokBoard.Paint += new System.Windows.Forms.PaintEventHandler(this.OmokBoard_Paint);
            this.OmokBoard.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OmokBoard_MouseDown);
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1159, 649);
            this.Controls.Add(this.OmokBoard);
            this.Controls.Add(this.Room);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.textBoxUserPW);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBoxUserID);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.labelStatus);
            this.Controls.Add(this.listBoxLog);
            this.Controls.Add(this.btnDisconnect);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.groupBox5);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "mainForm";
            this.Text = "네트워크 테스트 클라이언트";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.mainForm_FormClosing);
            this.Load += new System.EventHandler(this.mainForm_Load);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.Room.ResumeLayout(false);
            this.Room.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.CheckBox checkBoxLocalHostIP;
        private System.Windows.Forms.TextBox textBoxIP;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.ListBox listBoxLog;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxUserID;
        private System.Windows.Forms.TextBox textBoxUserPW;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.GroupBox Room;
        private System.Windows.Forms.Button btn_RoomLeave;
        private System.Windows.Forms.Button btn_RoomEnter;
        private System.Windows.Forms.TextBox textBoxRoomNumber;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnRoomChat;
        private System.Windows.Forms.TextBox textBoxRoomSendMsg;
        private System.Windows.Forms.ListBox listBoxRoomChatMsg;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ListBox listBoxRoomUserList;
        private System.Windows.Forms.Button btn_Cancel;
        private System.Windows.Forms.Button btn_Ready;
        private System.Windows.Forms.Button btn_GameStart;
        private System.Windows.Forms.Panel OmokBoard;
    }
}

