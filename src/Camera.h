#pragma once

// SDL Headers
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Timer.h"
#include "Math.h"

// Jonas christaens helped with this
namespace dae
{
	class Camera final
	{
	public:
		Camera() = default;
		Camera(const Vector3& _origin, float _fovAngle) :
			m_Origin{ _origin },
			m_FovAngle{ _fovAngle }
		{
		}

		void Initialize(float _fovAngle = 90.f, Vector3 _origin = { 0.f,0.f,0.f }, float aspect = 1.f)
		{
			m_FovAngle = _fovAngle;
			m_Fov = tanf((m_FovAngle * TO_RADIANS) / 2.f);
			aspectRatio = aspect;
			m_Origin = _origin;
			CalculateProjectionMatrix();
		}

		void CalculateViewMatrix()
		{
			//right = Vector3::Cross(Vector3::UnitY, forward).Normalized();
			//up = Vector3::Cross(forward, right);
			//
			//m_InvViewMatrix = Matrix{ right,up,forward,m_Origin };
			//m_ViewMatrix = m_InvViewMatrix.Inverse();


			m_ViewMatrix = Matrix::CreateLookAtLH(m_Origin, m_Origin + forward,up);
			
		}

		void CalculateProjectionMatrix()
		{
			Matrix projection{ Matrix::CreatePerspectiveFovLH(m_Fov, aspectRatio, .1f, 100.f) };

			m_ProjectionMatrix = m_WorldMatrix * m_ViewMatrix * projection;
			
		}
		float rotation{};
		bool rotateModel{ true };
		void Update(const Timer* pTimer)
		{

			const float deltaTime = pTimer->GetElapsed();
			const float m_MovementSpeed{ 3.f };
			const float rotationSpeed{ 10.f };

			//Keyboard Input
			const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);


			//Mouse Input
			int mouseX{}, mouseY{};
			const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

			if (pKeyboardState[SDL_SCANCODE_W])
			{
				m_Origin += forward * m_MovementSpeed * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_A])
			{
				m_Origin -= right * m_MovementSpeed * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_S])
			{
				m_Origin -= forward * m_MovementSpeed * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_D])
			{
				m_Origin += right * m_MovementSpeed * deltaTime;
			}


			if (mouseState == 4)
			{
				m_TotalPitch += (mouseY*-1)*rotationSpeed * deltaTime;
				m_TotalYaw += mouseX * rotationSpeed * deltaTime;
			}
			else if (mouseState == 1)
			{
				m_Origin += forward * static_cast<float>(mouseY * -1) * m_MovementSpeed * deltaTime;
				m_TotalYaw += mouseX * rotationSpeed * deltaTime;
			}
			else if (mouseState == 5)
			{
				m_Origin += up * static_cast<float>(mouseY * -1) * m_MovementSpeed * deltaTime;
			}
			if (mouseX || mouseY)
			{
				Matrix rotationMatrix{ Matrix::CreateRotationX(m_TotalPitch * TO_RADIANS) * Matrix::CreateRotationY(m_TotalYaw * TO_RADIANS) };

				forward = rotationMatrix.TransformVector(Vector3::UnitZ);
				forward.Normalize();
			}

			//Update Matrices
			CalculateViewMatrix();
			CalculateProjectionMatrix();
		}

		Vector3 forward{ Vector3::UnitZ };
		Vector3 up{ Vector3::UnitY };
		Vector3 right{ Vector3::UnitX };

		Matrix GetViewMatrix() const { return m_ViewMatrix; };
		Matrix GetProjectionMatrix() const { return m_ProjectionMatrix; };
		Matrix m_WorldMatrix{ Matrix::CreateIdentity() };
	private:

		Vector3 m_Origin{};
		float m_FovAngle{ 90.f };
		float m_Fov{ tanf((m_FovAngle * TO_RADIANS) / 2.f) };
		float aspectRatio{};

		

		float m_TotalPitch{};
		float m_TotalYaw{};

		
		Matrix m_InvViewMatrix{};
		Matrix m_ViewMatrix{};

		Matrix m_ProjectionMatrix{};

		float m_MouseSensitivity{ 5.f };
		float m_MovementSpeed{ 1.f };

		
	};
}
