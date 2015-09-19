#ifndef JLE_AST__H
#define JLE_AST__H


#include <string>
#include "core/shared_ptr.hpp"
#include "core/cont/list.hpp"




namespace jle {


class AST_node_item
{
public:
    std::string                     name        ;
    std::string                     value       ;
    std::string                     rule4replace;

    jle::shared_ptr<AST_node_item>    down        ;
    jle::shared_ptr<AST_node_item>    next        ;



    AST_node_item(const std::string& _name, const std::string& _value="", const std::string& _rule4replace="")
        :   name(_name),  value(_value), rule4replace(_rule4replace) {};


    jle::shared_ptr<AST_node_item> add_next (const std::string& _name, const std::string& _value, const std::string& _rule4replace);
    jle::shared_ptr<AST_node_item> add_child(const std::string& _name, const std::string& _value, const std::string& _rule4replace);


    void exec_replace               (void);
    void remove_nodes_manteining    (const jle::list<std::string>& lmodes2mantein);


private:
    void exec_replace_current(void);
};


std::string AST_get_string_nodes(const jle::shared_ptr<AST_node_item>& node);



};  //  namespace jle {



#endif // JLE_AST__H
