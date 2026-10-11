#pragma once
#include <locale>  // std::locale
#include <string>

#include <morphio/vector_types.h>  // floatType

namespace morphio {

struct LocaleGuard {
    std::locale old;
    LocaleGuard(const std::locale& newLoc)
        : old(std::locale::global(newLoc)) { }
    ~LocaleGuard() {
        std::locale::global(old);
    }
};

inline std::tuple<int64_t, size_t> toInt(const std::string& s, size_t offset) {
    const size_t base = 10;
    const char* pos = &s[offset];
    const char* endpos = &s[s.size()];
    int64_t ret = strtol(pos, const_cast<char**>(&endpos), base);

    auto new_offset = static_cast<size_t>(endpos - s.data());

    if (ret == 0 && new_offset == 0) {
        throw std::invalid_argument("could not parse integer");
    }

    return {ret, new_offset};
}

inline std::tuple<floatType, size_t> toFloat(const std::string& s, size_t offset) {
    const char* pos = &s[offset];
    const char* endpos = &s[s.size()];

#ifdef MORPHIO_USE_DOUBLE
    floatType ret = strtod(pos, const_cast<char**>(&endpos));
#else
    floatType ret = strtof(pos, const_cast<char**>(&endpos));
#endif

    auto new_offset = static_cast<size_t>(endpos - s.data());

    if (std::fabs(ret - 0) < morphio::epsilon && new_offset == 0) {
        throw std::invalid_argument("could not parse float");
    }

    return {ret, new_offset};
}

}  // namespace morphio
