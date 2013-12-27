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
class CMGameMap;
class CMItemBasic: public CCNode,public CMSender
{
protected:
	enum 
	{
		enTagMainImage,
	};
protected:
	CMMario		*m_pMario;
	CMGameMap	*m_pGameMap;
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario() = 0;//与Mario碰撞的事件
	virtual void OnCallPerFrame(float fT);
public:
};
/************************************************************************/
/* 金币类                               
/************************************************************************/
class CMItemCoin:public CMItemBasic
{
public:
	static CMItemCoin *CreateItemIcon(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario();
	virtual void OnCallPerFrame(float fT);
};

/************************************************************************/
/* 砖块类                               
/************************************************************************/
class CMItemBlock:public CMItemBasic
{
public:
	static CMItemBlock *CreateItemBlock(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver,enumBlockType eBlockType);
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver,enumBlockType eBlockType);

	virtual bool OnCollisionMario();
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