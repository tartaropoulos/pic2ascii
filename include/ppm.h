#ifndef PPM_H
#define PPM_H

#include <filesystem>
#include <iostream>
#include <vector> 

#include "color.h"
#include "image.h"
#include "resizer.h"
namespace PPM
{
    enum class PPMType
    {
        P3 = 3,
        P6 = 6
    };


    class PPMColor : public Color::ColorBase
    {
    private:
        int      m_r;
        int      m_g;
        int      m_b;
        PPMType  m_type;

    public:
        PPMColor() :
            m_r{0},
            m_g{0},
            m_b{0},
            m_type{PPMType::P6}
        {};

        PPMColor(int r, int g, int b, PPMType type = PPMType::P6) :
            m_r{r},
            m_g{g},
            m_b{b},
            m_type{type}
        {};

        ~PPMColor() = default;

        bool operator==(const int value) const override;
        std::strong_ordering operator<=>(const int value) const override;

        void setColor(int r, int g, int b) override;

        int getR() const override;
        int getG() const override;
        int getB() const override;
        PPMType getType() const;

        friend std::istream& operator>>(std::istream& is, PPMColor& color);
        friend std::ostream& operator<<(std::ostream& os, PPMColor color);
    };


    class PPMHeader
    {
    private:
        PPMType  m_type;
        int      m_width;
        int      m_height;
        int      m_maxValueColor;

    public:
        PPMHeader() :
            m_type{PPMType::P3},
            m_width{0},
            m_height{0},
            m_maxValueColor{255}
        {};

        PPMHeader(PPMType type, int width, int height, int maxValueColor) :
            m_type{type},
            m_width{width},
            m_height{height},
            m_maxValueColor{maxValueColor}
        {};

        PPMType getType() const;
        int getWidth() const;
        int getHeight() const;
        int getMaxValueColor() const;

        friend std::istream& operator>>(std::istream& is, PPMHeader& header);
        friend std::ostream& operator<<(std::ostream& os, PPMHeader header);
    };


    class PPMImage : public Image::ImageBase
    {
    private:
        PPMHeader              m_header;
        std::vector<PPMColor>  m_data;

    public:
        PPMImage() = default;
        explicit PPMImage(const std::filesystem::path& filepath);

        bool setImage(const std::filesystem::path& filepath) override;
        bool setColor(int x, int y, const Color::ColorBase& color) override;

        int getWidth() const override;
        int getHeight() const override;
        int getMaxValueColor() const override;
        std::unique_ptr<Color::ColorBase> getColor(int x, int y) const override;

        bool resize(
            int newWidth = 50, 
            int newHeight = 0, 
            Resizer::ResizeAlgorithm algorithm = Resizer::ResizeAlgorithm::NEAREST_NEIGHBOR) override;
        bool saveImage(const std::filesystem::path& filepath) override;
    };


    std::istream& operator>>(std::istream& is, PPMColor& color);
    std::istream& operator>>(std::istream& is, PPMType& type);
    std::istream& operator>>(std::istream& is, PPMHeader& header);

    std::ostream& operator<<(std::ostream& os, PPMColor color);
    std::ostream& operator<<(std::ostream& os, PPMType type);
    std::ostream& operator<<(std::ostream& os, PPMHeader header);
    
} // namespace PPM

#endif // PPM_H