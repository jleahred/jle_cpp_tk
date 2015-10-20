#include "ast.h"

#include <regex>
#include <map>

#include "core/tuple.hpp"
#include "core/string.h"
#include "core/cont/vector.hpp"


namespace jle {   namespace hp  {


/**
    Adds in brother
*/
jle::shared_ptr<AST_node_item> AST_node_item::add_next(const std::string& _name, const std::string& _value, const std::string& _rule4replace)
{
    next = jle::shared_ptr<AST_node_item>(new AST_node_item(_name, _value, _rule4replace));
    return next;
}

/**
    Adds in child
*/
jle::shared_ptr<AST_node_item> AST_node_item::add_child(const std::string& _name, const std::string& _value, const std::string& _rule4replace)
{
    down = jle::shared_ptr<AST_node_item>(new AST_node_item(_name, _value, _rule4replace));
    return down;
}



std::string AST_get_string_nodes_on_level(const AST_node_item* const node)
{
    std::ostringstream result;

//    if (node==0)
//        return "nil";

    const AST_node_item* current_node = node;

    while (true)
    {
        result << "NODE NAME:    " << current_node->name;
        result << "             code:         " << long(current_node) << std::endl;
        if (current_node->next.expired())
            break;
        current_node = current_node->next.get();
    }

    return result.str();
}


std::string AST_get_string_nodes(const jle::shared_ptr<AST_node_item>& node)
{
    std::ostringstream result;

//    if (node==0)
//        return "nil";

    //  first we look down and later to right (same level)

    result << "--------------------------------------------------------------" << std::endl;
    result << "NODE NAME:    " << node->name            << std::endl;
    result << "code:         " << long(node.get())      << std::endl;
    result << "value:        " << node->value           << std::endl;
    result << "rule4replace: " << node->rule4replace    << std::endl;
    result << "childs:       " << std::endl;

        if (node->down.expired()==false)
            result << AST_get_string_nodes_on_level(node->down.get());

    result << "--------------------------------------------------------------" << std::endl;


    if (node->down.expired()==false)
        result << AST_get_string_nodes(node->down);

    if (node->next.expired()==false)
        result << AST_get_string_nodes(node->next);


    return result.str();

}



/*
    Replace methods
*/

std::string replace_transf2(    AST_node_item&                                      current_node,
                                std::map<std::string, std::string>                  map_items_found,
                                const std::string&                                  rule4replace,
                                const jle::map<std::string /*name*/, std::string>&  templates,
                                jle::map<std::string, std::string>                  declared_vars);
std::string replace_transf2(    AST_node_item&                                      current_node,
                                const std::string&                                  founded,
                                const std::string&                                  rule4replace,
                                const jle::map<std::string /*name*/, std::string>&  templates,
                                jle::map<std::string, std::string>                  declared_vars);


namespace {

    std::string  exec_replace_no_vars(shared_ptr<AST_node_item> current_node, const std::string&  rule4replace)
    {
        std::ostringstream os;

        while (current_node.expired()==false)
        {
            if (jle::s_trim(rule4replace, ' ') =="")
                os << current_node->value;
            current_node = current_node->next;
        }
        return os.str();
    }

    std::map<std::string, std::string>  get_map_found(shared_ptr<AST_node_item> current_node)
    {
        std::map<std::string, std::string>  map_found;

        while (current_node.expired()==false)
        {
            std::string symbol = current_node->name;
            map_found[symbol] = current_node->value;

            current_node = current_node->next;
        }

        return map_found;
    }

}


void AST_node_item::exec_replace(const jle::map<std::string /*name*/, std::string>& templates,
                                 jle::map<std::string, std::string> declared_vars)
{
    //  first we look down and later to right (same level)
    if (this->down.expired()==false)
    {
        this->down->exec_replace(templates, declared_vars);
        //this->value = this->down->value;
    }

    if (this->next.expired()==false)
    {
        this->next->exec_replace(templates, declared_vars);
        //this->value += this->next->value;
    }

    //  check childs
    {
        if (this->down.expired())
        {
            if (rule4replace!="")
                value = replace_transf2(*this, value, rule4replace, templates, declared_vars);
            return;
        }

        if (rule4replace !="")
            this->value = replace_transf2(*this, get_map_found(this->down), rule4replace, templates, declared_vars);
        else
            this->value = exec_replace_no_vars(this->down, rule4replace);
    }
}



std::string replace_transf2(        AST_node_item&                                      current_node,
                                    const std::string&                                  founded,
                                    const std::string&                                  rule4replace,
                                    const jle::map<std::string /*name*/, std::string>&  templates,
                                    jle::map<std::string, std::string>                  declared_vars)
{
    if (jle::s_trim(rule4replace, ' ') !="")
    {
        std::map<std::string, std::string> map_found;
        map_found["t"] = founded;
        return replace_transf2(current_node, map_found, rule4replace, templates, declared_vars);
    }
    else
        return founded;
}



namespace {
    std::string replace_string(const std::string& subject,
                               const std::string& search,
                               const std::string& replace) {
        auto result = subject;
        size_t pos = 0;
        while((pos = result.find(search, pos)) != std::string::npos) {
             result.replace(pos, search.length(), replace);
             pos += replace.length();
        }
        return result;
    }

//    jle::tuple<std::string, jle::vector<std::string> >  get_command_and_params(const std::string& full_command)
//    {
//        auto v = jle::s_split(full_command, " ", true);
//        if(v.empty()==false)
//            return std::make_tuple(v[0], v);
//        else
//            return std::make_tuple("", v);
//    }

    jle::tuple<std::string, std::string>  get_id(const std::string&  from)
    {
        std::regex  re_rt(R"(([a-zA-Z_][a-zA-Z_0-1]*[\*]?)[ \n\r]?((.|\n|\r)*)?)");
        std::smatch re_result;

        if (std::regex_match(from, re_result, re_rt))
        {
            std::string  id = jle::s_trim(re_result[1], ' ');
            std::string  params = jle::s_trim(re_result[2], ' ');
            return std::make_tuple(id, params);
        }
        else
        {
            return std::make_tuple(JLE_SS("invalid id " << from), "");
        }
    }


    std::string   get_template_content(const std::string& var_name,
                                         const jle::map<std::string /*name*/, std::string>& templates,
                                         const jle::map<std::string /*name*/, std::string>& declared_vars)
    {
        auto ft = templates.find(var_name);
        if(ft != templates.cend())
            return ft->second;
        else {
            auto fta = declared_vars.find(var_name);
            if(fta!=declared_vars.cend())
            {
                return fta->second;
            }
        }
       return JLE_SS("TEMPLATE NOT FOUND (" << var_name << ")");
    }
 }



std::string replace_transf2(    AST_node_item&                                      current_node,
                                std::map<std::string, std::string>                  map_items_found,
                                const std::string&                                  _rule4replace,
                                const jle::map<std::string /*name*/, std::string>&  templates,
                                jle::map<std::string, std::string>                  declared_vars)
{
    static int replace_counter = 0;
    ++replace_counter;

    //  predefined vars on trasform
    static std::map<std::string, std::string> map_predefined_vars;
    if (map_predefined_vars.size() == 0)
    {
        map_predefined_vars["__nothing__"] = "";
        map_predefined_vars["__endl__"] =  JLE_SS("\n");  //JLE_SS(std::endl);
        map_predefined_vars["__space__"] = " ";
        map_predefined_vars["__dollar_open_par__"] = "$(";
        map_predefined_vars["__close_par__"] = ")";
    }
    map_predefined_vars["__counter__"] = JLE_SS(replace_counter);


    //  look for vars to replace
        //  el texto hasta el inicio de la siguiente variable se añade como está
        //  luego se añade el reemplazo de la variable


    std::string result;
    auto ident = std::string("");
    std::string add;

    auto add2result = [&add, &result, ident=ident](size_t col) {
        //if(add.empty()  ||  (add.find('\r') == std::string::npos  &&  add.find('\n') == std::string::npos))
        if(add.empty())
            return;
        auto full_ident = JLE_SS(ident << std::string(col, ' '));
        add  = replace_string(add, "\n", JLE_SS("\n" << full_ident));
        result = JLE_SS(result << add);
        add  = "";
    };


    size_t  col = 0;
    auto process_full_commnand = [&](const std::string& full_command, std::function<void(const std::string)> exec_rule_for_replace) {
        auto id_params = get_id(full_command);
        std::string id = std::get<0>(id_params);

        if(id.empty()==false  &&  std::get<1>(id_params).empty())        //  id without params
        {
            std::string var_name = id;
            std::map<std::string, std::string>::const_iterator it = map_items_found.find(var_name);
            std::map<std::string, std::string>::const_iterator itPredefined = map_predefined_vars.find(var_name);
            jle::map<std::string, std::string>::const_iterator itTemplates =  templates.find(var_name);
            auto found_var = declared_vars.find(id);
            if (it != map_items_found.end())        //  add var
            {
                add2result(0);
                add += it->second;
                add2result(col);
            }
            else if (itPredefined != map_predefined_vars.end()) {
                add += itPredefined->second;
                if(itPredefined->first == "__endl__")
                    col=0;
            } else if (itTemplates != templates.cend()  ||  found_var!=declared_vars.end()) {
                exec_rule_for_replace(get_template_content(var_name, templates, declared_vars));
            } else if(var_name == "__ident+__") {
                ident = JLE_SS(ident << "  ");
                add += JLE_SS("\n");
            }
            else if(var_name == "__ident-__"  &&  ident.length()>=2) {
                ident = ident.substr(0, ident.length()-2);
                add += JLE_SS("\n");
            }
            else if(var_name == "__date_time__") {
                add += JLE_SS(jle::chrono::now());
            }
            else if(var_name == "__date__") {
                add += JLE_SS(jle::chrono::today());
            }
            else if(var_name == "__run__") {
                // update map_items_found
                if(current_node.down.expired()==false)
                    current_node.down->exec_replace(templates, declared_vars);
                map_items_found = get_map_found(current_node.down);
            }
            else
            {
                add += JLE_SS("unknown (" << var_name << ")");
                add2result(0);
            }
        }
        else
        {
//                    if(var_name == "__get__")
//                    {
//                        if(std::get<1>(command_and_params).size()!=2)
//                        {
//                            add += JLE_SS("invalid param count (" << full_command << ")");
//                        }
//                        else {
//                            exec_rule_for_replace(get_template_content(std::get<1>(command_and_params)[1], templates, declared_vars));
//                        }
//                    }
//                    else
            if(id == "__set__")
            {
                auto id_params_command = get_id(std::get<1>(id_params));
                auto var_name = std::get<0>(id_params_command);
                auto param  = std::get<1>(id_params_command);
                if(var_name.empty())
                    add += JLE_SS("invalid var_name on  (" << full_command << ")");
                else if(param.empty())
                {
                    add += JLE_SS("empty param  (" << full_command << ")");
                }
                else {
                    declared_vars[var_name] = param;
                    //add += replace_transf2(*(current_node.down), map_items_found, get_template_content(std::get<1>(command_and_params)[1], templates, declared_vars), templates, declared_vars);
                }
            }
            else
            {
        add += JLE_SS("unknown (" << full_command << ") with id (" << id << ")");
                add2result(0);
            }
        }
    };

    std::function<jle::tuple<std::string, size_t, size_t>(const std::string&, size_t, size_t)>
    find_next_command = [&](const std::string& rule4replace, size_t start, size_t previous){
        for (std::string::size_type i=start; i<rule4replace.size()-1; ++i)
        {
            if (rule4replace.size()>i  &&   rule4replace[i] == '$'  &&  rule4replace[i+1] == '(')
            {
                //  eureka
                add += rule4replace.substr(previous, i-previous);
                i+=2;
                std::string::size_type initVar = i;
                int depth=0;
                while (i<rule4replace.size()  &&  (rule4replace[i] != ')'   ||   depth!=0)) {
                    if (rule4replace.size()>i  &&   rule4replace[i] == '$'  &&  rule4replace[i+1] == '(')
                        ++depth;
                    if (rule4replace[i] == ')')
                        --depth;
                    ++i;
                }
                std::string  full_command = rule4replace.substr(initVar, i-initVar);
                previous = i+1;
                return std::make_tuple(full_command, i+1, previous);
            }
            else if (rule4replace[i] == '\n'  ||  rule4replace[i] == '\r')
                col=0;
            else
            {
                ++col;
            }
        }
        return std::make_tuple(std::string{}, size_t{0}, previous);
    };

    std::function<void(const std::string)> exec_rule_for_replace = [&](const std::string& rule4replace) {
        col = 0;
        size_t pos = 0;
        size_t previous = 0;
        std::string full_command;

        std::tie(full_command, pos, previous) = find_next_command(rule4replace, pos, previous);
        while(full_command.empty() == false) {
            process_full_commnand(full_command, exec_rule_for_replace);
            std::tie(full_command, pos, previous) = find_next_command(rule4replace, pos, previous);
        }

        if(rule4replace.size()>previous)
            result += rule4replace.substr(previous);
    };



    exec_rule_for_replace(_rule4replace);

    return result;

}



bool  in_list   (const jle::list<std::string>& lmodes2mantein, const std::string& value)
{
    for(auto it = lmodes2mantein.cbegin(); it != lmodes2mantein.cend(); ++it)
    {
        if (*it  ==  value)
            return true;
    }
    return false;
}


void  AST_node_item::remove_nodes_manteining   (const jle::list<std::string>& lmodes2mantein)
{
    //  if down is an element to remove...
        //  down = down->next

    //  if next is an element to remove...
        //  next = next->next


    //  recursive  down
    //  recursive  next



    if (this->down.expired()==false)
    {
        //  if down is an element to remove...
        if (in_list(lmodes2mantein, this->down->name) == false)
        {
            //  down =   down->next
            if (this->down->next.expired()==false)
            {
                this->down = this->down->next;
                //  recursive down
                this->remove_nodes_manteining(lmodes2mantein);

            }
            else
                this->down = jle::shared_ptr<AST_node_item>();
        }
        else
        //  recursive down
            this->down->remove_nodes_manteining(lmodes2mantein);
    }


        //  if next is an element to remove...
        if (this->next.expired()==false)
        {
            if (in_list(lmodes2mantein, this->next->name) == false)
            {
                //  next =   next->next
                if (this->next->next.expired()==false)
                {
                    this->next = this->next->next;
                    this->remove_nodes_manteining(lmodes2mantein);
                }
                else
                    //  recursive next
                    this->next = jle::shared_ptr<AST_node_item>();
            }
            else
                //  recursive next
                this->next->remove_nodes_manteining(lmodes2mantein);

        }



}





}; };  //  namespace jle {  namespace hp  {
