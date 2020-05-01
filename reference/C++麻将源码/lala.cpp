 
#include <tchar.h>
#include <windows.h>
#include "CMJ.h"  
#include "CMJManage.h" 
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	//其它三个玩家  
	CMJ             t_OtherPlayer[3];
	//我  
	CMJ             t_MyPlayer;
	//洗牌器  
	CMJManage       t_MJManage;
	//分数  
	int             t_Score = 0;
GameStart:

	//初始化及洗牌  
	t_MJManage.InitPai();//初始化  
	t_MyPlayer.CleanUp();
	for (UINT i = 0; i < 3; i++)
	{
		t_OtherPlayer[i].CleanUp();
	}
	cout << "洗牌完成" << endl;
	cout << "起牌:========================================================" << endl;
	for (UINT i = 0; i < 13; i++)
	{

		stPAIEx t_Pai = t_MJManage.GetAPai();
		t_MyPlayer.AddPai(t_Pai.m_NewPai.m_Type, t_Pai.m_NewPai.m_Value);

		for (UINT j = 0; j < 3; j++)
		{
			stPAIEx t_Pai2 = t_MJManage.GetAPai();
			t_OtherPlayer[j].AddPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
		}
	}
	t_MyPlayer.Init();
	for (UINT j = 0; j < 3; j++)
	{
		t_OtherPlayer[j].Init();
	}
	//打牌循环  
	bool t_Finish = false;
	bool t_Ting = false;
	while (t_Finish == false)
	{

		t_MyPlayer.PrintAllPai();
		cout << endl;
		cout << "起牌:========================================================" << endl;
		stPAIEx t_Pai = t_MJManage.GetAPai();

		//刷新我方牌墙  
		t_MyPlayer.PrintPai(t_Pai.m_NewPai.m_Type, t_Pai.m_NewPai.m_Value);
		cout << endl;
		//如果没有听头  
		if (t_Ting == false)
		{
			cout << "要还是打？Y/N";
			char t_Result;
			cin >> t_Result;
			if (t_Result == 'Y' || t_Result == 'y')
			{
				//起牌  
				t_MyPlayer.AddPai(t_Pai.m_NewPai.m_Type, t_Pai.m_NewPai.m_Value);
				//起牌后胡牌判断  
				t_Finish = t_MyPlayer.CheckAllPai(MJPAI_GETPAI);
				if (t_Finish)
				{
					printf("胡啦!!!:%s-%d", t_MyPlayer.GetInfo()->m_GoodName, t_MyPlayer.GetInfo()->m_GoodValue);
					::Sleep(1000);
				}
				else
				{
					if (t_Pai.m_NewPai.m_Type == -1)//如果起牌数已达到上限  
					{
						cout << endl;
						cout << "黄庄了！！！！！！！！！！！！！" << endl;
						break;
					}

					t_MyPlayer.PrintAllPai();
					cout << endl;
				OUTPai:
					cout << "请打牌（输入牌序号）";
					int PaiIndex;
					cin >> PaiIndex;
					if (t_MyPlayer.DelPai(PaiIndex) == false)
					{
						cout << "没有此牌" << endl;
						goto OUTPai;
					}
					//==============================牌面刷新================================================  
					cout << "牌面刷新============================" << endl;
					t_MyPlayer.PrintAllPai();
					cout << endl;
					//==============================================================================  

					//======================包听========================================================  
					if (t_MyPlayer.CheckAllPai(MJPAI_PUTPAI))
					{

						char t_BTing;
						cout << "要包听吗？:(Y/N)";
						cin >> t_BTing;
						if (t_BTing == 'y' || t_BTing == 'Y')
						{
							t_Ting = true;
						}
					}
					//==============================================================================  

				}
			}
			else
			{
				//======================包听========================================================  
				if (t_MyPlayer.CheckAllPai(MJPAI_PUTPAI))
				{

					char t_BTing;
					cout << "要包听吗？:(Y/N)";
					cin >> t_BTing;
					if (t_BTing == 'y' || t_BTing == 'Y')
					{
						t_Ting = true;
					}
				}
				//==============================================================================  
			}
		}
		else
		{

			t_MyPlayer.AddPai(t_Pai.m_NewPai.m_Type, t_Pai.m_NewPai.m_Value);
			//起牌  
			int iPaiIndex = t_MyPlayer.GetPaiIndex(t_Pai.m_NewPai.m_Type, t_Pai.m_NewPai.m_Value);
			if (iPaiIndex >= 0)
			{
				//起牌后胡牌判断  
				t_Finish = t_MyPlayer.CheckAllPai(MJPAI_GETPAI);
				if (t_Finish)
				{
					cout << endl;
					printf("胡啦!!!:%s-合计%d番", t_MyPlayer.GetInfo()->m_GoodName, t_MyPlayer.GetInfo()->m_GoodValue);
					t_Score += t_MyPlayer.GetInfo()->m_GoodValue;
					::Sleep(1000);
					break;
				}
				else
				{
					t_MyPlayer.DelPai(iPaiIndex);
					cout << "打牌";
					t_MyPlayer.PrintPai(t_Pai.m_NewPai.m_Type, t_Pai.m_NewPai.m_Value);
					cout << endl;
					::Sleep(1000);
				}
			}
			else
			{
				cout << "程序出错!" << endl;
			}

		}
		cout << endl;
		//其它玩家起牌出牌  
		for (UINT j = 0; j < 3; j++)
		{
			stPAIEx t_Pai2 = t_MJManage.GetAPai();
			if (j == 0)
			{
				cout << "南家起牌出牌：";
				t_MyPlayer.PrintPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
				cout << endl;
				::Sleep(1000);
			}
			if (j == 1)
			{
				cout << "西家起牌出牌：";
				t_MyPlayer.PrintPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
				cout << endl;
				::Sleep(1000);
			}
			if (j == 2)
			{
				cout << "北家起牌出牌：";
				t_MyPlayer.PrintPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
				cout << endl;
				::Sleep(1000);
			}

			char t_Result;
			if (t_Ting == false)
			{
				if (t_Pai2.m_IsHZ)//如果起牌数已达到上限  
				{
					cout << endl;
					cout << "黄庄了！！！！！！！！！！！！！" << endl;
					t_Finish = true;
					break;
				}

				bool t_Check = false;
				//检查吃牌  
				if (t_MyPlayer.CheckChiPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value))
				{
					if (t_Check == false)
					{
						cout << "请选择:";
					}
					cout << "(吃)";
					t_Check = true;
				}
				//检查碰牌  
				if (t_MyPlayer.CheckPengPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value))
				{
					if (t_Check == false)
					{
						cout << "请选择:";
					}
					cout << "(碰)";
					t_Check = true;
				}
				//检查杠牌  
				if (t_MyPlayer.CheckGangPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value))
				{
					if (t_Check == false)
					{
						cout << "请选择:";
					}
					cout << "(杠)";
					t_Check = true;
				}
				//起牌  
				t_MyPlayer.AddPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
				//起牌后胡牌判断  
				if (t_MyPlayer.CheckAllPai(MJPAI_GETPAI))
				{
					if (t_Check == false)
					{
						cout << "请选择:";
					}
					cout << "(胡)";
					t_Check = true;
				}
				int iPaiIndex = t_MyPlayer.GetPaiIndex(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
				t_MyPlayer.DelPai(iPaiIndex);
				//如果查到  
				if (t_Check)
				{
					cout << endl;
					cin >> t_Result;
				}
				else
				{
					//返回循环  
					continue;
				}
			}
			else
			{
				t_Result = '4';
			}
			//吃牌  
			if (t_Result == '1')
			{
				t_MyPlayer.PrintChiChosePai();

				int index = 0;
				//如果吃牌组合大于  
				if (t_MyPlayer.GetChiChoseNum()>1)
				{
					cout << "请输入组合号:" << endl;
					cin >> index;
				}
				t_MyPlayer.DoChiPai(index, t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);

				//==============================================================================  
				cout << "牌面刷新============================" << endl;
				t_MyPlayer.PrintAllPai();
				cout << endl;
				//==============================================================================  

			OUTPai2:
				cout << "请打牌（输入牌序号）";
				int PaiIndex;
				cin >> PaiIndex;
				if (t_MyPlayer.DelPai(PaiIndex) == false)
				{
					cout << "没有此牌" << endl;
					goto OUTPai2;
				}

				//=================================牌面刷新=============================================  
				cout << "牌面刷新============================" << endl;
				t_MyPlayer.PrintAllPai();
				cout << endl;
				//==============================================================================  
				//======================包听========================================================  
				if (t_MyPlayer.CheckAllPai(MJPAI_PUTPAI))
				{

					char t_BTing;
					cout << "要包听吗？:(Y/N)";
					cin >> t_BTing;
					if (t_BTing == 'y' || t_BTing == 'Y')
					{
						t_Ting = true;
					}
				}
				//==============================================================================  
				//该我下家  
				j = -1;

			}
			else if (t_Result == '2')//碰牌  
			{

				t_MyPlayer.PrintPengChosePai();
				t_MyPlayer.DoPengPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
				//==============================================================================  
				cout << "牌面刷新============================" << endl;
				t_MyPlayer.PrintAllPai();
				cout << endl;
				//==============================================================================  
			OUTPai3:
				cout << "请打牌（输入牌序号）";
				int PaiIndex;
				cin >> PaiIndex;
				if (t_MyPlayer.DelPai(PaiIndex) == false)
				{
					cout << "没有此牌" << endl;
					goto OUTPai3;
				}
				//==========================牌面刷新====================================================  
				cout << "牌面刷新============================" << endl;
				t_MyPlayer.PrintAllPai();
				cout << endl;
				//==============================================================================  
				//======================包听========================================================  
				if (t_MyPlayer.CheckAllPai(MJPAI_PUTPAI))
				{

					char t_BTing;
					cout << "要包听吗？:(Y/N)";
					cin >> t_BTing;
					if (t_BTing == 'y' || t_BTing == 'Y')
					{
						t_Ting = true;
					}
				}
				//==============================================================================  
				j = -1;

			}
			else if (t_Result == '3')//杠牌  
			{

				t_MyPlayer.PrintGangChosePai();
				t_MyPlayer.DoGangPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
				cout << "起杠底牌" << endl;
				t_MyPlayer.AddPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
				//==============================================================================  
				cout << "牌面刷新============================" << endl;
				t_MyPlayer.PrintAllPai();
				cout << endl;
				//==============================================================================  
				stPAIEx t_Pai2 = t_MJManage.GetAPai();

				//起牌后胡牌判断  
				t_Finish = t_MyPlayer.CheckAllPai(MJPAI_GETPAI);
				if (t_Finish)
				{
					cout << "杠底花吗？(Y/N)" << endl;
					char t_Result;
					cin >> t_Result;
					if (t_Result == 'Y' || t_Result == 'y')
					{
						cout << endl;
						printf("胡啦!!!:%s-%d", t_MyPlayer.GetInfo()->m_GoodName, t_MyPlayer.GetInfo()->m_GoodValue);
						t_Score += t_MyPlayer.GetInfo()->m_GoodValue;
						::Sleep(1000);
						break;
					}
				}

				if (t_Pai2.m_IsHZ)//如果起牌数已达到上限  
				{
					cout << "黄庄了！！！！！！！！！！！！！" << endl;
					t_Finish = true;
					break;
				}

			OUTPai4:
				cout << "请打牌（输入牌序号）";
				int PaiIndex;
				cin >> PaiIndex;
				if (t_MyPlayer.DelPai(PaiIndex) == false)
				{
					cout << "没有此牌" << endl;
					goto OUTPai4;
				}
				//===========================牌面刷新===================================================  
				cout << "牌面刷新============================" << endl;
				t_MyPlayer.PrintAllPai();
				cout << endl;
				//==============================================================================  
				//======================包听========================================================  
				if (t_MyPlayer.CheckAllPai(MJPAI_PUTPAI))
				{

					char t_BTing;
					cout << "要包听吗？:(Y/N)";
					cin >> t_BTing;
					if (t_BTing == 'y' || t_BTing == 'Y')
					{
						t_Ting = true;
					}
				}
				//==============================================================================  
				//该我下家  
				j = -1;

			}
			else if (t_Result == '4')//胡牌  
			{
				//起牌  
				t_MyPlayer.AddPai(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
				//起牌后胡牌判断  
				t_Finish = t_MyPlayer.CheckAllPai(MJPAI_GETPAI);
				if (t_Finish)
				{
					printf("胡啦!!!:%s-合计%d番", t_MyPlayer.GetInfo()->m_GoodName, t_MyPlayer.GetInfo()->m_GoodValue);
					t_Score += t_MyPlayer.GetInfo()->m_GoodValue;
					::Sleep(1000);
					break;
				}
				else
				{
					if (t_Pai2.m_IsHZ)//如果起牌数已达到上限  
					{
						cout << "黄庄了！！！！！！！！！！！！！" << endl;
						t_Finish = true;
						break;
					}
					//起牌  
					int iPaiIndex = t_MyPlayer.GetPaiIndex(t_Pai2.m_NewPai.m_Type, t_Pai2.m_NewPai.m_Value);
					cout << endl;
					t_MyPlayer.DelPai(iPaiIndex);
				}
			}

		}


	}
	cout << "我的分数：" << t_Score << endl;
	::Sleep(3000);
	goto GameStart;//重新开始一局  

	return 0;
}