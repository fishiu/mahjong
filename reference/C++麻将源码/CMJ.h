#ifndef _CMJ_H
#define _CMJ_H
#include<vector>
#include "iostream"
using namespace std;
typedef unsigned int        UINT;


#define MJPAI_ZFB               0   //中，发，白 
#define MJPAI_FENG              1   //东西南北风 
#define MJPAI_WAN               2   //万 
#define MJPAI_TIAO              3   //条 
#define MJPAI_BING              4   //饼 
#define MJPAI_HUA               5   //花 

#define MJPAI_GETPAI            true    //起牌 
#define MJPAI_PUTPAI            false   //打牌 
//节点信息 
struct stPAI
{
	int     m_Type;             //牌类型 
	int     m_Value;            //牌字 

}
;

//吃牌顺 
struct stCHI
{
	int     m_Type;             //牌类型 
	int     m_Value1;           //牌字 
	int     m_Value2;           //牌字 
	int     m_Value3;           //牌字 
}
;
//  m_Type      m_Value 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-// 
//  0       |   中   1   发2  白                                              
//          | 
//  1       |   东 1 西2  南     北                                   
//          | 
//  2       |   一万  二万  ……  九万 
//          | 
//  3       |   一条  二条  ……  九条                   
//          | 
//  4       |   一饼  二饼  ……  九饼 
//          | 
//  5       |   春       夏       秋       东       竹       兰       梅       菊 
//          | 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-// 



//胡牌信息 
struct stGoodInfo
{
	char    m_GoodName[100];            //胡牌术语 
	int     m_GoodValue;                //胡牌番数 
}
;
//牌 
class CMJ
{
	vector<  int >        m_MyPAIVec[6];      //起的种牌型 
	vector<  int >        m_ChiPAIVec[6];     //吃的种牌型 
	vector<  int >        m_PengPAIVec[6];    //碰的种牌型 
	vector<  int >        m_GangPAIVec[6];    //杠的种牌型 

	stPAI               m_LastPAI;          //最后起的牌 
	stGoodInfo          m_GoodInfo;         //胡牌信息 

	bool                m_9LBD;             //是否听连宝灯牌型 
	bool                m_13Y;              //是否听十三幺 
	int                 m_MKNum;            //明刻数 
	int                 m_AKNum;            //暗刻数 
	bool                m_4AK;              //是否是听四暗刻 

	vector<  stCHI >      m_TempChiPAIVec;    //吃的可选组合 
	vector<  stPAI >      m_TempPengPAIVec;   //碰的可选组合 
	vector<  stPAI >      m_TempGangPAIVec;   //杠的可选组合 

public:

	//构造 
	CMJ();
	//析构 
	~CMJ();
	//初始化 
	void            Init();
	//起牌 
	bool            AddPai(int p_Type, int p_Value);
	//取得对应的牌在牌墙的索引 
	int             GetPaiIndex(int p_Type, int p_Value);
	//打牌(参数为对应的牌在牌墙的索引) 
	bool            DelPai(int PaiIndex);
	//删除牌 
	bool            DelPai(int p_Type, int p_Value);
	//清空牌 
	void            CleanUp();
	//取得胡牌信息 
	stGoodInfo      *GetInfo();
	//检测是否胡牌 
	bool            CheckAllPai(bool GetOrPut);
	//对所有的牌进行输出 
	void            PrintAllPai();
	//对一张牌进行输出 
	void            PrintPai(int p_Type, int p_Value);
	//吃牌 
	bool            CheckChiPai(int p_Type, int p_Value);
	//吃牌 
	bool            DoChiPai(int p_iIndex, int p_Type, int p_Value);
	//碰牌 
	bool            CheckPengPai(int p_Type, int p_Value);
	//碰牌 
	bool            DoPengPai(int p_Type, int p_Value);
	//杠牌 
	bool            CheckGangPai(int p_Type, int p_Value);
	//杠牌 
	bool            DoGangPai(int p_Type, int p_Value);
	//对可吃的组合进行输出 
	void            PrintChiChosePai();
	//对可碰的组合进行输出 
	void            PrintPengChosePai();
	//对可杠的组合进行输出 
	void            PrintGangChosePai();
	//取得吃牌组合数 
	UINT            GetChiChoseNum();

private:

	//检测是否胡牌（张） 
	bool    CheckAAPai(int iValue1, int iValue2);
	//检测是否三连张 
	bool    CheckABCPai(int iValue1, int iValue2, int iValu3);
	//检测是否三重张 
	bool    CheckAAAPai(int iValue1, int iValue2, int iValu3);
	//检测是否四重张 
	bool    CheckAAAAPai(int iValue1, int iValue2, int iValu3, int iValue4);
	//检测是否三连对 
	bool    CheckAABBCCPai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6);
	//检测是否三连高压 
	bool    CheckAAABBBCCCPai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9);
	//检测是否三连刻 
	bool    CheckAAAABBBBCCCCPai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11, int iValue12);
	//检测是否六连对 
	bool    CheckAABBCCDDEEFFPai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11, int iValue12);
	//带将牌检测=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 

	//检测是否胡牌（张） 
	bool    Check5Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5);
	//检测是否胡牌（张） 
	bool    Check8Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8);
	//检测是否胡牌（张） 
	bool    Check11Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11);
	//检测是否胡牌（张） 
	bool    Check14Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11, int iValue12, int iValue13, int iValue14);

	//不带将牌检测-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 

	//检测是否胡牌（张） 
	bool    Check3Pai(int iValue1, int iValue2, int iValue3);
	//检测是否胡牌（张） 
	bool    Check6Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6);
	//检测是否胡牌（张） 
	bool    Check9Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9);
	//检测是否胡牌（张） 
	bool    Check12Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11, int iValue12);


private:
	//胡牌判断 

	//检测是否胡大四喜 
	bool    CheckD4X_HU();
	//检则是否胡大三元 
	bool    CheckD3Y_HU();
	//检测是否胡绿一色 
	bool    CheckL1S_HU();
	//检测是否胡九莲宝灯 
	bool    Check9LBD_HU();
	//检测是否胡四杠 
	bool    Check4Gang_HU();
	//检测是否胡连七对 
	bool    CheckL7D_HU();
	//检测是否胡十三幺 
	bool    Chekc13Y_HU();
	//检测是否胡清幺九 
	bool    CheckQY9_HU();
	//检测是否胡小四喜 
	bool    CheckX4X_HU();
	//检测是否胡小三元 
	bool    CheckX3Y_HU();
	//检测是否胡字一色 
	bool    CheckZ1S_HU();
	//检测是否四暗刻 
	bool    Check4AK_HU();
	//检测是否一色双龙会 
	bool    Check1S2LH_HU();
	//检测是否一色四同顺 
	bool    Check1S4TS_HU();
	//检测是否一色四节高？ 
	bool    Check1S4JG_HU();
	//检测是否一色四步高？ 
	bool    Check1S4BG_HU();
	//检测是否三杠 
	bool    Check3Gang_HU();
	//检测是否混幺九 
	bool    CheckHY9_HU();
	//检测是否七对 
	bool    Check7D_HU();
	//检测是否七星不靠 
	bool    Check7XBK_HU();
	//检测是否全双刻？ 
	bool    CheckQSK_HU();
	//清一色 
	bool    CheckQ1S_HU();
	//检测是否一色三同顺 
	bool    Check1S3TS_HU();
	//检测是否一色三节高 
	bool    Check1S3JG_HU();
	//检测是否全大 
	bool    CheckQD_HU();
	//检测是否全中 
	bool    CheckQZ_HU();
	//检测是否全小 
	bool    CheckQX_HU();
	//检测是否青龙 
	bool    CheckQL_HU();
	//检测是否三色双龙会 
	bool    Check3S2LH_HU();
	//检测是否一色三步高 
	bool    Check1S3BG_HU();
	//全带五 
	bool    CheckQD5_HU();
	//三同刻 
	bool    Check3TK_HU();
	//三暗刻 
	bool    Check3AK_HU();
	//单钓将 
	bool    CheckDDJ_HU();
	//检测胡 
	bool    CheckHU();
private:
	//听牌判断 

	//检测是否听九莲宝灯 
	bool    Check9LBD_TING();
	//检测是否听十三幺 
	bool    Check13Y_TING();
	//检测是否听四暗刻 
	bool    Check4AK_TING();
	//检测是否听牌 
	bool    CheckTING();

}
;

#endif 