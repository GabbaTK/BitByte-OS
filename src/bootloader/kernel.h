#define KERNEL_LOAD_ADDRESS ((VOID*)0x101000)

typedef void (*kernel_entry_t)(void);

VOID* CopyMem(VOID* Dest, CONST VOID* Src, UINTN Length) {
    UINT8* D = (UINT8*)Dest;
    CONST UINT8* S = (CONST UINT8*)Src;
    for (UINTN i = 0; i < Length; i++) {
        D[i] = S[i];
    }
    return Dest;
}

EFI_STATUS LoadFileToMemory(EFI_SYSTEM_TABLE* SystemTable, EFI_FILE_PROTOCOL* Root, CHAR16* FileName, VOID** Buffer, UINTN* BufferSize) {
    EFI_STATUS Status;
    EFI_FILE_PROTOCOL* File;

    Status = Root->Open(Root, &File, FileName, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) return Status;

    UINTN FileInfoSize = sizeof(EFI_FILE_INFO) + 200;
    EFI_FILE_INFO* FileInfo = NULL;
    Status = SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (VOID**)&FileInfo);
    if (EFI_ERROR(Status)) {
        File->Close(File);
        return Status;
    }

    Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR(Status)) {
        SystemTable->BootServices->FreePool(FileInfo);
        File->Close(File);
        return Status;
    }

    *BufferSize = FileInfo->FileSize;
    SystemTable->BootServices->FreePool(FileInfo);

    Status = SystemTable->BootServices->AllocatePool(EfiLoaderData, *BufferSize, Buffer);
    if (EFI_ERROR(Status)) {
        File->Close(File);
        return Status;
    }
    
    Status = File->Read(File, BufferSize, *Buffer);
    File->Close(File);
    return Status;
}

EFI_STATUS LoadKernelFromFile(EFI_SYSTEM_TABLE* SystemTable, EFI_FILE_PROTOCOL* Root) {
    VOID* KernelBuffer = NULL;
    UINTN KernelSize = 0;
    EFI_STATUS Status;

    Status = LoadFileToMemory(SystemTable, Root, L"\\kernel.bin", &KernelBuffer, &KernelSize);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    CopyMem(KERNEL_LOAD_ADDRESS, KernelBuffer, KernelSize);

    SystemTable->BootServices->FreePool(KernelBuffer);
    return EFI_SUCCESS;
}

EFI_STATUS ExitBootServices(EFI_SYSTEM_TABLE* SystemTable, EFI_HANDLE ImageHandle) {
    EFI_STATUS Status;
    UINTN MemoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
    
    Status = SystemTable->BootServices->GetMemoryMap(
        &MemoryMapSize,
        MemoryMap,
        &MapKey,
        &DescriptorSize,
        &DescriptorVersion
    );
    
    if (Status != EFI_BUFFER_TOO_SMALL) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Unexpected status getting memory map size\n\r");
        return Status;
    }
    
    // Allocate memory map buffer
    MemoryMapSize += DescriptorSize * 2; // extra buffer
    Status = SystemTable->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, (VOID**)&MemoryMap);
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to allocate memory for memory map\n\r");
        return Status;
    }
    
    // Get the actual memory map
    Status = SystemTable->BootServices->GetMemoryMap(
        &MemoryMapSize,
        MemoryMap,
        &MapKey,
        &DescriptorSize,
        &DescriptorVersion
    );
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to get memory map\n\r");
        SystemTable->BootServices->FreePool(MemoryMap);
        return Status;
    }
    
    // Exit boot services
    Status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to exit boot services\n\r");
        SystemTable->BootServices->FreePool(MemoryMap);
        return Status;
    }

    return EFI_SUCCESS;
}

void JumpToKernel() {
    kernel_entry_t KernelEntry = (kernel_entry_t)KERNEL_LOAD_ADDRESS;
    KernelEntry(); // Should never return if kernel runs properly
}