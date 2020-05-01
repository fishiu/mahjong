#pragma once
//游戏类
#include "Method.h"
#include "Cards.h"
class CGame
{
public:
	CGame(void);
	void initGame();//初始化游戏
	void startGame();//开始游戏
	void updataGame(CDC * pdc);//更新游戏
	void endGame();//结束游戏
public:
	CMethod   m_member;
	CCards   m_gameCards;
	~CGame(void);
};
