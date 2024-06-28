#pragma once

//�J�����̃V���O���g����
class Camera
{
public:
	static Camera* const GetInstance();


private:
	// �R���X�g���N�^
	Camera();

	// ����͋��e���Ȃ�
	void operator=(const Camera& object) = delete;
	// �R�s�[�R���X�g���N�^�͋��e���Ȃ�
	Camera(const Camera& object) = delete;

private:
	// Camera�N���X�̃C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<Camera> m_camera;
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	//�J�����̐ݒ�
	DirectX::SimpleMath::Matrix m_view, m_proj;
	//�J�����֌W
	DirectX::SimpleMath::Vector3 m_eye,m_target,m_up;
	//�}�E�X�g���b�J�[
	DirectX::Mouse::ButtonStateTracker m_mouseTracker;
	//�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_stateTracker;
	//�}�E�X�ƃL�[�{�[�h
	DirectX::Keyboard* m_keybBoard;
	DirectX::Mouse* m_mosue;
	DirectX::Mouse::Mode m_mouseMode;
	//����p
	float m_fov;
	//�c����
	float m_aspectRatio;
	//�ő�`�拗��
	float m_farPlane;
	//�Œ�`�拗��
	float m_nearPlane;

private:
	//�}�E�X�̍��W�ʒu
	DirectX::SimpleMath::Vector3 m_mousePos;
	//�}�E�X�̉�]
	DirectX::SimpleMath::Vector2 m_mouseRot;
	//�}�E�X��]�ۑ��p
	DirectX::SimpleMath::Vector2 m_saveAngle;
	//��ʂɑ΂��Ă̔䗦
	DirectX::SimpleMath::Vector2 m_disRatio;

	//������̉�]
	DirectX::SimpleMath::Matrix m_MathRotate;

public:

	void CreateCameraSetting(DirectX::Keyboard* key, DirectX::Mouse* mouse);
	void PreUpdate();
	void Update();
	void LimitMouseCursol(float mousePosX,float mousePosy);

public:
	//�Z�b�^�[
	void SetTargetPosition(const DirectX::SimpleMath::Vector3& target) { m_target = target; }
	//�f�o�C�X���\�[�X�̐ݒ�
	void SetDeviceResource(DX::DeviceResources* pDR) { m_deviceResources = pDR; }

	//��]�ʂ�Ԃ��֐�
	DirectX::SimpleMath::Matrix GetRotateAngle() { return m_MathRotate; }

	//��]�ʂ������ŕԂ�
	float GetMouseRotate() { return m_mouseRot.y; }

	//�Q�b�^�[
	DirectX::SimpleMath::Vector3 GetEyePosition() { return m_eye; }
	DirectX::SimpleMath::Vector3 GetTargetPositon() { return m_target; }
	DirectX::SimpleMath::Vector3 GetUpRotate() { return m_up; }
	DirectX::SimpleMath::Matrix GetView() { return m_view; }
	DirectX::SimpleMath::Matrix GetProjection() { return m_proj; }
	DirectX::SimpleMath::Matrix GetRotate() { return m_MathRotate; }
};
