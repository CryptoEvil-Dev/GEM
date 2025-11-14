#include "include/GReport.hpp"


GTEST::GReport::GReport(int v_width, int v_height){
    this->v_width = v_width;
    this->v_height = v_height;
}

void GTEST::GReport::PixelFormatCheck(){
    struct Candidate{
        Uint32 fmt;
        const char* name;
    } candidates[] = {
        {SDL_PIXELFORMAT_ABGR1555, "SDL_PIXELFORMAT_ABGR1555"},
        {SDL_PIXELFORMAT_UNKNOWN, "SDL_PIXELFORMAT_UNKNOWN"},
        {SDL_PIXELFORMAT_INDEX1LSB, "SDL_PIXELFORMAT_INDEX1LSB"},
        {SDL_PIXELFORMAT_INDEX1MSB, "SDL_PIXELFORMAT_INDEX1MSB"},
        {SDL_PIXELFORMAT_INDEX4LSB, "SDL_PIXELFORMAT_INDEX4LSB"},
        {SDL_PIXELFORMAT_INDEX4MSB, "SDL_PIXELFORMAT_INDEX4MSB"},
        {SDL_PIXELFORMAT_INDEX8, "SDL_PIXELFORMAT_INDEX8"},
        {SDL_PIXELFORMAT_RGB332, "SDL_PIXELFORMAT_RGB332"},
        {SDL_PIXELFORMAT_RGB444, "SDL_PIXELFORMAT_RGB444"},
        {SDL_PIXELFORMAT_RGB555, "SDL_PIXELFORMAT_RGB555"},
        {SDL_PIXELFORMAT_BGR555, "SDL_PIXELFORMAT_BGR555"},
        {SDL_PIXELFORMAT_ARGB4444, "SDL_PIXELFORMAT_ARGB4444"},
        {SDL_PIXELFORMAT_RGBA4444, "SDL_PIXELFORMAT_RGBA4444"},
        {SDL_PIXELFORMAT_ABGR4444, "SDL_PIXELFORMAT_ABGR4444"},
        {SDL_PIXELFORMAT_BGRA4444, "SDL_PIXELFORMAT_BGRA4444"},
        {SDL_PIXELFORMAT_ARGB1555, "SDL_PIXELFORMAT_ARGB1555"},
        {SDL_PIXELFORMAT_RGBA5551, "SDL_PIXELFORMAT_RGBA5551"},
        {SDL_PIXELFORMAT_ABGR1555, "SDL_PIXELFORMAT_ABGR1555"},
        {SDL_PIXELFORMAT_BGRA5551, "SDL_PIXELFORMAT_BGRA5551"},
        {SDL_PIXELFORMAT_RGB565, "SDL_PIXELFORMAT_RGB565"},
        {SDL_PIXELFORMAT_BGR565, "SDL_PIXELFORMAT_BGR565"},
        {SDL_PIXELFORMAT_RGB24, "SDL_PIXELFORMAT_RGB24"},
        {SDL_PIXELFORMAT_BGR24, "SDL_PIXELFORMAT_BGR24"},
        {SDL_PIXELFORMAT_RGB888, "SDL_PIXELFORMAT_RGB888"},
        {SDL_PIXELFORMAT_RGBX8888, "SDL_PIXELFORMAT_RGBX8888"},
        {SDL_PIXELFORMAT_BGR888, "SDL_PIXELFORMAT_BGR888"},
        {SDL_PIXELFORMAT_BGRX8888, "SDL_PIXELFORMAT_BGRX8888"},
        {SDL_PIXELFORMAT_ARGB8888, "SDL_PIXELFORMAT_ARGB8888"},
        {SDL_PIXELFORMAT_RGBA8888, "SDL_PIXELFORMAT_RGBA8888"},
        {SDL_PIXELFORMAT_ABGR8888, "SDL_PIXELFORMAT_ABGR8888"},
        {SDL_PIXELFORMAT_BGRA8888, "SDL_PIXELFORMAT_BGRA8888"},
        {SDL_PIXELFORMAT_ARGB2101010, "SDL_PIXELFORMAT_ARGB2101010"},
        {SDL_PIXELFORMAT_RGBA32, "SDL_PIXELFORMAT_RGBA32"},
        {SDL_PIXELFORMAT_ARGB32, "SDL_PIXELFORMAT_ARGB32"},
        {SDL_PIXELFORMAT_BGRA32, "SDL_PIXELFORMAT_BGRA32"},
        {SDL_PIXELFORMAT_ABGR32, "SDL_PIXELFORMAT_ABGR32"},
        {SDL_PIXELFORMAT_YV12, "SDL_PIXELFORMAT_YV12"},
        {SDL_PIXELFORMAT_IYUV, "SDL_PIXELFORMAT_IYUV"},
        {SDL_PIXELFORMAT_YUY2, "SDL_PIXELFORMAT_YUY2"},
        {SDL_PIXELFORMAT_UYVY, "SDL_PIXELFORMAT_UYVY"},
        {SDL_PIXELFORMAT_YVYU, "SDL_PIXELFORMAT_YVYU"},
        {SDL_PIXELFORMAT_NV12, "SDL_PIXELFORMAT_NV12"},
        {SDL_PIXELFORMAT_NV21, "SDL_PIXELFORMAT_NV21"}
    };

    SDL_Window* tmpWin = SDL_CreateWindow("healthcheck", 0, 0, this->v_width, this->v_height, SDL_WINDOW_HIDDEN);
    SDL_Renderer* tmpRen = SDL_CreateRenderer(tmpWin, -1, 0);

    for(auto &c : candidates){
        SDL_Texture* tex = SDL_CreateTexture(tmpRen, c.fmt, SDL_TEXTUREACCESS_TARGET, this->v_width, this->v_height);
        if(tex){
            SupportedPixelFormats.push_back({c.fmt, c.name});
            SDL_DestroyTexture(tex);
        }
    }

    SDL_DestroyRenderer(tmpRen);
    SDL_DestroyWindow(tmpWin);
}

void GTEST::GReport::VideoDriverCheck(){
    int num = SDL_GetNumVideoDrivers();
    for(int i = 0; i < num; i++){
        const char* name = SDL_GetVideoDriver(i);
        bool ok = (SDL_VideoInit(name) == 0);
        if(ok){
            this->SupportedVideoDrivers.push_back(std::string(name));
            SDL_VideoQuit();
        }
    }
}

void GTEST::GReport::ImageCheck(){
    int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
    int initted = IMG_Init(flags);

    if(initted & IMG_INIT_JPG) SupportedImageFormats.push_back({IMG_INIT_JPG, "IMG_INIT_JPG"});
    if(initted & IMG_INIT_PNG) SupportedImageFormats.push_back({IMG_INIT_PNG, "IMG_INIT_PNG"});
    if(initted & IMG_INIT_TIF) SupportedImageFormats.push_back({IMG_INIT_TIF, "IMG_INIT_TIF"});
    if(initted & IMG_INIT_WEBP) SupportedImageFormats.push_back({IMG_INIT_WEBP, "IMG_INIT_WEBP"});

    IMG_Quit();
}

void GTEST::GReport::RenderBackendCheck(){
    int num = SDL_GetNumRenderDrivers();
    for(int i = 0; i < num; ++i){
        SDL_RendererInfo info;
        if(SDL_GetRenderDriverInfo(i, &info) == 0){
            RenderBackend rb;
            rb.name = info.name ? info.name : "Unknown";
            rb.accelerated = (info.flags & SDL_RENDERER_ACCELERATED);
            rb.targetTexture = (info.flags & SDL_RENDERER_TARGETTEXTURE);
            rb.maxTextureWidth = info.max_texture_width;
            rb.maxTextureHeight = info.max_texture_height;
            SupportedRenderBackend.push_back(rb);
        }
    }
}

void GTEST::GReport::HardwareCapsCheck(){
    SDL_Window* tmpWin = SDL_CreateWindow("healthcheck", 0, 0, this->v_width, this->v_height, SDL_WINDOW_HIDDEN);
    SDL_Renderer* tmpRen = SDL_CreateRenderer(tmpWin, -1, 0);

    HardwareCaps caps{};

    SDL_RendererInfo info{};
    if(SDL_GetRendererInfo(tmpRen, &info) == 0){
        caps.maxTextureWidth = info.max_texture_width;
        caps.maxTextureHeight = info.max_texture_height;
        caps.accelerated = (info.flags & SDL_RENDERER_ACCELERATED) != 0;
        caps.vsync = (info.flags & SDL_RENDERER_PRESENTVSYNC) != 0;
        caps.targetTexture = (info.flags & SDL_RENDERER_TARGETTEXTURE) != 0;
    }

    SDL_BlendMode modes[] = {
        SDL_BLENDMODE_NONE,
        SDL_BLENDMODE_BLEND,
        SDL_BLENDMODE_ADD,
        SDL_BLENDMODE_MOD
    };
    const char* names[] = {"NONE", "BLEND", "ADD", "MOD"};

    for(int i = 0; i < 4; ++i){
        if(SDL_SetRenderDrawBlendMode(tmpRen, modes[i]) == 0){
            caps.supportedBlendModes.push_back(names[i]);
        }
    }

    SupportedHardwareCaps.push_back(caps);
    SDL_DestroyRenderer(tmpRen);
    SDL_DestroyWindow(tmpWin);
}

void GTEST::GReport::EventCheck(){
    SDL_Window* tmpWin = SDL_CreateWindow("healthcheck", 0, 0, this->v_width, this->v_height, SDL_WINDOW_HIDDEN);
    SDL_Renderer* tmpRen = SDL_CreateRenderer(tmpWin, -1, 0);

    EventSupport es{false, false, false};

    SDL_Event e;
    for(int i = 0; i < 10; ++i){
        SDL_PumpEvents();
        while(SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) es.keyboard = true;
            if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) es.mouse = true;
            if (e.type == SDL_QUIT) es.quit == true;
        }
    }
    SupportedEvents.push_back(es);


    SDL_DestroyRenderer(tmpRen);
    SDL_DestroyWindow(tmpWin);
}

void GTEST::GReport::FontCheck(const std::string& fontPath){
    FontSupport fs{false, false};

    if(TTF_Init() == 0){
        fs.ttfInit = true;
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 16);
        if(font){
            fs.fontLoad = true;
            TTF_CloseFont(font);
        }
        TTF_Quit();
    }

    SupportedFonts.push_back(fs);
}

void GTEST::GReport::AudioCheck(const std::string& audioPath){
    AudioSupport as{false, false};

    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG != 0)){
        as.mixerInit = true;
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0){
            as.audioOpen = true;
            Mix_CloseAudio();
        }
        Mix_Quit();
    }

    SupportedAudio.push_back(as);
}

void GTEST::GReport::EnvironmentCheck(){
    EnvInfo env{};

    const char* sess = getenv("XDG_SESSION_TYPE");
    env.sessionType = sess ? sess : "Unknown";

    const char* disp = getenv("DISPLAY");
    env.display = disp ? disp : "None";

    const char* way = getenv("WAYLAND_DISPLAY");
    env.wayland = way ? way : "None";

    // Проверка доступа к /dev/dri/card0
    if(access("/dev/dri/card0", R_OK | W_OK) == 0)
        env.kmsAccess = true;
    else
        env.kmsAccess = false;
    
        SupportedEnvironmentCheck.push_back(env);
}

void GTEST::GReport::RunAll(){
    PixelFormatCheck();
    VideoDriverCheck();
    ImageCheck();
    RenderBackendCheck();
    HardwareCapsCheck();
    EventCheck();
    FontCheck("./assets/font.ttf");
    AudioCheck("./assets/test.mp3");
    EnvironmentCheck();
}

void GTEST::GReport::Print() const{
    std::cout << "=== GEM Healthcheck Report ===" << std::endl;
    std::cout << "\nPixel Formats:" << std::endl;
    for (const auto& pf : SupportedPixelFormats) {
        std::cout << "  " << pf.name << std::endl;
    }
    std::cout << "\nVideo Drivers:" << std::endl;
    for(const auto& d : SupportedVideoDrivers){
        std::cout << "  " << d;
    }
    std::cout << std::endl;
    std::cout << "\nRender Backends:" << std::endl;
    for(const auto& r : SupportedRenderBackend){
        std::cout << "  " << r.name
                  << " | Accelerated: " << (r.accelerated ? "Yes" : "No")
                  << " | TargetTexture: " << (r.targetTexture ? "Yes" : "No")
                  << " | MaxTex: " << r.maxTextureWidth << "x" << r.maxTextureHeight
                  << std::endl;
    }

    std::cout << "\nHardware Caps:" << std::endl;
    for (const auto& h : SupportedHardwareCaps) {
        std::cout << "  MaxTex: " << h.maxTextureWidth << "x" << h.maxTextureHeight
                  << " | Accelerated: " << (h.accelerated ? "Yes" : "No")
                  << " | VSync: " << (h.vsync ? "Yes" : "No")
                  << " | TargetTexture: " << (h.targetTexture ? "Yes" : "No")
                  << " | BlendModes: ";
        for (auto& bm : h.supportedBlendModes) std::cout << bm << " ";
        std::cout << std::endl;
    }

    std::cout << "\nEvents:" << std::endl;
    for (const auto& e : SupportedEvents) {
        std::cout << "  Keyboard: " << (e.keyboard ? "OK" : "FAIL")
                  << " | Mouse: " << (e.mouse ? "OK" : "FAIL")
                  << " | Quit: " << (e.quit ? "OK" : "FAIL") << std::endl;
    }

    std::cout << "\nImage Formats:" << std::endl;
    for (const auto& f : SupportedImageFormats) {
        std::cout << "  " << f.name << std::endl;
    }

    std::cout << "\nFonts:" << std::endl;
    for (const auto& f : SupportedFonts) {
        std::cout << "  TTF Init: " << (f.ttfInit ? "OK" : "FAIL")
                  << " | Font Load: " << (f.fontLoad ? "OK" : "FAIL") << std::endl;
    }

    std::cout << "\nAudio:" << std::endl;
    for (const auto& a : SupportedAudio) {
        std::cout << "  Mixer Init: " << (a.mixerInit ? "OK" : "FAIL")
                  << " | Audio Open: " << (a.audioOpen ? "OK" : "FAIL") << std::endl;
    }

    std::cout << "\nEnvironment:" << std::endl;
    for (const auto& env : SupportedEnvironmentCheck) {
        std::cout << "  Session: " << env.sessionType
                  << " | Display: " << env.display
                  << " | Wayland: " << env.wayland
                  << " | KMS Access: " << (env.kmsAccess ? "Yes" : "No") << std::endl;
    }
}

void GTEST::GReport::Save(const std::string& path) const{
    std::ofstream out(path);
    if (!out) return;

    out << "=== GEM Healthcheck Report ===\n";
    out << "\nPixel Formats:\n";
    for (const auto& pf : SupportedPixelFormats) {
        out << "  " << pf.name << "\n";
    }

    out << "\nVideo Drivers:\n";
    for (const auto& d : SupportedVideoDrivers) {
        out << "  " << d << "\n";
    }

    out << "\nRender Backends:\n";
    for (const auto& r : SupportedRenderBackend) {
        out << "  " << r.name
            << " | Accelerated: " << (r.accelerated ? "Yes" : "No")
            << " | TargetTexture: " << (r.targetTexture ? "Yes" : "No")
            << " | MaxTex: " << r.maxTextureWidth << "x" << r.maxTextureHeight
            << "\n";
    }

    out << "\nHardware Caps:\n";
    for (const auto& h : SupportedHardwareCaps) {
        out << "  MaxTex: " << h.maxTextureWidth << "x" << h.maxTextureHeight
            << " | Accelerated: " << (h.accelerated ? "Yes" : "No")
            << " | VSync: " << (h.vsync ? "Yes" : "No")
            << " | TargetTexture: " << (h.targetTexture ? "Yes" : "No")
            << " | BlendModes: ";
        for (auto& bm : h.supportedBlendModes) out << bm << " ";
        out << "\n";
    }

    out << "\nEvents:\n";
    for (const auto& e : SupportedEvents) {
        out << "  Keyboard: " << (e.keyboard ? "OK" : "FAIL")
            << " | Mouse: " << (e.mouse ? "OK" : "FAIL")
            << " | Quit: " << (e.quit ? "OK" : "FAIL") << "\n";
    }

    out << "\nImage Formats:\n";
    for (const auto& f : SupportedImageFormats) {
        out << "  " << f.name << "\n";
    }

    out << "\nFonts:\n";
    for (const auto& f : SupportedFonts) {
        out << "  TTF Init: " << (f.ttfInit ? "OK" : "FAIL")
            << " | Font Load: " << (f.fontLoad ? "OK" : "FAIL") << "\n";
    }

    out << "\nAudio:\n";
    for (const auto& a : SupportedAudio) {
        out << "  Mixer Init: " << (a.mixerInit ? "OK" : "FAIL")
            << " | Audio Open: " << (a.audioOpen ? "OK" : "FAIL") << "\n";
    }

    out << "\nEnvironment:\n";
    for (const auto& env : SupportedEnvironmentCheck) {
        out << "  Session: " << env.sessionType
            << " | Display: " << env.display
            << " | Wayland: " << env.wayland
            << " | KMS Access: " << (env.kmsAccess ? "Yes" : "No") << "\n";
    }
}


std::vector<GTEST::PixelFormat> GTEST::GReport::Get_PixelFormat(){
    if(this->SupportedPixelFormats.empty())
        this->PixelFormatCheck();
    return this->SupportedPixelFormats;
}

std::vector<std::string> GTEST::GReport::Get_VideoDrivers(){
    if(this->SupportedVideoDrivers.empty())
        this->VideoDriverCheck();
    return this->SupportedVideoDrivers;
}

std::vector<GTEST::ImageFormat> GTEST::GReport::Get_ImageFormat(){
    if(this->SupportedImageFormats.empty())
        this->ImageCheck();
    return this->SupportedImageFormats;
}

std::vector<GTEST::RenderBackend> GTEST::GReport::Get_RenderBackend(){
    if(this->SupportedRenderBackend.empty())
        this->RenderBackendCheck();
    return this->SupportedRenderBackend;
}

std::vector<GTEST::HardwareCaps> GTEST::GReport::Get_HardwareCaps(){
    if(this->SupportedHardwareCaps.empty())
        this->HardwareCapsCheck();
    return this->SupportedHardwareCaps;
}

std::vector<GTEST::EventSupport> GTEST::GReport::Get_EventSupport(){
    if(this->SupportedEvents.empty())
        this->EventCheck();
    return this->SupportedEvents;
}

std::vector<GTEST::FontSupport> GTEST::GReport::Get_FontsSupport(){
    if(this->SupportedFonts.empty())
        this->FontCheck("./assets/font.ttf");
    return this->SupportedFonts;
}

std::vector<GTEST::AudioSupport> GTEST::GReport::Get_AudioSupport(){
    if(this->SupportedAudio.empty())
        this->AudioCheck("./assets/test.mp3");
    return this->SupportedAudio;
}

std::vector<GTEST::EnvInfo> GTEST::GReport::Get_Environment(){
    if(this->SupportedEnvironmentCheck.empty())
        this->EnvironmentCheck();
    return this->SupportedEnvironmentCheck;
}