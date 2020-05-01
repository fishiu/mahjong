#include "StdAfx.h"
#include "Method.h"

#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;


char * mp3StrName[]=
{
	"g_1Tiao.mp3",          //0
	"g_2Tiao.mp3",          //1
	"g_3Tiao.mp3",          //2
	"g_4Tiao.mp3",          //3
	"g_5Tiao.mp3",          //4
	"g_6Tiao.mp3",          //5
	"g_7Tiao.mp3",          //6
	"g_8Tiao.mp3",          //7
	"g_9Tiao.mp3",          //8
	"m_TuoGuan.mp3",        //过庄
	"g_1Wan.mp3",           //20
	"g_2Wan.mp3",           //21
	"g_3Wan.mp3",           //22
	"g_4Wan.mp3",           //23
	"g_5Wan.mp3",           //24
	"g_6Wan.mp3",           //25
	"g_7Wan.mp3",           //26
	"g_8Wan.mp3",           //27
	"g_9Wan.mp3",           //28
	"g_Hu_03.mp3",          //胡牌
	"g_1Bing.mp3",          //10
	"g_2Bing.mp3",          //11
	"g_3Bing.mp3",          //12
	"g_4Bing.mp3",          //13
	"g_5Bing.mp3",          //14
	"g_6Bing.mp3",          //15
	"g_7Bing.mp3",          //16
	"g_8Bing.mp3",          //17
	"g_9Bing.mp3",          //18
	"g_Peng_04.mp3",        //碰
	"g_ZiMo_02.mp3",        //自摸
	"g_Gang_03.mp3",        //杠
	"m_basewin.mp3",        //赢了
};
CMethod::CMethod(void)
{
	isChuPai =false;
	duizi=false;

}
void  CMethod::Shuffle(int * P)    //洗牌
{
	srand( (unsigned)time( NULL ) );

	for (int i=0;i<108;i++)
	{
		P[i]=i/36*10+i%9;
	}
	for (int i=0;i<108;i++)
	{
		swap(P[i],P[rand()%108]);
	}

}

//提牌
bool  CMethod::Move(vector<CARDS> & card,CPoint p)
{
	for (int i=0;i<card.size();i++)
	{
		 if (card[i].s_rcet.PtInRect(p)&&!card[i].isclick)
		 {
			 card[i].isclick =true;
			card[i].y-=15;
			isChuPai =true;
			 return true;
		 }else if(card[i].isclick&&!card[i].s_rcet.PtInRect(p))
		 {
			 card[i].isclick =false;
			 card[i].y+=15;
			// m_player.chupai =false;
		 }
	}
 return false;
}
//出牌
bool  CMethod::buttonDown(vector<CARDS> & card,int& index)//出牌
{
	
	if ( isChuPai)
	{
		for (int i=0;i<card.size();)
		{
			 if (card[i].isclick)
			 {
				 index = card[i].index;
				 card.erase(card.begin()+i);
				 for (int j=i;j<card.size();j++)
				 {
					 card[j].x-=45;
					 card[j].s_rcet.left-=45;
					 card[j].s_rcet.right-=45;
				 }
				 	return true;
			 }
			 i++;
		}
	}
	
		
	return false;
}



void CMethod::deleteCards(vector<CARDS> & card,PLAYER & send)
{
//排序
	for (int i=0;i<card.size();)
	{
		if (card[i].index==send.index)
		{
			card.erase(card.begin()+i);
			for (int j=i;j<card.size();j++)
			{
				switch(send.whoChupai)
				{
				case PLAYER_A:
					card[j].x-=45;
					card[j].s_rcet.left-=45;
					card[j].s_rcet.right-=45;
					break;
				case PLAYER_B:
					card[j].y-=20;
					break;
				case PLAYER_C:
					card[j].x-=24;
					break;
				case PLAYER_D:
					card[j].y-=22;
					break;
				}
				
			}
		}
		i++;
	}


	 
	
  
}
void deleteNum(vector<temp1>&vec,int index)
{


	for (vector<temp1>::iterator iter=vec.begin();iter!=vec.end();)
	{
		if (iter->index==index)
		{
			iter =vec.erase(iter);
			break;
		}else
			iter++;
	}

}



bool lianpai(vector<temp1>&pai,int num)
{
	for (int i=0;i<pai.size();i++)
	{
		if (pai[i].index==num)
		{
			return true;
		}
	}
	return false;

}
void  biaoji(vector<temp1>&pai,int num)
{
	for (int i=0;i<pai.size();i++)
	{
		if (pai[i].duizi&&pai[i].index==num)
		{
			pai[i].duizi =false;
			break;
		}
	}
}
bool peng(vector<temp1>&pai)
{
	for (int i=1;i<pai.size();i++)
	{
		if (pai[i].index==pai[i-1].index&&pai[i].duizi&&pai[i-1].duizi)
		{
			return false;
		}

	}
	return true;
}


bool CMethod::hupai(vector<temp1>&pai)//胡牌算法
{


	if (tempvec.size()%3!=2)
	{

		return false;
	}
	if (pai.empty())
	{
		return true;
	}
	else 
	{
		//对子
		if (!duizi)
		{
			for (int i=1;i<pai.size();i++)
			{
				if (pai[i].index==pai[i-1].index&&(pai[i].duizi&&pai[i-1].duizi))
				{
					biaoji(tempvec,pai[i-1].index);
					deleteNum(pai,pai[i-1].index);
					biaoji(tempvec,pai[i-1].index);
					deleteNum(pai,pai[i-1].index);
					duizi =true;
					break;
				}		 
			}
		}
		//3条
		for (int i=1;i<pai.size()-1;i++)
		{
			
			if (pai[i-1].index==pai[i].index&&pai[i].index==pai[i+1].index)
			{
				deleteNum(pai,pai[i-1].index);
				deleteNum(pai,pai[i-1].index);
				deleteNum(pai,pai[i-1].index);
				if ( hupai(pai))
				{
					return true;
				}
			}

		}
		//顺子  5 , 6 7 
		for (int i=0;i<pai.size();i++)
		{
			if (pai.size()>=3)
			{
				if (lianpai(pai,pai[i].index+1)&&lianpai(pai,pai[i].index+2))
				{
					int temp =pai[i].index+1;
					deleteNum(pai,pai[i].index);
					deleteNum(pai,temp);
					deleteNum(pai,temp+1);
					if ( hupai(pai))
					{
						return true;
					}
				}
			}
		}

		if (peng(tempvec))
		{
			return  false;
		}
		duizi =false;
		pai.clear();
		for (int i=0;i<tempvec.size();i++)
			pai.push_back(tempvec[i]);
		if (hupai(pai))
		{
			return true;
		}


	}

	return false;
}



bool shunzi(vector<CARDS>&pai,int num)
{
	if (pai.size()>=3)
	{
		for (int i=0;i<pai.size();i++)
		{
			if (pai[i].index==num&&!pai[i].isOut)
			{

				return true;
			}
		}
	}

	return false;

}


vector <int> vecDouble;
void getduizi(vector<CARDS> &cards)
{
	vecDouble.clear();
	int temp;
	for (int i=0;i<cards.size()-1;i++)
	{
		if ((cards[i].index==cards[i+1].index)&&(!cards[i].isOut&&!cards[i+1].isOut))
		{
			temp =cards[i].index;
			cards[i].isOut=true;
			cards[i+1].isOut=true;
			cards[i].value1 =PAI_DUIZI;
			cards[i+1].value1 =PAI_DUIZI;
			vecDouble.push_back(temp);
		}

	}


}
void deleteDNum(vector<CARDS>&vec,int index)
{
	for (int i=0;i<vec.size();i++)
	{
		if (vec[i].index==index&&!vec[i].isOut)
		{
			vec[i].isOut =true;
			vec[i].value1 =PAI_SHUNZI;
			break;
		}
	}

}
void deleteIndex(vector<CARDS>&vec,int index)
{
	for (int i=0;i<vec.size();i++)
	{
		if (vec[i].index==index)
		{
			vec[i].isOut =true;
			vec[i+1].isOut =true;
			vec[i].value1=PAI_DUIZI;
			vec[i+1].value1=PAI_DUIZI;
			break;
		}
	}

}

struct jisuan
{
	int sum;
	int min;
};

vector<jisuan> vecSum;
int  CMethod::ChaiPai(vector<CARDS> & pai)//拆牌算法（电脑自动出牌算法）
{
	getduizi(pai);
	vecSum.clear();

	for (int i=0;i<pai.size();i++)
	{
		pai[i].isOut =false;
		pai[i].value1=PAI_NULL;
	}

	for (int i=0;i<vecDouble.size()+2;i++)
	{
		if (i<vecDouble.size())
		{
			deleteIndex(pai,vecDouble[i]);


		}else if(i==vecDouble.size())  //打开所有是对子的牌
		{
			vecDouble.clear();
			getduizi(pai);

		}

		//3条
		if (pai.size()>=3)
		{
			for (int i=0;i<pai.size()-2;i++)
			{
				if ((pai[i].index==pai[i+1].index&&pai[i].index==pai[i+2].index)&&(!pai[i].isOut&&!pai[i+1].isOut&&!pai[i+2].isOut))
				{
					pai[i].isOut=true;
					pai[i+1].isOut=true;
					pai[i+2].isOut=true;
					pai[i].value1 =PAI_SANTIAO;
					pai[i+1].value1 =PAI_SANTIAO;
					pai[i+2].value1 =PAI_SANTIAO;
				}

			}
		}
		//找出牌中的顺子
		for (int i=0;i<pai.size();i++)
		{
			if (!pai[i].isOut)
		 {
			 if (shunzi(pai,pai[i].index+1)&&shunzi(pai,pai[i].index+2))
			 {
				 deleteDNum(pai,pai[i].index);
				 deleteDNum(pai,pai[i].index+1);
				 deleteDNum(pai,pai[i].index+2);

			 }

		 }
		}
		//找出对子
		vecDouble.clear();
		getduizi(pai);

		//搭牌1
		if (pai.size()>=2)
		{
			for (int i=0;i<pai.size()-1;i++)
			{
				if (!pai[i].isOut&&!pai[i+1].isOut)
				{
					if (pai[i].index+2==pai[i+1].index)
					{
						pai[i].isOut=true;
						pai[i].value1=PAI_DAPAI_1;
						pai[i+1].value1=PAI_DAPAI_1;

					}
					if (pai[i].index+1==pai[i+1].index)
					{
						pai[i].isOut=true;
						pai[i+1].isOut=true;
						pai[i].value1=PAI_DAPAI_2;
						pai[i+1].value1=PAI_DAPAI_2;
					}
				}
			}
		}
    for (int i=0;i<pai.size();i++)
    {
		for (int j=0;j<pai.size();j++)
		{
			if (!pai[i].isOut&&pai[i].value1==PAI_NULL)
			{
				if (pai[i].index+1==pai[j].index||pai[i].index-1==pai[j].index)
				{
					 pai[i].value1 =PAI_DANPAI_1;
					 pai[i].isOut =true;
				}
			}
		}
    }
		//单牌
		for(int i=0;i<pai.size();i++)
		{
			if (!pai[i].isOut&&pai[i].value1==PAI_NULL)
			{
				pai[i].value1=PAI_DANGPAI;
				pai[i].isOut =true;

			}
		}


		int sum=0;
	    int minValue=0;
		jisuan temp;
		for (int i=0;i<pai.size();i++)
		{
			sum+=pai[i].value1;
			if (pai[minValue].value1<=pai[i].value1)
			{
				temp.min =pai[minValue].index;
			}else 
			{
				temp.min = pai[i].index;
				minValue=i;
			}
		}
		temp.sum =sum;
		vecSum.push_back(temp);


		for (int i=0;i<pai.size();i++)
		{
			pai[i].isOut =false;
			pai[i].value1=PAI_NULL;
		}
		getduizi(pai);
		for (int i=0;i<pai.size();i++)
		{
			pai[i].isOut =false;
			pai[i].value1=PAI_NULL;
		}

	}

	int minValue=0;
    int maxValue=0;
	for (int i=0;i<vecSum.size();i++)
	{
		if (vecSum[maxValue].sum>=vecSum[i].sum)
		{
			minValue = vecSum[maxValue].min;

		}else
		{
		  minValue = vecSum[i].min;
		  maxValue =i;
		}
	}
	

	 return minValue;
}



bool CMethod::pengPai(vector<CARDS> & card,PLAYER & send)//碰牌
{
	for(int i=0;i<card.size();i++)
	{
		card[i].isOut =false;
	}
    getduizi(card);
	
	
			for (int i=0;i<vecDouble.size();i++)
			{
				if (vecDouble[i]==send.index)
			 {
				 return true;
			 }
			}
		

	
	
	return false;

}
bool CMethod::gangPai(vector<CARDS> &card,PLAYER & send)//杠牌
{
	int sum=0;
	for (int i=0;i<card.size();i++)
	{
		 if (card[i].index==send.index)
		 {
			  sum++;
		 }
	}
	if (sum==3)
	{
		 return true;
	}
	return false;
}

bool CMethod::anGangPai(vector<CARDS> &card,int  index)//暗杠牌
{
      if (card.size()>=4)
     {
		 for (int i=0;i<card.size()-3;i++)
		 {
			 if (card[i].index==index&&index==card[i+1].index&&index==card[i+2].index)
			 {
				 return true;

			 }
		 }
     }
	return false;
}
int getIndex(vector<CARDS> & pi, PLAYER player)
{
	int num=0;
	for (int i=0;i<pi.size();i++)
	{
		 if (pi[i].whoCard==player.whoChupai&&pi[i].cardTp==player.cardTp)
		 {
			  num++;
		 }
	}
	return num;
}
void CMethod::sendCard(vector<CARDS> & pi,PLAYER & send)//出牌
{
	CARDS tempC;
	int index=getIndex(pi,send);
	 tempC.cardTp =send.cardTp;
	 if (tempC.cardTp==1)
	 {
		
		 if (m_mp3.IsOpened())
		 {
			
				 m_mp3.Close();
				 m_mp3.Open(mp3StrName[send.index]);
				 m_mp3.Play();
		 }else
		 {
			 m_mp3.Open(mp3StrName[send.index]);
			 m_mp3.Play();

		 }
		
	 }
	switch (send.whoChupai)
	{
	 
	case PLAYER_A:
		if (send.cardTp==1)
		{
			tempC.x=350+(index%9)*24;
			tempC.y=320-index/9*36;
			tempC.cx=(send.index/10*9+send.index%10)*24;
			tempC.cy=36;
		}else if(send.cardTp==2)
		{
			tempC.x=10+(index%3)*24;
			tempC.y=520-index/3*36;
			tempC.cx=(send.index/10*9+send.index%10)*24;
			tempC.cy=36;

		}
		
		
		tempC.wide=24;
		tempC.high=36;
		tempC.whoCard=PLAYER_A;
		break;
	case PLAYER_B:
		if (send.cardTp==1)
		{
			tempC.x=650-index/9*30;
			tempC.y=150+(index%9)*22;
		}else if(send.cardTp ==2)
		{
			tempC.x=830-index/3*30;
			tempC.y=50+(index%3)*22;

		}
	
		tempC.wide=30;
		tempC.high=27;
		
		tempC.cx=(send.index/10*9+send.index%10)*30;
		tempC.cy=27;
		tempC.whoCard=PLAYER_B;
		break;
	case PLAYER_C:
		if (send.cardTp==1)
		{
			tempC.x=350+(index%9)*24;
			tempC.y=150+index/9*36;
		}else if(send.cardTp==2)
		{
			tempC.x=520+(index%3)*24;
			tempC.y=10+index/3*36;
		}

	
		tempC.cx=(send.index/10*9+send.index%10)*24;
		tempC.cy=0;
		tempC.wide=24;
		tempC.high=36;
		tempC.whoCard=PLAYER_C;
       
		break;
	case PLAYER_D:
		if (send.cardTp==1)
		{
			tempC.x=250+index/9*30;
			tempC.y=150+(index%9)*22;
		}else if(send.cardTp==2)
		{
			tempC.x=20+index/3*30;
			tempC.y=60+(index%3)*22;

		}

		
		tempC.wide=30;
		tempC.high=27;

		tempC.cx=(send.index/10*9+send.index%10)*30;
		tempC.cy=0;
		tempC.whoCard=PLAYER_D;
		break;
	}
	pi.push_back(tempC);

}
CMethod::~CMethod(void)
{

}
