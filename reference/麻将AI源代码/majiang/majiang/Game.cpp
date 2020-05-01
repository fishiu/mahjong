#include "StdAfx.h"
#include "Game.h"

CGame::CGame(void)
{


}
void CGame::initGame()//初始化游戏
{

 m_gameCards.initCards();

 


}
void CGame::startGame()//开始游戏
{



}
void CGame::updataGame(CDC * pdc)//更新游戏
{
  m_gameCards.drawCards(pdc);
}
void CGame::endGame()//结束游戏
{



}
CGame::~CGame(void)
{
}
