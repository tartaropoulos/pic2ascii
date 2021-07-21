#include "ppm.h"

#include <fstream>

//////////////////////////////////////////////
//
// Method implementations of Color class
//
void PPM::Color::setColor(int r, int g, int b)
{
    m_r = r;
    m_g = g;
    m_b = b;
}

void PPM::Color::setColor(const PPM::Color& color)
{
    m_r = color.getR();
    m_g = color.getG();
    m_b = color.getB();
}

int PPM::Color::getR() const
{
    return m_r;
}

int PPM::Color::getG() const
{
    return m_g;
}

int PPM::Color::getB() const
{
    return m_b;
}

std::istream& PPM::operator>>(std::istream& is, PPM::Color& color)
{
    is >> color.m_r >> color.m_g >> color.m_b;

    return is;
}

std::ostream& PPM::operator<<(std::ostream& os, PPM::Color color)
{
    os << color.m_r << " " << color.m_g << " " << color.m_b << std::endl;

    return os;
}


//////////////////////////////////////////////
//
// Function implementation of PPMType enum
//
std::ostream& PPM::operator<<(std::ostream& os, PPM::PPMType type)
{
    return os << 'P' << static_cast<int>(type);
}

std::istream& PPM::operator>>(std::istream& is, PPM::PPMType& type)
{
    char P;
    int  number;

    is >> P >> number;

    if ( !(P == 'P') )
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }

    // TODO: add number check for membership of the PPMType enum
    type = static_cast<PPM::PPMType>(number);

    return is;
}