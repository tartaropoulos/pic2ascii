#ifndef RESIZER_H
#define RESIZER_H

#include <ranges>
#include <vector>

namespace Resizer
{
    template < typename T >
    concept IsColor = std::derived_from< T, Color::ColorBase >;


    enum class ResizeAlgorithm
    {
        NEAREST_NEIGHBOR,
    };


    class Resizer
    {
    public:
        template < IsColor T >
        std::vector< T > resize( std::vector< T >& data,
                                 int               currentWidth,
                                 int               currentHeight,
                                 int               newWidth,
                                 int               newHeight,
                                 ResizeAlgorithm   algorithm = ResizeAlgorithm::NEAREST_NEIGHBOR )
        {
            switch ( algorithm )
            {
                case ResizeAlgorithm::NEAREST_NEIGHBOR:
                default:
                    return nearestNeighborResize( data, currentWidth, currentHeight, newWidth, newHeight );
            }
        }

    private:
        template < IsColor T >
        std::vector< T > nearestNeighborResize( std::vector< T >& data,
                                                int               currentWidth,
                                                int               currentHeight,
                                                int               newWidth,
                                                int               newHeight )
        {
            float widthRatio{ currentWidth / static_cast< float >( newWidth ) };
            float heightRatio{ currentHeight / static_cast< float >( newHeight ) };

            std::vector< T > result( newWidth * newHeight, T{} );

            for ( int y : std::views::iota( 0, newHeight ) )
            {
                int sourceY{ static_cast< int >( y * heightRatio ) };

                for ( int x : std::views::iota( 0, newWidth ) )
                {
                    int sourceX{ static_cast< int >( x * widthRatio ) };
                    result.at( x + y * newWidth ) = data.at( sourceX + sourceY * currentWidth );
                }
            }

            return result;
        }
    };
} // namespace Resizer

#endif // RESIZER_H