#include "Effect.h"
#include <iostream>
#include <sstream>


dae::Effect::Effect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	m_pEffect = LoadEffect(pDevice, assetFile);

	m_pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
	if (!m_pTechnique->IsValid()) 
		std::wcout << L"Technique not valid\n";

	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pMatWorldViewProjVariable->IsValid())
		std::wcout << L"m_MatWorldViewProjVariablePtr not valid!\n";

	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	if (!m_pDiffuseMapVariable->IsValid())
		std::wcout << L"m_DiffuseMapVariablePtr not valid!\n";

}

dae::Effect::~Effect()
{
	if (m_pTechnique) m_pTechnique->Release();
	if (m_pEffect) m_pEffect->Release();
}

void dae::Effect::SetMatrix(const Matrix& projectionMatrix)
{
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<const float*>(&projectionMatrix));
}

void dae::Effect::SetDiffuseTexture(Texture* pDiffuseTexture)
{
	if (pDiffuseTexture)
		m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetShaderResourceView());
}

ID3DX11Effect* dae::Effect::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	HRESULT result = S_OK;
	ID3D10Blob* pErrorBlob = nullptr;
	ID3DX11Effect* pEffect;

	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	result = D3DX11CompileEffectFromFile(assetFile.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&pEffect,
		&pErrorBlob);

	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
		{
			char* pErrors = (char*)pErrorBlob->GetBufferPointer();

			std::wstringstream ss;
			for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
				ss << pErrors[i];

			OutputDebugStringW(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::wcout << ss.str() << '\n';
		}
		else
		{
			std::wstringstream ss;
			ss << "EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFile;
			std::wcout << ss.str() << '\n';
			return nullptr;
		}
	}
	return pEffect;
}

