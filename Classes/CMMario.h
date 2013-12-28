#ifndef _CM_MARIO_H_
#define _CM_MARIO_H_

#include "CMPublicDefine.h"

class CMMario : public CCNode,public CMSender
{
public:
	static CMMario* CreateHero();
	void OnCallPerFrame(float fT);

	//设置Mario等级
	void SetMarioLevel(enumMarioLevel eMarioLevel);
	//获得Mario等级
	enumMarioLevel GetMarioLevel();
	//设置Mario状态
	void SetMarioStatus(enumMarioStatus eMarioStatus);
	//获得Mario状态
	enumMarioStatus GetMarioStatus();
protected:	
	bool Init();
	//Mario设置的状态
	enumMarioStatus m_eSetMarioStatus;
	//Mario当前的状态
	enumMarioStatus m_eCurMarioStatus;
	//Mario设置的等级
	enumMarioLevel m_eSetMarioLevel;
	//Mario当前的等级
	enumMarioLevel m_eCurMarioLevel;
	enum 
	{
		enTagMario,
	};
};

#endif