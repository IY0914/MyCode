#include "pch.h"
#include "KeyObject.h"
#include <random>

using namespace DirectX;

KeyObject::KeyObject(DX::DeviceResources* m_pDR)
	:m_deviceResources(m_pDR)	//�f�o�C�X���\�[�X
	, m_key(false)
	, m_keyNum(0)
	, m_keyPiece(0)
{
	
}

KeyObject::~KeyObject()
{

}

void KeyObject::Initilize(int num)
{
	//�f�o�C�X
	auto device = m_deviceResources->GetD3DDevice();
	//�R���e�L�X�g
	auto context = m_deviceResources->GetD3DDeviceContext();
	
	//�o�������錮�̐�
	m_keyNum = num;
	m_keyPiece = num;

	//�G�t�F�N�g
	m_effect = std::make_unique<DirectX::EffectFactory>(device);

	//�R�����X�e�[�g�I�u�W�F�N�g�̍쐬
	m_state = std::make_unique<DirectX::CommonStates>(device);

	//�G�t�F�N�g�̃p�X��ݒ�
	m_effect->SetDirectory(L"Resources/cmo");

	//���̃��f��
	m_keyModel = DirectX::Model::CreateFromCMO(device,L"Resources/cmo/key.cmo",*m_effect);

	//�I�u�W�F�N�g�̍쐬
	m_object.resize(m_keyNum);		//�v�f���̕ύX
	//���̈ʒu�z��̃T�C�Y�ύX
	m_keyPosition.resize(m_keyNum);
	//���I�u�W�F�N�g�̐ݒ�
	for (int i = 0; i < m_object.size(); i++)
	{
		m_object[i] = std::make_unique<Object>(m_deviceResources, m_keyModel.get(),L"Resources/Texture/Luseter.png");
		
	}
	
	//�s��̏�����
	m_wallWorld = DirectX::SimpleMath::Matrix::Identity;
	
	//�o�����錮�̐�����
	for (int i = 0; i < m_keyPosition.size(); i++)
	{
		////�������牺�R�s�͍\��
		//std::random_device              seedGenerator;								//�񌈒�I�ɐ������o��(�{���̗���)�F�f�����b�g�͍Č������Ȃ�
		//std::mt19937                    randomNumberGenerator{ seedGenerator() };	//�����G���W���F�����̐�������ōČ������ς��
		//std::uniform_int_distribution<UINT64> distribution{ 0, m_floorPosition.size()};	//�������Ɍ^���w��ł���<>,�ϐ���{���� , ���}���ݒ�ł���

		//11�̗]�肪0�̏��ɒu�� ��:11,22,33
		m_keyPosition[i] = m_floorPosition[i];

		m_object[i]->Initilize(m_keyPosition[i]);
		m_object[i]->SetObjectNumber(i);

		//�����蔻���ǉ�
		Collision::GetInstance()->SetObjectModel(m_keyPosition[i], SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
	}

}

void KeyObject::Update(DX::StepTimer& m_stepTimer)
{
	for (int i = 0; i < m_keyNum; i++)
	{

		m_object[i]->SetObjectNumber(i);

		m_object[i]->Update(m_stepTimer);

		if (!m_object[i]->GetPopItem())
		{
			//�v�f���̌���
			m_keyNum--;

			//�v�f�̍폜
			m_object.erase(m_object.begin() + i);
		}
	}

	if (m_keyNum == 0)
	{
		m_key = true;
	}
}


void KeyObject::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto device =m_deviceResources->GetD3DDevice();
	auto context =m_deviceResources->GetD3DDeviceContext();
	
	//���̕`��
	for (int i = 0; i < m_keyNum; i++)
	{
		m_object[i]->Render(view, proj);
	}
}

void KeyObject::Finalize()
{
	
}