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

		CMMario* pHero = CMMario::CreateHero();
		CC_BREAK_IF(pHero==NULL);
		pHero->setPosition(ccp(SCREEN_WIDTH*0.1,SCREEN_HEIGHT*0.8));
		addChild(pHero,enZOrderFront,enTagHero);

		//注册Update函数
		this->schedule(schedule_selector(CMGameScene::Update,1));

		m_fMapMove = 0;
		m_fDropSpeed = 0;
		m_fJumpSpeed = 0;

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
float fSpeed = 2;
void CMGameScene::Update(float dt)
{
	do 
	{
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagHero));
		CC_BREAK_IF(pMario==NULL);

		CCPoint CurMarioPos = pMario->getPosition();

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
		//根据变量控制英雄移动及动作
		if (bIsLeftKeyDown)
		{
			//用英雄左方的两个瓦片来判断后退碰撞
			pTileSprite1 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX(),pMario->getPositionY()+pMario->boundingBox().size.height),m_fMapMove);
			pTileSprite2 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX(),pMario->getPositionY()+pMario->boundingBox().size.height/2),m_fMapMove);
			pTileSprite3 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX(),pMario->getPositionY()),m_fMapMove);
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				pMario->setPosition(CurMarioPos);
				//pHero->setPositionX(pHero->getPositionX()-2);
			}
			else
			{
				if (pMario->boundingBox().getMinX()>0)
				{
					pMario->setPositionX(pMario->getPositionX()-fSpeed);
				}
			}
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		if (bIsRightKeyDown)
		{
			//用英雄右方的两个瓦片来判断前进碰撞
			pTileSprite1 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width,pMario->getPositionY()+pMario->boundingBox().size.height),m_fMapMove);
			pTileSprite2 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width,pMario->getPositionY()+pMario->boundingBox().size.height/2),m_fMapMove);
			pTileSprite3 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width,pMario->getPositionY()),m_fMapMove);
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				pMario->setPosition(CurMarioPos);
				//pHero->setPositionX(pHero->getPositionX()+2);
			}
			else
			{
				if (pMario->getPositionX()>100)
				{
					pMap->setPositionX(pMap->getPositionX()-fSpeed);
					m_fMapMove += fSpeed;
				}
				else 
				{
					pMario->setPositionX(pMario->getPositionX()+fSpeed);
				}
			}
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//用英雄下方的三个瓦片来判断掉落碰撞
		pTileSprite1 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width/2,pMario->getPositionY()),m_fMapMove);
		pTileSprite2 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX()+5,pMario->getPositionY()),m_fMapMove);
		pTileSprite3 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width-5,pMario->getPositionY()),m_fMapMove);
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			pMario->setPosition(CurMarioPos);
			m_fDropSpeed = 0;
			m_fJumpSpeed = 8;
		}
		else
		{
			pMario->setPositionY(pMario->getPositionY()-m_fDropSpeed);
			m_fDropSpeed += 0.098;
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//用英雄上方的三个瓦片来判断头顶碰撞
		pTileSprite1 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width/2,pMario->getPositionY()+pMario->boundingBox().size.height),m_fMapMove);
		pTileSprite2 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX()+5,pMario->getPositionY()+pMario->boundingBox().size.height),m_fMapMove);
		pTileSprite3 = pMap->MarioPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width-5,pMario->getPositionY()+pMario->boundingBox().size.height),m_fMapMove);
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			pMario->setPosition(CurMarioPos);
			m_fJumpSpeed = 0;
		}
		else
		{
			//跳跃
			if (bIsJumpKeyDown)
			{
				pMario->setPositionY(pMario->getPositionY()+m_fJumpSpeed);
				m_fJumpSpeed -= 0.3;
			}
		}
		
		

		

		//CCLog("TileType = %d",pMap->HeroPosToTileType(pHero->getPosition()));
		//CCLog("HeroPosX=%f	HeroPosY=%f",pHero->getPositionX(),pHero->getPositionY());
		return;
	} while (false);
	CCLog("fun CMGameScene::Update Error!");
}
