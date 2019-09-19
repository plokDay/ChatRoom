# 好友数据库用ID存，因为查询快占空间小
# 一个好友关系用两条记录存
import socket
import threading
import struct
import pymysql
from enum import Enum

class Database:
    def __init__(self):
        self.connecter = pymysql.connect(host='127.0.0.1',
                                         user='root',
                                         password='123456',
                                         db='mysql01',
                                         charset='utf8',
                                         cursorclass=pymysql.cursors.DictCursor)
        #返回字典型游标
        self.cursor = self.connecter.cursor(cursor=pymysql.cursors.DictCursor)

# 构造一个全局的数据库对象
_db = Database()
# 消息类型
class Request(Enum):
    # 登陆请求
    # 客户端 -> 服务端, 参数: 账号\n密码
    login       = 1

    # 注册请求
    # 客户端 -> 服务端, 参数: 账号\n密码
    register    = 2

    # 发消息请求
    # 客户端 -> 服务端, 参数: 消息
    sendMultiMsg= 3
    # 加入聊天室
    addRoom=4
    # 退出账号
    logout=5
    # 离开房间
    leaveRoom=6
    # 查找
    findAll=7
    # 添加好友
    addFriend=8
    # 获取消息列表
    getFriendMsg=9
    # 同意好友申请
    agreeFriend=10
    # 获取好友列表
    getFriendList=11
    # 创建聊天室
    createRoom=12
    # 私聊消息
    sendSingle=13
    # 活动房间列表
    getRoomList=14
    # 更新房间列表
    updateRooms=15
    # 删除验证信息
    deleteMsg=16
    # 保存聊天记录
    saveChat=17
    # 查看聊天记录
    checkRecord=18
    # 得到用户信息
    getUserInfo=19
    # 更改用户信息
    updateUserInfo=20
    # 删除好友
    deleteFriend=21
    # 检测是不是好友
    isFriend=22
# 消息状态
class ResultCode(Enum):
    notify    = -1
    success   = 0
    bad_pack  = 1
    user_exist = 2
    Login_error = 3
    room_members = 4

#维护一个字典，保存了所有在线的用户
_clientDict = {}

def sendToclientSocket(clientSocket,type, status, strData):
    # 编码
    data = strData.encode('gb2312')
    # 加密
    data = bytearray(data)
    for x in range(len(data)):
        data[x] ^= 5

    size = len(data)
    strFormat = 'iii%ds'%(size)
    rawData = struct.pack( strFormat ,type, status, size,data)
    try:
        clientSocket.send( rawData )
    except Exception as e:
        print(e)


def recvFromclientSocket(clientSocket):
    try:
        data = clientSocket.recv(8)
        if data == b'\x05\x00\x00\x00\x00\x00\x00\x00':
            raise Exception('账号退出')
        # 离开聊天室
        if data == b'\x06\x00\x00\x00\x00\x00\x00\x00':
           pass
        # 接收8个字节的头部.
        type, size = struct.unpack('ii', data)
        print('type=%d , size%d' % (type, size))

        # 接收后续的内容
        bodyData = clientSocket.recv(size)
        # 解密
        bodyData = bytearray(bodyData)
        for x in range(len(bodyData)):
            bodyData[x] ^= 5
        print('bodyData: %s' % (bodyData.decode('gb2312')))
        # 解码之后分割参数
        return type , bodyData.decode('gb2312').split('\t')
    except Exception as e:
        print(e)

class Handler():
    def __init__(self):
        #实例数据属性
        self.args = None
        self.socket=None

    def register(self):
        print('注册 参数: ' , self.args)
        # 注册的流程
        # 1. 检查账号有没有注册过.
        sql = "select user_id from user_info where user_name='%s';"%(self.args[0])
        # 执行sql语句
        _db.cursor.execute(sql)
        # 获取结果集,保存的是字典，键是字段，值是一条记录
        result = _db.cursor.fetchone()
        # 列表，每一个元素都是字典
        #s=_db.cursor.fetchall()
        if result != None:
            sendToclientSocket(self.socket,
                               Request.register.value,
                               ResultCode.user_exist.value,
                               "用户已存在")
            return
        # 2. 保存账号到数据库

        sql = "insert into user_info (user_name, user_password) VALUE('%s',md5('%s'));" \
              %(self.args[0],self.args[1])
        _db.cursor.execute(sql)
        _db.cursor.execute('commit;')# 提交到数据库
        sendToclientSocket(self.socket,
                           Request.register.value,
                           ResultCode.success.value,
                           "注册成功")
    def login(self):
        print('登陆 参数: ' , self.args)
        # 1. 查表,判断用户是否存在
        sql = "select * from user_info where user_name='%s' and user_password=md5('%s');"\
              %(self.args[0],self.args[1])
        _db.cursor.execute(sql)
        userInfo =_db.cursor.fetchone()
        if userInfo == None:
            sendToclientSocket(self.socket,
                               Request.login.value,
                               ResultCode.Login_error.value,
                               "用户名或密码错误")
            return
        elif userInfo["is_online"]==1:
            sendToclientSocket(self.socket,
                               Request.login.value,
                               ResultCode.Login_error.value,
                               "用户已登录")
            return
        # 修改数据
        # 1. 更改登录状态
        _db.cursor.execute("update user_Info set is_online =1 where user_name='%s';"%(self.args[0]))
        _db.cursor.execute("commit;")
        # 2. 发送响应消息
        sendToclientSocket(self.socket,
                           Request.login.value,
                           ResultCode.success.value,
                           "登陆成功")

        # 将已登陆的用户信息保存到字典
        # self.user = userInfo
        # 对象中的成员变量放在字典__dict__中
        # 将userInfo和__dict__字典合并
        self.__dict__.update(userInfo)
        _clientDict[self.user_name] = self
    def logout(self):
        del _clientDict[self.user_name]
        # 更改登录状态
        _db.cursor.execute("update user_Info set is_online =0 where user_name='%s';" % (self.args[0]))
        _db.cursor.execute("commit;")
        print("下线")
    def getUserInfo(self):
        _db.cursor.execute("select * from user_info where user_name=('%s');" % (self.args[0]))
        res = _db.cursor.fetchone()
        if res!={}:
            date=str(res["regist_date"])
            msg=res["user_name"]+"\t"+res["user_sex"]+"\t"+date
            sendToclientSocket(_clientDict[self.user_name].socket,
                               Request.getUserInfo.value,
                               ResultCode.notify.value,
                               msg)
    def updateUserInfo(self):
        _db.cursor.execute("update user_info set user_sex='%s' where user_name='%s';" % (self.args[0], self.user_name))
        _db.cursor.execute('commit;')
    def findAll(self):
        # 在数据库中查找，并发送返回结构
        _db.cursor.execute("select * from user_info where user_name=('%s');"%(self.args[0]))
        res=_db.cursor.fetchone()
        if res==None:
            sendToclientSocket(_clientDict[self.user_name].socket,
                               Request.findAll.value,
                               ResultCode.notify.value,
                               "没找到")
            return
        isonlien=str(res["is_online"])
        msg=res["user_name"]+" "+isonlien
        sendToclientSocket(_clientDict[self.user_name].socket,
                           Request.findAll.value,
                           ResultCode.notify.value,
                           msg)
    def addFriend(self):
        # 1. 先看看在不在线
        _db.cursor.execute("select is_online from user_info where user_name='%s';" % (self.args[1]))
        online = _db.cursor.fetchone()
        msg='%s 申请添加您为好友' % (self.args[0])
        if online==1:
            sendToclientSocket(_clientDict[self.args[1]].socket,
                               Request.getFriendList.value,
                               ResultCode.notify.value,
                               msg)
        else :
        # 2. 将消息发送到数据库msg_info
            _db.cursor.execute("insert into msg_info (user_name,msg) values ('%s','%s 申请添加您为好友');" % (self.args[1],self.args[0]))
            _db.cursor.execute("commit;")
    def deleteFriend(self):
        _db.cursor.execute("delete from friend_info where me='%s' and my_friend='%s'" % (self.user_name,self.args[0] ))
        _db.cursor.execute('commit;')
    def isFriend(self):
        _db.cursor.execute("select * from friend_info where me='%s' and my_friend='%s';" % (self.args[0],self.user_name))
        res = _db.cursor.fetchone()
        if res==None:
            sendToclientSocket(_clientDict[self.user_name].socket,
                               Request.isFriend.value,
                               ResultCode.notify.value,
                               "No")
        else:
            sendToclientSocket(_clientDict[self.user_name].socket,
                               Request.isFriend.value,
                               ResultCode.notify.value,
                               "Yes")

    # 验证消息
    def getFriendMsg(self):
        # 将数据库中所有该客户端的消息返回
        _db.cursor.execute("SELECT * FROM `msg_info` where user_name='%s';" % (self.args[0]))
        msg_info=_db.cursor.fetchall()
        if msg_info==():
            sendToclientSocket(_clientDict[self.user_name].socket,
                               Request.getFriendMsg.value,
                               ResultCode.notify.value,
                               "Null")
            return
        messages=""
        for msg in msg_info:
            messages=messages+","+msg["msg"]
        sendToclientSocket(_clientDict[self.user_name].socket,
                           Request.getFriendMsg.value,
                           ResultCode.notify.value,
                           messages)
    def agreeFriend(self):
        # 1. 向数据库添加好友记录
        _db.cursor.execute("insert into friend_info (me,my_friend) values ('%s','%s');" % (self.args[0], self.args[1]))
        _db.cursor.execute("commit;")
        _db.cursor.execute("insert into friend_info (me,my_friend) values ('%s','%s');" % (self.args[1], self.args[0]))
        _db.cursor.execute("commit;")
    def getFriendList(self):

        # 1. 遍历数据库,找到好友
        _db.cursor.execute("select my_friend from friend_info where me='%s';" % (self.args[0]))
        m_friend = _db.cursor.fetchall()

        if m_friend != ():

            friend_list=""
            for friend in m_friend:
                # 2. 查询数据库得到好友状态
                _db.cursor.execute("select is_online from user_info where user_name='%s';" % (friend["my_friend"]))
                online =_db.cursor.fetchone()
                onlinee=str(online["is_online"])
                friend_list = friend_list +","+friend["my_friend"]+" "+onlinee
            # 3. 发消息
            if friend_list != '':
                sendToclientSocket(_clientDict[self.user_name].socket,
                                   Request.getFriendList.value,
                                   ResultCode.notify.value,
                                   friend_list)
    def deleteMsg(self):
        _db.cursor.execute("delete from msg_info where msg='%s' and user_name='%s';" % (self.args[0],self.user_name))
        _db.cursor.execute('commit;')

    def createRoom(self):
        # 1. 到rooms_info注册窗口
        creator=self.user_name
        _db.cursor.execute("insert into rooms_info (room_name,room_count,room_creator) values ('%s',1,'%s');" % (self.args[0],creator))
        _db.cursor.execute("commit;")
        # 2. 更新room_info的信息
        _db.cursor.execute("insert into `room_info` (m_member,room_name,member_status) values ('%s','%s',1);" % (creator,self.args[0]))
        _db.cursor.execute("commit;")
        msg = self.args[0] + " 聊天室创建成功"
        # 3. 发送消息“XX创建成功”
        sendToclientSocket(_clientDict[self.user_name].socket,
                           Request.addRoom.value,
                           ResultCode.notify.value,
                           msg)
        # 3. 发送到所有的客户端更新成员列表
        sendToclientSocket(_clientDict[self.user_name].socket,
                           Request.addRoom.value,
                           ResultCode.room_members.value,
                           self.user_name)
    def addRoom(self):
        # 1. 更新room_info中的信息
        sql="insert into `room_info` (m_member,room_name,member_status) values ('%s','%s',0);" % (self.args[0],self.args[1])
        _db.cursor.execute(sql)
        _db.cursor.execute('commit;')
        # 房间人数+1
        _db.cursor.execute("update rooms_info set room_count=room_count+1 where room_name='%s';" % (self.args[1]))
        _db.cursor.execute('commit;')
        msg = self.args[0]+" 加入了聊天室"
        # 2. 发送广播消息“XX加入聊天室”
            # 判断如果在聊天室里就发送
        _db.cursor.execute("select m_member from room_info;")
        ress = _db.cursor.fetchall()
        for res in ress:
            sendToclientSocket(_clientDict[res["m_member"]].socket,
                               Request.addRoom.value,
                               ResultCode.notify.value,
                               msg)
       # 3. 遍历表中的所有记录，并发送到所有的客户端更新成员列表
        _db.cursor.execute("select m_member from `room_info`")
        rows=_db.cursor.fetchall()
        mem=""
        for row in rows:
            mem =mem+" "+row['m_member']
        _db.cursor.execute("select m_member from room_info;")
        ress = _db.cursor.fetchall()
        for res in ress:
            sendToclientSocket(_clientDict[res["m_member"]].socket,
                               Request.addRoom.value,
                               ResultCode.room_members.value,
                               mem)

    # 离开房间
    def leaveRoom(self):
        # 1. 删除成员数据库
        _db.cursor.execute("delete from `room_info` where m_member='%s' and room_name='%s';" % (self.user_name,self.args[0]))
        _db.cursor.execute('commit;')  # 提交到数据库
        _db.cursor.execute("update rooms_info set room_count=room_count-1 where room_name='%s';" % (self.args[0]))
        _db.cursor.execute('commit;')
        # 2. 发送广播消息
        msg=self.user_name+" 离开了聊天室"
        _db.cursor.execute("select m_member from room_info;")
        ress = _db.cursor.fetchall()
        if ress!=():
            for res in ress:
                if res["m_member"]==self.user_name:
                    continue
                sendToclientSocket(_clientDict[res["m_member"]].socket,
                                   Request.addRoom.value,
                                   ResultCode.notify.value,
                                   msg)
                # 3. 遍历表中的所有记录，并发送到所有的客户端更新成员列表
                _db.cursor.execute("select m_member from `room_info`")
                rows = _db.cursor.fetchall()
                mem = ""
                for row in rows:
                    mem = mem + " " + row['m_member']
                _db.cursor.execute("select m_member from room_info;")
                ress = _db.cursor.fetchall()
                for res in ress:
                    sendToclientSocket(_clientDict[res["m_member"]].socket,
                                       Request.addRoom.value,
                                       ResultCode.room_members.value,
                                       mem)
    def getRoomList(self):
        # 1. 查询数据库
        _db.cursor.execute("select room_name,room_count from `rooms_info`")
        rows = _db.cursor.fetchall()
        msg = ""
        if rows != []:
            for row in rows:
                roomCount=str(row["room_count"])
                msg = msg + " " + row["room_name"]+","+roomCount
        # 2. 返回消息
        sendToclientSocket(_clientDict[self.user_name].socket,
                           Request.getRoomList.value,
                           ResultCode.notify.value,
                           msg)
    def sendMultiMsg(self):
        # 给每一位聊天室内的成员发送消息
        msg = self.user_name + ": " + self.args[0]
        _db.cursor.execute("select m_member from room_info;")
        ress = _db.cursor.fetchall()
        if ress != ():
            for res in ress:
                if res["m_member"] == self.user_name:
                    continue
                sendToclientSocket( _clientDict[res["m_member"]].socket,
                                    Request.sendMultiMsg.value,
                                    ResultCode.notify.value,
                                    msg)
    # 更新房间信息表
    def updateRooms(self):
        _db.cursor.execute("delete from rooms_info where room_count=0;" )
        _db.cursor.execute('commit;')
        # 私聊
    def sendSingle(self):
        des=self.args[0]
        msg=self.args[1]
        sendToclientSocket(_clientDict[self.args[0]].socket,
                           Request.sendSingle.value,
                           ResultCode.notify.value,
                           msg)
    def saveChat(self):
        # 1. 向数据库保存数据
        _db.cursor.execute("insert into chat_info (chat_from,chat_to,chat_content) "
                           "VALUES ('%s','%s','%s');" % (self.args[0],self.args[1],self.args[2]))
        _db.cursor.execute('commit;')
    def checkRecord(self):
        # 1. 查询数据库
        _db.cursor.execute("select * from chat_info where (chat_from='%s' and chat_to='%s') "
                           "or (chat_to='%s' and chat_from='%s');" % (self.args[0],self.args[1],self.args[0],self.args[1]))
        rows = _db.cursor.fetchall()
        msg = ""
        if rows != []:
            for row in rows:
                chatFrom=str(row["chat_from"])
                chatTime = str(row["chat_time"])
                chatContent=str(row["chat_content"])
                msg = msg + "\n" + chatFrom + "\t" + chatContent+"\t"+chatTime
        # 2. 返回消息
        sendToclientSocket(_clientDict[self.user_name].socket,
                           Request.checkRecord.value,
                           ResultCode.notify.value,
                           msg)


class Server:
    def __init__(self, ip , port):
        # 1. 创建一个套接字
        self.socket = socket.socket()
        # 2. 绑定ip和端口
        self.socket.bind( (ip,port))
        # 3. 监听
        self.socket.listen( socket.SOMAXCONN )

    def recvclientSocketData(self,handler):
        type,args = None,None

        while  True:
            type , args = None,None
            try:
                # 接收数据
                type , args = recvFromclientSocket(handler.socket)
            except Exception as e:
                print(e)
                #print(handler.user_name +"断开了服务器连接")
                print("断开")
                for k,v in _clientDict:
                    print(k)
                #del _clientDict[ handler.user_name ]
                break
            try:
                # 将一个整型值转换成枚举类型
                # 如果这个整型值在枚举类型中没有定义
                # 就会抛出异常
                type = Request(type)
            except:
                sendToclientSocket(handler.socket,
                                   type,
                                   ResultCode.bad_pack.value ,
                                   "无效的请求")
            # 1. 获取函数
            func = getattr(handler , type.name)
            # 2. 设置属性, 将客户端传来的参数设置成类对象的一个成员变量
            handler.args = args
            func()

    def run(self):
        # 4. 循环接收客户端连接
        while True:
            clientSocket , addr = self.socket.accept()
            h = Handler()
            h.socket = clientSocket
            h.addr = addr
            print('接入新客户端: ' , addr)
            threading.Thread(
                target=self.recvclientSocketData,
                args=(h,)
            ).start()


if __name__ == '__main__':
    s = Server('127.0.0.1',10086)
    print("服务器开始运行")
    s.run()