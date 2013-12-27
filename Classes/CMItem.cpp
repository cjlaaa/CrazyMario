#include "CMItem.h"

/************************************************************************/
/* 物品道具基类  
/************************************************************************/
bool CMItemBasic::init( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver )
{
	do 
	{
		CC_BREAK_IF(!CCNode::init());

		//初始化成员变量
		m_pMario = pMario;
		m_bIsActivation = false;
		m_MoveDirection = enMoveLeft;
		m_fDropSpeedPlus = 0;

		return true;
	} while (false);
	CCLog("fun CMItemBasic::init Error!");
	return false;
}

bool CMItemBasic::OnCollisionMario()
{
	do 
	{

		return true;
	} while (false);
	CCLog("fun CMItemBasic::OnCollisionMario Error!");
	return false;
}

void CMItemBasic::OnCallPerFrame(float fT)
{
	do 
	{
		OnCollisionMario();

		//判断当道具离开地图则发消息删除自己
		if (getPositionX()<0 || getPositionY()<0)
		{
			MsgForItem* pData = new MsgForItem;
			pData->pItem = this;
			SendMsg(enMsgItemDisappear,pData,sizeof(MsgForItem));
		}
		return;
	} while (false);
	CCLog("fun CMItemBasic::OnCallPerFrame Error!");
}

/************************************************************************/
/* 金币类                               
/************************************************************************/
CMItemCoin * CMItemCoin::CreateItemIcon( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver )
{
	do 
	{
		CMItemCoin *pItemCoin = new CMItemCoin();
		if (pItemCoin && pItemCoin->init(ptItemPos,szItemSize,pMario,pMsgRecver))
		{
			pItemCoin->autorelease();
			return pItemCoin;
		}
		CC_SAFE_DELETE(pItemCoin);
		return NULL;
	} while (false);
	CCLog("fun CMItemCoin::CreateItemIcon Error!");
	return NULL;
}

bool CMItemCoin::init( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver )
{
	do 
	{
		CC_BREAK_IF(!CMItemBasic::init(ptItemPos,szItemSize,pMario,pMsgRecver));

		CCSprite* pCoin = CCSprite::create("coin.png");
		pCoin->setAnchorPoint(ccp(0,0));
		CC_BREAK_IF(pCoin==NULL);
		addChild(pCoin,enZOrderFront,enTagMainImage);

		m_pReceiver = pMsgRecver;

		setContentSize(szItemSize);

		return true;
	} while (false);
	CCLog("fun CMItemCoin::init Error!");
	return false;
}

bool CMItemCoin::OnCollisionMario()
{
	do 
	{
		CCSprite* pCoin = dynamic_cast<CCSprite*>(getChildByTag(enTagMainImage));
		CC_BREAK_IF(pCoin==NULL);

		//判断马里奥与金币的碰撞
		if (m_pMario->boundingBox().intersectsRect(boundingBox()))
		{
			MsgForItem* pData = new MsgForItem;
			pData->pItem = this;
			SendMsg(enMsgItemDisappear,pData,sizeof(pCoin));
		}

		return true;
	} while (false);
	CCLog("fun CMItemCoin::OnCollisionMario Error!");
	return false;
}

void CMItemCoin::OnCallPerFrame( float fT )
{
	do 
	{
		CMItemBasic::OnCallPerFrame(fT);

		OnCollisionMario();
		return;
	} while (false);
	CCLog("fun CMItemCoin::OnCallPerFrame Error!");
}

/************************************************************************/
/* 砖块类                               
/************************************************************************/
CMItemBlock * CMItemBlock::CreateItemBlock( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver ,enumBlockType eBlockType)
{
	do 
	{
		CMItemBlock *pItemBlock = new CMItemBlock();
		if (pItemBlock && pItemBlock->init(ptItemPos,szItemSize,pMario,pMsgRecver,eBlockType))
		{
			pItemBlock->autorelease();
			return pItemBlock;
		}
		CC_SAFE_DELETE(pItemBlock);
		return NULL;
	} while (false);
	CCLog("fun CMItemBlock::create() Error!");
	return NULL;
}

bool CMItemBlock::init( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver,enumBlockType eBlockType )
{
	do 
	{
		CC_BREAK_IF(!CMItemBasic::init(ptItemPos,szItemSize,pMario,pMsgRecver));

		m_bIsTouched = false;

		//根据砖块类型设定纹理
		CCSprite* pBlock = NULL;
		switch (eBlockType)
		{
		case enBlockTypeNormal:
			{
				pBlock = CCSprite::create("singleblock.png",CCRectMake(0,0,16,16));
			}
			break;
		case enBlockTypeBox:
			{
				pBlock = CCSprite::create("shanshuodecoin.png",CCRectMake(0,0,16,16));
			}
			break;
		case enBlockTypeAddLife:
			{
				pBlock = CCSprite::create("singleblock.png",CCRectMake(0,0,16,16));
			}
			break;
		case enBlockTypeJustBlock:
			{
				pBlock = CCSprite::create("blocktype1.png");
			}
			break;
		}
		pBlock->setAnchorPoint(ccp(0,0));
		CC_BREAK_IF(pBlock==NULL);
		addChild(pBlock,enZOrderFront,enTagMainImage);

		m_pReceiver = pMsgRecver;
		m_eBlockType = eBlockType;

		setContentSize(szItemSize);

		return true;
	} while (false);
	CCLog("fun CMItemBlock::init Error!");
	return false;
}

bool CMItemBlock::OnCollisionMario()
{
	do 
	{
		CCSprite* pBlock = dynamic_cast<CCSprite*>(getChildByTag(enTagMainImage));
		CC_BREAK_IF(pBlock==NULL);

		//被顶
		if (m_pMario->boundingBox().intersectsRect(boundingBox()) &&
			getPositionY()>m_pMario->getPositionY() && 
			abs(m_pMario->getPositionY()-getPositionY())>m_pMario->boundingBox().size.height*0.8 &&
			abs(m_pMario->getPositionX()-getPositionX())<m_pMario->boundingBox().size.width*0.7 && m_bIsTouched==false)
		{
			CCLog("hited");
			m_bIsTouched = true;
			stopAllActions();
			switch (m_pMario->GetStatus())
			{
			case enMarioStatusSmall:
				{
					//砖块跳动动画
					CCActionInterval *pJumpBy = CCJumpBy::create(0.2f, CCPointZero, 
						16*0.5, 1);
					runAction(pJumpBy);

					if (m_eBlockType==enBlockTypeBox)
					{
						//图片换成被顶过的宝箱
						CCTexture2D *pTextureBlock = CCTextureCache::sharedTextureCache()->addImage("brokencoin.png");
						pBlock->setTexture(pTextureBlock);
						pBlock->setTextureRect(CCRectMake(0, 0, 16, 16));

						//发送砖块宝箱被顶消息
						MsgForBlockBoxHitted* pData = new MsgForBlockBoxHitted;
						pData->pBlockBox = this;
						SendMsg(enMsgBlockBoxHitted,pData,sizeof(MsgForBlockBoxHitted));
					}
				}
				break;
			case enMarioStatusBig:
			case enMarioStatusSuper:
				{
					if (m_eBlockType==enBlockTypeBox)
					{
						//图片换成被顶过的宝箱
						CCTexture2D *pTextureBlock = CCTextureCache::sharedTextureCache()->addImage("brokencoin.png");
						pBlock->setTexture(pTextureBlock);
						pBlock->setTextureRect(CCRectMake(0, 0, 16, 16));

						//发送砖块宝箱被顶消息
						MsgForBlockBoxHitted* pData = new MsgForBlockBoxHitted;
						pData->pBlockBox = this;
						SendMsg(enMsgBlockBoxHitted,pData,sizeof(MsgForBlockBoxHitted));
					}
				}
				break;
			}
		}

		return true;
	} while (false);
	CCLog("fun CMItemBlock::OnCollisionMario Error!");
	return false;
}

void CMItemBlock::OnCallPerFrame( float fT )
{
	do 
	{
		CMItemBasic::OnCallPerFrame(fT);

		OnCollisionMario();
		return;
	} while (false);
	CCLog("fun CMItemBlock::OnCallPerFrame Error!");
}

/************************************************************************/
/* 大人蘑                                               
/************************************************************************/
CMItemMashrooms * CMItemMashrooms::CreateItemCMItemMashrooms( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver )
{
	do 
	{
		CMItemMashrooms *pItemMashrooms = new CMItemMashrooms();
		if (pItemMashrooms && pItemMashrooms->init(ptItemPos,szItemSize,pMario,pGameMap,pMsgRecver))
		{
			pItemMashrooms->autorelease();
			return pItemMashrooms;
		}
		CC_SAFE_DELETE(pItemMashrooms);
		return NULL;
	} while (false);
	CCLog("fun CMHero::create() Error!");
	return NULL;
}

bool CMItemMashrooms::init( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver )
{
	do 
	{
		CC_BREAK_IF(!CMItemBasic::init(ptItemPos,szItemSize,pMario,pMsgRecver));

		CCSprite* pMashrooms = CCSprite::create("rewardMushroomSet.png");
		pMashrooms->setAnchorPoint(ccp(0,0));
		CC_BREAK_IF(pMashrooms==NULL);
		addChild(pMashrooms,enZOrderFront,enTagMainImage);

		m_pReceiver = pMsgRecver;
		m_pGameMap = pGameMap;
		m_MoveDirection = enMoveRight;
		m_bIsTouched = false;

		setContentSize(szItemSize);

		return true;
	} while (false);
	CCLog("fun CMItemMashrooms::init Error!");
	return false;
}

bool CMItemMashrooms::OnCollisionMario()
{
	do 
	{
		CCSprite* pMashrooms = dynamic_cast<CCSprite*>(getChildByTag(enTagMainImage));
		CC_BREAK_IF(pMashrooms==NULL);

		//与马里奥的碰撞
		if (m_pMario->boundingBox().intersectsRect(boundingBox()) && m_bIsTouched==false)
		{
			CCLog("Eat");
			MsgForItem* pData = new MsgForItem;
			pData->pItem = this;
			SendMsg(enMsgLevelUp,pData,sizeof(MsgForItem));

			m_bIsTouched = true;
		}

		return true;
	} while (false);
	CCLog("fun CMItemMashrooms::OnCollisionMario Error!");
	return false;
}

void CMItemMashrooms::OnCallPerFrame( float fT )
{
	do 
	{
		CMItemBasic::OnCallPerFrame(fT);

		OnCollisionMario();

		CCSprite* pTileSprite1 = NULL;
		CCSprite* pTileSprite2 = NULL;
		CCSprite* pTileSprite3 = NULL;
		//移动与碰撞
		if (m_MoveDirection == enMoveLeft)
		{
			//用怪物左方的2个瓦片来判断移动碰撞
			pTileSprite1 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height));
			pTileSprite2 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height/2));
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				m_MoveDirection = enMoveRight;
			}
			else
			{
				setPositionX(getPositionX()-1);
			}
		}
		else if(m_MoveDirection == enMoveRight)
		{
			//用怪物右方的2个瓦片来判断移动碰撞
			pTileSprite1 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height));
			pTileSprite2 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height/2));
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				m_MoveDirection = enMoveLeft;
			}
			else
			{
				setPositionX(getPositionX()+1);
			}
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//用怪物下方的三个瓦片来判断掉落碰撞
		pTileSprite1 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width/2,getPositionY()));
		pTileSprite2 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+COLLISION_POS_ADJUSTMENT,getPositionY()));
		pTileSprite3 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width-COLLISION_POS_ADJUSTMENT,getPositionY()));
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			//掉落速度归零
			m_fDropSpeedPlus = 0;
		}
		else
		{
			setPositionY(getPositionY()-m_fDropSpeedPlus);
			//掉落加速度
			m_fDropSpeedPlus += DROP_SPEED_PLUS;
		}

		return;
	} while (false);
	CCLog("fun CMItemMashrooms::OnCallPerFrame Error!");
}
