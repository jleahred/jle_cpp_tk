#include "string.h"


#include <math.h>
#include <tuple>
#include <regex>
#include <iostream>
#include <string>
#include <iomanip>


#include "core/misc.h"

namespace jle
{

    std::string     s_trim    (const std::string&    s , char char2remove)
    {
        auto  pos1 = s.find_first_not_of(char2remove);
        if (pos1==std::string::npos)
            return "";
        std::string::size_type pos2 = s.find_last_not_of (char2remove);
        return (
                    s.substr(
                            pos1,
                            pos2==std::string::npos ? s.length()-1 : pos2-pos1+1
                    )
        );
    }

    std::string s_trim (const std::string& s, const std::string& chars2remove)
    {
        auto pos1 = s.find_first_not_of(chars2remove);
        auto pos2 = s.find_last_not_of(chars2remove);

        if (pos1==std::string::npos)
            return "";
        return (
                    s.substr(
                            pos1,
                            pos2==std::string::npos ? s.length()-1 : pos2-pos1+1
                    )
        );
    }

    std::string     s_2lower(const std::string& s)
    {
        std::string result;
        for(auto& c : s)
            result.push_back(static_cast<char>(tolower(c)));
        return result;
    }

    std::string     s_2upper(const std::string& s)
    {
        std::string result;
        for(auto& c : s)
            result.push_back(static_cast<char>(toupper(c)));
        return result;
    }



    std::tuple<chrono::duration, bool>
    s_try_s2dur   (const std::string&    _s, const chrono::duration& def_val )
    {
        std::string s = jle::s_trim(_s, ' ');
        if (s.empty())
            return std::make_tuple(def_val, false);
        try
        {
            std::istringstream i;
            int days=0;
            int hours=0;
            int minuts=0;
            int segundos=0;
            int milliseconds=0;

            i.str(s);

            int num;
            char temp_delim;
            bool partial_processed = false;

            i >> num >> std::skipws >> temp_delim;

            if (temp_delim == 'd')
            {
                days = num;
                i >> num >> std::skipws >> temp_delim;
                partial_processed = true;
            }

            if (temp_delim == 'h')
            {
                hours = num;
                i >> num >> std::skipws >> temp_delim;
                partial_processed = true;
            }

            if (temp_delim == 'm'  &&  i.peek()!='s')        //  no ms
            {
                minuts = num;
                i >> num >> std::skipws >> temp_delim;
                partial_processed = true;
            }

            if (temp_delim == 's')
            {
                segundos = num;
                i >> num >> std::skipws >> temp_delim;
                partial_processed = true;
            }

            if (temp_delim == 'm')
            {
                i >> temp_delim;
                if (temp_delim=='s')
                {
                    milliseconds = num;
                    partial_processed = true;
                }
            }

            //  finished??
            std::string temp;
            i >> temp;

            if (i.eof() &&  temp==""  &&  partial_processed)
            {
                return std::make_tuple(
                                            std::chrono::hours{days*24} +
                                            std::chrono::hours{hours} +
                                            std::chrono::minutes{minuts} +
                                            std::chrono::seconds{segundos} +
                                            std::chrono::milliseconds{milliseconds}
                                , true);
            }
            else
                return std::make_tuple(def_val, false);
        }
        catch(...)
        {
            return std::make_tuple(def_val, false);
        }
    }


    std::tuple<chrono::time_point, bool>
    s_try_s2tp   (const std::string&    s, const chrono::time_point& def_val )
    {
        //  R" is not working  ???
        std::regex  re{ "^ *([12][0-9]{3})-(1[0-2]|0[1-9])-([012][0-9]|[3][01]) "
                        "([01][0-9]|[2][0-3]):([0-5][0-9]):([0-5][0-9])\\.([0-9]{3}) *$" };
        auto m = std::smatch {};
        if (std::regex_match(s, m, re))
        {
            try
            {
                return std::make_tuple(
                    chrono::make_from_date(
                                chrono::year {std::get<0>(s_try_s2i (m[1], -1))},
                                chrono::month{std::get<0>(s_try_s2i (m[2], -1))},
                                chrono::day  {std::get<0>(s_try_s2i (m[3], -1))})
                                + std::chrono::hours       {std::get<0>(s_try_s2i(m[4], -1))}
                                + std::chrono::minutes     {std::get<0>(s_try_s2i(m[5], -1))}
                                + std::chrono::seconds     {std::get<0>(s_try_s2i(m[6], -1))}
                                + std::chrono::milliseconds{std::get<0>(s_try_s2i(m[7], -1))}
                    , true);
            }
            catch(...)
            {
                return std::make_tuple(def_val, false);
            }
        }
        else
        {
            std::regex  re2{ "^ *([12][0-9]{3})-(1[0-2]|0[1-9])-([012][0-9]|[3][01]) *$"};
            auto m2 = std::smatch {};
            if (std::regex_match(s, m, re2))
            {
                try
                {
                return std::make_tuple(
                    chrono::make_from_date(
                                chrono::year {std::get<0>(s_try_s2i (m2[0], -1))},
                                chrono::month{std::get<0>(s_try_s2i (m2[1], -1))},
                                chrono::day  {std::get<0>(s_try_s2i (m2[2], -1))})
                    , true);
                }
                catch(...)
                {
                    return std::make_tuple(def_val, false);
                }
            }
        }
        return std::make_tuple(def_val, false);
    }








jle::vector<std::string> s_split (
                                const std::string&  s,
                                const std::string&  separator,
                                bool  remove_empty
                            )
{
    jle::vector<std::string> result;
    size_t current_pos = 0;
    size_t prev_pos    = 0;

    while ( (current_pos = s.find(separator, prev_pos)) != std::string::npos) {
        auto to_insert = s.substr(prev_pos, current_pos - prev_pos);
        if(to_insert.empty())
        {
            if(remove_empty==false)
                result.push_back("");
            prev_pos = current_pos+separator.length();
        }
        else
        {
            result.push_back(to_insert);
            prev_pos = current_pos+separator.length();
        }
    };
    auto tail = s.substr(prev_pos, s.length()-prev_pos);
    if(remove_empty  &&  tail.empty())
        ;
    else
        result.push_back(s.substr(prev_pos, s.length()-prev_pos));

    return result;
}



std::tuple<double, bool> s_try_s2d(const std::string&    _s, double def_val)
{
    std::string s = jle::s_trim(_s, ' ');
    if (s.empty())
        return std::make_tuple(def_val, false);

    std::istringstream i;

    i.str(s);
    double d = def_val;
    i >> d;

    if (i.eof())
        return std::make_tuple(d, true);
    else
        return std::make_tuple(def_val, false);
}

std::tuple<int, bool> s_try_s2i(const std::string&    _s, int    def_val )
{
    std::string s = jle::s_trim(_s, ' ');
    if (s.empty())
        return std::make_tuple(def_val, false);

    std::istringstream is;

    is.str(s);
    int i = def_val;
    is >> i;

    if (is.eof())
        return std::make_tuple(i, true);
    else
        return std::make_tuple(def_val, false);
}







std::string     s_align_left (const std::string& s, int size, char char_fill)
{
    std::ostringstream o;
    o.width(size);
    o.fill(char_fill);
    o << std::left << s;
    return o.str();
}

std::string     s_align_right(const std::string& s, int size, char char_fill)
{
    std::ostringstream o;
    o.width(size);
    o.fill(char_fill);
    o << std::right << s;
    return o.str();
}


std::string s_normalize_utf8(const std::string& source_string)
{
    std::string destination_string;
    unsigned char aux_char;

    for (unsigned int counter=0; counter < source_string.size(); counter++)
    {
        aux_char=source_string[counter];

        if (aux_char>127)
        {
            switch (aux_char)
            {
                case 0xE1: //'á'
                    destination_string+=char(0xC3);
                    destination_string+=char(0xA1);
                    break;
                case 0xC1: //'Á'
                    destination_string+=char(0xC3);
                    destination_string+=char(0x81);
                    break;
                case 0xE9: //'é'
                    destination_string+=char(0xC3);
                    destination_string+=char(0xA9);
                    break;
                case 0xC9: //'É'
                    destination_string+=char(0xC3);
                    destination_string+=char(0x89);
                    break;
                case 0xEA: //'í'
                    destination_string+=char(0xC3);
                    destination_string+=char(0xAD);
                    break;
                case 0xCD: //'Í'
                    destination_string+=char(0xC3);
                    destination_string+=char(0x8D);
                    break;
                case 0xF3: //'ó'
                    destination_string+=char(0xC3);
                    destination_string+=char(0xB3);
                    break;
                case 0xD3: //'Ó'
                    destination_string+=char(0xC3);
                    destination_string+=char(0x93);
                    break;
                case 0xFA: //'ú'
                    destination_string+=char(0xC3);
                    destination_string+=char(0xBA);
                    break;
                case 0xDA: //'Ú'
                    destination_string+=char(0xC3);
                    destination_string+=char(0x9A);
                    break;
                case 0xF1: //'ñ'
                    destination_string+=char(0xC3);
                    destination_string+=char(0xB1);
                    break;
                case 0xD1: //'Ñ'
                    destination_string+=char(0xC3);
                    destination_string+=char(0x91);
                    break;
                case 0xFC: //'ü'
                    destination_string+=char(0xC3);
                    destination_string+=char(0xBC);
                    break;
                case 0xDC: //'Ü'
                    destination_string+=char(0xC3);
                    destination_string+=char(0x9C);
                    break;
                default:
                    destination_string+="_";
            }
        }
        else
            destination_string+=source_string[counter];
    }
    return destination_string;
}


std::string  align_cols(const std::string&  text)
{
    auto lines = s_split(text, "\n", false);
    auto max_size_col = jle::vector<size_t>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


    auto update_if_bigger = [](size_t val, size_t& updatable) {
        if(val>updatable)
            updatable = val;
    };

    auto rows = jle::vector<jle::vector<std::string>>{};
    for(l : lines) {
        rows.push_back(s_split(l, " ", true));
        unsigned int col = 0;
        for(cell : rows.back()) {
            update_if_bigger(cell.size(), max_size_col[col]);
            ++col;
            if(col>=max_size_col.size())
                break;
        }
    }

    std::ostringstream  result;
    for(row : rows) {
        unsigned col = 0;
        for(cell : row) {
            result << std::setw(int(max_size_col[col])) << std::left << cell << "  ";
            ++col;
            if(col>=max_size_col.size())
                break;
        }
        result << std::endl;
    }

    return result.str();
}


};  //  namespace jle {





