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

		m_fMapMove = 0;

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

bool bIsLeftKeyDown = false;
bool bIsRightKeyDown = false;
bool bIsJumpKeyDown = false;
bool bIsJumping = false;
void CMGameScene::Update(float dt)
{
	do 
	{
		CMHero* pHero = dynamic_cast<CMHero*>(getChildByTag(enTagHero));
		CC_BREAK_IF(pHero==NULL);
		
		CCPoint CurMarioPos = pHero->getPosition();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)    
		if(KEY_DOWN(KEY_KEY_K))
		{
			
		}
		if (KEY_DOWN(KEY_KEY_J))
		{

		}

		if(KEY_DOWN(KEY_KEY_A))
		{
			bIsLeftKeyDown = true;
		}
		if(KEY_UP(KEY_KEY_A))
		{
			bIsLeftKeyDown = false;
		}
		if(KEY_DOWN(KEY_KEY_D))
		{
			bIsRightKeyDown = true;
		}
		if(KEY_UP(KEY_KEY_D))
		{
			bIsRightKeyDown = false;
		}
		if(KEY_DOWN(KEY_KEY_W))
		{
			bIsJumpKeyDown = true;
		}
		if(KEY_UP(KEY_KEY_W))
		{
			bIsJumpKeyDown = false;
		}
		if(KEY_DOWN(KEY_KEY_S))
		{
			
		}
#endif
		CMGameMap* pMap = dynamic_cast<CMGameMap*>(getChildByTag(enTagMap));
		CC_BREAK_IF(pMap==NULL);

		CCSprite* pTileSprite1 = NULL;
		CCSprite* pTileSprite2 = NULL;
		CCSprite* pTileSprite3 = NULL;
		//用英雄下方的三个瓦片来判断掉落碰撞
		pTileSprite1 = pMap->HeroPosToTileSprite(ccp(pHero->getPositionX()+pHero->boundingBox().size.width/2,pHero->getPositionY()),m_fMapMove);
		pTileSprite2 = pMap->HeroPosToTileSprite(ccp(pHero->getPositionX()+5,pHero->getPositionY()),m_fMapMove);
		pTileSprite3 = pMap->HeroPosToTileSprite(ccp(pHero->getPositionX()+pHero->boundingBox().size.width-5,pHero->getPositionY()),m_fMapMove);
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			pHero->setPosition(CurMarioPos);
		}
		else
		{
			pHero->setPositionY(pHero->getPositionY()-2);
		}
		
		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//根据变量控制英雄移动及动作
		if (bIsLeftKeyDown)
		{
			//用英雄左方的两个瓦片来判断后退碰撞
			pTileSprite1 = pMap->HeroPosToTileSprite(ccp(pHero->getPositionX(),pHero->getPositionY()+pHero->boundingBox().size.height),m_fMapMove);
			pTileSprite2 = pMap->HeroPosToTileSprite(ccp(pHero->getPositionX(),pHero->getPositionY()+pHero->boundingBox().size.height*0.15),m_fMapMove);
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				pHero->setPosition(CurMarioPos);
			}
			else
			{
				if (pHero->boundingBox().getMinX()>0)
				{
					pHero->setPositionX(pHero->getPositionX()-2);
				}
			}
		}
		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		if (bIsRightKeyDown)
		{
			//用英雄右方的两个瓦片来判断前进碰撞
			pTileSprite1 = pMap->HeroPosToTileSprite(ccp(pHero->getPositionX()+pHero->boundingBox().size.width,pHero->getPositionY()+pHero->boundingBox().size.height),m_fMapMove);
			pTileSprite2 = pMap->HeroPosToTileSprite(ccp(pHero->getPositionX()+pHero->boundingBox().size.width,pHero->getPositionY()+pHero->boundingBox().size.height*0.15),m_fMapMove);
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				pHero->setPosition(CurMarioPos);
			}
			else
			{
				if (pHero->getPositionX()>100)
				{
					pMap->setPositionX(pMap->getPositionX()-2);
					m_fMapMove += 2;
				}
				else 
				{
					pHero->setPositionX(pHero->getPositionX()+2);
				}
			}
		}
		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		if (bIsJumpKeyDown)
		{
			//if (pHero->boundingBox().getMaxY()<_SCREEN_HEIGHT_)
			if (bIsJumping==false)
			{
				CCMoveBy* pJump = CCMoveBy::create(0.3,ccp(0,80));
				pHero->runAction(CCSequence::create(pJump,CCCallFunc::create(this, callfunc_selector(CMGameScene::OnJumpCallBack)),NULL));
				bIsJumping = true;
			}
		}
		
		//CCLog("TileType = %d",pMap->HeroPosToTileType(pHero->getPosition()));
		//CCLog("HeroPosX=%f	HeroPosY=%f",pHero->getPositionX(),pHero->getPositionY());
		return;
	} while (false);
	CCLog("fun CMGameScene::Update Error!");
}

void CMGameScene::OnJumpCallBack()
{
	bIsJumping = false; 
}
