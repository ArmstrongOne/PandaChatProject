#pragma once
#include <stdint.h>

//包是否压缩过
enum
{
	PACKAGE_UNCOMPRESSED,
	PACKAGE_COMPRESSED
};

struct msg
{
	char     compressflag; //压缩标志，如果为1，则启用压缩，反之不启用压缩
	int32_t  originsize;   //包体压缩前大小
	int32_t  compresssize; //包体压缩后大小
	char     reserved[16];
};

enum Regist_Return_Msg
{
	RegisteredAlready=101, //已经注册
	RegisteredFailed=100,  //注册失败
	RegisteredSuccess=0    //注册成功
};

enum msg_type
{
	msg_type_unknown,
	msg_type_heartbeat = 1000,      //心跳包
	msg_type_register,              //注册
	msg_type_login,                 //登陆
	msg_type_getofriendlist,        //获取好友列表
	msg_type_finduser,              //查找用户
	msg_type_operatefriend,         //添加、删除等好友操作
	msg_type_userstatuschange,      //用户信息改变通知
	msg_type_updateuserinfo,        //更新用户信息
	msg_type_modifypassword,        //修改登陆密码
	msg_type_creategroup,           //创建群组
	msg_type_getgroupmembers,       //获取群组成员列表
	msg_type_chat = 1100,           //单聊消息
	msg_type_multichat,             //群发消息
	msg_type_kickuser,              //被踢下线
	msg_type_remotedesktop,         //远程桌面
	msg_type_updateteaminfo,        //更新用户好友分组信息
	msg_type_modifyfriendmarkname,  //更新好友备注信息
	msg_type_movefriendtootherteam, //移动好友至

#ifdef FXN_VERSION
//定制协议
	msg_type_uploaddeviceinfo = 2000, //上传设备信息
	msg_type_downloaddeviceinfo = 2001  //拉取设备信息
#endif
};

//格式大全
/**
*  心跳包协议
**/
/*
cmd = 1000, seq = 0
cmd = 1000, seq = 0
**/


/*注册协议

发送:
cmd = 1001, seq = 0,  {"username": "13917043329", "nickname": "balloon", "password": "123"}

返回:
cmd = 1001, seq = 0,  {"code": 0, "msg": "ok"}                    注册成功
cmd = 1001, seq = 0,  {"code": 101, "msg": "registered already"}  已经注册
cmd = 1001, seq = 0,  {"code": 100, "msg": "registered failed"}   注册失败

*/

/**
*  登录协议
**/
/*
//status: 在线状态 0离线 1在线 2忙碌 3离开 4隐身
//clienttype: 客户端类型,pc=1, android=2, ios=3
cmd = 1002, seq = 0, {"username": "13917043329", "password": "123", "clienttype": 1, "status": 1}
cmd = 1002, seq = 0, {"code": 0, "msg": "ok", "userid": 8, "username": "13917043320", "nickname": "zhangyl",
"facetype": 0, "customface":"文件md5", "gender":0, "birthday":19891208, "signature":"哈哈，终于成功了",
"address":"上海市东方路3261号", "phonenumber":"021-389456", "mail":"balloonwj@qq.com"}
**/

/**
* 获取用户列表
**/
/*
cmd = 1003, seq = 0
cmd = 1003, seq = 0,
{
"code": 0,
"msg": "ok",
"userinfo": [
{
"teamname": "我的好友",
"members": [
{
"userid": 2,
"username": "qqq",
"nickname": "qqq123",
"facetype": 0,
"customface": "466649b507cdf7443c4e88ba44611f0c",
"gender": 1,
"birthday": 19900101,
"signature": "生活需要很多的力气呀。xx",
"address": "",
"phonenumber": "",
"mail": "",
"clienttype": 1,
"status": 1,
"markname": "qq测试号"
},
{
"userid": 3,
"username": "hujing",
"nickname": "hujingx",
"facetype": 0,
"customface": "",
"gender": 0,
"birthday": 19900101,
"signature": "",
"address": "",
"phonenumber": "",
"mail": "",
"clienttype": 1,
"status": 0
}
]
},
{
"teamname": "我的同学",
"members": [
{
"userid": 4,
"username": "qqq",
"nickname": "qqq123",
"facetype": 0,
"customface": "466649b507cdf7443c4e88ba44611f0c",
"gender": 1,
"birthday": 19900101,
"signature": "生活需要很多的力气呀。xx",
"address": "",
"phonenumber": "",
"mail": "",
"clienttype": 1,
"status": 1
},
{
"userid": 5,
"username": "hujing",
"nickname": "hujingx",
"facetype": 0,
"customface": "",
"gender": 0,
"birthday": 19900101,
"signature": "",
"address": "",
"phonenumber": "",
"mail": "",
"clienttype": 1,
"status": 0,
"markname": "qq测试号"
}
]
}
]
}

**/

/**
* 查找用户
**/
/*
//type查找类型 0所有， 1查找用户 2查找群
cmd = 1004, seq = 0, {"type": 1, "username": "zhangyl"}
cmd = 1004, seq = 0, { "code": 0, "msg": "ok", "userinfo": [{"userid": 2, "username": "qqq", "nickname": "qqq123", "facetype":0}] }
**/

/**
*  操作好友，包括加好友、删除好友
**/
/*
//type为1发出加好友申请 2 收到加好友请求(仅客户端使用) 3应答加好友 4删除好友请求 5应答删除好友
//当type=3时，accept是必须字段，0对方拒绝，1对方接受
cmd = 1005, seq = 0, {"userid": 9, "type": 1}
cmd = 1005, seq = 0, {"userid": 9, "type": 2, "username": "xxx"}
cmd = 1005, seq = 0, {"userid": 9, "type": 3, "username": "xxx", "accept": 1}

//发送
cmd = 1005, seq = 0, {"userid": 9, "type": 4}
//应答
cmd = 1005, seq = 0, {"userid": 9, "type": 5, "username": "xxx"}
**/

/**
*  用户状态改变
**/
/*
//type 1用户在线状态改变 2离线 3用户签名、头像、昵称发生变化
//onlinestatus取值： 0离线 1上线 2隐身 3忙碌 4离开 5移动版在线 6移动版下线 7手机和电脑同时在线,
cmd = 1006, seq = 0, {"type": 1, "onlinestatus": 1, "clienttype": 1},  targetid(状态改变的用户信息)
cmd = 1006, seq = 0, {"type": 3}, targetid(状态改变的用户信息)
**/

/**
*  更新用户信息
**/
/*
cmd = 1007, seq = 0, 用户信息: {"nickname": "xx", "facetype": 0,"customface":"文件md5", "gender":0, "birthday":19891208, "signature":"哈哈，终于成功了",
"address":"上海市东方路3261号", "phonenumber":"021-389456", "mail":"balloonwj@qq.com"}
cmd = 1007, seq = 0, {"code": 0, "msg": "ok", "userid": 9, "username": "xxxx", "nickname": "xx", "facetype": 0,
"customface":"文件md5", "gender":0, "birthday":19891208, "signature":"哈哈，终于成功了",
"address":"上海市东方路3261号", "phonenumber":"021-389456", "mail":"balloonwj@qq.com"}
**/

/**
*  修改密码
**/
/*
cmd = 1008, seq = 0, {"oldpassword": "xxx", "newpassword": "yyy"}
cmd = 1008, seq = 0, {"code":0, "msg": "ok"}
**/

/**
*  创建群
**/
/*
cmd = 1009, seq = 0, {"groupname": "我的群名称", "type": 0}
cmd = 1009, seq = 0, {"code":0, "msg": "ok", "groupid": 12345678, "groupname": "我的群名称"}, 用户id和群id都是4字节整型，32位，群id的高位第七位为1
**/

/**
*  获取群成员
**/
/*
cmd = 1010, seq = 0, {"groupid": 群id}
cmd = 1010, seq = 0, {"code":0, "msg": "ok", "groupid": 12345678,
"members": [{"userid": 1, "username": xxxx, "nickname": yyyy, "facetype": 1, "customface": "ddd", "status": 1, "clienttype": 1},
{"userid": 1, "username": xxxx, "nickname": yyyy, "facetype": 1, "customface": "ddd", "status": 1, "clienttype": 1},
{"userid": 1, "username": xxxx, "nickname": yyyy, "facetype": 1, "customface": "ddd", "status": 1, "clienttype": 1}]}
**/

/**
*  聊天协议
**/
/*
cmd = 1100, seq = 0, data:聊天内容 targetid(消息接受者)
cmd = 1100, seq = 0, data:聊天内容 senderid(消息接受者), targetid(消息发送者)
**/

/**
*  群发协议
**/
/*
cmd = 1101, seq = 0, data:聊天内容 targetid(消息接受者)
cmd = 1101, seq = 0, data:聊天内容, {"targets": [1, 2, 3]}(消息接收者)
**/

/**
*  被踢下线
**/
/*
cmd = 1102, seq = 0, data:
**/

/**
* 屏幕截屏数据
**/
/*
cmd = 1103, seq = 0, string: 位图头部信息， 位图信息，targetId
**/

/**
* 更新用户好友分组信息
**/
/*
客户端请求： cmd = 1104, seq = 0, data(必填，空字符串), int(操作类型：0增 1删 2改), string(新的分组名称), string(旧的分组名)
服务器应答： cmd = 1003, seq = 0,
**/

/**
*  更新好友备注信息
*/
/*
客户端请求： cmd = 1105, seq = 0, data: 组装后的json, friendid, newmarkname
服务器应答： cmd = 1003, seq = 0, data: {"code": 0, "msg": ok}
*/

/**
*   移动好友至其他分组
*/
/*
客户端请求： cmd = 1105, seq = 0, data(必填，空字符串), friendid(int32, 操作的好友id), newteamname(string, 新分组名), oldteamname(string, 旧分组名)
服务器应答： cmd = 1003, seq = 0, data: {"code": 0, "msg": ok}
*/



////////////////////////
//定制信息
////////////////////////
/* 上传设备信息
cmd = 2000, seq = 0, data(设备信息json), 设备id(int32)，信息类型classtype(int32), 上传时间(int64， UTC时间)
cmd = 2000, seq = 0, data: {"code": 0, "msg": "ok"}
**/

/*
cmd = 2001, seq = 0, data(空)，设备id(int32)，信息类型classtype(int32), 上传时间(int64, UTC时间)
cmd = 2001, seq = 0, data: {具体的设备信息json}
**/












