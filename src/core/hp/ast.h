#ifndef JLE_AST__H
#define JLE_AST__H


#include <string>
#include "core/shared_ptr.hpp"
#include "core/cont/list.hpp"
#include "core/cont/map.hpp"




namespace jle {    namespace hp  {


enum class rule4replace_type { none, transf2, templ };

struct Rule4replace {
    rule4replace_type   type = rule4replace_type::none;
    std::string         data;
};

std::ostream& operator<< (std::ostream& os, const Rule4replace& r4r);



class AST_node_item
{
public:
    std::string                     name        ;
    std::string                     value       ;
    Rule4replace                    rule4replace;

    jle::shared_ptr<AST_node_item>    down        ;
    jle::shared_ptr<AST_node_item>    next        ;



    AST_node_item(const std::string& _name, const std::string& _value="", const Rule4replace& _rule4replace={})
        :   name(_name),  value(_value), rule4replace(_rule4replace) {}


    jle::shared_ptr<AST_node_item> add_next (const std::string& _name, const std::string& _value, const Rule4replace& _rule4replace);
    jle::shared_ptr<AST_node_item> add_child(const std::string& _name, const std::string& _value, const Rule4replace& _rule4replace);


    void exec_replace               (const jle::map<std::string /*name*/, std::string>& templates);
    void remove_nodes_manteining    (const jle::list<std::string>& lmodes2mantein);


private:
    void exec_replace_current_transf2(const jle::map<std::string /*name*/, std::string>& templates);
    void exec_replace_current_templ(const jle::map<std::string /*name*/, std::string>& templates);
};


std::string AST_get_string_nodes(const jle::shared_ptr<AST_node_item>& node);



};  };  //  namespace jle {    namespace hp  {



#endif // JLE_AST__H
