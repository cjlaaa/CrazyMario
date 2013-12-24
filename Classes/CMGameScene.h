#ifndef _CM_GAME_SCENE_H_
#define _CM_GAME_SCENE_H_

#include "CMPublicDefine.h"

class CMGameScene : public cocos2d::CCLayer,public CMReceiver
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* CreateGameScene();

private:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();     
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(CMGameScene);

	void OnMsgReceive( int enMsg,void* pData,int nSize );
	void OnCallPerFrame(float dt);

	void InitControlUI();
	void OnMenuLeftKeyCallBack(CCObject *pSender);
	void OnMenuRightKeyCallBack(CCObject *pSender);
	void OnMenuJumpKeyCallBack(CCObject *pSender);
	void OnMenuFireKeyCallBack(CCObject *pSender);

	enum 
	{
		enTagHero,
		enTagMap,
		enTagMenu,
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
