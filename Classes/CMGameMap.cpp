#include "CMGameMap.h"


CMGameMap* CMGameMap::CreateGameMap(const char* pFileName)
{
	do 
	{
		CMGameMap* pPlayer = new CMGameMap;
		if (pPlayer && pPlayer->initWithTMXFile(pFileName))
		{
			pPlayer->Init();
			pPlayer->autorelease();
			return pPlayer;
		}
		delete pPlayer;
	} while (false);
	CCLog("Fun CMGameMap::CreateGameMap Error!");
	return NULL;
}

bool CMGameMap::Init()
{
	do 
	{
// 		CCTMXLayer* pCloudLayer = layerNamed("cloud");
// 		CC_BREAK_IF(pCloudLayer==NULL);
// 		//addChild(pCloudLayer);
// 		CCTMXLayer* pBlockLayer = layerNamed("block");
// 		CC_BREAK_IF(pBlockLayer==NULL);
// 		//addChild(pBlockLayer);
// 		CCTMXLayer* pPipeLayer = layerNamed("pipe");
// 		CC_BREAK_IF(pPipeLayer==NULL);
// 		//addChild(pPipeLayer);
// 		CCTMXLayer* pLandLayer = layerNamed("land");
// 		CC_BREAK_IF(pLandLayer==NULL);
// 		//addChild(pLandLayer);
// 		CCTMXLayer* pTrapLayer = layerNamed("trap");
// 		CC_BREAK_IF(pTrapLayer==NULL);
// 		//addChild(pTrapLayer);
// 		CCTMXLayer* pObjectLayer = layerNamed("objects");
// 		CC_BREAK_IF(pObjectLayer==NULL);
// 		//addChild(pObjectLayer);
// 		CCTMXLayer* pCoinLayer = layerNamed("coin");
// 		CC_BREAK_IF(pCoinLayer==NULL);
// 		//addChild(pCoinLayer);
// 		CCTMXLayer* pFlagpoleLayer = layerNamed("flagpole");
// 		CC_BREAK_IF(pFlagpoleLayer==NULL);
// 		//addChild(pFlagpoleLayer);

		return true;
	} while (false);
	CCLog("Fun CMGameMap::Init Error!");
	return false;
}

TileType CMGameMap::HeroPosToTileType( CCPoint HeroPos )
{
	return enTileTypeBlock;
}
