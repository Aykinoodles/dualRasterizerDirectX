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

#include <string>
#include "Matrix.h"
#include "Texture.h"
namespace dae
{
	class Effect final
	{
	public:
		Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
		~Effect();

		Effect(const Effect& other) = delete;
		Effect& operator=(const Effect& rhs) = delete;
		Effect(Effect&& other) = delete;
		Effect& operator=(Effect&& rhs) = delete;

		ID3DX11Effect* GetEffect() const { return m_pEffect; };
		ID3DX11EffectTechnique* GetTechnique() const { return m_pTechnique; };

		void SetMatrix(const Matrix& projectionMatrix);
		void SetDiffuseTexture(Texture* pDiffuseTexture);

	private:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	};
}