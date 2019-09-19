#pragma once
#include <iostream>
enum DataPackType {
	login = 1,//登录类型
	registe = 2,
	sendMultiMsg = 3,
	addRoom = 4,
	logout = 5,
	leaveRoom = 6,
	findAll = 7,
	addFriend = 8,
	getFriendMsg = 9,
	agreeFriend = 10,
	getFriendList = 11,
	createRoom = 12,
	sendSingle = 13,
	getRoomList = 14,
	updateRooms = 15,
	deleteMsg = 16,
	saveChat = 17,
	checkRecord = 18,
	getUserInfo=19,
	updateUserInfo=20,
	deleteFriend=21,
	isFriend=22
};

#pragma pack(push,1)
struct DataPack
{
	DataPackType type;
	uint32_t	 size;
};

struct DataPackResult {
	uint32_t	type;
	int32_t		status;
	uint32_t	size;
	char		data[1];
};
#pragma pack(pop)
class CClientSocket
{
public:
	SOCKET m_hSocket;
	CString m_responce;
	DataPackResult* m_respPack;
public:
	CClientSocket();
	
	~CClientSocket();
	static void initSocket();//初始化socket

	void send(DataPackType type, const char* data, int size = -1);//发送消息
	void recv();//接收消息
	void freeResult(DataPackResult* p);//释放
	static CStringA CStrW2CStrA(const CStringW &cstrSrcW);
};

