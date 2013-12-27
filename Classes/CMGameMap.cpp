#include "CMGameMap.h"

CMGameMap* CMGameMap::CreateGameMap(const char* pFileName)
{
	do 
	{
		CMGameMap* pPlayer = new CMGameMap;
		if (pPlayer && pPlayer->initWithTMXFile(pFileName))
		{
			pPlayer->Init();
			pPlayer->autorelease();
			return pPlayer;
		}
		delete pPlayer;
	} while (false);
	CCLog("Fun CMGameMap::CreateGameMap Error!");
	return NULL;
}

bool CMGameMap::Init()
{
	do 
	{
		CCLog("initing Game Map...");

		//初始化成员变量
		m_fMapMove = 0;
		m_fDropSpeedPlus = 0;
		m_fJumpSpeed = 0;
		m_fSpeed = MOVE_SPEED;
		m_bIsLeftKeyDown = false;
		m_bIsRightKeyDown = false;
		m_bIsJumpKeyDown = false;
		m_bIsHeroDead = false;

		//初始化游戏对象数组
		m_pArrayItems = CCArray::create();
		m_pArrayItems->retain();
		m_pArrayItemForDelete = CCArray::create();
		m_pArrayItemForDelete->retain();
		m_pArrayMonsters = CCArray::create();
		m_pArrayMonsters->retain();
		m_pArrayMonstersForDelete = CCArray::create();
		m_pArrayMonstersForDelete->retain();

		//初始化Mario
		CMMario* pMario = CMMario::CreateHero();
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
				if (TileMapPosToTileType(ccp(i,j),m_fMapMove)==enTileTypeCoin)
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
				if (TileMapPosToTileType(ccp(i,j),m_fMapMove)==enTileTypeBlock)
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
					m_pArrayItems->addObject(pBlock);
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
			return pBlockSprite;
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

void CMGameMap::OnCallPerFrame(float dt)
{
	do 
	{
		//删除需要被删除的道具
		m_pArrayItems->removeObjectsInArray(m_pArrayItemForDelete);
		CCObject *pObj = NULL;
		//因为集合只是保存指针，还需要在父节点移除，所以需要遍历移除。
		CCARRAY_FOREACH(m_pArrayItemForDelete,pObj)
		{
			CMItemBasic* pItem = dynamic_cast<CMItemBasic*>(pObj);
			if (pItem==NULL)
			{
				CCLog("pCoin==NULL!");
			}
			pItem->removeFromParent();
		}
		m_pArrayItemForDelete->removeAllObjects();

		//删除需要被删除的怪物
		m_pArrayMonsters->removeObjectsInArray(m_pArrayMonstersForDelete);	//这句的m_pArrayMonsters被写成了m_pArrayItems也不报错，坑死我了！2013-12-27 15:51
		pObj = NULL;
		CCARRAY_FOREACH(m_pArrayMonstersForDelete,pObj)
		{
			CMMonsterBasic* pMonster = dynamic_cast<CMMonsterBasic*>(pObj);
			if (pMonster==NULL)
			{
				CCLog("pMonster==NULL");
			}
			pMonster->removeFromParent();
		}
		m_pArrayMonstersForDelete->removeAllObjects();

		//遍历调用子节点的循环函数
		pObj = NULL;
		CCARRAY_FOREACH(m_pArrayItems,pObj)
		{
			//调用基类的Update实际上调用的是子类重写的Update，若想调用基类的Update需要在子类的Update中调用。
			CMItemBasic* pItem = dynamic_cast<CMItemBasic*>(pObj);
			if (pItem==NULL)
			{
				CCLog("pMonster==NULL");
			}
			pItem->OnCallPerFrame(dt);
		}
		CCARRAY_FOREACH(m_pArrayMonsters,pObj)
		{
			//调用基类的Update实际上调用的是子类重写的Update，若想调用基类的Update需要在子类的Update中调用。
			CMMonsterBasic* pMonster = dynamic_cast<CMMonsterBasic*>(pObj);
			if (pMonster==NULL)
			{
				CCLog("pMonster==NULL");
				return;
			}
			pMonster->OnCallPerFrame(dt);
		}
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);
		pMario->OnCallPerFrame(dt);		

		MarioMove();
		return;
	} while (false);
	CCLog("fun CMGameMap::OnCallPerFrame Error!");
}

enumTileType CMGameMap::TileMapPosToTileType( CCPoint HeroPos,float fMapMove )
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
	m_pArrayItemForDelete->removeAllObjects();
	CC_SAFE_RELEASE(m_pArrayItemForDelete);
	m_pArrayMonstersForDelete->removeAllObjects();
	CC_SAFE_RELEASE(m_pArrayMonstersForDelete);
	CCTMXTiledMap::onExit();
}

cocos2d::CCPoint CMGameMap::TileMapPosToTileMapLayerPos( CCPoint TilePos )
{
	float fPositionX = TilePos.x * getTileSize().width;
	float fPositionY = SCREEN_HEIGHT - CONTROL_UI_HEIGHT - (TilePos.y * getTileSize().height + getTileSize().height);
	return ccp(fPositionX,fPositionY);
}

void CMGameMap::OnMsgReceive( int enMsg,void* pData,int nSize )
{
	CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
	if (pMario==NULL)
	{
		CCLog("pMario==NULL in fun CMGameMap::OnMsgReceive Error!");
	}

	switch (enMsg)
	{
	case enMsgItemDisappear:
		{
			if (sizeof(MsgForItem)!=nSize)
			{
				CCAssert(false,"sizeof(MsgForCoinCollision)!=nSize");
			}
			m_pArrayItemForDelete->addObject(((MsgForItem*)pData)->pItem);
		}
		break;
	case enMsgLevelUp:
		{
			if (sizeof(MsgForItem)!=nSize)
			{
				CCAssert(false,"sizeof(MsgForCoinCollision)!=nSize");
			}
			m_pArrayItemForDelete->addObject(((MsgForItem*)pData)->pItem);

			//Mario升级
			if (pMario->GetStatus()==enMarioStatusSmall)
			{
				pMario->SetStatus(enMarioStatusBig);
			}
			else if (pMario->GetStatus()==enMarioStatusBig)
			{
				pMario->SetStatus(enMarioStatusSuper);
			}
		}
		break;
	case enMsgBeHurt:
		{
			if (sizeof(MsgForItem)!=nSize)
			{
				CCAssert(false,"sizeof(MsgForCoinCollision)!=nSize");
			}

			//Mario降级
			if (pMario->GetStatus()==enMarioStatusBig)
			{
				pMario->SetStatus(enMarioStatusSmall);
			}
			else if (pMario->GetStatus()==enMarioStatusSuper)
			{
				pMario->SetStatus(enMarioStatusBig);
			}
		}
		break;
	case enMsgMonsterDisappear:
		{
			if (sizeof(MsgForMonsterDisappear)!=nSize)
			{
				CCAssert(false,"sizeof(MsgForMonsterDisappear)!=nSize");
			}
			m_pArrayMonstersForDelete->addObject(((MsgForMonsterDisappear*)pData)->pMonster);
		}
		break;
	case enMsgStamp:
		{
			if (sizeof(MsgForMonsterDisappear)!=nSize)
			{
				CCAssert(false,"sizeof(MsgForMonsterDisappear)!=nSize");
			}
			m_pArrayMonstersForDelete->addObject(((MsgForMonsterDisappear*)pData)->pMonster);
		}
		break;
	case enMsgBlockBoxHitted:
		{
			if (sizeof(MsgForBlockBoxHitted)!=nSize)
			{
				CCAssert(false,"sizeof(MsgForBlockBoxHitted)!=nSize");
			}

			CCPoint pMashroomsPos = ccp(((MsgForBlockBoxHitted*)pData)->pBlockBox->getPosition().x,((MsgForBlockBoxHitted*)pData)->pBlockBox->getPosition().y+getTileSize().height+10);
			CMItemMashrooms* pMashrooms = CMItemMashrooms::CreateItemCMItemMashrooms(pMashroomsPos,getTileSize(),pMario,this,this);
			if (pMashrooms==NULL)
			{
				CCLog("pMashrooms==NULL!");
			}
			pMashrooms->setPosition(pMashroomsPos);
			addChild(pMashrooms,enZOrderFront);
			m_pArrayItems->addObject(pMashrooms);
		}
		break;
	}
}

cocos2d::CCPoint CMGameMap::TileMapLayerPosToWorldPos( CCPoint TileMapLayerPos,float m_fMapMove )
{
	float fPositionY = TileMapLayerPos.y;
	float fPositionX = TileMapLayerPos.x - m_fMapMove;
	return ccp(fPositionX,fPositionY);
}

float CMGameMap::GetMapMove()
{
	return m_fMapMove;
}

void CMGameMap::MarioMove()
{
	do 
	{
		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);
		CCPoint CurMarioPos = pMario->getPosition();

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

		//判断马里奥是否落坑死亡
		float temp = pMario->getPositionY();
		if(pMario->getPositionY()/getTileSize().height<0)
		{
			CCDirector* pDirector = CCDirector::sharedDirector();
			CCScene *pScene = CMGameScene::CreateGameScene();
			pDirector->replaceScene(pScene);
			return;
		}

		//Mario的移动及碰撞
		CCSprite* pTileSprite1 = NULL;
		CCSprite* pTileSprite2 = NULL;
		CCSprite* pTileSprite3 = NULL;
		//根据变量控制Mario移动及动作
		if (m_bIsLeftKeyDown)
		{
			//用Mario左方的三个瓦片来判断后退碰撞
			pTileSprite1 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX(),pMario->getPositionY()+pMario->boundingBox().size.height));
			pTileSprite2 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX(),pMario->getPositionY()+pMario->boundingBox().size.height/2));
			pTileSprite3 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX(),pMario->getPositionY()));
			if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
			{
				pMario->setPosition(CurMarioPos);
			}
			else
			{
				if (pMario->getPositionX()>m_fMapMove)
				{
					pMario->setPositionX(pMario->getPositionX()-m_fSpeed);
				}
			}
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		if (m_bIsRightKeyDown)
		{
			//用Mario右方的三个瓦片来判断前进碰撞
			pTileSprite1 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width,pMario->getPositionY()+pMario->boundingBox().size.height));
			pTileSprite2 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width,pMario->getPositionY()+pMario->boundingBox().size.height/2));
			pTileSprite3 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width,pMario->getPositionY()));
			if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
			{
				pMario->setPosition(CurMarioPos);
			}
			else
			{
				if (getContentSize().width - m_fMapMove<=SCREEN_WIDTH)
				{
					pMario->setPositionX(pMario->getPositionX()+m_fSpeed);
				}
				else if (pMario->getPositionX() - m_fMapMove>100)
				{
					setPositionX(getPositionX()-m_fSpeed);
					pMario->setPositionX(pMario->getPositionX()+m_fSpeed);
					m_fMapMove += m_fSpeed;
				}
				else 
				{
					pMario->setPositionX(pMario->getPositionX()+m_fSpeed);
				}
			}
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//用Mario下方的三个瓦片来判断掉落碰撞
		pTileSprite1 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width/2,pMario->getPositionY()));
		pTileSprite2 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX()+COLLISION_POS_ADJUSTMENT,pMario->getPositionY()));
		pTileSprite3 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width-COLLISION_POS_ADJUSTMENT,pMario->getPositionY()));
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			pMario->setPosition(CurMarioPos);
			pMario->setPositionY(pMario->getPositionY()+1);
			//掉落速度归零
			m_fDropSpeedPlus = 0;
			//跳跃起始速度
			m_fJumpSpeed = JUMP_START_SPEED;
		}
		else
		{
			pMario->setPositionY(pMario->getPositionY()-m_fDropSpeedPlus);
			//掉落加速度
			m_fDropSpeedPlus += DROP_SPEED_PLUS;
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//用Mario上方的三个瓦片来判断头顶碰撞
		pTileSprite1 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width/2,pMario->getPositionY()+pMario->boundingBox().size.height));
		pTileSprite2 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX()+COLLISION_POS_ADJUSTMENT,pMario->getPositionY()+pMario->boundingBox().size.height));
		pTileSprite3 = TileMapLayerPosToTileSprite(ccp(pMario->getPositionX()+pMario->boundingBox().size.width-COLLISION_POS_ADJUSTMENT,pMario->getPositionY()+pMario->boundingBox().size.height));
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			pMario->setPosition(CurMarioPos);
			pMario->setPositionY(pMario->getPositionY()-1);
			//跳跃速度归零
			m_fJumpSpeed = 0;
			//HitBlock(ccp(pMario->getPositionX()+pMario->boundingBox().size.width/2,pMario->getPositionY()+pMario->boundingBox().size.height));
		}
		else
		{
			//跳跃
			if (m_bIsJumpKeyDown)
			{
				pMario->setPositionY(pMario->getPositionY()+m_fJumpSpeed);
				//跳跃递减速度
				m_fJumpSpeed -= JUMP_SPEED_MINUS;
			}
		}

		return;
	} while (false);
	CCLog("fun CMGameMap::MarioMove Error!");
	return;
}
// 
// void CMGameMap::HitBlock(CCPoint TileMapLayerPos)
// {
// 	do 
// 	{
// 		CMMario* pMario = dynamic_cast<CMMario*>(getChildByTag(enTagMario));
// 		CC_BREAK_IF(pMario==NULL);
// 
// 		//解析得到当前砖块的属性
// 		CCTMXLayer* pBlockLayer = layerNamed("block");
// 		CC_BREAK_IF(pBlockLayer==NULL);
// 		int GID = pBlockLayer->tileGIDAt(TileMapLayerPosToTileMapPos(TileMapLayerPos));
// 		CCDictionary *pDic = propertiesForGID(GID);
// 		CC_BREAK_IF(pDic==NULL);
// 		CCString *strBlockType = (CCString*)pDic->objectForKey("blockType");
// 		int nBlockType = strBlockType->intValue();
// 
// 		//对不同形态的砖块进行不同的反馈
// 		CCSprite* pBlockForHit = TileMapLayerPosToTileSprite(TileMapLayerPos);
// 		CC_BREAK_IF(pBlockForHit==NULL);
// 		switch (nBlockType)
// 		{
// 		case enBlockTypeNormal:		//普通砖块
// 			{
// 				switch (pMario->GetStatus())
// 				{
// 				case enMarioStatusSmall:
// 					{
// 						pBlockForHit->stopAllActions();
// 						CCActionInterval *pJumpBy = CCJumpBy::create(0.2f, CCPointZero, 
// 						this->getTileSize().height*0.5, 1);
// 						pBlockForHit->runAction(pJumpBy);
// 					}
// 					break;
// 				case enMarioStatusBig:
// 				case enMarioStatusSuper:
// 					{
// 						pBlockLayer->removeTileAt(TileMapLayerPosToTileMapPos(TileMapLayerPos));
// 					}
// 					break;
// 				}
// 			}
// 			break;
// 		case enBlockTypeBox:		//宝箱
// 			{
// 				pBlockForHit->stopAllActions();
// 				CCActionInterval *pJumpBy = CCJumpBy::create(0.2f, CCPointZero, 
// 					this->getTileSize().height*0.5, 1);
// 				pBlockForHit->runAction(pJumpBy);
// 				
// 				CCSprite* pTempCoin = CCSprite::create("coin.png");
// 				CC_BREAK_IF(pTempCoin==NULL);
// 				pTempCoin->setPosition(pBlockForHit->getPosition());
// 				pTempCoin->setAnchorPoint(ccp(0,0));
// 				addChild(pTempCoin);
// 				CCActionInterval *pJumpCoin = CCJumpBy::create(0.16f, ccp(0, this->getTileSize().height),
// 					this->getTileSize().height*1.5, 1);
// 				pTempCoin->runAction(pJumpCoin);
// 			}
// 			break;
// 		case enBlockTypeAddLife:	//隐藏砖块
// 			{
// 
// 			}
// 			break;
// 		}
// 		
// 		//pBlockLayer->removeTileAt(TilePos);
// 
// 		return;
// 	} while (false);
// 	CCLog("fun CMGameMap::HitBlock Error!");
// }

cocos2d::CCPoint CMGameMap::TileMapLayerPosToTileMapPos( CCPoint TileMapLayerPos )
{
	//将层坐标转换为地图瓦片坐标
	int nTileMapPosX = (TileMapLayerPos.x)/getTileSize().width;
	int nTileMapPosY = TILE_MAP_VERTICAL - (int)((TileMapLayerPos.y)/getTileSize().height);

	return ccp(nTileMapPosX,nTileMapPosY);
}
