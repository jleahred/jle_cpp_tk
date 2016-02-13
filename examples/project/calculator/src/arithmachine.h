#ifndef ARITHMACHINE_H
#define ARITHMACHINE_H


#include <string>
#include <vector>
#include <stack>
#include <map>


#include "core/signal_slot.hpp"
#include "core/dbl.h"



/*

    A r i t h M a c h i n e
    ______________________________

        * It works with double data types
        * It can work by value or by reference
        * It has a stack where add values and references
        * The heap is accesible by string with the var name
        * It has a method to fast modification of heap
        * When writting a reference and it will not exists
          a space for it will be reserverd.
          It's not possible to explicitly reserve the memory


*/







// -----------------------------------
//      D a t a
// -----------------------------------

struct Data
{
    enum enDataType {  dtValue, dtReference  };

    enDataType      dataType        ;

     jle::dbl       value           ;
    std::string     reference       ;


    Data();
    Data(const jle::dbl& val);
    Data(const std::string& ref);

};




// -----------------------------------
//      I n s t r u c t i o n
// -----------------------------------

enum enOperatorCode {         ocPush
                            , ocFunct1
                            , ocFunct2
                            , ocCopyStackTop2Heap   //  copy value from stack top, it continues on stack
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

class ArithMachine  : public jle::signal_receptor
{
        typedef     jle::signal<
                            const jle::dbl&      /*param 1  */,
                            const jle::dbl&      /*param 2  */,
                            jle::dbl&            /*result   */>
                    t_signalFunction2arity;

        typedef     jle::signal<
                            const jle::dbl&      /*param 1  */,
                            jle::dbl&            /*result   */>
                    t_signalFunction1arity;




public:

    void ResetMachine(void);
    void PartialReset(void);        //  keeps the heap

    //  to add functions directly
    void AddInstruction     (const Instruction& instruction );
    void AddProgram         (const t_program&   _program);

    //  write on memory directly
    void WriteValueOnStack  (const std::string& refName, const jle::dbl& val);

    jle::dbl Eval(void);
    void        Run (void);


    jle::shared_ptr<t_signalFunction1arity> RegisterFunction1Arity (const std::string& name);
    jle::shared_ptr<t_signalFunction2arity> RegisterFunction2Arity (const std::string& name);



    //  Direct access to memory allowed by performance and simplicity
    jle::dbl GetValueFromHeap(const std::string& refName) const;
    void        SetValueInHeap  (const std::string& refName, const jle::dbl& val);






private:

    std::vector<Instruction>                        program     ;
    std::stack<Data>                                stack       ;
    std::map<std::string, jle::dbl>              heap        ;

    std::map<std::string, jle::shared_ptr<t_signalFunction1arity> >   functions1  ;
    std::map<std::string, jle::shared_ptr<t_signalFunction2arity> >   functions2  ;


    jle::dbl StackTop(void);    //  If it's a reference, it will be solved and data will be returned
    jle::dbl StackPop(void);    //  It will return the data and it will remove from stack

};








// -----------------------------------
//      F u n c t i o n s _ B a s i c
// -----------------------------------
class Functions_Basic : public jle::signal_receptor
{
public:

    Functions_Basic(ArithMachine& am);


private:

    void UnnaryMinus(       const jle::dbl& par,
                            jle::dbl& result);

    void Add        (       const jle::dbl& par1,
                            const jle::dbl& par2,
                            jle::dbl& result);

    void Minus      (       const jle::dbl& par1,
                            const jle::dbl& par2,
                            jle::dbl& result);

    void Multiply   (       const jle::dbl& par1,
                            const jle::dbl& par2,
                            jle::dbl& result);

    void Divide     (       const jle::dbl& par1,
                            const jle::dbl& par2,
                            jle::dbl& result);
};




// ------------------------------------------------

//      A M _ A s e m b l e r


//    * We gave in one or more strings, the
//      compiled program

//    * This string is produced by Humble_parser (first compile step)
//
//    * It translate each string line to an ArithMachine sentence
//
//    * It throws a signal with the program generated for ArithMachine

// ------------------------------------------------

class AM_Asembler
{
public:
    void Compile(std::string& code);

    jle::signal<const t_program&>  signalCompiledProgram;

private:
    Instruction Compile(const std::string& code, const std::string& data) const;

};



#endif // ARITHMACHINE_H
