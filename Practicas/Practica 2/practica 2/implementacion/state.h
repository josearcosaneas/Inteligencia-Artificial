#ifndef STATE_H
#define STATE_H

#include "environment.h"
#include <list>
#include <iostream>

using namespace std;


class state
{
    public:
        state(){Tam_X=Tam_Y=posX=posY=Consumed_Energy=Pending_Dirty=0; mundo=0;last_action=5;g=h=f=0;}
        state(const Environment &env);
        virtual ~state();
        state(const state& other);
        state& operator=(const state& other);
        bool operator<(const state &other) const;
        bool operator>(const state &other) const;
        bool operator==(const state &other) const;
        void Add_Step_in_Road(int new_action);
        void Show_Road();
        list<int> Copy_Road() const {return road;}
        void possible_actions_escalada(bool *act, bool *lo_fue) const;
        void possible_actions(bool *act) const;
        int Generate_New_States(state *V) const;
        int Generate_New_States_escalada(state *V) const;
        bool Is_Solution() const {return Pending_Dirty==0;}
        double Evaluation() const {return f;}
        double Get_h() const {return h;}
        double Get_g() const {return g;}
        void Put_h(double value) {h=value;}
        void Put_f() {f=g+h;}
        void AlterState(const state &other);
        int Last_Action() const {return last_action;}
        void Datos_Finales() const;
        list<state>::iterator Get_Padre() const {return pos_padre;}
        void Put_Padre(list<state>::iterator padre) {pos_padre = padre;}
        void PrintState() const;
        int Get_Pending_Dirty() const {return Pending_Dirty;}
        double Get_f() const {return f;};
        double Suciedad_mas_cercana(int direccion) const;
        int Get_camino() const {return cam;};

        void dibuja() const {for (int i = 0; i < Tam_X; i++) {
                            for (int j = 0; j < Tam_Y; j++)
                                cout << camino[i][j] << "   ";
                                cout << endl;
                                    }};


    protected:
    private:
        int **mundo; // mapa de la habitación
        int **camino;
        int cam, reinicia;
        int posX, posY, Consumed_Energy, Pending_Dirty, Tam_X, Tam_Y;
        int last_action; // última acción realizada
        list<state>::iterator pos_padre;  // Un iterador al estado padre
        double g, h, f; // evaluación del nodo g: costo actual, h: costo hasta un estado objetivo, f=g+h
        list<int> road; // lista que almacena la secuencia de acciones hasta el momento.
};

#endif // STATE_H
