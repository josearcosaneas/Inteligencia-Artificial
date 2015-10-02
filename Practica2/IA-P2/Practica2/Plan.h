#ifndef PLAN_H
#define PLAN_H

#include <list>

using namespace std;

class Plan
{
    public:
        Plan();
        ~Plan();
        Plan(const Plan& other);
        Plan& operator=(const Plan& other);

        void AnadirPlan(const list<int> &c , int nExpan, int nExplo);
        void Put_Energia(int ene){energiaConsumida=ene;}
        int Get_Energia() const {return energiaConsumida;}
        void Put_EstadosExplorados(int explorados){nodosExplorados=explorados;}
        int Get_EstadosExplorados() const {return nodosExplorados;}
        void Put_EstadosExpandidos(int expandidos){nodosExpandidos=expandidos;}
        int Get_EstadosExpandidos(){return nodosExpandidos;}
        int Get_Longitud_Plan() const {return longitudCamino;}
        void EmpezarPlan(){siguiente=camino.begin();}
        int Siguiente_Accion();
        void Pinta_Plan();

    protected:
    private:
        list<int> camino;
        list<int>::iterator siguiente;
        int energiaConsumida;
        int nodosExplorados;
        int nodosExpandidos;
        int longitudCamino;
};

#endif // PLAN_H
