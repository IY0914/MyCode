#pragma once
#include "BaseClass/Object.h"
#include "BaseClass/IState.h"
#include "BaseClass/Collision.h"
#include "../Camera/Camera.h"

class Player
{
private:
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResource;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//�G�t�F�N�g
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//���f��
	DirectX::Model* m_model;
	//�l�p�`�̃��f��
	std::unique_ptr<DirectX::GeometricPrimitive> m_geometricModel;
	//�L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	//�L�[�{�[�h�X�e�[�g
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_stateTracker;
	//�}�E�X
	DirectX::Mouse* m_mouse;
	//�ʒu���W
	DirectX::SimpleMath::Vector3 m_position;
	//�����x
	DirectX::SimpleMath::Vector3 m_velocity;

	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_rot_Y;
	DirectX::SimpleMath::Matrix m_trans;

	//�����c���Ă��邩
	bool m_survive;
	
	//�A�C�e���擾��
	int m_itemNum;

private:
	/*�Œ�l*/

	//�v���C���[�̑��x
	const static float RUN_SPEED;
	//�������
	const static float AVOID_TIME;

public:
	/*------------�Q�b�^�[--------------*/
	//�L�[�{�[�h���擾
	DirectX::Keyboard* GetKeyBoard() { return m_keyboard; }
	//�v���C���[�̐����c�蔻��̎擾
	bool GetSurvive() { return m_survive; }
	//�v���C���[�̈ʒu���擾����
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	//�v���C���[�̑��x��Ԃ�
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }

	//�Z�b�^�[
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

public:
	//�R���X�g���N�^
	Player(DX::DeviceResources* pDR,DirectX::Model* model, DirectX::Keyboard* key, DirectX::Mouse* mouse, DirectX::SimpleMath::Vector3 spownPos);
	//�f�X�g���N�^
	~Player();

	void Initilize();
	void Update(const float& m_stepTimer);
	void MoveUpdate(const float frameTimer);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();
};