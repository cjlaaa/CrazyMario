#ifndef _CM_ITEM_H_
#define _CM_ITEM_H_
#include "CMPublicDefine.h"
#include "CMMario.h"
#include "CMGameMap.h"
/************************************************************************/
/* ��Ʒ���߻���                                                         
 * 1,Լ���ӿ�
 * 2,�������趨															*/
/************************************************************************/
class CMItemBasic: public CCNode,public CMSender
{
protected:
	CMMario		*m_pMario;
	//Mario�ĵ�ͼ������
	CCPoint		m_MarioTileMapLayerPos;
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario() = 0;//��Mario��ײ���¼�
	//�����Mario����ײ ������Ϣ���͸�MsgRecver
	virtual void OnCallPerFrame(float fT);
public:
	//ˢ�½�ҵ���������
	void RefreshCoinWorldPosition(CCPoint MarioPos);
};
/************************************************************************/
/* �����                                               
 * 1����ҵ���ֵ��ͬ(��TileMap�б༭)*/
/************************************************************************/
class CMItemCoin:public CMItemBasic
{
protected:
	CCArray* CoinPosArray;
	enum 
	{
		enTagCoin,
	};
public:
	static CMItemCoin *CreateItemIcon(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);
protected:
	virtual bool init(CCPoint ptItemPos,CCSize szItemSize,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario();
	//�����Mario����ײ ������Ϣ���͸�MsgRecver
	virtual void OnCallPerFrame(float fT);
};
/************************************************************************/
/* ����Ģ��                                               
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