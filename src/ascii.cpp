#include "ascii.h"

#include <algorithm>
#include <fstream>
#include <iostream>

auto output{ [](char ch, std::ostream& out) { ch == '\n' ? out << ch : out << ch << " "; } };


bool ASCII::Converter::save(const std::filesystem::path& savePath)
{
    if ( savePath.extension() != ".txt" )
    {
        return false;
    }

    std::ofstream resultFile{ savePath.c_str() };

    auto writeFile{ [&resultFile](char ch) { output(ch, resultFile); } };

    std::for_each(m_result.begin(), m_result.end(), writeFile);

    return true;
}


void ASCII::Converter::print()
{
    auto print{ [](char ch) { output(ch, std::cout); } };

    std::for_each(m_result.begin(), m_result.end(), print);
}