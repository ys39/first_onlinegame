#ifndef COMMON_H_
#define COMMON_H_
#include <gtkmm.h>
#include <time.h>
#include <vector>

const int max_units = 5;
const int max_players = 4;
const int max_bullet = 10;
const int period = 30;
const int max_msglen = 8000;
//1400が初期値
struct input_t {
	int up, down, left, right;		//0か1かをとる入力情報
	int key;
	int N;
	float Y[2];		//スマホの入力情報(縦)
	float Z[2];		//スマホの入力情報(横)
	float wh_rate;
	int s,b;		//0か1をとる入力情報
//float x,y;     //移動するために使う座標
};

class dot {
public:
	float x, y;
	int visible;
	int hp;	//体力
	int pw;	//攻撃力
	int speed;
    int kind;//20150204追加,弾の種類

	float rad;		//ラジアンで向きを表す
	dot(int a=100,int b=100,int c=0,int d=5, float r=0, int k=0){		//コンストラクタ(X Y)
				x=a; y=b; visible=c;speed=d;rad=r;kind=k;
		}
};
//*****************************************************
class bullet : public dot{
public:
};
//******************************************************
class unit : public dot{		//ユニットの情報(画面描写に用いる情報)
public:
	unit(int p=10,int h=10,int ub=0 ,int w=0,int l=1){		//コンストラクタ(体力　攻撃力　usebullet)
			pw=p; hp = h; usebullet=ub; wait=0;level=l;
	}

	//int weapon; //攻撃方法
	bullet shotbullet[max_bullet];
	int usebullet;
	//int numbullet;
	int wait;
	int level;

	void shot(float rad,int kind){
		if(usebullet >= max_bullet){
				usebullet = 0;
			}
		bullet shootbullet;
		shootbullet.x = x+(13.0+kind*6)*cos(rad);		//xはこれでいいのか…？
		shootbullet.y = y-(13.0+kind*6)*sin(rad);
		shootbullet.visible = 1;
		shootbullet.rad = rad;
		shootbullet.hp = 180;
		shootbullet.pw = 5;
		shootbullet.speed = 5;
		if(level == 2){shootbullet.speed = 8;}
		shootbullet.kind = kind;
		shotbullet[usebullet] = shootbullet;
		usebullet += 1;
	}
	void multi_shot(float rad,int kind){
		if(usebullet >= max_bullet){
						usebullet = 0;
					}
				bullet shootbullet;
				shootbullet.x = x+(13.0+kind*6)*cos(rad);
				shootbullet.y = y-(13.0+kind*6)*sin(rad);
				shootbullet.visible = 1;
				shootbullet.rad = rad;
				shootbullet.hp = 120;
				shootbullet.pw = 5;
				shootbullet.speed = 8;
				if(level == 2){shootbullet.speed = 5;}
				shootbullet.kind = kind;
				shotbullet[usebullet] = shootbullet;
				usebullet += 1;
				if(usebullet >= max_bullet){
										usebullet = 0;
									}
				shotbullet[usebullet] = shootbullet;
				shotbullet[usebullet].rad = rad+3.141592*2/9;
				shotbullet[usebullet].x = x+(13.0+kind*6)*cos(rad+3.141592*2/9);
				shotbullet[usebullet].y = y-(13.0+kind*6)*sin(rad+3.141592*2/9);

				usebullet += 1;

				if(usebullet >= max_bullet){
										usebullet = 0;
									}
				shotbullet[usebullet] = shootbullet;
				shotbullet[usebullet].rad = rad-3.141592*2/9;
				shotbullet[usebullet].x = x+(13.0+kind*6)*cos(rad-3.141592*2/9);
				shotbullet[usebullet].y = y-(13.0+kind*6)*sin(rad-3.141592*2/9);
				usebullet += 1;


	}
};

//**********************************************
struct castle{	 //城(拠点)の情報
		int hp;		//体力
		int x, y;//座標
		int visible;
		int count;


/*struct castle{	 //城(拠点)の情報
		short int hp;		//体力


struct castle{	 //城(拠点)の情報
		int hp;		//体力
>>>>>>> refs/remotes/origin/master
		int x, y;	//座標
*/

};

//**********************************************
struct player_t {		//プレイヤーの情報
	int attend;		//参加、非参加の情報
	char name[20];	//名前
	castle Castle;	//城
	unit units[max_units];
	int useunit;	//現在使っている(操作している)ユニットのid
	int numunit;	//現在使っているユニットの総数。
	int s_flag;
	int k_flag;
	int gameover_flag;//2015/02/01 追加
	float ini_Y;	//最初に触った移動用座標×２
	float ini_Z;

	void change_use_unit(){
		useunit +=1;
		while(units[useunit].visible == 0){
		useunit +=1;
		if(useunit >= max_units){
			useunit = 0;
		}
		}
	}
	void makeunit(int n, int x, int y, float r, int v, int h){	//ユニットの初期配置のための関数
		unit test;
		test.x = x;
		test.y = y;
		test.rad = r;
		test.visible = v;
		test.hp = h;
		test.level = 1;
		units[n] = test;
		numunit +=1;
	}

	void makecastle(int x, int y, int v, int h){	//ユニットの初期配置のための関数   //  int t 消してある
			castle test;
			test.x = x;
			test.y = y;
			test.visible = v;
			test.hp = h;
			//test.count = t;
			Castle=test;

		}


};
//**********************************************
struct mono{
	//int kind;	//種類。木箱とか、木とか、動物とか。
	//int hp;		//体力・０になると被破壊。
	int x,y;	//座標
	int x_end, y_end;//座標
	int visible;
	int count[2];//アイテム出現までの時間管理
	mono(int a=1){		//コンストラクタ
		visible=a;
	}

};

//**********************************************
struct item{	//壊せない。入手物。
	int kind;
	int x,y;
};
//**********************************************
struct field{		//フィールド情報
	int x, y;//座標
	int x_end, y_end;//座標
};

//**********************************************

struct message{			//メッセージウィンドウ用
	char c[200];

};
//**********************************************
struct Scene {		//サーバーがクライアントに送っている情報
	char c[2];
	struct tm tm;	//時間らしい
	player_t p[max_players];	//参加人数
	field Field[10];

	void makefield(int i, int x, int y, int x_end, int y_end){	//障害物の関数
			field test;
			test.x = x;
			test.y = y;
			test.x_end = x_end;
			test.y_end = y_end;
			Field[i]=test;
		}

	mono Mono[2];

	void makemono(int i, int x, int y, int x_end, int y_end){	//アイテムの関数
				mono test;
				//test.visible = v;
				test.x = x;
				test.y = y;
				test.x_end = x_end;
				test.y_end = y_end;
				Mono[i]=test;
			}

	//message Message;
};
//**********************************************
extern Gtk::Statusbar *statusBar;
extern int statusId, statusEraseId;
//extern Scene *scene;
extern input_t input[max_players];

gboolean eraseStatusbar(void *p);
gboolean tickServer(void *p);
void process_a_step(Scene *s, input_t *in);

#endif /* COMMON_H_ */
