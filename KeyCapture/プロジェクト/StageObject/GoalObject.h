#pragma once
#include "DeviceResources.h"
#include "BaseClass/Collision.h"

class GoalObject
{
private:
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	//���f��
	std::unique_ptr<DirectX::Model> m_goalModel;
	std::unique_ptr<DirectX::Model> m_doorModel;
	//�G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_state;

	//�ʒu���
	DirectX::SimpleMath::Vector3 m_position;

	//���̔���
	bool m_key;

public:
	GoalObject(DX::DeviceResources* m_pDR);
	~GoalObject();

	//������
	void Initilize(DirectX::SimpleMath::Vector3 position);
	//�X�V
	void Update( DX::StepTimer& m_stepTimer,bool key);
	//�`��
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//�I������
	void Finalize();

};