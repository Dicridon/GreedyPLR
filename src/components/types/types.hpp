#ifndef __PLR__TYPES__TYPES__
#define __PLR__TYPES__TYPES__
#include <type_traits>
namespace PLR::Types {
    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;
}
#endif
