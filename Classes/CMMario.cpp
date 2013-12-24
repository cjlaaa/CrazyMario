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
		addChild(pMainBody);

		RectForCollision = CCRectMake(getPositionX(),getPositionY(),pMainBody->boundingBox().size.width,pMainBody->boundingBox().size.height);

		return true;
	} while (false);
	CCLog("fun CMHero::Init Error!");
	return false;
}

cocos2d::CCRect CMMario::BoundingBox()
{
	return RectForCollision;
}
