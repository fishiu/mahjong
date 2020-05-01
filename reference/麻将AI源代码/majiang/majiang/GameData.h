#pragma once
#ifndef  _GAME_H_
#define _GAME_H_


#include <vector>
using namespace std;
#define   PLAYER_A         0        //自己的牌
#define   PLAYER_B         1        //右家的牌
#define   PLAYER_C         2        //对家的牌
#define   PLAYER_D         3        //左家的牌
#define   PLAYER_Z         4        //台面上还没摸得牌

#define  GAME_START       5
#define  GAME_END         6
#define  GAME_PAUSE        7
enum QUANZHI
{
	PAI_NULL,      //还没拆牌之前
	PAI_DANGPAI,   //单张
	PAI_DANPAI_1,
	PAI_DAPAI_1, //（2  3可以有2张牌成顺子）
	PAI_DAPAI_2, //（1  3只有1张牌能成顺子）
	PAI_DUIZI,    //对子
	PAI_SHUNZI,   //顺子
	PAI_SANTIAO    //3条
};
struct CARDS                          //牌的结构体 
{
	CARDS(){isclick =false;duizi=true;isOut=false;value1 =PAI_NULL;}
	int x;                            //牌在屏幕上的X坐标
	int y;                            //牌在屏幕上的Y坐标
	int cx;                           //排在图片上的X坐标
	int cy;                           //排在图片上的Y坐标
	int index;                        //牌值得大小
	int huase;                        //牌的花色
	int whoCard;                       // 谁的牌
	int cardTp;                       //牌的类型 （1 单牌 2 碰牌   3 杠牌  4 胡牌）
	bool isclick;                       //判断牌是否被点击起
	int wide;                       //图片的宽
	int high;                       //图片的高
	bool duizi;                     //判断是否还可以成为对子
	CRect  s_rcet;                     //rect 对象，用于判断玩家单击
	QUANZHI value1;
	bool  isOut;
	bool operator <   (const CARDS& rhs )   const //升序排序时必须写的函数
	{ 
		return index < rhs.index; 
	}

};

struct  PLAYER
{
	PLAYER()
	{
		whoChupai =PLAYER_A;
		cardTp =1;
	}
	int  whoChupai;     //是否到玩家出牌
	int  index;//当前牌的牌值 
	int  cardTp; //牌的样式          1  单牌   2 碰牌  3 明杠牌  4 袍牌
};



struct temp1 
{
	temp1()
	{
		duizi=true;
	}
	int index;
	bool duizi;

	bool operator <   (const temp1& rhs )   const //升序排序时必须写的函数
	{ 
		return index < rhs.index; 
	}
};


#endif    