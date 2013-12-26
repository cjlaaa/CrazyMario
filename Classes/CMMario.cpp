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
		CCLog("initing Mario...");
		CCSprite* pMainBody = CCSprite::create("smallWalkRight.png", CCRectMake(0, 0, 14, 16));
		CC_BREAK_IF(pMainBody==NULL);
		pMainBody->setAnchorPoint(ccp(0, 0));
		addChild(pMainBody);

		setContentSize(pMainBody->getContentSize());

		return true;
	} while (false);
	CCLog("fun CMHero::Init Error!");
	return false;
}
