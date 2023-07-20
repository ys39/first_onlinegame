#include "mysmartphone.h"
#include <iostream>
#include "view.h"
//#include "model.h"
using namespace std;

//float Y[2];
//float Z[2];//最終変更
//int N;
MySmartphone::MySmartphone(int p):Smartphone(p){
}

void MySmartphone::recvBinary(float *array, int n){
	int w(0), h(0);
	float wx,hy;

	switch((int)array[0]){
	case 1: // touch start
	case 2: // touch move
	case 3: // touch end
		cout << (int)array[0] << " ";
		input->N=(int)array[0];
		for(int i=1; i<n; i+=2){
			w=width-(int)array[i];//画面を横にした
			h=(int)array[i+1];//スクリーンサイズわからない//解決
			//cout << "(" << w << "," << h << ") ";
			//cout << "(" << width << "," << height << ") ";
			wx = (float)w/width; hy = (float)h/height;
			input->wh_rate = (float)height/width;
			//cout << "height=" << height << "   width=" << width << endl;
			//cout << "(" << wx << "," << hy << ") ";
			//cout << "(" << (int)array[0] <<  ") ";
			if(i==1){
				if((hy <=0.5) && (hy != 0)){

					input->Y[0]=wx; input->Z[0]=hy;
					cout << "左に代入" << endl;
					input->Y[1]=0.0;input->Z[1]=0.0;
				}
				else if(hy >=0.5){
					input->Y[1]=wx;input->Z[1]=hy;
					cout << "右に代入" << endl;
					input->Y[0]=0.0;input->Z[0]=0.0;
				}
				else{input->Y[1]=0.0;input->Z[1]=0.0;input->Y[0]=0.0;input->Z[0]=0.0;}
			}
			else if(i==3){
				if((input->Z[0] != 0) && (hy>0.5)){
					input->Y[1]=wx;input->Z[1]=hy;
					cout << "右に代入" << endl;
				}
				if((input->Z[1] != 0) && (hy<0.5)){
					input->Y[0]=wx;input->Z[0]=hy;
					cout << "左に代入" << endl;
				}
			}
		//	cout << "mysmartpphone.cpp　：　" <<input->Z[0] << "・" << input->Z[1] << endl;
		//	cout << "mysmartphone : " << &(input->Z[0]) << endl;
			cout << "wh_rate=" <<input->wh_rate << endl;

		}
		cout << endl;
		flush(cout);
//		if(drawingArea && n>1){       //ここのif文をコメントアウトすると丸出なくなる。
//			drawingArea->set_input(w,h);
//		}

		break;
	/*case 4: // accelerometer
		if(drawingArea){
			drawingArea->set_angle(array[1], array[2], array[3]);
		}
		break;*/
	}
}
//void MySmartphone::sendimage(const char filename)

void MySmartphone::onConnect(const char *from, int h, int w){ //スマホから接続があると、背景を設定して、connectを呼び出す。

	sendMessage((char*)"game");//メッセージ

	sendImage("src/button1.png");//button例

	cout << "Connected from " << from << ", screen size="<< w << "x" << h << endl;
}

void MySmartphone::onClose(void){
	cout << "Closed" << endl;
}
