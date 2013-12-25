#ifndef _CM_ITEM_H_
#define _CM_ITEM_H_
#include "CMPublicDefine.h"
#include "CMMario.h"
#include "CMGameMap.h"
/************************************************************************/
/* 物品道具基类                                                         
 * 1,约定接口
 * 2,做基本设定															*/
/************************************************************************/
class CMItemBasic: public CCNode,public CMSender
{
protected:
	enum 
	{
		enTagMainImage,
	};
protected:
	CMMario		*m_pMario;
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario() = 0;//与Mario碰撞的事件
	//检查与Mario的碰撞 并把消息发送给MsgRecver
	virtual void OnCallPerFrame(float fT);
public:
};
/************************************************************************/
/* 金币类                                               
 * 1，金币的数值不同(在TileMap中编辑)*/
/************************************************************************/
class CMItemCoin:public CMItemBasic
{
public:
	static CMItemCoin *CreateItemIcon(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario();
	//检查与Mario的碰撞 并把消息发送给MsgRecver
	virtual void OnCallPerFrame(float fT);
};
/************************************************************************/
/* 变身蘑菇                                               
/************************************************************************/
class CMItemMashrooms:public CMItemBasic
{		
public:
	static CMItemMashrooms *CreateItemCMItemMashrooms(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario();
};


#endif