#include "args.h"

#include <iostream>

Args::Args( std::vector< std::string >& args )
{
    if (args.at(0) == m_helpFlag)
    {
        m_hasHelp = true;
        return;
    }

    m_filepath = args.at(0);
    m_hasFilepath = true;

    auto argsEnd{ args.end() };

    auto checkFlag = [&argsEnd](std::vector< std::string >::iterator it, std::string flag)
        { return *it == flag && it + 1 < argsEnd; };

    for (auto it{ args.begin() + 1 }; it != argsEnd; ++it)
    {
        if ( checkFlag(it, m_savepathFlag) )
        {
            m_savepath = *(it + 1);
            m_hasSavepath = true;
        }
        else if ( checkFlag(it, m_widthFlag) )
        {
            m_width = std::stoi( *(it + 1) );
            m_hasWidth = true;
        }
        else if ( checkFlag(it, m_heightFlag) )
        {
            m_height = std::stoi( *(it + 1) );
            m_hasHeight = true;
        }
        else if ( *it == m_helpFlag )
        {
            m_hasHelp = true;

            // There is only help output with help flag
            m_hasFilepath = false;
            m_hasSavepath = false;
            m_hasWidth = false;
            m_hasHeight = false;

            break;
        }
        else
        {
            continue;
        }
    }

}


std::optional< std::filesystem::path > Args::getFilepath() const
{
    return checkReturn< std::filesystem::path >(m_filepath, hasFilepath);
}


std::optional< std::filesystem::path > Args::getSavepath() const
{
    return checkReturn< std::filesystem::path >(m_savepath, hasSavepath);
}


std::optional< int > Args::getWidth() const
{
    return checkReturn< int >(m_width, hasWidth);
}


std::optional< int > Args::getHeight() const
{
    return checkReturn< int >(m_height, hasHeight);
}


bool Args::hasFilepath() const
{
    return m_hasFilepath;
}


bool Args::hasSavepath() const
{
    return m_hasSavepath;
}


bool Args::hasWidth() const
{
    return m_hasWidth;
}


bool Args::hasHeight() const
{
    return m_hasHeight;
}


bool Args::hasHelp() const
{
    return m_hasHelp;
}


void Args::printHelp()
{
    int flagNameWidth{16};

    std::cout << "pic2ascii [path to image] [-flags [value except for help flag]]" << std::endl;
    std::cout << "Available flags:" << std::endl;
    std::cout << std::setw(flagNameWidth) << "--help:      " << "help info;" << std::endl;
    std::cout << std::setw(flagNameWidth) << "-w [value]:  " << "value for width resize image;" << std::endl;
    std::cout << std::setw(flagNameWidth) << "-h [value]:  " << "value for height resize image;" << std::endl;
    std::cout << std::setw(flagNameWidth) << "-s [path]:   " << "path for save ASCII-image." << std::endl;
}