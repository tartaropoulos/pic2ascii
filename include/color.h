#ifndef COLOR_H
#define COLOR_H

namespace Color
{
    class ColorBase
    {
    public:
        virtual ~ColorBase() = default;

        virtual bool operator==(const int value) const = 0;
        virtual std::strong_ordering operator<=>(const int value) const = 0;

        virtual void setColor(int r, int g, int b) = 0;

        virtual int getR() const = 0;
        virtual int getG() const = 0;
        virtual int getB() const = 0;
    };
}

#endif // COLOR_H