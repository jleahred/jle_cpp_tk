#include "ast.h"

#include <regex.h>
#include <map>
#include "core/string.h"





namespace jle {   namespace hp  {


std::ostream& operator<< (std::ostream& os, const Rule4replace& r4r)
{
    os << "{";
    switch (r4r.type) {
    case rule4replace_type::none:
        os << "none";
        break;
    case rule4replace_type::transf2:
        os << "transf2";
        break;
    case rule4replace_type::templ:
        os << "template";
        break;
    }
    os << "}";
    return os;
}



/**
    Adds in brother
*/
jle::shared_ptr<AST_node_item> AST_node_item::add_next(const std::string& _name, const std::string& _value, const Rule4replace& _rule4replace)
{
    next = jle::shared_ptr<AST_node_item>(new AST_node_item(_name, _value, _rule4replace));
    return next;
}

/**
    Adds in child
*/
jle::shared_ptr<AST_node_item> AST_node_item::add_child(const std::string& _name, const std::string& _value, const Rule4replace& _rule4replace)
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

std::string replace_transf2(const std::map<std::string, std::string>& map_items_found, const std::string& rule4replace);
std::string replace_transf2(const std::string& founded, const std::string& rule4replace);


void AST_node_item::exec_replace(const jle::map<std::string /*name*/, std::string>& templates)
{
    switch (this->rule4replace.type) {
    case rule4replace_type::none:
        exec_replace_current_transf2(templates);
        break;
    case rule4replace_type::transf2:
        exec_replace_current_transf2(templates);
        break;
    case rule4replace_type::templ:
        exec_replace_current_templ(templates);
        break;
    }
}




void AST_node_item::exec_replace_current_transf2(const jle::map<std::string /*name*/, std::string>& templates)
{
    //  first we look down and later to right (same level)
    if (this->down.expired()==false)
    {
        this->down->exec_replace(templates);
        //this->value = this->down->value;
    }

    if (this->next.expired()==false)
    {
        this->next->exec_replace(templates);
        //this->value += this->next->value;
    }

    //  check childs
    {
        if (this->down.expired())
        {
            if (rule4replace.data!="")
                value = replace_transf2(value, rule4replace.data);
            return;
        }
        shared_ptr<AST_node_item> current_node = this->down;
        std::map<std::string, std::string>  map_found;
        std::ostringstream os;

        while (current_node.expired()==false)
        {
            if (jle::s_trim(rule4replace.data, ' ') !="")
            {
                int counter = 0;
                std::string symbol = current_node->name;
                while (map_found.find(symbol) != map_found.end())
                {
                    ++counter;
                    symbol = JLE_SS(current_node->name << "#" << counter);
                }
                map_found[symbol] = current_node->value;

            }
            else
                os << current_node->value;
            current_node = current_node->next;
        }


        if (rule4replace.data !="")
            this->value = replace_transf2(map_found, rule4replace.data);
        else
            this->value = os.str();
    }
}

std::string replace_transf2(const std::string& founded, const std::string& rule4replace)
{
    if (jle::s_trim(rule4replace, ' ') !="")
    {
        std::map<std::string, std::string> map_found;
        map_found["t"] = founded;
        return replace_transf2(map_found, rule4replace);
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
}

std::string replace_transf2(const std::map<std::string, std::string>& map_items_found, const std::string& rule4replace)
{
    static auto ident = std::string("");
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
    //  look for starting of variable
    std::string::size_type previus = 0;
    for (std::string::size_type i=0; i<rule4replace.size()-1; ++i)
    {
        std::string add;
        if (rule4replace[i] == '$'  &&  rule4replace[i+1] == '(')
        {
            //  eureka
            add += rule4replace.substr(previus, i-previus);
            i+=2;
            std::string::size_type initVar = i;
            while (i<rule4replace.size()  &&  rule4replace[i] != ')')
                ++i;
            std::string var_name = rule4replace.substr(initVar, i-initVar);
            std::map<std::string, std::string>::const_iterator it = map_items_found.find(var_name);
            std::map<std::string, std::string>::const_iterator itPredefined = map_predefined_vars.find(var_name);
            if (it != map_items_found.end())
                add += it->second;
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
            else
                add += JLE_SS("$(" << var_name << ")");
            previus = i+1;
        }
        add  = replace_string(add, "\n", JLE_SS("\n" << JLE_SS(ident)));
        result = JLE_SS(result << add);
    }
    result += rule4replace.substr(previus);

    return result;

}


namespace  {

    struct Templ_running_status {
        std::string  generated_text;
        std::string  command;
        std::string  text_pending;
    };

    std::string   get_template_content(const std::string& templ,
                                         const jle::map<std::string /*name*/, std::string>& templates,
                                         const jle::map<std::string /*name*/, std::string>& templ_aliases)
    {
        auto effective_tmpl_name = templ;

        auto fta = templ_aliases.find(effective_tmpl_name);
        if(fta!=templ_aliases.cend())
        {
            effective_tmpl_name = fta->second;
        }

        auto ft = templates.find(effective_tmpl_name);
        if(ft != templates.cend())
            return ft->second;
        else
            return "TEMPLATE NOT FOUND";
    }

}

bool find_template_command(Templ_running_status& templ_run_status)
{
    templ_run_status.generated_text = JLE_SS(templ_run_status.generated_text << templ_run_status.text_pending);
    templ_run_status.text_pending = "";
    templ_run_status.command = "";
    return false;
}

void AST_node_item::AST_node_item::exec_replace_current_templ(const jle::map<std::string /*name*/, std::string>& templates)
{
    //  find template  (first, look for alias)

    //  while find template command
        //  write text on value till first command
        //  execute command
    //  write text till end


    auto templ_running_status = Templ_running_status{};
    templ_running_status.text_pending = get_template_content(this->rule4replace.data, templates, {});
    while(find_template_command(templ_running_status))
    {

    }

    this->value = templ_running_status.generated_text;

    //this->value = JLE_SS("pending... " << this->rule4replace.data << std::endl);
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
