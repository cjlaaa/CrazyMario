#include "CMMario.h"

CMMario* CMMario::CreateHero()
{
	do 
	{
		CMMario *pMario = new CMMario();
		if (pMario && pMario->Init())
		{
			pMario->autorelease();
			return pMario;
		}
		CC_SAFE_DELETE(pMario);
		return NULL;
	} while (false);
	CCLog("fun CMHero::create() Error!");
	return NULL;
}

bool CMMario::Init()
{
	do 
	{
		CCSprite* pMainBody = CCSprite::create("smallWalkRight.png", CCRectMake(0, 0, 14, 16));
		CC_BREAK_IF(pMainBody==NULL);
		pMainBody->setAnchorPoint(ccp(0, 0));
		addChild(pMainBody,enZOrderFront,enTagMario);

		setContentSize(pMainBody->getContentSize());
		m_eMarioStatus = enMarioStatusSmall;

		pTextureBig = CCTextureCache::sharedTextureCache()->addImage("walkRight.png");
		pTextureSuper = CCTextureCache::sharedTextureCache()->addImage("WalkRight_fire.png");
		pTextureSmall = CCTextureCache::sharedTextureCache()->addImage("smallWalkRight.png");	

		return true;
	} while (false);
	CCLog("fun CMHero::Init Error!");
	return false;
}

void CMMario::SetStatus(enumMarioStatus eMarioStatus)
{
	m_eMarioStatus = eMarioStatus;
}

void CMMario::OnCallPerFrame( float fT )
{
	do 
	{
		CCSprite* pMario = dynamic_cast<CCSprite*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMario==NULL);

		switch (m_eMarioStatus)
		{
		case enMarioStatusSmall:
			{
				if (pMario->getTexture()==pTextureSmall)
				{
					return;
				}
				pMario->setTexture(pTextureSmall);
				pMario->setTextureRect(CCRectMake(0, 0, 14, 16));
			}
			break;
		case enMarioStatusBig:
			{
				if (pMario->getTexture()==pTextureBig)
				{
					return;
				}
				pMario->setTexture(pTextureBig);
				pMario->setTextureRect(CCRectMake(0, 0, 18, 32));
			}
			break;
		case enMarioStatusSuper:
			{
				if (pMario->getTexture()==pTextureSuper)
				{
					return;
				}
				pMario->setTexture(pTextureSuper);
				pMario->setTextureRect(CCRectMake(0, 0, 18, 32));
			}
			break;
		}

		return;
	} while (false);
	CCLog("fun CMMario::OnCallPerFrame Error!");
}

enumMarioStatus CMMario::GetStatus()
{
	return m_eMarioStatus;
}
