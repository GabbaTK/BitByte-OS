#include "../datatypes.h"

__attribute__((section(".start"))) void _start();

KDT_BOOT_INFO* bootInfo;

// GOP (Graphics Output Protocol)
unsigned int* framebuffer;
unsigned long framebufferBase;
unsigned long framebufferSize;
unsigned int framebufferWidth;
unsigned int framebufferHeight;
unsigned int framebufferPitch;

void getGOP() {
    framebufferBase = bootInfo->FrameBufferBase;
    framebufferSize = bootInfo->FrameBufferSize;
    framebufferWidth = bootInfo->HorizontalResolution;
    framebufferHeight = bootInfo->VerticalResolution;

    //#ifdef QEMU
    //framebufferPitch = bootInfo->HorizontalResolution;
    //#else
    //framebufferPitch = bootInfo->PixelsPerScanLine;
    //#endif
    framebufferPitch = bootInfo->HorizontalResolution;

    framebuffer = (unsigned int*)framebufferBase;
}

void _start() {
    bootInfo = BOOTINFO_ADDRESS;

    getGOP();
    
    framebuffer[0]=0x0;
    framebuffer[1]=0x0;
    framebuffer[2]=0x0;
    framebuffer[3]=0x0;
    framebuffer[4]=0x0;
    framebuffer[5]=0x0;
    framebuffer[6]=0x0;
    framebuffer[7]=0x0;
    framebuffer[8]=0x0;
    framebuffer[9]=0x0;
    framebuffer[10]=0x0;
    framebuffer[11]=0x0;
    framebuffer[12]=0x0;
    framebuffer[13]=0x0;
    framebuffer[14]=0x0;
    framebuffer[15]=0x0;
    framebuffer[framebufferPitch]=0x0;
    framebuffer[framebufferPitch + 1]=0x0;
    framebuffer[framebufferPitch + 2]=0x0;
    framebuffer[framebufferPitch + 3]=0x0;
    framebuffer[framebufferPitch + 4]=0x00FF0000;
    framebuffer[framebufferPitch + 5]=0x0000FF00;
    framebuffer[framebufferPitch + 6]=0x0;
    framebuffer[framebufferPitch + 7]=0x0;
    framebuffer[framebufferPitch + 8]=0x0;
    framebuffer[framebufferPitch + 9]=0x0;
    framebuffer[framebufferPitch + 10]=0x0;
    framebuffer[framebufferPitch + 11]=0x0;
    framebuffer[framebufferPitch + 12]=0x0;
    framebuffer[framebufferPitch + 13]=0x000000FF;
    framebuffer[framebufferPitch + 14]=0x0;
    framebuffer[framebufferPitch + 15]=0x0;
    framebuffer[framebufferPitch * 2]=0x0;
    framebuffer[framebufferPitch * 2 + 1]=0x0;
    framebuffer[framebufferPitch * 2 + 2]=0x0;
    framebuffer[framebufferPitch * 2 + 3]=0x0;
    framebuffer[framebufferPitch * 2 + 4]=0xFFFFFFFF;
    framebuffer[framebufferPitch * 2 + 5]=0xFFFFFFFF;
    framebuffer[framebufferPitch * 2 + 6]=0x0;
    framebuffer[framebufferPitch * 2 + 7]=0x0;
    framebuffer[framebufferPitch * 2 + 8]=0x0;
    framebuffer[framebufferPitch * 2 + 9]=0x0;
    framebuffer[framebufferPitch * 2 + 10]=0x0;
    framebuffer[framebufferPitch * 2 + 11]=0xFFFFFFFF;
    framebuffer[framebufferPitch * 2 + 12]=0x0;
    framebuffer[framebufferPitch * 2 + 13]=0x0;
    framebuffer[framebufferPitch * 2 + 14]=0x0;
    framebuffer[framebufferPitch * 2 + 15]=0x0;
    framebuffer[framebufferPitch * 3]=0x0;
    framebuffer[framebufferPitch * 3 + 1]=0x0;
    framebuffer[framebufferPitch * 3 + 2]=0x0;
    framebuffer[framebufferPitch * 3 + 3]=0x0;
    framebuffer[framebufferPitch * 3 + 4]=0x0;
    framebuffer[framebufferPitch * 3 + 5]=0x0;
    framebuffer[framebufferPitch * 3 + 6]=0x0;
    framebuffer[framebufferPitch * 3 + 7]=0x0;
    framebuffer[framebufferPitch * 3 + 8]=0x0;
    framebuffer[framebufferPitch * 3 + 9]=0x0;
    framebuffer[framebufferPitch * 3 + 10]=0xFFFFFFF;
    framebuffer[framebufferPitch * 3 + 11]=0x0;
    framebuffer[framebufferPitch * 3 + 12]=0x0;
    framebuffer[framebufferPitch * 3 + 13]=0x0;
    framebuffer[framebufferPitch * 3 + 14]=0x0;
    framebuffer[framebufferPitch * 3 + 15]=0x0;
    framebuffer[framebufferPitch * 4]=0x0;
    framebuffer[framebufferPitch * 4 + 1]=0x0;
    framebuffer[framebufferPitch * 4 + 2]=0x0;
    framebuffer[framebufferPitch * 4 + 3]=0x0;
    framebuffer[framebufferPitch * 4 + 4]=0x0;
    framebuffer[framebufferPitch * 4 + 5]=0x0;
    framebuffer[framebufferPitch * 4 + 6]=0x0;
    framebuffer[framebufferPitch * 4 + 7]=0x0;
    framebuffer[framebufferPitch * 4 + 8]=0x0;
    framebuffer[framebufferPitch * 4 + 9]=0x0;
    framebuffer[framebufferPitch * 4 + 10]=0xFFFFFFF;
    framebuffer[framebufferPitch * 4 + 11]=0x0;
    framebuffer[framebufferPitch * 4 + 12]=0x0;
    framebuffer[framebufferPitch * 4 + 13]=0x0;
    framebuffer[framebufferPitch * 4 + 14]=0x0;
    framebuffer[framebufferPitch * 4 + 15]=0x0;
    framebuffer[framebufferPitch * 5]=0x0;
    framebuffer[framebufferPitch * 5 + 1]=0x0;
    framebuffer[framebufferPitch * 5 + 2]=0x0;
    framebuffer[framebufferPitch * 5 + 3]=0x0;
    framebuffer[framebufferPitch * 5 + 4]=0xFFFFFFFF;
    framebuffer[framebufferPitch * 5 + 5]=0xFFFFFFFF;
    framebuffer[framebufferPitch * 5 + 6]=0x0;
    framebuffer[framebufferPitch * 5 + 7]=0x0;
    framebuffer[framebufferPitch * 5 + 8]=0x0;
    framebuffer[framebufferPitch * 5 + 9]=0x0;
    framebuffer[framebufferPitch * 5 + 10]=0x0;
    framebuffer[framebufferPitch * 5 + 11]=0xFFFFFFFF;
    framebuffer[framebufferPitch * 5 + 12]=0x0;
    framebuffer[framebufferPitch * 5 + 13]=0x0;
    framebuffer[framebufferPitch * 5 + 14]=0x0;
    framebuffer[framebufferPitch * 5 + 15]=0x0;
    framebuffer[framebufferPitch *6]=0x0;
    framebuffer[framebufferPitch *6 + 1]=0x0;
    framebuffer[framebufferPitch *6 + 2]=0x0;
    framebuffer[framebufferPitch *6 + 3]=0x0;
    framebuffer[framebufferPitch *6 + 4]=0xFFFFFFFF;
    framebuffer[framebufferPitch *6 + 5]=0xFFFFFFFF;
    framebuffer[framebufferPitch *6 + 6]=0x0;
    framebuffer[framebufferPitch *6 + 7]=0x0;
    framebuffer[framebufferPitch *6 + 8]=0x0;
    framebuffer[framebufferPitch *6 + 9]=0x0;
    framebuffer[framebufferPitch *6 + 10]=0x0;
    framebuffer[framebufferPitch *6 + 11]=0x0;
    framebuffer[framebufferPitch *6 + 12]=0x0;
    framebuffer[framebufferPitch *6 + 13]=0xFFFFFFFF;
    framebuffer[framebufferPitch *6 + 14]=0x0;
    framebuffer[framebufferPitch *6 + 15]=0x0;
    framebuffer[framebufferPitch*7+0]=0x0;
    framebuffer[framebufferPitch*7+1]=0x0;
    framebuffer[framebufferPitch*7+2]=0x0;
    framebuffer[framebufferPitch*7+3]=0x0;
    framebuffer[framebufferPitch*7+4]=0x0;
    framebuffer[framebufferPitch*7+5]=0x0;
    framebuffer[framebufferPitch*7+6]=0x0;
    framebuffer[framebufferPitch*7+7]=0x0;
    framebuffer[framebufferPitch*7+8]=0x0;
    framebuffer[framebufferPitch*7+9]=0x0;
    framebuffer[framebufferPitch*7+10]=0x0;
    framebuffer[framebufferPitch*7+11]=0x0;
    framebuffer[framebufferPitch*7+12]=0x0;
    framebuffer[framebufferPitch*7+13]=0x0;
    framebuffer[framebufferPitch*7+14]=0x0;
    framebuffer[framebufferPitch*7+15]=0x0;

    while (1);
}
