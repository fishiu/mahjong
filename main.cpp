#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include "MahjongGB/MahjongGB.h"

#ifdef _BOTZONE_ONLINE
#include "jsoncpp/json.h"
#else
#include "json/json.h"
#endif

//定义几个策略名称的宏
#define STG_PENG_PENG 1
#define STG_HUN_YI_SE 2
#define STG_WU_MEN 3
#define STG_QUAN_QIU_REN 4

using namespace std;

//字符串输出流
ostringstream str_out;
//字符串输入流
istringstream str_in;
//json输出
Json::Value outputJSON;
//debug信息
string debug_info = "";

//我的手牌，这个变量有问题，不能用
//vector<string> my_card;
//我的门风
int my_player_id;
//花牌数量
int flower_count = 0;
//记录除去花牌之外所有的牌
int not_flower_count = 0;
//圈风
int quan;
//当前回合数的下标（如果turn_id是5，那么一共有6个request和5个response）有点坑
int turn_id;

string stmp;
int itmp;

//存输入和输出
vector<string> request, response;
//存所有player手中(已知)的牌, all_card[4]是牌桌上已经出出来的牌
vector<vector<string> > all_card;
//存当前回合"活动牌"，用来记录initCondition的返回值，主要用于算番器的winTile参数
string current_card;
//存手中活动牌 <牌名，此牌数量>
unordered_map<string, int> my_active_card;
//存我所有的吃碰杠 <类型，<牌名，具体情况>>
//吃：<CHI，<B2，2>> 表示 B1 B2 B3 第三个int：第i张牌是从上家吃来的
//碰：<PENG，<B2，2>> 表示 B2 B2 B2 第三个int：碰的牌是玩家i出的
//杠：<GANG，<B2，2>> 表示 B2 B2 B2 B2 第三个int：杠的牌是玩家i出的
vector<pair<string, pair<string, int> > > my_pack;
//存PENG完的牌
vector<string> peng;
//存CHI完的牌
vector<string> chi;
//存GANG完的牌
vector<string> gang;
//存我目前信息下牌墙中剩的牌
unordered_map<string, int>rest_card;
//存我目前信息下牌墙中剩的牌
unordered_map<string, int>all_rest_card;
//存我的手牌的排序形式
vector<string> sorted_my_card;

//下面是为了复式赛制增加的解决牌荒问题的变量
int left_card_num[4] = { 21, 21, 21, 21 }; //34张初始发掉13张剩下21张

//记录是否自摸（用来算番）
bool is_ZIMO = false;

//记录当前局的策略，用于debug
int current_strategy = -1;

/**
 * 快速生成牌名，注意是char
 * @param type 'W'
 * @param number 1
 * @return "W1"
 */
string makeCardName(char type, int number) {
    char c;
    c = to_string(number)[0];
    char d[3] = { 0 };
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
    tmp_string[2] = 0;
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
    tmp_string[2] = 0;
    string s1 = string(tmp_string);
    return s1;
}

/*
*计算一张牌亮出来过几次
*/
int getCardNumAll(string card_name) {
    int _cnt = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < all_card[i].size(); ++j) {
            if (all_card[i][j] == card_name) {
                _cnt++;
            }
        }
    }
    for (int i = 0; i < peng.size(); i++) {
        if (peng[i] == card_name) {
            _cnt += 3;
        }
    }
    for (int i = 0; i < gang.size(); i++) {
        if (gang[i] == card_name) {
            _cnt += 4;
        }
    }
    for (int i = 0; i < chi.size(); i++) {
        if (chi[i] == card_name) {
            _cnt++;
        }
        if (postCard(chi[i]) == card_name) {
            _cnt++;
        }
        if (previousCard(chi[i]) == card_name) {
            _cnt++;
        }
    }
    return _cnt;
}

/*获取牌堆中剩下的牌 + 别人手中的牌*/
void getRestCard() {
    for (int i = 1; i <= 9; i++) {
        rest_card[makeCardName('W', i)] = 4 - getCardNumAll(makeCardName('W', i));
        rest_card[makeCardName('B', i)] = 4 - getCardNumAll(makeCardName('B', i));
        rest_card[makeCardName('T', i)] = 4 - getCardNumAll(makeCardName('T', i));
    }
    for (int i = 1; i <= 4; i++) {
        rest_card[makeCardName('F', i)] = 4 - getCardNumAll(makeCardName('F', i));
    }
    for (int i = 1; i <= 3; i++) {
        rest_card[makeCardName('J', i)] = 4 - getCardNumAll(makeCardName('J', i));
    }
}

//和getrestcard相反，是已经有的牌的数量
void getAllCard() {
    for (int i = 1; i <= 9; i++) {
        all_rest_card[makeCardName('W', i)] = getCardNumAll(makeCardName('W', i));
        all_rest_card[makeCardName('B', i)] = getCardNumAll(makeCardName('B', i));
        all_rest_card[makeCardName('T', i)] = getCardNumAll(makeCardName('T', i));
    }
    for (int i = 1; i <= 4; i++) {
        all_rest_card[makeCardName('F', i)] = getCardNumAll(makeCardName('F', i));
    }
    for (int i = 1; i <= 3; i++) {
        all_rest_card[makeCardName('J', i)] = getCardNumAll(makeCardName('J', i));
    }
}

/**
 * 生成我的所有手牌的vector，包括鸣牌和暗牌
 * @return vector<string>
 */
vector<string> getAllMyCard() {
    vector<string> _v = all_card[my_player_id];
    for (int i = 0; i < chi.size(); ++i) {
        _v.push_back(chi[i]);
        _v.push_back(postCard(chi[i]));
        _v.push_back(previousCard(chi[i]));
    }
    for (int i = 0; i < peng.size(); ++i) {
        for (int j = 0; j < 3; ++j)
            _v.push_back(peng[i]);
    }
    for (int i = 0; i < gang.size(); ++i) {
        for (int j = 0; j < 4; ++j)
            _v.push_back(gang[i]);
    }
    return _v;
}

/**
 * 生成吃碰杠组合
 * @param type CHI/PENG/GANG
 * @param card_name 牌名
 * @param detail 对于吃：第几张牌是吃来的，对于碰杠：牌是谁供的
 * @return 返回pack
 */
pair<string, pair<string, int> > makePack(string type, string card_name, int detail) {
    return make_pair(type, make_pair(card_name, detail));
}

/**
 * 初始化手牌（生成映射）
 * W4”表示“四万”，“B6”表示“六筒”，“T8”表示“8条”  “F1”～“F4”表示“东南西北”，“J1”～“J3”表示“中发白”
 */
void initMyCard() {
    for (int i = 1; i <= 9; i++) {
        my_active_card[makeCardName('W', i)] = 0;
        my_active_card[makeCardName('B', i)] = 0;
        my_active_card[makeCardName('T', i)] = 0;
    }
    for (int i = 1; i <= 4; i++) {
        my_active_card[makeCardName('F', i)] = 0;
    }
    for (int i = 1; i <= 3; i++) {
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
        }
        else {
            my_active_card[tmp]++;
        }
    }
}

/**
 * 用来随便找成单的箭牌
 * @return 牌名
 */
string getSingleFengOrJian() {
    for (int i = 1; i <= 4; i++) {
        if (my_active_card[makeCardName('F', i)] == 1) {
            return makeCardName('F', i);
        }
    }
    for (int i = 1; i <= 4; i++) {
        if (my_active_card[makeCardName('J', i)] == 1) {
            return makeCardName('J', i);
        }
    }
    return "Fail";
}
/**
 * 从两头向中间除去间隔2个空位的单牌 W,B,T
 * @return 相应的牌名 如果找不到返回 Fail
 */
string eraseDouble() {
    int i = 1, j = 9;
    for (; i <= j; i++, j--) {
        if (my_active_card[makeCardName('W', i)] == 1) {
            if (!(i + 1 > 0 && i + 1 <= 9 && my_active_card[makeCardName('W', i + 1)] > 0) &&
                !(i - 1 > 0 && i - 1 <= 9 && my_active_card[makeCardName('W', i - 1)] > 0) &&
                !(i + 2 > 0 && i + 2 <= 9 && my_active_card[makeCardName('W', i + 2)] > 0) &&
                !(i - 2 > 0 && i - 2 <= 9 && my_active_card[makeCardName('W', i - 2)] > 0))
                return makeCardName('W', i);
        }
        if (my_active_card[makeCardName('B', i)] == 1) {
            if (!(i + 1 > 0 && i + 1 <= 9 && my_active_card[makeCardName('B', i + 1)] > 0) &&
                !(i - 1 > 0 && i - 1 <= 9 && my_active_card[makeCardName('B', i - 1)] > 0) &&
                !(i + 2 > 0 && i + 2 <= 9 && my_active_card[makeCardName('B', i + 2)] > 0) &&
                !(i - 2 > 0 && i - 2 <= 9 && my_active_card[makeCardName('B', i - 2)] > 0))
                return makeCardName('B', i);
        }
        if (my_active_card[makeCardName('T', i)] == 1) {
            if (!(i + 1 > 0 && i + 1 <= 9 && my_active_card[makeCardName('T', i + 1)] > 0) &&
                !(i - 1 > 0 && i - 1 <= 9 && my_active_card[makeCardName('T', i - 1)] > 0) &&
                !(i + 2 > 0 && i + 2 <= 9 && my_active_card[makeCardName('T', i + 2)] > 0) &&
                !(i - 2 > 0 && i - 2 <= 9 && my_active_card[makeCardName('T', i - 2)] > 0))
                return makeCardName('T', i);
        }
        if (j != i && my_active_card[makeCardName('W', j)] == 1) {
            if (!(j + 1 > 0 && j + 1 <= 9 && my_active_card[makeCardName('W', j + 1)] > 0) &&
                !(j - 1 > 0 && j - 1 <= 9 && my_active_card[makeCardName('W', j - 1)] > 0) &&
                !(j + 2 > 0 && j + 2 <= 9 && my_active_card[makeCardName('W', j + 2)] > 0) &&
                !(j - 2 > 0 && j - 2 <= 9 && my_active_card[makeCardName('W', j - 2)] > 0))
                return makeCardName('W', j);
        }
        if (j != i && my_active_card[makeCardName('B', j)] == 1) {
            if (!(j + 1 > 0 && j + 1 <= 9 && my_active_card[makeCardName('B', j + 1)] > 0) &&
                !(j - 1 > 0 && j - 1 <= 9 && my_active_card[makeCardName('B', j - 1)] > 0) &&
                !(j + 2 > 0 && j + 2 <= 9 && my_active_card[makeCardName('B', j + 2)] > 0) &&
                !(j - 2 > 0 && j - 2 <= 9 && my_active_card[makeCardName('B', j - 2)] > 0))
                return makeCardName('B', j);
        }
        if (j != i && my_active_card[makeCardName('T', j)] == 1) {
            if (!(j + 1 > 0 && j + 1 <= 9 && my_active_card[makeCardName('T', j + 1)] > 0) &&
                !(j - 1 > 0 && j - 1 <= 9 && my_active_card[makeCardName('T', j - 1)] > 0) &&
                !(j + 2 > 0 && j + 2 <= 9 && my_active_card[makeCardName('T', j + 2)] > 0) &&
                !(j - 2 > 0 && j - 2 <= 9 && my_active_card[makeCardName('T', j - 2)] > 0))
                return makeCardName('T', j);
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
        if (my_active_card[makeCardName('B', i)] == 1) {
            if (!(i + 1 > 0 && i + 1 <= 9 && my_active_card[makeCardName('B', i + 1)] > 0) &&
                !(i - 1 > 0 && i - 1 <= 9 && my_active_card[makeCardName(
                        'B', i - 1)] > 0))
                return makeCardName('B', i);
        }
        if (my_active_card[makeCardName('T', i)] == 1) {
            if (!(i + 1 > 0 && i + 1 <= 9 && my_active_card[makeCardName('T', i + 1)] > 0) &&
                !(i - 1 > 0 && i - 1 <= 9 && my_active_card[makeCardName(
                        'T', i - 1)] > 0))
                return makeCardName('T', i);
        }
        if (j != i && my_active_card[makeCardName('W', j)] == 1) {
            if (!(j + 1 > 0 && j + 1 <= 9 && my_active_card[makeCardName('W', j + 1)] > 0) &&
                !(j - 1 > 0 && j - 1 <= 9 && my_active_card[makeCardName(
                        'W', j - 1)] > 0))
                return makeCardName('W', j);
        }
        if (j != i && my_active_card[makeCardName('B', j)] == 1) {
            if (!(j + 1 > 0 && j + 1 <= 9 && my_active_card[makeCardName('B', j + 1)] > 0) &&
                !(j - 1 > 0 && j - 1 <= 9 && my_active_card[makeCardName(
                        'B', j - 1)] > 0))
                return makeCardName('B', j);
        }
        if (j != i && my_active_card[makeCardName('T', j)] == 1) {
            if (!(j + 1 > 0 && j + 1 <= 9 && my_active_card[makeCardName('T', j + 1)] > 0) &&
                !(j - 1 > 0 && j - 1 <= 9 && my_active_card[makeCardName(
                        'T', j - 1)] > 0))
                return makeCardName('T', j);
        }
    }
    return "Fail";
}



int getFan(string);

/**
 * 判断胡牌
 * @param Cards 手中的13张牌
 * @param gotCard 自己摸到的牌
 * @return 番数
 */
int checkHu(vector<string> Cards, string gotCard) {
    return getFan(gotCard);
}


/**
*判断听牌，如果可以听牌，返回扔掉的牌
*找不到返回Fail
*/
string checkTing() {
    auto it = my_active_card.begin();
    unordered_map<string, int> hu_possibility,second_possibility;
    //遍历所有的key
    for (; it != my_active_card.end(); it++) {
        //如果有超过一张
        if (it->second > 0) {
            //删牌
            string value = it->first;
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), value));
            //可能胡牌的个数
            int sum = 0;
            getRestCard();
            //遍历牌墙剩下的牌
            for (auto i = rest_card.begin(); i != rest_card.end(); i++) {
                if (i->second > 0 && checkHu(all_card[my_player_id], i->first) >= 8) {
                    sum += i->second;
                }
            }
            if (sum) {
                hu_possibility[it->first] = sum;
            }
            //需要插回去
            all_card[my_player_id].push_back(value);
        }
    }
    //找出sum最大的值
    if (hu_possibility.size() == 0) {
        //看看能不能换两张牌以后听牌

        return "Fail";
    }
    else {
        string throw_card;
        int max_card = 0;
        for (auto it = hu_possibility.begin(); it != hu_possibility.end(); it++) {
            if (it->second > max_card) {
                max_card = it->second;
                throw_card = it->first;
            }
        }
        return throw_card;
    }
}
/**
 * 从两头向中间除去单牌 W,B,T
 * @return 相应的牌名 如果找不到返回 Fail
 */
//可以考虑加上出手里最少的牌型
//可以考虑加上出手里最少的牌型
string single() {
    setMyCard(my_player_id);
    for (int remain = 4; remain >= 1; remain--) {
        for (int i = 1; i <= 4; i++) {
            if (my_active_card[makeCardName('F', i)] == 1 && getCardNumAll(makeCardName('F', i)) == remain) {
                return makeCardName('F', i);
            }
        }
        for (int i = 1; i <= 3; i++) {
            if (my_active_card[makeCardName('J', i)] == 1 && getCardNumAll(makeCardName('J', i)) == remain) {
                return makeCardName('J', i);
            }
        }
        int i = 1, j = 9;
        for (; i <= j; i++, j--) {
            if (my_active_card[makeCardName('W', i)] == 1 && getCardNumAll(makeCardName('W', i)) == remain) {
                return makeCardName('W', i);
            }
            if (j != i && my_active_card[makeCardName('W', j)] == 1 && getCardNumAll(makeCardName('W', j)) == remain) {
                return makeCardName('W', j);
            }
            if (my_active_card[makeCardName('B', i)] == 1 && getCardNumAll(makeCardName('B', i)) == remain) {
                return makeCardName('B', i);
            }
            if (j != i && my_active_card[makeCardName('B', j)] == 1 && getCardNumAll(makeCardName('B', j)) == remain) {
                return makeCardName('B', j);
            }
            if (my_active_card[makeCardName('T', i)] == 1 && getCardNumAll(makeCardName('T', i)) == remain) {
                return makeCardName('T', i);
            }
            if (j != i && my_active_card[makeCardName('T', j)] == 1 && getCardNumAll(makeCardName('T', j)) == remain) {
                return makeCardName('T', j);
            }
        }
    }

    return "Fail";
    /*
    int i = 1, j = 9;
    for (; i <= j; i++, j--) {
        if (my_active_card[makeCardName('W', i)] == 1) {
            return makeCardName('W', i);
        }
        if (j != i && my_active_card[makeCardName('W', j)] == 1) {
            return makeCardName('W', j);
        }
        if (my_active_card[makeCardName('B', i)] == 1) {
            return makeCardName('B', i);
        }
        if (j != i && my_active_card[makeCardName('B', j)] == 1) {
            return makeCardName('B', j);
        }
        if (my_active_card[makeCardName('T', i)] == 1) {
            return makeCardName('T', i);
        }
        if (j != i && my_active_card[makeCardName('T', j)] == 1) {
            return makeCardName('T', j);
        }
    }*/
    //return "Fail";
}


/**
 * 判断是否能CHI牌
 * @param newCard 新出现的牌（摸来 OR 玩家的牌）
 * @return bool
 */
string checkChi(string newCard) {
    //return "Fail";
    if (newCard[0] == 'F' || newCard[0] == 'J')
        return "Fail";
    int flag_1 = 0, flag_2 = 0, flag0 = 0, flag1 = 0, flag2 = 0;
    if (my_active_card[previousCard(previousCard(newCard))]) {
        flag_2 = my_active_card[previousCard(previousCard(newCard))];
    }
    if (my_active_card[previousCard(newCard)]) {
        flag_1 = my_active_card[previousCard(newCard)];
    }
    flag0 = 1;
    if (my_active_card[postCard(postCard(newCard))]) {
        flag2 = my_active_card[postCard(postCard(newCard))];
    }
    if (my_active_card[postCard(newCard)]) {
        flag1 = my_active_card[postCard(newCard)];
    }
    if (newCard[1] - '0' > 5) {
        if (flag1 == 1 && flag2 == 1) {
            return postCard(newCard);
        }
        if (flag_1 == 1 && flag1 == 1) {
            return newCard;
        }
        if (flag_1 == 1 && flag_2 == 1) {
            return previousCard(newCard);
        }
    }
    else {
        if (flag_1 == 1 && flag_2 == 1) {
            return previousCard(newCard);
        }
        if (flag_1 == 1 && flag1 == 1) {
            return newCard;
        }
        if (flag1 == 1 && flag2 == 1) {
            return postCard(newCard);
        }
    }
    if (newCard[1] - '0' > 5) {
        if (flag1 == 2 && flag2 == 2) {
            return postCard(newCard);
        }
        if (flag_1 == 2 && flag1 == 2) {
            return newCard;
        }
        if (flag_1 == 2 && flag_2 == 2) {
            return previousCard(newCard);
        }
    }
    else {
        if (flag_1 == 2 && flag_2 == 2) {
            return previousCard(newCard);
        }
        if (flag_1 == 2 && flag1 == 2) {
            return newCard;
        }
        if (flag1 == 2 && flag2 == 2) {
            return postCard(newCard);
        }
    }
    return "Fail";
    /*
    vector<string> card;
    card = all_card[my_player_id];
    card.push_back(newCard);
    sort(card.begin(), card.end());
    vector<string>::iterator itr;
    vector<string>::iterator tmptr;
    itr = find(card.begin(), card.end(), newCard);
    string str_1 = "N";
    string str_2 = "N";
    string str1 = "N";
    string str2 = "N";
    string str0 = *(itr);
    tmptr = itr;
    while (itr != card.begin() && !strcmp(tmptr->c_str(), itr->c_str()) {
        tmptr = itr - 1;
    }
    str_1 = *tmptr;
    tmptr = itr;
    while (itr != card.begin() && itr - 1 != card.begin() && !strcmp(tmptr->c_str(), itr->c_str()) {
        tmptr = itr - 1;
    }
    str_1 = *tmptr;
    if (itr - 1 != card.begin()) { str_2 = *(itr - 2); }
    //itr++; itr++;

    if (itr + 1 != card.end()) str1 = *(itr + 1);
    //itr++; itr++;
    if (itr + 2 < card.end()) str2 = *(itr + 2);

    if (str_1[0] == newCard[0] && str_2[0] == newCard[0]) {
        if (str_1.length() > 1 && str_2.length() > 1 && newCard[1] - str_1[1] == 1 && str_1[1] - str_2[1] == 1) {
            return str_1;
        }
    }
    if (str1[0] == newCard[0] && str2[0] == newCard[0]) {
        if (str1.length() > 1 && str2.length() > 1 && newCard[1] - str1[1] == -1 && str1[1] - str2[1] == -1) {
            return str1;
        }
    }
    if (str_1[0] == newCard[0] && str1[0] == newCard[0]) {
        if (str_1.length() > 1 && str1.length() > 1 && newCard[1] - str_1[1] == 1 && str1[1] - newCard[1] == 1) {
            return newCard;
        }
    }
    */

}

/**
 * 判断是否能PENG牌
 * @param newCard 新出现的牌（摸来 OR 玩家的牌）
 * @return bool
 */
bool checkPeng(string newCard) {
    if (my_active_card.find(newCard) == my_active_card.end()) {
        return false;
    }
    else if (my_active_card[newCard] == 2) {
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
    }
    else if (my_active_card[newCard] >= 3) {
        return true;
    }
    return false;
}

//下面是一些用来辅助判断番数的函数

/**
 * 判断是否是绝张胡
 * 逐个判断每一张手牌是不是绝张（桌上已经有了三张）
 * @return true 确实绝张胡，false 不是绝张胡
 */
bool isJueZhang() {
    for (auto& i : my_active_card) {
        string _card = i.first;
        int cnt = 0;
        for (int j = 0; j < all_card[4].size(); ++j)
            if (all_card[4][j] == _card)
                cnt++;
        if (i.second > 0 && cnt == 3)
            return true;
    }
    return false;
}

/**
 * 算番器里的pack中的int是玩家的位置而不是id
 * @param player_id 要计算的玩家
 * @return 相对自己的位置 0自己 上家 2对家 3下家
 */
int player_id_to_position(int _player_id) {
    switch ((_player_id + 4 - my_player_id) % 4) {
        case 0: //自己
            return 0;
        case 1: //下家
            return 3;
        case 2: //对家
            return 2;
        case 3: //上家
            return 1;
    }
}

/**
 * 算番器
 * @return 番数
 */
int getFan(string win_tile) {
    vector<pair<string, pair<string, int> > > pack;
    //计算pack
    for (int i = 0; i < my_pack.size(); ++i) {
        pair<string, pair<string, int> > _pack = my_pack[i];
        if (my_pack[i].first != "CHI") {
            _pack.second.second = player_id_to_position(my_pack[i].second.second);
        } else {
            _pack.second.second = my_pack[i].second.second;
        }
        pack.push_back(_pack);
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
    //用itmp判断自摸不对
    //bool is_ZIMO = (itmp == 2); //是否是自摸
    bool is_JUEZHANG = isJueZhang(); //绝张和
    bool is_GANG = 0; //杠上开花
    bool is_LAST = (flower_count + not_flower_count >= 144); //排墙最后一张
    int MEN_FENG = my_player_id; //门风
    int quanFeng = quan; //圈风

    // 算番函数
    MahjongInit();
    try {
        vector<pair<int, string> > res = MahjongFanCalculator(
                pack,
                fan_hand,
                win_tile,
                flower_count,
                is_ZIMO,
                is_JUEZHANG,
                is_GANG,
                is_LAST,
                MEN_FENG,
                quanFeng);

        // 记录番数的和
        int sum = 0;
        for (int i = 0; i < res.size(); ++i) { //遍历每一个番种
            sum += res[i].first;
        }
        return sum;
    }
    catch (const string &error) {
        return 0;
    }
}
/*不点炮*/
//目标是输入一副胡牌牌型，输出
string bu_dian_pao() {
    //有一个all_rest_card的map
    getAllCard();
    setMyCard(my_player_id);
    //先把我没有的牌删了,即all_rest_card记为-1，接下来就输出all_rest_card比较多的牌
    for (int i = 1; i <= 9; i++) {
        if (my_active_card[makeCardName('W', i)] == 0) {
            all_rest_card[makeCardName('W', i)] = -1;
        }
        if (my_active_card[makeCardName('B', i)] == 0) {
            all_rest_card[makeCardName('B', i)] = -1;
        }
        if (my_active_card[makeCardName('T', i)] == 0) {
            all_rest_card[makeCardName('T', i)] = -1;
        }
    }
    for (int i = 1; i <= 4; i++) {
        if (my_active_card[makeCardName('F', i)] == 0) {
            all_rest_card[makeCardName('F', i)] = -1;
        }
    }
    for (int i = 1; i <= 3; i++) {
        if (my_active_card[makeCardName('J', i)] == 0) {
            all_rest_card[makeCardName('J', i)] = -1;
        }
    }
    string most_card = "error";
    int max_card_num = -1;

    for (int i = 1; i <= 9; i++) {
        if (my_active_card[makeCardName('B', i)] > max_card_num) {
            max_card_num = my_active_card[makeCardName('B', i)];
            most_card = makeCardName('B', i);
        }
        if (my_active_card[makeCardName('W', i)] > max_card_num) {
            max_card_num = my_active_card[makeCardName('W', i)];
            most_card = makeCardName('W', i);
        }
        if (my_active_card[makeCardName('T', i)] > max_card_num) {
            max_card_num = my_active_card[makeCardName('T', i)];
            most_card = makeCardName('T', i);
        }
    }
    for (int i = 1; i <= 4; i++) {
        if (my_active_card[makeCardName('F', i)] > max_card_num) {
            max_card_num = my_active_card[makeCardName('F', i)];
            most_card = makeCardName('F', i);
        }
    }
    for (int i = 1; i <= 3; i++) {
        if (my_active_card[makeCardName('J', i)] > max_card_num) {
            max_card_num = my_active_card[makeCardName('J', i)];
            most_card = makeCardName('J', i);
        }
    }
    return most_card;
}


/**
 * 初始化现在的情况
 * @return 当前回合相关牌（摸到的牌，出的牌）
 */
string initCondition() {
    //记录信息的第一个字段：编号
    int record_type;
    //暂存牌名
    string str_tmp;
    str_in.clear();
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
            //记录一下自摸信息，itmp不靠谱
            is_ZIMO = true;
            str_in >> str_tmp;
            all_card[my_player_id].push_back(str_tmp);
            left_card_num[my_player_id]--;
            not_flower_count++;
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
                all_card[4].push_back(str_tmp);
            }
//            else if (str_tmp == "GANG") { //GANG T6 就是暗杠
//                str_in >> str_tmp;
//                for (int k = 0; k < 4; k++) {
//                    all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
//                }
//                //这里先注释掉了 因为对于 gang和mypack的处理 统一放在了读request的部分中
//                //gang.push_back(str_tmp);
//                //my_pack.push_back(makePack("GANG", str_temp, my_player_id));
//            }
        }

            //其他玩家的操作信息比如 3 playerID BUHUA Card1
        else if (record_type == 3) {
            is_ZIMO = false;

            int player_id;
            str_in >> player_id;
            str_in >> str_tmp;
            if (turn_id == i && player_id == my_player_id)
                return "Fail";
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
                //重写peng的牌
                //str_tmp = all_card[4][all_card[4].size() - 1]; //peng的牌
                //如果是自己，就需要存peng的情况
                if (player_id == my_player_id) {
                    //存peng
                    //peng.push_back(str_tmp);
                    //碰完打出的牌要删掉
                    string da_tmp;
                    str_in >> da_tmp;
                    all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), da_tmp));
                    all_card[4].push_back(da_tmp);
                    //改变输入流 读上一个request
                    str_in.clear();
                    //3 player play card1
                    //3 player peng card1
                    //3 palyer chi card1 card2 三种情况
                    str_in.str(request[i - 1]);
                    int temp_playerid;
                    str_in >> temp_playerid >> temp_playerid;
                    str_in >> str_tmp;
                    if (str_tmp == "CHI") {
                        str_in >> str_tmp >> str_tmp;
                    }
                    else
                        str_in >> str_tmp;
                    peng.push_back(str_tmp);
                    my_pack.push_back(makePack("PENG", str_tmp, temp_playerid));
                    //all_card中删掉这三张牌
                    for (int k = 0; k < 2; k++) {
                        all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
                    }
                    //map是最后一起生成的，这里不需要做
                }
                else {
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
                //中间牌card1
                str_in >> str_tmp;
                if (my_player_id == player_id) {
                    //chi里面调整
                    chi.push_back(str_tmp);

                    //之前漏写的，吃完要打掉一张牌
                    string da_tmp;
                    str_in >> da_tmp;
                    all_card[player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), da_tmp));
                    all_card[4].push_back(da_tmp);
                    //3 0 PLAY W1 request
                    //改变输入流 读上一个request
                    int Chi_position = 0;
                    string str_temp2;
                    str_in.clear();
                    str_in.str(request[i - 1]);
                    str_in >> str_temp2 >> str_temp2 >> str_temp2;
                    //CHI操作打出的牌是第五个string
                    if (str_temp2 == "CHI") {
                        str_in >> str_temp2 >> str_temp2;
                    }
                        //其他操作 打出的牌是第四个string
                    else
                        str_in >> str_temp2;
                    //str_out << str_temp2 << str_tmp << " ";
                    if (str_temp2 == previousCard(str_tmp)) {
                        Chi_position = 1;
                    }
                    else if (str_temp2 == postCard(str_tmp)) {
                        Chi_position = 3;
                    }
                    else if (str_temp2 == str_tmp) {
                        Chi_position = 2;
                    }
                    my_pack.push_back(makePack("CHI", str_tmp, Chi_position));
                    all_card[player_id].push_back(str_temp2);
                    //str_out << Chi_position;
                    //str_out << postCard(str_tmp) << "\n";
                    //hand里面调整
                    all_card[player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), previousCard(str_tmp)));
                    all_card[player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
                    all_card[player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), postCard(str_tmp)));
                }
                else {
                    all_card[player_id].push_back(previousCard(str_tmp));
                    all_card[player_id].push_back(str_tmp);
                    all_card[player_id].push_back(postCard(str_tmp));

                    //并且打出一张牌
                    string datmp;
                    str_in >> datmp;
                    all_card[4].push_back(datmp);
                    if (i == turn_id) {
                        return datmp;
                    }
                }
            }

                //处理GANG操作 3 playerID GANG
            else if (str_tmp == "GANG") {
                //需要判断明杠还是暗杠,通过检查GANG前上一次的情况判定
                str_in.clear();
                str_in.str(request[i - 1]);
                str_in >> record_type;

                //暗杠的情况 上回合是自己摸排
                if (record_type == 2) {
                    //2 Card1
                    if (player_id == my_player_id) {
                        str_in >> str_tmp;
                        for (int k = 0; k < 4; k++) {
                            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
                        }
                        gang.push_back(str_tmp);
                        my_pack.push_back(makePack("GANG", str_tmp, my_player_id));
                    }
                    /*if (prePlayerid == player_id) {

                        这里是改之前的原代码
                        str_in >> str_tmp;
                        if (str_tmp == "DRAW") {
                            //todo 暗摸的情况，暂时没想好怎么办
                            not_flower_count++;
                        }
                    }*/
                }
                    //明杠的情况 上回合非自己摸牌
                else {
                    int prePlayerid;
                    str_in >> prePlayerid;
                    str_tmp = all_card[4][all_card[4].size() - 1]; //gang的牌
                    //这里改掉，不用all_card读gang的牌而是读上一回合
                    if (my_player_id == player_id) {
                        all_card[4].pop_back();
                        for (int k = 0; k < 3; k++) {
                            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), str_tmp));
                        }
                        gang.push_back(str_tmp);
                        my_pack.push_back(makePack("GANG", str_tmp, prePlayerid));
                    }
                    else {
                        for (int k = 0; k < 4; k++) {
                            all_card[player_id].push_back(str_tmp);
                        }
                        all_card[4].pop_back();
                        //gang完以后的摸牌操作看不到
                    }
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
                    if (find(my_pack.begin(), my_pack.end(), makePack("PENG", str_tmp, 0)) != my_pack.end()) {
                        auto _itr = find(my_pack.begin(), my_pack.end(), makePack("PENG", str_tmp, 0));
                        int _player_id = _itr->second.second;
                        my_pack.erase(_itr);
                        my_pack.push_back(makePack("GANG", str_tmp, _player_id));
                    }
                    else if (find(my_pack.begin(), my_pack.end(), makePack("PENG", str_tmp, 1)) != my_pack.end()) {
                        my_pack.erase(find(my_pack.begin(), my_pack.end(), makePack("PENG", str_tmp, 1)));
                        my_pack.push_back(makePack("GANG", str_tmp, my_player_id));
                    }
                    else if (find(my_pack.begin(), my_pack.end(), makePack("PENG", str_tmp, 2)) != my_pack.end()) {
                        my_pack.erase(find(my_pack.begin(), my_pack.end(), makePack("PENG", str_tmp, 2)));
                        my_pack.push_back(makePack("GANG", str_tmp, my_player_id));
                    }
                    else if (find(my_pack.begin(), my_pack.end(), makePack("PENG", str_tmp, 3)) != my_pack.end()) {
                        my_pack.erase(find(my_pack.begin(), my_pack.end(), makePack("PENG", str_tmp, 3)));
                        my_pack.push_back(makePack("GANG", str_tmp, my_player_id));
                    }
                }
                else {
                    all_card[player_id].push_back(str_tmp);
                    if (i == turn_id)
                        return str_tmp;
                }
            }
            else if (str_tmp == "DRAW") {
                //处理摸牌情况，记录排山的减少
                left_card_num[player_id]--;
            }
            //else if (str_tmp == "BUHUA" && player_id == my_player_id) {
            //flower_count++;
            //}
        }
    }
    return "Fail";
}

/**
 * 将vector转换为map
 * @param start
 * @param end
 * @return
 */
map<string, int> easy_make_map(vector<string>::iterator start, vector<string>::iterator end) {
    map<string, int> tmp_map;
    for (auto it = start; it != end; it++) {
        if (tmp_map.find(*it) == tmp_map.end()) {
            tmp_map[*it] = 1;
        }
        else {
            tmp_map[*it] += 1;
        }
    }
    return tmp_map;
}

//是不是相邻的牌。如B4,B5返回true，B3,B5分会false
bool adjacent_card(string card1, string card2) {
    if (card1[0] == card2[0] && (card2[1] - card1[1] == 1 || card2[1] - card1[1] == -1))
        return true;
    return false;
}

//去除无效的排，即不可能拿到的双排
string eraseVoid() {
    setMyCard(my_player_id);
    for (int i = 1; i <= 4; i++) {
        if (my_active_card[makeCardName('F', i)] == 2 && my_active_card[makeCardName('F', i)] + getCardNumAll(makeCardName('F', i)) == 4) {
            return makeCardName('F', i);
        }
    }
    for (int i = 1; i <= 3; i++) {
        if (my_active_card[makeCardName('J', i)] == 2 && my_active_card[makeCardName('J', i)] + getCardNumAll(makeCardName('J', i)) == 4) {
            return makeCardName('J', i);
        }
    }
    int i = 1, j = 9;
    for (; i <= j; i++, j--) {
        if (my_active_card[makeCardName('W', i)] == 2 && my_active_card[makeCardName('W', i)] + getCardNumAll(makeCardName('W', i)) == 4) {
            return makeCardName('W', i);
        }
        if (j != i && my_active_card[makeCardName('W', j)] == 2 && my_active_card[makeCardName('W', j)] + getCardNumAll(makeCardName('W', j)) == 4) {
            return makeCardName('W', j);
        }
        if (my_active_card[makeCardName('B', i)] == 2 && my_active_card[makeCardName('B', i)] + getCardNumAll(makeCardName('B', i)) == 4) {
            return makeCardName('B', i);
        }
        if (j != i && my_active_card[makeCardName('B', j)] == 2 && my_active_card[makeCardName('B', j)] + getCardNumAll(makeCardName('B', j)) == 4) {
            return makeCardName('B', j);
        }
        if (my_active_card[makeCardName('T', i)] == 2 && my_active_card[makeCardName('T', i)] + getCardNumAll(makeCardName('T', i)) == 4) {
            return makeCardName('T', i);
        }
        if (j != i && my_active_card[makeCardName('T', j)] == 2 && my_active_card[makeCardName('T', j)] + getCardNumAll(makeCardName('T', j)) == 4) {
            return makeCardName('T', j);
        }
    }

    return "Fail";
}

//删除两张的牌
string eraseTwo() {
    vector<string> v;
    for (int i = 1; i <= 9; i++) {
        if (my_active_card[makeCardName('W', i)] > 0 && my_active_card[makeCardName('W', i)] < 3) {
            v.push_back(makeCardName('W', i));
        }
        if (my_active_card[makeCardName('B', i)] > 0 && my_active_card[makeCardName('B', i)] < 3) {
            v.push_back(makeCardName('B', i));
        }
        if (my_active_card[makeCardName('T', i)] > 0 && my_active_card[makeCardName('T', i)] < 3) {
            v.push_back(makeCardName('T', i));
        }
    }
    if (v.size() == 0)
        return "Fail";
    random_shuffle(v.begin(), v.end());
    return v[0];
}


/**
 * 判断是否进碰碰胡
 * @param sscore 参数
 * @return bool 是否进入
 */
bool ifpeng(int sscore) {
    //第一次ppp
    int score = 0;
    int total = 0;
    for (auto it = my_active_card.begin(); it != my_active_card.end(); it++) {
        total += it->second;
        if (it->second > 1) {
            score += it->second - 1;
        }
    }
    return score >= sscore * total / 14 && chi.empty();
}

char hunyise_main_huase() {
    int tiao = 0, bin = 0, wan = 0;
    for (auto it = my_active_card.begin(); it != my_active_card.end(); it++) {
        if (it->first[0] == 'W') wan++;
        if (it->first[0] == 'B') bin++;
        if (it->first[0] == 'T') tiao++;
    }
    //之前漏写了，要算所有的pengchigang
    for (auto it = peng.begin(); it != peng.end(); it++) {
        if ((*it)[0] == 'W') wan += 3;
        if ((*it)[0] == 'B') bin += 3;
        if ((*it)[0] == 'T') tiao += 3;
    }
    for (auto it = chi.begin(); it != chi.end(); it++) {
        if ((*it)[0] == 'W') wan += 3;
        if ((*it)[0] == 'B') bin += 3;
        if ((*it)[0] == 'T') tiao += 3;
    }
    for (auto it = gang.begin(); it != gang.end(); it++) {
        if ((*it)[0] == 'W') wan += 3;
        if ((*it)[0] == 'B') bin += 3;
        if ((*it)[0] == 'T') tiao += 3;
    }
    char max = 'n';
    int maxnum = 0;
    if (tiao >= maxnum) {
        max = 'T';
        maxnum = tiao;
    }
    if (bin >= maxnum) {
        max = 'B';
        maxnum = bin;
    }
    if (wan >= maxnum) {
        max = 'W';
        maxnum = wan;
    }
    return max;
}

/*
 *判断是否进入混一色策略的函数
 *第一个参数是主花色
 *第二个参数对目前主花色的牌数量的要求数量
 *第三个参数是要求有字牌的数量 取 0,1,2
*/
bool ifhunyise( int tongse_num) {
    int tiao = 0, bin = 0, wan = 0;
    for (auto it = my_active_card.begin(); it != my_active_card.end(); it++) {
        if (it->first[0] == 'W') wan+=it->second;
        if (it->first[0] == 'B') bin+=it->second;
        if (it->first[0] == 'T') tiao+=it->second;
    }
    //之前漏写了，要算所有的pengchigang
    for (auto it = peng.begin(); it != peng.end(); it++) {
        if ((*it)[0] == 'W') wan += 3;
        if ((*it)[0] == 'B') bin += 3;
        if ((*it)[0] == 'T') tiao += 3;
    }
    for (auto it = chi.begin(); it != chi.end(); it++) {
        if ((*it)[0] == 'W') wan += 3;
        if ((*it)[0] == 'B') bin += 3;
        if ((*it)[0] == 'T') tiao += 3;
    }
    for (auto it = gang.begin(); it != gang.end(); it++) {
        if ((*it)[0] == 'W') wan += 3;
        if ((*it)[0] == 'B') bin += 3;
        if ((*it)[0] == 'T') tiao += 3;
    }
    char max = 'n';
    int maxnum = 0;
    if (tiao >= maxnum) {
        max = 'T';
        maxnum = tiao;
    }
    if (bin >= maxnum) {
        max = 'B';
        maxnum = bin;
    }
    if (wan >= maxnum) {
        max = 'W';
        maxnum = wan;
    }
    //return max;

    char huase = max;
    for (auto it = peng.begin(); it != peng.end(); it++) {
        if ((*it)[0] != huase) return false;
    }
    for (auto it = chi.begin(); it != chi.end(); it++) {
        if ((*it)[0] != huase) return false;
    }
    for (auto it = gang.begin(); it != gang.end(); it++) {
        if ((*it)[0] != huase) return false;
    }
    //为什么要有这种要求？
    //int flag = 0;
    //for (int i = 1; i <= 3; i++) {
    //   if (my_active_card[makeCardName('J', i)] >= zipai_num) flag = 1;
    //}
    //for (int i = 1; i <= 4; i++) {
    //   if (my_active_card[makeCardName('F', i)] >= zipai_num) flag = 1;
    //}
    //if (flag == 0) return false;

    int counter = 0;
    for (auto it = my_active_card.begin(); it != my_active_card.end(); it++) {
        if (it->first[0] == huase || it->first[0] == huase == 'F' || it->first[0] == huase == 'J') {
            counter += it->second;
        }
    }

    for (auto it = peng.begin(); it != peng.end(); it++) {
        if ((*it)[0] == huase || (*it)[0] == 'F' || (*it)[0] ==  'J' ) counter += 3;
    }
    for (auto it = chi.begin(); it != chi.end(); it++) {
        if ((*it)[0] == huase || (*it)[0] == 'F' || (*it)[0] == 'J') counter += 3;
    }
    for (auto it = gang.begin(); it != gang.end(); it++) {
        if ((*it)[0] == huase || (*it)[0] == 'F' || (*it)[0] == 'J') counter += 3;
    }

    if (counter >= tongse_num) return true;
    return false;
}

/*
 * 混一色  挑出最适合出牌的牌
 * @参数huase是当前手牌中 主打的花色种类 分别为 'W' 'B' 'T'
 * @return 最优的牌
 */
string hunyise_bestcard(char huase) {
    //先判断我们要留的字牌是什么
    char zipai = 'n';
    for (auto it = my_active_card.begin(); it != my_active_card.end(); it++) {
        if (it->first[0] == 'F' && it->second >= 2) {
            zipai = it->first[0];
            break;
        }
        if (it->first[0] == 'J' && it->second >= 2) {
            zipai = it->first[0];
            break;
        }
    }
    if (zipai == 'n') {
        for (auto it = my_active_card.begin(); it != my_active_card.end(); it++) {
            if (it->first[0] == 'F' && it->second == 1) {
                zipai = it->first[0];
                break;
            }
            if (it->first[0] == 'J' && it->second == 1) {
                zipai = it->first[0];
                break;
            }
        }
    }
    //如果实在没有字牌，默认当成F 但是这种情况策略选择函数就不应该选择混一色
    if (zipai == 'n') zipai = 'F';

    //先删除其他花色单牌
    for (auto it = my_active_card.begin(); it != my_active_card.end(); it++) {
        if (it->first[0] != zipai && it->first[0] != huase && it->second == 1) {
            return it->first;
        }
    }
    //删除其他花色双牌
    for (auto it = my_active_card.begin(); it != my_active_card.end(); it++) {
        if (it->first[0] != zipai && it->first[0] != huase && it->second == 2) {
            return it->first;
        }
    }

    //如果上面都没有返回任何值，说明当前不存在除了主花色和一种字牌外的其他排种
    //（除非有三个一样的刻子，这种情况下可以怀疑我们的策略选择函数有问题，所以还是优先出主花色的单排）
    //从两头向中间除去间隔两个空位的单牌
    string esdouble = eraseDouble();
    if (esdouble != "Fail") {
        return esdouble;
    }
    //从两头向中间除去间隔一个空位的单牌
    string es = eraseSingle();
    if (es != "Fail") {
        return es;
    }

    //扔无效双牌 即不可能凑成刻子 扔一张 留一张用作顺子
    for (int i = 1, j = 9; i <= j; i++, j--) {
        if (my_active_card[makeCardName(huase, i)] == 2 && my_active_card[makeCardName(huase, i)] + getCardNumAll(makeCardName(huase, i)) == 4) {
            return makeCardName(huase, i);
        }
        if (j != i && my_active_card[makeCardName(huase, j)] == 2 && my_active_card[makeCardName(huase, j)] + getCardNumAll(makeCardName(huase, j)) == 4) {
            return makeCardName(huase, j);
        }
    }

    //什么都没有return的时候
    random_shuffle(all_card[my_player_id].begin(), all_card[my_player_id].end());
    auto it_return = all_card[my_player_id].begin();
    while (it_return != all_card[my_player_id].end()) {
        if ((*it_return)[0] != zipai && my_active_card[(*it_return)] != 3) {
            break;
        }
        it_return++;
    }
    if (it_return != all_card[my_player_id].end()) {
        return *it_return;
    }
    else {
        return *(all_card[my_player_id].rbegin());
    }
}

/**
 * 遍历生成一个该颜色的所有活动牌的vector
 * @param color
 * @return
 */
vector<string> getColorActiveCards(char color) {
    vector<string> _v;
    for (auto itr = my_active_card.begin(); itr != my_active_card.end(); ++itr) {
        if (itr->second && itr->first[0] == color) {
            _v.push_back(itr->first);
        }
    }
    return _v;
}

/**
 * 在五门齐的策略下，最佳出牌
 * @return
 */
string WuMenQi_bestcard() {
    //先遍历明group
    vector<pair<string, string>> _groups_inactive; //存放已经鸣牌的组合
    for (int i = 0; i < my_pack.size(); ++i)
        _groups_inactive.push_back(make_pair(my_pack[i].first, my_pack[i].second.first));
    for (int i = 0; i < _groups_inactive.size(); ++i) { //开始遍历明group
        //针对组合的每一种颜色进行考察
        char _color = _groups_inactive[i].second[0]; //当前考察的group的颜色
        string _type = _groups_inactive[i].first; //当前考察的group的种类
        int _number = _groups_inactive[i].second[1] - '0'; //当前考察的group的种类

        vector<string> _color_cards = getColorActiveCards(_color);
        int _color_cards_len = _color_cards.size();
        if (_color_cards_len == 1)
            return _color_cards[0];
        else if (_color_cards_len >= 2) {
            //遍历扔掉不成双的牌
            for (int j = 0; j < _color_cards_len; ++j) {
                bool flag_has_pair = false;
                for (int k = 0; k < _color_cards_len; ++k) {
                    if (k != j && _color_cards[j] == _color_cards[k]) {
                        flag_has_pair = true;
                        break;
                    }
                }
                if (!flag_has_pair) {
                    return _color_cards[j];
                }
            }
        }
    }

    //再遍历暗group，找是否有顺子或刻子 todo 有个问题，一个花色有不止一个group的情况没考虑（这个可能性比较小，暂时不考虑了）
    for (auto my_card_itr = my_active_card.begin(); my_card_itr != my_active_card.end(); ++my_card_itr) {
        string _name = my_card_itr->first;
        char _color = _name[0];
        int _n = _name[1] - '0';

        //如果这是一个碰（或杠）
        if (my_card_itr->second >= 3) {
            if (_color == 'B' || _color == 'w' || _color == 'W') {
                for (int i = 1; i <= 9; ++i) {
                    //这张牌数量是单张
                    string _card = makeCardName(_color, i);
                    if (my_active_card[_card] == 1 && i != _n) {
                        return _card;
                    }
                }
            }
            else if (_color == 'F' || _color == 'J') {
                for (int i = 1; i <= 4; ++i) {
                    //这张牌数量是单张
                    string _card = makeCardName(_color, i);
                    if (my_active_card[_card] == 1 && i != _n) {
                        return _card;
                    }
                }
            }
        }
            //如果是一个顺子
        else if (_color != 'F' && _color != 'J' && _n > 1 && _n < 9 && my_active_card[_name] &&
                 my_active_card[previousCard(_name)] && my_active_card[postCard(_name)]) {
            for (int i = 1; i <= 9; ++i) {
                //这张牌数量是单张
                string _card = makeCardName(_color, i);
                //下面这个if "或者 ||" 后面表示如果是顺子的一部分但是数量大于1
                if (my_active_card[_card] == 1 && i != _n ||
                    abs(i - _n) <= 1 && my_active_card[_card] > 1) {
                    return _card;
                }
            }
        }
    }

    //如果还没有选出，那么扔数量多的花色
    string color_list = "BTWFJ";
    char target_color = 'B';
    int color_cnt_max = 0;
    for (int i = 0; i < 5; ++i) {
        int color_cnt = getColorActiveCards(color_list[i]).size();
        if (color_cnt > color_cnt_max) {
            color_cnt_max = color_cnt;
            target_color = color_list[i];
        }
    }
    vector<string> color_cards_vector = getColorActiveCards(target_color);
    //todo 下面这部分算得很冗余，需要优化
    getRestCard(); //todo 我觉得这个应该有重复调用了
    map<string, int> color_cards = easy_make_map(color_cards_vector.begin(), color_cards_vector.end());
    map<string, int> color_cards_score;
    for (auto color_cards_itr = color_cards.begin(); color_cards_itr != color_cards.end(); ++color_cards_itr) {
        string _name = color_cards_itr->first;
        int score_dist = 0;
        for (auto itr = color_cards.begin(); itr != color_cards.end(); ++itr) {
            if (itr != color_cards_itr) {
                score_dist += abs(itr->first[1] - color_cards_itr->first[1]);
            }
        }
        color_cards_score[_name] = score_dist / color_cards.size() - rest_card[_name] - color_cards_itr->second * 2;
        if (color_cards.count(previousCard(_name)) || color_cards.count(postCard(_name)))
            color_cards_score[_name] -= 2;
    }
    int max_score = -1000;
    string best_card;
    for (auto itr = color_cards_score.begin(); itr != color_cards_score.end(); ++itr) {
        if (itr->second > max_score) {
            max_score = itr->second;
            best_card = itr->first;
        }
    }
    return best_card;
}

/**
 * 五门齐策略下是否允许此颜色的吃碰杠
 * @param color 'W' 注意是char
 * @return 可以鸣牌 true，否则 false
 */
bool checkColorWuMen(char color) {
    //如果已经有过这个颜色了就不能再鸣牌了
    for (int i = 0; i < my_pack.size(); ++i) {
        if (my_pack[i].second.first[0] == color) {
            return false;
        }
    }
    return true;
}

/**
 * 判断此情况下是否选择五门齐
 * @return 选择true 否则false
 */
bool IfWuMenQi() {
    //检查同一个花色的鸣牌，一票否决
    vector<string> group_color_cnt;
    for (int i = 0; i < my_pack.size(); ++i) {
        char c = my_pack[i].second.first[0];
        group_color_cnt.push_back(string(&c));
    }
    map<string, int> res = easy_make_map(group_color_cnt.begin(), group_color_cnt.end());
    for (auto itr = res.begin(); itr != res.end(); ++itr) {
        //一旦有一个鸣牌出现过就两次，就不可能是五门齐
        if (itr->second > 1) {
            return false;
        }
    }

    //记录风和箭的最高数量
    int max_feng_same = 0;
    int max_jian_same = 0;
    //计算风箭相同牌最高出现次数
    vector<string> feng_vector;
    vector<string> jian_vector;
    vector<string> _all_my_card = getAllMyCard();
    for (int i = 0; i < _all_my_card.size(); i++) {
        string name = _all_my_card[i];
        if(name[0] == 'F')
            feng_vector.push_back(name);
        else if(name[0] == 'J')
            jian_vector.push_back(name);
    }

    //生成map计算相同的数量
    map<string, int> feng_cnt_map = easy_make_map(feng_vector.begin(), feng_vector.end());
    map<string, int> jian_cnt_map = easy_make_map(jian_vector.begin(), jian_vector.end());
    for (auto itr = feng_cnt_map.begin(); itr != feng_cnt_map.end(); ++itr) {
        if (max_feng_same < itr->second)
            max_feng_same = itr->second;
    }
    for (auto itr = jian_cnt_map.begin(); itr != jian_cnt_map.end(); ++itr) {
        if (max_jian_same < itr->second)
            max_jian_same = itr->second;
    }

    //计算每种花色的数量
    int f_cnt = 0;
    int j_cnt = 0;
    int t_cnt = 0;
    int w_cnt = 0;
    int b_cnt = 0;
    for (int j = 0; j < _all_my_card.size(); ++j) {
        switch (_all_my_card[j][0]) {
            case 'F':
                f_cnt++; break;
            case 'J':
                j_cnt++; break;
            case 'T':
                t_cnt++; break;
            case 'B':
                b_cnt++; break;
            case 'W':
                w_cnt++; break;
        }
    }

    //下面进行决策
    if (turn_id <= 25)
        //回合数比较少的时候，只要求数字牌每个花色两张，字牌至少各有一张
        return t_cnt >= 2 && w_cnt >= 2 && b_cnt >= 2 && max_feng_same && max_jian_same;
    else if (turn_id <= 50)
        //fj至少有一个要大于等于2
        return (t_cnt >= 2 && w_cnt >= 2 && b_cnt >= 2) && !(t_cnt <= 2 && w_cnt <= 2 && b_cnt <= 2) &&
               (max_feng_same >= 2 || max_jian_same >= 2) && max_feng_same >= 1 && max_jian_same >= 1;
    else
        //回合数中等的时候，要求数字牌每个花色三张，字牌至少各有两张
        return t_cnt >= 3 && w_cnt >= 3 && b_cnt >= 3 && max_feng_same >= 2 && max_jian_same >= 2;
}

string quanqiuren_bestcard() {
    //F,J落单，就直接扔
    string fj = getSingleFengOrJian();
    //存在落单的东南西北中发白
    if (fj != "Fail") {
        return fj;
    }
    //从两头向中间除去间隔两个空位的单牌
    string esdouble = eraseDouble();
    if (esdouble != "Fail") {
        return esdouble;
    }
    //从两头向中间除去间隔一个空位的单牌
    string es = eraseSingle();
    if (es != "Fail") {
        return es;
    }
    //接下来需要形成新的数据结构，去除连续牌数为4、7、10、13中的一张牌，让牌型成为无将胡牌型。如2344条，去除4条。
    //先对我的手牌排序
    //复制我的手牌
    vector<string> my_card(all_card[my_player_id]);
    sort(all_card[my_player_id].begin(), all_card[my_player_id].end());
    vector<string> delete_card;
    //先把手牌中的3张的牌排除,再去除所有的顺子
    int len = all_card[my_player_id].size();
    for (int i = 2; i < len; i++) {
        //如果不是连续的牌
        if (all_card[my_player_id][i - 2] == all_card[my_player_id][i - 1] && all_card[my_player_id][i - 1] == all_card[my_player_id][i]) {
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), all_card[my_player_id][i - 2]));
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), all_card[my_player_id][i - 1]));
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), all_card[my_player_id][i]));
            len = all_card[my_player_id].size();
        }
    }
    len = all_card[my_player_id].size();
    for (int i = 2; i < len; i++) {
        //如果不是顺子
        if (adjacent_card(all_card[my_player_id][i - 2], all_card[my_player_id][i - 1]) && adjacent_card(all_card[my_player_id][i - 1], all_card[my_player_id][i]) && !(i - 3 > 0 && all_card[my_player_id][i - 2] == all_card[my_player_id][i - 3]
                                                                                                                                                                        && i + 1 < len && all_card[my_player_id][i + 1] == all_card[my_player_id][i])) {
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), all_card[my_player_id][i - 2]));
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), all_card[my_player_id][i - 1]));
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), all_card[my_player_id][i]));
            //delete_card.push_back(all_card[my_player_id][i - 2]);
            //delete_card.push_back(all_card[my_player_id][i - 1]);
            //delete_card.push_back(all_card[my_player_id][i]);
            len = all_card[my_player_id].size();
        }
    }
    len = all_card[my_player_id].size();
    //再去掉222
    for (int i = 5; i < len; i++) {
        if (all_card[my_player_id][i - 5] == all_card[my_player_id][i - 4] && adjacent_card(all_card[my_player_id][i - 4], all_card[my_player_id][i - 3])
            && all_card[my_player_id][i - 3] == all_card[my_player_id][i - 2] && adjacent_card(all_card[my_player_id][i - 2], all_card[my_player_id][i - 1]) &&
            all_card[my_player_id][i - 1] == all_card[my_player_id][i]) {
            delete_card.push_back(all_card[my_player_id][i - 5]);
            delete_card.push_back(all_card[my_player_id][i - 4]);
            delete_card.push_back(all_card[my_player_id][i - 3]);
            delete_card.push_back(all_card[my_player_id][i - 2]);
            delete_card.push_back(all_card[my_player_id][i - 1]);
            delete_card.push_back(all_card[my_player_id][i]);
            len = all_card[my_player_id].size();
        }
    }
    for (int i = 0; i < delete_card.size(); i++) {
        all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), delete_card[i]));
    }
    //然后再考虑一遍落单的单牌
    //从两头向中间除去间隔两个空位的单牌
    esdouble = eraseDouble();
    if (esdouble != "Fail") {
        return esdouble;
    }
    //从两头向中间除去间隔一个空位的单牌
    es = eraseSingle();
    if (es != "Fail") {
        return es;
    }

    //两个数据结构，第一个是分别是连续几个连在一起，第二个是到哪个index。比如第一个是1，3，4...，第二个就是1,4,8...
    //每个index是区间的开始
    vector<int> continue_sum_index;
    continue_sum_index.push_back(0);
    //更新长度
    len = all_card[my_player_id].size();
    for (int i = 1; i < len; i++) {
        //如果不是连续的牌
        if (!((all_card[my_player_id][i - 1][0] == all_card[my_player_id][i][0]) && ((all_card[my_player_id][i][1] - all_card[my_player_id][i - 1][1] == 0) || (all_card[my_player_id][i][1] - all_card[my_player_id][i - 1][1] == 1)))) {
            continue_sum_index.push_back(i);
        }
    }
    continue_sum_index.push_back(len);
    int len_index = continue_sum_index.size();
    //等于1因为是求差分所以少一个
    for (int i = 1; i < len; i++) {
        int interval = continue_sum_index[i] - continue_sum_index[i - 1];
        //去除连续牌数为4、7、10、13中的一张牌，让牌型成为无将胡牌型。如2344条，去除4条。
        //四条可能是1111，211，22，22就不处理
        if (interval == 4) {
            //map<string, int> tmp_map = easy_make_map(&(all_card[my_player_id][continue_sum_index[i - 1]]), &(all_card[my_player_id][continue_sum_index[i]]));
            map<string, int> tmp_map;
            for (int j = continue_sum_index[i - 1]; j != continue_sum_index[i]; j++) {
                if (tmp_map.find(all_card[my_player_id][j]) == tmp_map.end()) {
                    tmp_map[all_card[my_player_id][j]] = 1;
                }
                else {
                    tmp_map[all_card[my_player_id][j]] += 1;
                }
            }
            if (tmp_map.size() == 4) {
                return all_card[my_player_id][continue_sum_index[i - 1]];
            }
            else if (tmp_map.size() == 3) {
                for (auto it1 = tmp_map.begin(); it1 != tmp_map.end(); it1++) {
                    if (it1->second == 2) {
                        return it1->first;
                    }
                }
            }
            //22的情况不处理了
        }
        //if (interval == 7) {
        //7只处理2212的情况，前面处理过了
        //}
    }
    sort(my_card.begin(), my_card.end());
    //对于my_card操作，先扔三张牌
    len = my_card.size();
    for (int i = 2; i < len; i++) {
        //如果不是连续的牌
        if (my_card[i - 2] == my_card[i - 1] && my_card[i - 1] == my_card[i]) {
            return my_card[i];
        }
    }
    len = my_card.size();
    //判断顺子
    setMyCard(my_player_id);
    for (int i = 3; i <= 9; i++) {
        //W,B,T
        if (my_active_card[makeCardName('W', i - 2)] == 1 && my_active_card[makeCardName('W', i - 1)] == 1 && my_active_card[makeCardName('W', i)] == 1) {
            if (i == 3)
                return makeCardName('W', i - 2);
            if (i == 9)
                return makeCardName('W', i);
            if (getCardNumAll(makeCardName('W', i - 2)) + getCardNumAll(makeCardName('W', i + 1)) > getCardNumAll(makeCardName('W', i)) + getCardNumAll(makeCardName('W', i - 3)))
                return makeCardName('W', i);
            else
                return makeCardName('W', i - 2);
        }
        if (my_active_card[makeCardName('B', i - 2)] == 1 && my_active_card[makeCardName('B', i - 1)] == 1 && my_active_card[makeCardName('B', i)] == 1) {
            if (i == 3)
                return makeCardName('B', i - 2);
            if (i == 9)
                return makeCardName('B', i);
            if (getCardNumAll(makeCardName('B', i - 2)) + getCardNumAll(makeCardName('B', i + 1)) > getCardNumAll(makeCardName('B', i)) + getCardNumAll(makeCardName('B', i - 3)))
                return makeCardName('B', i);
            else
                return makeCardName('B', i - 2);
        }
        if (my_active_card[makeCardName('T', i - 2)] == 1 && my_active_card[makeCardName('T', i - 1)] == 1 && my_active_card[makeCardName('T', i)] == 1) {
            if (i == 3)
                return makeCardName('T', i - 2);
            if (i == 9)
                return makeCardName('T', i);
            if (getCardNumAll(makeCardName('T', i - 2)) + getCardNumAll(makeCardName('T', i + 1)) > getCardNumAll(makeCardName('T', i)) + getCardNumAll(makeCardName('T', i - 3)))
                return makeCardName('T', i);
            else
                return makeCardName('T', i - 2);
        }
    }
    if (all_card[my_player_id].size() != 0) {
        //什么都没有return的时候
        random_shuffle(all_card[my_player_id].begin(), all_card[my_player_id].end());
        auto it_return = all_card[my_player_id].begin();
        while (it_return != all_card[my_player_id].end()) {
            if ((*it_return)[0] != 'F' && (*it_return)[0] != 'J') {
                break;
            }
            it_return++;
        }
        if (it_return != all_card[my_player_id].end()) {
            return *it_return;
        }
        else {
            return *(all_card[my_player_id].rbegin());
        }
    }


    //什么都没有return的时候
    random_shuffle(my_card.begin(), my_card.end());
    auto it_return = my_card.begin();
    while (it_return != my_card.end()) {
        if ((*it_return)[0] != 'F' && (*it_return)[0] != 'J') {
            break;
        }
        it_return++;
    }
    if (it_return != my_card.end()) {
        return *it_return;
    }
    else {
        return *(my_card.rbegin());
    }
}

//选择策略,true选择ppp，否则qqr
// 1 ppp 2 混一色 3 五门齐 4 qqr

int chooseStrategy() {
    //在这之前应该setMyCard完成了
    //吃不是空时
    int addition = 0;
    if (turn_id > 50) {
        addition = 1;
    }
    else if (turn_id > 100) {
        addition = 2;
    }
    if (ifhunyise(9 + addition)) {
        return STG_HUN_YI_SE;
    }

    if (ifpeng(6)) {
        return STG_PENG_PENG;
    }

    if (IfWuMenQi()) {
        return STG_WU_MEN;
    }

    if (ifhunyise(7 + addition * 2)) {
        return STG_HUN_YI_SE;
    }

    if (ifpeng(4)) {
        return STG_PENG_PENG;
    }

    return STG_QUAN_QIU_REN;
}


/**
 * 挑选一张最适合出牌的牌
 * @return 最优的牌
 */
//true意味着吃完  para = 1则ppp，para = 2 qqr
string getBestCard(int para = 0) {
    setMyCard(my_player_id);
    //如果可以听牌还没写，写在这儿
    string ting = checkTing();
    if (ting != "Fail") {
        return ting;
    }

    /*
    //F,J落单，就直接扔
    string fj = getSingleFengOrJian();
    //存在落单的东南西北中发白
    if (fj != "Fail") {
        return fj;
    }*/
    int strategy = chooseStrategy();
    current_strategy = strategy;
    if (strategy == STG_PENG_PENG) {
        //F,J落单，就直接扔
        string fj = getSingleFengOrJian();
        //存在落单的东南西北中发白
        if (fj != "Fail") {
            return fj;
        }
        //删除单排
        string si = single();
        if (si != "Fail")
            return si;
        //删除没用的双排（虽然有可能送对方胡）
        string ev = eraseVoid();
        if (ev != "Fail")
            return ev;
        //删除双排
        string et = eraseTwo();
        if (et != "Fail")
            return et;
        //其他情况应该没有了，如果有就random吧
        //什么都没有return的时候
        random_shuffle(all_card[my_player_id].begin(), all_card[my_player_id].end());
        auto it_return = all_card[my_player_id].begin();
        while (it_return != all_card[my_player_id].end()) {
            if ((*it_return)[0] != 'F' && (*it_return)[0] != 'J') {
                break;
            }
            it_return++;
        }
        if (it_return != all_card[my_player_id].end()) {
            return *it_return;
        }
        else {
            return *(all_card[my_player_id].rbegin());
        }
    }

    if (strategy == STG_HUN_YI_SE) {
        return hunyise_bestcard(hunyise_main_huase());
    }

    if (strategy == STG_WU_MEN) {
        return WuMenQi_bestcard();
    }

    if (strategy == STG_QUAN_QIU_REN) {
        return quanqiuren_bestcard();
    }
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
    //str_out << "HU";
    //return;
    if (tmpbool && getFan(current_card) >= 8) {
        str_out << "HU";
    }
    else if (left_card_num[(my_player_id + 1) % 4] == 0) {
        //针对复式赛制的牌荒问题，如果发现当前玩家的下家已经没牌了
        str_out << "PASS";
        return;
    }
    else if (tmpbool) {
        //如果胡了但是番数没到
        //str_out << getFan(current_card);
        string best_card_hu = bu_dian_pao();
        str_out << "PLAY " << best_card_hu;
    }
    else {
        //这里是原erase的位置
        //生成一个自己手中牌的map<string,int>
        setMyCard(my_player_id);
        //bugang
        if (find(peng.begin(), peng.end(), stmp) != peng.end()) {
            //这句多余peng.erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), stmp));
            gang.push_back(stmp);
            str_out << "BUGANG " << stmp;
            return;
        }
        //判断能否杠
        if (checkGang(stmp)  && chooseStrategy() != STG_QUAN_QIU_REN) {
            all_card[my_player_id].push_back(stmp);
            str_out << "GANG " << stmp;
            //这里是暗杠，还没处理
            //把手中GANG的牌删掉
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), stmp));
        }
        else {
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
    setMyCard(my_player_id);
    //进这个函数就保证是可以操作的，而不是只能输出PASS
    str_in.clear();
    str_in.str(request[turn_id]);
    str_in >> itmp >> itmp;
    if (itmp != my_player_id) {
        //str_in >> stmp >> stmp;
        //一下是调试代码
        //if (checkHu(all_card[my_player_id], stmp)) {
        //	str_out << "HU";
        //str_out << getFan(current_card);
        //	return;
        //}
        //以上是调试代码
        //if (checkHu(all_card[my_player_id], stmp)) {
        //	str_out << "PASS";
        //	return;
        //}

        //先计算一下策略
        int strategy = chooseStrategy();
        current_strategy = strategy;

        //如果可以抢牌胡
        if (checkHu(all_card[my_player_id], stmp) && getFan(stmp) >= 8) {
            str_out << "HU";
        }
        else if (checkHu(all_card[my_player_id], stmp)) {
            str_out << "PASS";
        }
        else if (left_card_num[(itmp + 1) % 4] == 0) {
            //针对复式赛制的牌荒问题，如果发现当前玩家的下家已经没牌了
            str_out << "PASS";
            return;
        }
            //可以抢牌杠
        else if (checkGang(stmp) && !(strategy == STG_WU_MEN && !checkColorWuMen(stmp[0]))) {
            str_out << "GANG";
            gang.push_back(stmp);
            all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), stmp));
        }
            //碰完就是否一定成功？ 这里是个问题
            //可以碰
        else if (checkPeng(stmp) &&
                 (strategy == STG_QUAN_QIU_REN ||
                  strategy == STG_PENG_PENG ||
                  (strategy == STG_HUN_YI_SE && (stmp[0] == hunyise_main_huase() || stmp[0] == 'F' || stmp[0] == 'J' )) ||
                  (strategy == STG_WU_MEN && checkColorWuMen(stmp[0])))) {
            str_out << "PENG ";
            //把PENG的牌处理一下
            for (int k = 1; k <= 2; k++)
                all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), stmp));
            peng.push_back(stmp);
            int pre_Playerid;    //上回合出牌的玩家
            str_in.clear();
            str_in.str(request[turn_id - 1]);
            str_in >> pre_Playerid >> pre_Playerid;
            my_pack.push_back(makePack("PENG", stmp, pre_Playerid));
            //更新map
            setMyCard(my_player_id);
            stmp = getBestCard();
            str_out << stmp;
        } else if (itmp == (my_player_id + 3) % 4 && checkChi(stmp) != "Fail") {
            //stmp存了吃的牌的中间的牌
            string value = stmp;

            stmp = checkChi(stmp);
            all_card[my_player_id].push_back(value);//先存进来之后三个一起erase
            //这里新加
            //如果能听牌就吃
            setMyCard(my_player_id);
            if (checkTing() != "Fail" || strategy == STG_QUAN_QIU_REN ||
                (strategy == STG_HUN_YI_SE && (stmp[0] == hunyise_main_huase() || stmp[0] == 'F' || stmp[0] == 'J')) ||
                (strategy == STG_WU_MEN && checkColorWuMen(stmp[0]))) {
                //三个erase
                all_card[my_player_id].erase(
                        find(all_card[my_player_id].begin(), all_card[my_player_id].end(), previousCard(stmp)));
                all_card[my_player_id].erase(find(all_card[my_player_id].begin(), all_card[my_player_id].end(), stmp));
                all_card[my_player_id].erase(
                        find(all_card[my_player_id].begin(), all_card[my_player_id].end(), postCard(stmp)));
                str_out << "CHI " << stmp << " ";
                chi.push_back(stmp);
                stmp = getBestCard();
                str_out << stmp;
            } else {
                str_out << "PASS";
            }
        } else {
            str_out << "PASS";
        }
    }
    else {
        str_out << "PASS";
    }
}

void doBUGANG() {
    //需要删除peng中的，插入gang中的，并且输出bugang信息
}

int main() {
    //初始化all_card 0-3是四个玩家，4是桌上的牌
    for (int i = 0; i < 5; i++) {
        vector<string> vtmp;
        all_card.push_back(vtmp);
    }

    //
    //Json交互的输入（删掉了普通交互）
    Json::Value input_json;
#ifdef _BOTZONE_ONLINE
    cin >> input_json;
#else
    //==========debug============
    Json::Reader reader;
    string myin = string("{\"requests\":[\"0 0 3\",\"1 0 0 0 0 F3 T5 B3 W9 F2 W5 W5 T9 T6 T8 T7 B3 T5\",\"2 F3\",\"3 0 PLAY F2\",\"3 1 DRAW\",\"3 1 PLAY B4\",\"3 2 DRAW\",\"3 2 PLAY J2\",\"3 3 DRAW\",\"3 3 PLAY B9\",\"2 T5\",\"3 0 PLAY T8\",\"3 1 DRAW\",\"3 1 PLAY B8\",\"3 2 CHI B8 F1\",\"3 1 PENG T5\"],\"responses\":[\"PASS\",\"PASS\",\"PLAY F2\",\"PASS\",\"PASS\",\"PASS\",\"PASS\",\"PASS\",\"PASS\",\"PASS\",\"PLAY T8\",\"PASS\",\"PASS\",\"PASS\",\"PASS\"]}");
    reader.parse(myin, input_json);
    //==========debug============
#endif
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
    }
    else { //如果这局itmp是2，stmp就是我在这局摸到的牌，如果这局itmp是3，只有别的玩家出牌了才会返回出的是什么牌，其他都返回“Fail”
        stmp = initCondition();
        //stmp = "Fail";//debug用
        current_card = stmp;
        str_in.clear();
        str_in.str(request[turn_id]);
        str_in >> itmp;
        //for (int i = 0; i < all_card[my_player_id].size(); i++)
        //	str_out << all_card[my_player_id][i] << " ";
        if (itmp == 2) { //到我的时候，这时stmp就是牌的信息
            playCard();
        }
            //3 2 PLAY T1
        else if (itmp == 3) {
            if (stmp != "Fail") {
                //bugang
                //if (find(peng.begin(), peng.end(), stmp) != peng.end()) {
                //	doBUGANG();
                //}
                //else{
                //str_out << stmp << " ";
                //str_out << "PASS";
                responseOutTurn();
                //}
            }
            else
                str_out << "PASS";
        }
        response.push_back(str_out.str());
    }

    outputJSON["response"] = response[turn_id];

    //========debug==========
    string stg;
    if (current_strategy == STG_PENG_PENG) { stg = "碰碰胡";}
    else if (current_strategy == STG_HUN_YI_SE) { stg = "混一色"; }
    else if (current_strategy == STG_QUAN_QIU_REN) { stg = "全求人"; }
    else if (current_strategy == STG_WU_MEN) { stg = "五门齐"; }
    debug_info += stg;
    outputJSON["debug"] = debug_info;
    //========debug==========

    cout << outputJSON << endl;
    return 0;
}


/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 *
 *
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *            佛祖保佑       永不宕机     永无BUG
 *
 *        佛曰:
 *                写字楼里写字间，写字间里程序员；
 *                程序人员写程序，又拿程序换酒钱。
 *                酒醒只在网上坐，酒醉还来网下眠；
 *                酒醉酒醒日复日，网上网下年复年。
 *                但愿老死电脑间，不愿鞠躬老板前；
 *                奔驰宝马贵者趣，公交自行程序员。
 *                别人笑我忒疯癫，我笑自己命太贱；
 *                不见满街漂亮妹，哪个归得程序员？
 */