#ifndef _CM_MONSTER_HEAD_FILE_
#define _CM_MONSTER_HEAD_FILE_
#include "CMPublicDefine.h"
#include "CMMario.h"
#include "CMGameMap.h"
 
/************************************************************************/
/* �������                                                         
 * 1,Լ���ӿ�
 * 2,�������趨															*/
/************************************************************************/
enum enumMonsterDeadType
{
	enMonsterDeadTypeFall,		//����
	enMonsterDeadTypeStamp,		//��̤
	enMonsterDeadTypeFire,		//����
};
class CMMonsterBasic: public CCNode,public CMSender
{
protected:
	CMMario		*m_pMario;
	//CMGameMap	*m_pGameMap;
protected:
	virtual bool init(CCPoint ptMonsterPos,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario() = 0;//��Mario��ײ���¼�

	virtual void Dead(enumMonsterDeadType DeadType);
public:
	//�����Mario����ײ ������Ϣ���͸�MsgRecver
	virtual void OnCallPerFrame();
};

/************************************************************************/
/* Ģ����                                                                     */
/************************************************************************/
class CMMonsterMushrooms:public CMMonsterBasic
{
public:
	static CMMonsterMushrooms *CreateMonsterMushrooms(CCPoint ptMonsterPos,CMMario *pMario,CMReceiver *pMsgRecver);
protected:
	virtual bool init(CCPoint ptMonsterPos,CMMario *pMario,CMReceiver *pMsgRecver);

	virtual bool OnCollisionMario();

};
/************************************************************************/
/* �ڹ�                                                               */
/************************************************************************/

/************************************************************************/
/* ����                                                                    */
/************************************************************************/

/************************************************************************/
/* ��                                                                    */
/************************************************************************/

#endif