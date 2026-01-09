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

	
		void SetCameraPosition(const Vector3& cameraPosition) {
			m_pCameraPositionVariable->SetFloatVector(reinterpret_cast<const float*>(&cameraPosition));
		};
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
		void SetNormalTexture(Texture* pNormalMapVariable) {
			if (pNormalMapVariable)
				m_pNormalMapVariable->SetResource(pNormalMapVariable->GetShaderResourceView());
		};
		void SetGlossinessTexture(Texture* pGlossMapVariable) {
			if (pGlossMapVariable)
				m_pGlossMapVariable->SetResource(pGlossMapVariable->GetShaderResourceView());
		};
		void SetSpecularTexture(Texture* pSpecularMapVariable) {
			if (pSpecularMapVariable)
				m_pSpecularMapVariable->SetResource(pSpecularMapVariable->GetShaderResourceView());
		};
		

	private:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};
		ID3DX11EffectVectorVariable* m_pCameraPositionVariable{};

		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pGlossMapVariable{};

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	};
}