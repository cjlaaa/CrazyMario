#ifndef _CM_PUBLIC_DEFINE_H_
#define _CM_PUBLIC_DEFINE_H_
#include "cocos2d.h"
USING_NS_CC;
#define SCREEN_WIDTH CCDirector::sharedDirector()->getWinSize().width
#define SCREEN_HEIGHT CCDirector::sharedDirector()->getWinSize().height

enum 
{
	enZOrderBack,
	enZOrderMiddle,
	enZOrderFront,
};

enum enTileType
{
	enTileTypeLand,   // 地板
	enTileTypeBlock,  // 金币等小块块
	enTileTypePipe,   // 管道
	enTileTypeTrap,   // 陷阱
	enTileTypeCoin,   // 金币
	enTileTypeNone,  // 无碰撞物
	enTileTypeFlagpole,  // 旗杆
};

class CMReceiver
{
public:
	virtual void OnMsgReceive(int enMsg,void* pData,int nSize) = 0;
};

class CMSender
{
protected:
	CMReceiver* m_pReceiver;
public:
	void SetReceiver(CMReceiver* pReceiver)
	{
		m_pReceiver=pReceiver;
	}
	void SendMsg(int enMsg,void* pData = NULL,int nSize = 0)
	{
		m_pReceiver->OnMsgReceive(enMsg,pData,nSize);
	}
};

enum 
{
	enMsgCoinCollision,	//金币碰撞
	enMsgDead,
};

//金币碰撞消息
class CMItemCoin;
struct MsgForCoinCollision
{
	CMItemCoin* pCoin;
};

#endif