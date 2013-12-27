#ifndef _CM_PUBLIC_DEFINE_H_
#define _CM_PUBLIC_DEFINE_H_
#include "cocos2d.h"
USING_NS_CC;
#define SCREEN_WIDTH CCDirector::sharedDirector()->getWinSize().width
#define SCREEN_HEIGHT CCDirector::sharedDirector()->getWinSize().height

#define CONTROL_UI_HEIGHT			96					//控制UI高度
#define TILE_MAP_VERTICAL			13					//地图瓦片竖直块数
#define JUMP_START_SPEED			8					//跳跃起始速度
#define JUMP_SPEED_MINUS			0.3					//跳跃递减速度
#define DROP_SPEED_PLUS				0.098				//掉落加速度
#define MOVE_SPEED					2					//移动速度
#define MONSTER_ACTIVE_DISTANCE		SCREEN_WIDTH-100		//怪物激活距离
#define COLLISION_POS_ADJUSTMENT	3					//跳跃与坠落的碰撞检测点调整

enum 
{
	enZOrderBack,
	enZOrderMiddle,
	enZOrderFront,
};

//瓦片类型
enum enumTileType
{
	enTileTypeLand,   // 地板
	enTileTypeBlock,  // 金币等小块块
	enTileTypePipe,   // 管道
	enTileTypeTrap,   // 陷阱
	enTileTypeCoin,   // 金币
	enTileTypeNone,  // 无碰撞物
	enTileTypeFlagpole,  // 旗杆
};

//砖块类型
enum enumBlockType
{
	enBlockTypeBox = 1,			//宝箱砖块
	enBlockTypeNormal,			//普通砖块，可顶碎
	enBlockTypeAddLife,			//隐藏加人砖块
	enBlockTypeJustBlock,		//没有任何互动的砖块
};

//Mario状态
enum enumMarioStatus
{
	enMarioStatusSmall,
	enMarioStatusBig,
	enMarioStatusSuper,
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
	enMsgCoinCollision,		//金币碰撞
	enMsgDead,				//Mario死亡
	enMsgMonsterDisappear,	//怪物离开地图消失数据
	enMsgStamp,				//被踩死
};

//金币碰撞消息数据
class CMItemBasic;
struct MsgForCoinCollision
{
	CMItemBasic* pCoin;
};

//怪物离开地图消失数据
class CMMonsterBasic;
struct MsgForMonsterDisappear
{
	CMMonsterBasic* pMonster;
};

#endif