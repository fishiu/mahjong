#ifndef _CMJMANAGE_H  
#define _CMJMANAGE_H  
#include<vector>
#include "iostream"
#include <algorithm>    // std::random_shuffle
#include "CMJ.h"  
//剩余牌墙信息  
//扩展  
struct stPAIEx
{
	stPAI   m_NewPai;                       //起的新牌  
	int     m_PaiNum;                       //剩余牌数  
	bool    m_IsHZ;                         //是否黄庄  
}
;

//麻将管理器  
class CMJManage
{
	vector<stPAI> m_MJVec;                //麻将数据VEC  
	int             m_HZPaiNum;             //黄庄的牌数  
public:

	//构造函数  
	CMJManage();
	//析构函数  
	~CMJManage();
	//初始化牌  
	void    InitPai(int p_HZPaiNum = 0);
	//起牌  
	stPAIEx GetAPai();
private:
	//洗牌  
	void    XiPai();
}
;

#endif 
