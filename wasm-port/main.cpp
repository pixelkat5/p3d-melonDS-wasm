#include <GPU.h>
#include <NDS.h>
#include <NDSCart.h>
#include <Config.h>
#include <SPI.h>
#include <SPU.h>
#include <emscripten.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

extern "C" {

// Interleaved S16 stereo audio scratch buffer, filled by readAudioOutput()
// and read back by JS via getSymbol(6). Declared up here since getSymbol()
// references it below.
static const int kAudioBufSamples = 2048; // stereo sample pairs
static s16 AudioBuf[kAudioBufSamples * 2];

int main() {
  Config::DLDIEnable = 1;
  // SD card image is mounted by JS at this path inside the IDBFS-backed
  // virtual filesystem. Must match the path used in index.html's FS.mount().
  strcpy(Config::DLDISDPath, "/data/sd.img");

  printf("NDS Init %d\n", NDS::Init());
  GPU::InitRenderer(0);
  GPU::RenderSettings r;
  r.Soft_Threaded = false;
  r.GL_ScaleFactor = 1;
  r.GL_BetterPolygons = false;
  GPU::SetRenderSettings(0, r);

  printf("wasm ready.\n");
  EM_ASM(wasmReady(););
}

void* getSymbol(int id) {
  if (id == 0) {
    return NDS::ARM7BIOS;
  }
  if (id == 1) {
    return NDS::ARM9BIOS;
  }
  if (id == 2) {
    return SPI_Firmware::Firmware;
  }
  if (id == 3) {
    return NDSCart::CartROM;
  }
  if (id == 4) {
    return GPU::Framebuffer[0][0];
  }
  if (id == 5) {
    return &(GPU::FrontBuffer);
  }
  if (id == 6) {
    return AudioBuf;
  }
  return 0;
}

void reset() {
  NDS::Reset();
  // memset(NDSCart::CartROM, 0, sizeof(NDSCart::CartROM));
}

int loadROM(int romLen) {
  NDS::LoadROM((const u8*)NULL, romLen, "sram", true);
  return 0;
}

u32 runFrame() { return NDS::RunFrame(); }

// Active-low 12-bit button mask, matching the DS KEYINPUT register layout:
// bit0=A bit1=B bit2=Select bit3=Start bit4=Right bit5=Left bit6=Up
// bit7=Down bit8=R bit9=L (bits 10-11 unused, keep as 1/released).
// 0 = pressed, 1 = released. Default (nothing pressed) is 0xFFF.
void setKeyMask(u32 mask) {
  NDS::SetKeyMask(mask);
}

void touchScreen(int x, int y) {
  NDS::TouchScreen((u16)x, (u16)y);
}

void releaseScreen() {
  NDS::ReleaseScreen();
}

// SPU produces interleaved S16 stereo samples at the DS's native mixer
// rate (ARM7 clock / 1024 ~= 32729 Hz - see SPU::Mix()'s schedule interval).
// JS reads them out after every runFrame() call and queues them as a Web
// Audio AudioBuffer, which resamples to the output device's rate
// automatically - no manual resampling needed on this side.
int readAudioOutput() {
  int avail = SPU::GetOutputSize();
  if (avail > kAudioBufSamples) avail = kAudioBufSamples;
  if (avail <= 0) return 0;
  return SPU::ReadOutput(AudioBuf, avail);
}

}
