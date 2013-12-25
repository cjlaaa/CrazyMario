#ifndef _CM_MARIO_H_
#define _CM_MARIO_H_

#include "CMPublicDefine.h"

class CMMario : public CCNode,public CMSender
{
public:
	static CMMario* CreateHero();
private:
	bool Init();
};

#endif