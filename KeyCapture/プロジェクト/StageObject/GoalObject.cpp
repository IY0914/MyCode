#include "pch.h"
#include "GoalObject.h"

using namespace DirectX;

GoalObject::GoalObject(DX::DeviceResources* m_pDR)
	: m_deviceResources(m_pDR)	//�f�o�C�X���\�[�X
	, m_position(DirectX::SimpleMath::Vector3::Zero)
	, m_key(false)
{
	
}

GoalObject::~GoalObject()
{

}

void GoalObject::Initilize(DirectX::SimpleMath::Vector3 position)
{
	//�f�o�C�X
	auto device = m_deviceResources->GetD3DDevice();
	//�R���e�L�X�g
	auto context = m_deviceResources->GetD3DDeviceContext();

	//�ʒu���
	m_position = position;

	//�G�t�F�N�g�t�@�N�g���[�̍쐬
	m_effect = std::make_unique<DirectX::EffectFactory>(device);
	m_effect->SetDirectory(L"Resources/cmo");

	//���ʃX�e�[�g
	m_state = std::make_unique<DirectX::CommonStates>(device);

	//���f�����[�h
	m_goalModel = DirectX::Model::CreateFromCMO(device, L"Resources/cmo/GoalFlag.cmo", *m_effect.get());
	m_doorModel = DirectX::Model::CreateFromCMO(device, L"Resources/cmo/Door.cmo", *m_effect.get());

	//�����蔻��̎擾
	Collision::GetInstance()->SetGoalBoundingBox(position, m_goalModel->meshes[0]->boundingBox.Extents);
}

void GoalObject::Update(DX::StepTimer& m_stepTimer, bool key)
{
	m_key = key;
}

void GoalObject::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto device =m_deviceResources->GetD3DDevice();
	auto context =m_deviceResources->GetD3DDeviceContext();
	
	//���s�ړ��s��
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	//�S�[�����f���̕`��
	m_goalModel->Draw(context, *m_state.get(), world, view, proj);

	if (!m_key)
	{
		//�S�[���̎�O�ɏo��
		world *= DirectX::SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, 0.0f, -1.0f));

		//�S�[�����������Ă��Ȃ��ꍇ(���̕`��)
		m_doorModel->Draw(context, *m_state.get(), world, view, proj);
	}
}

void GoalObject::Finalize()
{
	
}