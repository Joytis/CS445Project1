#pragma once 

template<typename t_enum>  
struct enable_bitmask_operators  
{
    static const bool enable = false;
};

template<typename t_enum>  
typename std::enable_if<enable_bitmask_operators<t_enum>::enable, t_enum>::type  
operator |(t_enum lhs, t_enum rhs)  
{
    using underlying = typename std::underlying_type<t_enum>::type;
    return static_cast<t_enum> (
        static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs)
    );
}


#define ENABLE_BITMASK_OPERATORS(x)  \
template<> struct enable_bitmask_operators<x> { static const bool enable = true; };

