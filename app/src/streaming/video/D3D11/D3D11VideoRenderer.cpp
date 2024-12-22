#ifdef USE_D3D11_RENDERER

#include "D3D11VideoRenderer.hpp"

#include "borealis.hpp"

#include <borealis/platforms/driver/d3d11.hpp>
#include <borealis/platforms/sdl/sdl_video.hpp>

extern std::unique_ptr<brls::D3D11Context> D3D11_CONTEXT;

void D3D11VideoRenderer::initialize() {
    if (!m_is_initialized) {
        m_is_initialized = true;
        HRESULT hr;

        auto videoContext = (brls::SDLVideoContext*) brls::Application::getPlatform()->getVideoContext();
        m_window = videoContext->getSDLWindow();

        D3D11_CONTEXT->getDevice()->GetImmediateContext(&m_device_context);

        // Create render target view
        {
            ID3D11Resource* backBufferResource;
            hr = D3D11_CONTEXT->getSwapChain()->GetBuffer(0, __uuidof(ID3D11Resource),  (void**)&backBufferResource);

            if(FAILED(hr)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "IDXGISwapChain::GetBuffer() failed: %x",
                    hr);
            }

            hr = D3D11_CONTEXT->getDevice()->CreateRenderTargetView(backBufferResource, nullptr, &m_render_target_view);
            if(FAILED(hr)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "ID3D11Device::CreateRenderTargetView() failed: %x",
                    hr);
            }
        }
    }
}
void D3D11VideoRenderer::draw(NVGcontext* vg, int width, int height, AVFrame* frame, int imageFormat) {
    initialize();

    if (!m_video_render_stats.rendered_frames) {
        m_video_render_stats.measurement_start_timestamp = LiGetMillis();
    }

    uint64_t before_render = LiGetMillis();

    const float clearColor[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    m_device_context->ClearRenderTargetView(m_render_target_view, clearColor);
    m_device_context->OMSetRenderTargets(1, &m_render_target_view, nullptr);

    m_video_render_stats.total_render_time += LiGetMillis() - before_render;
    m_video_render_stats.rendered_frames++;
}

VideoRenderStats* D3D11VideoRenderer::video_render_stats() {
    m_video_render_stats.rendered_fps =
        (float)m_video_render_stats.rendered_frames /
        ((float)(LiGetMillis() -
                 m_video_render_stats.measurement_start_timestamp) /
         1000);
    return (VideoRenderStats*)&m_video_render_stats;
}

#endif // USE_D3D11_RENDERER