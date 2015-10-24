#include "humbleparser.h"

#include <regex>
#include "core/string.h"


#include <iostream>  // I/O
#include <fstream>   // file I/O
#include <iomanip>   // format manipulation
#include <regex>







namespace jle {  namespace hp  {




Humble_parser::Humble_parser()
    :       string2parse                    (),
            non_terminal_rules              (),
            terminal_rules                  (),
            errors_in_rules                 (false),
            deeper_error_parsing_pos        (-1),
            deeper_error_expected_symbols   (),
            ast_root_debug                  (0),
            adding_rule_multi_line          (false)
{
}

Humble_parser::~Humble_parser()
{
}



bool Humble_parser::is_terminal(const std::string& str) const
{
    if ( (str[0]>='A' && str[0]<='Z')  ||  (str[0]=='.'  &&  str[str.length()-1]=='.'))
        return false;
    else
        return true;

    /*  checking performance
    if (str=="_")       return true;

    jle::RegExp re ("^([A-Z][A-Z|0-9|_|']*)$");      //  checking if is a non terminal

    if (re.Match(str))
        return false;
    else
        return true;
    */
}


#define PREPARE_NODE                    \
    if (current_ast_node.expired())                \
    {                                                                   \
        current_ast_node = jle::make_shared<AST_node_item>(node_name);       \
        ast_node.down = current_ast_node;                                  \
    }                                                                   \
    else                                                                \
    {                                                                   \
        current_ast_node->next = jle::make_shared<AST_node_item>(node_name);       \
        current_ast_node = current_ast_node->next;                          \
    }                                                                   \

#define CHECK_ERRORS   \
    if (deeper_error_parsing_pos < remaining_str2_parse_pos  ||   deeper_error_parsing_pos==-1)   \
    {   \
        deeper_error_parsing_pos = remaining_str2_parse_pos;   \
        deeper_error_expected_symbols.clear();   \
        deeper_error_expected_symbols.insert(*it1);   \
    }   \
    else if (remaining_str2_parse_pos == deeper_error_parsing_pos  ||   deeper_error_parsing_pos==-1)   \
    {   \
        deeper_error_parsing_pos = remaining_str2_parse_pos;   \
        deeper_error_expected_symbols.insert(*it1);   \
    }



jle::tuple<bool, int> Humble_parser::execute_non_terminal( int str2parse_pos, const std::string& non_terminal_code, AST_node_item& ast_node) const
{
    Container_rules::const_iterator it_non_terminal = non_terminal_rules.find(non_terminal_code);
    if (it_non_terminal == non_terminal_rules.end())
        throw JLE_SS("non terminal symbol [" << non_terminal_code <<  "] has no expansion rule");

    auto  it0 = non_terminal_rules.find(non_terminal_code)->second.cbegin();

    bool result=false;
    int str_pos_rule = str2parse_pos;

    //  checking all rules with non terminal code on left
    while (it0!=non_terminal_rules.find(non_terminal_code)->second.end())
    {
        const std::list<std::string>& right_symbols = std::get<0>(*it0);

        str_pos_rule = str2parse_pos;
        ast_node.rule4replace = std::get<1>(*it0);

        //  testing all symbols on right
        jle::shared_ptr<AST_node_item> current_ast_node;
        for(auto it1 = right_symbols.begin(); it1 != right_symbols.end(); ++it1)
        {
            int remaining_str2_parse_pos = str_pos_rule;
            result = false;

            std::string node_name(*it1);
            PREPARE_NODE
            CHECK_ERRORS

            if (is_terminal(*it1))
            {
                if (it1->substr(it1->size()-1) == "*")
                {
                    std::string symbol = it1->substr(0, it1->size()-1);     //  remove klein star
                    std::tie(result, remaining_str2_parse_pos/*, current_ast_node*/) =
                            execute_terminal_klein(remaining_str2_parse_pos, symbol, *current_ast_node);
                }
                else
                    std::tie(result, remaining_str2_parse_pos) =
                            execute_terminal(remaining_str2_parse_pos, *it1, *current_ast_node);
            }
            else
            {
                //  it is a klein start symbol?
                if (it1->substr(it1->size()-1) == "*")
                {
                    std::string symbol = it1->substr(0, it1->size()-1);     //  remove klein star
                    std::tie(result, remaining_str2_parse_pos/*, current_ast_node*/) =
                            execute_non_terminal_klein(remaining_str2_parse_pos, symbol, *current_ast_node);
                }
                else
                    std::tie(result, remaining_str2_parse_pos) =
                            execute_non_terminal(remaining_str2_parse_pos, *it1, *current_ast_node);
            }
            if (result == false)
                break;
            else
                str_pos_rule = remaining_str2_parse_pos;
            //  for tail recursion, it has to be empty
            //  if it's the last element on list, is equivalent to empty
        }

        if (result)
            break;
        ++it0;
        //  for tail recursion, it has to be empty
        //  if it's the last element on list, is equivalent to empty
    }
    //  for tail recursion, it has to be empty
    if (result==false)
        return std::make_tuple(result, str2parse_pos);
    else
        return std::make_tuple(result, str_pos_rule);
}


jle::tuple<bool, int/*, jle::shared_ptr<AST_node_item> */> Humble_parser::execute_non_terminal_klein( int str2parse_pos, const std::string& non_terminal_code, AST_node_item& ast_node) const
{
    int added_items = -1;
    bool result=true;
    int remaining_str2_parse_pos=str2parse_pos;
    jle::shared_ptr<AST_node_item> ast_node_before_fail;
    jle::shared_ptr<AST_node_item> current_ast_node;
    std::string node_name(non_terminal_code);
    //CHECK_ERRORS
    while (result)
    {
        ++added_items;
        ast_node_before_fail = current_ast_node;
        PREPARE_NODE
        std::tie(result, remaining_str2_parse_pos) = execute_non_terminal(remaining_str2_parse_pos, non_terminal_code, *current_ast_node);
    }
    if (result==false)
    {
        if (added_items!=0)
            ast_node_before_fail->next = jle::shared_ptr<AST_node_item>();
        else
            ast_node.down = jle::shared_ptr<AST_node_item>();
    }


    return std::make_tuple(true, remaining_str2_parse_pos/*, ast_node_before_fail*/);
}

jle::tuple<bool, int/*, jle::shared_ptr<AST_node_item> */> Humble_parser::execute_terminal_klein( int str2parse_pos, const std::string& non_terminal_code, AST_node_item& ast_node) const
{
    int added_items = -1;
    bool result=true;
    int remaining_str2_parse_pos=str2parse_pos;
    jle::shared_ptr<AST_node_item> ast_node_before_fail;
    jle::shared_ptr<AST_node_item> current_ast_node;
    std::string node_name(non_terminal_code);

    while (result)
    {
        ++added_items;
        ast_node_before_fail = current_ast_node;
        PREPARE_NODE
        std::tie(result, remaining_str2_parse_pos) = execute_terminal(remaining_str2_parse_pos, non_terminal_code, *current_ast_node);
    }
    if (result==false)
    {
        if (added_items!=0)
            ast_node_before_fail->next = jle::shared_ptr<AST_node_item>();
        else
            ast_node.down = jle::shared_ptr<AST_node_item>();
    }
    return std::make_tuple(true, remaining_str2_parse_pos/*, ast_node_before_fail*/);
}



/*
    Esta versión es interesante
    Acumula todos los caracteres en el código no terminal* reduciendo significativamente el tamaño del árbol AST
    Lo malo es que no permitiría hacer transformaciones individuales
    Sería aplicable si no se hacen transformaciones
    Otra opción es poner la estrella de klein en el lado derecho de la definición de una regla terminal
jle::tuple<bool, int, jle::shared_ptr<AST_node_item> > HumbleParser::ExecuteTerminalKlein( int str2parse_pos, const std::string& non_terminal_code, shared_ptr<AST_node_item> _ast_node) const
{
    int added_items = -1;
    bool result=true;
    int remaining_str2_parse_pos=str2parse_pos;
    std::string finalValue;

    while (result)
    {
        ++added_items;
        ExecuteTerminal(remaining_str2_parse_pos, non_terminal_code, *_ast_node).assign(result, remaining_str2_parse_pos);
        if (result)
            finalValue += _ast_node->value;
    }
   _ast_node->value = finalValue;
   return std::make_tuple(true, remaining_str2_parse_pos, _ast_node);
}
*/






jle::tuple<bool, int> Humble_parser::execute_terminal(const int str2parse_pos, const std::string& terminal_code, AST_node_item& ast_node) const
{
    Container_rules::const_iterator it_terminal = terminal_rules.find(terminal_code);
    if (it_terminal == terminal_rules.end())
        throw JLE_SS("terminal symbol [" << terminal_code <<  "] has no expansion rule");



    int remaining_str2_parse_pos = str2parse_pos;
    bool result = false;
    auto  it = terminal_rules.find(terminal_code)->second.cbegin();
    while (it!=terminal_rules.find(terminal_code)->second.cend())
    {
        //  predefined consts     ------------------------------------------------------------
        if ((*(std::get<0>(*it).begin())).substr(0, 2) == "__"  ||  (*(std::get<0>(*it).begin())).substr(0, 3) == "!__") {
            std::tie(result, remaining_str2_parse_pos) =
                    execute_predefined_var(str2parse_pos,  (*(std::get<0>(*it).begin())),  ast_node, std::get<1>(*it));
        }
        //  regular expresions      ------------------------------------------------------------
        else if ((*(std::get<0>(*it).begin())).substr(0, 2) == "^("  ||  (*(std::get<0>(*it).begin())).substr(0, 3) == "!^(")
        {
            std::tie(result, remaining_str2_parse_pos) =
                    execute_regular_expresion(str2parse_pos,  (*(std::get<0>(*it).begin())),  ast_node, std::get<1>(*it));
        }
        //  literals
        else if ((*(std::get<0>(*it).begin())).substr(0, 1) == "\'"  ||  (*(std::get<0>(*it).begin())).substr(0, 2) == "!\'") {
            std::tie(result, remaining_str2_parse_pos) =
                    execute_literal(str2parse_pos,  (*(std::get<0>(*it).begin())),  ast_node, std::get<1>(*it));
        }
        else
            throw JLE_SS("terminal symbol [" << terminal_code <<  "] invalid format " <<  (*(std::get<0>(*it).begin())));



        if (result)
            return std::make_tuple(true, remaining_str2_parse_pos);

        ++it;
    }
    return std::make_tuple(false, str2parse_pos);
}




jle::tuple<bool, std::string>  Humble_parser::add_rule (const std::string& rule_t2)
{
    std::string rule;


    std::string transform2;
    {
        static std::regex  re_rt(" *(.*) *##transf2-> *(.*) *");
        std::smatch re_result;

        if (std::regex_match(rule_t2, re_result, re_rt))
        {
            rule = jle::s_trim(re_result[1], ' ');
            transform2 = jle::s_trim(re_result[2], ' ');
        }
        else
        {
            rule = rule_t2;
        }
    }

    //  separate rule sides
    static std::regex re ("^ *([^ \t]*) *::= *(.*)$");
    std::smatch re_result;
    if (std::regex_match(rule, re_result, re))
    {
        if (is_terminal(re_result[1]) == false)
        {
            //  insert rules in lists
            jle::vector<std::string> right_symbols = jle::s_split(re_result[2], " ");
            std::list<std::string>  lright_symbols;
            for(unsigned i=0; i<right_symbols.size(); ++i)
            {
                std::string termRule = jle::s_trim(right_symbols[i], ' ');
                if (termRule != "")
                    lright_symbols.push_back(termRule);
            }
            non_terminal_rules[re_result[1]].push_back(
                                        std::make_tuple(lright_symbols, transform2));
        }
        else
        {
            std::string toadd = jle::s_trim(re_result[2], ' ');
            std::list<std::string> lterminal;
            if (toadd[0] == '(' ||  toadd.substr(0,2) == "!(")
            {
            //  TERMINAL:  "regular expresion"
                if (toadd[0] != '!')
                    lterminal.push_back(JLE_SS("^" << toadd/* << ".*"*/));
                else
                    lterminal.push_back(JLE_SS("!^" << toadd.substr(1)/* << ".*"*/));
                terminal_rules[re_result[1]].push_back ( std::make_tuple(lterminal, transform2));
            }
            else if ((toadd.substr(0,2) == "__"  ||  toadd.substr(0,3) == "!__")
                            &&   toadd.substr(toadd.size()-2,2) == "__")
            {
                //  TERMINAL:  predefined consts
                lterminal.push_back(jle::s_trim(toadd, ' '));
                terminal_rules[re_result[1]].push_back ( std::make_tuple(lterminal, transform2));
            }
            else if ( (toadd[0] == '\''  || toadd.substr(0,2) == "!\'")  &&   toadd[toadd.size()-1] == '\'')
            {
                //  TERMINAL:  literals without scapes
                lterminal.push_back(jle::s_trim(toadd, ' '));
                terminal_rules[re_result[1]].push_back ( std::make_tuple(lterminal, transform2));
            }
//            else if (toadd[0] == '['  &&   toadd.substr(toadd.size(),1) == "]")
//            {
//                //  //  TERMINAL:  char lists
//                lterminal.push_back(jle::s_trim(toadd));
//                terminal_rules[re_result[1]].push_back ( std::make_tuple(lterminal, transform2));
//            }
            else
            {
                errors_in_rules = true;
                return std::make_tuple(false, JLE_SS("Error adding rule. TerminalFormat {" << rule << "}   "<<  rule_t2));
            }


        }
    }
    else
    {
        errors_in_rules = true;
        return std::make_tuple(false, JLE_SS("Error adding rule. Incorrect rule format {" << rule << "}   "<<  rule_t2));
    }
    return std::make_tuple(true, JLE_SS("ok"));
}




jle::tuple<bool, std::string> Humble_parser::add_rules_from_stream (std::istream& stream)
{
    clear();
    jle::tuple<bool, std::string> result = make_tuple(true, std::string());
    char buffer[4096];

    stream.getline(buffer, 4096);

    //  The first rule indicates the gramar starting rule
    default_init_symbol = jle::s_trim(buffer, ' ');

    while (stream.getline(buffer, 4096))
    {
        if (adding_template==false  &&  buffer[0] == '/'  &&  buffer[1]=='/')
            continue;
        if (adding_template  ||  jle::s_trim(buffer, ' ') != "")
        {
            //result = add_rule(buffer);
            result = add_line(buffer);
            if (std::get<0>(result) == false)
                return result;
        }
    }

    return result;
}


jle::tuple<bool, std::string> Humble_parser::add_rules_from_string (const std::string& rules)
{
    jle::tuple<bool, std::string> result = make_tuple(true, std::string());

    std::istringstream iss(rules);
    result = add_rules_from_stream(iss);

    return result;

}


jle::tuple<bool, std::string> Humble_parser::add_rules_from_file(const std::string& file_rules)
{
    jle::tuple<bool, std::string> result = make_tuple(true, std::string());
    {
        std::ifstream frules(file_rules.c_str(), std::ios::in);
        result = add_rules_from_stream(frules);
        frules.close();
    }

    return result;
}


jle::tuple<bool, std::string>  Humble_parser::_adding_rule_multi_line(const std::string& line)
{
    if (line != "")
    {
        if (line == "__endrule__")
        {
            adding_rule_multi_line = false;
            building_rule = "";
            return add_rule(building_rule);
        }
        else
        {
            building_rule.push_back(' ');      //  it's necessary because we get lines without nont valid codes
            for (unsigned i=0; i<line.size(); ++i)
            {
                char char2add = line[i];
                if (char2add == '\r'  ||  char2add == '\n'  ||  char2add=='\t')  // it's not necessary. It can be removed
                    char2add = ' ';

                building_rule.push_back(char2add);
            }
            return make_tuple(true, JLE_SS("ok"));
        }
    }
    else
        return make_tuple(true, JLE_SS("ok"));
}



jle::tuple<bool, std::string>  Humble_parser::_adding_template(const std::string& line)
{
    static std::regex  re(R"(^__END_TEMPLATE__::(.*)$)");
    std::smatch re_result;
    if (std::regex_match(line, re_result, re))
    {
        if(re_result.size()==1  ||  (re_result.size()>1 &&  jle::s_trim(re_result[1], ' ')==""))
        {
            templates[template_name] = building_template;
            adding_template = false;
            template_name = "";
            building_template = "";
            return make_tuple(true, JLE_SS("ok"));
        }
        else
            return make_tuple(false, JLE_SS("invalid end template format on  " << line  << "  expected: nothing after __END_TEMPLATE__:: "));
    }
    else
    {
        if(building_template.empty()  ||  false)
            building_template = line;
        else
            building_template = JLE_SS(building_template << std::endl << line);
        return make_tuple(true, JLE_SS("ok"));
    }
    /*
    std::regex  re(R"(^__END_TEMPLATE__:: *([A-Z][A-Z0-9_]*)$)");
    std::smatch re_result;

    if (std::regex_match(line, re_result, re))
    {
        if(template_name == re_result[1])
        {
            templates[template_name] = building_template;
            adding_template = false;
            template_name = "";
            building_template = "";
            return make_tuple(true, JLE_SS("ok"));
        }
        else
            return make_tuple(false, JLE_SS("invalid template name on  " << line  << "  expected:"  << template_name << "."));
    }
    else
    {
        if(building_template.empty()  ||  false)
            building_template = line;
        else
            building_template = JLE_SS(building_template << std::endl << line);
        return make_tuple(true, JLE_SS("ok"));
    }*/
}



jle::tuple<bool, std::string>
Humble_parser::add_line (std::string line)
{
    if (adding_rule_multi_line)
    {
        line = jle::s_trim(line, ' ');
        return _adding_rule_multi_line(line);
    }
    else if (adding_template)
    {
        return _adding_template(line);
    }
    else
    {
        if (line == "__beginrule__")
        {
            adding_rule_multi_line = true;
            building_rule = "";
            return make_tuple(true, JLE_SS("ok"));
        }
        else if (line.find("__BEGIN_TEMPLATE__::")==0)
        {
            adding_template = true;
            building_template = "";

            static std::regex  re{R"(__BEGIN_TEMPLATE__:: *([A-Z][A-Z0-9_]*))"};
            std::smatch re_result;

            if (std::regex_match(line, re_result, re)== false)
                return make_tuple(false, JLE_SS("invalid template name on   " << line << ". "));

            template_name = re_result[1];
            if(template_name.empty())
                return make_tuple(false, JLE_SS("missing template name " << line << ". "));
            else {
                template_name = re_result[1];
                return make_tuple(true, JLE_SS("ok"));
            }
        }
        else
            return add_rule(line);
    }
}





jle::tuple<bool, std::string/*remaining*/, AST_node_item>
Humble_parser::parse(const std::string& input, const std::string& init) const
{
    AST_node_item ast_root(init, "");
    ast_root_debug = &ast_root;    //  debug
    try
    {
        deeper_error_parsing_pos = -1;
        deeper_error_expected_symbols.clear();

        if (exist_errors_in_rules())
            return std::make_tuple(false, JLE_SS("CANNOT PARSE  there are errors in rules "), ast_root);

        bool result = false;

        int remaining_str2_parse_pos;
        string2parse = input;
        std::tie(result, remaining_str2_parse_pos) = execute_non_terminal(0, init, ast_root);

        std::string remaining_input = string2parse.substr(remaining_str2_parse_pos);

        if (result==false  ||  remaining_input!= "")
        {
            std::ostringstream expected_symbols;
            if (deeper_error_expected_symbols.size() > 0)
            {
                expected_symbols  <<  "   expected:  ";
                for(auto it = deeper_error_expected_symbols.begin(); it != deeper_error_expected_symbols.end(); ++it)
                    expected_symbols << " " << *it;
            }
            return std::make_tuple(false, JLE_SS("ERROR parsing... " << string2parse.substr(deeper_error_parsing_pos, 50) << "." << expected_symbols.str()), ast_root);
        }
        else
            return std::make_tuple(true, JLE_SS("ok"), ast_root);
    }
    catch(const std::string& error)
    {
            return std::make_tuple(false, error, ast_root);
    }
}


jle::tuple<bool, std::string/*remaining*/, AST_node_item>
Humble_parser::multi_parse(const std::string& input, std::string minit) const
{

    if (minit == "")
        minit = default_init_symbol;
    if (minit == "")
        return std::make_tuple(false, JLE_SS("CANNOT PARSE  missing init symbol"), AST_node_item("<none>"));


    //  splitting elements

    deeper_error_parsing_pos = -1;
    deeper_error_expected_symbols.clear();

    if (exist_errors_in_rules())
        return std::make_tuple(false, JLE_SS("CANNOT PARSE  there are errors in rules "), AST_node_item("<none>"));

    std::string new_input = input;
    AST_node_item ast_result("<pending>");
    static std::regex re("([^,]+)");

    auto re_begin =
        std::sregex_iterator(minit.begin(), minit.end(), re);
    auto re_end = std::sregex_iterator();

    for (std::sregex_iterator i = re_begin; i != re_end; ++i) {
        bool result = false;
        std::string remaining_input;
        std::tie(result, remaining_input, ast_result) =  parse(new_input, i->str());
        if (result==false  ||  remaining_input!= "ok")
            return std::make_tuple(result, remaining_input, ast_result);
        ast_result.exec_replace(templates, {});
        new_input =  ast_result.value;
    }

    return std::make_tuple(true, JLE_SS("ok"), ast_result);
}


void Humble_parser::clear(void)
{
    string2parse = "";
    non_terminal_rules.clear();
    terminal_rules.clear();
    errors_in_rules = false;
    deeper_error_parsing_pos = -1;
    deeper_error_expected_symbols.clear();
    default_init_symbol = "";
}









// ----------------------------------------------------------------------------------------------------------------------------
jle::tuple<bool, int>  Humble_parser::execute_predefined_var(int str2parse_pos, const std::string& _terminal_rule, AST_node_item& ast_node, const std::string& rule2replace) const
{
    //  predefined constants     ------------------------------------------------------------

//    if (str2parse_pos >= int(string2parse.size()))
//        return std::make_tuple(false, str2parse_pos);

    auto result = std::make_tuple(false, str2parse_pos);


    std::string  terminal_rule;
    bool positive = true;
    if(_terminal_rule[0] != '!')
    {
        terminal_rule = _terminal_rule;
        positive = true;
    }
    else
    {
        terminal_rule = _terminal_rule.substr(1);
        positive = false;
    }

    auto check_funct_positive = [positive=positive](bool check_result) {
        if(check_result  &&  positive)
            return true;
        else if(!check_result  &&  !positive)
            return true;
        else
            return false;
    };

    auto check_char = [&](std::function<bool (char)> f) {
        char char2check = string2parse[str2parse_pos];
        if(check_funct_positive(f(char2check)))
        {
            ast_node.value        = char2check;
            ast_node.rule4replace = rule2replace;
            result = std::make_tuple(true, str2parse_pos+1);
        }
    };


    if (terminal_rule  == "__any__") {
        auto local_check = [&](char) { return str2parse_pos >= int(string2parse.size()); };
        check_char(local_check);
    }
    else if (terminal_rule  == "__isalpha__")
    {
        check_char(isalpha);
    }
    else if (terminal_rule  == "__islower__")
    {
        check_char(islower);
    }
    else if (terminal_rule  == "__isupper__")
    {
        check_char(isupper);
    }
    else if (terminal_rule  == "__isdigit__")
    {
        check_char(isdigit);
    }
    else if (terminal_rule  == "__isalnum__")
    {
        check_char(isalnum);
    }

    else if (terminal_rule  == "__endl__")
    {
        auto local_check = [&](char) { return string2parse[str2parse_pos] == '\n'  ||  string2parse[str2parse_pos] == '\r'; };
        check_char(local_check);
    }
    else if (terminal_rule  == "__isspace__")
    {
        check_char(isspace);
    }
    else if (terminal_rule  == "__isspace*__")
    {
        int counter=0;
        char char2check = string2parse[str2parse_pos+counter];
        while (check_funct_positive(isspace(char2check)))     //  it's not necessary to verify the string end (\0 no es igual)
        {
            if (counter == 0)
                ast_node.rule4replace  = rule2replace;
            ast_node.value        += char2check;
            counter++;
            char2check = string2parse[str2parse_pos+counter];
        }
        return std::make_tuple(true, str2parse_pos+counter);
    }
    else if (terminal_rule  == "__isspace+__")
    {
        int counter=0;
        char char2check = string2parse[str2parse_pos+counter];
        while (check_funct_positive(isspace(char2check)))     //  it's not necessary to verify the string end (\0 no es igual)
        {
            if (counter == 0)
                ast_node.rule4replace  = rule2replace;
            ast_node.value        += char2check;
            counter++;
            char2check = string2parse[str2parse_pos+counter];
        }
        if (counter == 0)
            return std::make_tuple(false, str2parse_pos+counter);
        else
            return std::make_tuple(true, str2parse_pos+counter);
    }
    else if (terminal_rule  == "__space_tab__")
    {
        auto local_check = [&](char) { return string2parse[str2parse_pos]== '\t'  ||  string2parse[str2parse_pos]== ' '; };
        check_char(local_check);
    }
    else
        throw  JLE_SS("unknown constant [" << terminal_rule  << "]"  <<  std::endl);


    return result;
}


jle::tuple<bool, int> Humble_parser::execute_regular_expresion(int str2parse_pos, const std::string& terminal_rule, AST_node_item& ast_node, const std::string& rule2replace) const
{
    int  remaining_str2_parse_pos = str2parse_pos;
    std::string re2eval;
    bool positiv_eval = true;
    if ( terminal_rule[0] != '!')
    {
        re2eval = JLE_SS(terminal_rule << R"((.|\r|\n)*)");
        positiv_eval = true;
    }
    else
    {
        re2eval = JLE_SS(terminal_rule.substr(1) << R"((.|\r|\n)*)");
        positiv_eval = false;
    }

    std::regex re_terminal{re2eval};
    if (remaining_str2_parse_pos > int(string2parse.size()))
        std::cerr << "ERROR... " << std::endl;


    if (positiv_eval)
    {
        std::smatch re_result;
        //  http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#2329
        std::string re_bug = string2parse.substr(remaining_str2_parse_pos);
        if(std::regex_match(re_bug, re_result, re_terminal))
        {
            ast_node.value        = re_result[1];
            remaining_str2_parse_pos += int(ast_node.value.size());
            ast_node.rule4replace = rule2replace;
            return std::make_tuple(true, remaining_str2_parse_pos);
        }
    }
    else
    {
        if (remaining_str2_parse_pos >= int(string2parse.size()))
            return std::make_tuple(false, remaining_str2_parse_pos);
        std::smatch re_result;
        //  http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#2329
        const std::string re_bug = string2parse.substr(remaining_str2_parse_pos);
        if(std::regex_match(re_bug, re_result, re_terminal) == false)
        {
            ast_node.value        = string2parse.substr(remaining_str2_parse_pos, 1);
            remaining_str2_parse_pos += 1;
            ast_node.rule4replace = rule2replace;
            return std::make_tuple(true, remaining_str2_parse_pos);
        }
    }
    return std::make_tuple(false, str2parse_pos);
}



jle::tuple<bool, int> Humble_parser::execute_literal(int str2parse_pos, const std::string& _terminal_rule, AST_node_item& ast_node, const std::string& rule2replace) const
{
    if (str2parse_pos > int(string2parse.size()))
        std::cerr << "ERROR... " << std::endl;
    int  remaining_str2_parse_pos = str2parse_pos;
    std::string terminal_rule;
    bool positiv_eval = true;
    if ( _terminal_rule[0] != '!')
    {
        terminal_rule = _terminal_rule.substr(1, _terminal_rule.size()-2);
        positiv_eval = true;
    }
    else
    {
        terminal_rule = _terminal_rule.substr(2, _terminal_rule.size()-3);
        positiv_eval = false;
    }
    if (str2parse_pos+int(terminal_rule.size()) > int(string2parse.size()))
        return std::make_tuple(false, str2parse_pos);



    std::string substring2parse =  string2parse.substr(remaining_str2_parse_pos, terminal_rule.size());
    if (positiv_eval)
    {
        if (terminal_rule ==  substring2parse)
        {
            ast_node.value        = substring2parse;
            remaining_str2_parse_pos += int(ast_node.value.size());
            ast_node.rule4replace = rule2replace;
            return std::make_tuple(true, remaining_str2_parse_pos);
        }
    }
    else
    {
        if (remaining_str2_parse_pos >= int(string2parse.size()))
            return std::make_tuple(false, remaining_str2_parse_pos);
        if (terminal_rule !=  substring2parse)
        {
            ast_node.value        = string2parse.substr(remaining_str2_parse_pos, 1);
            remaining_str2_parse_pos += 1;
            ast_node.rule4replace = rule2replace;
            return std::make_tuple(true, remaining_str2_parse_pos);
        }
    }
    return std::make_tuple(false, str2parse_pos);
}


std::list<std::string>   Humble_parser::get_terminal_rules(void)const
{
    std::list<std::string> result;
    for(auto it = terminal_rules.begin(); it != terminal_rules.end(); ++it)
    {
        result.push_back(it->first);
    }
    return result;
}

std::list<std::string>   Humble_parser::get_non_terminal_rules(void)const
{
    std::list<std::string> result;
    for(auto it = non_terminal_rules.begin(); it != non_terminal_rules.end(); ++it)
    {
        result.push_back(it->first);
    }
    return result;
}


void Humble_parser::set_var(const std::string& name, const std::string& value)
{
    templates[name] = value;
}


}; };  //  namespace jle {   namespace hp  {
