#ifndef ASCII_H
#define ASCII_H

#include <concepts>
#include <filesystem>
#include <ranges>

#include "image.h"

namespace ASCII
{
    template < typename T >
    concept IsImage = requires( T image )
    {
        requires std::derived_from< std::remove_reference_t< decltype( image ) >, Image::ImageBase >;
    };


    class Converter
    {
    private:
        const std::vector< char > mc_asciiCharacters{ 'M', 'b', '5', 'f', 'l', '(', ')', '=', '-', ' ' };
        const int                 mc_length{ static_cast< int >( mc_asciiCharacters.size() - 1 ) };

        // Luminance values (Y in CIE xyY) for sRGB.
        // Taken from http://www.brucelindbloom.com/index.html?WorkingSpaceInfo.html
        const float mc_rY{ 0.2126f };
        const float mc_gY{ 0.7152f };
        const float mc_bY{ 0.0722f };

        std::vector< char > m_result;
        int                 m_imageWidth;
        int                 m_imageHeight;

    public:
        void convert( IsImage auto& image )
        {
            m_imageWidth  = image.getWidth();
            m_imageHeight = image.getHeight();
            int newResultSize( m_imageWidth * m_imageHeight + m_imageHeight );

            m_result.resize( newResultSize );

            int rangePerAsciiCharacter{ image.getMaxValueColor() / mc_length };

            for ( int y : std::views::iota( 0, m_imageHeight ) )
            {
                for ( int x : std::views::iota( 0, m_imageWidth ) )
                {
                    auto color{ image.getColor( x, y ) };
                    int  grayR{ static_cast< int >( color->getR() * mc_rY ) };
                    int  grayG{ static_cast< int >( color->getG() * mc_gY ) };
                    int  grayB{ static_cast< int >( color->getB() * mc_bY ) };
                    int  gray{ grayR + grayG + grayB };

                    m_result.at( x + y * m_imageWidth + y ) = mc_asciiCharacters.at( gray / rangePerAsciiCharacter );
                }

                m_result.at( m_imageWidth + y * m_imageWidth + y ) = '\n';
            }
        }


        bool save( const std::filesystem::path& savePath );
        void print();
    };
} // namespace ASCII

#endif // ASCII_H