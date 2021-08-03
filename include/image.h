#ifndef IMAGE_H
#define IMAGE_H

#include <filesystem>
#include <functional>
#include <unordered_map>

#include "color.h"
#include "resizer.h"

namespace Image
{
    class ImageBase
    {
    public:
        virtual ~ImageBase() = default;

        virtual bool setImage(const std::filesystem::path& filepath) = 0;
        virtual bool setColor(int x, int y, const Color::ColorBase& color) = 0;

        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;
        virtual int getMaxValueColor() const = 0;
        virtual std::unique_ptr<Color::ColorBase> getColor(int x, int y) const = 0;

        virtual bool resize(
            int newWidth = 50, 
            int newHeight = 0, 
            Resizer::ResizeAlgorithm algorithm = Resizer::ResizeAlgorithm::NEAREST_NEIGHBOR) = 0;
        virtual bool saveImage(const std::filesystem::path& filepath) = 0;
    };
}

#endif // IMAGE_H