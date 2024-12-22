#include "SwitchMoonlightSessionDecoderAndRenderProvider.hpp"
#include "FFmpegVideoDecoder.hpp"
#include "SDLAudiorenderer.hpp"
#include "Settings.hpp"

#ifdef __SWITCH__
#include "AudrenAudioRenderer.hpp"
#endif

#ifdef BOREALIS_USE_DEKO3D
#include "DKVideoRenderer.hpp"
#elif defined(USE_METAL_RENDERER)
#include "MetalVideoRenderer.hpp"
#elif defined(USE_GL_RENDERER)
#include "GLVideoRenderer.hpp"
#elif defined(USE_D3D11_RENDERER)
#include "D3D11VideoRenderer.hpp"
#else
#error No renderer selected, enable USE_GL_RENDERER or USE_METAL_RENDERER or USE_D3D11_RENDERER
#endif

IFFmpegVideoDecoder*
SwitchMoonlightSessionDecoderAndRenderProvider::video_decoder() {
    return new FFmpegVideoDecoder();
}

IVideoRenderer*
SwitchMoonlightSessionDecoderAndRenderProvider::video_renderer() {
#ifdef BOREALIS_USE_DEKO3D
    return new DKVideoRenderer();
#elif defined(USE_METAL_RENDERER)
    return new MetalVideoRenderer();
#elif defined(USE_GL_RENDERER)
    return new GLVideoRenderer();
#elif defined(USE_D3D11_RENDERER)
    return new D3D11VideoRenderer();
#endif
}

IAudioRenderer*
SwitchMoonlightSessionDecoderAndRenderProvider::audio_renderer() {
#ifdef __SWITCH__
    if (Settings::instance().audio_backend() == SDL) {
        return new SDLAudioRenderer();
    } else {
        return new AudrenAudioRenderer();
    }
#else
    return new SDLAudioRenderer();
#endif
}