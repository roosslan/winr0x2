#ifndef SETUP_INF_PARSER_H_INC_
#define SETUP_INF_PARSER_H_INC_

#include <string>
#include <vector>

namespace win32
{
namespace inf
{
    class line
    {
    public:
        line() { ZeroMemory( &_ctx, sizeof( _ctx ) ); }
        line( const INFCONTEXT& ctx ) : _ctx( ctx ) {};

        bool    operator==( const line& op2 ) const
        {
            return memcmp( &_ctx, &op2._ctx, sizeof( _ctx ) ) == 0;
        }
        bool    operator!=( const line& op2 ) const
        {
            return memcmp( &_ctx, &op2._ctx, sizeof( _ctx ) ) != 0;
        }
        bool    operator<( const line& op2 ) const
        {
            std::wstring str0, str1;
            if( string_at( 0, str0 ) && op2.string_at( 0, str1 ) )
                return str0 < str1;
            return memcmp( &_ctx, &op2._ctx, sizeof( _ctx ) ) < 0;
        }
        bool    isValid() const
        {
            return *this != line();
        }

        bool    string_at( unsigned int idx, std::wstring& str ) const
        {
            wchar_t buf[4096];
            DWORD size_needed = 0;
            if( SetupGetStringField( (PINFCONTEXT) &_ctx, idx, buf, sizeof( buf ), &size_needed ) )
            {
                str = buf;
                return true;
            }
            return false;
        }

        std::vector<std::wstring> contents()
        {
            std::vector<std::wstring> lst;
            for( unsigned int idx = 0; true; ++idx )
            {
                std::wstring str;
                if( !string_at( idx, str ) )
                {
                    break;
                }
                lst.push_back( str );
            }
            return lst;
        }


        operator INFCONTEXT*() { return &_ctx; }
    private:
        INFCONTEXT  _ctx;
    };

    class section
    {
    public:
        section()
        {
            ZeroMemory( &_ctx, sizeof( _ctx ) );
        }
        section( HINF hInf, const std::wstring& section_name )
            : _Name( section_name )
        {
            ZeroMemory( &_ctx, sizeof( _ctx ) );
            SetupFindFirstLine( hInf, section_name.c_str(), 0, &_ctx );
        }
        ~section()
        {
        }

        class iterator : public std::iterator<std::forward_iterator_tag,line>
        {
            friend section;
        public:
            iterator()
            {
                ZeroMemory( &_ctx, sizeof( _ctx ) );
            }
            iterator&   operator++()
            {
                INFCONTEXT tmpCtx;
                if( SetupFindNextLine( _ctx, &tmpCtx ) )
                {
                    _ctx = tmpCtx;
                }
                else
                {
                    _ctx = line();
                }
                return *this;
            }
            line    operator*()
            {
                return _ctx;
            }
            line*   operator->()
            {
                return &_ctx;
            }

            bool    operator==( const iterator& op2 ) const
            {
                return _ctx == op2._ctx;
            }
            bool    operator!=( const iterator& op2 ) const
            {
                return _ctx != op2._ctx;
            }
        private:
            iterator( INFCONTEXT& ctx )
                : _ctx( ctx )
            {}

            line            _ctx;
        };

        bool    operator<( const section& op2 ) const
        {
            return _Name < op2._Name;
        }

        iterator    begin()
        {
            return iterator( _ctx );
        }
        iterator    end()
        {
            return iterator();
        }
    private:
        std::wstring    _Name;
        INFCONTEXT      _ctx;
    };

    class inf_file
    {
    public:
        inf_file( const std::wstring& str )
        {
            UINT err_line = 0;
            _inf = SetupOpenInfFile( str.c_str(), 0, INF_STYLE_WIN4, &err_line );
            if( _inf == INVALID_HANDLE_VALUE )
            {
                DWORD err = GetLastError();
                // do something ..
                throw std::invalid_argument( "failed to open inf file" );
            }
        }
        ~inf_file()
        {
            SetupCloseInfFile( _inf );
        }
        section     get_section( const std::wstring& section_name )
        {
            return section( _inf, section_name );
        }


        class iterator : public std::iterator<std::forward_iterator_tag,section>
        {
            friend inf_file;
        public:
            iterator()  : _hInf( 0 ), _idx( 0 ) {}

            iterator&   operator++()
            {
                if( _idx < 0 )
                    _idx = 0;
                else
                    ++_idx;
                init_at( _idx );
                return *this;
            }
            const section&  operator*() const
            {
                return _Section;
            }
            section*    operator->()
            {
                return &_Section;
            }

            bool    operator==( const iterator& op2 ) const
            {
                return _idx == op2._idx;
            }
            bool    operator!=( const iterator& op2 ) const
            {
                return !(*this == op2);
            }
        private:
            void    init_at( int idx )
            {
                if( _hInf != 0 && _hInf != INVALID_HANDLE_VALUE )
                {
                    wchar_t buf[128] = { 0 };
                    UINT sizeNeeded = 0;
                    if( SetupEnumInfSectionsW( _hInf, _idx, buf, ARRAYSIZE( buf ), &sizeNeeded ) )
                    {
                        _Section = section( _hInf, buf );
                        return;
                    }
                }
                _Section = section();
            }

            iterator( HINF& ctx )
                : _idx( 0 ), _hInf( ctx )
            {}

            section         _Section;

            HINF            _hInf;
            int             _idx;
        };

        iterator    begin()
        {
            return iterator( _inf );
        }
        iterator    end()
        {
            return iterator();
        }

        struct VersionInfo 
        {
            std::wstring    signature;
            std::wstring    class_name;
            GUID            class_guid;
            std::wstring    provider;
            std::wstring    date;
            std::wstring    version;
        };

        bool    parseVersionInfo( VersionInfo& info )
        {
            section s( _inf, L"Version" );

            for( section::iterator i = s.begin(); i != s.end(); ++i )
            {
                std::vector<std::wstring> str_list = i->contents();
                if( str_list.size() > 1 )
                {
                    std::wstring& entry = str_list[0];
                    std::wstring& entry1 = str_list[1];
                    if( entry == L"Signature" )
                    {
                        info.signature = entry1;
                    }
                    else if( entry == L"Class" )
                    {
                        info.class_name = entry1;
                    }
                    else if( entry == L"ClassGUID" )
                    {
                        IIDFromString( const_cast<wchar_t*>( entry1.c_str() ), &info.class_guid );
                    }
                    else if( entry == L"Provider" )
                    {
                        info.provider = entry1;
                    }
                    else if( entry == L"DriverVer" )
                    {
                        info.date = entry1;
                        info.version = str_list[2];
                    }
                }
            }
            return true;
        }
    private:
        inf_file( const inf_file& );
        inf_file& operator=( const inf_file& );

        HINF    _inf;
    };
};
};

#endif // SETUP_INF_PARSER_H_INC_