#pragma once

#include "Method.h"
#include "GameData.h"
#include "Mp3Sound.h"


class CCards
{
public:
	void initCards();//初始话108张牌
	void drawCards(CDC * dc);//绘制游戏中的牌
	void mouseMove(CPoint &p);
	void lbuttonDown(CPoint &);//单击鼠标左键
	void computerSendCards();//电脑出牌
	void getPai();//摸牌
	bool isPao(vector<CARDS>&,int);//放炮
	bool fangPao(int);
	void SetMessage(bool IS){m_isMessage =IS;}//设置手动发送消息
	bool GetMessage(){return m_isMessage;}//判断是否手动发送消息
	int GetWhoSendCard(){return m_sendCard.whoChupai;}//获得当前该谁出牌
	bool  IsHuPai(vector<CARDS>& player);
	bool  whoPengPai();//判断谁要碰牌
	bool  whoGangPai();//判断谁要杠牌
	bool whoAnGang(int index);
	bool getIsPeng(){return m_computerGang;}
	void setPeng(bool is){ m_isPaintPeng =is;}

	bool getIsHu(){return m_isHuPai;}
	void setHu(bool is){ m_isHuPai =is;}

	int getGameState(){return m_gameState;} //获取游戏状态
	void setGameState(int game){m_gameState =game;}
	
	CCards(void);
public:
	//int   m_sendCardIndex;//记录当前出牌的牌值
	HBITMAP m_bitmap[30];
	CDC m_dcBuffer;//双缓冲背景DC
	CDC m_back;//背景
	CDC m_dcImage;//牌DC
	CDC m_2pass;
	CDC m_dcMask;//掩码图DC
	int intarray_arrow[8]; //箭头坐标数组
	int m_gameState;//游戏状态
    bool m_isMessage;  //是否手动发送消息
	bool m_isPaintPeng; //绘制碰得图片
	bool m_isHuPai;     //绘制胡牌的图片
	bool m_computerGang;//电脑杠牌
	bool m_meGang;//判断自己是否杠牌
	PLAYER  m_sendCard; //出牌

	Mp3Sound m_mp3;
 
	CMethod  m_metCards;
	vector<CARDS>  m_vecCards;
   vector<CARDS> m_vcsendCard;
	vector<CARDS> m_playA;
	vector<CARDS> m_playB;
	vector<CARDS> m_playC;
	vector<CARDS> m_playD;
	~CCards(void);
};
