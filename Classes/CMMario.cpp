#include "CMMario.h"

CMMario* CMMario::CreateHero()
{
	do 
	{
		CMMario *pMario = new CMMario();
		if (pMario && pMario->Init())
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

bool CMMario::Init()
{
	do 
	{
		CCSprite* pMario = CCSprite::create("small_walk_1.png");
		CC_BREAK_IF(pMario==NULL);
		pMario->setAnchorPoint(ccp(0, 0));
		addChild(pMario,enZOrderFront,enTagMario);

		setContentSize(MARIO_COLLISION_SIZE_SMALL);
		m_eSetMarioStatus = enMarioStatusStandRight;
		m_eCurMarioStatus = enMarioStatusStandRight;
		m_eSetMarioLevel = enMarioLevelSmall;
		m_eCurMarioLevel = enMarioLevelSmall;

		return true;
	} while (false);
	CCLog("fun CMHero::Init Error!");
	return false;
}

void CMMario::OnCallPerFrame( float fT )
{
	do 
	{
		//如果“设置状态”等于“当前状态”，则返回，防止反复设置动画
		if (m_eSetMarioStatus==m_eCurMarioStatus && m_eSetMarioLevel==m_eCurMarioLevel)
		{
			return;
		}
		else
		{
			m_eCurMarioLevel = m_eSetMarioLevel;
			removeChildByTag(enTagMario);
			switch (m_eCurMarioLevel)
			{
			case enMarioLevelSmall:
				{
					switch (m_eSetMarioStatus)
					{
					case enMarioStatusStandRight:	//小 右 站
						{
							CCSprite* pMario = CCSprite::create("small_walk_1.png");
							CC_BREAK_IF(pMario==NULL);
							pMario->setAnchorPoint(ccp(0, 0));
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusStandLeft:	//小 左 站
						{
							CCSprite* pMario = CCSprite::create("small_walk_1.png");
							CC_BREAK_IF(pMario==NULL);
							pMario->setAnchorPoint(ccp(0, 0));
							pMario->setRotationY(180);
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusRunRight:	//小 右 走
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("small_run");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusRunLeft:	//小 左 走
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("small_run");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
							pMario->setRotationY(180);
						}
						break;
					case enMarioStatusJumpRight:	//小 右 跳
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("small_jump");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusJumpLeft:	//小 左 跳
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("small_jump");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
							pMario->setRotationY(180);
						}
						break;
					}
					setContentSize(MARIO_COLLISION_SIZE_SMALL);
				}
				break;
			case enMarioLevelBig:
				{
					switch (m_eSetMarioStatus)
					{
					case enMarioStatusStandRight:	//大 右 站
						{
							CCSprite* pMario = CCSprite::create("normal_walk_1.png");
							CC_BREAK_IF(pMario==NULL);
							pMario->setAnchorPoint(ccp(0, 0));
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusStandLeft:	//大 左 站
						{
							CCSprite* pMario = CCSprite::create("normal_walk_1.png");
							CC_BREAK_IF(pMario==NULL);
							pMario->setAnchorPoint(ccp(0, 0));
							pMario->setRotationY(180);
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusRunRight:	//大 右 走
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("big_run");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusRunLeft:	//大 左 走
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("big_run");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
							pMario->setRotationY(180);
						}
						break;
					case enMarioStatusJumpRight:	//大 右 跳
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("big_jump");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusJumpLeft:	//大 左 跳
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("big_jump");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
							pMario->setRotationY(180);
						}
						break;
					}
					setContentSize(MARIO_COLLISION_SIZE_BIG);
				}
				break;
			case enMarioLevelSuper:
				{
					switch (m_eSetMarioStatus)
					{
					case enMarioStatusStandRight:	//超 右 站
						{
							CCSprite* pMario = CCSprite::create("lv3_walk_1.png");
							CC_BREAK_IF(pMario==NULL);
							pMario->setAnchorPoint(ccp(0, 0));
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusStandLeft:	//超 左 站
						{
							CCSprite* pMario = CCSprite::create("lv3_walk_1.png");
							CC_BREAK_IF(pMario==NULL);
							pMario->setAnchorPoint(ccp(0, 0));
							pMario->setRotationY(180);
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusRunRight:	//超 右 走
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("max_run");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusRunLeft:	//超 左 走
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("max_run");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
							pMario->setRotationY(180);
						}
						break;
					case enMarioStatusJumpRight:	//大 右 跳
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("max_jump");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
						}
						break;
					case enMarioStatusJumpLeft:	//大 左 跳
						{
							//构造一个ccbi文件读取器
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
							//读取动画文件
							CCNode *pMario = pCcbReader->readNodeGraphFromFile("ccbResources/mario.ccbi", this);
							CC_BREAK_IF(pMario==NULL);
							pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("max_jump");	
							//读完之后，立刻释放即可
							pCcbReader->release();
							CC_BREAK_IF(pMario==NULL);
							addChild(pMario,enZOrderFront,enTagMario);
							pMario->setRotationY(180);
						}
						break;
					}
					setContentSize(MARIO_COLLISION_SIZE_BIG);
				}
				break;
			}
			m_eCurMarioStatus = m_eSetMarioStatus;
		}
		return;
	} while (false);
	CCLog("fun CMMario::OnCallPerFrame Error!");
}

void CMMario::SetMarioLevel( enumMarioLevel eMarioLevel )
{
	m_eSetMarioLevel = eMarioLevel;
}

enumMarioLevel CMMario::GetMarioLevel()
{
	return m_eCurMarioLevel;
}

void CMMario::SetMarioStatus( enumMarioStatus eMarioStatus )
{
	m_eSetMarioStatus = eMarioStatus;
}

enumMarioStatus CMMario::GetMarioStatus()
{
	return m_eCurMarioStatus;
}
