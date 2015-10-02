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
#include"agent.h"
#include"evaluator.h"
#include "Plan.h"

const int NEWGAME_BTN_ID=102,SELECTMAP_BTN_ID=103,DISPLAY_BTN_ID=104,NEWMAP_BTN_ID=105;
const int NEWMAP_OK_BTN_ID=202,NEWMAP_CANCEL_BTN_ID=203;
const int SELECTMAP_SELECT_ID=302,SELECTMAP_CANCEL_BTN_ID=303;
const int AGENTE_REACTIVO=402,BUSQUEDA_ANCHURA=403,BUSQUEDA_PROFUNDIDAD=404,ESCALADA=405;
const int ERROR_OK_BTN_ID=502;

char life_time_input[1000]="2000",total_runs_input[1000]="10",file_name_input[1000]="agent.map";
char display_step[20]="1",display_time[20]="10";
char life_time_temp[1000],total_runs_temp[1000],file_name_temp[1000];
int tx,ty,tw,th;
float magnification_x,magnification_y;
long long life_time=2000,current_time,total_runs=10,current_run,dirty_degree,consumed_energy;
long long total_dirty_degree,total_consumed_energy;
char time_step_msg[100],action_msg[100],dirty_degree_msg[100],consumed_energy_msg[100];

//nuevas variables
int selected_option=-1;
bool pensado_plan=false;


Environment *env=NULL;
Agent *agent=NULL;
Evaluator *evaluator=NULL;
Environment::ActionType action=Environment::actIDLE;
Plan plan;

ifstream fin;

GLUI *main_glui,*score_glui,*new_map_glui,*select_map_glui,*error_glui;
GLUI_Button /* *new_map_btn, */ *select_map_btn,*new_map_ok_btn,*new_map_cancel_btn;
GLUI_Button *agente_reactivo_btn,*display_btn,*busqueda_anchura_btn,*busqueda_profundidad_btn,*escalada_btn;
GLUI_StaticText *time_step_text,*action_text,*dirty_degree_text,*consumed_energy_text;
GLUI_StaticText *complete_runs_text,*total_score_text,*average_score_text;
GLUI_FileBrowser *map_list;

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
		env->Show(tw,th);
		showScore();
	}
	glFlush();
	glutSwapBuffers();
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
				map_list=new GLUI_FileBrowser(select_map_glui,"",false,SELECTMAP_SELECT_ID,control_cb);
				new_map_cancel_btn=new GLUI_Button(select_map_glui,"Cancel",NEWMAP_CANCEL_BTN_ID,control_cb);
			break;
		case DISPLAY_BTN_ID:
			if(env->DirtAmount()>0){
				long long sleep = atol(display_time);
				for(long long t=atol(display_step);t>0 and current_time<life_time and env->DirtAmount()>0;--t){
				    if (selected_option==0){
					  doOneStep();
					  myGlutDisplay();
					  Sleep(sleep);
				    }
				    else{
                      int a = plan.Siguiente_Accion();
				      action = static_cast<Environment::ActionType> (a);
		              env->AcceptAction(action);
		              evaluator->Eval(action,*env);
		              ++current_time;
					  myGlutDisplay();
					  Sleep(sleep);
				    }
				}
			}
			if(env->DirtAmount()==0){
 			   agente_reactivo_btn->enable();
			   display_btn->disable();
			   busqueda_anchura_btn->enable();
			   busqueda_profundidad_btn->enable();
               escalada_btn->enable();
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
			select_map_btn->enable();
			select_map_glui->close();
			break;
		case SELECTMAP_CANCEL_BTN_ID:
			select_map_btn->enable();
			select_map_glui->close();
			break;
		case AGENTE_REACTIVO:
			ReactivoSeleccionado();
			break;
		case BUSQUEDA_ANCHURA:
			AnchuraSeleccionado();
			break;
		case BUSQUEDA_PROFUNDIDAD:
			ProfundidadSeleccionado();
			break;
		case ESCALADA:
			EscaladaSeleccionado();
			break;
		case ERROR_OK_BTN_ID:
			error_glui->close();
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

	int main_window=glutCreateWindow("PRACTICA2: Busqueda");
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
		new GLUI_Column(score_glui,false);
		new GLUI_Column(score_glui,false);
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		complete_runs_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		total_score_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
		average_score_text = new GLUI_StaticText(score_glui,"");
		new GLUI_StaticText(score_glui,"");//以空白行做分隔
	score_glui->set_main_gfx_window( main_window );
	main_glui=GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_RIGHT);

// Botones

		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		select_map_btn=new GLUI_Button(main_glui,"SelectMap",SELECTMAP_BTN_ID,control_cb);
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		agente_reactivo_btn=new GLUI_Button(main_glui,"A.Reactivo",AGENTE_REACTIVO,control_cb);
		agente_reactivo_btn->disable();
		busqueda_anchura_btn=new GLUI_Button(main_glui,"B.Anchura",BUSQUEDA_ANCHURA,control_cb);
		busqueda_anchura_btn->disable();
		busqueda_profundidad_btn=new GLUI_Button(main_glui,"B.Profundidad",BUSQUEDA_PROFUNDIDAD,control_cb);
		busqueda_profundidad_btn->disable();
		escalada_btn=new GLUI_Button(main_glui,"Escalada",ESCALADA,control_cb);
		escalada_btn->disable();
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"DisplayOption");//以空白行做分隔
		new GLUI_EditText(main_glui,"Pasos:",display_step);
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		display_btn=new GLUI_Button(main_glui,"Display",DISPLAY_BTN_ID,control_cb);
		display_btn->disable();
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_StaticText(main_glui,"");//以空白行做分隔
		new GLUI_Button(main_glui,"Salir",0,(GLUI_Update_CB)exit );//QUIT按鈕結束程式
	main_glui->set_main_gfx_window( main_window );
	glutMainLoop();
	return 0;
}

void GUI::showScore(){
	ostringstream sout;
	sout.str("");
	switch(selected_option){
	   case -1:
	           sout<<"Metodo no seleccionado ";
	           time_step_text->set_text(sout.str().c_str());
	           break;
	   case 0:
	           sout<<"AGENTE REACTIVO"<<"  , paso: "<<current_time;
	           time_step_text->set_text(sout.str().c_str());
	           break;
	   case 1:
	           sout<<"BUSQUEDA ANCHURA"<<"  , paso: "<<current_time;
	           time_step_text->set_text(sout.str().c_str());
	           break;
	   case 2:
	           sout<<"BUSQUEDA PROFUNDIDAD"<<"  , paso: "<<current_time;
	           time_step_text->set_text(sout.str().c_str());
	           break;
	   case 3:
	           sout<<"METODO ESCALADA "<<"  , paso: "<<current_time;
	           time_step_text->set_text(sout.str().c_str());
	           break;
	}

	sout.str("");
	sout<<"Accion "<<ActionStr(action)<<(env->isJustBump()?" Bump!":"");
	action_text->set_text(sout.str().c_str());

	sout.str("");
	sout<<"Suciedad = "<<env->DirtAmount();
	dirty_degree_text->set_text(sout.str().c_str());


	sout.str("");
	sout<<"Energia Consumida =  "<<evaluator->ConsumedEnergy();
	consumed_energy_text->set_text(sout.str().c_str());

	/*展示總分*/
	long long complete_runs=current_run-(current_time!=life_time);
	//The Round has been completed
	sout.str("");
	if (selected_option==-1){
	  sout<<"Longitud del Plan: ??";
	  complete_runs_text->set_text(sout.str().c_str());
	} else
	     if (selected_option==0){
	       sout<<"Longitud del Plan: "<<current_time;
	       complete_runs_text->set_text(sout.str().c_str());
	     }
	     else {
	      sout<<"Longitud del Plan: "<<plan.Get_Longitud_Plan();
	      complete_runs_text->set_text(sout.str().c_str());
	     }

	sout.str("");
	if (selected_option==-1 or selected_option==0){
	  total_score_text->set_text(sout.str().c_str());
	  average_score_text->set_text(sout.str().c_str());
	}
	else{
	  sout<<"Nodos Expandidos: "<<plan.Get_EstadosExpandidos()<<"      Nodos Explorados: "<<plan.Get_EstadosExplorados();
	  total_score_text->set_text(sout.str().c_str());
      sout.str("");
	  sout<<"Energia Consumida: "<<plan.Get_Energia();
	  average_score_text->set_text(sout.str().c_str());
	}

}

void GUI::ReactivoSeleccionado(){
  newGame();
  selected_option=0;
  pensado_plan=true;
  agente_reactivo_btn->disable();
  busqueda_anchura_btn->disable();
  busqueda_profundidad_btn->disable();
  escalada_btn->disable();
  display_btn->enable();
  life_time = 100000;
}


void GUI::AnchuraSeleccionado(){
  newGame();
  selected_option=1;
  agente_reactivo_btn->disable();
  busqueda_anchura_btn->disable();
  busqueda_profundidad_btn->disable();
  escalada_btn->disable();
  cout <<"Pensando ...." <<endl;
  plan = agent->Think(*env, selected_option);
  cout <<"Hecho!!" <<endl;
  display_btn->enable();
  life_time = plan.Get_Longitud_Plan();
  pensado_plan=true;
}


void GUI::ProfundidadSeleccionado(){
  newGame();
  selected_option=2;
  cout <<"Pensando ...." <<endl;
  plan = agent->Think(*env, selected_option);
  cout <<"Hecho!!" <<endl;
  if (plan.Get_Longitud_Plan()>0){
    agente_reactivo_btn->disable();
    busqueda_anchura_btn->disable();
    busqueda_profundidad_btn->disable();
    escalada_btn->disable();
    display_btn->enable();
    life_time = plan.Get_Longitud_Plan();
    pensado_plan=true;
  }
}


void GUI::EscaladaSeleccionado(){
  newGame();
  selected_option=3;
  cout <<"Pensando ...." <<endl;
  plan = agent->Think(*env, selected_option);
  cout <<"Hecho!!" <<endl;
  if (plan.Get_Longitud_Plan()>0){
    agente_reactivo_btn->disable();
    busqueda_anchura_btn->disable();
    busqueda_profundidad_btn->disable();
    escalada_btn->disable();
    display_btn->enable();
    life_time = plan.Get_Longitud_Plan();
    pensado_plan=true;
  }
}


void GUI::doOneStep(){

	if(current_time<life_time and env->DirtAmount()>0){
		env->Change();

		agent->Perceive(*env);

		action = agent->AgenteReactivo();
		env->AcceptAction(action);
		evaluator->Eval(action,*env);

		dirty_degree=evaluator->DirtyDegree();
		consumed_energy=evaluator->ConsumedEnergy();

		++current_time;
		if(current_time==life_time or dirty_degree==0){//一輪結束
			agente_reactivo_btn->enable();
			display_btn->enable();
			busqueda_anchura_btn->enable();
			busqueda_profundidad_btn->enable();
            escalada_btn->enable();

			total_dirty_degree+=dirty_degree;
			total_consumed_energy+=consumed_energy;

		}
	}
}


void GUI::newGame(){
	fin.close();
	fin.clear();
	delete env;
	delete agent;
	delete evaluator;

	fin.open(file_name_input);
	if(fin){
		current_time=0;
		env=new Environment(fin);
		evaluator=new Evaluator(env);
		agent=new Agent();
        dirty_degree = evaluator->DirtyDegree();
        consumed_energy = evaluator->ConsumedEnergy();

		agente_reactivo_btn->enable();
		display_btn->disable();
		busqueda_profundidad_btn->enable();
		busqueda_anchura_btn->enable();
		escalada_btn->enable();
	}
	else{
		fin.clear();
		char temp[1000]={"map/"};
		strcat(temp,file_name_input);
		fin.open(temp);
		if(fin){
			current_time=0;
			env=new Environment(fin);
			evaluator=new Evaluator(env);
			agent=new Agent();
            dirty_degree = evaluator->DirtyDegree();
            consumed_energy = evaluator->ConsumedEnergy();


			agente_reactivo_btn->enable();
			display_btn->disable();
		    busqueda_profundidad_btn->enable();
			busqueda_anchura_btn->enable();
			escalada_btn->enable();
		}
		else{
			agente_reactivo_btn->disable();
			display_btn->disable();
		    busqueda_profundidad_btn->disable();
			busqueda_anchura_btn->disable();
			escalada_btn->disable();
			error_glui=GLUI_Master.create_glui("Error",0,100,100);
				char error_message[1000]="File ";
				strcat(error_message,file_name_input);
				strcat(error_message," can't be open");
				new GLUI_StaticText(error_glui,error_message);
				new_map_ok_btn=new GLUI_Button(error_glui,"OK",ERROR_OK_BTN_ID,control_cb);
		}
	}
}




