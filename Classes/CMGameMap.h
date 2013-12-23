#ifndef _CM_GAME_MAP_H_
#define _CM_GAME_MAP_H_

#include "CMPublicDefine.h"

class CMGameMap : public cocos2d::CCTMXTiledMap
{
public:
	static CMGameMap* CreateGameMap(const char* pFileName);
	// 返回地图指定位置上的图块类型，用作碰撞检测
	CCSprite* MarioPosToTileSprite(CCPoint HeroPos,float fMapMove);
	
private:
	virtual bool Init();

	CREATE_FUNC(CMGameMap);
};

#endif
