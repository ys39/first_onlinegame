
//#ifndef MYSMARTPHONE_H//0102追加
//#define MYSMARTPHONE_H//0102追加

#include "smartphone.h"
#include "common.h"
//#include "model.h"//0104追加
//#include "common.h"//0104追加
//extern float Y[2],Z[2];//最終変更
//extern int N;
class MySmartphone : public Smartphone {

public:
	MySmartphone(int p);
	void recvBinary(float *array, int n);
	void onConnect(const char *from, int w, int h);
	void onClose(void);

	//int Y;
	//Scene *scene;
	//input_t *input;//0104追加
};
//#endif /*MYSMARTPHONE_H*///0102追加
