#ifndef RESIZER_H
#define RESIZER_H

#include <vector>

namespace Resizer
{
    template<typename T>
    concept IsRGBColor =
        requires (T color)
        {
            color.getR();
            color.getG();
            color.getB();
        };


    enum class ResizeAlgorithm
    {
        NEAREST_NEIGHBOR,
    };


    class Resizer
    {
    public:
        template<IsRGBColor T>
        std::vector<T> resize(
            std::vector<T> data,
            int currentWidth,
            int currentHeight, 
            int newWidth, 
            int newHeight, 
            ResizeAlgorithm algorithm = ResizeAlgorithm::NEAREST_NEIGHBOR)
        {
            switch (algorithm)
            {
                case ResizeAlgorithm::NEAREST_NEIGHBOR: return nearestNeighborResize(
                    data, 
                    currentWidth, 
                    currentHeight, 
                    newWidth, 
                    newHeight);
            }

            return data;
        }

    private:
        template<IsRGBColor T>
        std::vector<T> nearestNeighborResize(
            std::vector<T> data, 
            int currentWidth, 
            int currentHeight, 
            int newWidth, 
            int newHeight)
        {
            float widthRatio{ currentWidth / static_cast<float>(newWidth) };
            float heightRatio{ currentHeight / static_cast<float>(newHeight) };

            std::vector<T> result(newWidth * newHeight, T{});

            for (int y{0}; y < newHeight; ++y)
            {
                int sourceY{ static_cast<int>(y * heightRatio) };

                for (int x{0}; x < newWidth; ++x)
                {
                    int sourceX{ static_cast<int>(x * widthRatio) };
                    result.at( x + y * newWidth ) = data.at( sourceX + sourceY * currentWidth );
                }
            }

            return result;
        }
    };
}

#endif // RESIZER_H