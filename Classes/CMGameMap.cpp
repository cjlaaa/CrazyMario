#include "CMGameMap.h"


CMGameMap* CMGameMap::CreateGameMap(const char* pFileName,CMMario* pMario)
{
	do 
	{
		CMGameMap* pPlayer = new CMGameMap;
		if (pPlayer && pPlayer->initWithTMXFile(pFileName))
		{
			pPlayer->Init(pMario);
			pPlayer->autorelease();
			return pPlayer;
		}
		delete pPlayer;
	} while (false);
	CCLog("Fun CMGameMap::CreateGameMap Error!");
	return NULL;
}

bool CMGameMap::Init(CMMario* pMario)
{
	do 
	{
		//注册Update函数
		this->schedule(schedule_selector(CMGameMap::OnCallPerFrame));

		m_pMario = pMario;

		m_fMapMove = 0;
		m_fDropSpeed = 0;
		m_fJumpSpeed = 0;
		m_fSpeed = 2;

		m_bIsLeftKeyDown = false;
		m_bIsRightKeyDown = false;
		m_bIsJumpKeyDown = false;

		return true;
	} while (false);
	CCLog("Fun CMGameMap::Init Error!");
	return false;
}

CCSprite* CMGameMap::MarioPosToTileSprite( CCPoint HeroPos,float fMapMove, bool &bIsHeroDead)
{
	do 
	{
		bIsHeroDead = false;
		//将层坐标转换为地图瓦片坐标
		int nHeroTilePosX = (HeroPos.x + fMapMove)/this->getTileSize().width;
		int nHeroTempPosY = (HeroPos.y - 96)/this->getTileSize().height;
		int nHeroTilePosY = 13 - nHeroTempPosY;

		//获得地图的各个层
		CCTMXLayer* pCloudLayer = layerNamed("cloud");
		CC_BREAK_IF(pCloudLayer==NULL);
		CCTMXLayer* pBlockLayer = layerNamed("block");
		CC_BREAK_IF(pBlockLayer==NULL);
		CCTMXLayer* pPipeLayer = layerNamed("pipe");
		CC_BREAK_IF(pPipeLayer==NULL);
		CCTMXLayer* pLandLayer = layerNamed("land");
		CC_BREAK_IF(pLandLayer==NULL);
		CCTMXLayer* pTrapLayer = layerNamed("trap");
		CC_BREAK_IF(pTrapLayer==NULL);
		CCTMXObjectGroup* pObjectLayer = objectGroupNamed("objects");
		CC_BREAK_IF(pObjectLayer==NULL);
		CCTMXLayer* pCoinLayer = layerNamed("coin");
		CC_BREAK_IF(pCoinLayer==NULL);
		CCTMXLayer* pFlagpoleLayer = layerNamed("flagpole");
		CC_BREAK_IF(pFlagpoleLayer==NULL);

		//若马里奥 越下界
		if(nHeroTilePosY>=(int)getMapSize().height)
		{
			bIsHeroDead = true;
			return NULL;
		}

		//若马里奥超过上边界
		if(nHeroTilePosY<0)return NULL;

		CCSprite* pLandSprite = pLandLayer->tileAt(ccp(nHeroTilePosX,nHeroTilePosY));
		if (pLandSprite!=NULL)
		{
			return pLandSprite;
		}
		CCSprite* pBlockSprite = pBlockLayer->tileAt(ccp(nHeroTilePosX,nHeroTilePosY));
		if (pBlockSprite!=NULL)
		{
			return pBlockSprite;
		}
		CCSprite* pPipeSprite = pPipeLayer->tileAt(ccp(nHeroTilePosX,nHeroTilePosY));	
		if (pPipeSprite!=NULL)
		{
			return pPipeSprite;
		}
		return NULL;
	} while (false);
	CCLog("fun CMGameMap::HeroPosToTileType Error!");
	return NULL;
}

void CMGameMap::OnCallPerFrame(float dt)
{
	do 
	{
		CCPoint CurMarioPos = m_pMario->getPosition();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)    

// 		CCMenu* pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagMenu));
// 		CC_BREAK_IF(pMenu==NULL);
// 		CCMenuItemImage* pLeftKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagLeftKey));
// 		CC_BREAK_IF(pLeftKey==NULL);
// 		CCMenuItemImage* pRightKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagLeftKey));
// 		CC_BREAK_IF(pRightKey==NULL);
// 		CCMenuItemImage* pJumpKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagLeftKey));
// 		CC_BREAK_IF(pJumpKey==NULL);
// 		CCMenuItemImage* pFireKey = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(enTagLeftKey));
// 		CC_BREAK_IF(pFireKey==NULL);

		if(KEY_DOWN(KEY_KEY_K))
		{

		}
		if (KEY_DOWN(KEY_KEY_J))
		{

		}

		if(KEY_DOWN(KEY_KEY_A))
		{
			m_bIsLeftKeyDown = true;
		}
		if(KEY_UP(KEY_KEY_A))
		{
			m_bIsLeftKeyDown = false;
		}
		if(KEY_DOWN(KEY_KEY_D))
		{
			m_bIsRightKeyDown = true;
		}
		if(KEY_UP(KEY_KEY_D))
		{
			m_bIsRightKeyDown = false;
		}
		if(KEY_DOWN(KEY_KEY_W))
		{
			m_bIsJumpKeyDown = true;
		}
		if(KEY_UP(KEY_KEY_W))
		{
			m_bIsJumpKeyDown = false;
		}
		if(KEY_DOWN(KEY_KEY_S))
		{

		}
#endif
		CMGameMap* pMap = this;
		CC_BREAK_IF(pMap==NULL);

		//判断马里奥是否 [落坑] 死亡
		bool bIsHeroDead = false;
		pMap->MarioPosToTileSprite(m_pMario->getPosition(),m_fMapMove,bIsHeroDead);
		if(bIsHeroDead)
		{
			CCLog("@GameOver!");
			CCDirector::sharedDirector()->end();
			return;
		}

		CCSprite* pTileSprite1 = NULL;
		CCSprite* pTileSprite2 = NULL;
		CCSprite* pTileSprite3 = NULL;
		//根据变量控制英雄移动及动作
		if (m_bIsLeftKeyDown)
		{
			//用英雄左方的三个瓦片来判断后退碰撞
			pTileSprite1 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX(),m_pMario->getPositionY()+m_pMario->BoundingBox().size.height),m_fMapMove,bIsHeroDead);
			pTileSprite2 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX(),m_pMario->getPositionY()+m_pMario->BoundingBox().size.height/2),m_fMapMove,bIsHeroDead);
			pTileSprite3 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX(),m_pMario->getPositionY()),m_fMapMove,bIsHeroDead);
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				m_pMario->setPosition(CurMarioPos);
				//pHero->setPositionX(pHero->getPositionX()-2);
			}
			else
			{
				if (m_pMario->boundingBox().getMinX()>0)
				{
					m_pMario->setPositionX(m_pMario->getPositionX()-m_fSpeed);
				}
			}
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		if (m_bIsRightKeyDown)
		{
			//用英雄右方的三个瓦片来判断前进碰撞
			pTileSprite1 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX()+m_pMario->BoundingBox().size.width,m_pMario->getPositionY()+m_pMario->BoundingBox().size.height),m_fMapMove,bIsHeroDead);
			pTileSprite2 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX()+m_pMario->BoundingBox().size.width,m_pMario->getPositionY()+m_pMario->BoundingBox().size.height/2),m_fMapMove,bIsHeroDead);
			pTileSprite3 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX()+m_pMario->BoundingBox().size.width,m_pMario->getPositionY()),m_fMapMove,bIsHeroDead);
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				m_pMario->setPosition(CurMarioPos);
				//pHero->setPositionX(pHero->getPositionX()+2);
			}
			else
			{
				if (m_pMario->getPositionX()>100)
				{
					pMap->setPositionX(pMap->getPositionX()-m_fSpeed);
					m_fMapMove += m_fSpeed;
				}
				else 
				{
					m_pMario->setPositionX(m_pMario->getPositionX()+m_fSpeed);
				}
			}
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//用英雄下方的三个瓦片来判断掉落碰撞
		pTileSprite1 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX()+m_pMario->BoundingBox().size.width/2,m_pMario->getPositionY()),m_fMapMove,bIsHeroDead);
		pTileSprite2 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX()+5,m_pMario->getPositionY()),m_fMapMove,bIsHeroDead);
		pTileSprite3 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX()+m_pMario->BoundingBox().size.width-5,m_pMario->getPositionY()),m_fMapMove,bIsHeroDead);
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			m_pMario->setPosition(CurMarioPos);
			m_pMario->setPositionY(m_pMario->getPositionY()+1/*pMario->boundingBox().size.height*0.1*/);
			m_fDropSpeed = 0;
			m_fJumpSpeed = 8;
		}
		else
		{
			m_pMario->setPositionY(m_pMario->getPositionY()-m_fDropSpeed);
			m_fDropSpeed += 0.098;
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//用英雄上方的三个瓦片来判断头顶碰撞
		pTileSprite1 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX()+m_pMario->BoundingBox().size.width/2,m_pMario->getPositionY()+m_pMario->BoundingBox().size.height),m_fMapMove,bIsHeroDead);
		pTileSprite2 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX()+5,m_pMario->getPositionY()+m_pMario->BoundingBox().size.height),m_fMapMove,bIsHeroDead);
		pTileSprite3 = pMap->MarioPosToTileSprite(ccp(m_pMario->getPositionX()+m_pMario->BoundingBox().size.width-5,m_pMario->getPositionY()+m_pMario->BoundingBox().size.height),m_fMapMove,bIsHeroDead);
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			m_pMario->setPosition(CurMarioPos);
			m_pMario->setPositionY(m_pMario->getPositionY()-1);
			m_fJumpSpeed = 0;
		}
		else
		{
			//跳跃
			if (m_bIsJumpKeyDown)
			{
				m_pMario->setPositionY(m_pMario->getPositionY()+m_fJumpSpeed);
				m_fJumpSpeed -= 0.3;
			}
		}

		return;
	} while (false);
	CCLog("fun CMGameMap::OnCallPerFrame Error!");
}
