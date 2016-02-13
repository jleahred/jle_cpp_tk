#ifndef ARITHMACHINE_H
#define ARITHMACHINE_H


#include <string>
#include <vector>
#include <stack>
#include <map>


#include "support/signalslot.hpp"
#include "support/mtk_double.h"



/*

    A r i t h M a c h i n e
    ______________________________

        * Trabaja con datos tipo Double
        * Puede trabajar por valor o por referencia
        * Tiene una pila donde se pueden añadir los valores o referencias
        * El montículo se accede por un string con el nombre de la variable
        * Tiene un método para modificar de forma eficiente el montículo
        * Cuando se escriba una referencia en el montículo y esta no exista
          se asignará un espacio automáticamente.
          No es necesaria ni permitida la reserva explícita previa


*/







// -----------------------------------
//      D a t a
// -----------------------------------

struct Data
{
    enum enDataType {  dtValue, dtReference  };

    enDataType      dataType        ;

    mtk::Double     value           ;
    std::string     reference       ;


    Data();
    Data(const mtk::Double& val);
    Data(const std::string& ref);

};




// -----------------------------------
//      I n s t r u c t i o n
// -----------------------------------

enum enOperatorCode {         ocPush
                            , ocFunct1
                            , ocFunct2
                            , ocCopyStackTop2Heap   //  copia el valor top del la pila en el montículo, no saca de la pila
                    };
struct Instruction
{

    enOperatorCode  operatorCode;
    Data            data;


    Instruction(enOperatorCode _opcode, const Data& _data)
        :  operatorCode (_opcode), data(_data)  {};
};

typedef std::vector<Instruction>  t_program;




// -----------------------------------

//      A r i t h M a c h i n e

// -----------------------------------

class ArithMachine  : public mtk::SignalReceptor
{
        typedef     mtk::Signal<
                            const mtk::Double&      /*param 1  */,
                            const mtk::Double&      /*param 2  */,
                            mtk::Double&            /*result   */>
                    t_signalFunction2arity;

        typedef     mtk::Signal<
                            const mtk::Double&      /*param 1  */,
                            mtk::Double&            /*result   */>
                    t_signalFunction1arity;




public:

    void ResetMachine(void);
    void PartialReset(void);        //  no borra el montículo

    //  para añadir instrucciones directamente
    void AddInstruction     (const Instruction& instruction );
    void AddProgram         (const t_program&   _program);

    //  escribir directamente en la memoria
    void WriteValueOnStack  (const std::string& refName, const mtk::Double& val);

    mtk::Double Eval(void);
    void        Run (void);


    mtk::CountPtr<t_signalFunction1arity> RegisterFunction1Arity (const std::string& name);
    mtk::CountPtr<t_signalFunction2arity> RegisterFunction2Arity (const std::string& name);



    //  se permite el acceso directo (externo) a la memoria por rendimiento y sencillez
    mtk::Double GetValueFromHeap(const std::string& refName) const;
    void        SetValueInHeap  (const std::string& refName, const mtk::Double& val);






private:

    std::vector<Instruction>                        program     ;
    std::stack<Data>                                stack       ;
    std::map<std::string, mtk::Double>              heap        ;

    std::map<std::string, mtk::CountPtr<t_signalFunction1arity> >   functions1  ;
    std::map<std::string, mtk::CountPtr<t_signalFunction2arity> >   functions2  ;


    mtk::Double StackTop(void);    //  si es una referencia, la resuelve y devuelve siempre un dato
    mtk::Double StackPop(void);    //  además de devolver el dato, lo elimina de la pila

};








// -----------------------------------
//      F u n c t i o n s _ B a s i c
// -----------------------------------
class Functions_Basic : public mtk::SignalReceptor
{
public:

    Functions_Basic(ArithMachine& am);


private:

    void UnnaryMinus(       const mtk::Double& par,
                            mtk::Double& result);

    void Add        (       const mtk::Double& par1,
                            const mtk::Double& par2,
                            mtk::Double& result);

    void Minus      (       const mtk::Double& par1,
                            const mtk::Double& par2,
                            mtk::Double& result);

    void Multiply   (       const mtk::Double& par1,
                            const mtk::Double& par2,
                            mtk::Double& result);

    void Divide     (       const mtk::Double& par1,
                            const mtk::Double& par2,
                            mtk::Double& result);
};




// ------------------------------------------------

//      A M _ A s e m b l e r


//    * Le pasamos en una cadena de texto, o varias,
//    el programa parcialmente compilado
//
//    * Esta entrada es la salida de humbleParser (1er paso compilación)
//
//    * Traduce cada línea de texto a una sentencia de ArithMachine
//
//    * Lanza una señal con el programa generado para ArithMachine

// ------------------------------------------------

class AM_Asembler
{
public:
    void Compile(std::string& code);

    mtk::Signal<const t_program&>  signalCompiledProgram;

private:
    Instruction Compile(const std::string& code, const std::string& data) const;

};



#endif // ARITHMACHINE_H
