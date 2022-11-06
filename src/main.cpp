#include <iostream>

#include "args.h"
#include "ascii.h"
#include "image_creator.h"

int main( int argc, char* argv[] )
{
    if ( argc < 2 )
    {
        std::cout << "Program work only with arguments!" << std::endl;
        Args::printHelp();
        return EXIT_FAILURE;
    }

    std::vector< std::string > args{ argv + 1, argv + argc };

    Args resultArgs{ args };

    if ( !resultArgs.hasFilepath() || resultArgs.hasHelp() )
    {
        resultArgs.printHelp();
        return EXIT_SUCCESS;
    }

    std::filesystem::path               imageFilepath{ resultArgs.getFilepath().value() };
    ImageCreator                        creator;
    std::unique_ptr< Image::ImageBase > image{ creator.createImage( imageFilepath ) };

    if ( !image )
    {
        std::cout << "Can't find image!" << std::endl;
        resultArgs.printHelp();
        return EXIT_FAILURE;
    }

    bool hasWidth{ resultArgs.hasWidth() };
    bool hasHeight{ resultArgs.hasHeight() };

    if ( hasWidth && hasHeight )
    {
        image->resize( *resultArgs.getWidth(), *resultArgs.getHeight() );
    }
    else if ( hasWidth )
    {
        image->resize( *resultArgs.getWidth() );
    }
    else if ( hasHeight )
    {
        image->resize();
        image->resize( image->getWidth(), *resultArgs.getHeight() );
    }
    else
    {
        image->resize();
    }

    ASCII::Converter converter;

    converter.convert( *image );
    converter.print();

    if ( resultArgs.hasSavepath() )
    {
        bool isSaveImage{ converter.save( *resultArgs.getSavepath() ) };
        if ( !isSaveImage )
        {
            std::cout << "Failed to save image!" << std::endl;
        }
    }
}