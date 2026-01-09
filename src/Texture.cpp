#include "Texture.h"


dae::Texture::Texture(SDL_Surface* pSurface, ID3D11Device* pDevice):
	m_pSurfacePixels{static_cast<uint32_t*>(pSurface->pixels)},
	m_pSurface{pSurface}
{

    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = m_pSurface->w;
    desc.Height = m_pSurface->h;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = m_pSurface->pixels;
    initData.SysMemPitch = static_cast<UINT>(m_pSurface->pitch);
    initData.SysMemSlicePitch = static_cast<UINT>(m_pSurface->pitch * m_pSurface->h);

    HRESULT hr = pDevice->CreateTexture2D(&desc, &initData, &m_pResource);
    if (FAILED(hr))
    {
        std::cout << "Texture::Texture() failed: " << hr << '\n';
        return;
    }
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
    SRVDesc.Format = format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = desc.MipLevels;
    hr = pDevice->CreateShaderResourceView(m_pResource, &SRVDesc, &m_pShaderResourceView);
    if (FAILED(hr))
    {
        std::cout << "Texture::Texture() failed: " << hr << '\n';
        return;
    }

    // Clean up
    if (m_pSurface)
    {
        SDL_FreeSurface(m_pSurface);
        m_pSurface = nullptr;
    }
}

dae::Texture::~Texture()
{
    if (m_pSurface)
    {
        SDL_FreeSurface(m_pSurface);
        m_pSurface = nullptr;
    }
}

dae::Texture* dae::Texture::LoadFromFile(const std::string& path, ID3D11Device* devicePtr)
{
    SDL_Surface* pSurface = IMG_Load(path.c_str());
    if (!pSurface) return nullptr;
    
    return new dae::Texture(pSurface, devicePtr);
}

dae::ColorRGB dae::Texture::Sample(Vector2 uv)
{
    const float u{ dae::Clamp(uv.x, 0.f, 1.f) * float(m_pSurface->w) };
    const float v{ dae::Clamp(uv.y, 0.f, 1.f) * float(m_pSurface->h) };

    uint8_t r{};
    uint8_t g{};
    uint8_t b{};

    SDL_GetRGB(m_pSurfacePixels[static_cast<int>(u + (v * m_pSurface->w))], m_pSurface->format, &r, &g, &b);

    return  { r / 255.f, g / 255.f, b / 255.f };
}



