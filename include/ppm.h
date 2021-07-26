#ifndef PPM_H
#define PPM_H

#include <filesystem>
#include <iostream>
#include <vector> 

namespace PPM
{
    enum class PPMType
    {
        P3 = 3,
        P6 = 6
    };


    class Color
    {
    private:
        int      m_r;
        int      m_g;
        int      m_b;
        PPMType  m_type;

    public:
        Color() :
            m_r{0},
            m_g{0},
            m_b{0},
            m_type{PPMType::P6}
        {};

        Color(int r, int g, int b, PPMType type = PPMType::P6) :
            m_r{r},
            m_g{g},
            m_b{b},
            m_type{type}
        {};

        bool operator==(const int value) const;
        auto operator<=>(const int value) const;

        void setColor(int r, int g, int b);

        int getR() const;
        int getG() const;
        int getB() const;
        PPMType getType() const;

        friend std::istream& operator>>(std::istream& is, Color& color);
        friend std::ostream& operator<<(std::ostream& os, Color color);
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


    class PPMImage
    {
    private:
        PPMHeader           m_header;
        std::vector<Color>  m_data;

    public:
        PPMImage() = default;
        explicit PPMImage(const std::filesystem::path& filepath);

        bool setImage(const std::filesystem::path& filepath);
        bool setColor(int x, int y, const Color& color);

        int getWidth() const;
        int getHeight() const;
        int getMaxValueColor() const;
        Color getColor(int x, int y) const;

        bool saveImage(const std::filesystem::path& filepath);
    };


    std::istream& operator>>(std::istream& is, Color& color);
    std::istream& operator>>(std::istream& is, PPMType& type);
    std::istream& operator>>(std::istream& is, PPMHeader& header);

    std::ostream& operator<<(std::ostream& os, Color color);
    std::ostream& operator<<(std::ostream& os, PPMType type);
    std::ostream& operator<<(std::ostream& os, PPMHeader header);
    
} // namespace PPM

#endif // PPM_H