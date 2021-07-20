#include "ppm.h"

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