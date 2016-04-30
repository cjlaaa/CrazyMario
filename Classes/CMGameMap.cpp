#include "CMGameMap.h"

CMGameMap* CMGameMap::CreateGameMap(const char* pFileName,CMReceiver* pReceiver,enumMarioLevel &eMarioLevel)
{
	do 
	{
		CMGameMap* pPlayer = new CMGameMap;
		if (pPlayer && pPlayer->initWithTMXFile(pFileName))
		{
			pPlayer->Init(pReceiver,eMarioLevel);
			pPlayer->autorelease();
			return pPlayer;
		}
		delete pPlayer;
	} while (false);
	CCLog("Fun CMGameMap::CreateGameMap Error!");
	return NULL;
}

bool CMGameMap::Init(CMReceiver* pReceiver,enumMarioLevel &eMarioLevel)
{
	do 
	{
		//初始化成员变量
		m_fDropSpeedPlus = 0;
		m_fJumpSpeed = 0;
		m_bIsLeftKeyDown = false;
		m_bIsRightKeyDown = false;
		m_bIsJumpKeyDown = false;
		m_bIsFireKeyDown = false;
		m_bIsHeroDead = false;
		m_pReceiver = pReceiver;
		m_bNeedResetStage = false;

		//初始化游戏对象数组
		m_pArrayItems = CCArray::create();
		m_pArrayItems->retain();
		m_pArrayMonsters = CCArray::create();
		m_pArrayMonsters->retain();
		m_pArrayBlocks = CCArray::create();
		m_pArrayBlocks->retain();
		m_pArrayFireBall = CCArray::create();
		m_pArrayFireBall->retain();

		//初始化Mario
		CMMario* pMario = CMMario::CreateHero(this,eMarioLevel);
		CC_BREAK_IF(pMario==NULL);
		pMario->setPosition(TileMapPosToTileMapLayerPos(ccp(2,11)));
		addChild(pMario,enZOrderFront,enTagMario);
		//pMario->SetStatus(enMarioStatusBig);

		//隐藏原落坑判断层
		CCTMXLayer* pTrapLayer = layerNamed("trap");
		CC_BREAK_IF(pTrapLayer==NULL);
		pTrapLayer->setVisible(false);

		//初始化显示金币
		CCTMXLayer* pCoinLayer = layerNamed("coin");
		CC_BREAK_IF(pCoinLayer==NULL);
		pCoinLayer->setVisible(false);
		//获得地图的瓦片数量
		int nMapHorizontalTileNum = pCoinLayer->boundingBox().size.width/getTileSize().width;
		int nMapVerticalTileNum = pCoinLayer->boundingBox().size.height/getTileSize().height;
		//遍历每片瓦片，寻找金币，建立并加入金币集合
		for (int i = 0;i<nMapHorizontalTileNum;i++)
		{
			for (int j = 0;j<nMapVerticalTileNum;j++)
			{
				if (TileMapPosToTileType(ccp(i,j))==enTileTypeCoin)
				{
					//将瓦片地图坐标转换为瓦片地图层坐标
					CCPoint CoinTileMapLayerPos = TileMapPosToTileMapLayerPos(ccp(i,j));
					CMItemCoin* pCoin = CMItemCoin::CreateItemIcon(CoinTileMapLayerPos,getTileSize(),pMario,this);
					if (pCoin==NULL)
					{
						CCLog("Coin init Error!");
					}
					pCoin->setPosition(CoinTileMapLayerPos);
					pCoin->setAnchorPoint(ccp(0,0));
					m_pArrayItems->addObject(pCoin);
					addChild(pCoin);
				}
			}
		}

		//初始化怪物显示
		CCTMXObjectGroup* pObjectLayer = objectGroupNamed("objects");
		CC_BREAK_IF(pObjectLayer==NULL);
		CCArray *ObjectArray = pObjectLayer->getObjects();
		CCDictionary *pDic = NULL;
		for (unsigned int i = 0; i < ObjectArray->count(); i++)
		{
			pDic = (CCDictionary *)ObjectArray->objectAtIndex(i);
			int PosX = ((CCString*)pDic->objectForKey("x"))->intValue();
			int PosY = ((CCString*)pDic->objectForKey("y"))->intValue();
			PosY -= this->getTileSize().height;
			CCPoint TileXY = ccp(PosX, PosY);

			CCString *strName = (CCString*)pDic->objectForKey("name");
			CCString *strType = (CCString*)pDic->objectForKey("type");

			// 进行怪物的初始化，先根据名字来判断是不是enemy，再细分enemy类型		
			if (strName->m_sString == "enemy")
			{
				if (strType->m_sString == "mushroom")
				{
					CMMonsterMushrooms *pMonster = CMMonsterMushrooms::CreateMonsterMushrooms(TileXY,pMario,this,this);
					if (pMonster==NULL)
					{
						CCLog("pMonster==NULL!");
					}
					pMonster->setPosition(ccp(TileXY.x,TileXY.y));
					pMonster->setAnchorPoint(ccp(0,0));
					m_pArrayMonsters->addObject(pMonster);
					addChild(pMonster,enZOrderFront);
				}
				if (strType->m_sString == "tortoise")
				{
					CMMonsterTortoise *pMonster = CMMonsterTortoise::CreateMonsterTortoise(TileXY,pMario,this,this);
					if (pMonster==NULL)
					{
						CCLog("pMonster==NULL!");
					}
					pMonster->setPosition(ccp(TileXY.x,TileXY.y));
					pMonster->setAnchorPoint(ccp(0,0));
					m_pArrayMonsters->addObject(pMonster);
					addChild(pMonster,enZOrderFront);
				}
				if (strType->m_sString == "flower")
				{
					CMMonsterFlower *pMonster = CMMonsterFlower::CreateMonsterFlower(TileXY,pMario,this,this);
					if (pMonster==NULL)
					{
						CCLog("pMonster==NULL!");
					}
					pMonster->setPosition(ccp(TileXY.x,TileXY.y));
					pMonster->setAnchorPoint(ccp(0,0));
					m_pArrayMonsters->addObject(pMonster);
					addChild(pMonster,enZOrderFront);
				}
			}
		}

		//初始化砖块显示
		CCTMXLayer* pBlockLayer = layerNamed("block");
		CC_BREAK_IF(pBlockLayer==NULL);
		pBlockLayer->setVisible(false);
		//获得地图的瓦片数量
		// 		int nMapHorizontalTileNum = pCoinLayer->boundingBox().size.width/getTileSize().width;
		// 		int nMapVerticalTileNum = pCoinLayer->boundingBox().size.height/getTileSize().height;
		//遍历每片瓦片，寻找瓦片，建立并加入瓦片集合
		for (int i = 0;i<nMapHorizontalTileNum;i++)
		{
			for (int j = 0;j<nMapVerticalTileNum;j++)
			{
				if (TileMapPosToTileType(ccp(i,j))==enTileTypeBlock)
				{
					//解析得到当前砖块的属性
					int GID = pBlockLayer->tileGIDAt(ccp(i,j));
					CCDictionary *pDic = propertiesForGID(GID);
					CC_BREAK_IF(pDic==NULL);
					CCString *strBlockType = (CCString*)pDic->objectForKey("blockType");
					if (strBlockType==NULL)
					{
						continue;
					}
					int nBlockType = strBlockType->intValue();

					//将瓦片地图坐标转换为瓦片地图层坐标
					CCPoint BlockTileMapLayerPos = TileMapPosToTileMapLayerPos(ccp(i,j));
					CMItemBlock* pBlock = CMItemBlock::CreateItemBlock(BlockTileMapLayerPos,getTileSize(),pMario,this,(enumBlockType)nBlockType);
					if (pBlock==NULL)
					{
						CCLog("Block init Error!");
					}
					pBlock->setPosition(BlockTileMapLayerPos);
					pBlock->setAnchorPoint(ccp(0,0));
					m_pArrayBlocks->addObject(pBlock);
					addChild(pBlock);
				}
			}
		}

		return true;
	} while (false);
	CCLog("Fun CMGameMap::Init Error!");
	return false;
}

CCSprite* CMGameMap::TileMapLayerPosToTileSprite( CCPoint TileMapLayerPos)
{
	do 
	{
		CCPoint TileMapPos = TileMapLayerPosToTileMapPos(TileMapLayerPos);

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

		//若马里奥超过上边界
		if(TileMapPos.y<0)return NULL;

		CCSprite* pLandSprite = pLandLayer->tileAt(ccp(TileMapPos.x,TileMapPos.y));
		if (pLandSprite!=NULL)
		{
			return pLandSprite;
		}
		CCSprite* pBlockSprite = pBlockLayer->tileAt(ccp(TileMapPos.x,TileMapPos.y));
		if (pBlockSprite!=NULL)
		{
			//遍历砖块数组，如果砖块数组中未找到，则说明已被顶坏，返回空
			CCObject *pObj = NULL;
			CCARRAY_FOREACH(m_pArrayBlocks,pObj)
			{
				CMItemBlock* pItem = dynamic_cast<CMItemBlock*>(pObj);
				CC_BREAK_IF(pItem==NULL);
				CCPoint CurBlockWorldPos = (pBlockSprite->getPosition());
				CCPoint TempBlockWorldPos = (pItem->getPosition());

				//找到则返回砖块精灵
				if (abs(CurBlockWorldPos.x==TempBlockWorldPos.x) && 
					abs(CurBlockWorldPos.y==TempBlockWorldPos.y))
				{
					return pBlockSprite;
				}
			}
			return NULL;
		}
		CCSprite* pPipeSprite = pPipeLayer->tileAt(ccp(TileMapPos.x,TileMapPos.y));	
		if (pPipeSprite!=NULL)
		{
			return pPipeSprite;
		}
		return NULL;
	} while (false);
	CCLog("fun CMGameMap::HeroPosToTileType Error!");
	return NULL;
}

void CMGameMap::OnCallPerFrame(float fT)
{
	do 
	{
		//遍历调用子节点的循环函数
		CCObject *pObj = NULL;
		
		//在火球数组内删除需要删除的火球
		CCArray *pFireBallToDistroy = CCArray::create();
		CCARRAY_FOREACH(m_pArrayFireBall,pObj)
		{
			//调用基类的Update实际上调用的是子类重写的Update，若想调用基类的Update需要在子类的Update中调用。
			CMItemBasic* pItem = dynamic_cast<CMItemBasic*>(pObj);
			if (pItem==NULL)
			{
				CCLog("pItem==NULL");
			}
			if(pItem->OnCallPerFrame(fT)==true)//执行逻辑周期 并判断是否可以移除
			{
				pFireBallToDistroy->addObject(pItem);
			}
		}
		//将需要移除的物品从逻辑队列中移除掉
		m_pArrayFireBall->removeObjectsInArray(pFireBallToDistroy);

		//判断火球与怪物的碰撞
		CCObject* pTempMonsterForCollision = NULL;
		CCARRAY_FOREACH(m_pArrayFireBall,pObj)
		{
			CMItemFireBall* pFireBall = dynamic_cast<CMItemFireBall*>(pObj);
			CC_BREAK_IF(pFireBall==NULL);
			CCARRAY_FOREACH(m_pArrayMonsters,pTempMonsterForCollision)
			{
				CMMonsterBasic* pMonster = dynamic_cast<CMMonsterBasic*>(pTempMonsterForCollision);
				CC_BREAK_IF(pMonster==NULL);
				if (pMonster->boundingBox().containsPoint(pFireBall->getPosition()))
				{
					pFireBall->SetFireBallBoom();
					pMonster->SetDeadByFireBall();
				}
			}
		}

		//删除需要删除的道具
		CCArray *pItemToDistroy = CCArray::create();
		CCARRAY_FOREACH(m_pArrayItems,pObj)
		{
			//调用基类的Update实际上调用的是子类重写的Update，若想调用基类的Update需要在子类的Update中调用。
			CMItemBasic* pItem = dynamic_cast<CMItemBasic*>(pObj);
			if (pItem==NULL)
			{
				CCLog("pItem==NULL");
			}
			if(pItem->OnCallPerFrame(fT)==true)//执行逻辑周期 并判断是否可以移除
			{
				pItemToDistroy->addObject(pItem);
			}
		}
		//将需要移除的物品从逻辑队列中移除掉
		m_pArrayItems->removeObjectsInArray(pItemToDistroy);

		//删除需要删除的怪物
		CCArray *pMonsterToDistroy = CCArray::create();
		CCARRAY_FOREACH(m_pArrayMonsters,pObj)
		{
			//调用基类的Update实际上调用的是子类重写的Update，若想调用基类的Update需要在子类的Update中调用。
			CMMonsterBasic* pMonster = dynamic_cast<CMMonsterBasic*>(pObj);
			if (pMonster==NULL)
			{
				CCLog("pMonster==NULL");
				return;
			}
			if(pMonster->OnCallPerFrame(fT)==true)//执行逻辑周期 并判断是否可以移除
			{
				pMonsterToDistroy->addObject(pMonster);
			}
		}
		//将需要移除的怪从逻辑队列中移除掉
		m_pArrayMonsters->removeObjectsInArray(pMonsterToDistroy);

		//删除需要删除的砖块
		CCArray *pBlockToDistroy = CCArray::create();
		CCARRAY_FOREACH(m_pArrayBlocks,pObj)
		{
			//调用基类的Update实际上调用的是子类重写的Update，若想调用基类的Update需要在子类的Update中调用。
			CMItemBasic* pBlock = dynamic_cast<CMItemBasic*>(pObj);
			if (pBlock==NULL)
			{
				CCLog("pBlock==NULL");
			}
			if(pBlock->OnCallPerFrame(fT)==true)//执行逻辑周期 并判断是否可以移除 返回真 为何不执行括号内语句？
			{
				pBlockToDistroy->addObject(pBlock);
			}
		}
		//将需要移除的砖块从逻辑队列中移除掉
		m_pArrayBlocks->removeObjectsInArray(pBlockToDistroy);

		//执行Mario的循环
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);
		pMario->OnCallPerFrame(fT);		

		//判断Mario是否到达关底
		if (pMario->getPositionX()>getContentSize().width-250)
		{
			CCMoveBy* pMoveDown = CCMoveBy::create(2,ccp(50,0));

			runAction(CCSequence::create(
				pMoveDown,
				CCCallFunc::create(this,callfunc_selector(CMGameMap::PassLevel)),
				NULL));
		}

		//控制Mario的移动
		MarioMove(fT);

		//当Mario死亡需要重置关卡时，如果直接发消息移除当前关卡，会导致循环未完成而成员变量被移除而出错
		//解决方法就是设置成员变量开关，发消息打开开关，在当前循环结束后检测开关状态来移除、
		if (m_bNeedResetStage==true)
		{
			SendMsg(enMsgMarioDead);
			m_bNeedResetStage=false;
		}

		return;
	} while (false);
	CCLog("fun CMGameMap::OnCallPerFrame Error!");
}

enumTileType CMGameMap::TileMapPosToTileType( CCPoint HeroPos)
{
	do 
	{
		//地图瓦片坐标
		int nHeroTilePosX = HeroPos.x;
		int nHeroTilePosY = HeroPos.y;

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
		CCTMXLayer* pCoinLayer = layerNamed("coin");
		CC_BREAK_IF(pCoinLayer==NULL);
		CCTMXLayer* pFlagpoleLayer = layerNamed("flagpole");
		CC_BREAK_IF(pFlagpoleLayer==NULL);
		//根据坐标返回瓦片类型
		CCSprite* pLandSprite = pLandLayer->tileAt(ccp(nHeroTilePosX,nHeroTilePosY));
		if (pLandSprite!=NULL)
		{
			return enTileTypeLand;
		}
		CCSprite* pBlockSprite = pBlockLayer->tileAt(ccp(nHeroTilePosX,nHeroTilePosY));
		if (pBlockSprite!=NULL)
		{
			return enTileTypeBlock;
		}
		CCSprite* pPipeSprite = pPipeLayer->tileAt(ccp(nHeroTilePosX,nHeroTilePosY));	
		if (pPipeSprite!=NULL)
		{
			return enTileTypePipe;
		}
		CCSprite* pCoinSprite = pCoinLayer->tileAt(ccp(nHeroTilePosX,nHeroTilePosY));	
		if (pCoinSprite!=NULL)
		{
			return enTileTypeCoin;
		}
		return enTileTypeNone;
	} while (false);
	CCLog("fun CMGameMap::PosToTileType Error!");
	return enTileTypeNone;
}

void CMGameMap::onExit()
{
	m_pArrayItems->removeAllObjects();
	CC_SAFE_RELEASE(m_pArrayItems);
	m_pArrayMonsters->removeAllObjects();
	CC_SAFE_RELEASE(m_pArrayMonsters);
	m_pArrayBlocks->removeAllObjects();
	CC_SAFE_RELEASE(m_pArrayBlocks);
	m_pArrayFireBall->removeAllObjects();
	CC_SAFE_RELEASE(m_pArrayFireBall);
	CCTMXTiledMap::onExit();
}

cocos2d::CCPoint CMGameMap::TileMapPosToTileMapLayerPos( CCPoint TilePos )
{
	//地图块坐标转换为地图层坐标
	float fPositionX = TilePos.x * getTileSize().width;
	float fPositionY = SCREEN_HEIGHT - CONTROL_UI_HEIGHT - (TilePos.y * getTileSize().height + getTileSize().height);
	return ccp(fPositionX,fPositionY);
}

void CMGameMap::OnMsgReceive( int enMsg,void* pData,int nSize )
{
	switch (enMsg)
	{
	case enMsgFire:			//发射火球
		{
			OnSubMsgFire();
			return;
		}
		break;
	case enMsgBlockDisappear://砖块移除
		{
			OnSubMsgBlockRemove(pData,nSize);
			return;
		}
		break;
	case enMsgItemDisappear://道具移除处理
		{
			OnSubMsgItemRemove(pData,nSize);
			return;
		}
		break;
	case enMsgLevelUp:		//升级
		{
			OnSubMsgMarioLevelUp();
			return;
		}
		break;
	case enMsgBeHurt:		//受到伤害
		{
			OnSubMsgMarioBeHurt();
			return;
		}
		break;
	case enMsgMonsterDisappear:	//怪物移除
		{
			OnSubMsgMonsterDisappear(pData,nSize);
			return;
		}
		break;
	case enMsgMushroomsStamp:	//蘑菇被踩
		{
			OnSubMsgMushroomsStamp(pData,nSize);
			return;
		}
		break;
	case enMsgTortoiseStamp:	//乌龟被踩
		{
			OnSubMsgTortoiseStamp(pData,nSize);
			return;
		}
		break;
	case enMsgBlockBoxHitted:	//砖块被顶
		{
			OnSubMsgBlockBoxHitted(pData,nSize);
			return;
		}
		break;
	case enMsgMarioDead:		//马里奥死亡
		{
			OnSubMsgGameOver();
			return;
		}
		break;
	case enMsgEatCoin:			//吃金币
		{
			OnSubMsgEatCoin();
			return;
		}
		break;
	case enMsgStopCtrl:		//停止更新
		{
			OnSubMsgStopCtrl();
			return;
		}
		break;
	}
}

void CMGameMap::MarioMove(float fT)
{
	do 
	{
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);
		CCPoint CurMarioPos = pMario->getPosition();

		//如果左键按下
		if(m_bIsLeftKeyDown)
		{
			pMario->OnCtrlMove(fT,false);
		}
		//如果右键按下
		if (m_bIsRightKeyDown)
		{
			CCPoint CurMarioPos = pMario->getPosition();
			pMario->OnCtrlMove(fT,true);
			//如果Mario的位置变化了，则地图才会卷动
			if (convertToWorldSpace(pMario->getPosition()).x>120 && abs(pMario->getPositionX()-CurMarioPos.x)>1 &&
				pMario->getPositionX() < (getContentSize().width - SCREEN_WIDTH + 100))
			{
				setPositionX(getPositionX()-100*fT);
			}
		}
		//如果跳跃键按下
		if (m_bIsJumpKeyDown)
		{
			pMario->OnCtrlJump();
		}
		//如果子弹键按下
		if (m_bIsFireKeyDown)
		{
			pMario->OnCtrlFire();
		}
		//如果没有键按下
		if (m_bIsLeftKeyDown==false && m_bIsRightKeyDown==false && m_bIsJumpKeyDown==false)
		{
			pMario->OnCtrlNoAction();
		}

#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)    

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
		if(KEY_DOWN(KEY_KEY_K))
		{
			m_bIsJumpKeyDown = true;
		}
		if(KEY_UP(KEY_KEY_K))
		{
			m_bIsJumpKeyDown = false;
		}
		if(KEY_DOWN(KEY_KEY_J))
		{
			m_bIsFireKeyDown = true;
		}
		if(KEY_UP(KEY_KEY_J))
		{
			m_bIsFireKeyDown = false;
		}
#endif
		
		return;
	} while (false);
	CCLog("fun CMGameMap::MarioMove Error!");
	return;
}

cocos2d::CCPoint CMGameMap::TileMapLayerPosToTileMapPos( CCPoint TileMapLayerPos )
{
	//将层坐标转换为地图瓦片坐标
	int nTileMapPosX = (TileMapLayerPos.x)/getTileSize().width;
	int nTileMapPosY = TILE_MAP_VERTICAL - (int)((TileMapLayerPos.y)/getTileSize().height);

	return ccp(nTileMapPosX,nTileMapPosY);
}

void CMGameMap::OnSubMsgItemRemove( void *pData ,int nSize )
{
	do 
	{
		TCmd_Remove_Item *pCmd = (TCmd_Remove_Item*)pData;
		//参数校验
		CC_BREAK_IF(pCmd!=NULL&&pCmd->pItem==NULL);	
		CC_BREAK_IF(sizeof(TCmd_Remove_Item)!=nSize);
		//从画面移除
		pCmd->pItem->removeFromParent();
		return;
	} while (false);
	CCLOG("Error:  CMGameMap::OnSubMsgItemRemove Run Error!");
}

void CMGameMap::OnSubMsgMarioLevelUp()
{
	do 
	{
		//升级
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);
		pMario->MarioLevelUp();

		return;
	} while (false);
	CCLOG("Error: CMGameMap::OnSubMsgMarioGrowUp");
}

void CMGameMap::OnSubMsgMonsterDisappear( void *pData,int nSize )
{
	do 
	{
		TCmd_Remove_Monster *pCmd = (TCmd_Remove_Monster*)pData;
		//参数校验
		CC_BREAK_IF(pCmd!=NULL&&pCmd->pMonster==NULL);	
		CC_BREAK_IF(sizeof(TCmd_Remove_Monster)!=nSize);

		//从画面移除
		pCmd->pMonster->removeFromParent();

		return;
	} while (false);
	CCLog("fun CMGameMap::OnSubMsgMonsterDisappear Error!");
}

void CMGameMap::OnSubMsgMushroomsStamp( void *pData,int nSize )
{
	do 
	{
		TCmd_Remove_Monster *pCmd = (TCmd_Remove_Monster*)pData;
		//参数校验
		CC_BREAK_IF(pCmd!=NULL&&pCmd->pMonster==NULL);	
		CC_BREAK_IF(sizeof(TCmd_Remove_Monster)!=nSize);
		//从画面移除
		pCmd->pMonster->removeFromParent();

		return;
	} while (false);
	CCLog("fun CMGameMap::OnSubMsgStamp Error!");
}

void CMGameMap::OnSubMsgBlockBoxHitted( void *pData,int nSize )
{
	do 
	{
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);

		TCmd_Block_Box_Hitted *pCmd = (TCmd_Block_Box_Hitted*)pData;
		//参数校验
		CC_BREAK_IF(pCmd!=NULL&&pCmd->pBlockBox==NULL);	
		CC_BREAK_IF(sizeof(TCmd_Remove_Monster)!=nSize);


		CCPoint pPos = ccp(pCmd->pBlockBox->getPosition().x,pCmd->pBlockBox->getPosition().y+getTileSize().height);
		switch (rand()%4)
		{
		case 0:
			{
				if (GetMarioLevel()==enMarioLevelSmall)
				{
					//顶出蘑菇
					CMItemMashrooms* pMashrooms = CMItemMashrooms::CreateItemMashrooms(pPos,getTileSize(),pMario,this,this);
					CC_BREAK_IF(pMashrooms==NULL);
					pMashrooms->setPosition(pPos);
					addChild(pMashrooms,enZOrderFront);
					m_pArrayItems->addObject(pMashrooms);
				}
				else
				{
					//顶出花
					CMItemFlower* pFlower = CMItemFlower::CreateItemFlower(pPos,getTileSize(),pMario,this,this);
					CC_BREAK_IF(pFlower==NULL);
					pFlower->setPosition(pPos);
					addChild(pFlower,enZOrderFront);
					m_pArrayItems->addObject(pFlower);
				}
			}
			break;
		default:
			{
				if (getActionByTag(enTagCoinJumpAction)==NULL)
				{
					//顶出金币
					CCSprite* pCoin = CCSprite::create("coin.png");
					CC_BREAK_IF(pCoin==NULL);
					pCoin->setPosition(pPos);
					pCoin->setAnchorPoint(ccp(-0.3,0));
					addChild(pCoin,enZOrderFront,enTagJumpCoin);

					CCMoveBy* pMoveByUp = CCMoveBy::create(0.2,ccp(0,20));
					CCFadeOut* pFadeOut = CCFadeOut::create(0.1);
					CCSequence* pCoinJump = CCSequence::create(pMoveByUp,pFadeOut,CCCallFunc::create(this, callfunc_selector(CMGameMap::OnSubJumpCoinDisappear)),NULL);
					pCoinJump->setTag(enTagCoinJumpAction);
					pCoin->runAction(pCoinJump);
				}
				SendMsg(enMsgEatCoin);
			}
			break;
		}
		return;
	} while (false);
	CCLog("fun CMGameMap::OnSubMsgBlockBoxHitted Error!");
}

void CMGameMap::OnSubJumpCoinDisappear()
{
	removeChildByTag(enTagJumpCoin);
}

void CMGameMap::OnSubMsgMarioBeHurt()
{
	do 
	{
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);

		pMario->MarioGotHurt();

		return;
	} while (false);
	CCLog("fun CMGameMap::OnSubMsgMarioBeHurt error!");
}

void CMGameMap::OnSubMsgBlockRemove( void *pData ,int nSize )
{
	do 
	{
		TCmd_Remove_Item *pCmd = (TCmd_Remove_Item*)pData;
		//参数校验
		CC_BREAK_IF(pCmd!=NULL&&pCmd->pItem==NULL);	
		CC_BREAK_IF(sizeof(TCmd_Remove_Item)!=nSize);
		//从画面移除
		pCmd->pItem->removeFromParent();
		return;
	} while (false);
	CCLOG("Error:  CMGameMap::OnSubMsgBlockRemove Run Error!");
}

void CMGameMap::OnSubMsgGameOver()
{
	m_bNeedResetStage = true;
}

void CMGameMap::OnSubMsgTortoiseStamp( void *pData,int nSize )
{
	do 
	{
		TCmd_Remove_Monster *pCmd = (TCmd_Remove_Monster*)pData;
		//参数校验
		CC_BREAK_IF(pCmd!=NULL&&pCmd->pMonster==NULL);	
		CC_BREAK_IF(sizeof(TCmd_Remove_Monster)!=nSize);
		//从画面移除
		pCmd->pMonster->removeFromParent();

		return;
	} while (false);
	CCLog("fun CMGameMap::OnSubMsgTortoiseStamp Error!");
}

void CMGameMap::SetLeftKeyDown(bool bIsKeyDown)
{
	m_bIsLeftKeyDown = bIsKeyDown;
}

void CMGameMap::SetRightKeyDown(bool bIsKeyDown)
{
	m_bIsRightKeyDown = bIsKeyDown;
}

void CMGameMap::SetJumpKeyDown(bool bIsKeyDown)
{
	m_bIsJumpKeyDown = bIsKeyDown;
}

void CMGameMap::SetFireKeyDown(bool bIsKeyDown)
{
	m_bIsFireKeyDown = bIsKeyDown;
}

void CMGameMap::PassLevel()
{
	SendMsg(enMsgPassLevel);
}

void CMGameMap::OnSubMsgFire()
{
	do 
	{
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);

		//获得火球移动方向
		bool bFaceToRight;
		switch (pMario->GetMarioStatus())
		{
		case enMarioStatusStandLeft:
		case enMarioStatusRunLeft:
		case enMarioStatusOnAirLeft:
			bFaceToRight = false;
			break;
		case enMarioStatusOnAirRight:
		case enMarioStatusStandRight:
		case enMarioStatusRunRight:
			bFaceToRight = true;
			break;
		default:
			break;
		}
		//建立火球对象并加入火球集合
		CMItemFireBall* pFireBall = CMItemFireBall::CreateItemFireBall(
			pMario->getPosition(),
			CCSizeMake(getTileSize().width/2,getTileSize().height/2),
			pMario,
			this,
			this,bFaceToRight);
		CC_BREAK_IF(pFireBall==NULL);
		pFireBall->setPosition(ccp(pMario->getPositionX(),pMario->getPositionY()+20));
		addChild(pFireBall,enZOrderFront);

		m_pArrayFireBall->addObject(pFireBall);

		return;
	} while (false);
	CCLog("fun CMGameMap::OnSubMsgFire Error！");
}

enumMarioLevel CMGameMap::GetMarioLevel()
{
	do 
	{
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);
		//返回Mario的当前等级
		return pMario->GetMarioLevel();
	} while (false);
	CCLog("fun CMGameMap::GetMarioLevel Error!");
	return enMarioLevelSmall;
}

void CMGameMap::OnSubMsgEatCoin()
{
	SendMsg(enMsgEatCoin);
}

void CMGameMap::OnSubMsgStopCtrl()
{
	do 
	{
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);

		pMario->StopCtrl(true);

		return;
	} while (false);
	CCLog("fun CMGameMap::OnSubMsgStopUpdate error!");
}

