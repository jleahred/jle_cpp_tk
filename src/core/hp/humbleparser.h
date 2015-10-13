#ifndef JLE_HUMBLE_PARSER__H
#define JLE_HUMBLE_PARSER__H


#include <string>
#include <map>
#include <list>
#include <set>


#include "ast.h"
#include "core/tuple.hpp"
#include "core/cont/map.hpp"




namespace jle {  namespace hp  {




class Humble_parser
{
public:
    Humble_parser();
    virtual ~Humble_parser();

    //  parse ------------------------------------------------------------
    jle::tuple<bool, std::string/*remaining*/, AST_node_item>
    parse(const std::string& input, const std::string& init) const;

    jle::tuple<bool, std::string/*remaining*/, AST_node_item>
    multi_parse(const std::string& input, std::string minit="") const;


    //  load rules  ------------------------------------------------------

    /** @brief  Add a parsing rule, and a tansformation or templates
     */
    jle::tuple<bool, std::string>
    add_rule (const std::string& rule_t2);

    /**
     * @brief Adds a line. It can contain part of a rule
     * @param line
     *
     * Carry returns and tabs are changed by espaces, later, it will call add_rule
     */
    jle::tuple<bool, std::string>
    add_line (std::string line);


    jle::tuple<bool, std::string>
    load_rules_from_stream (std::istream& stream);


    jle::tuple<bool, std::string>
    load_rules_from_file(const std::string& file_rules);

    jle::tuple<bool, std::string>
    load_rules_from_string (const std::string& rules);



    //  status  ----------------------------------------------------------
    bool exist_errors_in_rules(void)  const    {  return errors_in_rules;  }
    void clear(void);
    std::list<std::string>   get_terminal_rules(void)const;
    std::list<std::string>   get_non_terminal_rules(void)const;


protected:

private:
    mutable std::string   string2parse;

    typedef std::map<   std::string,                                        //  rule left
                        std::list<  jle::tuple<
                                                std::list<std::string>,     //  right symbols
                                                Rule4replace> > >            //  transformation2   or template
            Container_rules  ;
    Container_rules      non_terminal_rules ;
    Container_rules      terminal_rules    ;


    jle::tuple<bool, int>
    execute_non_terminal    (int srt2parse_pos, const std::string& non_terminal_code, AST_node_item& ast_node) const;

    jle::tuple<bool, int>
    execute_terminal       (int str2parse_pos, const std::string& terminal_code   , AST_node_item& ast_node) const;

    jle::tuple<bool, int/*, jle::shared_ptr<AST_node_item> */>
    execute_non_terminal_klein   (int srt2parse_pos, const std::string& non_terminal_code, AST_node_item& ast_node) const;

    jle::tuple<bool, int/*, jle::shared_ptr<AST_node_item> */>
    execute_terminal_klein      (int srt2parse_pos, const std::string& non_terminal_code, AST_node_item& ast_node) const;


    bool errors_in_rules;

    mutable int                                             deeper_error_parsing_pos;
    mutable std::set<std::string>                           deeper_error_expected_symbols;

    bool is_terminal(const std::string& str) const;

    std::string  default_init_symbol;





    jle::tuple<bool, int> execute_predefined_var   (int srt2parse_pos, const std::string& terminal_rule, AST_node_item& ast_node, const Rule4replace& rule2replace) const;
    jle::tuple<bool, int> execute_regular_expresion(int srt2parse_pos, const std::string& terminal_rule, AST_node_item& ast_node, const Rule4replace& rule2replace) const;
    jle::tuple<bool, int> execute_literal          (int srt2parse_pos, const std::string& terminal_rule, AST_node_item& ast_node, const Rule4replace& rule2replace) const;


    //  debug
    mutable AST_node_item* ast_root_debug;


    bool adding_rule_multi_line;
    std::string building_rule;

    bool adding_template = false;
    std::string template_name;
    std::string building_template;
    jle::map<std::string, std::string>  templates;



    jle::tuple<bool, std::string>  _adding_rule_multi_line(const std::string& line);
    jle::tuple<bool, std::string>  _adding_template(const std::string& line);
};


} }  //  namespace jle {  namespace hp  {

#endif // JLE_HUMBLE_PARSER__H
