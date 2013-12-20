#include "CMGameScene.h"
#include "CMGameMap.h"
#include "CMHero.h"

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

		CMHero* pHero = CMHero::CreateHero();
		CC_BREAK_IF(pHero==NULL);
		pHero->setPosition(ccp(_SCREEN_WIDTH_*0.1,_SCREEN_HEIGHT_*0.8));
		addChild(pHero,enZOrderFront,enTagHero);

		//注册Update函数
		this->schedule(schedule_selector(CMGameScene::Update,1));

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

bool bMarioInSky = true;
void CMGameScene::Update(float dt)
{
	do 
	{
		CMHero* pHero = dynamic_cast<CMHero*>(getChildByTag(enTagHero));
		CC_BREAK_IF(pHero==NULL);
		
		CCPoint CurMarioPos = pHero->getPosition();

		if (bMarioInSky)
		{
			pHero->setPosition(ccp(pHero->getPositionX(),pHero->getPositionY()-2));
		}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)    
		if(KEY_DOWN(KEY_KEY_K))
		{
			
		}
		if (KEY_DOWN(KEY_KEY_J))
		{

		}

		if(KEY_DOWN(KEY_KEY_A))
		{
			int nHeroPosX = pHero->getPositionX()-3;
			pHero->setPosition(ccp(nHeroPosX,pHero->getPositionY()));
		}
		if(KEY_DOWN(KEY_KEY_D))
		{
			int nHeroPosX = pHero->getPositionX()+3;
			pHero->setPosition(ccp(nHeroPosX,pHero->getPositionY()));
		}
		if(KEY_DOWN(KEY_KEY_W))
		{
			int nHeroPosY = pHero->getPositionY()+5;
			pHero->setPosition(ccp(pHero->getPositionX(),nHeroPosY));
			bMarioInSky = true;
		}
		if(KEY_DOWN(KEY_KEY_S))
		{
			int nHeroPosY = pHero->getPositionY()-1;
			pHero->setPosition(ccp(pHero->getPositionX(),nHeroPosY));
		}
		if(KEY_UP(KEY_KEY_A)&&KEY_UP(KEY_KEY_D))
		{

		}
#endif
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);

		//纵向碰撞判断
		switch (pMap->HeroPosToTileType(pHero->getPosition()))
		{
		case enTileTypeLand:
		case enTileTypeBlock:
		case enTileTypePipe:
			{
				pHero->setPosition(CurMarioPos);
				bMarioInSky = false;
			}
			break;
		}

		//横向碰撞判断

		CCLog("TileType = %d",pMap->HeroPosToTileType(pHero->getPosition()));
		//CCLog("HeroPosX=%f	HeroPosY=%f",pHero->getPositionX(),pHero->getPositionY());
		return;
	} while (false);
	CCLog("fun CMGameScene::Update Error!");
}