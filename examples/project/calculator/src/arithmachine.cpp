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
    switch(d.data_type)
    {
        case Data::dt_value:
            os << "v·" << d.value;
            break;
        case Data::dt_reference:
            os << "r·" << d.reference;
            break;
    }

    return os;
}

std::ostream& operator<< (std::ostream& os, const Instruction& i)
{
    os << int(i.operator_code) << ": " << i.data;
    return os;
}


// -----------------------------------
//      D a t a
// -----------------------------------

Data::Data()
    :     data_type  ( dt_value )
        , value     ()
{};

Data::Data(const Odbl& val)
    :     data_type   ( dt_value)
        , value(val)
{};

Data::Data(const std::string& ref)
    :     data_type  ( dt_reference )
        , value     ()
        , reference (ref)
{
};






// -----------------------------------

//      A r i t h M ac h i n e

// -----------------------------------

void Arith_machine::add_instruction   (const Instruction&     instruction )
{
    //  if it's a function, verify it exits with valid arity
    bool  result = false;
    int   arity  = 0;
    if (instruction.operator_code == oc_funct1)
    {
        arity = 1;
        if (functions1.find(instruction.data.reference) != functions1.end())
            result = true;
    }
    if (instruction.operator_code == oc_funct2)
    {
        arity = 2;
        if (functions2.find(instruction.data.reference) != functions2.end())
            result = true;
    }

    if (result == false   &&  arity > 0)
        throw JLE_SS("ERROR...  function " <<  instruction.data.reference << "/" << arity << " non existent");

    program.push_back( instruction );

}

void Arith_machine::add_program(const t_program&   _program)
{
    partial_reset();
    for(inst : _program)
    {
        add_instruction(inst);
    }
}

void Arith_machine::reset_machine(void)
{
    program.clear();
    stack = std::stack<Data>();
    heap.clear();
}
void Arith_machine::partial_reset(void)        //  it doesn't delte the heap
{
    program.clear();
    stack = std::stack<Data>();
}





Odbl Arith_machine::eval(void)
{
    //  execute all instructions
    //  check at end... only one element on stack
    //  take it and return as result

    for(inst : program)
    {
        switch (inst.operator_code)
        {
            case oc_push:
                    stack.push(inst.data);
                    break;

            case oc_funct1:
                    {
                        Odbl par1   = stack_pop();
                        auto result = Odbl();
                        functions1[inst.data.reference]->emit(par1, result);
                        stack.push(result);
                    }
                    break;

            case oc_funct2:
                    {
                        Odbl par2   = stack_pop();
                        Odbl par1   = stack_pop();
                        auto result = Odbl();
                        functions2[inst.data.reference]->emit(par1, par2, result);
                        stack.push(result);
                    }
                    break;

            case oc_copy_stack_top2heap:
                    {
                        Odbl top = stack_top();
                        set_value_in_heap(inst.data.reference, top);
                    }
                    break;

            default:
		throw JLE_SS("Unknown operator code "  <<  int(inst.operator_code));

        };
    }
    if (stack.size() == 0)
        return Odbl();
    if (stack.size() != 1)
        throw JLE_SS("ERROR... stack is not empty  " << stack.size());

    return stack_pop();
}

//  if it's a referenci, result it and return a data
Odbl Arith_machine::stack_top(void)
{
    if (stack.size()==0)
        throw JLE_SS("ERROR... empty stack");

    Data result;

    result = stack.top();
    if (result.data_type == Data::dt_reference)
        return get_value_from_heap(result.reference);
    else
        return result.value;
}

Odbl Arith_machine::stack_pop(void)
{
    Data result = stack_top();
    stack.pop();
    return result.value;
}


Odbl Arith_machine::get_value_from_heap(const std::string& refName) const
{
    std::map<std::string, Odbl>::const_iterator it = heap.find(refName);
    if (it == heap.end())
        return Odbl();
    else
        return it->second;
}

void  Arith_machine::set_value_in_heap  (const std::string& refName, const Odbl& val)
{
    std::map<std::string, Odbl>::iterator it = heap.find(refName);
    if ( it == heap.end() )
        heap.insert(std::pair<std::string, Odbl>(refName, val));
    else
        it->second = val;
}


jle::shared_ptr<Arith_machine::t_signal_function1arity>
Arith_machine::register_function1arity (const std::string& name)
{
    functions1[name] = jle::make_shared<Arith_machine::t_signal_function1arity>();

    return functions1[name];
}

jle::shared_ptr<Arith_machine::t_signal_function2arity>
Arith_machine::register_function2arity (const std::string& name)
{
    functions2[name] = jle::make_shared<Arith_machine::t_signal_function2arity>();

    return functions2[name];
}





// -----------------------------------
//      F u n c t i o n s _ B a s i c
// -----------------------------------


Functions_basic::Functions_basic(Arith_machine& am)
{
    JLE_CONNECT_THIS( (*(am.register_function1arity("-"))) , unnary_minus );
    JLE_CONNECT_THIS( (*(am.register_function2arity("+"))) , add         );
    JLE_CONNECT_THIS( (*(am.register_function2arity("-"))) , minus       );
    JLE_CONNECT_THIS( (*(am.register_function2arity("*"))) , multiply    );
    JLE_CONNECT_THIS( (*(am.register_function2arity("/"))) , divide      );
}

void Functions_basic::add ( const Odbl& par1,
                            const Odbl& par2,
                            Odbl& result)
{
    result = par1 + par2;
}

void Functions_basic::minus (const Odbl& par1,
                        const Odbl& par2,
                        Odbl& result)
{
    result = par1 - par2;
}


void Functions_basic::multiply (const Odbl& par1,
                        const Odbl& par2,
                        Odbl& result)
{
    result = par1 * par2;
}

void Functions_basic::divide (const Odbl& par1,
                        const Odbl& par2,
                        Odbl& result)
{
    try {
        result = par1 / par2;
    }
    catch(...) {  result = Odbl();  }
}


void Functions_basic::unnary_minus(const Odbl& par, Odbl& result)
{
    result = Odbl(-1.) * par;
}

// ------------------------------------------------
//      A M _ A s e m b l e r
// ------------------------------------------------


void AM_asembler::compile(std::string& code)
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
            generatedProgram.push_back( compile(_code, data) );
        }
    }
    signal_compiled_program.emit(generatedProgram);
}


Instruction AM_asembler::compile(const std::string& code, const std::string& data) const
{
    if      (code == "fun/1")
        return Instruction (   oc_funct1, data );
    else if (code == "fun/2")
        return Instruction (   oc_funct2, data );
    else if (code == "copy2")
    {
        if (data.find("var:") == 0)
            return Instruction (   oc_copy_stack_top2heap, data.substr(4));
        else
            throw JLE_SS("copy2 ???  unknown destination " << data);
    }
    else if (code == "num")
    {
        double value;  bool result;
        std::tie(value, result) = jle::s_try_s2d(data, 0.);
        if (result)
            return Instruction (   oc_push  , Odbl(value) );
        else
            return Instruction (   oc_push  , Odbl() );
    }
    else if (code == "var")
        return Instruction (   oc_push, data );
    else
        throw JLE_SS("ERROR... invalid code  " << code);
}
