#include "EffectCombined.h"

dae::EffectCombined::EffectCombined(ID3D11Device* pDevice, const std::wstring& assetFile)
	:EffectBase(pDevice,assetFile)
{

	m_pCameraPositionVariable = m_pEffect->GetVariableByName("gCameraPosition")->AsVector();
	if (!m_pCameraPositionVariable->IsValid())
		std::wcout << L"m_pCameraPositionVariable not valid!\n";

	m_pNormalMapVariable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
	if (!m_pNormalMapVariable->IsValid())
		std::wcout << L"m_pNormalMapVariable not valid!\n";

	m_pSpecularMapVariable = m_pEffect->GetVariableByName("gSpecularMap")->AsShaderResource();
	if (!m_pSpecularMapVariable->IsValid())
		std::wcout << L"m_pSpecularMapVariable not valid!\n";

	m_pGlossMapVariable = m_pEffect->GetVariableByName("gGlossinessMap")->AsShaderResource();
	if (!m_pGlossMapVariable->IsValid())
		std::wcout << L"m_pGlossMapVariable not valid!\n";
}

dae::EffectCombined::~EffectCombined()
{
	if (m_pCameraPositionVariable) m_pCameraPositionVariable->Release();
	if (m_pNormalMapVariable) m_pNormalMapVariable->Release();
	if (m_pSpecularMapVariable) m_pSpecularMapVariable->Release();
	if (m_pGlossMapVariable) m_pGlossMapVariable->Release();
}
