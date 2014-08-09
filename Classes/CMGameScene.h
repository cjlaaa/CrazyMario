#ifndef _CM_GAME_SCENE_H_
#define _CM_GAME_SCENE_H_

#include "CMPublicDefine.h"

#define MARIO_LIFE 3

class CMGameScene : public cocos2d::CCLayer,public CMReceiver
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* CreateGameScene();

private:
	int					m_nCurStage;				//当前关卡
	int					m_nCurMarioLife;			//当前生命数
	int					m_nCurCoinNum;				//当前金币数
	enumMarioLevel		m_eCurLevel;				//当前等级
	bool				m_bNeedResetStage;			//是否需要重置关卡		
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();     
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(CMGameScene);

	void OnMsgReceive( int enMsg,void* pData,int nSize );
	void OnSubMsgPassLevel();								//通关消息处理
	void OnSubMsgMarioDead();								//游戏结束消息处理
	void OnSubMsgEatCoin();									//吃金币消息处理
	void OnSubMsgStartGame();								//开始游戏消息处理
	void OnCallPerFrame(float dt);

	void InitControlUI();
	//按钮回调
	void OnMenuLeftKeyDownCallBack(CCObject *pSender );
	void OnMenuLeftKeyUpCallBack(CCObject* pSender, CCControlEvent event);
	void OnMenuRightKeyDownCallBack(CCObject *pSender );
	void OnMenuRightKeyUpCallBack(CCObject* pSender, CCControlEvent event);
	void OnMenuJumpKeyDownCallBack(CCObject *pSender );
	void OnMenuJumpKeyUpCallBack(CCObject* pSender, CCControlEvent event);
	void OnMenuFireKeyDownCallBack(CCObject *pSender );
	void OnMenuFireKeyUpCallBack(CCObject* pSender, CCControlEvent event);
	void PrintErrorControlMsg();

	enum 
	{
		enTagMap,
		enTagMenu,
		enTagUI,
		enTagCoin,
		enTagCoinNum,
		enTagLife,
		enTagLifeNum,
	};

	enum 
	{
		enTagLeftKey,
		enTagRightKey,
		enTagJumpKey,
		enTagFireKey,
	};

	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
};




#endif // __HELLOWORLD_SCENE_H__
