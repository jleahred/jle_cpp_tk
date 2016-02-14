#include "arithmachine.h"


#include "core/string.h"



//  grammar and trasnsformation in aritheval.gra



std::ostream& operator<< (std::ostream& os, const Odbl& v)
{
    os  << v.value;
    return os;
}

std::ostream& operator<< (std::ostream& os, const Data& d)
{
    switch(d.dataType)
    {
        case Data::dtValue:
            os << "v·" << d.value;
            break;
        case Data::dtReference:
            os << "r·" << d.reference;
            break;
    }

    return os;
}

std::ostream& operator<< (std::ostream& os, const Instruction& i)
{
    os << int(i.operatorCode) << ": " << i.data;
    return os;
}


// -----------------------------------
//      D a t a
// -----------------------------------

Data::Data()
    :     dataType  ( dtValue )
        , value     ()
{};

Data::Data(const Odbl& val)
    :     dataType   ( dtValue)
        , value(val)
{};

Data::Data(const std::string& ref)
    :     dataType  ( dtReference )
        , value     ()
        , reference (ref)
{
};






// -----------------------------------

//      A r i t h M ac h i n e

// -----------------------------------

void Arith_machine::AddInstruction   (const Instruction&     instruction )
{
    //  if it's a function, verify it exits with valid arity
    bool  result = false;
    int   arity  = 0;
    if (instruction.operatorCode == ocFunct1)
    {
        arity = 1;
        if (functions1.find(instruction.data.reference) != functions1.end())
            result = true;
    }
    if (instruction.operatorCode == ocFunct2)
    {
        arity = 2;
        if (functions2.find(instruction.data.reference) != functions2.end())
            result = true;
    }

    if (result == false   &&  arity > 0)
        throw JLE_SS("ERROR...  function " <<  instruction.data.reference << "/" << arity << " non existent");

    program.push_back( instruction );

}

void Arith_machine::AddProgram(const t_program&   _program)
{
    PartialReset();
    for(auto itInstruction = _program.begin(); itInstruction != _program.end(); ++itInstruction)
    {
        AddInstruction(*itInstruction);
    }
}

void Arith_machine::reset_machine(void)
{
    program.clear();
    stack = std::stack<Data>();
    heap.clear();
}
void Arith_machine::PartialReset(void)        //  it doesn't delte the heap
{
    program.clear();
    stack = std::stack<Data>();
}





Odbl Arith_machine::Eval(void)
{
    //  execute all instructions
    //  check at end... only one element on stack
    //  take it and return as result

    for(auto itInstruction = program.begin(); itInstruction != program.end(); ++itInstruction)
    {
        switch (itInstruction->operatorCode)
        {
            case ocPush:
                    stack.push(itInstruction->data);
                    break;

            case ocFunct1:
                    {
                        Odbl par1   = StackPop();
                        auto result = Odbl();
                        functions1[itInstruction->data.reference]->emit(par1, result);
                        stack.push(result);
                    }
                    break;

            case ocFunct2:
                    {
                        Odbl par2   = StackPop();
                        Odbl par1   = StackPop();
                        auto result = Odbl();
                        functions2[itInstruction->data.reference]->emit(par1, par2, result);
                        stack.push(result);
                    }
                    break;

            case ocCopyStackTop2Heap:
                    {
                        Odbl top = StackTop();
                        SetValueInHeap(itInstruction->data.reference, top);
                    }
                    break;

            default:
		throw JLE_SS("Unknown operator code "  <<  int(itInstruction->operatorCode));

        };
    }
    if (stack.size() == 0)
        return Odbl();
    if (stack.size() != 1)
        throw JLE_SS("ERROR... stack is not empty  " << stack.size());

    return StackPop();
}

//  if it's a referenci, result it and return a data
Odbl Arith_machine::StackTop(void)
{
    if (stack.size()==0)
        throw JLE_SS("ERROR... empty stack");

    Data result;

    result = stack.top();
    if (result.dataType == Data::dtReference)
        return GetValueFromHeap(result.reference);
    else
        return result.value;
}

Odbl Arith_machine::StackPop(void)
{
    Data result = StackTop();
    stack.pop();
    return result.value;
}


Odbl Arith_machine::GetValueFromHeap(const std::string& refName) const
{
    std::map<std::string, Odbl>::const_iterator it = heap.find(refName);
    if (it == heap.end())
        return Odbl();
    else
        return it->second;
}

void  Arith_machine::SetValueInHeap  (const std::string& refName, const Odbl& val)
{
    std::map<std::string, Odbl>::iterator it = heap.find(refName);
    if ( it == heap.end() )
        heap.insert(std::pair<std::string, Odbl>(refName, val));
    else
        it->second = val;
}


jle::shared_ptr<Arith_machine::t_signalFunction1arity>
Arith_machine::RegisterFunction1Arity (const std::string& name)
{
    functions1[name] = jle::make_shared<Arith_machine::t_signalFunction1arity>();

    return functions1[name];
}

jle::shared_ptr<Arith_machine::t_signalFunction2arity>
Arith_machine::RegisterFunction2Arity (const std::string& name)
{
    functions2[name] = jle::make_shared<Arith_machine::t_signalFunction2arity>();

    return functions2[name];
}





// -----------------------------------
//      F u n c t i o n s _ B a s i c
// -----------------------------------


Functions_Basic::Functions_Basic(Arith_machine& am)
{
    JLE_CONNECT_THIS( (*(am.RegisterFunction1Arity("-"))) , UnnaryMinus );
    JLE_CONNECT_THIS( (*(am.RegisterFunction2Arity("+"))) , Add         );
    JLE_CONNECT_THIS( (*(am.RegisterFunction2Arity("-"))) , Minus       );
    JLE_CONNECT_THIS( (*(am.RegisterFunction2Arity("*"))) , Multiply    );
    JLE_CONNECT_THIS( (*(am.RegisterFunction2Arity("/"))) , Divide      );
}

void Functions_Basic::Add ( const Odbl& par1,
                            const Odbl& par2,
                            Odbl& result)
{
    result = par1 + par2;
}

void Functions_Basic::Minus (const Odbl& par1,
                        const Odbl& par2,
                        Odbl& result)
{
    result = par1 - par2;
}


void Functions_Basic::Multiply (const Odbl& par1,
                        const Odbl& par2,
                        Odbl& result)
{
    result = par1 * par2;
}

void Functions_Basic::Divide (const Odbl& par1,
                        const Odbl& par2,
                        Odbl& result)
{
    try {
        result = par1 / par2;
    }
    catch(...) {  result = Odbl();  }
}


void Functions_Basic::UnnaryMinus(const Odbl& par, Odbl& result)
{
    result = Odbl(-1.) * par;
}

// ------------------------------------------------
//      A M _ A s e m b l e r
// ------------------------------------------------


void AM_Asembler::Compile(std::string& code)
{
    t_program generatedProgram;

    jle::vector<std::string> lines = jle::s_split(code, "\n");

    for (unsigned i=0; i<lines.size(); ++i)
    {
        if ( jle::s_trim(lines[i], ' ') != "")
        {
            //  code and data separated
            std::string::size_type codeTill = lines[i].find_first_of(':');
            std::string _code = lines[i].substr(0, codeTill);
            std::string data = lines[i].substr(codeTill+1);
            generatedProgram.push_back( Compile(_code, data) );
        }
    }
    signalCompiledProgram.emit(generatedProgram);
}


Instruction AM_Asembler::Compile(const std::string& code, const std::string& data) const
{
    if      (code == "fun/1")
        return Instruction (   ocFunct1, data );
    else if (code == "fun/2")
        return Instruction (   ocFunct2, data );
    else if (code == "copy2")
    {
        if (data.find("var:") == 0)
            return Instruction (   ocCopyStackTop2Heap, data.substr(4));
        else
            throw JLE_SS("copy2 ???  unknown destination " << data);
    }
    else if (code == "num")
    {
        double value;  bool result;
        std::tie(value, result) = jle::s_try_s2d(data, 0.);
        if (result)
            return Instruction (   ocPush  , Odbl(value) );
        else
            return Instruction (   ocPush  , Odbl() );
    }
    else if (code == "var")
        return Instruction (   ocPush, data );
    else
        throw JLE_SS("ERROR... invalid code  " << code);
}
