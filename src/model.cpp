#include <iostream>
#include "model.h"
//#include "smartphone.h"//0102追加
#include "mysmartphone.h"//1228追加
#include "view.h"//1228追加

using namespace std;

Model::Model() {
	// TODO Auto-generated constructor stub
}

Model::~Model() {
	// TODO Auto-generated destructor stub
}

void Model::initModelWithScene(Scene *s){		//初期設定をしている。ユニットの配置やHPなども設定。
//	std::cout << "Init" << std::endl;
	scene=s;
	time_t t;
	t=time(NULL);
	localtime_r(&t, &scene->tm);
	scene->c[0]=0;
	scene->c[1]=0;
	//input->Y[0] =0; input->Z[0]=0;


	for(int i=0; i<max_players; ++i){
		scene->p[i].attend=0;
		scene->p[i].useunit=0;
		scene->p[i].k_flag=0;
		scene->p[i].gameover_flag=0;//20150204追加

			for(int j=0; j<=max_units; ++j){
				scene->p[i].units[j].usebullet = 0;
				scene->p[i].units[j].visible = 0;
				scene->p[i].units[j].wait = 0;
				//std::cout << "Pid:" << i;
				//std::cout << "usebullet:" <<scene->p[0].units[scene->p[0].useunit].usebullet << endl;
			}
				/*scene->p[i].units[0].visible=1;
				scene->p[i].units[0].x=100;
				scene->p[i].units[0].y=100;
				scene->p[i].units[0].rad=0;
				 */
	}
//void makefield(障害物の数, 始点x, 始点y, 終点x, 終点y){	//障害物の関数

	scene->makefield(0,295,295,445,375);//横長い障害物  //x y 5ずつへらした
	scene->makefield(1,295,575,445,655);
	scene->makefield(2,1455,295,1605,375);
	scene->makefield(3,1455,575,1605,655);

	scene->makefield(4,595,45,675,195);//縦長い障害物 //x y 5ずつへらした
	scene->makefield(5,595,755,675,905);
	scene->makefield(6,1225,45,1305,195);
	scene->makefield(7,1225,755,1305,905);

	scene->makefield(8,895,420,985,510);//中心の障害物//5ずつへらした

	scene->makemono(0,925,350,955,400);//アイテム//x 5ずつへらした
	scene->makemono(1,925,540,955,590);//アイテム//x 5ずつへらした

	scene->p[0].makeunit(0, 100, 50, 3.14/2, 1,20);
	scene->p[0].makeunit(1, 50, 100, 3.14/2, 1,20);
	scene->p[1].makeunit(0, 1800, 50, 3.14/2, 1,20);
	scene->p[1].makeunit(1, 1750, 100, 3.14/2, 1,20);
	scene->p[2].makeunit(0, 50, 900,-3.14/2, 1,20);
	scene->p[2].makeunit(1, 100, 950, -3.14/2, 1,20);
//	scene->p[3].makeunit(0, 1750, 950, -3.14/2, 1,20);
//	scene->p[3].makeunit(1, 1800, 900, -3.14/2, 1,20);
	scene->p[0].makecastle(500,100,1,100);
	scene->p[1].makecastle(1375,100,1,100);//x 25へらした
	scene->p[2].makecastle(500,800,1,100);
//	scene->p[3].makecastle(1375,800,1,100);//x 25へらした


}

void Model::preAction(void){
	time_t t;
	t=time(NULL);
	localtime_r(&t, &scene->tm);
}

void Model::postAction(void){//???????????????????????????scene->p[l].units[m].hp -= scene->p[i].units[j].pw;わからない??????????????????????

	for(int i=0; i<max_players; ++i){
		for(int j=0; j<max_units; ++j){
			for(int k=0; k<max_bullet; ++k){
				for(int l=0; l<max_players;++l){
					for(int m=0; m<max_units;++m){//max_players→max_unitsにした
						//if(scene->p[l].attend){
															if(scene->p[i].units[j].shotbullet[k].visible == 1){
																if(scene->p[l].units[m].visible == 1){
																	if(scene->p[i].units[j].shotbullet[k].kind == 0){
																	if(scene->p[i].units[j].shotbullet[k].x >= scene->p[l].units[m].x-13*1.41/2){
																		if(scene->p[i].units[j].shotbullet[k].x <= scene->p[l].units[m].x+13*1.41/2){
																			if(scene->p[i].units[j].shotbullet[k].y >= scene->p[l].units[m].y-13*1.41/2){
																				if(scene->p[i].units[j].shotbullet[k].y <= scene->p[l].units[m].y+13*1.41/2){

																					scene->p[l].units[m].hp -= scene->p[i].units[j].pw;
																					scene->p[i].units[j].shotbullet[k].visible = 0;
																					std::cout << "何かに当たったよ！！"<< std::endl;
																					if(scene->p[l].units[m].hp <= 0){
																						scene->p[l].units[m].visible = 0;
																					std::cout << "何かを倒したよ！！"<< std::endl;
																					scene->p[l].numunit -= 1;
																					if(scene->p[i].units[j].level != 3){
																						scene->p[i].units[j].level += 1;
																						cout << "level up!!" << endl;
																						}
																					//std::cout << "numunit:" <<scene->p[l].numunit << std::endl;
																					}
																				}
																			}
																		}
																	}
																}
																	if(scene->p[i].units[j].shotbullet[k].kind == 1){
																		if(scene->p[i].units[j].shotbullet[k].x >= scene->p[l].units[m].x-19*1.41/2){
																			if(scene->p[i].units[j].shotbullet[k].x <= scene->p[l].units[m].x+19*1.41/2){
																				if(scene->p[i].units[j].shotbullet[k].y >= scene->p[l].units[m].y-19*1.41/2){
																					if(scene->p[i].units[j].shotbullet[k].y <= scene->p[l].units[m].y+19*1.41/2){

																						scene->p[l].units[m].hp -= scene->p[i].units[j].pw*3/2;
																						scene->p[i].units[j].shotbullet[k].visible = 0;
																						std::cout << "何かに当たったよ！！"<< std::endl;
																						if(scene->p[l].units[m].hp <= 0){
																							scene->p[l].units[m].visible = 0;
																						std::cout << "何かを倒したよ！！"<< std::endl;
																						scene->p[l].numunit -= 1;
																						if(scene->p[i].units[j].level != 3){
																						scene->p[i].units[j].level += 1;
																						}
																						//std::cout << "numunit:" <<scene->p[l].numunit << std::endl;
																						}
																					}
																				}
																			}
																		}
																	}
															}
														}
					}
															for(int n(0); n<9; n++){
																if(scene->p[i].units[j].shotbullet[k].visible == 1){
																	if(scene->p[i].units[j].shotbullet[k].x >= scene->Field[n].x){
																		if(scene->p[i].units[j].shotbullet[k].x <= scene->Field[n].x_end){
																			if(scene->p[i].units[j].shotbullet[k].y >= scene->Field[n].y){
																				if(scene->p[i].units[j].shotbullet[k].y <= scene->Field[n].y_end){
																					scene->p[i].units[j].shotbullet[k].visible = 0;//弾が壁に当たって消える。
																				}
																			}
																		}
																	}
																}
															}

				}

			}
			if(scene->p[i].units[j].wait > 0){
				scene->p[i].units[j].wait -= 1;
				//cout << scene->p[i].units[j].wait << endl;
			}

		}
	}

//敵が自分の城に攻撃してhpが0以下になったら30秒後?くらいに出現。
	for(int i=0; i<max_players; ++i){
		for(int j=0; j<max_units; ++j){
			for(int k=0; k<max_bullet; ++k){
				for(int l=0; l<max_players;++l){
					if(scene->p[i].units[j].shotbullet[k].visible == 1){
						if(scene->p[l].Castle.visible == 1){
							if(scene->p[i].units[j].shotbullet[k].x >= scene->p[l].Castle.x-20){
								if(scene->p[i].units[j].shotbullet[k].x <= scene->p[l].Castle.x+20){
									if(scene->p[i].units[j].shotbullet[k].y >= scene->p[l].Castle.y-20){
										if(scene->p[i].units[j].shotbullet[k].y <= scene->p[l].Castle.y+20){

											scene->p[i].units[j].shotbullet[k].visible = 0;
											if(i != l){
												scene->p[l].Castle.hp -= scene->p[i].units[j].pw;
												std::cout << "何かに当たったよ！！"<< std::endl;
												if(scene->p[l].Castle.hp<=0){
													scene->p[l].Castle.visible=0;
													scene->p[l].Castle.x=rand()%1900+1;//				乱数とか…。
													scene->p[l].Castle.y=rand()%950+1;
													scene->p[l].Castle.count=0;
													scene->p[l].Castle.hp=100;//追加
													for(int fieldcount=0; fieldcount<9; ++fieldcount){//	再描画(障害物の中にはいったとき)、追加
														if(scene->p[l].Castle.x >= scene->Field[fieldcount].x+10 && scene->p[l].Castle.y >= scene->Field[fieldcount].y+10
															 && scene->p[l].Castle.x <= scene->Field[fieldcount].x_end+10 && scene->p[l].Castle.y <= scene->Field[fieldcount].y_end+10){
															scene->p[l].Castle.x=rand()%1900+1;
															scene->p[l].Castle.y=rand()%950+1;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	for(int i=0; i<max_players; ++i){
		if(scene->p[i].numunit != 0){
		if((scene->p[i].units[scene->p[i].useunit].x>=scene->p[i].Castle.x-30) && (scene->p[i].units[scene->p[i].useunit].x<=scene->p[i].Castle.x+30)){
			if((scene->p[i].units[scene->p[i].useunit].y>=scene->p[i].Castle.y-30)&& (scene->p[i].units[scene->p[i].useunit].y<=scene->p[i].Castle.y+30)){
				std::cout << "入ったよ☆"<< std::endl;
				if(scene->p[i].numunit<5){
					int l;
					for(int j = 5; j > 0 ; j--){
						if(scene->p[i].units[j-1].visible == 0){
							l = j-1;
						}
					}
					if(i==0)scene->p[i].makeunit(l, 50+(scene->p[i].numunit-1)*30, 50, 3.14/2, 1,20);
					//	次のunitを作る。

					if(i==1)scene->p[i].makeunit(l, 1700+(scene->p[i].numunit-1)*30, 50, 3.14/2, 1,20);


					if(i==2)scene->p[i].makeunit(l, 50+(scene->p[i].numunit-1)*30, 950, -3.14/2, 1,20);


					if(i==3)scene->p[i].makeunit(l, 1700+(scene->p[i].numunit-1)*30, 950, -3.14/2, 1,20);

					scene->p[i].Castle.visible=0;
					scene->p[i].Castle.x=rand()%1900+1;//				乱数とか…。
					scene->p[i].Castle.y=rand()%950+1;
					scene->p[i].Castle.count=600;

					for(int fieldcount=0; fieldcount<9; ++fieldcount){//	再描画(障害物の中にはいったとき)
						if(scene->p[i].Castle.x >= scene->Field[fieldcount].x+10 && scene->p[i].Castle.y >= scene->Field[fieldcount].y+10
							 && scene->p[i].Castle.x <= scene->Field[fieldcount].x_end+10 && scene->p[i].Castle.y <= scene->Field[fieldcount].y_end+10){
							scene->p[i].Castle.x=rand()%1900+1;
							scene->p[i].Castle.y=rand()%950+1;
						}
					}
				}
			}
		}
	}
	}
	//std::cout << "更新中…"<< std::endl;
	for(int i=0; i<max_players; ++i){//アイテム(回復,弾の大きさのところ)
		for(int itemcount(0); itemcount<2; ++itemcount){
			if(scene->p[i].units[scene->p[i].useunit].x >= scene->Mono[itemcount].x){
				if(scene->p[i].units[scene->p[i].useunit].x <= scene->Mono[itemcount].x_end){
					if(scene->p[i].units[scene->p[i].useunit].y >= scene->Mono[itemcount].y){
						if(scene->p[i].units[scene->p[i].useunit].y <= scene->Mono[itemcount].y_end){
							cout << "アイテムゲット" << endl;
								if(itemcount==0){
									scene->p[i].units[scene->p[i].useunit].hp += 10;//(hp=10回復)
								}else if(itemcount==1){
									for(int j=0;j<max_units; j++){
									scene->p[i].units[j].kind = 1;//(弾の種類変えるフラグのところ→view.cppの弾描くところへ)
									}
								}
							scene->Mono[itemcount].visible=0;
							scene->Mono[itemcount].x=rand()%1900+1;//	出現乱数
							scene->Mono[itemcount].y=rand()%950+1;
							scene->Mono[itemcount].x_end=scene->Mono[itemcount].x + 30;
							scene->Mono[itemcount].y_end=scene->Mono[itemcount].y + 50;
							scene->Mono[itemcount].count[0]=0;
							scene->Mono[itemcount].count[1]=0;
							for(int fieldcount=0; fieldcount<9; ++fieldcount){//	再描画(障害物の中にはいったとき)
								if(scene->Mono[itemcount].x >= scene->Field[fieldcount].x+10 && scene->Mono[itemcount].y >= scene->Field[fieldcount].y+10
										&& scene->Mono[itemcount].x_end <= scene->Field[fieldcount].x_end+10 && scene->Mono[itemcount].y_end <= scene->Field[fieldcount].y_end+10){
									scene->Mono[itemcount].x=rand()%1900+1;
									scene->Mono[itemcount].y=rand()%950+1;
									scene->Mono[itemcount].x_end=scene->Mono[itemcount].x + 30;
									scene->Mono[itemcount].y_end=scene->Mono[itemcount].y + 50;
								}
							}
						}
					}
				}
			}
		}
	}
	//*************************************アイテムの出現管理********************************************************************

			if(scene->Mono[0].count[0]<200){//count[0]==200になったら出現
						scene->Mono[0].count[0]++;
						 if(scene->Mono[0].count[0]==200){
							  scene->Mono[0].visible=1;
						 }
					}

			if(scene->Mono[1].count[0]<400){//count[0]==200になったら出現
						scene->Mono[1].count[0]++;
						 if(scene->Mono[1].count[0]==400){
							  scene->Mono[1].visible=1;
						 }
			}


			if(scene->Mono[1].count[1]<400){//count[1]==200になったら弾の大きさに戻る
				scene->Mono[1].count[1]++;
				 if(scene->Mono[1].count[1]==400){
					 for(int i=0; i<max_players; ++i){//すべての味方ユニットに対して
						 for(int j=0; j<max_units; ++j){
							 scene->p[i].units[j].kind = 0;
						 }
					 }
				 }
			}




}



void Model::stepPlayer(int id, input_t *input){			//フレーム毎のゲームの処理

	if(input->N == 3){						//最後の一個を離したときの初期化処理
		if(input->Z[0] == 0){
			input->Z[1] = 0;
			input->Y[1] = 0;
		}
		else if(input->Z[1] == 0){
			input->Z[0] = 0;
			input->Y[0] = 0;
		}

	}

	if(input->Z[0] == 0){					//もしも、左側が離されていたら、移動の処理を止める。
		scene->p[id].ini_Z = 0;
		scene->p[id].ini_Y = 0;
	}
	if(input->Z[1] == 0){					//もしも、右側が離されていたら、攻撃、切り替えを止める。
		scene->p[id].k_flag =0;
		scene->p[id].s_flag =0;
		scene->p[id].units[scene->p[id].useunit].speed = 5;
	}



/*
	cout << "Z[0]=" << input->Z[0] << "   Y[0]=" << input->Y[0] << endl;
	cout << "Z[1]=" << input->Z[1] << "   Y[1]=" << input->Y[1] << endl;
	cout <<endl;
*/

	while(scene->p[id].units[scene->p[id].useunit].visible == 0){

		if(scene->p[id].numunit == 0){
			std::cout << "全滅しますた…。"<< std::endl;
			break;
		}
		scene->p[id].change_use_unit();
	}
//*******************************************************入力の操作************************************
	if(input->key!=0){		//何かキーが押されたとき
		scene->c[0]=input->key;
		cout << input->key << endl;
	}

//*******************************************************ダッシュボタン(β版)＊＊＊＊＊＊＊＊＊＊＊＊＊＊
	if(input->Y[1]>0.66 && input->Y[1]<1.0 && input->Z[1]>0.5 && input->Z[1]<0.66){   //dの位置が押された場合
		scene->p[id].units[scene->p[id].useunit].speed = 10;
	}


//*******************************************************切り替えの操作*********************************
	if(input->Y[1]>0.33 && input->Y[1]<0.66 && input->Z[1]>0.66 && input->Z[1]<0.83 && (scene->p[id].k_flag == 0)){   //kの位置が押された場合
			std::cout << "切り替え中…"<< std::endl;
			if(scene->p[id].k_flag == 0){
			scene->p[id].change_use_unit();		//「操作ユニット切り替え」の関数を呼び出す。
			scene->p[id].k_flag =1;
			}
			std::cout << "useunit;" << scene->p[id].useunit << std::endl;
		}
		/*else if(Y[i]>0.33 && Y[i]<0.66 && Z[i]>0.66 && Z[i]<0.83 &&){
			if(Y[1])
			scene->p[id].k_flag =0;


		}*/
		/*if(input->N == 3 && input->Z[1] == 0){
			scene->p[id].k_flag =0;
		}
		*/
			if(input->key == 107){			//107='k'の入力で発動
			if(scene->p[id].numunit != 0){//
				scene->p[id].change_use_unit();		//「操作ユニット切り替え」の関数を呼び出す。
					}
		}
//***********************************************************弾を発射する操作******************************

				if(input->Y[1]>0 && input->Y[1]<0.33 && input->Z[1]>0.83 && input->Z[1]<1){
					scene->p[id].s_flag =1;
					cout << "shotフラグが1になった。" << endl;
				}

				if(scene->p[id].s_flag == 1 && scene->p[id].units[scene->p[id].useunit].wait==0 && scene->p[id].units[scene->p[id].useunit].level !=3){
						std::cout << "発射★"<< std::endl;
						scene->p[id].units[scene->p[id].useunit].shot(scene->p[id].units[scene->p[id].useunit].rad,scene->p[id].units[scene->p[id].useunit].kind);	//shot関数を呼び出して弾を作成する。
						scene->p[id].units[scene->p[id].useunit].wait=10;
					}
				else if(scene->p[id].s_flag == 1 && scene->p[id].units[scene->p[id].useunit].wait==0 && scene->p[id].units[scene->p[id].useunit].level ==3){
					scene->p[id].units[scene->p[id].useunit].multi_shot(scene->p[id].units[scene->p[id].useunit].rad,scene->p[id].units[scene->p[id].useunit].kind);	//shot関数を呼び出して弾を作成する。
					scene->p[id].units[scene->p[id].useunit].wait=15;
				}
				for(int i=0; i<max_units; ++i){
					if((scene->p[id].useunit != i) && (scene->p[id].units[i].visible == 1) && (scene->p[id].units[i].wait==0) &&(scene->p[id].units[i].level ==1)){
						std::cout << "援護射撃！"<< std::endl;
						scene->p[id].units[i].shot(scene->p[id].units[i].rad,scene->p[id].units[scene->p[id].useunit].kind);	//shot関数を呼び出して弾を作成する。
						scene->p[id].units[i].wait=50;
					}
					else if((scene->p[id].useunit != i) && (scene->p[id].units[i].visible == 1) && (scene->p[id].units[i].wait==0) &&(scene->p[id].units[i].level !=1)){
						std::cout << "援護射撃！"<< std::endl;
						scene->p[id].units[i].multi_shot(scene->p[id].units[i].rad,scene->p[id].units[scene->p[id].useunit].kind);	//shot関数を呼び出して弾を作成する。
						scene->p[id].units[i].wait=50;
						if(scene->p[id].units[scene->p[id].useunit].level != 1){
							scene->p[id].units[i].wait=30;
						}
					}
				}
	//**********************************************************************************************

	//＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ユニットが移動したり当たったりする所　(作成中)*******************************************************


	scene->p[id].units[scene->p[id].useunit].x+=(input->right-input->left)*5;		//ユニットの移動
	scene->p[id].units[scene->p[id].useunit].y+=(input->down-input->up)*5;


	if(scene->p[id].units[scene->p[id].useunit].x < 10){
		scene->p[id].units[scene->p[id].useunit].x = 10;
	}
	if(scene->p[id].units[scene->p[id].useunit].x > 1900){
			scene->p[id].units[scene->p[id].useunit].x = 1900;
		}
	if(scene->p[id].units[scene->p[id].useunit].y < 10){
			scene->p[id].units[scene->p[id].useunit].y = 10;
		}
	if(scene->p[id].units[scene->p[id].useunit].y > 950){
			scene->p[id].units[scene->p[id].useunit].y = 950;
		}

	if(input->right-input->left ==1 && input->down-input->up == 0){
			scene->p[id].units[scene->p[id].useunit].rad = 0;
	}
	if(input->right-input->left ==1 && input->down-input->up == -1){
			scene->p[id].units[scene->p[id].useunit].rad = M_PI/4;
	}	if(input->right-input->left ==0 && input->down-input->up == -1){
	scene->p[id].units[scene->p[id].useunit].rad = M_PI/2;
	}	if(input->right-input->left ==-1 && input->down-input->up == -1){
	scene->p[id].units[scene->p[id].useunit].rad = M_PI*3/4;
	}	if(input->right-input->left ==-1 && input->down-input->up == 0){
	scene->p[id].units[scene->p[id].useunit].rad = M_PI;
	}	if(input->right-input->left ==-1 && input->down-input->up == 1){
	scene->p[id].units[scene->p[id].useunit].rad = M_PI*5/4;
	}	if(input->right-input->left ==0 && input->down-input->up == 1){
	scene->p[id].units[scene->p[id].useunit].rad = M_PI*3/2;
	}	if(input->right-input->left ==1 && input->down-input->up == 1){
	scene->p[id].units[scene->p[id].useunit].rad = M_PI*7/4;
	}
//***************************************移動の操作(スマホ)****

	if((input->Z[0] != 0) && (scene->p[id].ini_Z ==0)){		//最初の入力を格納
		scene->p[id].ini_Y=input->Y[0];
		scene->p[id].ini_Z=input->Z[0];
		cout << "移動開始" << endl;
		//input->Y[0] =0; input->Z[0]=0;
		cout<< "基準点"<< scene->p[id].ini_Y << "　:　" << scene->p[id].ini_Z <<endl;
	//	input->Y[0] =0; input->Z[0]=0;
	}
	if((input->Z[0] != 0) &&  (scene->p[id].ini_Y != 0)){

		cout << "移動値=" << (input->Z[0]-scene->p[id].ini_Z)*5*input->wh_rate << endl;
		cout << "移動値=" << (input->Y[0]-scene->p[id].ini_Y)*5 <<endl;

		scene->p[id].units[scene->p[id].useunit].x+=(input->Z[0]-scene->p[id].ini_Z)*scene->p[id].units[scene->p[id].useunit].speed*input->wh_rate;		//ユニットの移動
		scene->p[id].units[scene->p[id].useunit].y+=(input->Y[0]-scene->p[id].ini_Y)*scene->p[id].units[scene->p[id].useunit].speed;
	//	cout << "移動中" << endl << endl;
		cout<< "Y[0]="<< input->Y[0] << "Z[0]=" << input->Z[0] <<endl;
		if(scene->p[id].s_flag == 0){
			if((input->Z[0]-scene->p[id].ini_Z)>0){
				scene->p[id].units[scene->p[id].useunit].rad = atan((input->Y[0]-scene->p[id].ini_Y)/(input->Z[0]-scene->p[id].ini_Z)*input->wh_rate*-1);
				cout << "右向き" << endl;
			}
			else if((input->Z[0]-scene->p[id].ini_Z) == 0){
				if(input->Y[0] > 0){
				scene->p[id].units[scene->p[id].useunit].rad = 3.141592/2;
				cout << "上向き" << endl;
				}
				else{
				scene->p[id].units[scene->p[id].useunit].rad = 3.141592/2*3;
				cout << "下向き" << endl;
				}
			}
			else{
				scene->p[id].units[scene->p[id].useunit].rad = atan((input->Y[0]-scene->p[id].ini_Y)/(input->Z[0]-scene->p[id].ini_Z)*input->wh_rate*-1)+3.141592;
				cout << "左向き" << endl;
			}
		}
	}
	for(int n(0); n < 9; n++){
	if(scene->p[id].units[scene->p[id].useunit].x+10 >= scene->Field[n].x){
		if(scene->p[id].units[scene->p[id].useunit].x-10 <= scene->Field[n].x_end){
			if(scene->p[id].units[scene->p[id].useunit].y+10 >= scene->Field[n].y){
				if(scene->p[id].units[scene->p[id].useunit].y-10 <= scene->Field[n].y_end){
					cout << "衝突エリア内侵入" << endl;
							scene->p[id].units[scene->p[id].useunit].x -= (input->Z[0]-scene->p[id].ini_Z)*scene->p[id].units[scene->p[id].useunit].speed*input->wh_rate;
							if(scene->p[id].units[scene->p[id].useunit].x+10 >= scene->Field[n].x){
									if(scene->p[id].units[scene->p[id].useunit].x-10 <= scene->Fi/eld[n].x_end){
										if(scene->p[id].units[scene->p[id].useunit].y+10 >= scene->Field[n].y){
											if(scene->p[id].units[scene->p[id].useunit].y-10 <= scene->Field[n].y_end){
												scene->p[id].units[scene->p[id].useunit].x += (input->Z[0]-scene->p[id].ini_Z)*scene->p[id].units[scene->p[id].useunit].speed*input->wh_rate;
												scene->p[id].units[scene->p[id].useunit].y -= (input->Y[0]-scene->p[id].ini_Y)*scene->p[id].units[scene->p[id].useunit].speed;
											}}}}
							}//unitが壁に当たったら一つ前に戻る
				}
			}
		}
	}
//***************************************待機中のユニットが弾を撃つ所(β版)***************************************************




//＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊撃った弾が移動したり当たったり、消えたりする所　(作成中)**************************************************
		for(int i=0; i<max_units; ++i){
			for(int j=0; j<max_bullet; ++j){

				if(scene->p[id].units[i].shotbullet[j].visible == 1){
					scene->p[id].units[i].shotbullet[j].x +=cos(scene->p[id].units[i].shotbullet[j].rad)*5;
					scene->p[id].units[i].shotbullet[j].y -=sin(scene->p[id].units[i].shotbullet[j].rad)*5;
					scene->p[id].units[i].shotbullet[j].hp -= 2;

					if(scene->p[id].units[i].shotbullet[j].hp <= 0){		//弾の寿命の動作。何秒後に消える…など。
						scene->p[id].units[i].shotbullet[j].visible = 0;
					}
					}
			}
		}

//***********************************castleの出現管理************************************************************************

		for(int i=0; i<max_players; ++i){
			if(scene->p[i].Castle.count<1200){
				scene->p[i].Castle.count++;
				if(scene->p[i].Castle.count==1200){
					scene->p[i].Castle.visible=1;
				}
			}
		}

//**********************************全滅のときの<GAME OVER>表示***************************************************************
		for(int i=0; i<max_players; ++i){
			if(scene->p[i].numunit==0){
				scene->p[i].gameover_flag=1;
				scene->p[i].Castle.visible = 0;
			//	cout << "gameover" <<endl;
			}
		}
}
