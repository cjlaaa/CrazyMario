#ifndef _CM_MONSTER_HEAD_FILE_
#define _CM_MONSTER_HEAD_FILE_
#include "CMPublicDefine.h"
#include "CMMario.h"
#include "CMGameMap.h"

/************************************************************************/
/* 怪物基类                                                         
 * 1,约定接口
 * 2,做基本设定															*/
/************************************************************************/
class CMGameMap;
enum enMonsterDeadType
{
	enMonsterDeadTypeFall,		//掉坑
	enMonsterDeadTypeStamp,		//踩踏
	enMonsterDeadTypeFire,		//火球
};
class CMMonsterBasic: public CCNode,public CMSender
{
protected:	
	CMMario				*m_pMario;
	CMGameMap			*m_pGameMap;
	enMoveDirection		 m_MoveDirection;		//移动方向
	bool				 m_bIsActivation;		//是否激活（与Mario到达一定距离则激活移动）
	float				 m_fDropSpeedPlus;		//掉落加速度
	bool				 m_bIsTouched;			//是否被碰触过
protected:
	virtual bool init(CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario() = 0;//与Mario碰撞的事件

	virtual void Dead(enMonsterDeadType DeadType);
public:
	//检查与Mario的碰撞 并把消息发送给MsgRecver
	virtual void OnCallPerFrame(float fT);
protected:
	enum 
	{
		enTagMainImage,
	};
};

/************************************************************************/
/* 蘑菇怪                                                                     */
/************************************************************************/
class CMMonsterMushrooms:public CMMonsterBasic
{
public:
	static CMMonsterMushrooms *CreateMonsterMushrooms(CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);
protected:
	virtual bool init(CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario();
	virtual void OnCallPerFrame(float fT);
};
/************************************************************************/
/* 乌龟                                                               */
/********1****************************************************************/
// class CMMonsterTortoise:public CMMonsterBasic
// {
// public:
// 	static CMMonsterTortoise *CreateMonsterTortoise(CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);
// protected:
// 	virtual bool init(CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver);
// 
// 	virtual bool OnCollisionMario();
// 	virtual void OnCallPerFrame(float fT);
// };
/************************************************************************/
/* 飞鱼                                                                    */
/************************************************************************/

/************************************************************************/
/* 花                                                                    */
/************************************************************************/

#endif