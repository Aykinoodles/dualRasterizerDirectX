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
	class EffectBase
	{
	public:
		EffectBase(ID3D11Device* pDevice, const std::wstring& assetFile);
		virtual ~EffectBase();

		EffectBase(const EffectBase& other) = delete;
		EffectBase(EffectBase&& other) noexcept = delete;
		EffectBase& operator=(const EffectBase& other) = delete;
		EffectBase& operator=(EffectBase&& other) noexcept = delete;

		ID3DX11Effect* GetEffect() const { return m_pEffect; };
		ID3DX11EffectTechnique* GetTechnique() const { return m_pTechnique; };

	
		
		void SetWorldMatrix(const Matrix& worldMatrix) {
			m_pMatWorldVariable->SetMatrix(reinterpret_cast<const float*>(&worldMatrix));
		};
		void SetProjectionMatrix(const Matrix& projectionMatrix) {
			m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<const float*>(&projectionMatrix));
		};
		void SetDiffuseTexture(Texture* pDiffuseTexture) {
			if (pDiffuseTexture)
				m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetShaderResourceView());
		};
		
		

	protected:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};

		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};
		
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};
		

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	};
}