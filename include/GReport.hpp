#ifndef _GREPORT_HPP_
#define _GREPORT_HPP_

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <unistd.h> 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace GTEST {

struct PixelFormat{
    Uint32 fmt;
    const char* name;
};

struct ImageFormat{
    int fmt;
    const char* name;
};

struct RenderBackend{
    std::string name;
    bool accelerated;
    bool targetTexture;
    int maxTextureWidth;
    int maxTextureHeight;
};

struct HardwareCaps{
    int maxTextureWidth;
    int maxTextureHeight;
    bool accelerated;
    bool vsync;
    bool targetTexture;
    std::vector<std::string> supportedBlendModes;
};

struct EventSupport{
    bool keyboard;
    bool mouse;
    bool quit;
};

struct FontSupport{
    bool ttfInit;
    bool fontLoad;
};

struct AudioSupport{
    bool mixerInit;
    bool audioOpen;
};

struct EnvInfo{
    std::string sessionType;
    std::string display;
    std::string wayland;
    bool kmsAccess;
};

class GReport{
public:
    GReport(int v_width, int v_height);
    void RunAll();
    void Print() const;
    void Save(const std::string& path) const;

    std::vector<PixelFormat> Get_PixelFormat();
    std::vector<std::string> Get_VideoDrivers();
    std::vector<ImageFormat> Get_ImageFormat();
    std::vector<RenderBackend> Get_RenderBackend();
    std::vector<HardwareCaps> Get_HardwareCaps();
    std::vector<EventSupport> Get_EventSupport();
    
    std::vector<FontSupport> Get_FontsSupport();
    std::vector<AudioSupport> Get_AudioSupport();
    std::vector<EnvInfo> Get_Environment();

private:
    void PixelFormatCheck();
    void VideoDriverCheck();
    void ImageCheck();
    void RenderBackendCheck();
    void HardwareCapsCheck();
    void EventCheck();
    void FontCheck(const std::string& fontPath);
    void AudioCheck(const std::string& audioPath);
    void EnvironmentCheck();

    std::vector<PixelFormat> SupportedPixelFormats;
    std::vector<std::string> SupportedVideoDrivers;
    std::vector<ImageFormat> SupportedImageFormats;
    std::vector<RenderBackend> SupportedRenderBackend;
    std::vector<HardwareCaps> SupportedHardwareCaps;
    std::vector<EventSupport> SupportedEvents;
    std::vector<FontSupport> SupportedFonts;
    std::vector<AudioSupport> SupportedAudio;
    std::vector<EnvInfo> SupportedEnvironmentCheck;
    int v_width;
    int v_height;
};

}

#endif