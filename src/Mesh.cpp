#include "Mesh.h"

dae::Mesh::Mesh(ID3D11Device* pDevice, std::vector<Vertex> vertices, std::vector<uint32_t> indices , bool isShadedFlat):
	m_Indices{indices},
	m_Vertices{vertices},
	m_IsShadedFlat{isShadedFlat}
{
	if(m_IsShadedFlat)
	{
		m_pEffectFlat = new EffectFlat{ pDevice, L"./resources/Fire.fx" };
		m_pTechnique = m_pEffectFlat->GetTechnique();
	}
	else
	{
		m_pEffectCombined = new EffectCombined{ pDevice, L"./resources/PosCol3D.fx" };
		m_pTechnique = m_pEffectCombined->GetTechnique();
	}

	// Create Vertex Layout
	static constexpr uint32_t numElements{ 5 };
	D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

	;

	vertexDesc[0].SemanticName = "POSITION";
	vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[0].AlignedByteOffset = 0;
	vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[1].SemanticName = "COLOR";
	vertexDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[1].AlignedByteOffset = 12;
	vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[2].SemanticName = "TEXCOORD";
	vertexDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDesc[2].AlignedByteOffset = 24;
	vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[3].SemanticName = "NORMAL";
	vertexDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[3].AlignedByteOffset = 32;
	vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[4].SemanticName = "TANGENT";
	vertexDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[4].AlignedByteOffset = 44;
	vertexDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;



	// input layout

	D3DX11_PASS_DESC passDesc{};
	m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT result{ pDevice->CreateInputLayout
		(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pVertexLayout
		) };
	if (FAILED(result)) return;

	// vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(result)) return;

	// index buffer
	m_NumIndices = static_cast<uint32_t>(indices.size());
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(result)) return;

}

dae::Mesh::~Mesh()
{
	delete m_pEffectCombined;
	delete m_pEffectFlat;
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
}

void dae::Mesh::Render(ID3D11DeviceContext* pDeviceContext, const Matrix& worldViewProjectionMatrix, const Matrix& worldMatrix, const Vector3& cameraPosition,
	Texture* pDiffuseTexture, Texture* pNormalMapTexture, Texture* pSpecularMapTexture, Texture* pGlossinessTexture) const
{
	if (m_IsShadedFlat)
	{
		m_pEffectFlat->SetProjectionMatrix(worldViewProjectionMatrix);
		m_pEffectFlat->SetWorldMatrix(worldMatrix);
				 
		m_pEffectFlat->SetDiffuseTexture(pDiffuseTexture);
	}
	else
	{
		m_pEffectCombined->SetProjectionMatrix(worldViewProjectionMatrix);
		m_pEffectCombined->SetWorldMatrix(worldMatrix);
		m_pEffectCombined->SetCameraPosition(cameraPosition);

		m_pEffectCombined->SetDiffuseTexture(pDiffuseTexture);
		m_pEffectCombined->SetNormalTexture(pNormalMapTexture);
		m_pEffectCombined->SetSpecularTexture(pSpecularMapTexture);
		m_pEffectCombined->SetGlossinessTexture(pGlossinessTexture);
	}
	

	// Set primitive topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set input layout
	pDeviceContext->IASetInputLayout(m_pVertexLayout);

	// Set vertex buffer
	constexpr UINT stride{ sizeof(Vertex) };
	constexpr UINT offset{};
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	m_pTechnique->GetDesc(&techniqueDesc);
	if(m_PassIndex < techniqueDesc.Passes)
	{
		m_pTechnique->GetPassByIndex(m_PassIndex)->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
	
}
