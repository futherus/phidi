#ifndef DEBUG_HH
#define DEBUG_HH

#define RESET "\u001b[0m"
#define BLACK "\u001b[30m"
#define RED "\u001b[31m"
#define GREEN "\u001b[32m"
#define YELLOW "\u001b[33m"
#define BLUE "\u001b[34m"
#define MAGENTA "\u001b[35m"
#define CYAN "\u001b[36m"
#define WHITE "\u001b[37m"

#define $M(FMT, ...) std::fprintf(stderr, BLUE "MSG: " RESET FMT, ##__VA_ARGS__)

#ifndef NDEBUG
    #define $$ std::fprintf(stderr, BLUE "\t\% " RESET "%s:%d %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    #define $D(FMT, ...) std::fprintf(stderr, MAGENTA "DBG: " RESET FMT, ##__VA_ARGS__)
    #ifndef NFUNC
        #define $FUNC std::fprintf(stderr, YELLOW "-> " RESET "%s:%d %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    #else
        #define $FUNC
    #endif
#else
    #define $FUNC
    #define $$
    #define $D(FMT, ...)
#endif

#endif // DEBUG_HH