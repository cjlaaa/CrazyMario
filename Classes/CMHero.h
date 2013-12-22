#ifndef _CM_HERO_H_
#define _CM_HERO_H_

#include "CMPublicDefine.h"

class CMHero : public CCSprite,public CMSender
{
public:
	static CMHero* CreateHero();
private:
	bool Init();
};

#endif