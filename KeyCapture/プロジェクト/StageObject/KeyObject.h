#pragma once
#include "DeviceResources.h"
#include "BaseClass/Collision.h"
#include "BaseClass/Object.h"
#include <vector>

class KeyObject
{
private:
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	//���W���f��
	std::unique_ptr<DirectX::Model> m_keyModel;
	//�G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_state;

	//�I�u�W�F�N�g
	std::vector<std::unique_ptr<Object>> m_object;
	
	
	//�o�����錮�̐�
	int m_keyNum;		//�󂯓n���p
	int m_keyPiece;		//����p
	bool m_key;

	//���̈ʒu�z��
	std::vector<DirectX::SimpleMath::Vector3> m_floorPosition;
	//���̈ʒu�z��
	std::vector<DirectX::SimpleMath::Vector3> m_keyPosition;

private:
	//�Ǖ`��s��
	DirectX::SimpleMath::Matrix m_wallWorld;

public:
	bool Getkeyget() { return m_key; }

	int GetkeyNum() { return m_keyNum; }

public:
	KeyObject(DX::DeviceResources* m_pDR);
	~KeyObject();

	//������
	void Initilize(int num);
	//�X�V
	void Update( DX::StepTimer& m_stepTimer);
	//�`��
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//�I������
	void Finalize();

	//���̈ʒu���l�ߍ���
	void PushPosition(DirectX::SimpleMath::Vector3 pos) { m_floorPosition.push_back(pos); }

};