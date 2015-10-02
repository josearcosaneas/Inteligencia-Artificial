#include<fstream>
#include<sstream>
#include<iomanip>
#include<iostream>
#include<windows.h>
using namespace std;
#include"GUI.h"
using namespace GUI;
#include<stdlib.h>
#include"environment.h"
#include "player74740565.h"

const int NEWGAME_BTN_ID=102,SELECTMAP_BTN_ID=103,DISPLAY_BTN_ID=104,NEWMAP_BTN_ID=105;
const int NEWMAP_OK_BTN_ID=202,NEWMAP_CANCEL_BTN_ID=203;
const int SELECTMAP_SELECT_ID=302,SELECTMAP_CANCEL_BTN_ID=303;
const int DO_ONE_STEP_BTN_ID=402,DO_ONE_RUN_BTN_ID=403,NEXT_RUN_BTN_ID=404,DO_ALL_RUN_BTN_ID=405;
const int CURSOR_FORWARD_BTN_ID=500, CURSOR_TURN_LEFT_BTN_ID=501, CURSOR_TURN_RIGHT_BTN_ID=502, CURSOR_SHOOT_BTN_ID=503, REINICIAR_BTN_ID=504;
const int ERROR_OK_BTN_ID=602;

int main_window;
char life_time_input[1000]="1000",total_runs_input[1000]="10",file_name_input[1000]="agent.map";
char display_step[20]="10",display_time[20]="10";
char life_time_temp[1000],total_runs_temp[1000],file_name_temp[1000];
int tx,ty,tw,th;
float magnification_x,magnification_y;
long long life_time=1000,current_time,total_runs=10,current_run,dirty_degree,consumed_energy;
long long total_dirty_degree,total_consumed_energy;
char time_step_msg[100],action_msg[100],dirty_degree_msg[100],consumed_energy_msg[100];


void GUI::cerrar(){

  cout << "Cerrando ventana\n";
}



//int JugadorActivo = 1;
int human1, human2;

Environment *env=NULL;
Player74740565 *player1=NULL, *player274740565=NULL;
//RandomNumberGenerator *rng=NULL;
//Evaluator *evaluator=NULL;
Environment::ActionType action=Environment::actIDLE;
ifstream fin;

GLUI *main_glui,*score_glui,*new_map_glui,*select_map_glui,*error_glui, *cursor_glui;
GLUI_Button *new_map_btn,*select_map_btn,*new_map_ok_btn,*new_map_cancel_btn;
GLUI_Button *do_one_step_btn,*display_btn,*do_one_run_btn,*next_run_btn,*do_all_run_btn;
GLUI_Button *cursor_forward, *cursor_turn_left, *cursor_turn_right, *cursor_shoot;
GLUI_Button *reiniciar_btn;
GLUI_StaticText *time_step_text,*action_text,*dirty_degree_text,*consumed_energy_text;
GLUI_StaticText *complete_runs_text,*total_score_text,*average_score_text, *juega_txt;
GLUI_FileBrowser *map_list;
GLUI_RadioGroup *radio1, *radio2;
GLUI_Panel *obj_panel1, *obj_panel2;

void GUI::myGlutReshape(int w,int h){
	GLUI_Master.get_viewport_area(&tx,&ty,&tw,&th);
	glViewport(tx,ty,tw,th);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(tw<=th){
		magnification_x=1;
		magnification_y=(GLfloat)th/(GLfloat)tw;
	}
	else{
		magnification_x=(GLfloat)tw/(GLfloat)th;
		magnification_y=1;
	}
	glOrtho(-10.0*magnification_x,10.0*magnification_x,-10.0*magnification_y,10.0*magnification_y,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
}



void GUI::myGlutDisplay(){
	glClearColor(200.0/255,200.0/255,200.0/255,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(-50.0,50.0,-50.0,50.0);
	if(env!=NULL){
		showScore();
		env->Show(tw,th);
	}
	glFlush();
	glutSwapBuffers();
}


bool Juego_Terminado(){
  bool act[6];

  env->possible_actions(act);
  int n_act=0;
  for (int i=0; i<5 and n_act==0; i++)
    if (act[i])
      n_act++;



  if (env->Total_Suciedad()==0)
    return true;
  else if (env->Get_Instante()>=1000)
         return true;
       else if ( (env->Marcador(1)>=env->Suciedad_Objetivo()) or (env->Marcador(2)>=env->Suciedad_Objetivo()))
              return true;
            else if (n_act==0)
                    return true;
                 else
                    return false;
}


void GUI::control_cb(int id){
	switch(id){
		case SELECTMAP_BTN_ID:
			//new_map_btn->disable();
			select_map_btn->disable();
			strcpy(life_time_temp,life_time_input);
			strcpy(total_runs_temp,total_runs_input);
			strcpy(file_name_temp,file_name_input);
			select_map_glui=GLUI_Master.create_glui("New Game Setting",0,100,100);
				//new GLUI_EditText(select_map_glui,"Life Time:",life_time_temp);
                obj_panel1 = new GLUI_Panel( select_map_glui, "Jugador Verde" );
                  radio1 = new GLUI_RadioGroup( obj_panel1,&human1,2,control_cb );
                          new GLUI_RadioButton( radio1, "Automatico" );
                          new GLUI_RadioButton( radio1, "Humano" );

                obj_panel2 = new GLUI_Panel( select_map_glui, "Jugador Azul" );
                  radio2 = new GLUI_RadioGroup( obj_panel2,&human2,2,control_cb );
                          new GLUI_RadioButton( radio2, "Automatico" );
                          new GLUI_RadioButton( radio2, "Humano" );
				//new GLUI_EditText(select_map_glui,"Test Case:",total_runs_temp);
				map_list=new GLUI_FileBrowser(select_map_glui,"",false,SELECTMAP_SELECT_ID,control_cb);
				new_map_cancel_btn=new GLUI_Button(select_map_glui,"Cancel",SELECTMAP_CANCEL_BTN_ID,control_cb);
			break;
		case DISPLAY_BTN_ID:
				//int sleep = atoi(display_time);
				for(long long t=atol(display_step);t>0 && !Juego_Terminado();--t){
                  doOneStep();
                  myGlutDisplay();
                  glutPostRedisplay();
                  glutSetWindow(main_window);
                  glutPostRedisplay();
                  Sleep(10);
				}
				if (Juego_Terminado()){
                  do_one_run_btn->disable();
                  do_one_step_btn->disable();
                  cursor_forward->disable();
                  cursor_turn_left->disable();
                  cursor_turn_right->disable();
                  cursor_shoot->disable();
                  display_btn->disable();
				}

			break;
		case NEWMAP_OK_BTN_ID:
			strcpy(life_time_input,life_time_temp);
			strcpy(total_runs_input,total_runs_temp);
			strcpy(file_name_input,file_name_temp);
			life_time=atol(life_time_input);
			total_runs=atol(total_runs_input);
			current_run=1;
			total_dirty_degree=0;
			total_consumed_energy=0;
			newGame();
			//new_map_btn->enable();
			select_map_btn->enable();
			new_map_glui->close();
			break;
		case NEWMAP_CANCEL_BTN_ID:
			//new_map_btn->enable();
			select_map_btn->enable();
            new_map_glui->close();
			break;
		case SELECTMAP_SELECT_ID:
			strcpy(life_time_input,life_time_temp);
			strcpy(total_runs_input,total_runs_temp);
			strcpy(file_name_input,map_list->get_file());
			life_time=atol(life_time_input);
			total_runs=atol(total_runs_input);
			current_run=1;
			total_dirty_degree=0;
			total_consumed_energy=0;
			newGame();
			//new_map_btn->enable();
			select_map_btn->enable();
			select_map_glui->close();
			break;
		case SELECTMAP_CANCEL_BTN_ID:
			//new_map_btn->enable();
			select_map_btn->enable();
			select_map_glui->close();
			break;
		case DO_ONE_STEP_BTN_ID:
			doOneStep();
			break;
		case DO_ONE_RUN_BTN_ID:
			doOneRun();
			break;
        case CURSOR_FORWARD_BTN_ID:
            if ((env->JugadorActivo()==1 and human1==1) or (env->JugadorActivo()==2 and human2==1))
              do_move(0);
            else
              doOneStep();
            break;
        case CURSOR_TURN_LEFT_BTN_ID:
            if ((env->JugadorActivo()==1 and human1==1) or (env->JugadorActivo()==2 and human2==1))
              do_move(1);
            else
              doOneStep();
            break;
        case CURSOR_TURN_RIGHT_BTN_ID:
            if ((env->JugadorActivo()==1 and human1==1) or (env->JugadorActivo()==2 and human2==1))
              do_move(2);
            else
              doOneStep();
            break;
        case CURSOR_SHOOT_BTN_ID:
            if ((env->JugadorActivo()==1 and human1==1) or (env->JugadorActivo()==2 and human2==1))
              do_move(4);
            else
              doOneStep();
            break;
		case ERROR_OK_BTN_ID:
			error_glui->close();
			break;
        case REINICIAR_BTN_ID:
            newGame();
            break;
		default:
			break;
	}
	glutPostRedisplay();
}
int GUI::startDraw(int argc,char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition(50,50);
	glutInitWindowSize(668,693);


    //glutCloseFunc(cerrar);
    //glutWMCloseFunc(cerrar);
    //glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

	main_window=glutCreateWindow("PRACTICA 3");
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	glutDisplayFunc(myGlutDisplay);
		glEnable(GL_DEPTH_TEST);


	score_glui=GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_BOTTOM);
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		time_step_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		action_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		dirty_degree_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		consumed_energy_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		complete_runs_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		total_score_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		average_score_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
	score_glui->set_main_gfx_window( main_window );
	main_glui=GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_RIGHT);
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		//new_map_btn=new GLUI_Button(main_glui,"NewMap",NEWMAP_BTN_ID,control_cb);
		select_map_btn=new GLUI_Button(main_glui,"SelectMap",SELECTMAP_BTN_ID,control_cb);
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		do_one_step_btn=new GLUI_Button(main_glui,"DoOneStep",DO_ONE_STEP_BTN_ID,control_cb);
		do_one_step_btn->disable();
		do_one_run_btn=new GLUI_Button(main_glui,"DoOneRun",DO_ONE_RUN_BTN_ID,control_cb);
		do_one_run_btn->disable();
		//next_run_btn=new GLUI_Button(main_glui,"NextRun",NEXT_RUN_BTN_ID,control_cb);
		//next_run_btn->disable();
		//do_all_run_btn=new GLUI_Button(main_glui,"DoAllRun",DO_ALL_RUN_BTN_ID,control_cb);
		//do_all_run_btn->disable();
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		reiniciar_btn=new GLUI_Button(main_glui,"Reiniciar",REINICIAR_BTN_ID,control_cb);
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"DisplayOption");//以空白行做分隔
		new GLUI_EditText(main_glui,"Steps:",display_step);
		//new GLUI_EditText(main_glui,"Time:",display_time);
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		display_btn=new GLUI_Button(main_glui,"Display",DISPLAY_BTN_ID,control_cb);
		display_btn->disable();
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		juega_txt =new GLUI_StaticText(main_glui,"");
		cursor_forward=new GLUI_Button(main_glui,"FORWARD",CURSOR_FORWARD_BTN_ID,control_cb);
		cursor_forward->disable();
		cursor_turn_left=new GLUI_Button(main_glui,"TURN LEFT",CURSOR_TURN_LEFT_BTN_ID,control_cb);
		cursor_turn_left->disable();
		cursor_turn_right=new GLUI_Button(main_glui,"TURN RIGHT",CURSOR_TURN_RIGHT_BTN_ID,control_cb);
		cursor_turn_right->disable();
		cursor_shoot=new GLUI_Button(main_glui,"SHOOT",CURSOR_SHOOT_BTN_ID,control_cb);
		cursor_shoot->disable();

		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_Button(main_glui,"Quit",0,(GLUI_Update_CB)exit );//QUIT按鈕結束程式
	main_glui->set_main_gfx_window( main_window );
	glutMainLoop();
	return 0;
}




void GUI::showScore(){
	ostringstream sout;

	sout.str("");
	if (env->JugadorActivo()==1)
	  sout<<"Juega VERDE";
    else
      sout << "Juega AZUL";
	juega_txt->set_text(sout.str().c_str());


	sout.str("");
	sout<<"JUGADOR VERDE: " << env->Marcador(1)  << " (" << env->ActionStr( (Environment::ActionType) env->Last_Action(1)) << " )" << endl;
	time_step_text->set_text(sout.str().c_str());

	sout.str("");
	sout<<"JUGADOR AZUL: " << env->Marcador(2)<< " (" << env->ActionStr( (Environment::ActionType) env->Last_Action(2)) << " )" << endl;
	action_text->set_text(sout.str().c_str());

	sout.str("");
	sout<<"";
	dirty_degree_text->set_text(sout.str().c_str());


	sout.str("");
	sout<<"INSTANTE : " << env->Get_Instante() << endl;
	consumed_energy_text->set_text(sout.str().c_str());


	long long complete_runs=current_run-(current_time!=life_time);
	//The Round has been completed
	sout.str("");
	sout<<"SUCIEDAD RESTANTE: " << env->Total_Suciedad();
	complete_runs_text->set_text(sout.str().c_str());

	sout.str("");
	if (Juego_Terminado()){
	    bool act[6];
	    env->possible_actions(act);
        int n_act=0;
        for (int i=0; i<5 and n_act==0; i++)
          if (act[i])
            n_act++;

        if (n_act==0){
            if (env->JugadorActivo()==1)
              sout<<"GANA EL JUGADOR AZUL";
            else
	           sout<<"GANA EL JUGADOR VERDE";
        }
	    else if (env->Marcador(1)>env->Marcador(2))
	           sout<<"GANA EL JUGADOR VERDE";
             else if (env->Marcador(2)>env->Marcador(1))
                    sout<<"GANA EL JUGADOR AZUL";
	              else
	                sout<<"SE HA PRODUCIDO UN EMPATE";
	}
	total_score_text->set_text(sout.str().c_str());

	sout.str("");
    sout<<"SUCIEDAD OBJETIVO: " << env->Suciedad_Objetivo() << endl;
	average_score_text->set_text(sout.str().c_str());


}


void GUI::doOneStep(){
  if (!Juego_Terminado()){


   if (env->JugadorActivo()==1){
	if(human1==0){
		  player1->Perceive(*env);

		  action = player1->Think();
		  env->AcceptAction(action);
		  env->ChangePlayer();
	}
   }
   else {
	if (human2==0){

		  player2->Perceive(*env);

		  action = player2->Think();
		  env->AcceptAction(action);

		  env->ChangePlayer();

	}
   }
  }

   UpdateButton();

}

void GUI::do_move(int accion){
    env->AcceptAction(static_cast<Environment::ActionType> (accion));

	myGlutDisplay();
	Sleep(100);

    env->ChangePlayer();

    UpdateButton();

}


void GUI::doOneRun(){
	do_one_run_btn->disable();

	while(!Juego_Terminado()){
		doOneStep();
	    myGlutDisplay();
	    Sleep(100);
	    //glutMainLoop();

	}

    UpdateButton();
}


void GUI::UpdateButton(){

  if (env==NULL or Juego_Terminado()){
	do_one_step_btn->disable();
    cursor_forward->disable();
    cursor_turn_left->disable();
    cursor_turn_right->disable();
    cursor_shoot->disable();
    display_btn->disable();
  }
  else {
    bool act[6];
	env->possible_actions(act);

	if (act[0])
	  cursor_forward->enable();
    else
	  cursor_forward->disable();

	if (act[1])
	  cursor_turn_left->enable();
    else
	  cursor_turn_left->disable();

	if (act[2])
	  cursor_turn_right->enable();
    else
	  cursor_turn_right->disable();

	if (act[4])
	  cursor_shoot->enable();
    else
	  cursor_shoot->disable();
  }

}

void GUI::newGame(){
	fin.close();
	fin.clear();
	delete env;
	//delete agent1;
	delete player2;
	delete player1;
	//delete rng;
	//delete evaluator;

	fin.open(file_name_input);
	if(fin){
		current_time=0;
		dirty_degree=0;
		consumed_energy=0;
		env=new Environment(fin);
		//rng=new RandomNumberGenerator(env->RandomSeed()+current_run);
		//evaluator=new Evaluator();
		//agent1=new Agent(1);
		player1=new Player(1);
		player2=new Player(2);


        //JugadorActivo=rand()%2+1;

		do_one_step_btn->enable();
		display_btn->enable();
		do_one_run_btn->enable();

	    UpdateButton();


		//do_all_run_btn->enable();
	}
	else{
		fin.clear();
		char temp[1000]={"map/"};
		strcat(temp,file_name_input);
		fin.open(temp);
		if(fin){
			current_time=0;
			dirty_degree=0;
			consumed_energy=0;
			env=new Environment(fin);
			//rng=new RandomNumberGenerator(env->RandomSeed()+current_run);
			//evaluator=new Evaluator();
			//agent1=new Agent(1);
			player1=new Player(1);
			player2=new Player(2);

            //JugadorActivo=rand()%2+1;

			do_one_step_btn->enable();
			display_btn->enable();
			do_one_run_btn->enable();
			//do_all_run_btn->enable();
            UpdateButton();
		}
		else{
			do_one_step_btn->disable();
			display_btn->disable();
			do_one_run_btn->disable();
			//do_all_run_btn->disable();
		    cursor_forward->disable();
		    cursor_turn_left->disable();
		    cursor_turn_right->disable();
		    cursor_shoot->disable();
			error_glui=GLUI_Master.create_glui("Error",0,100,100);
				char error_message[1000]="File ";
				strcat(error_message,file_name_input);
				strcat(error_message," can't be open");
				new GLUI_StaticText(error_glui,error_message);
				new_map_ok_btn=new GLUI_Button(error_glui,"OK",ERROR_OK_BTN_ID,control_cb);
		}
	}
}
