#include </usr/share/edk2/MdePkg/Include/Uefi.h>
#include </usr/share/edk2/MdePkg/Include/Protocol/LoadedImage.h>
#include </usr/share/edk2/MdePkg/Include/Guid/FileInfo.h>
#include </usr/share/edk2/MdePkg/Include/Protocol/SimpleFileSystem.h>
#include </usr/share/edk2/MdePkg/Include/Protocol/GraphicsOutput.h>
#include "efi.h"
//#include "paging.h"
#include "kernel.h"
#include "passdata.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Booting BitByte...\n\r");

    EFI_FILE_PROTOCOL* Root = NULL;
    EFI_STATUS Status;
    
    // Get the loaded image protocol
    EFI_LOADED_IMAGE_PROTOCOL* LoadedImage = NULL;
    Status = SystemTable->BootServices->HandleProtocol(
        ImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (VOID**)&LoadedImage);
    if (EFI_ERROR(Status)) return Status;

    // Get the file system protocol
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem = NULL;
    Status = SystemTable->BootServices->HandleProtocol(
        LoadedImage->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID**)&FileSystem);
    if (EFI_ERROR(Status)) return Status;
        
    Status = FileSystem->OpenVolume(FileSystem, &Root);
    if (EFI_ERROR(Status)) return Status;
        
    Status = LoadKernelFromFile(SystemTable, Root);
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to load kernel\n\r");
        return Status;
    }
    
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel loaded, passing hardware data...\n\r");

    PassDataToKernel(SystemTable, ImageHandle);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Done, exiting boot services...\n\r");
    
    // Exit boot services
    Status = ExitBootServices(SystemTable, ImageHandle);
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to exit boot services\n\r");
        return Status;
    }
    
    // Can't use BootServices anymore, so no FreePool here
    //SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Jumping to kernel...\n");

    //init_identity_paging();
    //enable_paging();
    
    JumpToKernel();
    
    // If kernel returns (should not)
    //SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Kernel returned\n");
    
    return EFI_SUCCESS;
}
