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
	//type==3 �����̏�̒l
	//0=�����l�����ɕ����Ɍ���
	//1=�v���C���[��⑫���Č���
	//2=�v���C���[��ǂ������Ȃ��猂�B
	//3=���g����
	ld pre, eval;
	//�O�Ƀr�[����ł�����
	//�ʒu�A���˂��邩�A�^�C�v�AHP�A���̑��̒l
	//���l�i�ǔ��^�̏ꍇ���x,���g�^�̏ꍇ���g�̉����j
	//�����F���̂܂܂O�A�ߑ�1�A�ǔ�2�A���g3
	Enemy(Vec2 enem, bool refl, int nume,int hp, int res);
	void update(ld delta);
	void hit(Beam& beam);
	void draw(ld delta);
};//���ʂ̓G

struct Whether {
	Texture main, arm;

};//�{�X1

struct Oct {

};//�{�X2

struct Orc {

};//�{�X3

extern Array<Enemy>enemies[10];

#endif