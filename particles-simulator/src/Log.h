#pragma once


#ifdef PS_ENABLE_LOG
    #define PS_LOG(...) \
    { \
        printf("%s at line %d: ", __FILE__, __LINE__); \
        printf(__VA_ARGS__); \
        putchar('\n'); \
    }

    #define PS_EVENT_LOG(e, ...) \
    { \
        printf("Event #%d %s: ", e.getType(), e.getName()); \
        printf(__VA_ARGS__); \
        putchar('\n'); \
    }

    #define GL_CALL(func) \
        GLerrorClear();\
        func;\
        PS_ASSERT(GLerrorCheck(), #func" failed.");
#else
    #define PS_LOG(...)
    #define PS_EVENT_LOG(...)
    #define GL_CALL(func) func
#endif // PS_ENABLE_LOG


#ifdef PS_ENABLE_ASSERT
    #ifdef PS_SYSTEM_WINDOWS
        #define PS_ASSERT(x, ...) \
        { \
            if (!x) \
            {\
                printf("ASSERTION FAILED!:\n"); \
                PS_LOG(__VA_ARGS__) \
                __debugbreak(); \
            } \
        }
    #else
        #define PS_ASSERT(x, ...) \
		{ \
            if (!x) \
            {\
                printf("ASSERTION FAILED!:\n"); \
                PS_LOG(__VA_ARGS__) \
            } \
        }
    #endif
#else
    #define PS_ASSERT(...)
#endif // PS_ENABLE_ASSERT


void GLerrorClear();
bool GLerrorCheck();
