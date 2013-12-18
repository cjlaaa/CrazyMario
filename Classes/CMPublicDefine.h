#ifndef _CM_PUBLIC_DEFINE_H_
#define _CM_PUBLIC_DEFINE_H_
#include "cocos2d.h"
USING_NS_CC;
#define _SCREEN_WIDTH_ CCDirector::sharedDirector()->getWinSize().width
#define _SCREEN_HEIGHT_ CCDirector::sharedDirector()->getWinSize().height

class CMReceiver
{
public:
	virtual void OnMsgReceive(int enMsg,void* pData,int nSize) = 0;
};

class CMSender
{
protected:
	CMReceiver* m_pReceiver;
public:
	void SetReceiver(CMReceiver* pReceiver)
	{
		m_pReceiver=pReceiver;
	}
	void SendMsg(int enMsg,void* pData,int nSize)
	{
		m_pReceiver->OnMsgReceive(enMsg,pData,nSize);
	}
};

#endif