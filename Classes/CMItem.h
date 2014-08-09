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
class CMMario;
class CMItemBasic: public CCNode,public CMSender
{
protected:	
	CMMario				*m_pMario;
	CMGameMap			*m_pGameMap;
	enMoveDirection		 m_MoveDirection;		//移动方向
	bool				 m_bIsActivation;		//是否激活（与Mario到达一定距离则激活移动）
	float				 m_fDropSpeedPlus;		//掉落速度
	float				 m_fJumpSpeed;			//跳跃速度
	bool				 m_bIsTouched;			//是否被碰触过
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario() = 0;//与Mario碰撞的事件
public:
	virtual bool OnCallPerFrame(float fT);
protected:
	enum 
	{
		enTagMainNode,
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
	virtual bool OnCallPerFrame(float fT);
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
	//砖块爆裂动画播放完毕的回调函数
	void OnCallDeadAnimationFinished();
	virtual bool OnCollisionMario();
	virtual bool OnCallPerFrame(float fT);
};

/************************************************************************/
/* 变身蘑菇                                               
/************************************************************************/
class CMItemMashrooms:public CMItemBasic
{		
protected:
public:
	static CMItemMashrooms *CreateItemMashrooms(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario();
	virtual bool OnCallPerFrame(float fT);
};

/************************************************************************/
/* 火球	                                             
/************************************************************************/
class CMItemFireBall:public CMItemBasic
{		
protected:
public:
	static CMItemFireBall *CreateItemFireBall(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver,bool bFaceRight);
	void SetFireBallBoom();
protected:
	bool m_bGoDown;				//是否在掉落
	bool m_bNeedBoom;			//需要播放爆炸动画
	bool m_bGoRight;			//火球射击方向
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver,bool bFaceRight);

	virtual bool OnCollisionMario();
	virtual bool OnCallPerFrame(float fT);
	void OnCallBoomAnimationFinished();
};

/************************************************************************/
/* 花                                        
/************************************************************************/
class CMItemFlower:public CMItemBasic
{		
protected:
public:
	static CMItemFlower *CreateItemFlower(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario();
	virtual bool OnCallPerFrame(float fT);
};

#endif