#include "arithmachine.h"


#include "core/string.h"



//  grammar and trasnsformation in aritheval.gra





// -----------------------------------
//      D a t a
// -----------------------------------

Data::Data()
    :     dataType  ( dtValue )
        , value     (jle::dbl::InvalidValue())
{};

Data::Data(const jle::dbl& val)
    :     dataType   ( dtValue)
        , value(val)
{};

Data::Data(const std::string& ref)
    :     dataType  ( dtReference )
        , value     (jle::dbl::InvalidValue())
        , reference (ref)
{
};






// -----------------------------------

//      A r i t h M ac h i n e

// -----------------------------------

void ArithMachine::AddInstruction   (const Instruction&     instruction )
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

void ArithMachine::AddProgram(const t_program&   _program)
{
    PartialReset();
    for(auto itInstruction = _program.begin(); itInstruction != _program.end(); ++itInstruction)
        AddInstruction(*itInstruction);
}

void ArithMachine::ResetMachine(void)
{
    program.clear();
    stack = std::stack<Data>();
    heap.clear();
}
void ArithMachine::PartialReset(void)        //  it doesn't delte the heap
{
    program.clear();
    stack = std::stack<Data>();
}





jle::dbl ArithMachine::Eval(void)
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
                        jle::dbl par1   = StackPop();
                        jle::dbl result = jle::dbl::InvalidValue();
                        functions1[itInstruction->data.reference]->emit(par1, result);
                        stack.push(result);
                    }
                    break;

            case ocFunct2:
                    {
                        jle::dbl par2   = StackPop();
                        jle::dbl par1   = StackPop();
                        jle::dbl result = jle::dbl::InvalidValue();
                        functions2[itInstruction->data.reference]->emit(par1, par2, result);
                        stack.push(result);
                    }
                    break;

            case ocCopyStackTop2Heap:
                    {
                        jle::dbl top = StackTop();
                        SetValueInHeap(itInstruction->data.reference, top);
                    }
                    break;

            default:
		throw JLE_SS("Unknown operator code "  <<  int(itInstruction->operatorCode));

        };
    }
    if (stack.size() == 0)
        return jle::dbl::InvalidValue();
    if (stack.size() != 1)
        throw JLE_SS("ERROR... stack is not empty  " << stack.size());

    return StackPop();


}

//  if it's a referenci, result it and return a data
jle::dbl ArithMachine::StackTop(void)
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

jle::dbl ArithMachine::StackPop(void)
{
    Data result = StackTop();
    stack.pop();
    return result.value;
}


jle::dbl ArithMachine::GetValueFromHeap(const std::string& refName) const
{
    std::map<std::string, jle::dbl>::const_iterator it = heap.find(refName);
    if (it == heap.end())
        return jle::dbl::InvalidValue();
    else
        return it->second;
}

void  ArithMachine::SetValueInHeap  (const std::string& refName, const jle::dbl& val)
{
    std::map<std::string, jle::dbl>::iterator it = heap.find(refName);
    if ( it == heap.end() )
        heap.insert(std::pair<std::string, jle::dbl>(refName, val));
    else
        it->second = val;
}


jle::shared_ptr<ArithMachine::t_signalFunction1arity>
ArithMachine::RegisterFunction1Arity (const std::string& name)
{
    functions1[name] = jle::make_cptr( new t_signalFunction1arity());

    return functions1[name];
}

jle::shared_ptr<ArithMachine::t_signalFunction2arity>
ArithMachine::RegisterFunction2Arity (const std::string& name)
{
    functions2[name] = jle::make_cptr( new t_signalFunction2arity());

    return functions2[name];
}





// -----------------------------------
//      F u n c t i o n s _ B a s i c
// -----------------------------------


Functions_Basic::Functions_Basic(ArithMachine& am)
{
    JLE_CONNECT_THIS( (*(am.RegisterFunction1Arity("-"))) , UnnaryMinus );
    JLE_CONNECT_THIS( (*(am.RegisterFunction2Arity("+"))) , Add         );
    JLE_CONNECT_THIS( (*(am.RegisterFunction2Arity("-"))) , Minus       );
    JLE_CONNECT_THIS( (*(am.RegisterFunction2Arity("*"))) , Multiply    );
    JLE_CONNECT_THIS( (*(am.RegisterFunction2Arity("/"))) , Divide      );
}

void Functions_Basic::Add (const jle::dbl& par1,
                        const jle::dbl& par2,
                        jle::dbl& result)
{
    result = par1 + par2;
}

void Functions_Basic::Minus (const jle::dbl& par1,
                        const jle::dbl& par2,
                        jle::dbl& result)
{
    result = par1 - par2;
}


void Functions_Basic::Multiply (const jle::dbl& par1,
                        const jle::dbl& par2,
                        jle::dbl& result)
{
    result = par1 * par2;
}

void Functions_Basic::Divide (const jle::dbl& par1,
                        const jle::dbl& par2,
                        jle::dbl& result)
{
    try {
        result = par1 / par2;
    }
    catch(...) {  result = jle::dbl::InvalidValue();  }
}


void Functions_Basic::UnnaryMinus(       const jle::dbl& par,
                        jle::dbl& result)
{
    result = -1. * par;
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
        jle::s_TRY_stod(data, 0).assign(value, result);
        if (result)
            return Instruction (   ocPush  , jle::dbl(value) );
        else
            return Instruction (   ocPush  , jle::dbl::InvalidValue() );
    }
    else if (code == "var")
        return Instruction (   ocPush, data );
    else
        throw JLE_SS("ERROR... invalid code  " << code);
}
