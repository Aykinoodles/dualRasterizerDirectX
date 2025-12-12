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
namespace dae
{
	struct Vertex
	{
		Vector3 position;
		ColorRGB color;
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
	
		void Render(ID3D11DeviceContext* pDeviceContext) const;

	private:
		ID3D11Buffer* m_pVertexBuffer{};
		Effect* m_pEffect;
		ID3D11InputLayout* m_pVertexLayout{};
		uint32_t m_NumIndices{};
		ID3D11Buffer* m_pIndexBuffer{};
	};
}