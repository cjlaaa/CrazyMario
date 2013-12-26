#ifndef _CM_MARIO_H_
#define _CM_MARIO_H_

#include "CMPublicDefine.h"

class CMMario : public CCNode,public CMSender
{
public:
	static CMMario* CreateHero();
	//设置Mario状态
	void SetStatus(enumMarioStatus eMarioStatus);
	//获得Mario状态
	enumMarioStatus GetStatus();
private:
	bool Init();
	enumMarioStatus m_eMarioStatus;
	void OnCallPerFrame(float fT);
	enum 
	{
		enTagMario,
	};
};

#endif