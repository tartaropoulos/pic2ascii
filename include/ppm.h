#ifndef PPM_H
#define PPM_H

namespace PPM
{
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


    
} // namespace PPM


#endif // PPM_H