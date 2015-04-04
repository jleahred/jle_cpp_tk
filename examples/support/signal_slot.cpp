#include <iostream>

#include "support/signal_slot.hpp"






//-------------------------
class Prueba : public jle::signal_receptor
{
    int valor;

public:
    Prueba(int _valor=1) : valor(_valor) {}

    void Recibio(void){
        std::cout << std::endl <<  "(" << valor << ")" << "recibio... 111" << std::endl;
    }
    void Recibio2(void){
        std::cout << "recibio 2222..." << std::endl;
    }
    ~Prueba() { valor=-1; }
};



class Prueba2 : public jle::signal_receptor
{
    jle::Signal<> signal;

public:
    Prueba2() {}


    void Connect(void) {
        signal.connect(this, &Prueba2::Recibio);
    };

    void Signal(void) {
        signal();
    }
    void Recibio(void){
        std::cout << std::endl <<  "DESCONECTAMOS_________" << std::endl;
        signal.disconnect(this, &Prueba2::Recibio);
    }

};




int main()
{
    Prueba pr0;

    jle::Signal<> mySignal;

    std::cout << std::endl << "una conexión a uno     Tres conexiones a 2, mandamos un signal" << std::endl;
    mySignal.connect(&pr0, &Prueba::Recibio);
    mySignal.connect(&pr0, &Prueba::Recibio2);
    mySignal.connect(&pr0, &Prueba::Recibio2);
    mySignal.connect(&pr0, &Prueba::Recibio2);
    mySignal.emit();




    std::cout << std::endl << "Desconectamos un 2" << std::endl;
    bool result = mySignal.disconnect(&pr0, &Prueba::Recibio2);
    if (result)
        std::cout << std::endl << "Desconectamos un 2  OK" << std::endl;
    else
        std::cout << std::endl << "Desconectamos un 2  FAIL" << std::endl;
    mySignal.emit();


    std::cout << std::endl << "Desconectamos un 1" << std::endl;
    result = mySignal.disconnect(&pr0, &Prueba::Recibio);
    if (result)
        std::cout << std::endl << "Desconectamos un 1  OK" << std::endl;
    else
        std::cout << std::endl << "Desconectamos un 1  FAIL" << std::endl;
    mySignal.emit();


    std::cout << std::endl << "Desconectamos un 1" << std::endl;
    result = mySignal.disconnect(&pr0, &Prueba::Recibio);
    if (result)
        std::cout << std::endl << "Desconectamos un 1  OK" << std::endl;
    else
        std::cout << std::endl << "Desconectamos un 1  FAIL" << std::endl;
    mySignal();



    std::cout << std::endl << "desconectamos todo... y mandamos un mensaje nuevo";
    mySignal.disconnect_all();
    mySignal();



    //  pendiente la copia de signal
    //  falta meter el countPtr
//    Signal<> mySignal2;
//    mySignal2 = mySignal;

    //  pendiente que se desconecte cuando el receptor salga de ámbito
    //  delicado
    Prueba pr6(22);
    mySignal.connect(&pr6, &Prueba::Recibio);

    std::cout << std::endl << "Creamos otro recpetor temporal, conectamos a 1 mandamo señal";
    {
        Prueba pr1(23);

        mySignal.connect(&pr1, &Prueba::Recibio);
        mySignal();

    }
    std::cout << std::endl << "el últmo receptor temporal sale de ámbito";
    Prueba pr2(24);
    mySignal.connect(&pr2, &Prueba::Recibio);
    std::cout << std::endl << "acceso incorrecto a la memoria";
    std::cout << std::endl << "Sólo se debería escribir 1";
    mySignal();





    //jle::Signal<> mySignal;
    //std::cout << std::endl << "Creamos un temporal 777 que sale de ámbito antes de poner mandar nada";

    {
        Prueba pr1(77);

        mySignal.connect(&pr1, &Prueba::Recibio);
        mySignal.connect(&pr1, &Prueba::Recibio);
        mySignal.emit();

    }
    Prueba pr22(88);
    mySignal.connect(&pr22, &Prueba::Recibio);
    mySignal();

    mySignal.disconnect(&pr22, &Prueba::Recibio);
    mySignal();



    Prueba2 prueba2;
    prueba2.Connect();
    prueba2.Signal();


    {
        //  conectamos dos signal al mismo receptor
        //  sacamos de ámbito a dicho receptor
        //  mandamos una signal
        //  tiene que recibirse
        //  esto generaba un error antes del 5 de julio de 2010

        Prueba pr;
        jle::Signal<>  signalpr1;
        jle::Signal<>  signalpr2;

        signalpr1.connect(&pr, &Prueba::Recibio);
        signalpr2.connect(&pr, &Prueba::Recibio);
        signalpr1.disconnect(&pr, &Prueba::Recibio);
        std::cout << "SE TIENE QUE RECIBIR..."  << std::endl;
        signalpr2.emit();
    }


    return 0;
}
