#ifndef ARGS_H
#define ARGS_H

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

class Args
{
private:
    std::filesystem::path m_filepath{ "" };
    std::filesystem::path m_savepath{ "" };
    int                   m_width{ 0 };
    int                   m_height{ 0 };

    const std::string m_savepathFlag{ "-s" };
    const std::string m_widthFlag{ "-w" };
    const std::string m_heightFlag{ "-h" };
    const std::string m_helpFlag{ "--help" };

    bool m_hasFilepath{ false };
    bool m_hasSavepath{ false };
    bool m_hasWidth{ false };
    bool m_hasHeight{ false };
    bool m_hasHelp{ false };

public:
    Args() = delete;
    Args( std::vector< std::string >& args );

    std::optional< std::filesystem::path > getFilepath() const;
    std::optional< std::filesystem::path > getSavepath() const;
    std::optional< int >                   getWidth() const;
    std::optional< int >                   getHeight() const;

    bool hasFilepath() const;
    bool hasSavepath() const;
    bool hasWidth() const;
    bool hasHeight() const;
    bool hasHelp() const;

    static void printHelp();

private:
    template < typename T >
    std::optional< T > checkReturn( T value, std::function< bool( const Args& ) > hasValue ) const
    {
        if ( hasValue( *this ) )
        {
            return value;
        }

        return {};
    }
};

#endif // ARGS_H