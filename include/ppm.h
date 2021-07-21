#ifndef PPM_H
#define PPM_H

#include <iostream>

namespace PPM
{
    class Color
    {
    private:
        int m_r;
        int m_g;
        int m_b;

    public:
        Color() :
            m_r{0},
            m_g{0},
            m_b{0}
        {};

        Color(int r, int g, int b) :
            m_r{r},
            m_g{g},
            m_b{b}
        {};

        void setColor(int r, int g, int b);
        void setColor(const Color& color);

        int getR() const;
        int getG() const;
        int getB() const;

        friend std::istream& operator>>(std::istream& is, Color& color);
        friend std::ostream& operator<<(std::ostream& os, Color color);
    };

    enum class PPMType
    {
        P3 = 3,
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

    };

    std::ostream& operator<<(std::ostream& os, PPMType type);
    std::istream& operator>>(std::istream& is, PPMType& type);


    
} // namespace PPM

#endif // PPM_H