#include <stdint.h>

typedef unsigned char                 T_BOOL;
typedef signed long long int          T_INTN;
typedef unsigned long long int        T_UINTN;
typedef signed char                   T_INT8;
typedef unsigned char                 T_UINT8;
typedef signed short int              T_INT16;
typedef unsigned short int            T_UINT16;
typedef signed long int               T_INT32;
typedef unsigned long int             T_UINT32;
typedef signed long long int          T_INT64;
typedef unsigned long long int        T_UINT64;
typedef unsigned char                 T_CHAR8;
typedef unsigned short                T_CHAR16;

// KDT (Kernel Data Type) definitions for passing data from bootloader to kernel

typedef struct {
    // Graphics Output Protocol
    T_UINT64 FrameBufferBase;
    T_UINT64 FrameBufferSize;
    T_UINT32 HorizontalResolution;
    T_UINT32 VerticalResolution;
    T_UINT32 PixelsPerScanLine;
} KDT_BOOT_INFO;

#define BOOTINFO_ADDRESS ((KDT_BOOT_INFO*)0x100000)
