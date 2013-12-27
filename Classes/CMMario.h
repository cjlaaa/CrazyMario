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
	void OnCallPerFrame(float fT);
private:
	CCTexture2D *pTextureBig;
	CCTexture2D *pTextureSuper;
	CCTexture2D *pTextureSmall;	
	bool Init();
	//Mario状态
	enumMarioStatus m_eMarioStatus;
	enum 
	{
		enTagMario,
	};
};

#endif