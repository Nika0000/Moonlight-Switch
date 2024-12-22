#ifdef USE_D3D11_RENDERER

#pragma once

#include "IVideoRenderer.hpp"

#include <SDL2/SDL.h>
#include <d3d11.h>

class D3D11VideoRenderer : public IVideoRenderer {
public:
    D3D11VideoRenderer() {};
    ~D3D11VideoRenderer() {};

    void draw(NVGcontext* vg, int width, int height, AVFrame* frame, int imageFormat) override;

    VideoRenderStats* video_render_stats() override;

private:
    void initialize();

    bool m_is_initialized = false;

    SDL_Window* m_window;

    ID3D11DeviceContext* m_device_context;
    ID3D11RenderTargetView* m_render_target_view;

    VideoRenderStats m_video_render_stats = {};
};

#endif // USE_D3D11_RENDERER