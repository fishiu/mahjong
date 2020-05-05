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

using namespace std;

//字符串输出流
ostringstream str_out;
//字符串输入流
istringstream str_in;

//我的手牌，这个变量有问题，不能用
//vector<string> my_card;
//我的门风
int my_player_id;
//花牌数量
int flower_count = 0;
//圈风
int quan;
//当前回合数
int turn_id;
string stmp;
int itmp;

//存输入和输出
vector<string> request, response;
//存所有player手中(已知)的牌, all_card[4]是牌桌上已经出出来的牌
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
string checkChi(string newCard) {
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
bool checkPeng(string newCard) {
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
bool checkGang(string newCard) {
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
    int aPlayerId = (my_player_id + 3) % 4;
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
    int menFeng = my_player_id; //门风
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
 * @return 当前回合我摸到的牌
 */
string initCondition() {
    //记录信息的第一个字段：编号
    int record_type;
    //暂存牌名
    string str_tmp;
    str_in.str(request[0]);
    str_in >> record_type >> my_player_id >> quan;
    str_in.clear();
    str_in.str(request[1]);
    for (int j = 0; j < 5; j++) {
        str_in >> record_type;
        if (j == my_player_id + 1) {
            flower_count = record_type;
        }
    }
    for (int j = 0; j < 13; j++) {
        str_in >> str_tmp;
        all_card[my_player_id].push_back(str_tmp);
    }
    for (int i = 2; i <= turn_id; i++) {
        //读（过去）的输入
        str_in.clear();
        str_in.str(request[i]);
        str_in >> record_type;

        if (record_type == 2) { //我自己摸牌
            str_in >> str_tmp;
            all_card[my_player_id].push_back(str_tmp);
            str_in.clear();
            if (i == turn_id) {
                // 返回我摸到的牌
                return str_tmp;
            }

            //读（我过去的）输出
            str_in.str(response[i]);
            str_in >> str_tmp;
            if (str_tmp == "PLAY") {
                str_in >> str_tmp;
                all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
            }
            else if (str_tmp == "GANG") { //GANG T6 就是暗杠
                str_in >> str_tmp;
                for (int k = 0; k < 4; k++) {
                    all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
                }
                gang.push_back(str_tmp);
            }
        }

        //其他玩家的操作信息比如 3 playerID BUHUA Card1
        else if (record_type == 3) {

            int player_id;
            str_in >> player_id;
            str_in >> str_tmp;

            //处理出牌操作，自己的已经处理过了
            if (str_tmp == "PLAY" && player_id != my_player_id) {
                str_in >> str_tmp;
                //其他玩家出的牌要存在all_card[4]
                all_card[4].push_back(str_tmp);
                if (i == turn_id) {
                    //返回值在这儿
                    return str_tmp;
                }
            }

            //处理PENG操作 3 playerID PENG Card1
            else if (str_tmp == "PENG") {
                str_tmp = all_card[4][all_card[4].size() - 1]; //peng的牌
                //如果是自己，就需要存peng的情况
                if (player_id == my_player_id) {
                    //存peng
                    peng.push_back(str_tmp);
                    //all_card中删掉这三张牌
                    for (int k = 0; k < 3; k++) {
                        all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
                    }
                    //map是最后一起生成的，这里不需要做
                } else {
                    for (int k = 0; k < 3; k++) {
                        all_card[player_id].push_back(str_tmp);
                    }
                    all_card[4].pop_back();
                    str_in >> str_tmp;
                    all_card[4].push_back(str_tmp);
                    if (turn_id == i) {
                        return str_tmp;
                    }
                }
            }

            //处理CHI操作 3 playerID CHI Card1 Card2
            else if (str_tmp == "CHI") {
                str_in >> str_tmp;
                if (my_player_id == player_id) {
                    //chi里面调整
                    chi.push_back(str_tmp);
                    //hand里面调整
                    all_card[player_id].erase(
                            find(all_card[my_player_id].begin(), all_card[my_player_id].end(), previousCard(str_tmp)));
                    all_card[player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
                    all_card[player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), postCard(str_tmp)));
                } else {
                    all_card[player_id].push_back(previousCard(str_tmp));
                    all_card[player_id].push_back(str_tmp);
                    all_card[player_id].push_back(postCard(str_tmp));
                    if (i == turn_id) {
                        return str_tmp;
                    }
                }
            }

            //处理GANG操作 3 playerID GANG
            else if (str_tmp == "GANG") {
                //需要判断明杠还是暗杠,通过检查GANG前上一次的情况判定
                str_in.clear();
                str_in.str(request[i - 1]);
                str_in >> record_type;
                //排除暗摸的情况
                if (record_type == 3) {
                    str_in >> record_type;
                    if (record_type == player_id) {
                        str_in >> str_tmp;
                        if (str_tmp == "DRAW") {
                            //暗摸的情况，暂时没想好怎么办
                            continue;
                        }
                    }
                }
                str_tmp = all_card[4][all_card[4].size() - 1]; //gang的牌
                if (my_player_id == player_id) {
                    for (int k = 0; k < 4; k++) {
                        all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
                    }
                    gang.push_back(str_tmp);
                } else {
                    for (int k = 0; k < 4; k++) {
                        all_card[player_id].push_back(str_tmp);
                    }
                    all_card[4].pop_back();
                    //gang完以后的摸牌操作看不到
                }
            }

            //处理BUGANG操作 3 playerID BUGANG Card1
            else if (str_tmp == "BUGANG") {
                //补杠的牌
                str_in >> str_tmp;
                if (player_id == my_player_id) {
                    //hand中一张牌即可
                    all_card[player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
                    //把peng变成gang
                    peng.erase(find(peng.begin(), peng.end(), str_tmp));
                    gang.push_back(str_tmp);
                } else {
                    all_card[player_id].push_back(str_tmp);
                    if (i == turn_id)
                        return str_tmp;
                }
            } else if (str_tmp == "BUHUA" && player_id == my_player_id) {
                flower_count++;
            }
        }
    }
    return "Fail";
}

/**
 * 挑选一张最适合出牌的牌
 * @return 最优的牌
 */
string getBestCard() {
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
    return *(all_card[my_player_id].rbegin());

}

/**
 * 轮到我出牌
 */
void playCard() {
    /*
     random_shuffle(all_card.begin(), all_card.end());
     str_out << "PLAY " << *all_card.rbegin();
     hand.pop_back();
     */
    //以上是随机算法

	//这里这个 erase 是针对initCondition作出的调整 因为下面的判断胡的函数和判断杠的函数都默认手牌是13张（不包括新摸的牌）
	all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), stmp));
    //判断能不能胡，如果能胡输出HU，并调用算番器
    bool tmpbool = checkHu(all_card[my_player_id], stmp);
    if (tmpbool && getFan() >= 8) {
        str_out << "HU";
    } else {
		//这里是原erase的位置
        //生成一个自己手中牌的map<string,int>
        setMyCard(my_player_id);
        //判断能否杠
        if (checkGang(stmp)) {
            all_card[my_player_id].push_back(stmp);
            str_out << "GANG " << stmp;
            //这里是暗杠，还没处理
            //把手中GANG的牌删掉
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), stmp));
        } else {
            all_card[my_player_id].push_back(stmp);
            setMyCard(my_player_id);
            string s = getBestCard();
            str_out << "PLAY " << s;
        }
    }
}

/**
 * 不是我出牌的Turn时进行决策
 */
void responseOutTurn() {
    //进这个函数就保证是可以操作的，而不是只能输出PASS
    str_in.clear();
    str_in.str(request[turn_id]);
    str_in >> itmp;
    if (itmp != my_player_id) {
        str_in >> stmp >> stmp;
        //如果可以抢牌胡
        if (checkHu(all_card[my_player_id], stmp)) {
            str_out << "HU";
        }
            //可以抢牌杠
        else if (checkGang(stmp)) {
            str_out << "GANG";
        }
            //可以碰
        else if (checkPeng(stmp)) {
            str_out << "PENG ";
            //把PENG的牌处理一下
            for (int k = 1; k <= 2; k++)
                all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), stmp));
            peng.push_back(stmp);
            //更新map
            setMyCard(my_player_id);
            stmp = getBestCard();
            str_out << stmp;
        } else if (checkChi(stmp) != "Fail") {
            //stmp存了吃的牌的中间的牌
            all_card[my_player_id].push_back(stmp);//先存进来之后三个一起erase
            stmp = checkChi(stmp);
            //三个erase
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), previousCard(stmp)));
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), stmp));
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), postCard(stmp)));
            str_out << "CHI " << stmp << " ";
            stmp = getBestCard();
            str_out << stmp;
        } else {
            str_out << "PASS";
        }
    } else {
        str_out << "PASS";
    }
}


int main() {
    //初始化all_card 0-3是四个玩家，4是桌上的牌
    for (int i = 0; i < 5; i++) {
        vector<string> vtmp;
        all_card.push_back(vtmp);
    }

    //Json交互的输入（删掉了普通交互）
    Json::Value input_json;
    cin >> input_json;
    //当前回合的下标（即总共回合数量-1）
    turn_id = input_json["responses"].size();
    //读取交互信息，存入request和response
    for (int i = 0; i < turn_id; i++) {
        request.push_back(input_json["requests"][i].asString());
        response.push_back(input_json["responses"][i].asString());
    }
    request.push_back(input_json["requests"][turn_id].asString());

    if (turn_id < 2) { //如果当前回合数是1或0
        response.push_back("PASS");
    } else { //如果这局itmp是2，stmp就是我在这局摸到的牌，如果这局itmp是3，只有别的玩家出牌了才会返回出的是什么牌，其他都返回“Fail”
        stmp = initCondition();
        str_in.clear();
        str_in.str(request[turn_id]);
        str_in >> itmp;
        if (itmp == 2) { //到我的时候，这时stmp就是牌的信息
            playCard();
        }
        //3 2 PLAY T1
        else if (itmp == 3) {
            if (stmp != "Fail")
                responseOutTurn();
            else
                str_out << "PASS";
        }
        response.push_back(str_out.str());
    }

    //Json交互的输出（删掉了普通交互）
    Json::Value outputJSON;
    outputJSON["response"] = response[turn_id];
    cout << outputJSON << endl;
    return 0;
}
