#ifndef ARITH_MACHINE__H
#define ARITH_MACHINE__H


#include <string>
#include <vector>
#include <stack>
#include <map>


#include "core/signal_slot.hpp"
#include "core/dbl.h"
#include "core/optional.hpp"



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





class Odbl {
public:
        explicit Odbl() : value() {};
        explicit Odbl(double val) : value(val) {};
        explicit Odbl(const jle::dbl& val) : value(val) {};

        Odbl(const Odbl&)=default;
        Odbl(Odbl&&) =default;

        Odbl& operator=(const Odbl&) = default;
        Odbl& operator=(Odbl&&) = default;

        #define DEF_OPERATOR(__OP__)  \
        Odbl operator __OP__(const Odbl& v) const  \
        {  \
            if(value.has_value()  &&  v.value.has_value())  \
            {  \
                return Odbl(*value __OP__ *(v.value));  \
            }  \
            else  \
                return Odbl();  \
        }
        DEF_OPERATOR(+)
        DEF_OPERATOR(-)
        DEF_OPERATOR(*)
        DEF_OPERATOR(/)
        #undef DEF_OPERATOR

private:
    jle::optional<jle::dbl>  value;
    friend std::ostream& operator<< (std::ostream& os, const Odbl& v);
};
std::ostream& operator<< (std::ostream& os, const Odbl& v);


// -----------------------------------
//      D a t a
// -----------------------------------

struct Data
{
    enum enDataType {  dtValue, dtReference  };

    enDataType      dataType        ;

    Odbl            value           ;
    std::string     reference       ;


    Data();
    Data(const Odbl& val);
    Data(const std::string& ref);

};
std::ostream& operator<< (std::ostream& os, const Data& d);




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
std::ostream& operator<< (std::ostream& os, const Instruction& i);


typedef std::vector<Instruction>  t_program;




// -----------------------------------

//      A r i t h M a c h i n e

// -----------------------------------

class Arith_machine  : public jle::signal_receptor
{
        typedef     jle::signal<
                            const Odbl&             /*param 1  */,
                            const Odbl&             /*param 2  */,
                            Odbl&    /*result   */>
                    t_signalFunction2arity;

        typedef     jle::signal<
                            const Odbl&         /*param 1  */,
                            Odbl& /*result   */>
                    t_signalFunction1arity;




public:

    void reset_machine(void);
    void PartialReset(void);        //  keeps the heap

    //  to add functions directly
    void AddInstruction     (const Instruction& instruction );
    void AddProgram         (const t_program&   _program);

    //  write on memory directly
    void WriteValueOnStack  (const std::string& refName, const Odbl& val);

    Odbl Eval(void);
    void        Run (void);


    jle::shared_ptr<t_signalFunction1arity> RegisterFunction1Arity (const std::string& name);
    jle::shared_ptr<t_signalFunction2arity> RegisterFunction2Arity (const std::string& name);



    //  Direct access to memory allowed by performance and simplicity
    Odbl GetValueFromHeap(const std::string& refName) const;
    void                    SetValueInHeap  (const std::string& refName, const Odbl& val);






private:

    std::vector<Instruction>                        program     ;
    std::stack<Data>                                stack       ;
    std::map<std::string, Odbl>  heap        ;

    std::map<std::string, jle::shared_ptr<t_signalFunction1arity> >   functions1  ;
    std::map<std::string, jle::shared_ptr<t_signalFunction2arity> >   functions2  ;


    Odbl StackTop(void);    //  If it's a reference, it will be solved and data will be returned
    Odbl StackPop(void);    //  It will return the data and it will remove from stack

};








// -----------------------------------
//      F u n c t i o n s _ B a s i c
// -----------------------------------
class Functions_Basic : public jle::signal_receptor
{
public:

    Functions_Basic(Arith_machine& am);


private:

    void UnnaryMinus(       const Odbl& par,
                            Odbl& result);

    void Add        (       const Odbl& par1,
                            const Odbl& par2,
                            Odbl& result);

    void Minus      (       const Odbl& par1,
                            const Odbl& par2,
                            Odbl& result);

    void Multiply   (       const Odbl& par1,
                            const Odbl& par2,
                            Odbl& result);

    void Divide     (       const Odbl& par1,
                            const Odbl& par2,
                            Odbl& result);
};




// ------------------------------------------------

//      A M _ A s e m b l e r


//    * We gave in one or more strings, the
//      compiled program

//    * This string is produced by Humble_parser (first compile step)
//
//    * It translate each string line to an Arith_machine sentence
//
//    * It throws a signal with the program generated for Arith_machine

// ------------------------------------------------

class AM_Asembler
{
public:
    void Compile(std::string& code);

    jle::signal<const t_program&>  signalCompiledProgram;

private:
    Instruction Compile(const std::string& code, const std::string& data) const;

};



#endif // ARITH_MACHINE__H
