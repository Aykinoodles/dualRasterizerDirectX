#pragma once
#include "EffectBase.h"

namespace dae
{
	class EffectCombined : public EffectBase
	{
	public:
		EffectCombined(ID3D11Device* pDevice, const std::wstring& assetFile);
		~EffectCombined() override;

		void SetCameraPosition(const Vector3& cameraPosition) {
			m_pCameraPositionVariable->SetFloatVector(reinterpret_cast<const float*>(&cameraPosition));
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

		ID3DX11EffectVectorVariable* m_pCameraPositionVariable{};

		ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pGlossMapVariable{};
	};
}