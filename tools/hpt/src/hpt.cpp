#include "core/hp/humbleparser.h"
#include "core/hp/ast.h"
#include "core/alarm.h"
#include "core/string.h"


#include <iostream>  // I/O
#include <fstream>   // file I/O
#include <iomanip>   // format manipulation





std::string get_string2parse_from_file(const std::string& file_name)
{
    char buffer[4096];
    std::ifstream ftext2transform(file_name.c_str(), std::ios::in);
    std::ostringstream ostext4parse;
    bool first = true;
    while (ftext2transform.getline(buffer, 4096))
    {
        if (!first)
            ostext4parse   << std::endl;
        first = false;
        ostext4parse << std::string(buffer);
    }
    ftext2transform.close();

    return std::string(ostext4parse.str());
}






int main(int argc, char* argv[])
{
    std::string file_rules1;
    std::string file_rules2;
    std::string input_file;

    if (argc==3)
    {
        file_rules1 = argv[1];
        input_file = argv[2];
    }
    else if (argc==4)
    {
        file_rules1 = argv[1];
        file_rules2 = argv[2];
        input_file = argv[3];
    }
    else
    {
        std::cerr << "bad argument count... " << argc << std::endl;
        std::cerr << "ussage...\nhpt  <gram-file> <2gram-file>  <input-file>" << std::endl;
        return -1;
    }

    JLE_COUT_TRACE(file_rules1)
    JLE_COUT_TRACE(file_rules2)
    JLE_COUT_TRACE(input_file)


    jle::hp::Humble_parser  hparser;
    bool result;
    std::string result_string;
    JLE_COUT_TRACE(file_rules1);
    std::tie(result, result_string) =  hparser.add_rules_from_file(file_rules1);
    if (result==false){
      std::cerr << std::endl << result_string << std::endl;
      return -1;
    }

    JLE_COUT_TRACE(file_rules2);
    std::tie(result, result_string) =  hparser.add_rules_from_file(file_rules2);
    if (result==false){
      std::cerr << std::endl << result_string << std::endl;
      return -1;
    }

    std::string string2parse = get_string2parse_from_file(input_file);

    bool result2;
    std::string resultTest;
    jle::hp::AST_node_item   ast_root("");
    std::tie(result2, resultTest, ast_root) = hparser.multi_parse(string2parse);


    if (result2 == false)
    {
        std::cerr << "ERROR PARSING..." << resultTest << std::endl;
        return -1;
    }
    else
    {
        std::cout << ast_root.value << std::endl;
    }


    return  0;
}




//---------------------------------------------------------------------------

//  GENERAL ALARM RECEPTOR

//---------------------------------------------------------------------------

void jle::alarm_msg(const jle::alarm& al)
{
    std::cout << "\n\r";
    std::cout << std::endl << "ALARM..." << std::endl ;
    std::cout << al << std::endl ;
}
