#ifndef ARGS_H
#define ARGS_H

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

class Args
{
private:
    std::filesystem::path m_filepath;
    int m_width;
    int m_height;

    bool m_hasFilepath;
    bool m_hasWidth;
    bool m_hasHeight;
    bool m_hasHelp;

public:
    Args() = delete;
    Args(std::vector< std::string >& args);

    std::optional< std::filesystem::path > getFilepath() const;
    std::optional< int > getWidth() const;
    std::optional< int > getHeight() const;

    bool hasFilepath() const;
    bool hasWidth() const;
    bool hasHeight() const;
    bool hasHelp() const;

    void printHelp() const;

private:
    template<typename T>
    std::optional<T> checkReturn(T value, std::function< bool( const Args& ) > hasValue) const
    {
        if ( hasValue() )
        {
            return value;
        }

        return {};
    }
};

#endif // ARGS_H