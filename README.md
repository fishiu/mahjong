# mahjong
botzone 麻将程序对战作业

#### 基本信息

- 所有麻将牌均以“大写字母+数字”组合表示。

- 如：“W4”表示“四万”，“B6”表示“六筒”，“T8”表示“8条”

- “F1”～“F4”表示“东南西北”，“J1”～“J3”表示“中发白”，“H1”～“H8”表示“春夏秋冬梅兰竹菊”

# 初步分工

写几个最基本函数，输入都是手牌，vector \<string> 的格式

```c++
// 所有牌命都用系统的命名
// myCards 手牌
// lastCard 刚才玩家出的牌（注意，不是我的上家出的牌！）
// gotCard 我摸到的牌
returnType funcName (vector<string> myCards, string lastCard, string getCard)
```

- 判断胡牌（不考虑番薯），返回bool
- 判断碰吃杠，返回写的人看着办我也没想好，反正肯定要说明哪几张
- 算番 返回番薯
- 考虑出哪张牌（来不及就搞个随机hhhh）

# 参考资料

## 资料1

[CSDN麻将游戏结构与AI算法](http://blog.sina.com.cn/s/blog_405ee76b0100kp5w.html) 讲述了最基本的架构，算法上提出了三级AI，对于一级AI给出了一些基本策略：

(a) 检查听牌

(b) 去除间隔2个空位的不连续单牌，从两头向中间排查

(c) 去除间隔1个空位的不连续单牌，从两头向中间排查

(d) 去除连续牌数为4、7、10、13中的一张牌，让牌型成为无将胡牌型。如2344条，去除4条。

(e) 去除连续牌数为3、6、9、12中的一张牌，有将则打一吃二成为无将听牌型（如233条，去除3条）；无将则打一成将成为有将胡牌型（如233条，去除2条）。

(f) 去除连续牌数位2、5、8、11中的一张牌，让牌型成为有将听牌型。如23445条，去除5条。

(g) 从将牌中打出一张牌。

## 资料2

[和电脑打国标麻将，一定要注意](https://www.cnblogs.com/suanguade/p/4038132.html)

* 8番起胡，人脑首先会往成型牌上靠，如：

* 6番的：碰碰、三色、五门、全求、混一色

* 然后再加上自摸的1番，缺一门、无字、边张、坎张的一番

这应该是最快的8番起胡方法。

这是第一步，然后自然而然的，人脑会想到：

* 8番的：三色三节高、三色三同顺、花龙。

* 12番的：大于五、小于五。

* 16番的：三色双龙会、青龙。