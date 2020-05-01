#include "StdAfx.h"
#include "Cards.h"
#include <algorithm>
#include <functional> 
using namespace std;

CCards::CCards(void)
{
	intarray_arrow[0]=0;
	intarray_arrow[1]=35;
	intarray_arrow[2]=35;
	intarray_arrow[3]=68;	
	intarray_arrow[4]=110;
	intarray_arrow[5]=50;
	intarray_arrow[6]=160;
	intarray_arrow[7]=64;

	 m_isMessage =false;
	 m_isHuPai =false;
	 m_computerGang =false;
	 m_meGang  =false;

	 m_gameState =GAME_START;

	
}
CARDS   temp;
CRect g_peng(680,425,720,465);
CRect g_guo(750,425,790,465);
void CCards::initCards()//初始话108张牌
{
	int intarray[108];
	m_metCards.Shuffle(intarray);
	
	for (int i=0;i<108;i++)
	{
		temp.huase=intarray[i]/10;
		
		temp.cy=0;
		temp.index =intarray[i];

       if (i<14)//A
       {
		 //  temp.cx=(temp.huase*9+intarray[i]%10)*45;
		   temp.y=485;
		   temp.whoCard =PLAYER_A;
		   m_playA.push_back(temp);

	   }else if(i<27)//B
	   {
		   temp.cy=27;
		   temp.cx=(temp.huase*9+intarray[i]%10)*30;
		   temp.x=760;  
		   temp.whoCard =PLAYER_B;
		    m_playB.push_back(temp);

	   }else if(i<40)//C
	   {
		   temp.cx=(temp.huase*9+intarray[i]%10)*24;
		   temp.y=30;
		   temp.whoCard =PLAYER_C;
		    m_playC.push_back(temp);
	   }else if(i<53)//D
	   {
		   temp.cx=(temp.huase*9+intarray[i]%10)*30;
		   temp.x=120;
		   temp.whoCard =PLAYER_D;
		    m_playD.push_back(temp);
	   }else
	   {
		   if (m_vecCards.size()<17)
		   {
			   temp.cx =32/*(temp.huase*9+intarray[i]%10)*24*/;
			   temp.cy =0;
			   temp.wide=24;
			   temp.high =36;
			   if (!m_vecCards.empty())
			   {
				   temp.x=480+(m_vecCards.size()/2+m_vecCards.size()%2)*24;
				   if (m_vecCards.size()%2)temp.y=88;
				   else   temp.y =80;
			   }else 
			   {
				   temp.y =88;
				   temp.x=480;
			   }
		   }else if(m_vecCards.size()<17+28 )
		   {
			   temp.cx =0;
			   temp.cy =0;
			   temp.wide=30;
			   temp.high =27;
			   if (m_vecCards.size()%2) temp.y=((m_vecCards.size())/2+(m_vecCards.size())%2)*20-50;
			   else temp.y=((m_vecCards.size())/2+(m_vecCards.size())%2)*20-6-50;
			      temp.x =700;
		   }else
		   {

			   temp.cx =32;
			   temp.cy =0;
			   temp.wide=24;
			   temp.high =36;
			 
				   temp.x=650-((m_vecCards.size()-44)/2+(m_vecCards.size()-44)%2)*24;
				   if (m_vecCards.size()%2)temp.y=410;
				   else   temp.y =402;
				 
		   }
		  m_vecCards.push_back( temp);
		  
	   }
		
	}

	sort(m_playA.begin(),m_playA.end(),less<CARDS>());
	for (int i=0;i<m_playA.size();i++)
	{
            m_playA[i].x=215+i*45;
			m_playA[i].s_rcet = CRect(m_playA[i].x,m_playA[i].y,m_playA[i].x+45,m_playA[i].y+71);
	}
	m_playA[m_playA.size()-1].x+=15;

	sort(m_playB.begin(),m_playB.end(),less<CARDS>());
	for (int i=0;i<m_playB.size();i++)
	 m_playB[i].y=150+i*20;

	sort(m_playC.begin(),m_playC.end(),less<CARDS>());
	for (int i=0;i<m_playC.size();i++)
	 m_playC[i].x=205+i*24;

	sort(m_playD.begin(),m_playD.end(),less<CARDS>());
	for (int i=0;i<m_playD.size();i++)
	 m_playD[i].y=170+i*20;

	m_bitmap[0]=(HBITMAP)::LoadImage(NULL,_T("Image\\back.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//背景
	m_bitmap[1]=(HBITMAP)::LoadImage(NULL,_T("Image\\back.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//背景
	m_bitmap[2]=(HBITMAP)::LoadImage(NULL,_T("Image\\back_shu.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//玩家出的牌
	m_bitmap[3]=(HBITMAP)::LoadImage(NULL,_T("Image\\2_pass.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//2家出的牌
	m_bitmap[4]=(HBITMAP)::LoadImage(NULL,_T("Image\\me_pass.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//玩家对面出的牌
	m_bitmap[5]=(HBITMAP)::LoadImage(NULL,_T("Image\\111.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//对家从牌的图片
	//m_bitmap[5]=(HBITMAP)::LoadImage(NULL,_T("Image\\me_pass.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//对家从牌的图片
	m_bitmap[6]=(HBITMAP)::LoadImage(NULL,_T("Image\\cross.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//对家从牌的图片
	m_bitmap[7]=(HBITMAP)::LoadImage(NULL,_T("Image\\peng.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//碰的图片
	m_bitmap[9]=(HBITMAP)::LoadImage(NULL,_T("Image\\hu.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//过的图片
	m_bitmap[10]=(HBITMAP)::LoadImage(NULL,_T("Image\\gang.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//过的图片
	m_bitmap[8]=(HBITMAP)::LoadImage(NULL,_T("Image\\guo.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//过的图片
//	cross.bmp


	
}


void CCards::drawCards(CDC * pDc)//绘制游戏中的牌
{
	
  m_dcBuffer.CreateCompatibleDC(pDc);
  m_dcImage.CreateCompatibleDC(pDc);
  m_2pass.CreateCompatibleDC(pDc);
 m_back.CreateCompatibleDC(pDc);

  m_dcBuffer.SelectObject(m_bitmap[0]);

 
   //绘制背景
   m_back.SelectObject(m_bitmap[1]);
   m_dcBuffer.BitBlt(0,0,900,620,&m_back,0,0,SRCCOPY);

  m_dcImage.SelectObject(m_bitmap[2]);

  m_2pass.SelectObject(m_bitmap[3]);


  //自家
  for (int i=0;i<m_playA.size();i++)
	  m_dcBuffer.BitBlt(m_playA[i].x,m_playA[i].y,45,71,&m_dcImage,(m_playA[i].index/10*9+m_playA[i].index%10)*45,0,SRCCOPY);
  //右边家
  for (int i=0;i<m_playB.size();i++)
	  m_dcBuffer.BitBlt(m_playB[i].x,m_playB[i].y,30,27,&m_2pass,m_playB[i].cx,m_playB[i].cy,SRCCOPY);
//对家
    m_2pass.SelectObject(m_bitmap[4]);
  for (int i=0;i<m_playC.size();i++)
	  m_dcBuffer.BitBlt(m_playC[i].x,m_playC[i].y,24,36,&m_2pass,m_playC[i].cx,m_playC[i].cy,SRCCOPY);


//左边家
  m_2pass.SelectObject(m_bitmap[3]);
  for (int i=0;i<m_playD.size();i++)
	  m_dcBuffer.BitBlt(m_playD[i].x,m_playD[i].y,30,27,&m_2pass,m_playD[i].cx,m_playD[i].cy,SRCCOPY);

    m_2pass.SelectObject(m_bitmap[5]);
	for (int i=0;i<m_vecCards.size();i++)
	{
      m_dcBuffer.BitBlt(m_vecCards[i].x,m_vecCards[i].y,m_vecCards[i].wide,m_vecCards[i].high,&m_2pass,m_vecCards[i].cx,m_vecCards[i].cy,SRCCOPY);
	}
 
 
            m_2pass.SelectObject(m_bitmap[4]);
	for (int i=0;i<m_vcsendCard.size();i++)
	{
		if (m_vcsendCard[i].whoCard==PLAYER_A||m_vcsendCard[i].whoCard==PLAYER_C)
		{
			m_2pass.SelectObject(m_bitmap[4]);
		}else
			m_2pass.SelectObject(m_bitmap[3]);
		m_dcBuffer.BitBlt(m_vcsendCard[i].x,m_vcsendCard[i].y,m_vcsendCard[i].wide,m_vcsendCard[i].high,&m_2pass,m_vcsendCard[i].cx,m_vcsendCard[i].cy,SRCCOPY);

	}

	//绘制箭头

	m_2pass.SelectObject(m_bitmap[6]);
	m_dcBuffer.BitBlt(420,225,intarray_arrow[m_sendCard.whoChupai*2+1],58,&m_2pass,intarray_arrow[m_sendCard.whoChupai*2],58,MERGEPAINT);
	m_dcBuffer.BitBlt(420,225,intarray_arrow[m_sendCard.whoChupai*2+1],58,&m_2pass,intarray_arrow[m_sendCard.whoChupai*2],0,SRCAND);

	//绘制碰，过
	if (m_isPaintPeng)
	{
		m_2pass.SelectObject(m_bitmap[7]);
		m_dcBuffer.BitBlt(680,425,40,40,&m_2pass,0,0,SRCCOPY);
		m_2pass.SelectObject(m_bitmap[8]);
		m_dcBuffer.BitBlt(750,425,40,40,&m_2pass,0,0,SRCCOPY);

	}
	//绘制碰，过
	if (m_isHuPai)
	{
		m_2pass.SelectObject(m_bitmap[9]);
		m_dcBuffer.BitBlt(680,425,40,40,&m_2pass,0,0,SRCCOPY);
		m_2pass.SelectObject(m_bitmap[8]);
		m_dcBuffer.BitBlt(750,425,40,40,&m_2pass,0,0,SRCCOPY);
	}
	
	if (m_meGang)
	{
		m_2pass.SelectObject(m_bitmap[10]);
		m_dcBuffer.BitBlt(680,425,40,40,&m_2pass,0,0,SRCCOPY);
		m_2pass.SelectObject(m_bitmap[8]);
		m_dcBuffer.BitBlt(750,425,40,40,&m_2pass,0,0,SRCCOPY);
		 
	}

	
	

  pDc->BitBlt(0,0,900,620,&m_dcBuffer,0,0,SRCCOPY);


  m_dcBuffer.DeleteDC();
   m_dcImage.DeleteDC();
   m_2pass.DeleteDC();
   m_back.DeleteDC();
}

void CCards::mouseMove(CPoint& p)
{ 
	
	if (m_metCards.Move(m_playA,p))
	{
    }
}
bool  CCards::IsHuPai(vector<CARDS>& player)
{
	temp1  tempvec;
	vector<temp1> num;
	for (int i=0;i<player.size();i++)
	{
		tempvec.index = player[i].index;
		num.push_back(tempvec);
	}
	sort(num.begin(),num.end(),less<temp1>());
	m_metCards.tempvec.assign(num.begin(),num.end());
	m_metCards.duizi =false;
	if (m_metCards.hupai(num))
	{
		//AfxMessageBox(_T("恭喜你自摸"));
		m_metCards.m_mp3.Close();
		m_metCards.m_mp3.Open("g_ZiMo_02.mp3");
		m_metCards.m_mp3.Play();
		m_isMessage =false;
		m_gameState =GAME_END;
		return true;
	}

	return false;

}
bool CCards::isPao(vector<CARDS>& player,int index)//放炮
{
	temp1  tempvec;
	vector<temp1> num;
	tempvec.index =index;
	num.push_back(tempvec);
	for (int i=0;i<player.size();i++)
	{
		tempvec.index = player[i].index;
		num.push_back(tempvec);
	}
	sort(num.begin(),num.end(),less<temp1>());
	m_metCards.tempvec.assign(num.begin(),num.end());
	m_metCards.duizi =false;
	if (m_metCards.hupai(num))
	{
		//AfxMessageBox(_T("哈哈，恭喜你放炮了！"));
		m_isMessage =false;
		return true;
	}

	return false;
}

int chupai=0;
bool CCards::fangPao(int index)
{
	CARDS temp;
	if (isPao(m_playA,index)) //玩家A胡牌
	{
		chupai = m_sendCard .whoChupai;
		m_sendCard.whoChupai  =PLAYER_A;
		m_isHuPai =true;
		return true;
	}
	if (isPao(m_playB,index))//玩家B胡牌
	{
		temp.index =index;
		temp.cy=27;
		temp.cx=(temp.index/10*9+temp.index%10)*30;
		temp .x=760;
		temp.y =m_playB[m_playB.size()-1].y+30;
		m_playB.push_back(temp);
		m_vcsendCard.pop_back();
		m_gameState =GAME_END;
		

		return true;
	}
	if (isPao(m_playC,index))//玩家C胡牌
	{
		temp.index =index;

		temp.cy=0;
		temp.cx=(temp.index/10*9+temp.index%10)*24;
		temp .y=30;
		temp.x =m_playC[m_playC.size()-1].x+34;
	   m_playC.push_back(temp);
	   m_vcsendCard.pop_back();
	   m_gameState =GAME_END;

		return true;
	}
	if (isPao(m_playD,index))//玩家D胡牌
	{
		//m_sendCard.whoChupai =PLAYER_D;
		temp.index =index;

		temp.cy=0;
		temp.cx=(temp.index/10*9+temp.index%10)*30;
		temp .x=120;
		temp.y =m_playD[m_playD.size()-1].y+30;
		m_playD.push_back(temp);
		m_vcsendCard.pop_back();
		m_gameState =GAME_END;
		return true;
	}
	 return false;

}
 int i=0;

void CCards::lbuttonDown(CPoint & p)
{

	

	if (m_sendCard.whoChupai==PLAYER_A &&m_playA.size()%3==2&&m_gameState==GAME_START)
	{
	   
	if (!m_vecCards.empty()&&m_metCards.buttonDown(m_playA,m_sendCard.index))
	{
		//出牌
		m_metCards.sendCard(m_vcsendCard,m_sendCard);

		if (fangPao(m_sendCard.index))
		{
			//m_vcsendCard.pop_back();
			m_gameState =GAME_END;
		}else
		{
			//判断碰牌
			if (whoGangPai())
			{
                m_isMessage=true;
			}else
			{
				if (whoPengPai())
				{
					computerSendCards();

				}else 
				{
					m_sendCard.whoChupai =PLAYER_B;
					m_isMessage=true;
				}

			}

		}
   
		//排序
		sort(m_playA.begin(),m_playA.end(),less<CARDS>());
		for (int j=0;j<m_playA.size();j++)
		{
			m_playA[j].x=215+j*45;
			m_playA[j].s_rcet = CRect(m_playA[j].x,m_playA[j].y,m_playA[j].x+45,m_playA[j].y+71);
		}
	}
    }
	if (m_isPaintPeng)
	{
		if (g_peng.PtInRect(p)) //碰
		{
			m_sendCard.cardTp =2;
			m_metCards.deleteCards(m_playA,m_sendCard);
			m_metCards.deleteCards(m_playA,m_sendCard);
			m_vcsendCard.pop_back();
			m_metCards.sendCard(m_vcsendCard,m_sendCard);
			m_metCards.sendCard(m_vcsendCard,m_sendCard);
			m_metCards.sendCard(m_vcsendCard,m_sendCard);
			m_sendCard.cardTp =1;
			m_isPaintPeng =false;

		}else if(g_guo.PtInRect(p))//过
		{
			m_isPaintPeng =false;
			if (chupai==PLAYER_D)
		 {
			 m_sendCard.whoChupai =PLAYER_A;
			}else
				m_sendCard.whoChupai= chupai+1;
			m_isMessage=true;
		}

	}

	if (m_isHuPai)
	{
		if (g_peng.PtInRect(p)) //胡牌
		{
			m_vcsendCard.pop_back();
			m_isHuPai =false;
			m_gameState =GAME_END;
			CARDS temp;
			m_sendCard.whoChupai =PLAYER_A;
			temp.index =m_sendCard.index;

			temp.cy=0;
			temp.cx=(temp.index/10*9+temp.index%10)*30;
			temp .x=m_playA[m_playA.size()-1].x+60;
			temp.y =m_playA[m_playA.size()-1].y;
			m_playA.push_back(temp);

		}else if(g_guo.PtInRect(p))//过
		{
			if (chupai==PLAYER_D)
		 {
			 m_sendCard.whoChupai =PLAYER_A;
			}else
				m_sendCard.whoChupai= chupai+1;

			m_gameState =GAME_START;
			m_isHuPai =false;
			m_isMessage =true;
		}

	}
	if (m_meGang)
	{
		if (g_peng.PtInRect(p)) //胡牌
		{
			m_computerGang =true;
			m_vcsendCard.pop_back();
			m_meGang =false;
			m_sendCard.cardTp =2;
			m_metCards.deleteCards(m_playA,m_sendCard);
			m_metCards.deleteCards(m_playA,m_sendCard);
			m_metCards.deleteCards(m_playA,m_sendCard);
			m_vcsendCard.pop_back();
			m_metCards.sendCard(m_vcsendCard,m_sendCard);
			m_metCards.sendCard(m_vcsendCard,m_sendCard);
			m_metCards.sendCard(m_vcsendCard,m_sendCard);
			m_metCards.sendCard(m_vcsendCard,m_sendCard);
			m_sendCard.cardTp =1;
			m_isMessage =true;
		

		}else if(g_guo.PtInRect(p))//过
		{
			if (chupai==PLAYER_D)
		 {
			 m_sendCard.whoChupai =PLAYER_A;
			}else
				m_sendCard.whoChupai= chupai+1;

			m_gameState =GAME_START;
			m_meGang =false;
			m_isMessage =true;
		}

	}
	
}
bool CCards::whoAnGang(int index)
{

	if (m_metCards.anGangPai(m_playA,index)&&m_sendCard.whoChupai==PLAYER_A)
	{
		m_computerGang =true;
		m_isMessage =true;
		//AfxMessageBox(_T("玩家A杠牌"));
	
		m_sendCard.cardTp =2;
		m_sendCard.index =index;
		
		m_metCards.deleteCards(m_playA,m_sendCard);
		m_metCards.deleteCards(m_playA,m_sendCard);
		m_metCards.deleteCards(m_playA,m_sendCard);
		m_metCards.deleteCards(m_playA,m_sendCard);

		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;
		computerSendCards();
		
		return true;
	}
	if (m_metCards.anGangPai(m_playB,index)&&m_sendCard.whoChupai==PLAYER_B)
	{

		m_computerGang =true;
		m_isMessage =true;
	//	AfxMessageBox(_T("玩家B杠牌"));
		m_sendCard.cardTp =2;
		m_sendCard.index =index;

		m_metCards.deleteCards(m_playB,m_sendCard);
		m_metCards.deleteCards(m_playB,m_sendCard);
		m_metCards.deleteCards(m_playB,m_sendCard);
		m_metCards.deleteCards(m_playB,m_sendCard);

		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;
		computerSendCards();
		return true;

	}
	if (m_metCards.anGangPai(m_playC,index)&&m_sendCard.whoChupai==PLAYER_C)
	{
		m_computerGang =true;
		m_isMessage =true;
		//AfxMessageBox(_T("玩家C杠牌"));
		m_sendCard.cardTp =2;
		m_sendCard.index =index;

		m_metCards.deleteCards(m_playC,m_sendCard);
		m_metCards.deleteCards(m_playC,m_sendCard);
		m_metCards.deleteCards(m_playC,m_sendCard);
		m_metCards.deleteCards(m_playC,m_sendCard);

		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;
		computerSendCards();
		return true;

	}
	if (m_metCards.anGangPai(m_playD,index)&&m_sendCard.whoChupai==PLAYER_D)
	{
		m_computerGang =true;
		m_isMessage =true;
		//AfxMessageBox(_T("玩家D杠牌"));
		m_sendCard.cardTp =2;
		m_sendCard.index =index;

		m_metCards.deleteCards(m_playD,m_sendCard);
		m_metCards.deleteCards(m_playD,m_sendCard);
		m_metCards.deleteCards(m_playD,m_sendCard);
		m_metCards.deleteCards(m_playD,m_sendCard);

		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;
		computerSendCards();
        return true;
	}
	
 return false;
}
bool CCards:: whoGangPai()//判断谁要杠牌
{
	if (m_metCards.gangPai(m_playA,m_sendCard)) //A
	{
		chupai = m_sendCard.whoChupai;
		m_meGang =true;
		m_sendCard.whoChupai  =PLAYER_A;
		m_isMessage =false;
		/*chupai = m_sendCard.whoChupai;
		m_sendCard.whoChupai = PLAYER_A; 
		m_isPaintPeng =true;*/
	//	m_isMessage=false;
		return true;

	}else if (m_metCards.gangPai(m_playB,m_sendCard))  //b
	{
		m_computerGang =true;
		m_isMessage =true;
	//	AfxMessageBox(_T("玩家B杠牌"));
		m_sendCard.whoChupai = PLAYER_B;
		m_sendCard.cardTp =2;
		m_metCards.deleteCards(m_playB,m_sendCard);
		m_metCards.deleteCards(m_playB,m_sendCard);
		m_metCards.deleteCards(m_playB,m_sendCard);

		m_vcsendCard.pop_back();
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;

		//m_isMessage=false;
		return true;
	}else if (m_metCards.gangPai(m_playC,m_sendCard))  //C
	{
		m_computerGang =true;
		m_isMessage =true;
	//	AfxMessageBox(_T("玩家C杠牌"));
		m_sendCard.whoChupai = PLAYER_C;
		m_sendCard.cardTp =2;
		m_metCards.deleteCards(m_playC,m_sendCard);
		m_metCards.deleteCards(m_playC,m_sendCard);
		m_metCards.deleteCards(m_playC,m_sendCard);

		m_vcsendCard.pop_back();
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;
		//m_isMessage=false;
		return true;
	}else if (m_metCards.gangPai(m_playD,m_sendCard)) //D
	{
		m_computerGang =true;
		m_isMessage =true;
	//		AfxMessageBox(_T("玩家D杠牌"));
		m_sendCard.whoChupai = PLAYER_D;
		m_sendCard.cardTp =2;
		m_metCards.deleteCards(m_playD,m_sendCard);
		m_metCards.deleteCards(m_playD,m_sendCard);
		m_metCards.deleteCards(m_playD,m_sendCard);
		m_vcsendCard.pop_back();
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;

	//	m_isMessage=false;
		return true;
	}
	return false;

}
bool  CCards::whoPengPai()//判断谁要碰牌
{
	if (m_metCards.pengPai(m_playA,m_sendCard)&&m_sendCard.whoChupai!=PLAYER_A) //A
	{
		chupai = m_sendCard.whoChupai;
		m_sendCard.whoChupai = PLAYER_A; 
		m_isPaintPeng =true;
		m_isMessage=false;
		return true;

	}else if (m_metCards.pengPai(m_playB,m_sendCard)&&m_sendCard.whoChupai!=PLAYER_B)  //b
	{
		//AfxMessageBox(_T("玩家B碰牌"));
		m_sendCard.whoChupai = PLAYER_B;
		m_sendCard.cardTp =2;
		m_metCards.deleteCards(m_playB,m_sendCard);
		m_metCards.deleteCards(m_playB,m_sendCard);

		m_vcsendCard.pop_back();
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;
		
		m_isMessage=false;
		return true;
	}else if (m_metCards.pengPai(m_playC,m_sendCard)&&m_sendCard.whoChupai!=PLAYER_C)  //C
	{
		//AfxMessageBox(_T("玩家C碰牌"));
		m_sendCard.whoChupai = PLAYER_C;
		m_sendCard.cardTp =2;
		m_metCards.deleteCards(m_playC,m_sendCard);
		m_metCards.deleteCards(m_playC,m_sendCard);

		m_vcsendCard.pop_back();
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;
		m_isMessage=false;
		return true;
	}else if (m_metCards.pengPai(m_playD,m_sendCard)&&m_sendCard.whoChupai!=PLAYER_D) //D
	{
	//	AfxMessageBox(_T("玩家D碰牌"));
	   m_sendCard.whoChupai = PLAYER_D;
	     m_sendCard.cardTp =2;
		m_metCards.deleteCards(m_playD,m_sendCard);
		m_metCards.deleteCards(m_playD,m_sendCard);
		m_vcsendCard.pop_back();
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_metCards.sendCard(m_vcsendCard,m_sendCard);
		m_sendCard.cardTp =1;
		
		m_isMessage=false;
		return true;
	}
	return false;

}void CCards::computerSendCards()//电脑出牌
{
	if (m_gameState == GAME_START)
	{
	
	switch (m_sendCard.whoChupai)
	{
	case PLAYER_A:
		//if (!m_isPaintPeng)
		//{
		//	temp.x = m_playA[m_playA.size()-1].x+45+15;
		//	temp.y = m_playA[m_playA.size()-1].y;
		//	temp.s_rcet = CRect(temp.x,temp.y,temp.x+45,temp.y+71);
		//	m_playA.push_back(temp);
		//	//IsHuPai(m_playA);
		//}
		
		break;
	case PLAYER_B:
			sort(m_playB.begin(),m_playB.end(),less<CARDS>());
			for (int i=0;i<m_playB.size();i++)
				m_playB[i].y=150+i*20;
			m_sendCard.index=m_metCards.ChaiPai(m_playB);
			m_metCards.deleteCards(m_playB,m_sendCard);
			m_metCards.sendCard(m_vcsendCard,m_sendCard);
			if (!fangPao(m_sendCard.index))
			{
				if (!whoGangPai())
				{
					if (!whoPengPai())
					{
						m_isMessage =true;
						m_sendCard.whoChupai =PLAYER_C;
					}else
					{
						computerSendCards();
					}
				
				}
			}
			
		break;
	case PLAYER_C:
			sort(m_playC.begin(),m_playC.end(),less<CARDS>());
			for (int i=0;i<m_playC.size();i++)
				m_playC[i].x=205+i*24;
			m_sendCard.index=m_metCards.ChaiPai(m_playC);
			m_metCards.deleteCards(m_playC,m_sendCard);
			m_metCards.sendCard(m_vcsendCard,m_sendCard);

			if (!fangPao(m_sendCard.index))
			{
				if (!whoGangPai())
				{
					if (!whoPengPai())
					{
						m_isMessage =true;
						m_sendCard.whoChupai =PLAYER_D;
					}else
					{
						computerSendCards();
					}

				}
			}
			
			
			
		break;
	case PLAYER_D:
			sort(m_playD.begin(),m_playD.end(),less<CARDS>());
			for (int i=0;i<m_playD.size();i++)
				m_playD[i].y=170+i*20;
			m_sendCard.index=m_metCards.ChaiPai(m_playD);
			m_metCards.deleteCards(m_playD,m_sendCard);
			m_metCards.sendCard(m_vcsendCard,m_sendCard);
			if (!fangPao(m_sendCard.index))
			{
				if (!whoGangPai())
				{
					if (!whoPengPai())
					{
						m_isMessage =true;
						m_sendCard.whoChupai =PLAYER_A;
					}else
					{
						computerSendCards();
					}
				}
			}
		
		break;
	}
	}
}
void CCards::getPai()//摸牌
{
	vector<CARDS>::iterator itor;
	CARDS temp;
if (!m_vecCards.empty())
{
	if (m_computerGang)
	{
		itor = m_vecCards.end();
		temp.index=m_vecCards[m_vecCards.size()-1].index;
		temp.huase=m_vecCards[m_vecCards.size()-1].huase;
		m_computerGang =false;
		m_vecCards.pop_back();
	}else
	{
		if (i%2==0)
		{
			itor= m_vecCards.begin();
			temp.index=m_vecCards[0].index;
			temp.huase = m_vecCards[0].huase;
		}else 
		{
			if (m_vecCards.size()>1)
			{
				itor= m_vecCards.begin()+1;
				temp.index=m_vecCards[1].index;
				temp.huase = m_vecCards[1].huase;
			}else
			{
				itor= m_vecCards.begin();
				temp.index=m_vecCards[0].index;
				temp.huase = m_vecCards[0].huase;

			}
		}
		i++;
		m_vecCards.erase(itor);
	}
     switch (m_sendCard.whoChupai)
     {
	 case PLAYER_A:
		 temp.x = m_playA[m_playA.size()-1].x+45+15;
		 temp.y = 485;
		 temp.s_rcet = CRect(temp.x,temp.y,temp.x+45,temp.y+71);
		 m_playA.push_back(temp);
		IsHuPai(m_playA);
		whoAnGang(temp.index);
		 break;
	 case PLAYER_B:
		 temp.x = m_playB[m_playB.size()-1].x;
		 temp.y = m_playB[m_playB.size()-1].y+22+10;
		 temp.cx=(temp.huase*9+temp.index%10)*30;
		 temp.cy =27;
		 m_playB.push_back(temp);
	     IsHuPai(m_playB);
	   whoAnGang(temp.index);
		 break;
	 case PLAYER_C:
		 temp.x = m_playC[m_playC.size()-1].x+24+10;
		 temp.y = m_playC[m_playC.size()-1].y;
		 temp.cx=(temp.huase*9+temp.index%10)*24;
		 temp.cy =0;
		 m_playC.push_back(temp);
		 IsHuPai(m_playC);
       whoAnGang(temp.index);
		 
		 break;
	 case PLAYER_D:
		 temp.x = m_playD[m_playD.size()-1].x;
		 temp.y = m_playD[m_playD.size()-1].y+20+10;
		 temp.cx=(temp.huase*9+temp.index%10)*30;
		 temp.cy =0;
		 m_playD.push_back(temp);
		IsHuPai(m_playD);
		whoAnGang(temp.index);
		 break;
     }
}else m_gameState =GAME_END;
	
}

CCards::~CCards(void)
{
}
