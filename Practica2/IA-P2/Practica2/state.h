#ifndef STATE_H
#define STATE_H

#include "environment.h"
#include <list>

using namespace std;


class state
{
    public:
        state(){Tam_X=Tam_Y=posX=posY=Consumed_Energy=Pending_Dirty=0; mundo=0;last_action=4;g=h=f=0;orientacion=0;}
        state(const Environment &env);
        virtual ~state();
        state(const state& other);
        state& operator=(const state& other);
        bool operator<(const state &other) const;
        bool operator>(const state &other) const;
        bool operator==(const state &other) const;
        void do_fail(){Consumed_Energy=Pending_Dirty=-1;}
        void Add_Step_in_Road(int new_action);
        void Show_Road();
        list<int> Copy_Road() const {return road;}
        void possible_actions(bool *act) const;
        int Generate_New_States(state *V) const;
        state Generate_Next_State(int i) const;
        bool Is_Solution() const {return Pending_Dirty==0;}
        bool Is_Fail() const {return Pending_Dirty==-1;}
        double Evaluation() const {return f;}
        double Get_h() const {return h;}
        double Get_g() const {return g;}
        int Get_TamX() const {return Tam_X;}
        int Get_TamY() const {return Tam_Y;}
        int Get_X() const {return posX;}
        int Get_Y() const {return posY;}
        int Get_Orientation() const {return orientacion;}
        int SeeRoom(int x, int y) const {return mundo[x][y];}
        void Put_h(double value) {h=value;}
        void Put_f() {f=g+h;}
        void AlterState(const state &other);
        int Last_Action() const {return last_action;}
        void Datos_Finales() const;
        list<state>::iterator Get_Padre() const {return pos_padre;}
        void Put_Padre(list<state>::iterator padre) {pos_padre = padre;}
        void PrintState() const;
        int Get_Pending_Dirty() const {return Pending_Dirty;};
        int Number_of_actions() const {return road.size();}
        double Suciedad_mas_cercana(int direccion) const;



    protected:


    private:
        int **mundo; // mapa de la habitación
        int posX, posY, Consumed_Energy, Pending_Dirty, Tam_X, Tam_Y, orientacion;
        int last_action; // última acción realizada
        list<state>::iterator pos_padre;  // Un iterador al estado padre
        double g, h, f; // evaluación del nodo g: costo actual, h: costo hasta un estado objetivo, f=g+h
        list<int> road; // lista que almacena la secuencia de acciones hasta el momento.
};

#endif // STATE_H
