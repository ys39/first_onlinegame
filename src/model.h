#ifndef MODEL_H_
#define MODEL_H_
#include <time.h>

#include "common.h"
//#include "mysmartphone.h"//0102変更点

class Model {
public:
	Model();
	virtual ~Model();

	void initModelWithScene(Scene *);
	void preAction();
	void stepPlayer(int, input_t *);
	void postAction();
private:
	Scene *scene;
	//MySmartphone *smapho;//0102追加
};

#endif /* MODEL_H_ */
