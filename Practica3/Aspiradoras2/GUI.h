#ifndef GUI_H
#define GUI_H

#include<fstream>
#include"include/GL/glui.h"
namespace GUI{
	void myGlutReshape(int,int);//�ھڵ����j�p�]�w�i���d��
	void myGlutDisplay();//ø��
	void control_cb(int);//�ϧΤ����Ҧ����s����
	int startDraw(int,char *[]);
	void showScore();
	void cerrar();
	void UpdateButton();


	void doOneStep();
	void doOneRun();
	void doAllRun();
	void nextRun();
	void newGame();
	void do_move(int action);
	//bool Juego_Terminado() const;
};
#endif