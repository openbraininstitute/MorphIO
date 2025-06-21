#include "./utils.h"

namespace morphio {

#ifdef HAS_LOCALE_EXTENSIONS

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define freelocale _free_locale
#define str_to_long _strtol_l

#ifdef MORPHIO_USE_DOUBLE
#define str_to_float _strtod_l
#else
#define str_to_float _strtof_l
#endif

#else  // not WIN32
#define str_to_long strtol_l

#ifdef MORPHIO_USE_DOUBLE
#define str_to_float strtod_l
#else
#define str_to_float strtof_l
#endif

#endif

#else // not HAS_LOCALE_EXTENSIONS

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define str_to_long _strtol

#ifdef MORPHIO_USE_DOUBLE
#define str_to_float _strtod
#else
#define str_to_float _strtof
#endif

#else // not WIN32
#define str_to_long strtol

#ifdef MORPHIO_USE_DOUBLE
#define str_to_float strtod
#else
#define str_to_float strtof
#endif

#endif

#endif  // HAS_LOCALE_EXTENSIONS

// Only create the `locale` to facilitate number handling once
StringToNumber::StringToNumber()
#ifdef HAS_LOCALE_EXTENSIONS
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    // On windows, use their locale handling for their runtime
    : locale(_create_locale(LC_ALL, "C")){}
#else
    // On other platforms, use the POSIX version
    : locale(newlocale(LC_NUMERIC_MASK, "POSIX", nullptr)) {
}
#endif
#else
    {}
#endif

    StringToNumber::~StringToNumber() {
#ifdef HAS_LOCALE_EXTENSIONS
    freelocale(locale);
#endif
}

std::tuple<int64_t, size_t> StringToNumber::toInt(const std::string& s, size_t offset) const {
    const size_t base = 10;
    const char* pos = &s[offset];
    const char* endpos = &s[s.size()];

#ifdef HAS_LOCALE_EXTENSIONS
    int64_t ret = str_to_long(pos, const_cast<char**>(&endpos), base, locale);
#else
    int64_t ret = str_to_long(pos, const_cast<char**>(&endpos), base);
#endif

    auto new_offset = static_cast<size_t>(endpos - s.data());

    if (ret == 0 && new_offset == 0) {
        throw std::invalid_argument("could not parse integer");
    }

    return {ret, new_offset};
}

std::tuple<floatType, size_t> StringToNumber::toFloat(const std::string& s, size_t offset) const {
    const char* pos = &s[offset];
    const char* endpos = &s[s.size()];

#ifdef HAS_LOCALE_EXTENSIONS
    floatType ret = str_to_float(pos, const_cast<char**>(&endpos), locale);
#else
    floatType ret = str_to_float(pos, const_cast<char**>(&endpos));
#endif

    auto new_offset = static_cast<size_t>(endpos - s.data());

    if (std::fabs(ret - 0) < morphio::epsilon && new_offset == 0) {
        throw std::invalid_argument("could not parse float");
    }

    return {ret, new_offset};
}

StringToNumber& getStringToNumber() {
    static StringToNumber stn;
    return stn;
}

#ifdef HAS_LOCALE_EXTENSIONS
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#undef freelocale
#endif
#endif

#undef str_to_long
#undef str_to_float

}  // namespace morphio
