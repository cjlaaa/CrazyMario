#include "CMMario.h"

CMMario* CMMario::CreateHero()
{
	do 
	{
		CMMario *pHero = new CMMario();
		if (pHero && pHero->Init())
		{
			pHero->autorelease();
			return pHero;
		}
		CC_SAFE_DELETE(pHero);
		return NULL;
	} while (false);
	CCLog("fun CMHero::create() Error!");
	return NULL;
}

bool CMMario::Init()
{
	do 
	{
		CC_BREAK_IF(!CCSprite::initWithFile("smallWalkRight.png", CCRectMake(0, 0, 14, 16)));
		setAnchorPoint(ccp(0, 0));

		return true;
	} while (false);
	CCLog("fun CMHero::Init Error!");
	return false;
}
