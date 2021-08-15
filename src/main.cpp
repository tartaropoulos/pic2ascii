#include <iostream>

#include "ascii.h"
#include "args.h"
#include "image_creator.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Program work only with arguments!" << std::endl;
        Args::printHelp();
        return EXIT_FAILURE;
    }

    std::vector<std::string> args{argv + 1, argv + argc};

    Args resultArgs{args};

    if ( !resultArgs.hasFilepath() ||
         resultArgs.hasHelp())
    {
        resultArgs.printHelp();
        return EXIT_SUCCESS;
    }

    std::filesystem::path imageFilepath{ resultArgs.getFilepath().value() };
    ImageCreator creator;
    std::optional result{ creator.createImage(imageFilepath) };

    if (!result.has_value())
    {
        std::cout << "Can't find image!" << std::endl;
        resultArgs.printHelp();
        return EXIT_FAILURE;
    }

    std::unique_ptr<Image::ImageBase> resultImage;
    resultImage = std::move(result.value());

    bool hasWidth{ resultArgs.hasWidth() };
    bool hasHeight{ resultArgs.hasHeight() };

    if ( hasWidth &&
         hasHeight )
    {
        resultImage->resize( *resultArgs.getWidth(), *resultArgs.getHeight() );
    }
    else if ( hasWidth )
    {
        resultImage->resize( *resultArgs.getWidth() );
    }
    else if ( hasHeight )
    {
        resultImage->resize();
        resultImage->resize( resultImage->getWidth(), *resultArgs.getHeight() );
    }
    else
    {
        resultImage->resize();
    }

    ASCII::Converter converter;

    converter.convert(resultImage);
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