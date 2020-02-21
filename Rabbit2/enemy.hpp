#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "base.hpp"
#include "beam.hpp"

struct Enemy {
	Rect enemy;
	Texture img;
	bool refl;
	int type;
	int res;
	ld three;
	int hp;
	//type==3 初期の上の値
	//0=何も考えずに方向に撃つ
	//1=プレイヤーを補足して撃つ
	//2=プレイヤーを追いかけながら撃つ。
	//3=分身する
	ld pre, eval;
	//前にビームを打った時
	//位置、反射するか、タイプ、HP、その他の値
	//備考（追尾型の場合速度,分身型の場合分身の下限）
	//属性：そのまま０、捕捉1、追尾2、分身3
	Enemy(Vec2 enem, bool refl, int nume,int hp, int res);
	void update(ld delta);
	void hit(Beam& beam);
	void draw(ld delta);
};//普通の敵

struct Whether {
	Texture main, arm;

};//ボス1

struct Oct {

};//ボス2

struct Orc {

};//ボス3

extern Array<Enemy>enemies[10];

#endif