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
	CMMario				*m_pMario;
	CMGameMap			*m_pGameMap;
	enMoveDirection		 m_MoveDirection;		//移动方向
	bool				 m_bIsActivation;		//是否激活（与Mario到达一定距离则激活移动）
	float				 m_fDropSpeedPlus;		//掉落加速度
	bool				 m_bIsTouched;			//是否被碰触过
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario() = 0;//与Mario碰撞的事件
public:
	virtual void OnCallPerFrame(float fT);
protected:
	enum 
	{
		enTagMainImage,
	};
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
protected:
	enumBlockType	m_eBlockType;		//砖块类型
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
protected:
public:
	static CMItemMashrooms *CreateItemCMItemMashrooms(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario();
	virtual void OnCallPerFrame(float fT);
};


#endif