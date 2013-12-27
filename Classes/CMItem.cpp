#include "CMItem.h"

/************************************************************************/
/* 物品道具基类  
/************************************************************************/
bool CMItemBasic::init( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver )
{
	do 
	{
		CC_BREAK_IF(!CCNode::init());

		m_pMario = pMario;

		//注册Update函数
		this->schedule(schedule_selector(CMItemBasic::OnCallPerFrame));

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
			MsgForCoinCollision* pData = new MsgForCoinCollision;
			pData->pCoin = this;
			SendMsg(enMsgCoinCollision,pData,sizeof(pCoin));
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
			abs(m_pMario->getPositionX()-getPositionX())<m_pMario->boundingBox().size.width*0.7)
		{
			stopAllActions();
			CCActionInterval *pJumpBy = CCJumpBy::create(0.2f, CCPointZero, 
				16*0.5, 1);
			runAction(pJumpBy);
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
/* 变身蘑菇                                               
/************************************************************************/
CMItemMashrooms * CMItemMashrooms::CreateItemCMItemMashrooms( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver )
{
	do 
	{
		CMItemMashrooms *pItemMashrooms = new CMItemMashrooms();
		if (pItemMashrooms && pItemMashrooms->init(ptItemPos,szItemSize,pMario,pMsgRecver))
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

bool CMItemMashrooms::init( CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver )
{
	do 
	{

		return true;
	} while (false);
	CCLog("fun CMItemMashrooms::init Error!");
	return false;
}

bool CMItemMashrooms::OnCollisionMario()
{
	do 
	{

		return true;
	} while (false);
	CCLog("fun CMItemMashrooms::OnCollisionMario Error!");
	return false;
}
