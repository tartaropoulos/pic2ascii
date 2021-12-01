#include "image_creator.h"

std::optional< std::unique_ptr< Image::ImageBase > > ImageCreator::createImage( const std::filesystem::path& filepath )
{
    if ( !filepath.has_extension() )
    {
        return {};
    }

    std::string extension{ filepath.extension().string() };

    // If file not exists or had wrong extension
    if ( !std::filesystem::exists( filepath ) || !mc_createImageFunctions.contains( extension ) )
    {
        return {};
    }

    return mc_createImageFunctions.at( extension )( filepath );
}