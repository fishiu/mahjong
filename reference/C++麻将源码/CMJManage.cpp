#include "CMJManage.h"  
#include <time.h>
#include <windows.h>

using namespace std;

//构造函数  
CMJManage::CMJManage()
{
	m_HZPaiNum = 0;
}
//析构函数  
CMJManage::~CMJManage()
{

}

//初始化牌  
void    CMJManage::InitPai(int p_HZPaiNum)
{
	m_HZPaiNum = p_HZPaiNum;
	m_MJVec.clear();
	//中发白  
	for (UINT i = 1; i <= 3; i++)
	{
		stPAI t_Pai;
		t_Pai.m_Type = 0;
		t_Pai.m_Value = i;
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
	}
	//东南西北  
	for (UINT i = 1; i <= 4; i++)
	{
		stPAI t_Pai;
		t_Pai.m_Type = 1;
		t_Pai.m_Value = i;
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
	}
	//万  
	for (UINT i = 1; i <= 9; i++)
	{
		stPAI t_Pai;
		t_Pai.m_Type = 2;
		t_Pai.m_Value = i;
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
	}
	//条  
	for (UINT i = 1; i <= 9; i++)
	{
		stPAI t_Pai;
		t_Pai.m_Type = 3;
		t_Pai.m_Value = i;
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
	}
	//饼  
	for (UINT i = 1; i <= 9; i++)
	{
		stPAI t_Pai;
		t_Pai.m_Type = 4;
		t_Pai.m_Value = i;
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
		m_MJVec.push_back(t_Pai);
	}
	XiPai();
}

//洗牌  
void    CMJManage::XiPai()
{

	srand(GetTickCount());
	random_shuffle(m_MJVec.begin(), m_MJVec.end());
}

//起牌  
stPAIEx CMJManage::GetAPai()
{
	//如果所有牌都起完了  

	stPAIEx t_Pai;
	t_Pai.m_NewPai.m_Type = m_MJVec.back().m_Type;
	t_Pai.m_NewPai.m_Value = m_MJVec.back().m_Value;
	t_Pai.m_PaiNum = m_MJVec.size() - 1;
	if (t_Pai.m_PaiNum == m_HZPaiNum)
	{
		t_Pai.m_IsHZ = true;
	}
	else
	{
		t_Pai.m_IsHZ = false;
	}
	//扔去一个  
	m_MJVec.pop_back();
	return t_Pai;
}


