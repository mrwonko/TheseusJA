#pragma once

#include <cstdarg>
#include <tuple>

template< typename... Args >
struct VarArgReader;

template< typename Head, typename... Tail >
struct VarArgReader< Head, Tail... >
{
	static std::tuple< Head, Tail... > get( va_list& ap )
	{
		std::tuple< Head > convertedHead( va_arg( ap, Head ) );
		return std::tuple_cat( std::move( convertedHead ), VarArgReader< Tail... >::get( ap ) );
	}
};

template<>
struct VarArgReader<>
{
	static std::tuple<> get( va_list& ap )
	{
		return std::make_tuple();
	}
};

class VarArgHelper
{
public:
	VarArgHelper( va_list& ap )
		: _ap( ap )
	{
	}
	~VarArgHelper()
	{
		va_end( _ap );
	}
	template< typename... Args >
	std::tuple< Args... > args()
	{
		return VarArgReader< Args... >::get( _ap );
	}
private:
	va_list& _ap;
};
