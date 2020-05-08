#include <iostream>
#include <vector>

using namespace std;

//存明牌组合
class cardGroup {
public:
    string type_; //组合类型: CHI, PENG, GANG
    string card_; //组合牌面
    int info_; //组合信息; 碰、杠时表示上家1、对家2、下家3供牌; 吃时表示第几张是上家供牌,取值123
    cardGroup(const string &type, const string &card, int info) : type_(type), card_(card), info_(info) {}
};



