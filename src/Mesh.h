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

#include <Vector>
#include "Math.h"
#include "Effect.h"
#include "Texture.h"

namespace dae
{
	struct Vertex
	{
		Vector3 position;
		ColorRGB color;
		Vector2 uv;
	};
	
	class Mesh final
	{
	public:
		Mesh(ID3D11Device* pDevice, std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		~Mesh();

		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& rhs) = delete;
		Mesh(Mesh&& other) = delete;
		Mesh& operator=(Mesh&& rhs) = delete;
	
		void Render(ID3D11DeviceContext* pDeviceContext, const Matrix& worldViewProjectionMatrix,Texture* pDiffuseTexture) const;
		void ChangeFilteringMethod(UINT passIndex) { m_PassIndex = passIndex; };

	private:
		
		ID3DX11EffectTechnique* m_pTechnique{};
		Effect* m_pEffect{};

		ID3D11InputLayout* m_pVertexLayout{};
		ID3D11Buffer* m_pVertexBuffer{};
		ID3D11Buffer* m_pIndexBuffer{};

		std::vector<Vertex>		m_Vertices{};
		std::vector<uint32_t>	m_Indices{};

		uint32_t m_NumIndices{};
		UINT m_PassIndex{};
	};
}