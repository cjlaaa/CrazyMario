#include "CMGameScene.h"
#include "CMGameMap.h"
#include "CMMario.h"

CCScene* CMGameScene::CreateGameScene()
{
	do 
	{
		//建立场景
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		//建立层
		CMGameScene* pLayer = CMGameScene::create();
		CC_BREAK_IF(pLayer==NULL);

		//将层加入场景
		pScene->addChild(pLayer);
		return pScene;
	} while (false);
	CCLog("Fun GameScene::CreateScene Error!");
	return NULL;
}

// on "init" you need to initialize your instance
bool CMGameScene::init()
{
	do 
	{
		//super init first
		if (CCLayer::init()==NULL)
		{
			return false;
		}

		CMGameMap* pGameMap = CMGameMap::CreateGameMap("MarioMap1.tmx");
		CC_BREAK_IF(pGameMap==NULL);
		pGameMap->setPosition(ccp(0,96));
		addChild(pGameMap,enZOrderBack,enTagMap);
		
		//注册Update函数
		this->schedule(schedule_selector(CMGameScene::OnCallPerFrame));



		InitControlUI();

		return true;
	} while (false);
	CCLog("Fun CMGameScene::init Error!");
	return false;
}


void CMGameScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

void CMGameScene::OnMsgReceive( int enMsg,void* pData,int nSize )
{

}

void CMGameScene::OnCallPerFrame(float dt)
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		pMap->OnCallPerFrame(dt);
		
		//CCLog("TileType = %d",pMap->HeroPosToTileType(pHero->getPosition()));
		//CCLog("HeroPosX=%f	HeroPosY=%f",pHero->getPositionX(),pHero->getPositionY());
		return;
	} while (false);
	CCLog("fun CMGameScene::Update Error!");
}

void CMGameScene::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{

}

void CMGameScene::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{

}

void CMGameScene::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{

}

void CMGameScene::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{

}

void CMGameScene::InitControlUI()
{
	do 
	{
		CCSprite* pControlUIBg = CCSprite::create("controlUI.png");
		CC_BREAK_IF(pControlUIBg==NULL);
		pControlUIBg->setAnchorPoint(ccp(0, 0));
		pControlUIBg->setPosition(ccp(0,0));
		this->addChild(pControlUIBg);

		CCMenu* pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(ccp(0,0));
		addChild(pMenu,enZOrderFront,enTagMenu);

		CCMenuItemImage *pLeftKey = CCMenuItemImage::create(
			"AB_normal.png",
			"AB_select.png",
			this, 
			menu_selector(CMGameScene::OnMenuLeftKeyCallBack));
		pLeftKey->setPosition(ccp(40,48));
		pMenu->addChild(pLeftKey,enZOrderFront,enTagLeftKey);

		CCMenuItemImage *pRightKey = CCMenuItemImage::create(
			"AB_normal.png",
			"AB_select.png",
			this, 
			menu_selector(CMGameScene::OnMenuRightKeyCallBack));
		pRightKey->setPosition(ccp(128,48));
		pMenu->addChild(pRightKey,enZOrderFront,enTagLeftKey);

		CCMenuItemImage *pJumpKey = CCMenuItemImage::create(
			"AB_normal.png",
			"AB_select.png",
			this, 
			menu_selector(CMGameScene::OnMenuJumpKeyCallBack));
		pJumpKey->setPosition(ccp(432,35));
		pMenu->addChild(pJumpKey,enZOrderFront,enTagLeftKey);

		CCMenuItemImage *pFireKey = CCMenuItemImage::create(
			"AB_normal.png",
			"AB_select.png",
			this, 
			menu_selector(CMGameScene::OnMenuFireKeyCallBack));
		pFireKey->setPosition(ccp(353,35));
		pMenu->addChild(pFireKey,enZOrderFront,enTagLeftKey);

		return;
	} while (false);
	CCLog("Fun CMGameScene::InitControlUI Error!");
}

void CMGameScene::OnMenuLeftKeyCallBack( CCObject *pSender )
{
	CCLog("Left");
}

void CMGameScene::OnMenuRightKeyCallBack( CCObject *pSender )
{
	CCLog("Right");
}

void CMGameScene::OnMenuJumpKeyCallBack( CCObject *pSender )
{
	CCLog("Jump");
}

void CMGameScene::OnMenuFireKeyCallBack( CCObject *pSender )
{
	CCLog("Fire");
}
