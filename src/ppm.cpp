#include "ppm.h"

#include <exception>
#include <fstream>

//////////////////////////////////////////////
//
// Method implementations of Color class
//
bool PPM::Color::operator==(const int value) const
{
    return m_r == value && m_g == value && m_b == value;
}


auto PPM::Color::operator<=>(const int value) const
{
    if (auto compareR{m_r <=> value}; compareR != 0) return compareR;

    if (auto compareG{m_g <=> value}; compareG != 0) return compareG;

    return m_b <=> value;
}


void PPM::Color::setColor(int r, int g, int b)
{
    m_r = r;
    m_g = g;
    m_b = b;
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


PPM::PPMType PPM::Color::getType() const
{
    return m_type;
}


std::istream& PPM::operator>>(std::istream& is, PPM::Color& color)
{
    if (color.getType() == PPM::PPMType::P3)
    {
        is >> color.m_r >> color.m_g >> color.m_b;
    }
    else
    {
        color.m_r = is.get();
        color.m_g = is.get();
        color.m_b = is.get();
    }
    
    return is;
}


std::ostream& PPM::operator<<(std::ostream& os, PPM::Color color)
{
    if (color.getType() == PPM::PPMType::P3)
    {
        os << color.m_r << " " << color.m_g << " " << color.m_b << std::endl;
    }
    else
    {
        os << static_cast<char>(color.m_r) << 
              static_cast<char>(color.m_g) << 
              static_cast<char>(color.m_b);
    }

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
PPM::PPMType PPM::PPMHeader::getType() const
{
    return m_type;
}


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
    is >> header.m_type >> 
          header.m_width >> 
          header.m_height >> 
          header.m_maxValueColor;

    // Get '\n' from file between header and first color
    [[maybe_unused]]
    int unused{ is.get() };

    return is;
}


std::ostream& PPM::operator<<(std::ostream& os, PPM::PPMHeader header)
{
    os << header.m_type << '\n' <<
          header.m_width << " " << 
          header.m_height << '\n' <<
          header.m_maxValueColor << std::endl;

    return os;
}


//////////////////////////////////////////////
//
// Method implementation of PPMImage class
//
PPM::PPMImage::PPMImage(const std::filesystem::path& filepath)
{
    if ( filepath.extension() != ".ppm" )
    {
        throw std::runtime_error{"Wrong extension."};
    }

    std::ifstream file(filepath.c_str(), std::ios::binary);

    if ( !(file >> m_header) )
    {
        throw std::runtime_error{"Can't read ppm header from file."};
    }

    m_data.resize( m_header.getWidth() * m_header.getHeight(), PPM::Color{0, 0, 0, m_header.getType()} );

    for (auto& pixel : m_data)
    {
        if ( !(file >> pixel) )
        {
            throw std::runtime_error{"Can't read color ppm data from file."};
        }
    }

    file.close();
}


bool PPM::PPMImage::setImage(const std::filesystem::path& filepath)
{
    if ( filepath.extension() != ".ppm" )
    {
        return false;
    }

    std::ifstream file(filepath.c_str(), std::ios::binary);

    PPM::PPMHeader tempHeader{m_header}; 
    if ( !(file >> m_header) )
    {
        m_header = tempHeader;
        return false;
    }

    std::vector<PPM::Color> tempData{ std::move(m_data) };
    m_data.resize( m_header.getWidth() * m_header.getHeight(), PPM::Color{0, 0, 0, m_header.getType()} );

    for (auto& pixel : m_data)
    {
        if ( !(file >> pixel) )
        {
            m_header = tempHeader;
            m_data = std::move(tempData);
            return false;
        }
    }

    file.close();

    return true;
}


bool PPM::PPMImage::setColor(int x, int y, const PPM::Color& color)
{
    if ( 0 <= color && color <= m_header.getMaxValueColor() )
    {
        m_data.at( x + y * m_header.getWidth() ) = color;
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


int PPM::PPMImage::getMaxValueColor() const
{
    return m_header.getMaxValueColor();
}


PPM::Color PPM::PPMImage::getColor(int x, int y) const
{
    return m_data.at( x + y * m_header.getWidth() );
}


bool PPM::PPMImage::resize(int newWidth, 
                           int newHeight, 
                           Resizer::ResizeAlgorithm algorithm)
{
    int currentWidth{ m_header.getWidth() };
    int currentHeight{ m_header.getHeight() };

    if (newWidth < 1 || newHeight < 0)
    {
        return false;
    }

    if (newHeight == 0)
    {
        newHeight = currentHeight * ( newWidth / static_cast<float>( currentWidth ) ); 
    }

    if ( currentWidth == newWidth &&
         currentHeight == newHeight )
    {
        return false;
    }

    Resizer::Resizer resizer;
    std::vector<PPM::Color> newData = resizer.resize(
        m_data, 
        currentWidth, 
        currentHeight, 
        newWidth, 
        newHeight, 
        algorithm);

    PPM::PPMHeader newHeader{ m_header.getType(), newWidth, newHeight, m_header.getMaxValueColor() };

    m_data = std::move(newData);
    m_header = std::move(newHeader);

    return true;
}


bool PPM::PPMImage::saveImage(const std::filesystem::path& filepath)
{
    if ( filepath.extension() != ".ppm" )
    {
        return false;
    }

    bool isAlreadyExists{ std::filesystem::exists(filepath) };
    std::filesystem::path filepathCopy;

    if (isAlreadyExists)
    {
        filepathCopy = filepath;
        filepathCopy.replace_filename( filepathCopy.stem() += 
                                       std::filesystem::path{"_temp"} += 
                                       filepathCopy.extension() );

        std::filesystem::copy_file(filepath, filepathCopy);
    }

    auto clearTempFile{ [isAlreadyExists, &filepath, &filepathCopy]
        {
            if (isAlreadyExists) 
            {
                std::filesystem::copy_file(filepathCopy, filepath);
                std::filesystem::remove(filepathCopy);
            }
            else
            {
                std::filesystem::remove(filepath);
            }
        } };

    std::ofstream file(filepath.c_str(), std::ios::binary);

    if ( !(file << m_header) )
    {
        clearTempFile();

        return false;
    }

    for (auto& pixel : m_data)
    {
        if ( !(file << pixel) )
        {
            clearTempFile();

            return false;
        }
    }

    file.close();

    if (isAlreadyExists)
    {
        std::filesystem::remove(filepathCopy);
    }

    return true;
}