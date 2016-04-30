#ifndef _CM_MARIO_H_
#define _CM_MARIO_H_

#include "CMPublicDefine.h"
#include "CMGameMap.h"

#define		_MARIO_BASIC_SPEED_PER_SEC_			100				//定义马里奥运动速度
#define		_MARIO_BASIC_JUMP_HEIGHT_			70.f			//定义马里奥的跳跃高度
#define		_MARIO_BASIC_JUMP_TIME_				1.f				//定义马里奥跳跃时间
#define		_MARIO_FIRE_BALL_CD_TIME_			30.f			//定义马里奥cd时间

class CMGameMap;
class CMMario : public CCNode,public CMSender
{
protected:
	enum
	{
		enTagOfActionJump,		//跳跃运动的Tag
	};
	enum 
	{
		enTagMario,
	};
protected:
	CMGameMap			*m_pGameMap;			//游戏地图指针

	//马里奥掉落相关
	bool				m_bFell;				//马里奥是否处于跌落状态
	float				m_fFellCurSpeed;		//马里奥掉落当前速度
	bool				m_bFaceToRight;			//Mario掉落后的朝向
	float				m_fMarioJumpMaxHeight;	//通过这个变量，来计算Mario的跳起最大高度，当Mario开始下落时，则停止Jump动作，开始落体		
	//马里奥掉落相关 结束

	float				m_fFireBallCdTime;		//火球冷却时间
	float				m_fGodModeLeftTime;		//无敌模式剩余时间
	enumMarioStatus		m_eCurMarioStatus;		//Mario当前的状态(站立 跑动 悬空)
	enumMarioLevel		m_eCurMarioLevel;		//Mario当前的等级(小 大 可射击)
	CCBReader			*m_pCcbReader;			//马里奥的动作管理指针
	bool				m_bDeadAnimationPlaying;//死亡动画播放完毕
	bool				m_bIsStopCtrl;			//是否停止控制（在播放Mario死亡动画时需要停止控制）	
public:
	static CMMario* CreateHero(CMGameMap *pGameMap,enumMarioLevel &eMarioLevel);

	void OnCallPerFrame(float fT);			//fT为距离上一次调用周期函数的时间差 在这个函数里处理 马里奥的运动 和与地图的碰撞关系

	//马里奥操作指令
	void OnCtrlMove(float fT,bool bToRight);		//移动
	void OnCtrlJump();								//跳跃
	void OnCtrlFire();								//开火
	void OnCtrlNoAction();							//没有任何指令
	void StopCtrl(bool bIsStopCtrl);				//设置是否停止对Mario的控制		

	//马里奥升级
	void MarioLevelUp();
	//马里奥受伤
	void MarioGotHurt();
	//马里奥死亡
	void MarioDead();
	//马里奥死亡回调
	void OnCallDeadAnimationFinished();
	//马里奥掉落
	void MarioFell();
public:
	//设置Mario等级
	void SetMarioLevel(enumMarioLevel eMarioLevel);
	//获得Mario等级
	enumMarioLevel GetMarioLevel();
	//设置Mario状态
	void SetMarioStatus(enumMarioStatus eMarioStatus);
	//获得Mario状态
	enumMarioStatus GetMarioStatus();
	//Mario翻转
	void MarioTurn(bool bFaceRight);

protected:	
	bool Init(CMGameMap *pGameMap,enumMarioLevel &eMarioLevel);
	

};

#endif