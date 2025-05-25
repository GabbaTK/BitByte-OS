#include "../datatypes.h"

// Individual
EFI_GRAPHICS_OUTPUT_PROTOCOL* GetGOP(EFI_SYSTEM_TABLE* SystemTable) {
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;
    EFI_GUID GOPGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    EFI_STATUS Status = SystemTable->BootServices->LocateProtocol(
        &GOPGuid, NULL, (VOID**)&GOP
    );

    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to get GOP\n");
        return NULL;
    }

    GOP->SetMode(GOP, 0);

    return GOP;
}

// All
EFI_STATUS PassDataToKernel(EFI_SYSTEM_TABLE* SystemTable, EFI_HANDLE ImageHandle) {
    KDT_BOOT_INFO* Info = BOOTINFO_ADDRESS;

    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP = GetGOP(SystemTable);
    
    // GOP
    Info->FrameBufferBase = (UINT64)GOP->Mode->FrameBufferBase;
    Info->FrameBufferSize = GOP->Mode->FrameBufferSize;
    Info->HorizontalResolution = GOP->Mode->Info->HorizontalResolution;
    Info->VerticalResolution = GOP->Mode->Info->VerticalResolution;
    Info->PixelsPerScanLine = GOP->Mode->Info->PixelsPerScanLine;

    return EFI_SUCCESS;
}