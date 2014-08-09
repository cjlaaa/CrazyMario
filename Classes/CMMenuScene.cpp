#include "CMMenuScene.h"


CMMenuScene* CMMenuScene::CreateMenuScene()
{
	do 
	{
		CMMenuScene* pLayer = new CMMenuScene;
		if (pLayer && pLayer->init())
		{
			pLayer->autorelease();
			return pLayer;
		}
		delete pLayer;
	} while (false);
	CCLog("Fun CMMenuScene::CreateMenuScene Error!");
	return NULL;
}

bool CMMenuScene::init()
{
	do 
	{
		CC_BREAK_IF(CCLayerColor::initWithColor(ccc4(0,50,0,50))==false);
		m_bIsBtnClicked = false;

		//添加背景
		CCSprite* pPauseLayerBG = CCSprite::create("menuscene/background.png");
		CC_BREAK_IF(pPauseLayerBG==NULL);
		pPauseLayerBG->setPosition(ccp(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 + 58));
		this->addChild(pPauseLayerBG,enZOrderMiddle);

		//添加菜单
		CCMenu * pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu,enZOrderFront,enTagMenu);

		//添加按钮
		CCMenuItemImage * pStartButton = CCMenuItemImage::create("menuscene/startgame_normal.png","menuscene/startgame_select.png",this,menu_selector(CMMenuScene::OnStartCallBack));
		CC_BREAK_IF(pStartButton==NULL);
		pStartButton->setPosition(ccp(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 - 28));
		pMenu->addChild(pStartButton,enZOrderFront);

		//添加按钮
		CCMenuItemImage * pQuitButton = CCMenuItemImage::create("menuscene/quitgame_normal.png","menuscene/quitgame_select.png",this,menu_selector(CMMenuScene::OnQuitCallBack));
		CC_BREAK_IF(pQuitButton==NULL);
		pQuitButton->setPosition(ccp(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 - 78));
		pMenu->addChild(pQuitButton,enZOrderFront);

		//添加按钮
		CCMenuItemImage * pAboutButton = CCMenuItemImage::create("menuscene/about_normal.png","menuscene/about_select.png",this,menu_selector(CMMenuScene::OnAboutCallBack));
		CC_BREAK_IF(pAboutButton==NULL);
		pAboutButton->setPosition(ccp(SCREEN_WIDTH - 50,20));
		pMenu->addChild(pAboutButton,enZOrderFront);

		return true;
	} while (false);
	CCLog("Fun CSMMenuSceneLayer::init Error!");
	return false;
}

void CMMenuScene::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

void CMMenuScene::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

//触摸：开始
bool CMMenuScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//获得菜单
		CCMenu * pMenu = dynamic_cast<CCMenu *>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenu==NULL);
		m_bIsBtnClicked = pMenu->ccTouchBegan(pTouch, pEvent);

		return true;
	} while (false);
	CCLog("Fun CSMMenuSceneLayer::ccTouchBegan Error!");
	return false;
}

//触摸：移动
void CMMenuScene::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//获得菜单
		CCMenu * pMenu = dynamic_cast<CCMenu *>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenu==NULL);
		if(m_bIsBtnClicked){
			pMenu->ccTouchMoved(pTouch, pEvent);
		}

		return;
	} while (false);
	CCLog("Fun CSMMenuSceneLayer::ccTouchMoved Error!");
}

//触摸：结束
void CMMenuScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//获得菜单
		CCMenu * pMenu = dynamic_cast<CCMenu *>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenu==NULL);
		if(m_bIsBtnClicked){
			pMenu->ccTouchEnded(pTouch, pEvent);
		}

		return;
	} while (false);
	CCLog("Fun CSMMenuSceneLayer::ccTouchEnded Error!");
}

void CMMenuScene::OnStartCallBack( CCObject *pSender )
{
	//将父节点设置为接受者并发送游戏开始消息
	CMReceiver* pReceiver = dynamic_cast<CMReceiver*>(getParent());
	if (pReceiver!=NULL)
	{
		SetReceiver(pReceiver);
		SendMsg(enMsgStartGame);
	}
	
}

void CMMenuScene::OnQuitCallBack( CCObject *pSender )
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

void CMMenuScene::OnAboutCallBack( CCObject *pSender )
{
	do 
	{
		CMAboutLayer* pLayer = CMAboutLayer::CreateAboutLayer();
		CC_BREAK_IF(pLayer==NULL);
		addChild(pLayer,enZOrderFront);
		return;
	} while (false);
	CCLog("Fun CSMMenuSceneLayer::OnAboutCallBack Error!");
}

//======================================
CMAboutLayer* CMAboutLayer::CreateAboutLayer()
{
	do 
	{
		CMAboutLayer * pLayer = new  CMAboutLayer();
		CC_BREAK_IF(pLayer==NULL);
		CC_BREAK_IF(pLayer->init()==false);
		pLayer->autorelease();
		return pLayer;
	} while (false);
	CCLog("Fun CSMAboutLayer::CreateAboutLayer Error!");
	return NULL;
}

bool CMAboutLayer::init()
{
	do 
	{
		CC_BREAK_IF(CCLayerColor::initWithColor(ccc4(0,0,0,255))==false);
		m_bIsBtnClicked = false;

		for(int i = 0;i < 40;i++){
			//添加背景
			CCSprite* pPauseLayerBG = CCSprite::create("menuscene/bglabel.png");
			CC_BREAK_IF(pPauseLayerBG==NULL);
			pPauseLayerBG->setPosition(ccp(8 + i*16,8));
			this->addChild(pPauseLayerBG,enZOrderMiddle);
		}
		for(int i = 0;i < 40;i++){
			//添加背景
			CCSprite* pPauseLayerBG = CCSprite::create("menuscene/bglabel.png");
			CC_BREAK_IF(pPauseLayerBG==NULL);
			pPauseLayerBG->setPosition(ccp(8 + i*16,SCREEN_HEIGHT - 8));
			this->addChild(pPauseLayerBG,enZOrderMiddle);
		}
		
		CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("menuscene/aboutinfo.xml");
		CC_BREAK_IF(pDicLang==NULL);

		CCString *pStringRule =dynamic_cast<CCString*>(pDicLang->objectForKey("producer_name"));
		CC_BREAK_IF(pStringRule==NULL);

		CCLabelTTF* pLabel = CCLabelTTF::create(pStringRule->m_sString.c_str(), "Arial", 24);
		pLabel->setPosition(SCREEN_CENTER);
		this->addChild(pLabel, 1);

		//添加菜单
		CCMenu * pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu,enZOrderFront,enTagMenu);

		//添加按钮
		CCMenuItemImage * pStartButton = CCMenuItemImage::create("menuscene/back_normal.png","menuscene/back_select.png",this,menu_selector(CMAboutLayer::OnBackMenuCallBack));
		CC_BREAK_IF(pStartButton==NULL);
		pStartButton->setPosition(ccp(SCREEN_WIDTH - 100,50));
		pMenu->addChild(pStartButton,enZOrderFront);

		return true;
	} while (false);
	CCLog("Fun CSMAboutLayer::init Error!");
	return false;
}

void CMAboutLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -130, true);
}

void CMAboutLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

bool CMAboutLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//获得菜单
		CCMenu * pMenu = dynamic_cast<CCMenu *>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenu==NULL);
		m_bIsBtnClicked = pMenu->ccTouchBegan(pTouch, pEvent);

		return true;
	} while (false);
	CCLog("Fun CSMAboutLayer::ccTouchBegan Error!");
	return false;
}

void CMAboutLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//获得菜单
		CCMenu * pMenu = dynamic_cast<CCMenu *>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenu==NULL);
		if(m_bIsBtnClicked){
			pMenu->ccTouchMoved(pTouch, pEvent);
		}

		return;
	} while (false);
	CCLog("Fun CSMAboutLayer::ccTouchMoved Error!");
}

void CMAboutLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//获得菜单
		CCMenu * pMenu = dynamic_cast<CCMenu *>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenu==NULL);
		if(m_bIsBtnClicked){
			pMenu->ccTouchEnded(pTouch, pEvent);
		}

		return;
	} while (false);
	CCLog("Fun CSMAboutLayer::ccTouchEnded Error!");
}

void CMAboutLayer::OnBackMenuCallBack( CCObject *pSender )
{
	removeFromParentAndCleanup(true);
}
