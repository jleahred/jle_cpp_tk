#include "arithmachine.h"


#include "support/mtk_string.h"



//  gramática y transformación en  aritheval.gra





// -----------------------------------
//      D a t a
// -----------------------------------

Data::Data()
    :     dataType  ( dtValue )
        , value     (mtk::Double::InvalidValue())
{};

Data::Data(const mtk::Double& val)
    :     dataType   ( dtValue)
        , value(val)
{};

Data::Data(const std::string& ref)
    :     dataType  ( dtReference )
        , value     (mtk::Double::InvalidValue())
        , reference (ref)
{
};






// -----------------------------------

//      A r i t h M ac h i n e

// -----------------------------------

void ArithMachine::AddInstruction   (const Instruction&     instruction )
{
    //  si es una función, verificar que esta existe para la paridad indicada
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
        throw MTK_SS("ERROR...  function " <<  instruction.data.reference << "/" << arity << " non existent");

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
void ArithMachine::PartialReset(void)        //  no borra el montículo
{
    program.clear();
    stack = std::stack<Data>();
}





mtk::Double ArithMachine::Eval(void)
{
    //  ejecutar todas las instrucciones
    //  verificar que al final queda sólo 1 elemento en la pila
    //  scarlo y devolverlo como resultado

    for(auto itInstruction = program.begin(); itInstruction != program.end(); ++itInstruction)
    {
        switch (itInstruction->operatorCode)
        {
            case ocPush:
                    stack.push(itInstruction->data);
                    break;

            case ocFunct1:
                    {
                        mtk::Double par1   = StackPop();
                        mtk::Double result = mtk::Double::InvalidValue();
                        functions1[itInstruction->data.reference]->emit(par1, result);
                        stack.push(result);
                    }
                    break;

            case ocFunct2:
                    {
                        mtk::Double par2   = StackPop();
                        mtk::Double par1   = StackPop();
                        mtk::Double result = mtk::Double::InvalidValue();
                        functions2[itInstruction->data.reference]->emit(par1, par2, result);
                        stack.push(result);
                    }
                    break;

            case ocCopyStackTop2Heap:
                    {
                        mtk::Double top = StackTop();
                        SetValueInHeap(itInstruction->data.reference, top);
                    }
                    break;

            default:
		throw MTK_SS("Unknown operator code "  <<  int(itInstruction->operatorCode));

        };
    }
    if (stack.size() == 0)
        return mtk::Double::InvalidValue();
    if (stack.size() != 1)
        throw MTK_SS("ERROR... stack is not empty  " << stack.size());

    return StackPop();


}

//  si es una referencia, la resuelve y devuelve siempre un dato
mtk::Double ArithMachine::StackTop(void)
{
    if (stack.size()==0)
        throw MTK_SS("ERROR... empty stack");

    Data result;

    result = stack.top();
    if (result.dataType == Data::dtReference)
        return GetValueFromHeap(result.reference);
    else
        return result.value;
}

mtk::Double ArithMachine::StackPop(void)
{
    Data result = StackTop();
    stack.pop();
    return result.value;
}


mtk::Double ArithMachine::GetValueFromHeap(const std::string& refName) const
{
    std::map<std::string, mtk::Double>::const_iterator it = heap.find(refName);
    if (it == heap.end())
        return mtk::Double::InvalidValue();
    else
        return it->second;
}

void  ArithMachine::SetValueInHeap  (const std::string& refName, const mtk::Double& val)
{
    std::map<std::string, mtk::Double>::iterator it = heap.find(refName);
    if ( it == heap.end() )
        heap.insert(std::pair<std::string, mtk::Double>(refName, val));
    else
        it->second = val;
}


mtk::CountPtr<ArithMachine::t_signalFunction1arity>
ArithMachine::RegisterFunction1Arity (const std::string& name)
{
    functions1[name] = mtk::make_cptr( new t_signalFunction1arity());

    return functions1[name];
}

mtk::CountPtr<ArithMachine::t_signalFunction2arity>
ArithMachine::RegisterFunction2Arity (const std::string& name)
{
    functions2[name] = mtk::make_cptr( new t_signalFunction2arity());

    return functions2[name];
}





// -----------------------------------
//      F u n c t i o n s _ B a s i c
// -----------------------------------


Functions_Basic::Functions_Basic(ArithMachine& am)
{
    MTK_CONNECT_THIS( (*(am.RegisterFunction1Arity("-"))) , UnnaryMinus );
    MTK_CONNECT_THIS( (*(am.RegisterFunction2Arity("+"))) , Add         );
    MTK_CONNECT_THIS( (*(am.RegisterFunction2Arity("-"))) , Minus       );
    MTK_CONNECT_THIS( (*(am.RegisterFunction2Arity("*"))) , Multiply    );
    MTK_CONNECT_THIS( (*(am.RegisterFunction2Arity("/"))) , Divide      );
}

void Functions_Basic::Add (const mtk::Double& par1,
                        const mtk::Double& par2,
                        mtk::Double& result)
{
    result = par1 + par2;
}

void Functions_Basic::Minus (const mtk::Double& par1,
                        const mtk::Double& par2,
                        mtk::Double& result)
{
    result = par1 - par2;
}


void Functions_Basic::Multiply (const mtk::Double& par1,
                        const mtk::Double& par2,
                        mtk::Double& result)
{
    result = par1 * par2;
}

void Functions_Basic::Divide (const mtk::Double& par1,
                        const mtk::Double& par2,
                        mtk::Double& result)
{
    try {
        result = par1 / par2;
    }
    catch(...) {  result = mtk::Double::InvalidValue();  }
}


void Functions_Basic::UnnaryMinus(       const mtk::Double& par,
                        mtk::Double& result)
{
    result = -1. * par;
}

// ------------------------------------------------
//      A M _ A s e m b l e r
// ------------------------------------------------


void AM_Asembler::Compile(std::string& code)
{
    t_program generatedProgram;

    mtk::vector<std::string> lines = mtk::s_split(code, "\n");

    for (unsigned i=0; i<lines.size(); ++i)
    {
        if ( mtk::s_trim(lines[i], ' ') != "")
        {
            //  separar el código de los datos
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
            throw MTK_SS("copy2 ???  unknown destination " << data);
    }
    else if (code == "num")
    {
        double value;  bool result;
        mtk::s_TRY_stod(data, 0).assign(value, result);
        if (result)
            return Instruction (   ocPush  , mtk::Double(value) );
        else
            return Instruction (   ocPush  , mtk::Double::InvalidValue() );
    }
    else if (code == "var")
        return Instruction (   ocPush, data );
    else
        throw MTK_SS("ERROR... invalid code  " << code);
}
