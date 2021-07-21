#include "ppm.h"

#include <exception>
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


//////////////////////////////////////////////
//
// Method implementation of PPMHeader class
//
int PPM::PPMHeader::getWidth() const
{
    return m_width;
}

int PPM::PPMHeader::getHeight() const
{
    return m_height;
}

int PPM::PPMHeader::getMaxValueColor() const
{
    return m_maxValueColor;
}

std::istream& PPM::operator>>(std::istream& is, PPM::PPMHeader& header)
{
    is >> header.m_type >> header.m_width >> header.m_height >> header.m_maxValueColor;

    return is;
}

std::ostream& PPM::operator<<(std::ostream& os, PPM::PPMHeader header)
{
    os << header.m_type << " " << header.m_width << " " << header.m_height << " " << header.m_maxValueColor << std::endl;

    return os;
}


//////////////////////////////////////////////
//
// Method implementation of PPMImage class
//
PPM::PPMImage::PPMImage(const std::string& filepath)
{
    std::ifstream file(filepath);

    if ( !(file >> m_header) )
    {
        throw std::runtime_error{"Can't read ppm header from file."};
    }

    m_data.resize(m_header.getWidth() * m_header.getHeight());

    for (auto& pixel : m_data)
    {
        if ( !(file >> pixel) )
        {
            throw std::runtime_error{"Can't read color ppm data from file."};
        }
    }

    file.close();
}

bool PPM::PPMImage::setImage(const std::string& filepath)
{
    std::ifstream file(filepath);

    PPM::PPMHeader tempHeader{m_header}; 
    if ( !(file >> m_header) )
    {
        m_header = tempHeader;
        return false;
    }

    std::vector<PPM::Color> tempData{std::move(m_data)};
    m_data.resize(m_header.getWidth() * m_header.getHeight());

    for (auto& pixel : m_data)
    {
        if ( !(file >> pixel) )
        {
            m_data = std::move(tempData);
            return false;
        }
    }

    file.close();

    return true;
}

bool PPM::PPMImage::setColor(int x, int y, const PPM::Color& color)
{
    int maxValueColor{m_header.getMaxValueColor()};
    if (color.getR() <= maxValueColor && color.getG() <= maxValueColor && color.getB() <= maxValueColor)
    {
        m_data.at(x + y * m_header.getHeight()).setColor(color);
        return true;
    }

    return false;
}

int PPM::PPMImage::getWidth() const
{
    return m_header.getWidth();
}

int PPM::PPMImage::getHeight() const
{
    return m_header.getHeight();
}

PPM::Color PPM::PPMImage::getColor(int x, int y) const
{
    return m_data.at(x + y * m_header.getHeight());
}

bool PPM::PPMImage::saveImage(const std::string& filepath)
{
    std::ofstream file(filepath);

    if ( !(file << m_header) )
    {
        return false;
    }

    for (auto& pixel : m_data)
    {
        if ( !(file << pixel) )
        {
            return false;
        }
    }

    file.close();

    return true;
}