#include "CMMario.h"

#define _CCB_MARIO_SMALL_HOLD_		"small_hold"
#define _CCB_MARIO_BIG_HOLD_		"big_hold"
#define _CCB_MARIO_MAX_HOLD_		"max_hold"

#define _CCB_MARIO_SMALL_RUN_		"small_run"
#define _CCB_MARIO_BIG_RUN_			"big_run"
#define _CCB_MARIO_MAX_RUN_			"max_run"

#define _CCB_MARIO_SMALL_JUMP_		"small_jump"
#define _CCB_MARIO_BIG_JUMP_		"big_jump"
#define _CCB_MARIO_MAX_JUMP_		"max_jump"

#define _CCB_MARIO_SMALL_TO_BIG_	"small_to_big"
#define _CCB_MARIO_BIG_TO_MAX_		"big_to_max"
#define _CCB_MARIO_MAX_TO_SMALL_	"big_to_small"

CMMario* CMMario::CreateHero(CMGameMap *pGameMap,enumMarioLevel &eMarioLevel)
{
	do 
	{
		CMMario *pMario = new CMMario();
		if (pMario && pMario->Init(pGameMap,eMarioLevel))
		{
			pMario->autorelease();
			return pMario;
		}
		CC_SAFE_DELETE(pMario);
		return NULL;
	} while (false);
	CCLog("fun CMHero::create() Error!");
	return NULL;
}

bool CMMario::Init(CMGameMap *pGameMap,enumMarioLevel &eMarioLevel)
{
	do 
	{
		//初始化参数
		m_pGameMap = pGameMap;
		m_pReceiver = pGameMap;
		m_fFireBallCdTime = _MARIO_FIRE_BALL_CD_TIME_;
		m_fFellCurSpeed = 0.f;
		m_bFell = false;
		m_bFaceToRight = true;
		m_fGodModeLeftTime = 0.f;
		m_bDeadAnimationPlaying = false;
		m_bIsStopCtrl = false;

		//根据马里奥等级设置状态和碰撞
		switch (eMarioLevel)
		{
		case enMarioLevelSmall:
			{
				setContentSize(CCSizeMake(16,16));
				m_eCurMarioLevel = enMarioLevelSmall;
			}
			break;
		case enMarioLevelBig:
			{
				setContentSize(CCSizeMake(32,32));
				m_eCurMarioLevel = enMarioLevelBig;
			}
			break;
		case enMarioLevelSuper:
			{
				setContentSize(CCSizeMake(32,32));
				m_eCurMarioLevel = enMarioLevelSuper;
			}
			break;
		}
		//构建CCB读取器
		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		m_pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);

		//读取动画文件
		CCNode *pMario = m_pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
		addChild(pMario,enZOrderFront,enTagMario);
		pMario->setPositionX(8);

		//默认状态为向右站立
		m_eCurMarioStatus = enMarioStatusStandRight;
		m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_HOLD_);

		return true;
	} while (false);
	CCLog("fun CMHero::Init Error!");
	return false;
}

void CMMario::OnCallPerFrame( float fT )
{
	do 
	{
		CC_BREAK_IF(m_pGameMap==NULL);

		//修正Mario坠落
		if(getActionByTag(enTagOfActionJump)!=NULL)
		{
			if(getPositionY()>m_fMarioJumpMaxHeight)//若满足此条件 则说明Mario在上升
			{
				m_fMarioJumpMaxHeight = getPositionY();
			}
			else //说明Mario已经跳起到最高高度
			{
				stopActionByTag(enTagOfActionJump);
				m_bFell = true;
			}
		}

		//判断马里奥是否落坑死亡
		if(getPositionY()/m_pGameMap->getTileSize().height<0)
		{
			MarioDead();
			return;
		}

		//若马里奥处于悬空状态(可能由[跳跃]或[坠落]引起)
		if(m_eCurMarioStatus == enMarioStatusOnAirLeft||m_eCurMarioStatus == enMarioStatusOnAirRight)
		{
			//先判断头顶正上方是否有砖块
			CCSprite *pTileTopMid = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+COLLISION_POS_ADJUSTMENT,getPositionY()+boundingBox().size.height));
			CCSprite *pTileTopLeft = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width/2,getPositionY()+boundingBox().size.height));
			CCSprite *pTileTopRight = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width-COLLISION_POS_ADJUSTMENT,getPositionY()+boundingBox().size.height));

			//若头顶找到砖块
			if ((pTileTopMid!=NULL || pTileTopLeft!=NULL || pTileTopRight!=NULL)&&m_bFell==false)//发送破坏砖块消息(宝箱被顶也是这里处理)
			{
				//进入下落状态
				MarioFell();
			}

			//用Mario下方的三个瓦片来判断掉落碰撞
			CCSprite* pTileBottomLeft = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width*0.25f,getPositionY()));
			CCSprite* pTileBottomMid = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width*0.5f,getPositionY()));
			CCSprite* pTileBottomRight = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width*0.75f,getPositionY()));
			//若脚下三个位置有方块 或 跳跃动作完成且不处于下落状态
			if (pTileBottomLeft!=NULL || pTileBottomMid!=NULL || pTileBottomRight!=NULL||(getActionByTag(enTagOfActionJump)==NULL&&m_bFell==false))
			{
				//因为有强制瞬间离地的跳跃动作，所以这里没问题
				stopActionByTag(enTagOfActionJump);

				//防止下落速度过快导致陷进地里
				if (pTileBottomMid!=NULL)
				{
					if (getPositionY()<pTileBottomMid->getPositionY()+pTileBottomMid->getContentSize().height)
					{
						setPositionY(getPositionY()+abs(getPositionY()-(pTileBottomMid->getPositionY()+pTileBottomMid->getContentSize().height))-0.1);
					}
				}

				//坠落速度归零
				m_fFellCurSpeed = 0;
				m_bFell = false;
				//根据状态播放动画
				switch(m_eCurMarioLevel)
				{
				case enMarioLevelSmall:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_HOLD_);
					}break;
				case enMarioLevelBig:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_BIG_HOLD_);
					}break;
				case enMarioLevelSuper:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_MAX_HOLD_);
					}break;
				}
				//根据朝向设置状态
				if (m_bFaceToRight)
				{
					SetMarioStatus(enMarioStatusStandRight);
				}
				else
				{
					SetMarioStatus(enMarioStatusStandLeft);
				}
			}
			else if(m_bFell==true)
			{
				//坠落
				m_fFellCurSpeed += DROP_SPEED_PLUS;
				setPositionY(getPositionY() - m_fFellCurSpeed);
			}
		}
		else 	//若 跑动 或者 站立 
		{			
			//用Mario下方的三个瓦片来判断掉落碰撞
			CCSprite* pTileBottomLeft = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width*0.25f,getPositionY()));
			CCSprite* pTileBottomMid = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width*0.5f,getPositionY()));
			CCSprite* pTileBottomRight = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width*0.75f,getPositionY()));
			//若脚下三个位置都没有方块
			if (pTileBottomLeft==NULL && pTileBottomMid==NULL&& pTileBottomRight==NULL)
			{
				//坠落速度归零
				m_bFell = true;
				//加速度
				m_fFellCurSpeed += DROP_SPEED_PLUS;
				setPositionY(getPositionY() - m_fFellCurSpeed);
				//根据状态播放动画
				switch(m_eCurMarioLevel)
				{
				case enMarioLevelSmall:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_JUMP_);
					}break;
				case enMarioLevelBig:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_BIG_JUMP_);
					}break;
				case enMarioLevelSuper:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_MAX_JUMP_);
					}break;
				}
				//根据朝向设置状态
				if (m_bFaceToRight)
				{
					SetMarioStatus(enMarioStatusOnAirRight);
				}
				else
				{
					SetMarioStatus(enMarioStatusOnAirLeft);
				}
			}

		}

		//若有无敌时间，则时间减少
		if (m_fGodModeLeftTime>0)
		{
			m_fGodModeLeftTime--;
		}

		//若火球有冷却时间，则时间减少
		if(m_fFireBallCdTime>0)
		{
			m_fFireBallCdTime--;
		}

		return;
	}while (false);
	CCLog("fun CMMario::OnCallPerFrame Error!");
}

void CMMario::SetMarioLevel( enumMarioLevel eMarioLevel )
{
	m_eCurMarioLevel = eMarioLevel;
}

enumMarioLevel CMMario::GetMarioLevel()
{
	return m_eCurMarioLevel;
}

void CMMario::SetMarioStatus( enumMarioStatus eMarioStatus )
{
	m_eCurMarioStatus = eMarioStatus;
}

enumMarioStatus CMMario::GetMarioStatus()
{
	return m_eCurMarioStatus;
}

void CMMario::MarioLevelUp()
{
	//根据状态播放动画
	switch(m_eCurMarioLevel)
	{
	case enMarioLevelSmall:
		{
			m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_JUMP_);
		}break;
	case enMarioLevelBig:
		{
			m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_BIG_JUMP_);
		}break;
	case enMarioLevelSuper:
		{
			m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_MAX_JUMP_);
		}break;
	}

	//Mario升级
	if (GetMarioLevel()==enMarioLevelSmall)
	{
		SetMarioLevel(enMarioLevelBig);
		setContentSize(CCSizeMake(16,32));
		m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_TO_BIG_);
	}
	else if (GetMarioLevel()==enMarioLevelBig)
	{
		SetMarioLevel(enMarioLevelSuper);
		m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_BIG_TO_MAX_);
	}}

void CMMario::MarioGotHurt()
{
	do 
	{
		//若处于无敌状态
		if(m_fGodModeLeftTime>0.f)return;

		//根据状态播放动画
		switch(m_eCurMarioLevel)
		{
		case enMarioLevelSmall:
			{
				m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_JUMP_);
			}break;
		case enMarioLevelBig:
			{
				m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_BIG_JUMP_);
			}break;
		case enMarioLevelSuper:
			{
				m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_MAX_JUMP_);
			}break;
		}

		//Mario降级
		if (GetMarioLevel()!=enMarioLevelSmall)
		{
			SetMarioLevel(enMarioLevelSmall);
			setContentSize(CCSizeMake(16,16));
			m_fGodModeLeftTime = 180;

			m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_MAX_TO_SMALL_);
		}
		else
		{
			//播放死亡动画
			if (m_bDeadAnimationPlaying==false)
			{
				removeChildByTag(enTagMario);
				SendMsg(enMsgStopCtrl);
				//ccbi读取
				//构造一个ccbi文件读取器
				CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
				//读取动画文件
				CCNode *pNode = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
				CC_BREAK_IF(pNode==NULL);
				pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("small_die");	
				pCcbReader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(CMMario::OnCallDeadAnimationFinished));
				//读完之后，立刻释放即可
				pCcbReader->release();
				CC_BREAK_IF(pNode==NULL);
				pNode->setAnchorPoint(ccp(0,0));
				addChild(pNode,enZOrderFront);//这里不能设置Tag，否者Mario节点会被挤占，导致无法调用回调函数
				pNode->setPosition(ccp(0,0));
				m_bDeadAnimationPlaying=true;
			}
		}
		return;
	} while (false);
	CCLog("CMMario::MarioGotHurt Error!");
}

void CMMario::OnCtrlMove(float fT,bool bToRight)
{
	if (m_bIsStopCtrl==false)
	{
		if(m_pGameMap==NULL)
		{
			CCAssert(false,"Error:No Map!");
			return;
		}
		//判断是否可以移动
		bool bCanMove = true;
		CCSprite* pTileSpriteTop = NULL;
		CCSprite* pTileSpriteMid = NULL;
		CCSprite* pTileSpriteBottom = NULL;
		CCPoint ptPosTop = bToRight?ccp(getPositionX()+boundingBox().size.width,getPositionY()+boundingBox().size.height):
			ccp(getPositionX(),getPositionY()+boundingBox().size.height);
		CCPoint ptPosMid = bToRight?ccp(getPositionX()+boundingBox().size.width,getPositionY()+boundingBox().size.height/2):
			ccp(getPositionX(),getPositionY()+boundingBox().size.height/2);
		CCPoint ptPosBottom = bToRight?ccp(getPositionX()+boundingBox().size.width,getPositionY()+5):
			ccp(getPositionX(),getPositionY()+5);

		//检查马里奥前进方向是否有障碍(检查 上 中 下 三个方向)
		pTileSpriteTop = m_pGameMap->TileMapLayerPosToTileSprite(ptPosTop);
		pTileSpriteMid = m_pGameMap->TileMapLayerPosToTileSprite(ptPosMid);
		pTileSpriteBottom = m_pGameMap->TileMapLayerPosToTileSprite(ptPosBottom);
		if (pTileSpriteTop!=NULL || pTileSpriteMid!=NULL || pTileSpriteBottom!=NULL)
		{
			bCanMove = false;//若在前进方向找到了砖块 则 禁止移动
		}
		//判断马里奥是否移动出屏幕边界
		if(bToRight==false)//判断是否左出界
		{
			CCPoint ptMarioInWorld = m_pGameMap->convertToWorldSpace(getPosition());
			if(ptMarioInWorld.x<=0)
			{
				bCanMove = false;
			}
		}
		else//判断是否右出界（假定，实际不应该出现这种情况）
		{
			CCPoint ptMarioInWorld = m_pGameMap->convertToWorldSpace(getPosition());
			if(ptMarioInWorld.x>=SCREEN_WIDTH-boundingBox().size.width)
			{
				bCanMove = false;
			}
		}

		//根据马里奥当前状态 来处理
		switch(m_eCurMarioStatus)
		{
		case enMarioStatusStandLeft:		//如果是待机状态
		case enMarioStatusStandRight:
			{
				//根据状态播放动画
				switch(m_eCurMarioLevel)
				{
				case enMarioLevelSmall:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_RUN_);
					}break;
				case enMarioLevelBig:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_BIG_RUN_);
					}break;
				case enMarioLevelSuper:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_MAX_RUN_);
					}break;
				}

				//变更位移
				if(bCanMove==true)
				{
					float fCurPosX = getPositionX();
					if(bToRight)
					{
						fCurPosX += _MARIO_BASIC_SPEED_PER_SEC_*fT;	//向右移动
						m_bFaceToRight = true;
					}
					else
					{
						fCurPosX -= _MARIO_BASIC_SPEED_PER_SEC_*fT;	//向左移动
						m_bFaceToRight = false;
					}
					setPositionX(fCurPosX);
				}

				//改变马里奥状态
				m_eCurMarioStatus = bToRight?enMarioStatusRunRight:enMarioStatusRunLeft;

				//设置马里奥面对方向
				MarioTurn(bToRight);

			}break;
		case enMarioStatusRunLeft:	//若马里奥正在向左移动
		case enMarioStatusRunRight:	//这里设计成允许直接变向，如果要增加额外动作效果，可在这里区分
			{
				//变更位移
				if(bCanMove==true)
				{
					float fCurPosX = getPositionX();
					if(bToRight)
					{
						fCurPosX += _MARIO_BASIC_SPEED_PER_SEC_*fT;	//向右移动
					}
					else
					{
						fCurPosX -= _MARIO_BASIC_SPEED_PER_SEC_*fT;	//向左移动
					}
					setPositionX(fCurPosX);
				}
				//改变马里奥状态
				m_eCurMarioStatus = bToRight?enMarioStatusRunRight:enMarioStatusRunLeft;

				//设置马里奥面对方向
				MarioTurn(bToRight);
			}break;
		case enMarioStatusOnAirLeft:		//同方向跳跃正常位移 反方向跳跃 不改变面对方向 且位移量减半
		case enMarioStatusOnAirRight:		//
			{
				if(bCanMove==true)
				{
					//变更位移
					float fCurPosX = getPositionX();

					//计算位移量
					float fMoveDis = 0.f;
					if((m_eCurMarioStatus==enMarioStatusOnAirLeft&&bToRight==true)||(m_eCurMarioStatus==enMarioStatusOnAirRight&&bToRight==false))
					{
						fMoveDis = _MARIO_BASIC_SPEED_PER_SEC_*fT*0.5f;	//若反向 则 位移量减半
					}
					else
					{
						fMoveDis = _MARIO_BASIC_SPEED_PER_SEC_*fT;			//若同向 则 正常位移
					}

					if(bToRight)
					{
						fCurPosX += fMoveDis;	//向右移动
					}
					else
					{
						fCurPosX -= fMoveDis;	//向左移动
					}
					setPositionX(fCurPosX);
				}
			}break;
		}
	}
}
void CMMario::OnCtrlJump()
{
	if (m_bIsStopCtrl==false)
	{
		//根据马里奥当前状态 来处理
		switch(m_eCurMarioStatus)
		{
		case enMarioStatusStandLeft:	//待机 和 跑动 时 马里奥允许跳跃
		case enMarioStatusStandRight:
		case enMarioStatusRunLeft:	
		case enMarioStatusRunRight:	
			{
				//如果动画在播放，则停止动画，防止内存泄露
				CCNode *pMarioBody = dynamic_cast<CCNode*>(getChildByTag(enTagMario));
				if(pMarioBody!=NULL)
				{
					pMarioBody->stopAllActions();
				}
				//播放跳跃动画
				switch(m_eCurMarioLevel)
				{
				case enMarioLevelSmall:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_JUMP_);
					}break;
				case enMarioLevelBig:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_BIG_JUMP_);
					}break;
				case enMarioLevelSuper:
					{
						m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_MAX_JUMP_);
					}break;
				}

				if(getActionByTag(enTagOfActionJump)==NULL)
				{
					//强制瞬间离地
					setPositionY(getPositionY()+10.f);
					//增加跳跃运动
					CCJumpTo *pJump = CCJumpTo::create(_MARIO_BASIC_JUMP_TIME_,ccp(getPositionX(),getPositionY()-10.f),_MARIO_BASIC_JUMP_HEIGHT_,1);
					if(pJump==NULL)
					{
						CCAssert(false,"Can't Create Jump!");
						return;
					}
					//为Jump设置独立的Tag
					pJump->setTag(enTagOfActionJump);
					runAction(pJump);

					m_fMarioJumpMaxHeight = -100.f;
				}
				//将Mario状态设置为悬空
				SetMarioStatus(m_bFaceToRight?enMarioStatusOnAirRight:enMarioStatusOnAirLeft);

			}break;
		}
	}
}
//射击
void CMMario::OnCtrlFire()
{
	if (m_fFireBallCdTime==0 && GetMarioLevel()==enMarioLevelSuper)
	{
		//发送射击指令给主场景
		SendMsg(enMsgFire);
		//重置冷却时间
		m_fFireBallCdTime = _MARIO_FIRE_BALL_CD_TIME_;
	}
}
//当没有指令时
void CMMario::OnCtrlNoAction()
{
	//根据马里奥当前状态 来处理
	switch(m_eCurMarioStatus)
	{
	case enMarioStatusRunLeft:	//若马里奥正在向左移动
	case enMarioStatusRunRight:	//这里设计成允许直接变向，如果要增加额外动作效果，可在这里区分
		{
			//如果动画在播放，则停止动画，防止内存泄露
			CCNode *pMarioBody = dynamic_cast<CCNode*>(getChildByTag(enTagMario));
			if(pMarioBody!=NULL)
			{
				pMarioBody->stopAllActions();
			}
			//这里需要重置状态，否者会引起移动时不播放移动动画的情况
			m_eCurMarioStatus = m_bFaceToRight?enMarioStatusStandRight:enMarioStatusStandLeft;
			//根据状态播放动画
			switch(m_eCurMarioLevel)
			{
			case enMarioLevelSmall:
				{
					m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_SMALL_HOLD_);
				}break;
			case enMarioLevelBig:
				{
					m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_BIG_HOLD_);
				}break;
			case enMarioLevelSuper:
				{
					m_pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed(_CCB_MARIO_MAX_HOLD_);
				}break;
			}
		}break;
	}
}
void CMMario::MarioTurn( bool bFaceRight )
{
	do 
	{
		CCNode *pMarioRoot = dynamic_cast<CCNode*>(getChildByTag(enTagMario));
		CC_BREAK_IF(pMarioRoot==NULL);
		//遍历所有精灵的子节点 使其翻转
		CCArray *pArrayChild = pMarioRoot->getChildren();
		CC_BREAK_IF(pArrayChild==NULL);

		CCObject *pItem=NULL;
		CCARRAY_FOREACH(pArrayChild,pItem)
		{
			CCSprite *pSprite = dynamic_cast<CCSprite*>(pItem);
			if(pSprite==NULL)continue;
			pSprite->setFlipX(!bFaceRight);
		}
		return;
	} while (false);
	CCLOG("CMMario::MarioTurn Run Error!");
}

void CMMario::MarioFell()
{
	do 
	{
		//若跳跃动作未完成
		if(getActionByTag(enTagOfActionJump)!=NULL)
		{
			stopActionByTag(enTagOfActionJump);
		}
		//设置默认下落状态
		m_bFell = true;
		m_fFellCurSpeed = 0.f;
		return;
	} while (false);
	CCLOG("CMMario::MarioFell Run Error!");
}

void CMMario::MarioDead()
{
	do 
	{
		OnCallDeadAnimationFinished();
		return ;
	} while (false);
	CCLog("fun CMMario::MarioDead Error!");
}

void CMMario::OnCallDeadAnimationFinished()
{
	SendMsg(enMsgMarioDead);
}

void CMMario::StopCtrl(bool bIsStopCtrl)
{
	m_bIsStopCtrl = bIsStopCtrl;
}
