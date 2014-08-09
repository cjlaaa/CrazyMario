#ifndef _CM_GAME_MAP_H_
#define _CM_GAME_MAP_H_

#include "CMPublicDefine.h"
#include "CMMario.h"
#include "CMItem.h"
#include "CMMonster.h"
#include "CMGameScene.h"

class CMGameMap : public cocos2d::CCTMXTiledMap ,public CMReceiver , public CMSender
{
protected:
	enum 
	{
		enTagMario = 998,				//通过这个Tag查找Mario
		enTagAction,					//动画
		enTagCoinJumpAction,			//金币跳动的动画
		enTagJumpCoin,					//跳出的金币
	};
protected:
	CCArray	*m_pArrayItems;				//金币数组
	CCArray *m_pArrayMonsters;			//怪物数组
	CCArray *m_pArrayBlocks;			//砖块数组
	CCArray *m_pArrayFireBall;			//火球数组

	float m_fMapMove;					//地图偏移量
	float m_fDropSpeedPlus;				//掉落速度
	float m_fJumpSpeed;					//跳跃速度
	bool  m_bNeedResetStage;			//是否需要重置关卡	

	bool m_bIsLeftKeyDown;				//左键是否按下
	bool m_bIsRightKeyDown;				//右键是否按下
	bool m_bIsJumpKeyDown;				//跳跃键是否按下
	bool m_bIsFireKeyDown;				//火球键是否按下
	bool m_bIsHeroDead;					//是否死亡
public:
	static CMGameMap* CreateGameMap(const char* pFileName,CMReceiver* pReceiver,enumMarioLevel &eMarioLevel);

	//返回地图指定位置上的图块
	CCSprite*	TileMapLayerPosToTileSprite(CCPoint TileMapLayerPos);			
	//通过地图块坐标返回地图块类型
	enumTileType	TileMapPosToTileType(CCPoint HeroPos);					
	//地图块坐标转换为地图层坐标
	CCPoint		TileMapPosToTileMapLayerPos(CCPoint TileMapPos);						
	//地图层坐标转换为地图块坐标
	CCPoint		TileMapLayerPosToTileMapPos(CCPoint TileMapLayerPos);
	//获得Mario当前等级
	enumMarioLevel GetMarioLevel();
	virtual void OnCallPerFrame(float dt);

protected:
	virtual bool Init(CMReceiver* pReceiver,enumMarioLevel &eMarioLevel);
	
	void		 onExit();

	//Mario碰撞
	void	MarioMove(float fT);
	//顶砖块
	void	HitBlock(CCPoint TileMapLayerPos);
	//删除砖块
	void	DeleteBlock();
	//通关动画播放完毕回调
	void	PassLevel();
	//跳出金币消失
	void	OnSubJumpCoinDisappear();
	//消息处理
protected:
	virtual void OnMsgReceive( int enMsg,void* pData,int nSize );
	
	void OnSubMsgFire();								//发火球消息处理

	void OnSubMsgBlockRemove(void *pData ,int nSize);	//顶碎砖块消息处理

	void OnSubMsgItemRemove(void *pData ,int nSize);	//[道具移除]消息处理

	void OnSubMsgMarioLevelUp();						//[马里奥长大]消息处理

	void OnSubMsgMonsterDisappear(void *pData,int nSize);//[怪物消失]消息处理

	void OnSubMsgMushroomsStamp(void *pData,int nSize);	//[被马里奥踩踏蘑菇]消息处理

	void OnSubMsgTortoiseStamp(void *pData,int nSize);	//[被马里奥踩踏乌龟]消息处理
	
	void OnSubMsgBlockBoxHitted(void *pData,int nSize);	//[砖块被顶]消息处理

	void OnSubMsgMarioBeHurt();							//马里奥被伤害的消息处理

	void OnSubMsgGameOver();							//游戏结束消息处理

	void OnSubMsgEatCoin();								//吃金币

	void OnSubMsgStopCtrl();							//停止更新

public:
	//设置按键状态
	void SetLeftKeyDown(bool bIsKeyDown);
	void SetRightKeyDown(bool bIsKeyDown);
	void SetJumpKeyDown(bool bIsKeyDown);
	void SetFireKeyDown(bool bIsKeyDown);
};


//win32平台按键控制部分
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)
#endif

enum EKEY_CODE
{ 
	KEY_KEY_A            = 0x41,  // A key  
	KEY_KEY_D            = 0x44,  // D key   
	KEY_KEY_J            = 0x4A,  // J key  
	KEY_KEY_K            = 0x4B,  // K key  
	KEY_KEY_S            = 0x53,  // S key  
	KEY_KEY_W            = 0x57,  // W key  

	KEY_KEY_CODES_COUNT  = 0xFF // this is not a key, but the amount of keycodes there are.
};

#endif