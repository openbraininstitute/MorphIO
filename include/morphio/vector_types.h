#pragma once

#if !gsl_CONFIG_ALLOWS_SPAN_COMPARISON
#include <algorithm>
#endif
#include <array>
#include <cmath>  // M_PI
#include <vector>

#include <gsl-lite/gsl-lite.hpp>

namespace morphio {

template <typename T>
using range = gsl_lite::span<T>;

#if !gsl_CONFIG_ALLOWS_SPAN_COMPARISON
template <typename T>
inline bool operator==(morphio::range<T> l, morphio::range<T> r) {
    return l.size() == r.size() &&
           (l.data() == r.data() || std::equal(l.begin(), l.end(), r.begin()));
}
#endif

#ifdef MORPHIO_USE_DOUBLE
using floatType = double;
constexpr floatType epsilon = 1e-6;
constexpr floatType PI = M_PI;
constexpr int FLOAT_PRECISION_PRINT = 17;

#else
/** Type of float to use. Can be double or float depending on MORPHIO_USE_DOUBLE */
using floatType = float;
/** A really small value that is used to measure how close are two values */
constexpr floatType epsilon = 1e-6F;
constexpr floatType PI = static_cast<floatType>(M_PI);
constexpr int FLOAT_PRECISION_PRINT = 9;
#endif

/** An array of size 3 for x,y,z coordinates */
using Point = std::array<morphio::floatType, 3>;
/** An array of points */
using Points = std::vector<Point>;

}  // namespace morphio
