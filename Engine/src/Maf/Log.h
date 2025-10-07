#pragma once
#include <cmath>
#include <algorithm>

namespace Math
{
    class Mathf
    {
     public:
        
        static inline float Log(float value)
        {
            // avoid log(0) or negative
            value = std::max(value, 1e-37f);
            return std::log(value);
        }

        // logarithm with custom base
        static inline float Log(float value, float base)
        {
            value = std::max(value, 1e-37f);
            base = std::max(base, 1e-37f);
            return std::log(value) / std::log(base);
        }

        // Base-10 logarithm
        static inline float Log10(float value)
        {
            value = std::max(value, 1e-37f);
            return std::log10(value);
        }
    };
}


