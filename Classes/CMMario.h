#ifndef _CM_MARIO_H_
#define _CM_MARIO_H_

#include "CMPublicDefine.h"

class CMMario : public CCNode,public CMSender
{
public:
	static CMMario* CreateHero();
	CCRect BoundingBox();
private:
	CCRect RectForCollision;
	bool Init();
};

#endif