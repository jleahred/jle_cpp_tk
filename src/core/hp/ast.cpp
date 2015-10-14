#include "ast.h"

#include <regex>
#include <map>
#include "core/string.h"
#include "core/cont/vector.hpp"


//namespace jle {
//template<typename T1, typename T2>
//std::ostream& operator<< (std::ostream& os, const jle::map<T1, T2>&  m)
//{
//    auto it = m.cbegin();
//    while(it!=m.cend())
//    {
//        os << "(" << it->first << ", " << it->second << ") ";
//        ++it;
//    }

//    return os;
//}

//template<typename T1, typename T2>
//std::ostream& operator<< (std::ostream& os, jle::map<T1, T2>&  m)
//{
//    auto it = m.begin();
//    while(it!=m.end())
//    {
//        os << "(" << it->first << ", " << it->second << ") ";
//        ++it;
//    }

//    return os;
//}

//}
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
                                jle::map<std::string, std::string>                  renamed_templates);
std::string replace_transf2(    AST_node_item&                                      current_node,
                                const std::string&                                  founded,
                                const std::string&                                  rule4replace,
                                const jle::map<std::string /*name*/, std::string>&  templates,
                                jle::map<std::string, std::string>                  renamed_templates);


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
                                 jle::map<std::string, std::string> ren_templ)
{
    //  first we look down and later to right (same level)
    if (this->down.expired()==false)
    {
        this->down->exec_replace(templates, ren_templ);
        //this->value = this->down->value;
    }

    if (this->next.expired()==false)
    {
        this->next->exec_replace(templates, ren_templ);
        //this->value += this->next->value;
    }

    //  check childs
    {
        if (this->down.expired())
        {
            if (rule4replace!="")
                value = replace_transf2(*this, value, rule4replace, templates, ren_templ);
            return;
        }

        if (rule4replace !="")
            this->value = replace_transf2(*this, get_map_found(this->down), rule4replace, templates, ren_templ);
        else
            this->value = exec_replace_no_vars(this->down, rule4replace);
    }
}



std::string replace_transf2(        AST_node_item&                                      current_node,
                                    const std::string&                                  founded,
                                    const std::string&                                  rule4replace,
                                    const jle::map<std::string /*name*/, std::string>&  templates,
                                    jle::map<std::string, std::string>                  renamed_templates)
{
    if (jle::s_trim(rule4replace, ' ') !="")
    {
        std::map<std::string, std::string> map_found;
        map_found["t"] = founded;
        return replace_transf2(current_node, map_found, rule4replace, templates, renamed_templates);
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
    std::tuple<std::string, jle::vector<std::string> >  get_command_and_params(const std::string& full_command)
    {
        auto v = jle::s_split(full_command, " ");
        if(v.empty()==false)
            return std::make_tuple(v[0], v);
        else
            return std::make_tuple("", v);
    }


    std::string   get_template_content(const std::string& templ,
                                         const jle::map<std::string /*name*/, std::string>& templates,
                                         const jle::map<std::string /*name*/, std::string>& templ_aliases)
    {
        JLE_COUT_TRACE(templ)
if(templ_aliases.empty()==false) {
JLE_COUT_TRACE(templ_aliases.cbegin()->first)
        JLE_COUT_TRACE(templ_aliases.cbegin()->second)
        }
        auto effective_tmpl_name = templ;

        auto fta = templ_aliases.find(effective_tmpl_name);
        if(fta!=templ_aliases.cend())
        {
            effective_tmpl_name = fta->second;
        }
        JLE_COUT_TRACE(effective_tmpl_name)
        auto ft = templates.find(effective_tmpl_name);
        if(ft != templates.cend())
            return ft->second;
        else
            return JLE_SS("TEMPLATE NOT FOUND (" << templ << ")");
    }
}



std::string replace_transf2(    AST_node_item&                                      current_node,
                                std::map<std::string, std::string>                  map_items_found,
                                const std::string&                                  _rule4replace,
                                const jle::map<std::string /*name*/, std::string>&  templates,
                                jle::map<std::string, std::string>                  renamed_templates)
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
//        if(col>0)
//        JLE_COUT_TRACE(col)
        auto full_ident = JLE_SS(ident << std::string(col, ' '));
        add  = replace_string(add, "\n", JLE_SS("\n" << full_ident));
        result = JLE_SS(result << add);
        add  = "";
    };


    std::function<void(const std::string)> exec_rule_for_replace = [&](const std::string& rule4replace) {
        size_t  col = 0;
        std::string::size_type previus = 0;
        for (std::string::size_type i=0; i<rule4replace.size()-1; ++i)
        {
            if (rule4replace.size()>i  &&   rule4replace[i] == '$'  &&  rule4replace[i+1] == '(')
            {
                //  eureka
                add += rule4replace.substr(previus, i-previus);
                i+=2;
                std::string::size_type initVar = i;
                while (i<rule4replace.size()  &&  rule4replace[i] != ')')
                    ++i;

                std::string  full_command = rule4replace.substr(initVar, i-initVar);
                auto command_and_params = get_command_and_params(full_command);
                std::string var_name = std::get<0>(command_and_params);

                if(std::get<1>(command_and_params).size()==1)        //  no params
                {
                    std::map<std::string, std::string>::const_iterator it = map_items_found.find(var_name);
                    std::map<std::string, std::string>::const_iterator itPredefined = map_predefined_vars.find(var_name);
                    if (it != map_items_found.end())        //  add var
                    {
                        add2result(0);
                        add += it->second;
                        add2result(col);
                    }
                    else if (itPredefined != map_predefined_vars.end())
                        add += itPredefined->second;
                    else if(var_name == "__ident+__") {
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
                            current_node.down->exec_replace(templates, renamed_templates);
                        //  TODO
//                        JLE_COUT_TRACE((++map_items_found.cbegin())->first)
//                        JLE_COUT_TRACE((++map_items_found.cbegin())->second)
                        map_items_found = get_map_found(current_node.down);
//                        JLE_COUT_TRACE((++map_items_found.cbegin())->first)
//                        JLE_COUT_TRACE((++map_items_found.cbegin())->second)
//                                std::cout << std::endl;
                    }
                    else
                    {
                        add += JLE_SS("unknown (" << var_name << ")");
                    }
                }
                else
                {
                    if(var_name == "__template__")
                    {
                        if(std::get<1>(command_and_params).size()!=2)
                        {
                            add += JLE_SS("invalid param count (" << full_command << ")");
                        }
                        else {
                            //if(current_node.down.expired()==false)
                            //    current_node.down->exec_replace(templates, renamed_templates);
                            //map_items_found = get_map_found(current_node.down);
                            //add += replace_transf2(current_node, map_items_found, get_template_content(std::get<1>(command_and_params)[1], templates, renamed_templates), templates, renamed_templates);
                            //rule4replace = JLE_SS(get_template_content(std::get<1>(command_and_params)[1], templates, renamed_templates) << rule4replace);
                            exec_rule_for_replace(get_template_content(std::get<1>(command_and_params)[1], templates, renamed_templates));
                        }
                    }
                    else if(var_name == "__rename__")
                    {
                        if(std::get<1>(command_and_params).size()!=3)
                        {
                            add += JLE_SS("invalid param count (" << full_command << ")");
                        }
                        else {
                            renamed_templates[std::get<1>(command_and_params)[1]] = std::get<1>(command_and_params)[2];
                            //add += replace_transf2(*(current_node.down), map_items_found, get_template_content(std::get<1>(command_and_params)[1], templates, renamed_templates), templates, renamed_templates);
                        }
                    }
                    else
                    {
                        add += JLE_SS("unknown (" << full_command << ")");
                    }
                }
                previus = i+1;
            }
            else if (rule4replace[i] == '\n'  ||  rule4replace[i] == '\r')
                col=0;
            else
            {
    //            JLE_COUT_TRACE(rule4replace[i])
    //            JLE_COUT_TRACE(col)
                ++col;
            }

            add2result(0);
        }
        result += rule4replace.substr(previus);
    };
    exec_rule_for_replace(_rule4replace);

    return result;

}


//namespace  {

//    struct Templ_running_status {
//        std::string  generated_text;
//        std::string  command;
//        std::string  text_pending;
//    };

//    std::string   get_template_content(const std::string& templ,
//                                         const jle::map<std::string /*name*/, std::string>& templates,
//                                         const jle::map<std::string /*name*/, std::string>& templ_aliases)
//    {
//        auto effective_tmpl_name = templ;

//        auto fta = templ_aliases.find(effective_tmpl_name);
//        if(fta!=templ_aliases.cend())
//        {
//            effective_tmpl_name = fta->second;
//        }

//        auto ft = templates.find(effective_tmpl_name);
//        if(ft != templates.cend())
//            return ft->second;
//        else
//            return "TEMPLATE NOT FOUND";
//    }
//    bool find_template_command(Templ_running_status& templ_run_status)
//    {
////        templ_run_status.generated_text = JLE_SS(templ_run_status.generated_text << templ_run_status.text_pending);
////        templ_run_status.text_pending = "";
////        templ_run_status.command = "";
////        return false;

//        std::regex  re{R"(((.|\n|\r)*?)__(RUN|INSERT|RENAME)__::((.|\n|\r)*))"};
//        std::smatch re_result;

//        if (std::regex_match(templ_run_status.text_pending, re_result, re)==false)
//        {
//            templ_run_status.generated_text = JLE_SS(templ_run_status.generated_text << templ_run_status.text_pending);
//            templ_run_status.text_pending = "";
//            templ_run_status.command = "";

//            return false;
//        }
//        else
//        {
//            auto text_pending = JLE_SS(re_result[4]);
//            auto command      = JLE_SS(re_result[3]);
//            auto generated_text = JLE_SS(re_result[1]);

//            templ_run_status.generated_text = JLE_SS(templ_run_status.generated_text << generated_text);
//            templ_run_status.text_pending = JLE_SS(text_pending);
//            templ_run_status.command = JLE_SS(command);

//            return true;
//        }
//    }

//}


//void AST_node_item::AST_node_item::exec_replace_current_templ(const jle::map<std::string /*name*/, std::string>& templates)
//{
//    //  find template  (first, look for alias)

//    //  while find template command
//        //  write text on value till first command
//        //  execute command
//    //  write text till end


//    std::map<std::string, std::string>  map_found = get_map_found(this->down);
//    auto templ_running_status = Templ_running_status{};
//    templ_running_status.text_pending = get_template_content(this->rule4replace, templates, {});
//    while(find_template_command(templ_running_status))
//    {
//        if(templ_running_status.command == "INSERT")
//        {
//            auto found_var_content = map_found.find("id");
//            if(found_var_content != map_found.end())
//                templ_running_status.generated_text = JLE_SS(templ_running_status.generated_text << found_var_content->second);
//            else
//                templ_running_status.generated_text = JLE_SS(templ_running_status.generated_text << "UNKNOWN VAR $$" << "id" << "$$" );
//        }
//        else
//            templ_running_status.generated_text = JLE_SS(templ_running_status.generated_text << "UNKNOWN COMMAND $$" << templ_running_status.command<< "$$" );
//    }

//    this->value = templ_running_status.generated_text;

//    //this->value = JLE_SS("pending... " << this->rule4replace << std::endl);
//}





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
