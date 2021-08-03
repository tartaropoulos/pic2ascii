#ifndef IMAGE_CREATOR_H
#define IMAGE_CREATOR_H

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

#include "image.h"
#include "ppm.h"

class ImageCreator
{
private:
    const std::unordered_map
    < 
        std::string, 
        std::function< std::unique_ptr< Image::ImageBase >(const std::filesystem::path&) > 
    > 
    mc_createImageFunctions
    {
        { ".ppm", [](const std::filesystem::path& filepath) { return std::make_unique<PPM::PPMImage>(filepath); } },
    };

public:
    std::optional< std::unique_ptr<Image::ImageBase> > createImage(const std::filesystem::path& filepath)
    {
        // If file not exists or had wrong extension
        if ( !std::filesystem::exists(filepath) ||
            !mc_createImageFunctions.contains( filepath.string() ) )
        {
            return {};
        }

        return mc_createImageFunctions.at( filepath.extension().string() )(filepath);
    }
};

#endif // IMAGE_CREATOR_H