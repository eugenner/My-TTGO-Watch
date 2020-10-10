#ifndef _ALLOC_H
    #if defined( BOARD_HAS_PSRAM )
        #define MALLOC         ps_malloc
        #define CALLOC         ps_calloc
        #define REALLOC        ps_realloc
    #else
        #define MALLOC         malloc
        #define CALLOC         calloc
        #define REALLOC        realloc
    #endif // BOARD_HAS_PSRAM
#endif // _ALLOC_H