#include "CMGameScene.h"
#include "CMMenuScene.h"
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

		//初始化变量
		m_nCurStage = 1;
		m_nCurMarioLife = MARIO_LIFE-1;
		m_eCurLevel = enMarioLevelSmall;
		m_bNeedResetStage = false;
		m_nCurCoinNum = 0;

		//载入地图
		CMGameMap* pGameMap = CMGameMap::CreateGameMap(CCString::createWithFormat("MarioMap%d.tmx",m_nCurStage)->getCString(),this,m_eCurLevel);
		CC_BREAK_IF(pGameMap==NULL);
		pGameMap->setPosition(ccp(0,96));
		addChild(pGameMap,enZOrderBack,enTagMap);

		//载入菜单
		CMMenuScene* pMenuLayer = CMMenuScene::CreateMenuScene();
		CC_BREAK_IF(pMenuLayer==NULL);
		pMenuLayer->setPosition(ccp(0,0));
		addChild(pMenuLayer,enZOrderFront,enTagMenu);

		//UI生命
		CCSprite* pCoinSprite = CCSprite::create("small_walk_1.png");
		CC_BREAK_IF(pCoinSprite==NULL);
		pCoinSprite->setScale(1);
		pCoinSprite->setPosition(ccp(SCREEN_WIDTH*0.88,SCREEN_HEIGHT*0.9));
		addChild(pCoinSprite,enZOrderFront,enTagCoin);
				
		//生命数
		CCLabelTTF* pCoinNum = CCLabelTTF::create("3","Arial",18);
		CC_BREAK_IF(pCoinNum==NULL);
		pCoinNum->setPosition(ccp(SCREEN_WIDTH*0.93,SCREEN_HEIGHT*0.9));
		pCoinNum->setColor(ccc3(0,0,0));
		addChild(pCoinNum,enZOrderFront,enTagLifeNum);

		//UI金币
		CCSprite* pLifeSprite = CCSprite::create("coin.png");
		CC_BREAK_IF(pLifeSprite==NULL);
		pLifeSprite->setScale(1);
		pLifeSprite->setPosition(ccp(SCREEN_WIDTH*0.78,SCREEN_HEIGHT*0.9));
		addChild(pLifeSprite,enZOrderFront,enTagLife);

		//金币数
		CCLabelTTF* pLifeNum = CCLabelTTF::create("0","Arial",18);
		CC_BREAK_IF(pLifeNum==NULL);
		pLifeNum->setPosition(ccp(SCREEN_WIDTH*0.83,SCREEN_HEIGHT*0.9));
		pLifeNum->setColor(ccc3(0,0,0));
		addChild(pLifeNum,enZOrderFront,enTagCoinNum);

		InitControlUI();
		//开启触摸
		setTouchEnabled(true);

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
	switch (enMsg)
	{
	case enMsgPassLevel:
		{
			OnSubMsgPassLevel();
		}
		break;
	case enMsgMarioDead:
		{
			OnSubMsgMarioDead();
		}
		break;
	case enMsgEatCoin:
		{
			OnSubMsgEatCoin();
		}
		break;
	case enMsgStartGame:
		{
			OnSubMsgStartGame();
		}
		break;
	}
}

void CMGameScene::OnCallPerFrame(float dt)
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		pMap->OnCallPerFrame(dt);

#if(CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)    

		CCMenu* pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagUI));
		CC_BREAK_IF(pMenu==NULL);
		CCMenuItemImage *pLeftKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagLeftKey));
		CC_BREAK_IF(pLeftKey==NULL);
		CCMenuItemImage *pRightKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagRightKey));
		CC_BREAK_IF(pRightKey==NULL);
		CCMenuItemImage *pJumpKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagJumpKey));
		CC_BREAK_IF(pJumpKey==NULL);
		CCMenuItemImage *pFireKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagFireKey));
		CC_BREAK_IF(pFireKey==NULL);

		if (pJumpKey->isSelected()==false)
		{
			pMap->SetJumpKeyDown(false);
		}
		if (pJumpKey->isSelected()==true)
		{
			pMap->SetJumpKeyDown(true);
		}

		if (pFireKey->isSelected()==false)
		{
			pMap->SetFireKeyDown(false);
		}
		if (pFireKey->isSelected()==true)
		{
			pMap->SetFireKeyDown(true);
		}

		if (pLeftKey->isSelected()==false)
		{
			pMap->SetLeftKeyDown(false);
		}
		if (pLeftKey->isSelected()==true)
		{
			pMap->SetLeftKeyDown(true);
		}

		if (pRightKey->isSelected()==false)
		{
			pMap->SetRightKeyDown(false);
		}
		if (pRightKey->isSelected()==true)
		{
			pMap->SetRightKeyDown(true);
		}
#endif
		//更新金币数
		CCLabelTTF* pCoinNum = dynamic_cast<CCLabelTTF*>(getChildByTag(enTagCoinNum));
		CC_BREAK_IF(pCoinNum==NULL);
		char CoinNum[20] = {0};
		sprintf(CoinNum,"%d",m_nCurCoinNum);
		pCoinNum->setString(CoinNum);
		if (m_nCurCoinNum>100)
		{
			m_nCurCoinNum = 0;
			m_nCurMarioLife++;
		}
		
		//更新生命数
		CCLabelTTF* pLifeNum = dynamic_cast<CCLabelTTF*>(getChildByTag(enTagLifeNum));
		CC_BREAK_IF(pLifeNum==NULL);
		char LifeNum[20] = {0};
		sprintf(LifeNum,"%d",m_nCurMarioLife);
		pLifeNum->setString(LifeNum);

		//获得mario当前等级
		m_eCurLevel = pMap->GetMarioLevel();

		//如果死亡则重置关卡
		if (m_bNeedResetStage==true)
		{
			removeChildByTag(enTagMap);
			m_eCurLevel = enMarioLevelSmall;
			CMGameMap* pGameMap = CMGameMap::CreateGameMap(CCString::createWithFormat("MarioMap%d.tmx",m_nCurStage)->getCString(),this,m_eCurLevel);
			CC_BREAK_IF(pGameMap==NULL);
			pGameMap->setPosition(ccp(0,96));
			addChild(pGameMap,enZOrderBack,enTagMap);

			m_bNeedResetStage=false;
		}

		return;
	} while (false);
	CCLog("fun CMGameScene::Update Error!");
}


void CMGameScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		CCMenu* pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagUI));
		CC_BREAK_IF(pMenu==NULL);
		CCMenuItemImage* pLeftKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagLeftKey));
		CC_BREAK_IF(pLeftKey==NULL);
		CCMenuItemImage* pRightKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagRightKey));
		CC_BREAK_IF(pRightKey==NULL);
		CCMenuItemImage* pJumpKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagJumpKey));
		CC_BREAK_IF(pJumpKey==NULL);
		CCMenuItemImage* pFireKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagFireKey));
		CC_BREAK_IF(pFireKey==NULL);

		for(CCSetIterator iterTouch = pTouches->begin(); iterTouch != pTouches->end(); iterTouch++)
		{
			CCTouch *pCurTouch =  (cocos2d::CCTouch*)(*iterTouch);

			if (pJumpKey->boundingBox().containsPoint(pCurTouch->getLocation()))
			{
				pJumpKey->selected();
			}

			if (pFireKey->boundingBox().containsPoint(pCurTouch->getLocation()))
			{
				pFireKey->selected();
			}

			if (pLeftKey->boundingBox().containsPoint(pCurTouch->getLocation()))
			{
				pLeftKey->selected();
			}

			if (pRightKey->boundingBox().containsPoint(pCurTouch->getLocation()))
			{
				pRightKey->selected();
			}

		}

		return;
	} while (false);
	CCLog("CGameLayer::ccTouchesBegan Error!");
}

void CMGameScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		CCMenu* pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagUI));
		CC_BREAK_IF(pMenu==NULL);
		CCMenuItemImage* pLeftKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagLeftKey));
		CC_BREAK_IF(pLeftKey==NULL);
		CCMenuItemImage* pRightKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagRightKey));
		CC_BREAK_IF(pRightKey==NULL);
		CCMenuItemImage* pJumpKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagJumpKey));
		CC_BREAK_IF(pJumpKey==NULL);
		CCMenuItemImage* pFireKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagFireKey));
		CC_BREAK_IF(pFireKey==NULL);

		for(CCSetIterator iterTouch = pTouches->begin(); iterTouch != pTouches->end(); iterTouch++)
		{
			CCTouch *pCurTouch =  (cocos2d::CCTouch*)(*iterTouch);

			if (pLeftKey->boundingBox().containsPoint(pCurTouch->getLocation()))
			{
				pLeftKey->selected();
			}
			if (pRightKey->boundingBox().containsPoint(pCurTouch->getLocation()))
			{
				pRightKey->selected();
			}
			if (pJumpKey->boundingBox().containsPoint(pCurTouch->getLocation()))
			{
				pJumpKey->selected();
			}
			else
			{
				pJumpKey->unselected();
			}
			if (pFireKey->boundingBox().containsPoint(pCurTouch->getLocation()))
			{
				pFireKey->selected();
			}
			else
			{
				pFireKey->unselected();
			}

			if (pLeftKey->boundingBox().containsPoint(pCurTouch->getLocation())==false &&
				pRightKey->boundingBox().containsPoint(pCurTouch->getLocation())==false)
			{
				pLeftKey->unselected();
				pRightKey->unselected();
			}
		}

		return;
	} while (false);
	CCLog("CGameLayer::ccTouchesMoved Error!");
}

void CMGameScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);

		CCMenu* pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagUI));
		CC_BREAK_IF(pMenu==NULL);
		CCMenuItemImage* pLeftKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagLeftKey));
		CC_BREAK_IF(pLeftKey==NULL);
		CCMenuItemImage* pRightKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagRightKey));
		CC_BREAK_IF(pRightKey==NULL);
		CCMenuItemImage* pJumpKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagJumpKey));
		CC_BREAK_IF(pJumpKey==NULL);
		CCMenuItemImage* pFireKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagFireKey));
		CC_BREAK_IF(pFireKey==NULL);

		pLeftKey->unselected();
		pRightKey->unselected();
		pFireKey->unselected();
		pJumpKey->unselected();

		return;
	} while (false);
	CCLog("CGameLayer::ccTouchesEnded Error!");
}

void CMGameScene::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);

		CCMenu* pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagUI));
		CC_BREAK_IF(pMenu==NULL);
		CCMenuItemImage* pLeftKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagLeftKey));
		CC_BREAK_IF(pLeftKey==NULL);
		CCMenuItemImage* pRightKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagRightKey));
		CC_BREAK_IF(pRightKey==NULL);
		CCMenuItemImage* pJumpKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagJumpKey));
		CC_BREAK_IF(pJumpKey==NULL);
		CCMenuItemImage* pFireKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagFireKey));
		CC_BREAK_IF(pFireKey==NULL);

		pLeftKey->unselected();
		pRightKey->unselected();
		pFireKey->unselected();
		pJumpKey->unselected();

		return;
	} while (false);
	CCLog("CGameLayer::ccTouchesCancelled Error!");
}

void CMGameScene::InitControlUI()
{
	do 
	{
		//控制UI背景图
		CCSprite* pControlUIBg = CCSprite::create("controlUI.png");
		CC_BREAK_IF(pControlUIBg==NULL);
		pControlUIBg->setAnchorPoint(ccp(0, 0));
		pControlUIBg->setPosition(ccp(0,0));
		this->addChild(pControlUIBg,enZOrderMiddle);

		CCMenu* pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(ccp(0,0));
		addChild(pMenu,enZOrderMiddle,enTagUI);

		CCMenuItemImage *pLeftKey = CCMenuItemImage::create(
			"AB_normal.png",
			"AB_select.png",
			this, 
			menu_selector(CMGameScene::OnMenuLeftKeyDownCallBack));
		CC_BREAK_IF(pLeftKey==NULL);
		pLeftKey->setPosition(ccp(40,48));
		pMenu->addChild(pLeftKey,enZOrderFront,enTagLeftKey);

		CCMenuItemImage *pRightKey = CCMenuItemImage::create(
			"AB_normal.png",
			"AB_select.png",
			this, 
			menu_selector(CMGameScene::OnMenuRightKeyDownCallBack));
		CC_BREAK_IF(pRightKey==NULL);
		pRightKey->setPosition(ccp(128,48));
		pMenu->addChild(pRightKey,enZOrderFront,enTagRightKey);

		CCMenuItemImage *pJumpKey = CCMenuItemImage::create(
			"AB_normal.png",
			"AB_select.png",
			this, 
			menu_selector(CMGameScene::OnMenuJumpKeyDownCallBack));
		CC_BREAK_IF(pJumpKey==NULL);
		pJumpKey->setPosition(ccp(432,35));
		pMenu->addChild(pJumpKey,enZOrderFront,enTagJumpKey);

		CCMenuItemImage *pFireKey = CCMenuItemImage::create(
			"AB_normal.png",
			"AB_select.png",
			this, 
			menu_selector(CMGameScene::OnMenuFireKeyDownCallBack));
		CC_BREAK_IF(pFireKey==NULL);
		pFireKey->setPosition(ccp(353,35));
		pMenu->addChild(pFireKey,enZOrderFront,enTagFireKey);
/*此处可以使用CCControlButton控件做按钮，可以提供多种回调方式，这里因为没有CCScale9Sprite资源，固采用传统的CCMenuItemImage控件。

		//左键
		CCControlButton *pLeftKey = CCControlButton::create(CCScale9Sprite::create("AB_normal.png"));
		CC_BREAK_IF(pLeftKey==NULL);
		pLeftKey->setBackgroundSpriteForState( CCScale9Sprite::create("AB_select.png"), CCControlStateSelected); 
		pLeftKey->setPosition(ccp(40,48));
		addChild(pLeftKey,enZOrderFront,enTagLeftKey);
		// 当鼠标处于按下并曾经点中按钮时，则触发一次 //  
		pLeftKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuLeftKeyDownCallBack), CCControlEventTouchDown);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发一次 //  
		pLeftKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuLeftKeyDownCallBack), CCControlEventTouchDragEnter);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发一次 //  
		pLeftKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuLeftKeyUpCallBack), CCControlEventTouchDragExit);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标松开且在按钮范围内，则触发一次 //  
		pLeftKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuLeftKeyUpCallBack), CCControlEventTouchUpInside);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发，只要达到条件，就不断触发 //  
		pLeftKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuLeftKeyDownCallBack), CCControlEventTouchDragInside);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发，只要达到条件，就不断触发 //  
		pLeftKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuLeftKeyUpCallBack), CCControlEventTouchDragOutside);  

		//右键
		CCControlButton *pRightKey = CCControlButton::create(CCScale9Sprite::create("AB_normal.png"));
		CC_BREAK_IF(pRightKey==NULL);
		pRightKey->setBackgroundSpriteForState( CCScale9Sprite::create("AB_select.png"), CCControlStateSelected); 
		pRightKey->setPosition(ccp(128,48));
		addChild(pRightKey,enZOrderFront,enTagRightKey);
		// 当鼠标处于按下并曾经点中按钮时，则触发一次 //  
		pRightKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuRightKeyDownCallBack), CCControlEventTouchDown);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发一次 //  
		pRightKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuRightKeyDownCallBack), CCControlEventTouchDragEnter);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发一次 //  
		pRightKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuRightKeyUpCallBack), CCControlEventTouchDragExit);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标松开且在按钮范围内，则触发一次 //  
		pRightKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuRightKeyUpCallBack), CCControlEventTouchUpInside);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发，只要达到条件，就不断触发 //  
		pRightKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuRightKeyDownCallBack), CCControlEventTouchDragInside);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发，只要达到条件，就不断触发 //  
		pRightKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuRightKeyUpCallBack), CCControlEventTouchDragOutside);  

		//跳键
		CCControlButton *pJumpKey = CCControlButton::create(CCScale9Sprite::create("AB_normal.png"));
		CC_BREAK_IF(pJumpKey==NULL);
		pJumpKey->setBackgroundSpriteForState( CCScale9Sprite::create("AB_select.png"), CCControlStateSelected); 
		pJumpKey->setPosition(ccp(432,35));
		addChild(pJumpKey,enZOrderFront,enTagJumpKey);
		// 当鼠标处于按下并曾经点中按钮时，则触发一次 //  
		pJumpKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuJumpKeyDownCallBack), CCControlEventTouchDown);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发一次 //  
		pJumpKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuJumpKeyDownCallBack), CCControlEventTouchDragEnter);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发一次 //  
		pJumpKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuJumpKeyUpCallBack), CCControlEventTouchDragExit);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标松开且在按钮范围内，则触发一次 //  
		pJumpKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuJumpKeyUpCallBack), CCControlEventTouchUpInside);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发，只要达到条件，就不断触发 //  
		pJumpKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuJumpKeyDownCallBack), CCControlEventTouchDragInside);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发，只要达到条件，就不断触发 //  
		pJumpKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuJumpKeyUpCallBack), CCControlEventTouchDragOutside);  

		//开火键
		CCControlButton *pFireKey = CCControlButton::create(CCScale9Sprite::create("AB_normal.png"));
		CC_BREAK_IF(pFireKey==NULL);
		pFireKey->setBackgroundSpriteForState( CCScale9Sprite::create("AB_select.png"), CCControlStateSelected); 
		pFireKey->setPosition(ccp(353,35));
		addChild(pFireKey,enZOrderFront,enTagFireKey);
		// 当鼠标处于按下并曾经点中按钮时，则触发一次 //  
		pFireKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuFireKeyDownCallBack), CCControlEventTouchDown);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发一次 //  
		pFireKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuFireKeyDownCallBack), CCControlEventTouchDragEnter);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发一次 //  
		pFireKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuFireKeyUpCallBack), CCControlEventTouchDragExit);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标松开且在按钮范围内，则触发一次 //  
		pFireKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuFireKeyUpCallBack), CCControlEventTouchUpInside);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发，只要达到条件，就不断触发 //  
		pFireKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuFireKeyDownCallBack), CCControlEventTouchDragInside);  
		// 当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发，只要达到条件，就不断触发 //  
		pFireKey->addTargetWithActionForControlEvents(this, cccontrol_selector(CMGameScene::OnMenuFireKeyUpCallBack), CCControlEventTouchDragOutside);  
*/
		return;
	} while (false);
	CCLog("Fun CMGameScene::InitControlUI Error!");
}

void CMGameScene::OnMenuLeftKeyDownCallBack( CCObject *pSender  )
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		//pMap->SetLeftKeyDown(true);
		PrintErrorControlMsg();
		return ;
	} while (false);
	CCLog("fun CMGameScene::OnMenuLeftKeyCallBack error!");
}

void CMGameScene::OnMenuRightKeyDownCallBack( CCObject *pSender  )
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		//pMap->SetRightKeyDown(true);
		PrintErrorControlMsg();
		return ;
	} while (false);
	CCLog("fun CMGameScene::OnMenuRightKeyCallBack error!");
}

void CMGameScene::OnMenuJumpKeyDownCallBack( CCObject *pSender )
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		//pMap->SetJumpKeyDown(true);
		PrintErrorControlMsg();
		return ;
	} while (false);
	CCLog("fun CMGameScene::OnMenuJumpKeyCallBack error!");
}

void CMGameScene::OnMenuFireKeyDownCallBack( CCObject *pSender )
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		//pMap->SetFireKeyDown(true);
		PrintErrorControlMsg();
		return ;
	} while (false);
	CCLog("fun CMGameScene::OnMenuFireKeyCallBack error!");
}

void CMGameScene::OnMenuLeftKeyUpCallBack( CCObject* pSender, CCControlEvent event )
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		pMap->SetLeftKeyDown(false);
		return;
	} while (false);
	CCLog("fun CMGameScene::OnMenuLeftKeyUpCallBack Error!");
}

void CMGameScene::OnMenuRightKeyUpCallBack( CCObject* pSender, CCControlEvent event )
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		pMap->SetRightKeyDown(false);
		return;
	} while (false);
	CCLog("fun CMGameScene::OnMenuRightKeyUpCallBack Error!");
}

void CMGameScene::OnMenuJumpKeyUpCallBack( CCObject* pSender, CCControlEvent event )
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		pMap->SetJumpKeyDown(false);
		return;
	} while (false);
	CCLog("fun CMGameScene::OnMenuJumpKeyUpCallBack Error!");
}

void CMGameScene::OnMenuFireKeyUpCallBack( CCObject* pSender, CCControlEvent event )
{
	do 
	{
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);
		pMap->SetFireKeyDown(false);
		return;
	} while (false);
	CCLog("fun CMGameScene::OnMenuFireKeyUpCallBack Error!");
}

void CMGameScene::OnSubMsgPassLevel()
{
	do 
	{
		removeChildByTag(enTagMap);
		//关卡数
		if (m_nCurStage>2)
		{
			m_nCurStage=1;
		}
		else
		{
			m_nCurStage++;
		}
		//载入新关卡
		CMGameMap* pGameMap = CMGameMap::CreateGameMap(CCString::createWithFormat("MarioMap%d.tmx",m_nCurStage)->getCString(),this,m_eCurLevel);
		CC_BREAK_IF(pGameMap==NULL);
		pGameMap->setPosition(ccp(0,96));
		addChild(pGameMap,enZOrderBack,enTagMap);

		return ;
	} while (false);
	CCLog("fun CMGameScene::OnSubMsgPassLevel Error!");
}

void CMGameScene::OnSubMsgMarioDead()
{
	do 
	{
		//生命数为0则返回菜单
		if (m_nCurMarioLife==0)
		{
			//停止Update函数
			unschedule(schedule_selector(CMGameScene::OnCallPerFrame));
			//显示菜单界面
			CMMenuScene* pMenuLayer = dynamic_cast<CMMenuScene*>(getChildByTag(enTagMenu));
			CC_BREAK_IF(pMenuLayer==NULL);
			CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(pMenuLayer, -130, true);
			setTouchEnabled(true);
			pMenuLayer->setVisible(true);
			m_nCurCoinNum = 0;
			m_nCurMarioLife = 3;
		}
		//需要重置关卡
		m_bNeedResetStage = true;
		m_nCurMarioLife--;

		return ;
	} while (false);
	CCLog("fun CMGameScene::OnSubMsgGameOver Error!");
}

void CMGameScene::PrintErrorControlMsg()
{
	CCLog("This is NOT support on Platform Win32,please use key A D J K.");
}

void CMGameScene::OnSubMsgEatCoin()
{
	do 
	{
		//当前金币数加一
		m_nCurCoinNum++;
		return;
	} while (false);
	CCLog("fun CMGameScene::OnSubEatCoin Error!");
}

void CMGameScene::OnSubMsgStartGame()
{
	do 
	{
		//注册Update函数
		this->schedule(schedule_selector(CMGameScene::OnCallPerFrame));
		//游戏开始，隐藏菜单界面
		CMMenuScene* pMenuLayer = dynamic_cast<CMMenuScene*>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenuLayer==NULL);
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pMenuLayer);
		pMenuLayer->setVisible(false);

		return;
	} while (false);
	CCLog("fun CMGameScene::OnSubStartGame Error!");
}

