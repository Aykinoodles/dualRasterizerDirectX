#pragma once

// SDL Headers
#include "SDL.h"
#include "SDL_syswm.h"
#include "SDL_surface.h"
#include "SDL_image.h"

// DirectX Headers
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include "ColorRGB.h"
#include "Math.h"
#include <iostream>
#include <string.h>

namespace dae
{
	class Texture final
	{
	public:
		Texture(SDL_Surface* pSurface, ID3D11Device* pDevice);
		~Texture();

		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& rhs) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(Texture&& rhs) = delete;
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pShaderResourceView; };

		static Texture* LoadFromFile(const std::string& path, ID3D11Device* devicePtr);

		ColorRGB Sample(Vector2 uv);


	private:
		uint32_t* m_pSurfacePixels{};
		SDL_Surface* m_pSurface{};

		ID3D11ShaderResourceView* m_pShaderResourceView{ nullptr };
		ID3D11Texture2D* m_pResource{ nullptr };
	};
}