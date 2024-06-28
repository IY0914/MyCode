#include "pch.h"
#include "Camera.h"
#include <winuser.h>	//�}�E�X�J�[�\��

using namespace DirectX;

std::unique_ptr<Camera> Camera::m_camera = nullptr;

Camera* const Camera::GetInstance()
{
	if (m_camera == nullptr)
	{
		// �J�����N���X�̃C���X�^���X�𐶐�����
		m_camera.reset(new Camera());
	}
	// �J�����N���X�̃C���X�^���X��Ԃ�
	return m_camera.get();
}

Camera::Camera():
	m_deviceResources(nullptr),
	m_fov(DirectX::XMConvertToRadians(45.0f)),
	m_aspectRatio(0.0f),
	m_nearPlane(0.01f),
	m_farPlane(1000.0f),
	m_eye(0.0f,10.0f, 100.0f),
	m_target(0.1f,0.1f,0.1f),
	m_up(DirectX::SimpleMath::Vector3::UnitY),
	m_mousePos(0.0f,0.0f,0.0f),
	m_disRatio(0.0f,0.0f),
	m_saveAngle(0.0f,0.0f),
	m_keybBoard(nullptr),
	m_mosue(nullptr),
	m_mouseMode(Mouse::Mode::MODE_ABSOLUTE)
{

}

void Camera::CreateCameraSetting(DirectX::Keyboard* keyboard,DirectX::Mouse* mouse)
{
	//�|�C���^�[
	m_mosue = mouse;
	m_keybBoard = keyboard;

	//��ʔ䗦
	m_aspectRatio =
		static_cast<float>(m_deviceResources->GetOutputSize().right /
			m_deviceResources->GetOutputSize().bottom);
	//��ʐݒ�
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		m_fov,
		m_aspectRatio,
		m_nearPlane,
		m_farPlane
	);
	//�`��s��
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(
		m_eye,
		m_target,
		m_up
	);
	//�}�E�X�ʒu�p��ʔ䗦
	m_disRatio.x = 1.0f / float(m_deviceResources->GetOutputSize().right);
	m_disRatio.y = 1.0f / float(m_deviceResources->GetOutputSize().bottom);
	//�}�E�X�̏����擾
	auto mouseVel = mouse->Get().GetState();
	//�}�E�X�̈ʒu��ۑ�
	m_mousePos.x = mouseVel.x;
	m_mousePos.y = mouseVel.y;

	m_eye = SimpleMath::Vector3(m_target.x, m_target.y + 1.0f, m_target.z - 30.0f);
	m_target = SimpleMath::Vector3(m_target.x, m_target.y + 3.0f, m_target.z);
	m_up = SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
}

void Camera::Update()
{
	//�}�E�X�̏����擾
	auto mouseVel = m_mosue->Get().GetState();
	//�L�[�{�[�h�̏����擾
	auto key = m_keybBoard->Get().GetState();
	//�g���b�J�[�̃X�e�[�g�̕ύX
	m_mouseTracker.Update(mouseVel);
	//�L�[�{�[�h�̃X�e�[�g�ύX
	m_stateTracker.Update(key);

	//�}�E�X�J�[�\������ʊO�ɏo�Ȃ��悤��
	LimitMouseCursol(mouseVel.x, mouseVel.y);

	if (key.R)
	{
		//�}�E�X�̈ʒu��ۑ�
		m_mousePos.x = mouseVel.x;
		m_mousePos.y = mouseVel.y;
		//�}�E�X����̉�]��ێ�
		m_saveAngle.x = m_mouseRot.x;
		m_saveAngle.y = m_mouseRot.y;
	}

	//�䗦����̃}�E�X�ʒu(�O�̍��W - ���ݍ��W) * ��ʔ䗦
	float dx = (m_mousePos.x - mouseVel.x);
	float dy = (m_mousePos.y - mouseVel.y);

	//�}�E�X�ʒu����̉�]
	float yAngle = dx * DirectX::XM_PI * m_disRatio.x;
	float xAngle = dy * DirectX::XM_PI * m_disRatio.y;

	////�c
	//m_mouseRot.x = m_saveAngle.x + xAngle;
	////��
	//m_mouseRot.y = m_saveAngle.y + yAngle;

	//�}�E�X�̉�]���s��ɕϊ�
	DirectX::SimpleMath::Matrix m_xRot = SimpleMath::Matrix::CreateRotationX(xAngle);
	DirectX::SimpleMath::Matrix m_yRot = SimpleMath::Matrix::CreateRotationY(yAngle);
	//��]�̍���
	DirectX::SimpleMath::Matrix m_rot = m_yRot * m_xRot;
	//�Ԃ��p�̕ϐ�
	m_MathRotate = m_rot.Invert();

	//SimpleMath::Vector3 eye(0.0f, 1.0f, -10.0f/*m_target.x, m_target.y + 7.0f, m_target.z - 10.0f*/);
	//SimpleMath::Vector3 target = SimpleMath::Vector3(m_target.x, m_target.y + 3.0f, m_target.z);
	//SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);

	//�J�����̎��_�ݒ�p�̃��[�J���ϐ�
	SimpleMath::Vector3 eye(m_target.x, m_target.y + 20.0f, m_target.z - 10.0f);
	SimpleMath::Vector3 target = SimpleMath::Vector3(m_target.x, m_target.y, m_target.z);
	SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);
	
	//SimpleMath::Vector3 eye = SimpleMath::Vector3::Transform(m_target, m_rot);//(m_target.x, m_target.y + 1.0f, m_target.z);
	//SimpleMath::Vector3 target = SimpleMath::Vector3(m_target.x + 1.0f, m_target.y, m_target.z);
	//SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);

	//�J�����̈ʒu�ɉ�]��K�p
	//eye = 
	//��]�����̊��ύX
	//up = SimpleMath::Vector3::Transform(up, m_rot);

	//��]���݂̃r���[�s��
	m_eye = eye;
	m_up = up;
	m_target = target;

	//�J�����̕ύX��K�p
	m_view = SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

void Camera::LimitMouseCursol(float mousePosX, float mousePosY)
{
	mousePosX;
	mousePosY;
}
