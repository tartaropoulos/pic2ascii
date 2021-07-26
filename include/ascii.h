#ifndef ASCII_H
#define ASCII_H

#include <concepts>
#include <filesystem>

namespace ASCII
{
    template<typename T>
    concept IsImage = 
        requires (T image)
        {
            image.getWidth();
            image.getHeight();
            image.getMaxValueColor();
        } &&
        requires (T image, int x, int y)
        {
            image.getColor(x, y).getR();
            image.getColor(x, y).getG();
            image.getColor(x, y).getB();
        };


    class Converter
    {
    private:
        const std::vector<char>  m_asciiCharacters{'M', 'b', '5', 'f', 'l', '(', ')', '=', '-', ' '};
        const int                m_length{ static_cast<int>( m_asciiCharacters.size() - 1 ) };

        // Luminance values (Y in CIE xyY) for sRGB. 
        // Taken from http://www.brucelindbloom.com/index.html?WorkingSpaceInfo.html
        const float              m_rY{0.2126f};
        const float              m_gY{0.7152f};
        const float              m_bY{0.0722f};

        std::vector<char>        m_result;
        int                      m_imageWidth;
        int                      m_imageHeight;

    public:
        void convert(IsImage auto& image)
        {
            m_imageWidth = image.getWidth();
            m_imageHeight = image.getHeight();
            int newResultSize( m_imageWidth * m_imageHeight + m_imageHeight );

            m_result.resize(newResultSize);

            int rangePerAsciiCharacter{ image.getMaxValueColor() / m_length };

            for (int y{0}; y < m_imageHeight; ++y)
            {
                for (int x{0}; x < m_imageWidth; ++x)
                {
                    auto color{ image.getColor(x, y) };
                    int  grayR{ static_cast<int>( color.getR() * m_rY ) };
                    int  grayG{ static_cast<int>( color.getG() * m_gY ) };
                    int  grayB{ static_cast<int>( color.getB() * m_bY ) };
                    int   gray{ grayR + grayG + grayB };

                    m_result.at( x + y * m_imageWidth + y ) = m_asciiCharacters.at( gray / rangePerAsciiCharacter );
                }

                m_result.at( m_imageWidth + y * m_imageWidth + y ) = '\n';
            }
        }


        bool save(const std::filesystem::path& savePath);
        void print();
    };
}

#endif // ASCII_H