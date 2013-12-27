#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "CMPublicDefine.h"
#include "CMGameScene.h"

#define SCREEN_WIDTH		CCDirector::sharedDirector()->getWinSize().width	//屏幕宽(指定分辨率)
#define SCREEN_HEIGHT		CCDirector::sharedDirector()->getWinSize().height	//屏幕高(指定分辨率)
#define SCREEN_CENTER		ccp(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f)			//屏幕中心
#define SCREEN_SIZE			CCSizeMake(SCREEN_WIDTH,SCREEN_HEIGHT)				//屏幕尺寸
#define NAME_LEN			128													//字符串长度
#define _TR_ABS(x)			(    (x)>=0 ? (x) : -(x)   )						//绝对值

//菜单层
class CMMenuScene : public cocos2d::CCLayerColor
{
private:
	bool                    m_bIsBtnClicked;        //是否点击上按钮

public:
	static CCScene* CreateMenuScene();

	//触摸函数
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

protected:
	virtual bool init(); 
	CREATE_FUNC(CMMenuScene);

	virtual void onEnter();
	virtual void onExit();

	//按钮回调：开始游戏
	void OnStartCallBack(CCObject *pSender);

	//按钮回调：退出游戏
	void OnQuitCallBack(CCObject *pSender);

	//按钮回调：关于
	void OnAboutCallBack(CCObject *pSender);
protected:
	//标签
	enum
	{
		enTagMenu=100,	
	};
};

//==========================================
//关于层
class CMAboutLayer : public cocos2d::CCLayerColor
{
private:
	bool                    m_bIsBtnClicked;        //是否点击上按钮

public:
	static CMAboutLayer* CreateAboutLayer();

	//触摸函数
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

protected:
	virtual bool init(); 

	virtual void onEnter();
	virtual void onExit();

	//按钮回调：返回菜单
	void OnBackMenuCallBack(CCObject *pSender);

protected:
	//标签
	enum
	{
		enTagMenu=100,	
	};
};


#endif  




