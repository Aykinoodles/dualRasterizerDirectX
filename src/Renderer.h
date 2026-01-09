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

// Framework Headers
#include "Timer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Utils.h"

namespace dae
{
	enum class FilteringMethod
	{
		Point,
		Linear,
		Anisotropic
	};
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(const Timer* pTimer);
		void Render() const;

		void ChangeFilteringMethod();
		void ToggleRotation() { m_IsRotating = !m_IsRotating; };

	private:
		SDL_Window* m_pWindow{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };
		bool m_IsRotating{ true };
		static const int FILTERMETHODCOUNT{3};
		//Mesh Info

		Mesh* m_pMeshVehicle{};
		Mesh* m_pMeshFire{};

		Texture* m_pVehicleDiffuseTexture{};
		Texture* m_pFireDiffuseTexture{};
		Texture* m_pNormalMapTexture{};
		Texture* m_pGlossinessMapTexture{};
		Texture* m_pSpecularMapTexture{};
		Camera* m_Camera{};

		FilteringMethod m_ActiveFilterMethod{};
		//DIRECTX
		HRESULT InitializeDirectX();
		ID3D11Device* m_pDevice{};
		ID3D11DeviceContext* m_pDeviceContext{};
		IDXGISwapChain* m_pSwapChain{};
		ID3D11Texture2D* m_pDepthStencilBuffer{};
		ID3D11DepthStencilView* m_pDepthStencilView{};
		ID3D11Resource* m_pRenderTargetBuffer{};
		ID3D11RenderTargetView* m_pRenderTargetView{};
		//...
	};
}
