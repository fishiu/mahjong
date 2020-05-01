#include "CMJ.h" 
//构造 
CMJ::CMJ()
{
	m_9LBD = false;
	m_13Y = false;
	m_4AK = false;
	m_AKNum = 0;
	m_MKNum = 0;
}

//析构 
CMJ::~CMJ()
{

}
//初始化 
void CMJ::Init()
{
	m_9LBD = false;
	m_13Y = false;
	m_4AK = false;
	m_AKNum = 0;
	m_MKNum = 0;
}
//加入新牌,并排序 
bool  CMJ::AddPai(int p_Type, int p_Value)
{
	int iSize = m_MyPAIVec[p_Type].size();
	bool t_Find = false;
	vector<  int >::iterator Iter;
	for (Iter = m_MyPAIVec[p_Type].begin(); Iter != m_MyPAIVec[p_Type].end(); Iter++)
	{
		if ((*Iter)>p_Value)
		{
			m_MyPAIVec[p_Type].insert(Iter, p_Value);
			t_Find = true;
			break;
		}

	}

	if (t_Find == false)
	{
		m_MyPAIVec[p_Type].push_back(p_Value);
	}
	m_LastPAI.m_Type = p_Type;
	m_LastPAI.m_Value = p_Value;

	return true;
}

//取得对应的牌在牌墙的索引 
int CMJ::GetPaiIndex(int p_Type, int p_Value)
{
	int count = 0;
	for (UINT i = 0; i < 6; i++)
	{
		vector<  int >::iterator Iter;
		for (Iter = m_MyPAIVec[i].begin(); Iter != m_MyPAIVec[i].end(); Iter++)
		{
			if (p_Type == i && (*Iter) == p_Value)
			{
				return count;
			}
			count++;
		}
	}
	return -1;
}
//打牌 
bool    CMJ::DelPai(int PaiIndex)
{
	int count = 0;
	for (UINT i = 0; i < 6; i++)
	{
		vector<  int >::iterator Iter;
		for (Iter = m_MyPAIVec[i].begin(); Iter != m_MyPAIVec[i].end(); Iter++)
		{
			if (count == PaiIndex)
			{
				m_MyPAIVec[i].erase(Iter);
				return true;
			}
			count++;
		}
	}
	return false;
}
//删除牌 
bool    CMJ::DelPai(int p_Type, int p_Value)
{
	vector<  int >::iterator Iter;
	for (Iter = m_MyPAIVec[p_Type].begin(); Iter != m_MyPAIVec[p_Type].end(); Iter++)
	{
		if ((*Iter) == p_Value)
		{
			m_MyPAIVec[p_Type].erase(Iter);
			return true;
		}
	}
	return false;
}
//清空牌 
void    CMJ::CleanUp()
{
	for (UINT i = 0; i < 6; i++)
	{
		m_MyPAIVec[i].clear();
		m_ChiPAIVec[i].clear();
		m_PengPAIVec[i].clear();
		m_GangPAIVec[i].clear();
	}
}
//取得胡牌信息 
stGoodInfo      *CMJ::GetInfo()
{
	return &m_GoodInfo;
}

//对所有的牌进行函数调用 
void    CMJ::PrintAllPai()
{
	cout << " ";
	for (UINT i = 0; i < 13; i++)
	{
		cout << i << "  - ";
	}
	cout << endl;
	int icount = 0;
	//箭牌 
	if (m_MyPAIVec[0].empty() == false)
	{
		vector<  int >::iterator Iter;
		for (Iter = m_MyPAIVec[0].begin(); Iter != m_MyPAIVec[0].end(); Iter++)
		{
			switch (*Iter)
			{
			case 1:
				cout << "[ 中]";
				break;
			case 2:
				cout << "[ 发]";
				break;
			case 3:
				cout << "[ 白]";
				break;

			}
			icount++;
		}

	}
	cout << endl;
	for (UINT i = 0; i < icount; i++)
	{
		cout << "     ";
	}
	//风牌 
	if (m_MyPAIVec[1].empty() == false)
	{
		vector<  int >::iterator Iter;
		for (Iter = m_MyPAIVec[1].begin(); Iter != m_MyPAIVec[1].end(); Iter++)
		{
			switch (*Iter)
			{
			case 1:
				cout << "[ 东]";
				break;
			case 2:
				cout << "[ 南]";
				break;
			case 3:
				cout << "[ 西]";
				break;
			case 4:
				cout << "[ 北]";
				break;
			}
			icount++;
		}
	}
	cout << endl;
	for (UINT i = 0; i < icount; i++)
	{
		cout << "     ";
	}
	//万 
	if (m_MyPAIVec[2].empty() == false)
	{
		vector<  int >::iterator Iter;
		for (Iter = m_MyPAIVec[2].begin(); Iter != m_MyPAIVec[2].end(); Iter++)
		{
			cout << "[" << (*Iter) << "万]";
			icount++;
		}
	}
	cout << endl;
	for (UINT i = 0; i < icount; i++)
	{
		cout << "     ";
	}
	//条 
	if (m_MyPAIVec[3].empty() == false)
	{
		vector<  int >::iterator Iter;
		for (Iter = m_MyPAIVec[3].begin(); Iter != m_MyPAIVec[3].end(); Iter++)
		{
			cout << "[" << (*Iter) << "条]";
			icount++;
		}
	}
	cout << endl;
	for (UINT i = 0; i < icount; i++)
	{
		cout << "     ";
	}
	//饼 
	if (m_MyPAIVec[4].empty() == false)
	{
		vector<  int >::iterator Iter;
		for (Iter = m_MyPAIVec[4].begin(); Iter != m_MyPAIVec[4].end(); Iter++)
		{
			cout << "[" << (*Iter) << "饼]";
			icount++;
		}
	}
	cout << endl;
	for (UINT i = 0; i < icount; i++)
	{
		cout << "     ";
	}
}
//对一张牌进行输出 
void CMJ::PrintPai(int p_Type, int p_Value)
{
	//箭牌 
	if (p_Type == 0)
	{
		switch (p_Value)
		{
		case 1:
			cout << "[中]";
			break;
		case 2:
			cout << "[发]";
			break;
		case 3:
			cout << "[白]";
			break;
		}
	}
	//风牌 
	if (p_Type == 1)
	{
		switch (p_Value)
		{
		case 1:
			cout << "[东]";
			break;
		case 2:
			cout << "[南]";
			break;
		case 3:
			cout << "[西]";
			break;
		case 4:
			cout << "[北]";
			break;
		}
	}
	//万 
	if (p_Type == 2)
	{
		cout << "[" << p_Value << "万]";
	}
	//条 
	if (p_Type == 3)
	{
		cout << "[" << p_Value << "条]";
	}
	//饼 
	if (p_Type == 4)
	{
		cout << "[" << p_Value << "饼]";
	}
}

//吃牌 
bool    CMJ::CheckChiPai(int p_Type, int p_Value)
{
	m_TempChiPAIVec.clear();
	//饼 
	if (m_MyPAIVec[p_Type].empty() == false)
	{
		int iSize = m_MyPAIVec[p_Type].size();
		if (iSize >= 2)
		{
			for (UINT i = 0; i < iSize - 1; i++)
			{
				if ((m_MyPAIVec[p_Type][i] == (p_Value - 2)) && (m_MyPAIVec[p_Type][i + 1] == (p_Value - 1)))
				{
					stCHI t_Chi;
					t_Chi.m_Type = p_Type;
					t_Chi.m_Value1 = p_Value - 2;
					t_Chi.m_Value2 = p_Value - 1;
					t_Chi.m_Value3 = p_Value;
					m_TempChiPAIVec.push_back(t_Chi);

				}
				if ((m_MyPAIVec[p_Type][i] == (p_Value - 1)) && (m_MyPAIVec[p_Type][i + 1] == (p_Value + 1)))
				{
					stCHI t_Chi;
					t_Chi.m_Type = p_Type;
					t_Chi.m_Value1 = p_Value - 1;
					t_Chi.m_Value2 = p_Value;
					t_Chi.m_Value3 = p_Value + 1;
					m_TempChiPAIVec.push_back(t_Chi);

				}
				if ((m_MyPAIVec[p_Type][i] == (p_Value + 1)) && (m_MyPAIVec[p_Type][i + 1] == (p_Value + 2)))
				{
					stCHI t_Chi;
					t_Chi.m_Type = p_Type;
					t_Chi.m_Value1 = p_Value;
					t_Chi.m_Value2 = p_Value + 1;
					t_Chi.m_Value3 = p_Value + 2;
					m_TempChiPAIVec.push_back(t_Chi);
				}
			}

		}
		//假设吃B，已有ABC 
		if (iSize >= 3)
		{
			for (UINT i = 1; i < iSize - 1; i++)
			{
				if ((m_MyPAIVec[p_Type][i - 1] == (p_Value - 1)) && (m_MyPAIVec[p_Type][i] == p_Value) && (m_MyPAIVec[p_Type][i + 1] == (p_Value + 1)))
				{
					stCHI t_Chi;
					t_Chi.m_Type = p_Type;
					t_Chi.m_Value1 = p_Value - 1;
					t_Chi.m_Value2 = p_Value;
					t_Chi.m_Value3 = p_Value + 1;
					m_TempChiPAIVec.push_back(t_Chi);
				}
			}
		}
		//假设吃B，已有ABBC 
		if (iSize >= 4)
		{
			for (UINT i = 1; i < iSize - 2; i++)
			{
				if ((m_MyPAIVec[p_Type][i - 1] == (p_Value - 1)) && (m_MyPAIVec[p_Type][i] == p_Value) && (m_MyPAIVec[p_Type][i + 2] == (p_Value + 1)))
				{
					stCHI t_Chi;
					t_Chi.m_Type = p_Type;
					t_Chi.m_Value1 = p_Value - 1;
					t_Chi.m_Value2 = p_Value;
					t_Chi.m_Value3 = p_Value + 1;
					m_TempChiPAIVec.push_back(t_Chi);
				}
			}
		}
		//假设吃B，已有ABBBC 
		if (iSize >= 5)
		{
			for (UINT i = 1; i < iSize - 3; i++)
			{
				if ((m_MyPAIVec[p_Type][i - 1] == (p_Value - 1)) && (m_MyPAIVec[p_Type][i] == p_Value) && (m_MyPAIVec[p_Type][i + 3] == (p_Value + 1)))
				{
					stCHI t_Chi;
					t_Chi.m_Type = p_Type;
					t_Chi.m_Value1 = p_Value - 1;
					t_Chi.m_Value2 = p_Value;
					t_Chi.m_Value3 = p_Value + 1;
					m_TempChiPAIVec.push_back(t_Chi);
				}
			}
		}
		//假设吃B，已有ABBBBC 
		if (iSize >= 6)
		{
			for (UINT i = 1; i < iSize - 4; i++)
			{
				if ((m_MyPAIVec[p_Type][i - 1] == (p_Value - 1)) && (m_MyPAIVec[p_Type][i] == p_Value) && (m_MyPAIVec[p_Type][i + 4] == (p_Value + 1)))
				{
					stCHI t_Chi;
					t_Chi.m_Type = p_Type;
					t_Chi.m_Value1 = p_Value - 1;
					t_Chi.m_Value2 = p_Value;
					t_Chi.m_Value3 = p_Value + 1;
					m_TempChiPAIVec.push_back(t_Chi);
				}
			}
		}
		if (m_TempChiPAIVec.size() > 0)
		{
			return  true;
		}
	}
	return false;
}
//吃牌 
bool CMJ::DoChiPai(int p_iIndex, int p_Type, int p_Value)
{
	AddPai(p_Type, p_Value);
	vector< stCHI>::iterator Iter;
	int icount = 0;
	for (Iter = m_TempChiPAIVec.begin(); Iter != m_TempChiPAIVec.end(); Iter++)
	{
		if (icount == p_iIndex)
		{
			DelPai((*Iter).m_Type, (*Iter).m_Value1);
			DelPai((*Iter).m_Type, (*Iter).m_Value2);
			DelPai((*Iter).m_Type, (*Iter).m_Value3);

			m_ChiPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value1);
			m_ChiPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value2);
			m_ChiPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value3);

			return true;
		}
		icount++;
	}
	return false;
}
//对可吃的组合进行输出 
void CMJ::PrintChiChosePai()
{
	cout << "================吃牌组合=======================" << endl;
	vector< stCHI>::iterator Iter;
	for (Iter = m_TempChiPAIVec.begin(); Iter != m_TempChiPAIVec.end(); Iter++)
	{
		//万 
		if ((*Iter).m_Type == 2)
		{
			cout << "[" << (*Iter).m_Value1 << "万";
			cout << "" << (*Iter).m_Value2 << "万";
			cout << "" << (*Iter).m_Value3 << "万]";
		}
		//条 
		if ((*Iter).m_Type == 3)
		{
			cout << "[" << (*Iter).m_Value1 << "条";
			cout << "" << (*Iter).m_Value2 << "条";
			cout << "" << (*Iter).m_Value3 << "条]";
		}
		//饼 
		if ((*Iter).m_Type == 4)
		{
			cout << "[" << (*Iter).m_Value1 << "饼";
			cout << "" << (*Iter).m_Value2 << "饼";
			cout << "" << (*Iter).m_Value3 << "饼]";
		}
	}
	cout << endl << "=========================================" << endl;
}
//对可碰的组合进行输出 
void CMJ::PrintPengChosePai()
{
	cout << "=====================碰牌==================" << endl;
	vector< stPAI>::iterator Iter;
	for (Iter = m_TempPengPAIVec.begin(); Iter != m_TempPengPAIVec.end(); Iter++)
	{
		//万 
		if ((*Iter).m_Type == 2)
		{
			cout << "[" << (*Iter).m_Value << "万";
			cout << "" << (*Iter).m_Value << "万";
			cout << "" << (*Iter).m_Value << "万]";
		}
		//条 
		if ((*Iter).m_Type == 3)
		{
			cout << "[" << (*Iter).m_Value << "条";
			cout << "" << (*Iter).m_Value << "条";
			cout << "" << (*Iter).m_Value << "条]";
		}
		//饼 
		if ((*Iter).m_Type == 4)
		{
			cout << "[" << (*Iter).m_Value << "饼";
			cout << "" << (*Iter).m_Value << "饼";
			cout << "" << (*Iter).m_Value << "饼]";
		}
	}
	cout << endl << "=========================================" << endl;
}
//对可杠的组合进行输出 
void CMJ::PrintGangChosePai()
{
	cout << "====================杠牌===================" << endl;
	vector< stPAI>::iterator Iter;
	for (Iter = m_TempGangPAIVec.begin(); Iter != m_TempGangPAIVec.end(); Iter++)
	{
		//万 
		if ((*Iter).m_Type == 2)
		{
			cout << "[" << (*Iter).m_Value << "万";
			cout << "" << (*Iter).m_Value << "万";
			cout << "" << (*Iter).m_Value << "万";
			cout << "" << (*Iter).m_Value << "万]";
		}
		//条 
		if ((*Iter).m_Type == 3)
		{
			cout << "[" << (*Iter).m_Value << "条";
			cout << "" << (*Iter).m_Value << "条";
			cout << "" << (*Iter).m_Value << "条";
			cout << "" << (*Iter).m_Value << "条]";
		}
		//饼 
		if ((*Iter).m_Type == 4)
		{
			cout << "[" << (*Iter).m_Value << "饼";
			cout << "" << (*Iter).m_Value << "饼";
			cout << "" << (*Iter).m_Value << "饼";
			cout << "" << (*Iter).m_Value << "饼]";
		}
	}
	cout << endl << "=========================================" << endl;
}
//取得吃牌组合数 
UINT CMJ::GetChiChoseNum()
{
	return m_TempChiPAIVec.size();
}
//碰牌 
bool    CMJ::CheckPengPai(int p_Type, int p_Value)
{
	m_TempPengPAIVec.clear();
	//饼 
	if (m_MyPAIVec[p_Type].empty() == false)
	{
		int iSize = m_MyPAIVec[p_Type].size();
		if (iSize >= 2)
		{
			for (UINT i = 0; i < iSize - 1; i++)
			{
				if ((m_MyPAIVec[p_Type][i] == p_Value) && (m_MyPAIVec[p_Type][i + 1] == p_Value))
				{
					stPAI t_Peng;
					t_Peng.m_Type = p_Type;
					t_Peng.m_Value = p_Value;
					m_TempPengPAIVec.push_back(t_Peng);
					break;
				}
			}
		}
		if (m_TempPengPAIVec.size() > 0)
		{
			return true;
		}
	}
	return false;
}
//碰牌 
bool    CMJ::DoPengPai(int p_Type, int p_Value)
{
	AddPai(p_Type, p_Value);
	vector< stPAI>::iterator Iter;
	for (Iter = m_TempPengPAIVec.begin(); Iter != m_TempPengPAIVec.end(); Iter++)
	{
		DelPai((*Iter).m_Type, (*Iter).m_Value);
		DelPai((*Iter).m_Type, (*Iter).m_Value);
		DelPai((*Iter).m_Type, (*Iter).m_Value);

		m_PengPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value);
		m_PengPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value);
		m_PengPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value);
		return true;
	}
	return false;
}
//杠牌 
bool    CMJ::CheckGangPai(int p_Type, int p_Value)
{
	m_TempGangPAIVec.clear();
	//饼 
	if (m_MyPAIVec[p_Type].empty() == false)
	{
		int iSize = m_MyPAIVec[p_Type].size();
		if (iSize >= 3)
		{
			for (UINT i = 0; i < iSize - 2; i++)
			{
				if ((m_MyPAIVec[p_Type][i] == p_Value) && (m_MyPAIVec[p_Type][i + 1] == p_Value) && (m_MyPAIVec[p_Type][i + 2] == p_Value))
				{
					stPAI t_Gang;
					t_Gang.m_Type = p_Type;
					t_Gang.m_Value = p_Value;
					m_TempGangPAIVec.push_back(t_Gang);
					break;
				}
			}
		}
		if (m_TempGangPAIVec.size() > 0)
		{
			return true;
		}
	}
	return false;
}
//杠牌 
bool    CMJ::DoGangPai(int p_Type, int p_Value)
{
	AddPai(p_Type, p_Value);
	vector< stPAI>::iterator Iter;
	for (Iter = m_TempGangPAIVec.begin(); Iter != m_TempGangPAIVec.end(); Iter++)
	{
		DelPai((*Iter).m_Type, (*Iter).m_Value);
		DelPai((*Iter).m_Type, (*Iter).m_Value);
		DelPai((*Iter).m_Type, (*Iter).m_Value);
		DelPai((*Iter).m_Type, (*Iter).m_Value);

		//排序放入 
		if (m_GangPAIVec[(*Iter).m_Type].empty())
		{
			m_GangPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value);
			m_GangPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value);
			m_GangPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value);
			m_GangPAIVec[(*Iter).m_Type].push_back((*Iter).m_Value);
		}
		else
		{
			vector< int>::iterator Iter2;
			for (Iter2 = m_GangPAIVec[(*Iter).m_Type].begin(); Iter2 != m_GangPAIVec[(*Iter).m_Type].end(); Iter2++)
			{
				if ((*Iter2)>(*Iter).m_Value)
				{
					m_GangPAIVec[(*Iter).m_Type].insert(Iter2, (*Iter).m_Value);
					m_GangPAIVec[(*Iter).m_Type].insert(Iter2, (*Iter).m_Value);
					m_GangPAIVec[(*Iter).m_Type].insert(Iter2, (*Iter).m_Value);
					m_GangPAIVec[(*Iter).m_Type].insert(Iter2, (*Iter).m_Value);
					break;
				}
			}
		}
		return true;
	}
	return false;
}
//检测是否胡牌 
bool    CMJ::CheckAllPai(bool GetOrPut)
{
	if (GetOrPut == MJPAI_GETPAI)
	{
		//检查大四喜 
		if (CheckD4X_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "大四喜");
			m_GoodInfo.m_GoodValue = 88;
			return true;
		}
		//检查大三元 
		if (CheckD3Y_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "大三元");
			m_GoodInfo.m_GoodValue = 88;
			return true;
		}
		//检查绿一色 
		if (CheckL1S_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "绿一色");
			m_GoodInfo.m_GoodValue = 88;
			return true;
		}
		//检查九莲宝灯 
		if (Check9LBD_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "九莲宝灯");
			m_GoodInfo.m_GoodValue = 88;
			return true;
		}
		//检查四杠 
		if (Check4Gang_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "四杠");
			m_GoodInfo.m_GoodValue = 88;
			return true;
		}
		//检查连七对 
		if (CheckL7D_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "连七对");
			m_GoodInfo.m_GoodValue = 88;
			return true;
		}
		//检查十三幺 
		if (Chekc13Y_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "十三幺");
			m_GoodInfo.m_GoodValue = 88;
			return true;
		}
		//检查清幺九 
		if (CheckQY9_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "清幺九");
			m_GoodInfo.m_GoodValue = 64;
			return true;
		}
		//检查小四喜 
		if (CheckX4X_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "小四喜");
			m_GoodInfo.m_GoodValue = 64;
			return true;
		}
		//检查小三元 
		if (CheckX3Y_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "小三元");
			m_GoodInfo.m_GoodValue = 64;
			return true;
		}
		//检测是否四暗刻 
		if (Check4AK_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "四暗刻");
			m_GoodInfo.m_GoodValue = 64;
			return true;
		}
		//检测是否一色双龙会 
		if (Check1S2LH_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "一色双龙会");
			m_GoodInfo.m_GoodValue = 64;
			return true;
		}
		//检测是否一色四同顺 
		if (Check1S4TS_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "一色四同顺");
			m_GoodInfo.m_GoodValue = 48;
			return true;
		}
		//检测是否一色四节高 
		if (Check1S4JG_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "一色四节高");
			m_GoodInfo.m_GoodValue = 48;
			return true;
		}
		//检测是否一色四步高 
		if (Check1S4BG_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "一色四步高");
			m_GoodInfo.m_GoodValue = 32;
			return true;
		}
		//检测是否三杠 
		if (Check3Gang_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "三杠");
			m_GoodInfo.m_GoodValue = 32;
			return true;
		}

		//检测是否七对 
		if (Check7D_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "七对");
			m_GoodInfo.m_GoodValue = 24;
			return true;
		}
		//检测是否七星不靠 
		if (Check7XBK_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "七星不靠");
			m_GoodInfo.m_GoodValue = 24;
			return true;
		}
		//检测是否全双刻 
		if (CheckQSK_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "全双刻");
			m_GoodInfo.m_GoodValue = 24;
			return true;
		}
		//检测是否清一色 
		if (CheckQ1S_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "清一色");
			m_GoodInfo.m_GoodValue = 24;
			return true;
		}
		//检测是否一色三同顺 
		if (Check1S3TS_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "一色三同顺");
			m_GoodInfo.m_GoodValue = 24;
			return true;
		}
		//检测是否一色三节高 
		if (Check1S3JG_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "一色三节高");
			m_GoodInfo.m_GoodValue = 24;
			return true;
		}
		//检测是否全大 
		if (CheckQD_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "全大");
			m_GoodInfo.m_GoodValue = 24;
			return true;
		}
		//检测是否全中 
		if (CheckQZ_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "全中");
			m_GoodInfo.m_GoodValue = 24;
			return true;
		}
		//检测是否全小 
		if (CheckQX_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "全小");
			m_GoodInfo.m_GoodValue = 24;
			return true;
		}
		//检测是否青龙 
		if (CheckQL_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "青龙");
			m_GoodInfo.m_GoodValue = 16;
			return true;
		}
		//检测是否三色双龙会 
		if (Check3S2LH_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "三色双龙会");
			m_GoodInfo.m_GoodValue = 16;
			return true;
		}
		//检测是否一色三步高 
		if (Check1S3BG_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "一色三步高");
			m_GoodInfo.m_GoodValue = 16;
			return true;
		}

		//检测是否单调将 
		if (CheckDDJ_HU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "单调将");
			m_GoodInfo.m_GoodValue = 1;
			return true;
		}

		//检测是否平胡 
		if (CheckHU())
		{
			strcpy_s(m_GoodInfo.m_GoodName, "平胡");
			m_GoodInfo.m_GoodValue = 1;
			return true;
		}

	}
	else
	{
		//判断是否听连宝灯 
		m_9LBD = Check9LBD_TING();
		if (m_9LBD)return true;
		//判断是否听幺 
		m_13Y = Check13Y_TING();
		if (m_13Y)return true;
		//判断是否四暗刻 
		m_4AK = Check4AK_TING();
		if (m_4AK)return true;
		//检测是否听头 
		return CheckTING();
	}
	return false;
}

//检测是否胡牌（张） 
inline bool CMJ::CheckAAPai(int iValue1, int iValue2)
{
	if (iValue1 == iValue2)return true;
	return false;
}

//检测是否三连张 
inline bool CMJ::CheckABCPai(int iValue1, int iValue2, int iValue3)
{
	if (iValue1 == (iValue2 - 1) && iValue2 == (iValue3 - 1))return true;
	return false;
}

//检测是否三重张 
inline bool CMJ::CheckAAAPai(int iValue1, int iValue2, int iValue3)
{
	if (iValue1 == iValue2&&iValue2 == iValue3)return true;
	return false;
}

//检测是否四重张 
inline bool CMJ::CheckAAAAPai(int iValue1, int iValue2, int iValue3, int iValue4)
{
	if (iValue1 == iValue2&&iValue2 == iValue3&&iValue3 == iValue4)return true;
	return false;
}
//检测是否三连对 
inline bool CMJ::CheckAABBCCPai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6)
{
	if (iValue1 == iValue2&&iValue3 == iValue4&&iValue5 == iValue6)
	{
		if ((iValue1 == iValue3 - 1) && (iValue3 == iValue5 - 1))
		{
			return true;
		}
	}
	return false;
}
//检测是否三连高压 
inline bool CMJ::CheckAAABBBCCCPai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9)
{
	if ((iValue1 == iValue2&&iValue2 == iValue3) && (iValue4 == iValue5&&iValue5 == iValue6) && (iValue7 == iValue8&&iValue8 == iValue9))
	{
		if ((iValue1 == iValue4 - 1) && (iValue4 == iValue7 - 1))
		{
			return true;
		}
	}
	return false;
}
//检测是否三连刻 
inline bool CMJ::CheckAAAABBBBCCCCPai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11, int iValue12)
{
	if ((iValue1 == iValue2&&iValue2 == iValue3&&iValue3 == iValue4) && (iValue5 == iValue6&&iValue6 == iValue7&&iValue7 == iValue8) && (iValue9 == iValue10&&iValue10 == iValue11&&iValue11 == iValue12))
	{
		if ((iValue1 == iValue5 - 1) && (iValue5 == iValue9 - 1))
		{
			return true;
		}
	}
	return false;
}
//检测是否六连对 
inline bool CMJ::CheckAABBCCDDEEFFPai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11, int iValue12)
{
	if (iValue1 == iValue2&&iValue3 == iValue4&&iValue5 == iValue6&&iValue7 == iValue8&&iValue9 == iValue10&&iValue11 == iValue12)
	{
		if ((iValue1 == iValue3 - 1) && (iValue3 == iValue5 - 1) && (iValue5 == iValue7 - 1) && (iValue7 == iValue9 - 1) && (iValue9 == iValue11 - 1))
		{
			return true;
		}
	}
	return false;
}

//检测是否胡牌（张） 
bool    CMJ::Check5Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5)
{
	//如果是左边两个为将，右边为三重张或三连张 
	if (CheckAAPai(iValue1, iValue2))
	{
		if (Check3Pai(iValue3, iValue4, iValue5))return true;

	}
	//如果左中两个为将 
	if (CheckAAPai(iValue2, iValue3))
	{
		if (CheckABCPai(iValue1, iValue4, iValue5))return true;
	}
	//如果右中两个为将 
	if (CheckAAPai(iValue3, iValue4))
	{
		if (CheckABCPai(iValue1, iValue2, iValue5))return true;
	}
	//如果是右边两个为将，左边为三重张或三连张 
	if (CheckAAPai(iValue4, iValue5))
	{
		if (Check3Pai(iValue1, iValue2, iValue3))return true;
	}

	return false;
}
//检测是否胡牌（8张） 
bool    CMJ::Check8Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8)
{
	//如果是左边两个为将，右边为三重张或三连张 
	if (CheckAAPai(iValue1, iValue2))
	{
		return Check6Pai(iValue3, iValue4, iValue5, iValue6, iValue7, iValue8);
	}

	//如果是中间两个为将，左右边为三重张或三连张 
	if (CheckAAPai(iValue4, iValue5))
	{
		if (Check3Pai(iValue1, iValue2, iValue3) && Check3Pai(iValue6, iValue7, iValue8))return true;
	}

	//如果是右边两个为将，左边为三重张或三连张 
	if (CheckAAPai(iValue7, iValue8))
	{
		return Check6Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6);
	}

	return false;
}
//检测是否胡牌（张） 
bool    CMJ::Check11Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11)
{
	//如果是左边两个为将 
	if (CheckAAPai(iValue1, iValue2))
	{
		return Check9Pai(iValue3, iValue4, iValue5, iValue6, iValue7, iValue8, iValue9, iValue10, iValue11);
	}

	//如果是中间两个为将 
	if (CheckAAPai(iValue4, iValue5))
	{
		//无AAA，全ABC 
		if (Check3Pai(iValue1, iValue2, iValue3) && Check6Pai(iValue4, iValue5, iValue6, iValue7, iValue8, iValue9))return true;
	}

	//如果是右边两个为将 
	if (CheckAAPai(iValue7, iValue8))
	{
		//无AAA，全ABC 
		if (Check3Pai(iValue9, iValue10, iValue11) && Check6Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6))return true;
	}

	//如果是右边两个为将 
	if (CheckAAPai(iValue10, iValue11))
	{
		return Check9Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6, iValue7, iValue8, iValue9);
	}
	return false;
}
//检测是否胡牌（14张） 
bool    CMJ::Check14Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11, int iValue12, int iValue13, int iValue14)
{
	//如果是左边两个为将，右边为三重张或三连张 
	if (CheckAAPai(iValue1, iValue2))
	{
		//无AAA，全ABC 
		if (Check12Pai(iValue3, iValue4, iValue5, iValue6, iValue7, iValue8, iValue9, iValue10, iValue11, iValue12, iValue13, iValue14))return true;
		return false;
	}

	//如果是中间两个为将，左右边为三重张或三连张 
	if (CheckAAPai(iValue4, iValue5))
	{
		//无AAA，全ABC 
		if (Check3Pai(iValue1, iValue2, iValue3) && Check9Pai(iValue6, iValue7, iValue8, iValue9, iValue10, iValue11, iValue12, iValue13, iValue14))return true;
		return false;
	}

	//如果是中间两个为将，左右边为三重张或三连张 
	if (CheckAAPai(iValue7, iValue8))
	{
		//无AAA，全ABC 
		if (Check6Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6) && Check6Pai(iValue9, iValue10, iValue11, iValue12, iValue13, iValue14))return true;
		return false;
	}

	//如果是中间两个为将，左右边为三重张或三连张 
	if (CheckAAPai(iValue10, iValue11))
	{
		//无AAA，全ABC 
		if (Check3Pai(iValue12, iValue13, iValue14) && Check9Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6, iValue7, iValue8, iValue9))return true;
		return false;
	}

	//如果是右边两个为将，左右边为三重张或三连张 
	if (CheckAAPai(iValue13, iValue14))
	{
		//无AAA，全ABC 
		if (Check12Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6, iValue7, iValue8, iValue9, iValue10, iValue11, iValue12))return true;
	}
	return false;
}
//检测是否胡牌（张） 
bool    CMJ::Check3Pai(int iValue1, int iValue2, int iValue3)
{
	if (CheckABCPai(iValue1, iValue2, iValue3))return true;
	if (CheckAAAPai(iValue1, iValue2, iValue3))return true;
	return false;
}
//检测是否胡牌（张） 
bool    CMJ::Check6Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6)
{
	if (Check3Pai(iValue1, iValue2, iValue3) && Check3Pai(iValue4, iValue5, iValue6))return true;
	if (Check3Pai(iValue1, iValue2, iValue3) && Check3Pai(iValue4, iValue5, iValue6))return true;
	//三连对 
	if (CheckAABBCCPai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6))return true;
	//第一张牌四连张 
	if (CheckAAAAPai(iValue2, iValue3, iValue4, iValue5))
	{
		if (CheckABCPai(iValue1, iValue2, iValue6))return true;
	}
	return false;
}

//检测是否胡牌（张） 
bool    CMJ::Check9Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9)
{
	if (CheckABCPai(iValue1, iValue2, iValue3) && Check6Pai(iValue4, iValue5, iValue6, iValue7, iValue8, iValue9))return true;
	if (CheckAAAPai(iValue1, iValue2, iValue3) && Check6Pai(iValue4, iValue5, iValue6, iValue7, iValue8, iValue9))return true;
	if (CheckABCPai(iValue7, iValue8, iValue9) && Check6Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6))return true;
	if (CheckAAAPai(iValue7, iValue8, iValue9) && Check6Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6))return true;

	return false;
}

//检测是否胡牌（张） 
bool    CMJ::Check12Pai(int iValue1, int iValue2, int iValue3, int iValue4, int iValue5, int iValue6, int iValue7, int iValue8, int iValue9, int iValue10, int iValue11, int iValue12)
{
	if (CheckABCPai(iValue1, iValue2, iValue3) && Check9Pai(iValue4, iValue5, iValue6, iValue7, iValue8, iValue9, iValue10, iValue11, iValue12))return true;
	if (CheckAAAPai(iValue1, iValue2, iValue3) && Check9Pai(iValue4, iValue5, iValue6, iValue7, iValue8, iValue9, iValue10, iValue11, iValue12))return true;
	if (CheckABCPai(iValue10, iValue11, iValue12) && Check9Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6, iValue7, iValue8, iValue9))return true;
	if (CheckAAAPai(iValue10, iValue11, iValue12) && Check9Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6, iValue7, iValue8, iValue9))return true;
	if (Check6Pai(iValue1, iValue2, iValue3, iValue4, iValue5, iValue6) && Check6Pai(iValue7, iValue8, iValue9, iValue10, iValue11, iValue12))return true;

	return false;
}
//检测是否是大四喜 
bool    CMJ::CheckD4X_HU()
{
	//东西南北四杠 
	if (m_GangPAIVec[1].size() == 16)
	{
		//将牌 
		for (int i = 0; i < 6; i++)
		{
			if (m_MyPAIVec[i].size() == 2)
			{
				//如果是将 
				if (m_MyPAIVec[i][0] == m_MyPAIVec[i][1])
				{
					return true;
				}
			}
		}
	}
	return false;

}

//检则是否是大三元 
bool    CMJ::CheckD3Y_HU()
{
	//中发白三杠 
	if (m_GangPAIVec[0].size() == 12)
	{
		//将牌 
		for (int i = 0; i < 6; i++)
		{
			if (m_MyPAIVec[i].size() == 2)
			{
				//如果是将 
				if (m_MyPAIVec[i][0] == m_MyPAIVec[i][1])
				{
					return true;
				}
			}
		}
	}
	return false;
}

//检测是否绿一色 
bool    CMJ::CheckL1S_HU()
{
	//只准有发财和条 
	if (m_MyPAIVec[1].size()>0)return false;
	if (m_MyPAIVec[2].size()>0)return false;
	if (m_MyPAIVec[4].size()>0)return false;
	if (m_MyPAIVec[5].size()>0)return false;
	if (m_ChiPAIVec[1].size()>0)return false;
	if (m_ChiPAIVec[2].size()>0)return false;
	if (m_ChiPAIVec[4].size()>0)return false;
	if (m_ChiPAIVec[5].size()>0)return false;
	if (m_PengPAIVec[1].size()>0)return false;
	if (m_PengPAIVec[2].size()>0)return false;
	if (m_PengPAIVec[4].size()>0)return false;
	if (m_PengPAIVec[5].size()>0)return false;
	if (m_GangPAIVec[1].size()>0)return false;
	if (m_GangPAIVec[2].size()>0)return false;
	if (m_GangPAIVec[4].size()>0)return false;
	if (m_GangPAIVec[5].size()>0)return false;
	//对发财 
	if (m_MyPAIVec[0].size() == 2)
	{
		if (m_MyPAIVec[0][0] == 2 && m_MyPAIVec[0][1] == 2)
		{
			for (int i = 1; i < 6; i++)
			{
				if (i == 3)continue;
				if (m_MyPAIVec[i].size()>0)return false;
				if (m_ChiPAIVec[i].size()>0)return false;
				if (m_PengPAIVec[i].size()>0)return false;
				if (m_GangPAIVec[i].size()>0)return false;
			}
			//吃 
			int iSize = m_ChiPAIVec[3].size();
			if (iSize>0)
			{
				vector<  int >::iterator Iter;
				for (Iter = m_ChiPAIVec[3].begin(); Iter != m_ChiPAIVec[3].end(); Iter++)
				{
					if ((*Iter) == 1)return false;
					if ((*Iter) == 5)return false;
					if ((*Iter) == 7)return false;
					if ((*Iter) == 9)return false;
				}
			}
			//碰 
			iSize = m_PengPAIVec[3].size();
			if (iSize>0)
			{
				vector<  int >::iterator Iter;
				for (Iter = m_PengPAIVec[3].begin(); Iter != m_PengPAIVec[3].end(); Iter++)
				{
					if ((*Iter) == 1)return false;
					if ((*Iter) == 5)return false;
					if ((*Iter) == 7)return false;
					if ((*Iter) == 9)return false;
				}
			}
			//杠 
			iSize = m_GangPAIVec[3].size();
			if (iSize>0)
			{
				vector<  int >::iterator Iter;
				for (Iter = m_GangPAIVec[3].begin(); Iter != m_GangPAIVec[3].end(); Iter++)
				{
					if ((*Iter) == 1)return false;
					if ((*Iter) == 5)return false;
					if ((*Iter) == 7)return false;
					if ((*Iter) == 9)return false;
				}
			}
			//起 
			iSize = m_MyPAIVec[3].size();
			if (iSize>0)
			{
				vector<  int >::iterator Iter;
				for (Iter = m_MyPAIVec[3].begin(); Iter != m_MyPAIVec[3].end(); Iter++)
				{
					if ((*Iter) == 1)return false;
					if ((*Iter) == 5)return false;
					if ((*Iter) == 7)return false;
					if ((*Iter) == 9)return false;
				}

			}
		}
	}
	else
	{
		return false;
	}
	//如果有三张 
	if (m_MyPAIVec[3].size() == 3)
	{
		if (Check3Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2]))return true;
	}
	//如果有六张 
	if (m_MyPAIVec[3].size() == 6)
	{
		if (Check6Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4], m_MyPAIVec[3][5]))return true;
	}
	//九张 
	if (m_MyPAIVec[3].size() == 9)
	{
		if (Check9Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4], m_MyPAIVec[3][5], m_MyPAIVec[3][6], m_MyPAIVec[3][7], m_MyPAIVec[3][8]))return true;
	}
	//十二张 
	if (m_MyPAIVec[3].size() == 12)
	{
		if (Check12Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4], m_MyPAIVec[3][5], m_MyPAIVec[3][6], m_MyPAIVec[3][7], m_MyPAIVec[3][8], m_MyPAIVec[3][9], m_MyPAIVec[3][10], m_MyPAIVec[3][11]))return true;
	}
	return  false;
}

//检测是否九莲宝灯（胡） 
bool    CMJ::Check9LBD_HU()
{
	if (m_9LBD)//如果已经成九连宝灯牌型 
	{
		if (m_MyPAIVec[2].size() == 14)return true;
		if (m_MyPAIVec[3].size() == 14)return true;
		if (m_MyPAIVec[4].size() == 14)return true;
	}
	return false;
}
//检测是否九莲宝灯牌型（听） 
bool    CMJ::Check9LBD_TING()
{
	for (UINT i = 2; i < 5; i++)
	{
		if (m_MyPAIVec[i].size() == 13)
		{
			if (m_MyPAIVec[i][0] == 1 && m_MyPAIVec[i][1] == 1 && m_MyPAIVec[i][2] == 1)
			{
				if (m_MyPAIVec[i][3] == 2 && m_MyPAIVec[i][4] == 3 && m_MyPAIVec[i][5] == 4 && m_MyPAIVec[i][6] == 5 && m_MyPAIVec[i][7] == 6 && m_MyPAIVec[i][8] == 7 && m_MyPAIVec[i][9] == 8)
				{
					if (m_MyPAIVec[i][10] == 9 && m_MyPAIVec[i][11] == 9 && m_MyPAIVec[i][12] == 9)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}
//检测是否是四杠 
bool    CMJ::Check4Gang_HU()
{
	int iSize = 0;
	for (UINT i = 0; i < 6; i++)
	{
		iSize = m_GangPAIVec[i].size();
	}

	if (iSize == 16)
	{
		//将牌 
		for (int i = 0; i < 6; i++)
		{
			//如果是将 
			if (CheckAAPai(m_MyPAIVec[i][0], m_MyPAIVec[i][1]))
			{
				return true;
			}
		}

	}
	return false;
}

//检测是否连七对 
bool    CMJ::CheckL7D_HU()
{
	for (UINT i = 2; i < 5; i++)
	{
		if (m_MyPAIVec[i].size() == 14)
		{
			if (m_MyPAIVec[i][0] == 1 && m_MyPAIVec[i][1] == 1 && m_MyPAIVec[i][2] == 2 && m_MyPAIVec[i][3] == 2 && m_MyPAIVec[i][4] == 3 && m_MyPAIVec[i][5] == 3 && m_MyPAIVec[i][6] == 4 && m_MyPAIVec[i][7] == 4 && m_MyPAIVec[i][8] == 5 && m_MyPAIVec[i][9] == 5 && m_MyPAIVec[i][10] == 6 && m_MyPAIVec[i][11] == 6 && m_MyPAIVec[i][12] == 7 && m_MyPAIVec[i][13] == 7)
			{
				return true;
			}
			if (m_MyPAIVec[i][0] == 2 && m_MyPAIVec[i][1] == 2 && m_MyPAIVec[i][2] == 3 && m_MyPAIVec[i][3] == 3 && m_MyPAIVec[i][4] == 4 && m_MyPAIVec[i][5] == 4 && m_MyPAIVec[i][6] == 5 && m_MyPAIVec[i][7] == 5 && m_MyPAIVec[i][8] == 6 && m_MyPAIVec[i][9] == 6 && m_MyPAIVec[i][10] == 7 && m_MyPAIVec[i][11] == 7 && m_MyPAIVec[i][12] == 8 && m_MyPAIVec[i][13] == 8)
			{
				return true;
			}
			if (m_MyPAIVec[i][0] == 3 && m_MyPAIVec[i][1] == 3 && m_MyPAIVec[i][2] == 4 && m_MyPAIVec[i][3] == 4 && m_MyPAIVec[i][4] == 5 && m_MyPAIVec[i][5] == 5 && m_MyPAIVec[i][6] == 6 && m_MyPAIVec[i][7] == 6 && m_MyPAIVec[i][8] == 7 && m_MyPAIVec[i][9] == 7 && m_MyPAIVec[i][10] == 8 && m_MyPAIVec[i][11] == 8 && m_MyPAIVec[i][12] == 9 && m_MyPAIVec[i][13] == 9)
			{
				return true;
			}
		}
	}

	return false;
}

//检测是否胡十三幺 
bool    CMJ::Chekc13Y_HU()
{
	if (m_13Y)
	{
		bool        i13YSize[13];
		for (UINT i = 0; i < 13; i++)
		{
			i13YSize[i] = false;
		}
		//中发白 
		vector< int>::iterator Iter;
		for (Iter = m_MyPAIVec[0].begin(); Iter != m_MyPAIVec[0].end(); Iter++)
		{
			if ((*Iter) == 1)
			{
				i13YSize[0] = true;
			}
			if ((*Iter) == 2)
			{
				i13YSize[1] = true;
			}
			if ((*Iter) == 3)
			{
				i13YSize[2] = true;
			}
		}
		//东南西北风 
		for (Iter = m_MyPAIVec[1].begin(); Iter != m_MyPAIVec[1].end(); Iter++)
		{
			if ((*Iter) == 1)
			{
				i13YSize[3] = true;
			}
			if ((*Iter) == 2)
			{
				i13YSize[4] = true;
			}
			if ((*Iter) == 3)
			{
				i13YSize[5] = true;
			}
			if ((*Iter) == 4)
			{
				i13YSize[6] = true;
			}
		}
		//一九万 
		for (Iter = m_MyPAIVec[2].begin(); Iter != m_MyPAIVec[2].end(); Iter++)
		{
			if ((*Iter) == 1)
			{
				i13YSize[7] = true;
			}
			if ((*Iter) == 9)
			{
				i13YSize[8] = true;
			}
		}

		//一九条 
		for (Iter = m_MyPAIVec[3].begin(); Iter != m_MyPAIVec[3].end(); Iter++)
		{
			if ((*Iter) == 1)
			{
				i13YSize[9] = true;
			}
			if ((*Iter) == 9)
			{
				i13YSize[10] = true;
			}
		}

		//一九饼 
		for (Iter = m_MyPAIVec[4].begin(); Iter != m_MyPAIVec[4].end(); Iter++)
		{
			if ((*Iter) == 1)
			{
				i13YSize[11] = true;
			}
			if ((*Iter) == 9)
			{
				i13YSize[12] = true;
			}
		}
		int icount = 0;
		for (UINT i = 0; i < 13; i++)
		{
			if (i13YSize[i] == true)
			{
				icount++;
			}
		}
		if (icount == 13)return true;

	}
	return false;
}
//检测是否清幺九 
bool    CMJ::CheckQY9_HU()
{
	int iSize = 0;
	int iCount = 0;
	for (UINT i = 2; i < 5; i++)
	{
		iSize = m_GangPAIVec[i].size();
		iCount += iSize;
		for (UINT j = 0; j < iSize; j++)
		{
			if (m_GangPAIVec[i][j] != 1 || m_GangPAIVec[i][j] != 9)return false;
		}
	}

	if (iCount == 12)
	{
		if (m_MyPAIVec[2].size() == 2)
		{
			if (m_MyPAIVec[2][0] == 1 && m_MyPAIVec[2][1] == 1)return true;
			if (m_MyPAIVec[2][0] == 9 && m_MyPAIVec[2][1] == 9)return true;
		}

		if (m_MyPAIVec[3].size() == 2)
		{
			if (m_MyPAIVec[3][0] == 1 && m_MyPAIVec[3][1] == 1)return true;
			if (m_MyPAIVec[3][0] == 9 && m_MyPAIVec[3][1] == 9)return true;
		}

		if (m_MyPAIVec[4].size() == 2)
		{
			if (m_MyPAIVec[4][0] == 1 && m_MyPAIVec[4][1] == 1)return true;
			if (m_MyPAIVec[4][0] == 9 && m_MyPAIVec[4][1] == 9)return true;
		}
	}

	return false;

}
//检测是否胡小四喜 
bool    CMJ::CheckX4X_HU()
{
	//东西南北四杠 
	if (m_GangPAIVec[1].size() == 12)
	{
		//将牌的位置 
		int iJiangPos = -1;
		//将牌 
		for (int i = 0; i < 6; i++)
		{
			if (m_MyPAIVec[i].size() == 5)
			{

				if (Check5Pai(m_MyPAIVec[i][0], m_MyPAIVec[i][1], m_MyPAIVec[i][2], m_MyPAIVec[i][3], m_MyPAIVec[i][4]))
				{
					return true;
				}
			}
			if (m_MyPAIVec[i].size() == 2)
			{
				//如果是将 
				if (CheckAAPai(m_MyPAIVec[i][0], m_MyPAIVec[i][1]))
				{
					iJiangPos = i;
					break;
				}
			}
		}
		// 
		if (iJiangPos>0)
		{
			for (int i = 0; i < 6; i++)
			{
				if (i != iJiangPos)
				{
					if ((m_MyPAIVec[i].size() == 3))
					{
						if (Check3Pai(m_MyPAIVec[i][0], m_MyPAIVec[i][1], m_MyPAIVec[i][2]))return true;
					}
				}
			}
		}
	}
	return false;
}
//检测是否胡小三元 
bool    CMJ::CheckX3Y_HU()
{
	//东西南北四杠 
	if (m_GangPAIVec[0].size() == 8)
	{
		if (m_MyPAIVec[0].size() == 5)
		{
			if (Check5Pai(m_MyPAIVec[0][0], m_MyPAIVec[0][1], m_MyPAIVec[0][2], m_MyPAIVec[0][3], m_MyPAIVec[0][4]))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (m_MyPAIVec[0].size() == 2)
		{
			//如果是将 
			if (CheckAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1]) == false)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		return CheckHU();

	}
	return false;
}
//检测是否胡字一色 
bool    CMJ::CheckZ1S_HU()
{
	//只准有字 
	if (m_MyPAIVec[2].size()>0)return false;
	if (m_MyPAIVec[3].size()>0)return false;
	if (m_MyPAIVec[4].size()>0)return false;
	if (m_MyPAIVec[5].size()>0)return false;
	if (m_ChiPAIVec[2].size()>0)return false;
	if (m_ChiPAIVec[3].size()>0)return false;
	if (m_ChiPAIVec[4].size()>0)return false;
	if (m_ChiPAIVec[5].size()>0)return false;
	if (m_PengPAIVec[2].size()>0)return false;
	if (m_PengPAIVec[3].size()>0)return false;
	if (m_PengPAIVec[4].size()>0)return false;
	if (m_PengPAIVec[5].size()>0)return false;
	if (m_GangPAIVec[2].size()>0)return false;
	if (m_GangPAIVec[3].size()>0)return false;
	if (m_GangPAIVec[4].size()>0)return false;
	if (m_GangPAIVec[5].size()>0)return false;
	int iSize = m_MyPAIVec[0].size();
	if (iSize > 0)
	{
		if (iSize == 2)
		{
			if (m_MyPAIVec[0][0] == m_MyPAIVec[0][1])
			{
				iSize = m_MyPAIVec[1].size();
				if (iSize == 0)return true;
				if (iSize == 3)
				{
					if (CheckAAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1], m_MyPAIVec[1][2]))return true;
				}
			}
		}

	}
	return false;
}
//检测是否四暗刻 
bool    CMJ::Check4AK_HU()
{
	if (m_4AK)
	{
		//将牌 
		for (int i = 0; i < 6; i++)
		{
			if (m_MyPAIVec[i].size() == 2)
			{
				//如果是将 
				if (m_MyPAIVec[i][0] == m_MyPAIVec[i][1])
				{
					return true;
				}
			}
		}
	}
	return false;
}
//检测是否一色双龙会 
bool    CMJ::Check1S2LH_HU()
{
	//万，条，饼 
	for (UINT i = 0; i <= 4; i++)
	{
		int iType = i;
		if (m_MyPAIVec[iType].size() == 14)
		{

			if (m_MyPAIVec[iType][0] == 1 && m_MyPAIVec[iType][1] == 1)
			{

			}
			else
			{
				return false;
			}
			if (m_MyPAIVec[iType][2] == 2 && m_MyPAIVec[iType][3] == 2)
			{

			}
			else
			{
				return false;
			}
			if (m_MyPAIVec[iType][4] == 3 && m_MyPAIVec[iType][5] == 3)
			{

			}
			else
			{
				return false;
			}
			if (m_MyPAIVec[iType][6] == 5 && m_MyPAIVec[iType][7] == 5)
			{

			}
			else
			{
				return false;
			}
			if (m_MyPAIVec[iType][8] == 7 && m_MyPAIVec[iType][9] == 7)
			{

			}
			else
			{
				return false;
			}
			if (m_MyPAIVec[iType][10] == 8 && m_MyPAIVec[iType][11] == 8)
			{

			}
			else
			{
				return false;
			}
			if (m_MyPAIVec[iType][12] == 9 && m_MyPAIVec[iType][13] == 9)
			{

			}
			else
			{
				return false;
			}

			return true;
		}
	}
	return false;
}

//检测是否一色四同顺 
bool    CMJ::Check1S4TS_HU()
{
	//万，条，饼 
	for (UINT i = 0; i <= 4; i++)
	{
		int iType = i;
		//吃过的顺 
		int iSize1 = m_ChiPAIVec[iType].size();
		//剩余牌墙中的顺 
		int iSize2 = m_MyPAIVec[iType].size();
		//万 
		if (iSize1 + iSize2 >= 12)
		{
			//无吃的顺 
			if (iSize1 == 0)
			{
				if (iSize2 == 12)
				{
					//三连暗杠成顺 
					if (CheckAAAABBBBCCCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10], m_MyPAIVec[iType][11]))return CheckHU();
					return false;
				}
				if (iSize2 == 14)
				{
					//三连暗杠成顺 
					if ((m_MyPAIVec[iType][12] == (m_MyPAIVec[iType][13])) && CheckAAAABBBBCCCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10], m_MyPAIVec[iType][11]))return true;
					//三连暗杠成顺 
					if ((m_MyPAIVec[iType][0] == (m_MyPAIVec[iType][1])) && CheckAAAABBBBCCCCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10], m_MyPAIVec[iType][11], m_MyPAIVec[iType][12], m_MyPAIVec[iType][13]))return true;
					return false;
				}
			}
			//吃到一个顺 
			if (iSize1 == 3)
			{
				if (iSize2 == 9)
				{
					//三连高压 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][6])
						{
							return CheckHU();
						}
					}
					return false;
				}
				if (iSize2 == 11)
				{

					//三连高压 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8]))
					{
						if (m_MyPAIVec[iType][9] == m_MyPAIVec[iType][10] && m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][6])
						{
							return true;
						}
					}
					//三连高压 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10]))
					{
						if (m_MyPAIVec[iType][0] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][5] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][8])
						{
							return true;
						}
					}

					return false;
				}
			}
			//吃到二个顺 
			if (iSize1 == 6)
			{
				if (iSize2 == 6)
				{
					//三连对 
					if (CheckAABBCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][4])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][4])
							{
								return CheckHU();
							}
						}
					}
					return false;
				}
				if (iSize2 == 8)
				{
					//三连对 
					if (CheckAABBCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5]))
					{
						if (m_MyPAIVec[iType][6] == m_MyPAIVec[iType][7] && m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][4])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][4])
							{
								return true;
							}

						}
					}

					//三连对 
					if (CheckAABBCCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7]))
					{
						if (m_MyPAIVec[iType][0] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][4] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][6])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][4] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][6])
							{
								return true;
							}
						}
					}

					return false;
				}
			}
			//吃到三个顺 
			if (iSize1 == 9)
			{
				if (iSize2 == 3)
				{
					//顺子 
					if (CheckABCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][2])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][2])
							{
								if (m_ChiPAIVec[iType][6] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][7] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][8] == m_MyPAIVec[iType][2])
								{
									return CheckHU();
								}
							}
						}
					}
					return false;
				}
				if (iSize2 == 5)
				{
					//顺子 
					if (CheckABCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2]))
					{
						if (m_MyPAIVec[iType][3] == m_MyPAIVec[iType][4] && m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][2])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][2])
							{
								if (m_ChiPAIVec[iType][6] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][7] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][8] == m_MyPAIVec[iType][2])
								{
									return true;
								}
							}
						}
					}

					//顺子 
					if (CheckABCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4]))
					{
						if (m_MyPAIVec[iType][0] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][4])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][4])
							{
								if (m_ChiPAIVec[iType][6] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][7] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][8] == m_MyPAIVec[iType][4])
								{
									return true;
								}

							}
						}
					}
					return false;
				}
			}

			//吃到三连顺 
			if (iSize1 == 12)
			{
				if ((m_ChiPAIVec[iType][0] == m_ChiPAIVec[iType][3]) && (m_ChiPAIVec[iType][3] == m_ChiPAIVec[iType][6]) && (m_ChiPAIVec[iType][6] == m_ChiPAIVec[iType][9]))
				{
					return CheckHU();
				}
				return false;
			}
		}

	}
	return false;
}

//检测是否一色四节高 
bool    CMJ::Check1S4JG_HU()
{
	//万，条，饼 
	for (UINT i = 2; i <= 4; i++)
	{
		int iType = i;
		if (m_GangPAIVec[iType].size() == 16)
		{
			if ((m_GangPAIVec[iType][0] == m_GangPAIVec[iType][4] - 1) && (m_GangPAIVec[iType][4] == m_GangPAIVec[iType][8] - 1) && (m_GangPAIVec[iType][8] == m_GangPAIVec[iType][12] - 1))
			{
				return CheckHU();
			}
		}
	}
	return false;
}
//检测是否一色四步高 
bool    CMJ::Check1S4BG_HU()
{

	return false;
}
//检测是否三杠 
bool    CMJ::Check3Gang_HU()
{
	int iSize = 0;
	for (UINT i = 0; i < 6; i++)
	{
		iSize = m_GangPAIVec[i].size();
	}

	if (iSize == 12)
	{
		//将牌 
		return CheckHU();

	}
	return false;
}
//检测是否混幺九 
bool    CMJ::CheckHY9_HU()
{
	return false;
}
//检测是否七对 
bool    CMJ::Check7D_HU()
{
	int iDoubleNum = 0;
	for (UINT i = 0; i < 6; i++)
	{
		UINT iSize = m_MyPAIVec[i].size();
		if (iSize % 2 == 1 || iSize == 0)return false;//如果是奇数肯定不是对 
		for (UINT j = 0; j < iSize - 1; j++)
		{
			if (m_MyPAIVec[i][j] == m_MyPAIVec[i][j + 1])
			{
				iDoubleNum++;
				j++;
			}
		}
	}
	if (iDoubleNum == 7)return true;
	return false;
}
//检测是否七星不靠 
bool    CMJ::Check7XBK_HU()
{
	bool        bIs7XBK[14];
	for (UINT i = 0; i < 14; i++)
	{
		bIs7XBK[i] = false;
	}
	//中发白 
	vector< int>::iterator Iter;
	if (m_MyPAIVec[0].size() != 3)return false;
	for (Iter = m_MyPAIVec[0].begin(); Iter != m_MyPAIVec[0].end(); Iter++)
	{
		if ((*Iter) == 1)
		{
			bIs7XBK[7] = true;
		}
		if ((*Iter) == 2)
		{
			bIs7XBK[8] = true;
		}
		if ((*Iter) == 3)
		{
			bIs7XBK[9] = true;
		}
	}
	//东南西北风 
	if (m_MyPAIVec[1].size() != 4)return false;
	for (Iter = m_MyPAIVec[1].begin(); Iter != m_MyPAIVec[1].end(); Iter++)
	{
		if ((*Iter) == 1)
		{
			bIs7XBK[10] = true;
		}
		if ((*Iter) == 2)
		{
			bIs7XBK[11] = true;
		}
		if ((*Iter) == 3)
		{
			bIs7XBK[12] = true;
		}
		if ((*Iter) == 4)
		{
			bIs7XBK[13] = true;
		}
	}
	//万,条,饼 
	for (UINT i = 2; i <= 4; i++)
	{
		if (m_MyPAIVec[i].size() == 3)
		{
			if (m_MyPAIVec[i][0] == 1 && m_MyPAIVec[i][1] == 4 && m_MyPAIVec[i][2] == 7)
			{
				bIs7XBK[0] = true;
				bIs7XBK[3] = true;
				bIs7XBK[6] = true;
			}
		}
		else if (m_MyPAIVec[2].size() == 2)
		{
			if (m_MyPAIVec[i][0] == 2 && m_MyPAIVec[i][1] == 5)
			{
				bIs7XBK[1] = true;
				bIs7XBK[4] = true;
			}
			else if (m_MyPAIVec[i][0] == 3 && m_MyPAIVec[i][1] == 6)
			{
				bIs7XBK[2] = true;
				bIs7XBK[5] = true;
			}
		}
		else
		{
			return false;
		}
	}
	bool t_Result = true;
	for (UINT i = 0; i < 14; i++)
	{
		if (bIs7XBK[i] == false)t_Result = false;
	}
	if (t_Result)return true;

	for (UINT i = 2; i <= 4; i++)
	{
		if (m_MyPAIVec[i].size() == 3)
		{
			if (m_MyPAIVec[i][0] == 2 && m_MyPAIVec[i][1] == 5 && m_MyPAIVec[i][2] == 8)
			{
				bIs7XBK[0] = true;
				bIs7XBK[3] = true;
				bIs7XBK[6] = true;
			}
		}
		else if (m_MyPAIVec[2].size() == 2)
		{
			if (m_MyPAIVec[i][0] == 3 && m_MyPAIVec[i][1] == 6)
			{
				bIs7XBK[1] = true;
				bIs7XBK[4] = true;
			}
			else if (m_MyPAIVec[i][0] == 4 && m_MyPAIVec[i][1] == 7)
			{
				bIs7XBK[2] = true;
				bIs7XBK[5] = true;
			}
		}
		else
		{
			return false;
		}
	}
	t_Result = true;
	for (UINT i = 0; i < 14; i++)
	{
		if (bIs7XBK[i] == false)t_Result = false;
	}
	if (t_Result)return true;

	for (UINT i = 2; i <= 4; i++)
	{
		if (m_MyPAIVec[i].size() == 3)
		{
			if (m_MyPAIVec[i][0] == 3 && m_MyPAIVec[i][1] == 6 && m_MyPAIVec[i][2] == 9)
			{
				bIs7XBK[0] = true;
				bIs7XBK[3] = true;
				bIs7XBK[6] = true;
			}
		}
		else if (m_MyPAIVec[2].size() == 2)
		{
			if (m_MyPAIVec[i][0] == 4 && m_MyPAIVec[i][1] == 7)
			{
				bIs7XBK[1] = true;
				bIs7XBK[4] = true;
			}
			else if (m_MyPAIVec[i][0] == 5 && m_MyPAIVec[i][1] == 8)
			{
				bIs7XBK[2] = true;
				bIs7XBK[5] = true;
			}
		}
		else
		{
			return false;
		}
	}

	t_Result = true;
	for (UINT i = 0; i < 14; i++)
	{
		if (bIs7XBK[i] == false)t_Result = false;
	}
	if (t_Result)return true;

	return false;
}
//检测是否全双刻 
bool    CMJ::CheckQSK_HU()
{
	//万，条，饼 
	for (UINT i = 2; i <= 4; i++)
	{
		int iType = i;
		if (m_GangPAIVec[iType].size() == 16)
		{
			if (m_GangPAIVec[iType][0] == 2 && m_GangPAIVec[iType][4] == 4 && m_GangPAIVec[iType][8] == 6 && m_GangPAIVec[iType][12] == 8)
			{
				return CheckHU();
			}
		}
	}
	return false;
}
//清一色 
bool    CMJ::CheckQ1S_HU()
{

	if (m_MyPAIVec[0].empty() == false)return false;
	if (m_MyPAIVec[1].empty() == false)return false;
	if (m_ChiPAIVec[0].empty() == false)return false;
	if (m_ChiPAIVec[1].empty() == false)return false;
	if (m_PengPAIVec[0].empty() == false)return false;
	if (m_PengPAIVec[1].empty() == false)return false;
	if (m_GangPAIVec[0].empty() == false)return false;
	if (m_GangPAIVec[1].empty() == false)return false;
	//清万 
	if (m_MyPAIVec[2].empty() == false)
	{
		if (m_MyPAIVec[3].empty() == false)return false;
		if (m_MyPAIVec[4].empty() == false)return false;
		if (m_ChiPAIVec[3].empty() == false)return false;
		if (m_ChiPAIVec[4].empty() == false)return false;
		if (m_PengPAIVec[3].empty() == false)return false;
		if (m_PengPAIVec[4].empty() == false)return false;
		if (m_GangPAIVec[3].empty() == false)return false;
		if (m_GangPAIVec[4].empty() == false)return false;
		return CheckHU();
	}
	//清条 
	if (m_MyPAIVec[3].empty() == false)
	{
		if (m_MyPAIVec[2].empty() == false)return false;
		if (m_MyPAIVec[4].empty() == false)return false;
		if (m_ChiPAIVec[2].empty() == false)return false;
		if (m_ChiPAIVec[4].empty() == false)return false;
		if (m_PengPAIVec[2].empty() == false)return false;
		if (m_PengPAIVec[4].empty() == false)return false;
		if (m_GangPAIVec[2].empty() == false)return false;
		if (m_GangPAIVec[4].empty() == false)return false;
		return CheckHU();
	}
	//清饼 
	if (m_MyPAIVec[4].empty() == false)
	{
		if (m_MyPAIVec[2].empty() == false)return false;
		if (m_MyPAIVec[3].empty() == false)return false;
		if (m_ChiPAIVec[2].empty() == false)return false;
		if (m_ChiPAIVec[3].empty() == false)return false;
		if (m_PengPAIVec[2].empty() == false)return false;
		if (m_PengPAIVec[3].empty() == false)return false;
		if (m_GangPAIVec[2].empty() == false)return false;
		if (m_GangPAIVec[3].empty() == false)return false;
		return CheckHU();
	}
	return false;
}
//检测是否一色三同顺 
bool    CMJ::Check1S3TS_HU()
{
	//万条饼 
	for (UINT i = 2; i <= 4; i++)
	{
		int iType = i;
		//吃过的顺 
		int iSize1 = m_ChiPAIVec[iType].size();
		//剩余牌墙中的顺 
		int iSize2 = m_MyPAIVec[iType].size();
		if (iSize1 + iSize2 >= 9)
		{
			//无吃的顺 
			if (iSize1 == 0)
			{
				if (iSize2 == 9)
				{
					//三连高压 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8]))
					{
						return CheckHU();
					}
					return false;
				}
				if (iSize2 == 11)
				{
					//三连高压 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8]) && CheckAAPai(m_MyPAIVec[iType][9], m_MyPAIVec[iType][10]))
					{
						return true;
					}
					//三连高压 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10]) && CheckAAPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1]))
					{
						return true;
					}
					return false;
				}
				if (iSize2 == 12)
				{
					//三连高压 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8]) && Check3Pai(m_MyPAIVec[iType][9], m_MyPAIVec[iType][10], m_MyPAIVec[iType][11]))
					{
						return CheckHU();
					}
					//三连高压 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10], m_MyPAIVec[iType][11]) && Check3Pai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2]))
					{
						return CheckHU();
					}
					return false;
				}
				if (iSize2 == 14)
				{
					//三连顺（前） 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8]) && Check5Pai(m_MyPAIVec[iType][9], m_MyPAIVec[iType][10], m_MyPAIVec[iType][11], m_MyPAIVec[iType][12], m_MyPAIVec[iType][13]))
					{
						return true;
					}
					//三连顺(中) 
					if (CheckAAPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1]) && CheckAAABBBCCCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10]) && Check3Pai(m_MyPAIVec[iType][11], m_MyPAIVec[iType][12], m_MyPAIVec[iType][13]))
					{
						return true;
					}
					//三连顺(中) 
					if (Check3Pai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2]) && CheckAAABBBCCCPai(m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10], m_MyPAIVec[iType][11]) && CheckAAPai(m_MyPAIVec[iType][12], m_MyPAIVec[iType][13]))
					{
						return true;
					}
					//三连顺（后） 
					if (CheckAAABBBCCCPai(m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10], m_MyPAIVec[iType][11], m_MyPAIVec[iType][12], m_MyPAIVec[iType][13]) && Check5Pai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4]))
					{
						return true;
					}
					return false;
				}
			}
			//吃到一个顺 
			if (iSize1 == 3)
			{
				if (iSize2 == 6)
				{
					//三连对 
					if (CheckAABBCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][2])
						{
							return CheckHU();
						}
					}
				}
				if (iSize2 == 8)
				{
					//三连对（前） 
					if (CheckAABBCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5]) && CheckAAPai(m_MyPAIVec[iType][6], m_MyPAIVec[iType][7]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][4])
						{
							return true;
						}
					}
					//三连对（后） 
					if (CheckAABBCCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7]) && CheckAAPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][4] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][6])
						{
							return true;
						}
					}
				}
				if (iSize2 == 9)
				{
					//三连对（前） 
					if (CheckAABBCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5]) && Check3Pai(m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][4])
						{
							return CheckHU();
						}
					}
					//三连对（后） 
					if (CheckAABBCCPai(m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8]) && Check3Pai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][5] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][7])
						{
							return CheckHU();
						}
					}
				}
				if (iSize2 == 11)
				{
					//三连对（前） 
					if (CheckAABBCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5]) && Check5Pai(m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][4])
						{
							return true;
						}
					}
					//三连对（中） 
					if (CheckAAPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1]) && CheckAABBCCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7]) && Check3Pai(m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][4] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][6])
						{
							return true;
						}
					}
					//三连对（中） 
					if (Check3Pai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2]) && CheckAABBCCPai(m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8]) && CheckAAPai(m_MyPAIVec[iType][9], m_MyPAIVec[iType][10]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][5] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][7])
						{
							return true;
						}
					}
					//三连对（前） 
					if (CheckAABBCCPai(m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7], m_MyPAIVec[iType][8], m_MyPAIVec[iType][9], m_MyPAIVec[iType][10]) && Check5Pai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][5] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][7] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][9])
						{
							return true;
						}
					}
				}
			}
			//吃到二个顺 
			if (iSize1 == 6)
			{
				if (iSize2 == 3)
				{
					//顺子 
					if (CheckABCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][2])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][2])
							{
								return CheckHU();
							}
						}
					}
				}
				if (iSize2 == 5)
				{
					//顺子（前） 
					if (CheckABCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2]) && CheckAAPai(m_MyPAIVec[iType][3], m_MyPAIVec[iType][4]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][2])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][2])
							{
								return true;
							}
						}
					}
					//顺子（后） 
					if (CheckABCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4]) && CheckAAPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][4])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][4])
							{
								return true;
							}
						}
					}
				}
				if (iSize2 == 6)
				{
					//三连对 
					if (CheckAABBCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][4])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][5])
							{
								return CheckHU();
							}
						}
					}
				}
				if (iSize2 == 8)
				{
					//三连对(前) 
					if (CheckAABBCCPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1], m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5]) && CheckAAPai(m_MyPAIVec[iType][6], m_MyPAIVec[iType][7]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][0] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][4])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][1] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][5])
							{
								return true;
							}
						}
					}
					//三连对(后) 
					if (CheckAABBCCPai(m_MyPAIVec[iType][2], m_MyPAIVec[iType][3], m_MyPAIVec[iType][4], m_MyPAIVec[iType][5], m_MyPAIVec[iType][6], m_MyPAIVec[iType][7]) && CheckAAPai(m_MyPAIVec[iType][0], m_MyPAIVec[iType][1]))
					{
						if (m_ChiPAIVec[iType][0] == m_MyPAIVec[iType][2] && m_ChiPAIVec[iType][1] == m_MyPAIVec[iType][4] && m_ChiPAIVec[iType][2] == m_MyPAIVec[iType][6])
						{
							if (m_ChiPAIVec[iType][3] == m_MyPAIVec[iType][3] && m_ChiPAIVec[iType][4] == m_MyPAIVec[iType][5] && m_ChiPAIVec[iType][5] == m_MyPAIVec[iType][7])
							{
								return true;
							}
						}
					}
				}
			}
			//吃到三个顺 
			if (iSize1 == 9)
			{
				if ((m_ChiPAIVec[2][0] == m_ChiPAIVec[iType][3]) && (m_ChiPAIVec[iType][3] == m_ChiPAIVec[iType][6]))
				{
					return CheckHU();
				}
			}
		}
	}
	return false;
}
//检测是否一色三节高 
bool    CMJ::Check1S3JG_HU()
{
	//万，条，饼 
	for (UINT i = 2; i <= 4; i++)
	{
		int iType = i;
		if (m_GangPAIVec[iType].size() == 12)
		{
			if ((m_GangPAIVec[iType][0] == m_GangPAIVec[iType][4] - 1) && (m_GangPAIVec[iType][4] == m_GangPAIVec[iType][8] - 1))
			{
				return CheckHU();
			}
		}
	}
	return false;
}
//检测是否全大 
bool    CMJ::CheckQD_HU()
{
	//剑牌，风牌 
	if (m_MyPAIVec[0].empty() == false)return false;
	if (m_MyPAIVec[1].empty() == false)return false;
	if (m_ChiPAIVec[0].empty() == false)return false;
	if (m_ChiPAIVec[1].empty() == false)return false;
	if (m_PengPAIVec[0].empty() == false)return false;
	if (m_PengPAIVec[1].empty() == false)return false;
	if (m_GangPAIVec[0].empty() == false)return false;
	if (m_GangPAIVec[1].empty() == false)return false;

	//万，条，饼 
	for (UINT i = 2; i <= 4; i++)
	{
		if (m_MyPAIVec[i].empty() == false)
		{
			//剩余牌墙 
			int iSize = m_MyPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_MyPAIVec[i][j]<7)return false;
			}
			//吃 
			iSize = m_ChiPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_ChiPAIVec[i][j]<7)return false;
			}
			//碰 
			iSize = m_PengPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_PengPAIVec[i][j]<7)return false;
			}
			//杠 
			iSize = m_GangPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_GangPAIVec[i][j]<7)return false;
			}
		}
	}
	return CheckHU();
}

//检测是否全中 
bool    CMJ::CheckQZ_HU()
{
	//剑牌，风牌 
	if (m_MyPAIVec[0].empty() == false)return false;
	if (m_MyPAIVec[1].empty() == false)return false;
	if (m_ChiPAIVec[0].empty() == false)return false;
	if (m_ChiPAIVec[1].empty() == false)return false;
	if (m_PengPAIVec[0].empty() == false)return false;
	if (m_PengPAIVec[1].empty() == false)return false;
	if (m_GangPAIVec[0].empty() == false)return false;
	if (m_GangPAIVec[1].empty() == false)return false;

	//万，条，饼 
	for (UINT i = 2; i <= 4; i++)
	{
		if (m_MyPAIVec[i].empty() == false)
		{
			//剩余牌墙 
			int iSize = m_MyPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_MyPAIVec[i][j]<4)return false;
				if (m_MyPAIVec[i][j]>6)return false;
			}
			//吃 
			iSize = m_ChiPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_ChiPAIVec[i][j]<4)return false;
				if (m_ChiPAIVec[i][j]>6)return false;
			}
			//碰 
			iSize = m_PengPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_PengPAIVec[i][j]<4)return false;
				if (m_PengPAIVec[i][j]>6)return false;
			}
			//杠 
			iSize = m_GangPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_GangPAIVec[i][j]<4)return false;
				if (m_GangPAIVec[i][j]>6)return false;
			}
		}
	}
	return CheckHU();
}

//检测是否全小 
bool    CMJ::CheckQX_HU()
{
	//剑牌，风牌 
	if (m_MyPAIVec[0].empty() == false)return false;
	if (m_MyPAIVec[1].empty() == false)return false;
	if (m_ChiPAIVec[0].empty() == false)return false;
	if (m_ChiPAIVec[1].empty() == false)return false;
	if (m_PengPAIVec[0].empty() == false)return false;
	if (m_PengPAIVec[1].empty() == false)return false;
	if (m_GangPAIVec[0].empty() == false)return false;
	if (m_GangPAIVec[1].empty() == false)return false;

	//万，条，饼 
	for (UINT i = 2; i <= 4; i++)
	{
		if (m_MyPAIVec[i].empty() == false)
		{
			//剩余牌墙 
			int iSize = m_MyPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_MyPAIVec[i][j]>3)return false;
			}
			//吃 
			iSize = m_ChiPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_ChiPAIVec[i][j]>3)return false;
			}
			//碰 
			iSize = m_PengPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_PengPAIVec[i][j]>3)return false;
			}
			//杠 
			iSize = m_GangPAIVec[i].size();
			for (UINT j = 0; j < iSize; j++)
			{
				if (m_GangPAIVec[i][j]>3)return false;
			}
		}
	}
	return CheckHU();
}
//检测是否青龙 
bool    CMJ::CheckQL_HU()
{
	//花色 
	int iColorNum = 0;
	//万，条，饼 
	for (UINT i = 2; i <= 4; i++)
	{
		if (m_MyPAIVec[i].empty() == false)
		{
			iColorNum++;

			if (m_MyPAIVec[i].size() == 9)
			{
				for (UINT j = 0; j < 9; j++)
				{
					if (m_MyPAIVec[i][j] != j)return false;
				}
			}
		}
	}
	if (iColorNum == 1)return CheckHU();
	return false;
}
//检测是否三色双龙会 
bool    CMJ::Check3S2LH_HU()
{
	//五万为双龙 
	if (m_MyPAIVec[2].size() == 2)
	{
		//双龙 
		if (m_MyPAIVec[2][0] == 5 && m_MyPAIVec[2][1] == 5)
		{
			//老少副 
			if (m_MyPAIVec[3].size() == 6 && m_MyPAIVec[4].size() == 6)
			{
				if (m_MyPAIVec[3][0] == 1 && m_MyPAIVec[3][1] == 2 && m_MyPAIVec[3][2] == 3 && m_MyPAIVec[3][3] == 7 && m_MyPAIVec[3][4] == 8 && m_MyPAIVec[3][5] == 9)
				{
					if (m_MyPAIVec[4][0] == 1 && m_MyPAIVec[4][1] == 2 && m_MyPAIVec[4][2] == 3 && m_MyPAIVec[4][3] == 7 && m_MyPAIVec[4][4] == 8 && m_MyPAIVec[4][5] == 9)
					{
						return true;
					}
				}
			}
		}
	}

	//五条为双龙 
	if (m_MyPAIVec[3].size() == 2)
	{
		//双龙 
		if (m_MyPAIVec[3][0] == 5 && m_MyPAIVec[3][1] == 5)
		{
			//老少副 
			if (m_MyPAIVec[2].size() == 6 && m_MyPAIVec[4].size() == 6)
			{
				if (m_MyPAIVec[2][0] == 1 && m_MyPAIVec[2][1] == 2 && m_MyPAIVec[2][2] == 3 && m_MyPAIVec[2][3] == 7 && m_MyPAIVec[2][4] == 8 && m_MyPAIVec[2][5] == 9)
				{
					if (m_MyPAIVec[4][0] == 1 && m_MyPAIVec[4][1] == 2 && m_MyPAIVec[4][2] == 3 && m_MyPAIVec[4][3] == 7 && m_MyPAIVec[4][4] == 8 && m_MyPAIVec[4][5] == 9)
					{
						return true;
					}
				}
			}
		}
	}

	//五饼为双龙 
	if (m_MyPAIVec[4].size() == 2)
	{
		//双龙 
		if (m_MyPAIVec[4][0] == 5 && m_MyPAIVec[4][1] == 5)
		{
			//老少副 
			if (m_MyPAIVec[2].size() == 6 && m_MyPAIVec[3].size() == 6)
			{
				if (m_MyPAIVec[2][0] == 1 && m_MyPAIVec[2][1] == 2 && m_MyPAIVec[2][2] == 3 && m_MyPAIVec[2][3] == 7 && m_MyPAIVec[2][4] == 8 && m_MyPAIVec[2][5] == 9)
				{
					if (m_MyPAIVec[3][0] == 1 && m_MyPAIVec[3][1] == 2 && m_MyPAIVec[3][2] == 3 && m_MyPAIVec[3][3] == 7 && m_MyPAIVec[3][4] == 8 && m_MyPAIVec[3][5] == 9)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

//检测是否一色三步高 
bool    CMJ::Check1S3BG_HU()
{
	return false;
}
//全带五 
bool    CMJ::CheckQD5_HU()
{
	//剑牌，风牌 
	if (m_MyPAIVec[0].empty() == false)return false;
	if (m_MyPAIVec[1].empty() == false)return false;
	if (m_ChiPAIVec[0].empty() == false)return false;
	if (m_ChiPAIVec[1].empty() == false)return false;
	if (m_PengPAIVec[0].empty() == false)return false;
	if (m_PengPAIVec[1].empty() == false)return false;
	if (m_GangPAIVec[0].empty() == false)return false;
	if (m_GangPAIVec[1].empty() == false)return false;
	return false;
}
//三同刻 
bool    CMJ::Check3TK_HU()
{

	return false;

}
//三暗刻 
bool    CMJ::Check3AK_HU()
{
	return false;
}
//单钓将 
bool    CMJ::CheckDDJ_HU()
{
	int count = 0;
	for (UINT i = 0; i < 6; i++)
	{
		count += m_MyPAIVec[i].size();
	}
	if (count == 2)
	{
		if (m_MyPAIVec[m_LastPAI.m_Type].size() == 2)
		{
			if (m_MyPAIVec[m_LastPAI.m_Type][0] == m_MyPAIVec[m_LastPAI.m_Type][1])return true;
		}
	}
	return false;
}
//检测是否听十三幺 
bool    CMJ::Check13Y_TING()
{
	bool        i13YSize[13];
	for (UINT i = 0; i < 13; i++)
	{
		i13YSize[i] = false;
	}
	//中发白 
	vector<int>::iterator Iter;
	for (Iter = m_MyPAIVec[0].begin(); Iter != m_MyPAIVec[0].end(); Iter++)
	{
		if ((*Iter) == 1)
		{
			i13YSize[0] = true;
		}
		if ((*Iter) == 2)
		{
			i13YSize[1] = true;
		}
		if ((*Iter) == 3)
		{
			i13YSize[2] = true;
		}
	}
	//东南西北风 
	for (Iter = m_MyPAIVec[1].begin(); Iter != m_MyPAIVec[1].end(); Iter++)
	{
		if ((*Iter) == 1)
		{
			i13YSize[3] = true;
		}
		if ((*Iter) == 2)
		{
			i13YSize[4] = true;
		}
		if ((*Iter) == 3)
		{
			i13YSize[5] = true;
		}
		if ((*Iter) == 4)
		{
			i13YSize[6] = true;
		}
	}
	//一九万 
	for (Iter = m_MyPAIVec[2].begin(); Iter != m_MyPAIVec[2].end(); Iter++)
	{
		if ((*Iter) == 1)
		{
			i13YSize[7] = true;
		}
		if ((*Iter) == 9)
		{
			i13YSize[8] = true;
		}
	}

	//一九条 
	for (Iter = m_MyPAIVec[3].begin(); Iter != m_MyPAIVec[3].end(); Iter++)
	{
		if ((*Iter) == 1)
		{
			i13YSize[9] = true;
		}
		if ((*Iter) == 9)
		{
			i13YSize[10] = true;
		}
	}

	//一九饼 
	for (Iter = m_MyPAIVec[4].begin(); Iter != m_MyPAIVec[4].end(); Iter++)
	{
		if ((*Iter) == 1)
		{
			i13YSize[11] = true;
		}
		if ((*Iter) == 9)
		{
			i13YSize[12] = true;
		}
	}
	int icount = 0;
	for (UINT i = 0; i < 13; i++)
	{
		if (i13YSize[i] == true)
		{
			icount++;
		}
	}
	if (icount >= 12)return true;
	return  false;
}
//检测是否听四暗刻 
bool    CMJ::Check4AK_TING()
{
	if (m_AKNum == 4)return true;
	return false;
}
//检测胡 
bool    CMJ::CheckHU()
{
	bool t_Ok = false;
	int iJiangNum = 0;

	int iSize = m_MyPAIVec[0].size();
	if (iSize>0)
	{
		//中发白 
		if (iSize == 2)
		{
			if (!CheckAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 3)
		{
			if (!CheckAAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1], m_MyPAIVec[0][2]))
			{
				return false;
			}
		}
		else if (iSize == 5)
		{
			if (CheckAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1]) && CheckAAAPai(m_MyPAIVec[0][2], m_MyPAIVec[0][3], m_MyPAIVec[0][4]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1], m_MyPAIVec[0][2]) && CheckAAPai(m_MyPAIVec[0][3], m_MyPAIVec[0][4]))
			{
				iJiangNum++;
			}
			else
			{
				return false;
			}
		}
		else if (iSize == 8)
		{
			if (CheckAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1]) && CheckAAAPai(m_MyPAIVec[0][2], m_MyPAIVec[0][3], m_MyPAIVec[0][4]) && CheckAAAPai(m_MyPAIVec[0][5], m_MyPAIVec[0][6], m_MyPAIVec[0][7]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1], m_MyPAIVec[0][2]) && CheckAAPai(m_MyPAIVec[0][3], m_MyPAIVec[0][4]) && CheckAAAPai(m_MyPAIVec[0][5], m_MyPAIVec[0][6], m_MyPAIVec[0][7]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1], m_MyPAIVec[0][2]) && CheckAAAPai(m_MyPAIVec[0][3], m_MyPAIVec[0][4], m_MyPAIVec[0][5]) && CheckAAPai(m_MyPAIVec[0][6], m_MyPAIVec[0][7]))
			{
				iJiangNum++;
			}
			else
			{
				return false;
			}
		}
		else if (iSize == 11)
		{
			if (CheckAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1]) && CheckAAAPai(m_MyPAIVec[0][2], m_MyPAIVec[0][3], m_MyPAIVec[0][4]) && CheckAAAPai(m_MyPAIVec[0][5], m_MyPAIVec[0][6], m_MyPAIVec[0][7]) && CheckAAAPai(m_MyPAIVec[0][8], m_MyPAIVec[0][9], m_MyPAIVec[0][10]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1], m_MyPAIVec[0][2]) && CheckAAPai(m_MyPAIVec[0][3], m_MyPAIVec[0][4]) && CheckAAAPai(m_MyPAIVec[0][5], m_MyPAIVec[0][6], m_MyPAIVec[0][7]) && CheckAAAPai(m_MyPAIVec[0][8], m_MyPAIVec[0][9], m_MyPAIVec[0][10]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1], m_MyPAIVec[0][2]) && CheckAAAPai(m_MyPAIVec[0][3], m_MyPAIVec[0][4], m_MyPAIVec[0][5]) && CheckAAPai(m_MyPAIVec[0][6], m_MyPAIVec[0][7]) && CheckAAAPai(m_MyPAIVec[0][8], m_MyPAIVec[0][9], m_MyPAIVec[0][10]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[0][0], m_MyPAIVec[0][1], m_MyPAIVec[0][2]) && CheckAAAPai(m_MyPAIVec[0][3], m_MyPAIVec[0][4], m_MyPAIVec[0][5]) && CheckAAAPai(m_MyPAIVec[0][6], m_MyPAIVec[0][7], m_MyPAIVec[0][8]) && CheckAAPai(m_MyPAIVec[0][9], m_MyPAIVec[0][10]))
			{
				iJiangNum++;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

	}
	//东南西北 
	iSize = m_MyPAIVec[1].size();
	if (iSize>0)
	{
		if (iSize == 2)
		{
			if (!CheckAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 3)
		{
			if (!CheckAAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1], m_MyPAIVec[1][2]))
			{
				return false;
			}
		}
		else if (iSize == 5)
		{
			if (CheckAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1]) && CheckAAAPai(m_MyPAIVec[1][2], m_MyPAIVec[1][3], m_MyPAIVec[1][4]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1], m_MyPAIVec[1][2]) && CheckAAPai(m_MyPAIVec[1][3], m_MyPAIVec[1][4]))
			{
				iJiangNum++;
			}
			else
			{
				return false;
			}
		}
		else if (iSize == 8)
		{
			if (CheckAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1]) && CheckAAAPai(m_MyPAIVec[1][2], m_MyPAIVec[1][3], m_MyPAIVec[1][4]) && CheckAAAPai(m_MyPAIVec[1][5], m_MyPAIVec[1][6], m_MyPAIVec[1][7]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1], m_MyPAIVec[1][2]) && CheckAAPai(m_MyPAIVec[1][3], m_MyPAIVec[1][4]) && CheckAAAPai(m_MyPAIVec[1][5], m_MyPAIVec[1][6], m_MyPAIVec[1][7]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1], m_MyPAIVec[1][2]) && CheckAAAPai(m_MyPAIVec[1][3], m_MyPAIVec[1][4], m_MyPAIVec[1][5]) && CheckAAPai(m_MyPAIVec[1][6], m_MyPAIVec[1][7]))
			{
				iJiangNum++;
			}
			else
			{
				return false;
			}
		}
		else if (iSize == 11)
		{
			if (CheckAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1]) && CheckAAAPai(m_MyPAIVec[1][2], m_MyPAIVec[1][3], m_MyPAIVec[1][4]) && CheckAAAPai(m_MyPAIVec[1][5], m_MyPAIVec[1][6], m_MyPAIVec[1][7]) && CheckAAAPai(m_MyPAIVec[1][8], m_MyPAIVec[1][9], m_MyPAIVec[1][10]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1], m_MyPAIVec[1][2]) && CheckAAPai(m_MyPAIVec[1][3], m_MyPAIVec[1][4]) && CheckAAAPai(m_MyPAIVec[1][5], m_MyPAIVec[1][6], m_MyPAIVec[1][7]) && CheckAAAPai(m_MyPAIVec[1][8], m_MyPAIVec[1][9], m_MyPAIVec[1][10]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1], m_MyPAIVec[1][2]) && CheckAAAPai(m_MyPAIVec[1][3], m_MyPAIVec[1][4], m_MyPAIVec[1][5]) && CheckAAPai(m_MyPAIVec[1][6], m_MyPAIVec[1][7]) && CheckAAAPai(m_MyPAIVec[1][8], m_MyPAIVec[1][9], m_MyPAIVec[1][10]))
			{
				iJiangNum++;
			}
			else if (CheckAAAPai(m_MyPAIVec[1][0], m_MyPAIVec[1][1], m_MyPAIVec[1][2]) && CheckAAAPai(m_MyPAIVec[1][3], m_MyPAIVec[1][4], m_MyPAIVec[1][5]) && CheckAAAPai(m_MyPAIVec[1][6], m_MyPAIVec[1][7], m_MyPAIVec[1][8]) && CheckAAPai(m_MyPAIVec[1][9], m_MyPAIVec[1][10]))
			{
				iJiangNum++;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	//万  
	iSize = m_MyPAIVec[2].size();
	if (iSize>0)
	{
		if (iSize == 2)
		{
			if (!CheckAAPai(m_MyPAIVec[2][0], m_MyPAIVec[2][1]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 3)
		{
			if (!CheckAAAPai(m_MyPAIVec[2][0], m_MyPAIVec[2][1], m_MyPAIVec[2][2]))
			{
				if (!CheckABCPai(m_MyPAIVec[2][0], m_MyPAIVec[2][1], m_MyPAIVec[2][2]))
				{
					return false;
				}
			}
		}
		else if (iSize == 5)
		{
			if (!Check5Pai(m_MyPAIVec[2][0], m_MyPAIVec[2][1], m_MyPAIVec[2][2], m_MyPAIVec[2][3], m_MyPAIVec[2][4]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 6)
		{
			if (!Check6Pai(m_MyPAIVec[2][0], m_MyPAIVec[2][1], m_MyPAIVec[2][2], m_MyPAIVec[2][3], m_MyPAIVec[2][4], m_MyPAIVec[2][5]))
			{
				return false;
			}
		}
		else if (iSize == 8)
		{
			if (!Check8Pai(m_MyPAIVec[2][0], m_MyPAIVec[2][1], m_MyPAIVec[2][2], m_MyPAIVec[2][3], m_MyPAIVec[2][4], m_MyPAIVec[2][5], m_MyPAIVec[2][6], m_MyPAIVec[2][7]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 9)
		{
			if (!Check9Pai(m_MyPAIVec[2][0], m_MyPAIVec[2][1], m_MyPAIVec[2][2], m_MyPAIVec[2][3], m_MyPAIVec[2][4], m_MyPAIVec[2][5], m_MyPAIVec[2][6], m_MyPAIVec[2][7], m_MyPAIVec[2][8]))
			{
				return false;
			}
		}
		else if (iSize == 11)
		{
			if (!Check11Pai(m_MyPAIVec[2][0], m_MyPAIVec[2][1], m_MyPAIVec[2][2], m_MyPAIVec[2][3], m_MyPAIVec[2][4], m_MyPAIVec[2][5], m_MyPAIVec[2][6], m_MyPAIVec[2][7], m_MyPAIVec[2][8], m_MyPAIVec[2][9], m_MyPAIVec[2][10]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 12)
		{
			if (!Check12Pai(m_MyPAIVec[2][0], m_MyPAIVec[2][1], m_MyPAIVec[2][2], m_MyPAIVec[2][3], m_MyPAIVec[2][4], m_MyPAIVec[2][5], m_MyPAIVec[2][6], m_MyPAIVec[2][7], m_MyPAIVec[2][8], m_MyPAIVec[2][9], m_MyPAIVec[2][10], m_MyPAIVec[2][11]))
			{
				return false;
			}
		}
		else if (iSize == 14)
		{
			if (!Check14Pai(m_MyPAIVec[2][0], m_MyPAIVec[2][1], m_MyPAIVec[2][2], m_MyPAIVec[2][3], m_MyPAIVec[2][4], m_MyPAIVec[2][5], m_MyPAIVec[2][6], m_MyPAIVec[2][7], m_MyPAIVec[2][8], m_MyPAIVec[2][9], m_MyPAIVec[2][10], m_MyPAIVec[2][11], m_MyPAIVec[2][12], m_MyPAIVec[2][13]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else
		{
			return false;
		}
	}

	//条  
	iSize = m_MyPAIVec[3].size();
	if (iSize>0)
	{
		if (iSize == 2)
		{
			if (!CheckAAPai(m_MyPAIVec[3][0], m_MyPAIVec[3][1]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 3)
		{
			if (!CheckAAAPai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2]))
			{
				if (!CheckABCPai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2]))
				{
					return false;
				}
			}
		}
		else if (iSize == 5)
		{
			if (!Check5Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 6)
		{
			if (!Check6Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4], m_MyPAIVec[3][5]))
			{
				return false;
			}
		}
		else if (iSize == 8)
		{
			if (!Check8Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4], m_MyPAIVec[3][5], m_MyPAIVec[3][6], m_MyPAIVec[3][7]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 9)
		{
			if (!Check9Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4], m_MyPAIVec[3][5], m_MyPAIVec[3][6], m_MyPAIVec[3][7], m_MyPAIVec[3][8]))
			{
				return false;
			}
		}
		else if (iSize == 11)
		{
			if (!Check11Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4], m_MyPAIVec[3][5], m_MyPAIVec[3][6], m_MyPAIVec[3][7], m_MyPAIVec[3][8], m_MyPAIVec[3][9], m_MyPAIVec[3][10]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 12)
		{
			if (!Check12Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4], m_MyPAIVec[3][5], m_MyPAIVec[3][6], m_MyPAIVec[3][7], m_MyPAIVec[3][8], m_MyPAIVec[3][9], m_MyPAIVec[3][10], m_MyPAIVec[3][11]))
			{
				return false;
			}
		}
		else if (iSize == 14)
		{
			if (!Check14Pai(m_MyPAIVec[3][0], m_MyPAIVec[3][1], m_MyPAIVec[3][2], m_MyPAIVec[3][3], m_MyPAIVec[3][4], m_MyPAIVec[3][5], m_MyPAIVec[3][6], m_MyPAIVec[3][7], m_MyPAIVec[3][8], m_MyPAIVec[3][9], m_MyPAIVec[3][10], m_MyPAIVec[3][11], m_MyPAIVec[3][12], m_MyPAIVec[3][13]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else
		{
			return false;
		}

	}

	//饼  
	iSize = m_MyPAIVec[4].size();
	if (iSize>0)
	{
		if (iSize == 2)
		{
			if (!CheckAAPai(m_MyPAIVec[4][0], m_MyPAIVec[4][1]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 3)
		{
			if (!CheckAAAPai(m_MyPAIVec[4][0], m_MyPAIVec[4][1], m_MyPAIVec[4][2]))
			{
				if (!CheckABCPai(m_MyPAIVec[4][0], m_MyPAIVec[4][1], m_MyPAIVec[4][2]))
				{
					return false;
				}
			}
		}
		else if (iSize == 5)
		{
			if (!Check5Pai(m_MyPAIVec[4][0], m_MyPAIVec[4][1], m_MyPAIVec[4][2], m_MyPAIVec[4][3], m_MyPAIVec[4][4]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 6)
		{
			if (!Check6Pai(m_MyPAIVec[4][0], m_MyPAIVec[4][1], m_MyPAIVec[4][2], m_MyPAIVec[4][3], m_MyPAIVec[4][4], m_MyPAIVec[4][5]))
			{
				return false;
			}
		}
		else if (iSize == 8)
		{
			if (!Check8Pai(m_MyPAIVec[4][0], m_MyPAIVec[4][1], m_MyPAIVec[4][2], m_MyPAIVec[4][3], m_MyPAIVec[4][4], m_MyPAIVec[4][5], m_MyPAIVec[4][6], m_MyPAIVec[4][7]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 9)
		{
			if (!Check9Pai(m_MyPAIVec[4][0], m_MyPAIVec[4][1], m_MyPAIVec[4][2], m_MyPAIVec[4][3], m_MyPAIVec[4][4], m_MyPAIVec[4][5], m_MyPAIVec[4][6], m_MyPAIVec[4][7], m_MyPAIVec[4][8]))
			{
				return false;
			}
		}
		else if (iSize == 11)
		{
			if (!Check11Pai(m_MyPAIVec[4][0], m_MyPAIVec[4][1], m_MyPAIVec[4][2], m_MyPAIVec[4][3], m_MyPAIVec[4][4], m_MyPAIVec[4][5], m_MyPAIVec[4][6], m_MyPAIVec[4][7], m_MyPAIVec[4][8], m_MyPAIVec[4][9], m_MyPAIVec[4][10]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else if (iSize == 12)
		{
			if (!Check12Pai(m_MyPAIVec[4][0], m_MyPAIVec[4][1], m_MyPAIVec[4][2], m_MyPAIVec[4][3], m_MyPAIVec[4][4], m_MyPAIVec[4][5], m_MyPAIVec[4][6], m_MyPAIVec[4][7], m_MyPAIVec[4][8], m_MyPAIVec[4][9], m_MyPAIVec[4][10], m_MyPAIVec[4][11]))
			{
				return false;
			}
		}
		else if (iSize == 14)
		{
			if (!Check14Pai(m_MyPAIVec[4][0], m_MyPAIVec[4][1], m_MyPAIVec[4][2], m_MyPAIVec[4][3], m_MyPAIVec[4][4], m_MyPAIVec[4][5], m_MyPAIVec[4][6], m_MyPAIVec[4][7], m_MyPAIVec[4][8], m_MyPAIVec[4][9], m_MyPAIVec[4][10], m_MyPAIVec[4][11], m_MyPAIVec[4][12], m_MyPAIVec[4][13]))
			{
				return false;
			}
			else
			{
				iJiangNum++;
			}
		}
		else
		{
			return false;
		}
	}

	if (iJiangNum == 1)return true;
	return false;
}
//检测听  
bool    CMJ::CheckTING()
{
	//剑牌  
	for (UINT j = 0; j < 9; j++)
	{
		//起牌  
		AddPai(0, j + 1);
		if (CheckAllPai(MJPAI_GETPAI))
		{
			int iPaiIndex = GetPaiIndex(0, j + 1);
			DelPai(iPaiIndex);
			return true;
		}
		else
		{
			int iPaiIndex = GetPaiIndex(0, j + 1);
			DelPai(iPaiIndex);
		}
	}
	//风牌  
	for (UINT j = 0; j < 9; j++)
	{
		//起牌  
		AddPai(1, j + 1);
		if (CheckAllPai(MJPAI_GETPAI))
		{
			int iPaiIndex = GetPaiIndex(1, j + 1);
			DelPai(iPaiIndex);
			return true;
		}
		else
		{
			int iPaiIndex = GetPaiIndex(1, j + 1);
			DelPai(iPaiIndex);
		}
	}
	for (UINT i = 2; i < 5; i++)
	{
		for (UINT j = 0; j < 9; j++)
		{
			//起牌  
			AddPai(i, j + 1);
			if (CheckAllPai(MJPAI_GETPAI))
			{
				int iPaiIndex = GetPaiIndex(i, j + 1);
				DelPai(iPaiIndex);
				return true;
			}
			else
			{
				int iPaiIndex = GetPaiIndex(i, j + 1);
				DelPai(iPaiIndex);
			}
		}
	}
	return false;
}

