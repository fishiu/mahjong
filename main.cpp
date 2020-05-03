#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
//#include "MahjongGB/MahjongGB.h"

#ifdef _BOTZONE_ONLINE
#include "jsoncpp/json.h"
#else

#include "json/json.h"

#endif

#define SIMPLEIO 0
//由玩家自己定义，0表示JSON交互，1表示简单交互。

using namespace std;

//字符串输出流
ostringstream str_out;
//字符串输入流
istringstream str_in;

//我的手牌，这个变量有问题，不能用
//vector<string> my_card;
//我的门风
int my_player_Id;
//花牌数量
int flower_count = 0;
//圈风
int quan;
//当前回合数
int turn_Id;
string stmp;
int itmp;

//存输入和输出
vector<string> request, response;
//存所有player手中的牌
vector<vector<string> > all_card;
//存手中活动牌 <牌名，此牌数量>
unordered_map<string, int> my_active_card;
//存PENG完的牌
vector<string> peng;
//存CHI完的牌
vector<string> chi;
//存GANG完的牌
vector<string> gang;

/**
 * 快速生成牌名，注意是char
 * @param type 'W'
 * @param number 1
 * @return "W1"
 */
string makeCardName(char type, int number) {
    char c;
    c = to_string(number)[0];
    char d[3] = {0};
    d[0] = type;
    d[1] = c;
    return string(d);
}

/**
 * 生成上一张牌的名字
 * @param card_name
 * @return 上一张牌名
 */
string previousCard(string card_name) {
    char tmp = --card_name[1];
    char tmp_string[2];
    tmp_string[0] = card_name[0];
    tmp_string[1] = tmp;
    string s1 = string(tmp_string);
    return s1;
}

/**
 * 生成下一张牌的名字
 * @param card_name
 * @return 下一张牌名
 */
string postCard(string card_name) {
    char tmp = ++card_name[1];
    char tmp_string[2];
    tmp_string[0] = card_name[0];
    tmp_string[1] = tmp;
    string s1 = string(tmp_string);
    return s1;
}

/**
 * 初始化手牌（生成映射）
 * W4”表示“四万”，“B6”表示“六筒”，“T8”表示“8条”  “F1”～“F4”表示“东南西北”，“J1”～“J3”表示“中发白”
 */
void initMyCard() {
    for (int i = 0; i <= 9; i++) {
        my_active_card[makeCardName('W', i)] = 0;
        my_active_card[makeCardName('B', i)] = 0;
        my_active_card[makeCardName('T', i)] = 0;
    }
    for (int i = 0; i < 4; i++) {
        my_active_card[makeCardName('F', i)] = 0;
    }
    for (int i = 0; i < 3; i++) {
        my_active_card[makeCardName('J', i)] = 0;
    }
}

/**
 * 根据输入结果存放我的手牌
 * @param my_player_ID 我的手牌
 */
void setMyCard(int my_player_ID) {
    initMyCard();
    int len = all_card[my_player_ID].size();
    for (int i = 0; i < len; i++) {
        string tmp = all_card[my_player_ID][i];
        //key不存在的情况
        if (my_active_card.find(tmp) == my_active_card.end()) {
            my_active_card[tmp] = 1;
        } else {
            my_active_card[tmp]++;
        }
    }
}

/**
 * 用来随便找成单的箭牌
 * @return 牌名
 */
string getSingleFengOrJian() {
    for (int i = 0; i < 4; i++) {
        if (my_active_card[makeCardName('F', i)] == 1) {
            return makeCardName('F', i);
        }
    }
    for (int i = 0; i < 3; i++) {
        if (my_active_card[makeCardName('J', i)] == 1) {
            return makeCardName('J', i);
        }
    }
    return "Fail";
}

/**
 * 从两头向中间除去间隔1个空位的单牌 W,B,T
 * @return 相应的牌名 如果找不到返回 Fail
 */
string eraseSingle() {
    int i = 1, j = 9;
    for (; i <= j; i++, j--) {
        if (my_active_card[makeCardName('W', i)] == 1) {
            if (!(i + 1 > 0 && i + 1 <= 9 && my_active_card[makeCardName('W', i + 1)] > 0) &&
                !(i - 1 > 0 && i - 1 <= 9 && my_active_card[makeCardName(
                        'W', i - 1)] > 0))
                return makeCardName('W', i);
        }
        if (j != i && my_active_card[makeCardName('W', j)] == 1) {
            if (!(j + 1 > 0 && j + 1 <= 9 && my_active_card[makeCardName('W', j + 1)] > 0) &&
                !(j - 1 > 0 && j - 1 <= 9 && my_active_card[makeCardName(
                        'W', j - 1)] > 0))
                return makeCardName('W', j);
        }
    }
    return "Fail";
}

/**
 * 从两头向中间除去单牌 W,B,T
 * @return 相应的牌名 如果找不到返回 Fail
 */
string single() {
    int i = 1, j = 9;
    for (; i <= j; i++, j--) {
        if (my_active_card[makeCardName('W', i)] == 1) {
            return makeCardName('W', i);
        }
        if (j != i && my_active_card[makeCardName('W', j)] == 1) {
            return makeCardName('W', j);
        }
    }
    return "Fail";
}

/**
 * 其中后两个参数有默认值，选其一 不得两个都有
 * @param Cards 手中的13张牌
 * @param lastCards 上轮打出来的牌
 * @param gotCard 自己摸到的牌
 * @return 胡 true 不胡 false
 */
bool checkHu(vector<string> Cards, string lastCards = "n", string gotCard = "n") {
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
        } else if (str[0] == 'W') {
            W.push_back(str);
        } else if (str[0] == 'T') {
            T.push_back(str);
        } else if (str[0] == 'F') {
            F.push_back(str);
        } else if (str[0] == 'J') {
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
                        } else {
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
                    } else {
                        tong = 1;
                    }
                }
            } else {
                //判断对子
                int tong = 1;
                for (int i = 1; i < vec.size(); i++) {
                    if (!strcmp(vec[i].c_str(), vec[i - 1].c_str())) {
                        tong++;
                        if (tong == 2) {
                            duizi++;
                            tong = 1;
                        }
                    } else {
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
                if (str[1] != '1' && str[1] != '9') {    //存在别的牌说明十三幺不成立
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
                if (str[1] != '1' && str[1] != '9') {    //存在别的牌说明十三幺不成立
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
                if (str[1] != '1' && str[1] != '9') {    //存在别的牌说明十三幺不成立
                    mainflag = 0;
                    break;
                }
            }
            if (mainflag == 1 && bflag == 2 && wflag == 2 && tflag == 2) return true;
        }
    }
    return false;
}

/**
 * 判断是否能CHI牌
 * @param newCard 新出现的牌（摸来 OR 玩家的牌）
 * @return bool
 */
string ableCHI(string newCard) {
    vector<string> card;
    card = my_card;
    card.push_back(newCard);
    sort(card.begin(), card.end());
    vector<string>::iterator itr;
    itr = find(card.begin(), card.end(), newCard);
    string str_1 = "N";
    string str_2 = "N";
    string str1 = "N";
    string str2 = "N";
    if (itr != card.begin()) str_1 = *(itr - 1);

    if (itr - 1 != card.begin()) { str_2 = *(itr - 2); }
    //itr++; itr++;
    string str0 = *(itr);
    if (itr + 1 < card.end()) str1 = *(itr + 1);
    //itr++; itr++;
    if (itr + 2 < card.end()) str2 = *(itr + 2);

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

/**
 * 判断是否能PENG牌
 * @param newCard 新出现的牌（摸来 OR 玩家的牌）
 * @return bool
 */
bool ablePENG(string newCard) {
    if (my_active_card.find(newCard) == my_active_card.end()) {
        return false;
    } else if (my_active_card[newCard] >= 2) {
        return true;
    }
    return false;
}

/**
 * 判断是否能GANG牌
 * @param newCard 新出现的牌（摸来 OR 玩家的牌）
 * @return bool
 */
bool ableGANG(string newCard) {
    if (my_active_card.find(newCard) == my_active_card.end()) {
        return false;
    } else if (my_active_card[newCard] >= 3) {
        return true;
    }
    return false;
}

/**
 * 算番器
 * @return 番数
 */
int getFan() {
    return 10;
    // 随便找个玩家，不是我自己就行
    int aPlayerId = (my_player_Id + 3) % 4;
    // 使用前初始化
    void MahjongInit();
    vector<pair<string, pair<string, int> > > pack;
    //计算pack
    for (int i = 0; i < peng.size(); ++i) {
        pack.push_back(make_pair("PENG", make_pair(peng[i], aPlayerId)));
    }
    for (int i = 0; i < chi.size(); ++i) {
        pack.push_back(make_pair("CHI", make_pair(chi[i], aPlayerId)));
    }
    for (int i = 0; i < gang.size(); ++i) {
        pack.push_back(make_pair("GANG", make_pair(gang[i], aPlayerId)));
    }

    //为了区别全局的hand，前面加上fan
    vector<string> fan_hand; //{"W2","W2","W2","W3","W3","W3","W4","W4","W4","W5"}
    //生成手牌vector
    auto myMapIter = my_active_card.begin();
    while (myMapIter != my_active_card.end()) {
        for (int i = 0; i < myMapIter->second; ++i) {
            fan_hand.push_back(myMapIter->first);
        }
        myMapIter++;
    }
    string winTile = my_active_card.end()->first; //"W5"
    int flowerCount = flower_count; //花牌数量
    bool isZIMO; //是否是自摸
    bool isJUEZHANG = 0; //绝张和
    bool isGANG = 0; //杠上开花
    bool isLAST = 0; //排墙最后一张
    int menFeng = my_player_Id; //门风
    int quanFeng = quan; //圈风

    // 算番函数
    vector<pair<int, string> > res = MahjongFanCalculator(
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

/**
 * 初始化现在的情况
 * @return 摸到的牌
 */
string init_position() {
    int itmp;
    string stmp;
    //itmp是轮次，myPlayerID，quan是风圈
    str_in.str(request[0]);
    str_in >> itmp >> my_player_Id >> quan;
    str_in.clear();
    str_in.str(request[1]);
    for (int j = 0; j < 5; j++) {
        str_in >> itmp;
        if (j == my_player_Id + 1) {
            flower_count = itmp;
        }
    }
    for (int j = 0; j < 13; j++) {
        str_in >> stmp;
        all_card[my_player_Id].push_back(stmp);
    }
    for (int i = 2; i <= turn_Id; i++) {
        str_in.clear();
        str_in.str(request[i]);
        str_in >> itmp;
        if (itmp == 2) {
            str_in >> stmp;
            all_card[my_player_Id].push_back(stmp);
            str_in.clear();
            if (i == turn_Id) {
                //return 在这里
                return stmp;
            }
            str_in.str(response[i]);
            str_in >> stmp;
            if (stmp == "PLAY") {
                str_in >> stmp;
                all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
            }
                //GANG T6 就是暗杠
            else if (stmp == "GANG") {
                str_in >> stmp;
                for (int k = 0; k < 4; k++) {
                    all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
                }
                gang.push_back(stmp);
            }
        }
            //其他情况
            //3 playerID BUHUA Card1
        else if (itmp == 3) {
            int playerID;
            str_in >> playerID;
            str_in >> stmp;
            //自己playerID的已经读过了
            if (stmp == "PLAY" && playerID != my_player_Id) {
                str_in >> stmp;
                all_card[4].push_back(stmp);
                if (i == turn_Id) {
                    //返回值在这儿
                    return stmp;
                }
            }
                //3 playerID PENG Card1
            else if (stmp == "PENG") {
                stmp = all_card[4][all_card[4].size() - 1]; //peng的牌
                //如果是自己，就需要存peng的情况
                if (playerID == my_player_Id) {
                    //存peng
                    peng.push_back(stmp);
                    //hand中删掉这三张牌
                    for (int k = 0; k < 3; k++) {
                        all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
                    }
                    //map是最后一起生成的，这里不需要做
                } else {
                    for (int k = 0; k < 3; k++) {
                        all_card[playerID].push_back(stmp);
                    }
                    all_card[4].pop_back();
                    str_in >> stmp;
                    all_card[4].push_back(stmp);
                    if (turn_Id == i) {
                        return stmp;
                    }
                }
            }
                //3 playerID CHI Card1 Card2
            else if (stmp == "CHI") {
                str_in >> stmp;
                if (my_player_Id == playerID) {
                    //chi里面调整
                    chi.push_back(stmp);
                    //hand里面调整
                    all_card[playerID].erase(
                            find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), previousCard(stmp)));
                    all_card[playerID].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
                    all_card[playerID].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), postCard(stmp)));
                } else {
                    all_card[playerID].push_back(previousCard(stmp));
                    all_card[playerID].push_back(stmp);
                    all_card[playerID].push_back(postCard(stmp));
                    if (i == turn_Id) {
                        return stmp;
                    }
                }
            }
                //3 2 GANG
            else if (stmp == "GANG") {
                //需要判断明杠还是暗杠,通过检查GANG前上一次的情况判定
                str_in.clear();
                str_in.str(request[i - 1]);
                str_in >> itmp;
                //排除暗摸的情况
                if (itmp == '3') {
                    str_in >> itmp;
                    if (itmp == playerID) {
                        str_in >> stmp;
                        if (stmp == "DRAW") {
                            //暗摸的情况，暂时没想好怎么办
                            continue;
                        }
                    }
                }
                stmp = all_card[4][all_card[4].size() - 1]; //gang的牌
                if (my_player_Id == playerID) {
                    for (int k = 0; k < 4; k++) {
                        all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
                    }
                    gang.push_back(stmp);
                } else {
                    for (int k = 0; k < 4; k++) {
                        all_card[playerID].push_back(stmp);
                    }
                    all_card[4].pop_back();
                    //gang完以后的摸牌操作看不到
                }
            }
                //3 playerID BUGANG Card1
            else if (stmp == "BUGANG") {
                //补杠的牌
                str_in >> stmp;
                if (playerID == my_player_Id) {
                    //hand中一张牌即可
                    all_card[playerID].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
                    //把peng变成gang
                    peng.erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
                    gang.push_back(stmp);
                } else {
                    all_card[playerID].push_back(stmp);
                    if (i == turn_Id)
                        return stmp;
                }
            } else if (stmp == "BUHUA" && playerID == my_player_Id) {
                flower_count++;
            }
        }
    }
    return "Fail";
}

/**
 * 出牌
 * @return 要出的牌
 */
string play_card() {
    //如果可以听牌还没写，写在这儿

    //F,J落单，就直接扔
    string fj = getSingleFengOrJian();
    //存在落单的东南西北中发白
    if (fj != "Fail") {
        return fj;
    } else {
        //从两头向中间除去间隔一个空位的单牌
        string es = eraseSingle();
        if (es != "Fail") {
            return es;
        } else {
            string sing = single();
            if (sing != "Fail") {
                return sing;
            }
        }
    }
    //什么都没有return的时候
    return *(all_card[my_player_Id].rbegin());

}

/**
 * 我还没看懂这在干啥
 */
void decision_to_play_card() {
    //random_shuffle(hand.begin(), hand.end());
    //sout << "PLAY " << *hand.rbegin();
    //hand.pop_back();
    //以上是随机算法

    //判断能不能胡，如果能胡输出HU，并调用算番器
    bool tmpbool = checkHu(all_card[my_player_Id], stmp);
    if (tmpbool && getFan() >= 8) {
        str_out << "HU";
    } else {
        all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
        //生成一个自己手中牌的map<string,int>
        setMyCard(my_player_Id);
        //判断能否杠
        if (ableGANG(stmp)) {
            all_card[my_player_Id].push_back(stmp);
            str_out << "GANG " << stmp;
            //这里是暗杠，还没处理
            //把手中GANG的牌删掉
            all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
        } else {
            all_card[my_player_Id].push_back(stmp);
            setMyCard(my_player_Id);
            string s = play_card();
            str_out << "PLAY " << s;
        }
    }
}

/**
 * 这个函数我也不知道在干嘛
 */
void decision_to_response() {
    //进这个函数就保证是可以操作的，而不是只能输出PASS
    str_in.clear();
    str_in.str(request[turn_Id]);
    str_in >> itmp;
    if (itmp != my_player_Id) {
        str_in >> stmp >> stmp;
        //如果可以抢牌胡
        if (checkHu(all_card[my_player_Id], stmp)) {
            str_out << "HU";
        }
            //可以抢牌杠
        else if (ableGANG(stmp)) {
            str_out << "GANG";
        }
            //可以碰
        else if (ablePENG(stmp)) {
            str_out << "PENG ";
            //把PENG的牌处理一下
            for (int k = 1; k <= 2; k++)
                all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
            peng.push_back(stmp);
            //更新map
            setMyCard(my_player_Id);
            stmp = play_card();
            str_out << stmp;
        } else if (ableCHI(stmp) != "Fail") {
            //stmp存了吃的牌的中间的牌
            all_card[my_player_Id].push_back(stmp);//先存进来之后三个一起erase
            stmp = ableCHI(stmp);
            //三个erase
            all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), previousCard(stmp)));
            all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), stmp));
            all_card[my_player_Id].erase(find(all_card[my_player_Id].begin(), all_card[my_player_Id].end(), postCard(stmp)));
            str_out << "CHI " << stmp << " ";
            stmp = play_card();
            str_out << stmp;
        } else {
            str_out << "PASS";
        }
    } else {
        str_out << "PASS";
    }
}


int main() {
    //初始化all_card 0-4是四个玩家，5是桌上的牌
    for (int i = 0; i < 5; i++) {
        vector<string> vtmp;
        all_card.push_back(vtmp);
    }
#if SIMPLEIO
    cin >> turn_Id;
    turn_Id--;
    getline(cin, stmp);
    for (int i = 0; i < turn_Id; i++) {
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
    //记录当前回合数（responses的数量）
    turn_Id = inputJSON["responses"].size();
    //读取交互信息，存入request和response
    for (int i = 0; i < turn_Id; i++) {
        request.push_back(inputJSON["requests"][i].asString());
        response.push_back(inputJSON["responses"][i].asString());
    }
    request.push_back(inputJSON["requests"][turn_Id].asString());
#endif

    if (turn_Id < 2) { //如果当前回合数是1或0
        response.push_back("PASS");
    } else { //如果这局itmp是2，stmp就是我在这局摸到的牌，如果这局itmp是3，只有别的玩家出牌了才会返回出的是什么牌，其他都返回“Fail”
        stmp = init_position();
        str_in.clear();
        str_in.str(request[turn_Id]);
        str_in >> itmp;
        if (itmp == 2) { //到我的时候，这时stmp就是牌的信息
            decision_to_play_card();
        }
        //3 2 PLAY T1
        else if (itmp == 3) {
            if (stmp != "Fail")
                decision_to_response();
            else
                str_out << "PASS";
        }
        response.push_back(str_out.str());
    }

#if SIMPLEIO
    cout << response[turn_Id] << endl;
#else
    Json::Value outputJSON;
    outputJSON["response"] = response[turn_Id];
    cout << outputJSON << endl;
#endif
    return 0;
}
