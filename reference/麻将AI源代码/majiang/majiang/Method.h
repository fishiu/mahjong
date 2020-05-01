#pragma once
#include "GameData.h"
#include "Mp3Sound.h"
//游戏算法类
class CMethod
{
public:

	CMethod(void);
	void  Shuffle(int *P);    //洗牌
	bool  Move(vector<CARDS> &,CPoint p); //提牌
	bool  buttonDown(vector<CARDS> &,int&);//鼠标单击
	bool  hupai(vector<temp1> &);//胡牌算法
	void  sendCard(vector<CARDS> &,PLAYER &);//出牌
	int  ChaiPai(vector<CARDS> &);//拆牌算法（电脑自动出牌算法）
	void deleteCards(vector<CARDS> &,PLAYER & send );
	bool pengPai(vector<CARDS> &,PLAYER & send);//碰牌
	bool gangPai(vector<CARDS> &,PLAYER & send);//杠牌
	bool anGangPai(vector<CARDS> &d,int index);//暗杠牌
	
public:
	bool isChuPai;
	bool duizi;
	vector<temp1> tempvec;
	 Mp3Sound  m_mp3;
	~CMethod(void);
};
