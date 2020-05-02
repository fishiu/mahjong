#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "MahjongGB/MahjongGB.h"

#ifdef _BOTZONE_ONLINE
#include "jsoncpp/json.h"
#else
#include "json/json.h"
#endif

#define SIMPLEIO 0
//由玩家自己定义，0表示JSON交互，1表示简单交互。

using namespace std;
ostringstream sout;
istringstream sin;
vector<string> myCards = { "F4","F3","F1","F2","J2","J1","J3","B7","B9","W1","W9","T1","T9" };
string previous(string s) {
    char tmp = --s[1];
    char tmp_string[2];
    tmp_string[0] = s[0];
    tmp_string[1] = tmp;
    string s1 = string(tmp_string);
    return s1;
}
string post(string s) {
    char tmp = ++s[1];
    char tmp_string[2];
    tmp_string[0] = s[0];
    tmp_string[1] = tmp;
    string s1 = string(tmp_string);
    return s1;
}
int myPlayerID;
int flowerCnt = 0;
int quan;
int turnID;
string stmp;
int itmp;

vector<string> request, response;
//存所有player手中的牌
vector<vector<string> > hand;
//用来存手中可以动的牌的映射情况
unordered_map<string, int> myMap;
//用来存pengchigang完的牌
vector<string> peng;
vector<string> chi;
vector<string> gang;
//eg: genString('W',1)返回"W1"
string genString(char a, int b) {
    char c;
    c = to_string(b)[0];
    char d[3] = { 0 };
    d[0] = a;
    d[1] = c;
    return string(d);
}
//W4”表示“四万”，“B6”表示“六筒”，“T8”表示“8条”  “F1”～“F4”表示“东南西北”，“J1”～“J3”表示“中发白”,把每个key都映射到0
void initMap() {
    for (int i = 0; i <= 9; i++) {
        myMap[genString('W', i)] = 0;
        myMap[genString('B', i)] = 0;
        myMap[genString('T', i)] = 0;
    }
    for (int i = 0; i < 4; i++) {
        myMap[genString('F', i)] = 0;
    }
    for (int i = 0; i < 3; i++) {
        myMap[genString('J', i)] = 0;
    }
}
void refreshMap(int myPlayerID) {
    ////W4”表示“四万”，“B6”表示“六筒”，“T8”表示“8条”  “F1”～“F4”表示“东南西北”，“J1”～“J3”表示“中发白”
    initMap();
    int len = hand[myPlayerID].size();
    for (int i = 0; i < len; i++) {
        string tmp = hand[myPlayerID][i];
        //key不存在的情况
        if (myMap.find(tmp) == myMap.end()) {
            myMap[tmp] = 1;
        }
        else {
            myMap[tmp]++;
        }
    }
}
string FJ() {
    for (int i = 0; i < 4; i++) {
        if (myMap[genString('F', i)] == 1) {
            return genString('F', i);
        }
    }
    for (int i = 0; i < 3; i++) {
        if (myMap[genString('J', i)] == 1) {
            return genString('J', i);
        }
    }
    return "Fail";
}
////从两头向中间除去间隔1个空位的单牌
//W,B,T
string eraseSingle() {
    int i = 1, j = 9;
    for (; i <= j; i++, j--) {
        if (myMap[genString('W', i)] == 1) {
            if (!(i + 1 > 0 && i + 1 <= 9&& myMap[genString('W', i + 1)] > 0) && !(i - 1 > 0 && i - 1 <= 9 && myMap[genString('W', i - 1)] > 0))
                return genString('W', i);
        }
        if (j != i && myMap[genString('W', j)] == 1) {
            if (!( j + 1 > 0 && j + 1 <= 9 && myMap[genString('W', j + 1)] > 0 ) && !(j - 1 > 0 && j- 1 <= 9 && myMap[genString('W', j - 1)] > 0))
                return genString('W', j);
        }
    }
    return "Fail";
}
////从两头向中间除去单牌
//W,B,T
string single() {
    int i = 1, j = 9;
    for (; i <= j; i++, j--) {
        if (myMap[genString('W', i)] == 1) {
            return genString('W', i);
        }
        if (j != i && myMap[genString('W', j)] == 1) {
            return genString('W', j);
        }
    }
    return "Fail";
}
//三个参数分别是手中的13张牌 上轮打出来的牌 自己摸到的牌 其中后两个参数有默认值，选其一 不得两个都有
bool ifHU(vector<string> Cards, string lastCards = "n", string gotCard = "n") {
    vector<string> myCards;
    //复制出一个vector方便操作
    myCards = Cards;
    //如果是要了牌桌上的牌
    if (lastCards.size() == 2) {
        myCards.push_back(lastCards);
    }
    //如果是自己摸的牌
    if (gotCard.size() == 2) {
        myCards.push_back(gotCard);
    }
    //两种情况下都把这个牌加入到序列中

    //首先进行排序
    sort(myCards.begin(), myCards.end());
    int duizi = 0;
    int kezi = 0;
    int shunzi = 0;
    //int gangzi = 0;
    char flag = 'n';
    //分别记录每种牌的vector
    vector<string> B;
    vector<string> W;
    vector<string> T;
    vector<string> F;
    vector<string> J;
    vector<string>::iterator it;
    for (it = myCards.begin(); it < myCards.end(); it++) {
        string str = *it;
        if (str[0] == 'B') {
            B.push_back(str);
        }
        else if (str[0] == 'W') {
            W.push_back(str);
        }
        else if (str[0] == 'T') {
            T.push_back(str);
        }
        else if (str[0] == 'F') {
            F.push_back(str);
        }
        else if (str[0] == 'J') {
            J.push_back(str);
        }
        //cout << *it << " ";
    }
    vector<vector<string> > zong;
    zong.push_back(B);
    zong.push_back(F);
    zong.push_back(J);
    zong.push_back(T);
    zong.push_back(W);
    vector<vector<string> >::iterator zongit;
    for (zongit = zong.begin(); zongit < zong.end(); zongit++) {
        vector<string> vec = *zongit;
        sort(vec.begin(), vec.end());
        if (vec.size() >= 2) {
            if (vec.size() >= 3) {
                string str = vec[0];
                //判断顺子
                if (str[0] == 'B' || str[0] == 'T' || str[0] == 'W') {
                    int cha = -1, pcha = 1, len = 1;
                    for (int i = 1; i < vec.size(); i++) {
                        cha = vec[i][1] - vec[i - 1][1];
                        if (cha == pcha && cha == 1) {
                            len++;
                            if (len == 3) {
                                shunzi++;
                                len = 1;
                                pcha = cha;
                                continue;
                            }
                        }
                        else {
                            len = 1;
                        }
                        pcha = cha;
                    }
                }
                //判断刻子和杠子
                int tong = 1;
                for (int i = 1; i < vec.size(); i++) {
                    if (!strcmp(vec[i].c_str(), vec[i - 1].c_str())) {
                        tong++;
                        if (tong == 3) {
                            kezi++;
                            tong = 1;
                        }
                    }
                    else {
                        tong = 1;
                    }
                }
            }
            else {
                //判断对子
                int tong = 1;
                for (int i = 1; i < vec.size(); i++) {
                    if (!strcmp(vec[i].c_str(), vec[i - 1].c_str())) {
                        tong++;
                        if (tong == 2) {
                            duizi++;
                            tong = 1;
                        }
                    }
                    else {
                        tong = 1;
                    }
                }
            }
        }
    }
    //在这里加一个顺子刻子对子
    shunzi += chi.size();
    kezi += gang.size() + peng.size();
    //七连对
    if (duizi == 7) {
        return true;
    }

    //最正常的胡法
    if (duizi == 1 && kezi + shunzi == 4) {
        return true;
    }

    //十三幺
    if (shunzi == 0 && kezi == 0) {
        char prenum = '0';
        int feng = 0;
        for (it = F.begin(); it != F.end(); it++) {
            string str = *it;
            if (str[1] != prenum) {
                feng++;
            }
            prenum = str[1];
        }
        int jian = 0;
        for (it = J.begin(); it != J.end(); it++) {
            string str = *it;
            if (str[1] != prenum) {
                jian++;
            }
            prenum = str[1];
        }
        if (feng == 4 && jian == 3) {
            int mainflag = 1;
            int bflag = 0;
            for (it = B.begin(); it != B.end(); it++) {
                string str = *it;
                if (bflag == 0 && str[1] == '1') {
                    bflag++;
                }
                if (bflag == 1 && str[1] == '9') {
                    bflag++;
                }
                if (str[1] != '1'&&str[1] != '9') {	//存在别的牌说明十三幺不成立
                    mainflag = 0;
                    break;
                }
            }
            int tflag = 0;
            for (it = T.begin(); it != T.end(); it++) {
                string str = *it;
                if (tflag == 0 && str[1] == '1') {
                    tflag++;
                }
                if (tflag == 1 && str[1] == '9') {
                    tflag++;
                }
                if (str[1] != '1'&&str[1] != '9') {	//存在别的牌说明十三幺不成立
                    mainflag = 0;
                    break;
                }
            }
            int wflag = 0;
            for (it = W.begin(); it != W.end(); it++) {
                string str = *it;
                if (wflag == 0 && str[1] == '1') {
                    wflag++;
                }
                if (wflag == 1 && str[1] == '9') {
                    wflag++;
                }
                if (str[1] != '1'&&str[1] != '9') {	//存在别的牌说明十三幺不成立
                    mainflag = 0;
                    break;
                }
            }
            if (mainflag == 1 && bflag == 2 && wflag == 2 && tflag == 2) return true;
        }
    }
    return false;
}
string ableCHI(string newCard) {
    vector<string> card;
    card = myCards;
    card.push_back(newCard);
    sort(card.begin(), card.end());
    vector<string>::iterator itr;
    itr = find(card.begin(), card.end(), newCard);
    string str_1 = "N"; string str_2 = "N"; string str1 = "N"; string str2 = "N";
    if (itr != card.begin())	str_1 = *(itr - 1);

    if (itr - 1 != card.begin()) { str_2 = *(itr - 2); }
    //itr++; itr++;
    string str0 = *(itr);
    if (itr + 1 < card.end())	str1 = *(itr + 1);
    //itr++; itr++;
    if (itr + 2 < card.end())	str2 = *(itr + 2);

    if (str_1[0] == newCard[0] && str_2[0] == newCard[0]) {
        if (newCard[1] - str_1[1] == 1 && str_1[1] - str_2[1] == 1) {
            return str_1;
        }
    }
    if (str1[0] == newCard[0] && str2[0] == newCard[0]) {
        if (newCard[1] - str1[1] == -1 && str1[1] - str2[1] == -1) {
            return str1;
        }
    }
    if (str_1[0] == newCard[0] && str1[0] == newCard[0]) {
        if (newCard[1] - str_1[1] == 1 && str1[1] - newCard[1] == 1) {
            return newCard;
        }
    }
    return "Fail";
}

bool ablePENG(string newCard) {
    if (myMap.find(newCard) == myMap.end()) {
        return false;
    }
    else if (myMap[newCard] >= 2) {
        return true;
    }
    return false;
}

bool ableGANG(string newCard) {
    if (myMap.find(newCard) == myMap.end()) {
        return false;
    }
    else if (myMap[newCard] >= 3) {
        return true;
    }
    return false;
}

int getFan() {
    return 10;
    // 随便找个玩家，不是我自己就行
    int aPlayerId = (myPlayerID + 3) % 4;
    // 使用前初始化
    void MahjongInit();
    vector<pair<string, pair<string, int> > > pack;
    //计算pack
    for (int i = 0; i < peng.size(); ++i) {
        pack.push_back(make_pair("PENG",make_pair(peng[i],aPlayerId)));
    }
    for (int i = 0; i < chi.size(); ++i) {
        pack.push_back(make_pair("CHI",make_pair(chi[i],aPlayerId)));
    }
    for (int i = 0; i < gang.size(); ++i) {
        pack.push_back(make_pair("GANG",make_pair(gang[i],aPlayerId)));
    }

    //为了区别全局的hand，前面加上fan
    vector<string> fan_hand; //{"W2","W2","W2","W3","W3","W3","W4","W4","W4","W5"}
    //生成手牌vector
    auto myMapIter = myMap.begin();
    while(myMapIter != myMap.end()) {
        for (int i = 0; i < myMapIter->second; ++i)
        {
            fan_hand.push_back(myMapIter->first);
        }
        myMapIter++;
    }
    string winTile = myMap.end()->first; //"W5"
    int flowerCount = flowerCnt; //花牌数量
    bool isZIMO; //是否是自摸
    bool isJUEZHANG = 0; //绝张和
    bool isGANG = 0; //杠上开花
    bool isLAST = 0; //排墙最后一张
    int menFeng = myPlayerID; //门风
    int quanFeng = quan; //圈风

    // 算番函数
    vector<pair<int,string> > res = MahjongFanCalculator(
            pack,
            fan_hand,
            winTile,
            flowerCount,
            isZIMO,
            isJUEZHANG,
            isGANG,
            isLAST,
            menFeng,
            quanFeng);

    // 记录番数的和
    int sum = 0;
    for (int i = 0; i < res.size(); ++i) { //遍历每一个番种
        res[i].first += sum;
    }

    return sum;
}
//初始化现在的情况，返回摸到的牌
string init_position() {
    int itmp;
    string stmp;
    //itmp是轮次，myPlayerID，quan是风圈
    sin.str(request[0]);
    sin >> itmp >> myPlayerID >> quan;
    sin.clear();
    sin.str(request[1]);
    for (int j = 0; j < 5; j++) {
        sin >> itmp;
        if (j == myPlayerID + 1) {
            flowerCnt = itmp;
        }
    }
    for (int j = 0; j < 13; j++) {
        sin >> stmp;
        hand[myPlayerID].push_back(stmp);
    }
    for (int i = 2; i <= turnID; i++) {
        sin.clear();
        sin.str(request[i]);
        sin >> itmp;
        if (itmp == 2) {
            sin >> stmp;
            hand[myPlayerID].push_back(stmp);
            sin.clear();
            if (i == turnID) {
                //return 在这里
                return stmp;
            }
            sin.str(response[i]);
            sin >> stmp;
            if (stmp == "PLAY") {
                sin >> stmp;
                hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
            }
                //GANG T6 就是暗杠
            else if (stmp == "GANG") {
                sin >> stmp;
                for (int k = 0; k < 4; k++) {
                    hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
                }
                gang.push_back(stmp);
            }
        }
            //其他情况
            //3 playerID BUHUA Card1
        else if (itmp == 3) {
            int playerID;
            sin >> playerID;
            sin >> stmp;
            //自己playerID的已经读过了
            if (stmp == "PLAY" && playerID != myPlayerID) {
                sin >> stmp;
                hand[4].push_back(stmp);
                if (i == turnID) {
                    //返回值在这儿
                    return stmp;
                }
            }
                //3 playerID PENG Card1
            else if (stmp == "PENG") {
                stmp = hand[4][hand[4].size() - 1]; //peng的牌
                //如果是自己，就需要存peng的情况
                if (playerID == myPlayerID) {
                    //存peng
                    peng.push_back(stmp);
                    //hand中删掉这三张牌
                    for (int k = 0; k < 3; k++) {
                        hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
                    }
                    //map是最后一起生成的，这里不需要做
                }
                else {
                    for (int k = 0; k < 3; k++) {
                        hand[playerID].push_back(stmp);
                    }
                    hand[4].pop_back();
                    sin >> stmp;
                    hand[4].push_back(stmp);
                    if (turnID == i) {
                        return stmp;
                    }
                }
            }
                //3 playerID CHI Card1 Card2
            else if (stmp == "CHI") {
                sin >> stmp;
                if (myPlayerID == playerID) {
                    //chi里面调整
                    chi.push_back(stmp);
                    //hand里面调整
                    hand[playerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), previous(stmp)));
                    hand[playerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
                    hand[playerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), post(stmp)));
                }
                else {
                    hand[playerID].push_back(previous(stmp));
                    hand[playerID].push_back(stmp);
                    hand[playerID].push_back(post(stmp));
                    if (i == turnID) {
                        return stmp;
                    }
                }
            }
                //3 2 GANG
            else if (stmp == "GANG") {
                //需要判断明杠还是暗杠,通过检查GANG前上一次的情况判定
                sin.clear();
                sin.str(request[i - 1]);
                sin >> itmp;
                //排除暗摸的情况
                if (itmp == '3') {
                    sin >> itmp;
                    if (itmp == playerID) {
                        sin >> stmp;
                        if (stmp == "DRAW") {
                            //暗摸的情况，暂时没想好怎么办
                            continue;
                        }
                    }
                }
                stmp = hand[4][hand[4].size() - 1]; //gang的牌
                if (myPlayerID == playerID) {
                    for (int k = 0; k < 4; k++) {
                        hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
                    }
                    gang.push_back(stmp);
                }
                else {
                    for (int k = 0; k < 4; k++) {
                        hand[playerID].push_back(stmp);
                    }
                    hand[4].pop_back();
                    //gang完以后的摸牌操作看不到
                }
            }
                //3 playerID BUGANG Card1
            else if (stmp == "BUGANG") {
                //补杠的牌
                sin >> stmp;
                if (playerID == myPlayerID) {
                    //hand中一张牌即可
                    hand[playerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
                    //把peng变成gang
                    peng.erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
                    gang.push_back(stmp);
                }
                else {
                    hand[playerID].push_back(stmp);
                    if(i == turnID)
                        return stmp;
                }
            }
            else if (stmp == "BUHUA" && playerID == myPlayerID) {
                flowerCnt++;
            }
        }
    }
    return "Fail";
}
string play_card() {
    //如果可以听牌还没写，写在这儿

    //F,J落单，就直接扔
    string fj = FJ();
    //存在落单的东南西北中发白
    if (fj != "Fail") {
        return fj;
    }
    else {
        //从两头向中间除去间隔一个空位的单牌
        string es = eraseSingle();
        if (es != "Fail") {
            return es;
        }
        else {
            string sing = single();
            if (sing != "Fail") {
                return sing;
            }
        }
    }
    //什么都没有return的时候
    return  *(hand[myPlayerID].rbegin());

}


void decision_to_play_card() {
    //random_shuffle(hand.begin(), hand.end());
    //sout << "PLAY " << *hand.rbegin();
    //hand.pop_back();
    //以上是随机算法

    //判断能不能胡，如果能胡输出HU，并调用算番器
    bool tmpbool = ifHU(hand[myPlayerID], stmp);
    if (tmpbool && getFan() >= 8) {
        sout << "HU";
    }
    else {
        hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
        //生成一个自己手中牌的map<string,int>
        refreshMap(myPlayerID);
        //判断能否杠
        if (ableGANG(stmp)) {
            hand[myPlayerID].push_back(stmp);
            sout << "GANG " << stmp;
            //这里是暗杠，还没处理
            //把手中GANG的牌删掉
            hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
        }
        else {
            hand[myPlayerID].push_back(stmp);
            refreshMap(myPlayerID);
            string s = play_card();
            sout << "PLAY " << s;
        }
    }
}

void decision_to_response() {
    //进这个函数就保证是可以操作的，而不是只能输出PASS
    sin.clear();
    sin.str(request[turnID]);
    sin >> itmp;
    if (itmp != myPlayerID) {
        sin >> stmp >> stmp;
        //如果可以抢牌胡
        if (ifHU(hand[myPlayerID], stmp)) {
            sout << "HU";
        }
            //可以抢牌杠
        else if (ableGANG(stmp)) {
            sout << "GANG";
        }
            //可以碰
        else if (ablePENG(stmp)) {
            sout << "PENG ";
            //把PENG的牌处理一下
            for(int k = 1; k <= 2; k++)
                hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
            peng.push_back(stmp);
            //更新map
            refreshMap(myPlayerID);
            stmp = play_card();
            sout << stmp;
        }
        else if(ableCHI(stmp) != "Fail") {
            //stmp存了吃的牌的中间的牌
            hand[myPlayerID].push_back(stmp);//先存进来之后三个一起erase
            stmp = ableCHI(stmp);
            //三个erase
            hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), previous(stmp)));
            hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), stmp));
            hand[myPlayerID].erase(find(hand[myPlayerID].begin(), hand[myPlayerID].end(), post(stmp)));
            sout << "CHI " << stmp << " ";
            stmp = play_card();
            sout << stmp;
        }
        else {
            sout << "PASS";
        }
    }
    else {
        sout << "PASS";
    }
}


int main()
{
    //初始化hand 0-4是四个玩家，5是桌上的牌
    for (int i = 0; i < 5; i++) {
        vector<string> vtmp;
        hand.push_back(vtmp);
    }
#if SIMPLEIO
    cin >> turnID;
	turnID--;
	getline(cin, stmp);
	for (int i = 0; i < turnID; i++) {
		getline(cin, stmp);
		request.push_back(stmp);
		getline(cin, stmp);
		response.push_back(stmp);
	}
	getline(cin, stmp);
	request.push_back(stmp);
#else
    Json::Value inputJSON;
    cin >> inputJSON;
    turnID = inputJSON["responses"].size();
    for (int i = 0; i < turnID; i++) {
        request.push_back(inputJSON["requests"][i].asString());
        response.push_back(inputJSON["responses"][i].asString());
    }
    request.push_back(inputJSON["requests"][turnID].asString());
#endif

    if (turnID < 2) {
        response.push_back("PASS");
    }
    else {
        //如果这局itmp是2，stmp就是我在这局摸到的牌，如果这局itmp是3，只有别的玩家出牌了才会返回出的是什么牌，其他都返回“Fail”
        stmp = init_position();
        sin.clear();
        sin.str(request[turnID]);
        sin >> itmp;
        //到我的时候，这时stmp就是牌的信息
        if (itmp == 2) {
            decision_to_play_card();
        }
            //3 2 PLAY T1
        else if (itmp == 3) {
            if(stmp != "Fail")
                decision_to_response();
            else
                sout << "PASS";
        }
        response.push_back(sout.str());
    }

#if SIMPLEIO
    cout << response[turnID] << endl;
#else
    Json::Value outputJSON;
    outputJSON["response"] = response[turnID];
    cout << outputJSON << endl;
#endif
    return 0;
}
