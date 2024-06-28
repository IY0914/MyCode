#include "pch.h"
#include "Player.h"
#include "Scene/PlayScene.h"

using namespace DirectX;

const float	Player::RUN_SPEED      = 5.0f;	//�v���C���[�̍ō����x
const float	Player::AVOID_TIME	   = 1.0f;	//�������

Player::Player(DX::DeviceResources* pDR, DirectX::Model* model,
	DirectX::Keyboard* key,DirectX::Mouse* mouse, DirectX::SimpleMath::Vector3 pos):
	m_deviceResource(pDR),					//�f�o�C�X���\�[�X			
	m_model(model),							//�v���[���[���f��	
	m_keyboard(key),						//�L�[�{�[�h
	m_mouse(mouse),							//�}�E�X
	m_position(pos),						//�ʒu
	m_survive(false),						//�����c�蔻��									
	m_itemNum(0)
{
	
}

Player::~Player()
{
	
}

void Player::Initilize()
{
	auto device = m_deviceResource->GetD3DDevice();
	auto context = m_deviceResource->GetD3DDeviceContext();

	//�R�����X�e�[�g�̍쐬
	m_states = std::make_unique<DirectX::CommonStates>(device);
	//�G�t�F�N�g�̍쐬
	m_effect = std::make_unique<DirectX::EffectFactory>(device);
	//�L�[�{�[�h�X�e�[�g�g���b�J�[���쐬
	m_stateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//�W�I���g�����f��
	m_geometricModel = DirectX::GeometricPrimitive::CreateBox(context, SimpleMath::Vector3(1.0f, 2.0f, 1.0f));

	//�����c�菉����
	m_survive = true;
}

void Player::Update(const float& m_stepTimer)
{
	//�L�[���
	auto key = m_keyboard->Get().GetState();

	//�v���C���[�̓���
	MoveUpdate(m_stepTimer);

	if (//���̃v���C���[�̈ʒu��n��
		Collision::GetInstance()->GetNextWallBound(m_velocity))
	{
		m_velocity = SimpleMath::Vector3::Zero;
	}
	//�S�[�����������Ă��Ȃ��ꍇ�ʂ�Ȃ�
	if (Collision::GetInstance()->GetGoalCollider())
	{
		m_velocity = SimpleMath::Vector3::Zero;
	}

	//�ʒu�ɉ����x�����Z
	m_position += m_velocity;
}

void Player::MoveUpdate(const float frameTimer)
{
	//�L�[���
	auto key = m_keyboard->GetState();
	//�L�[�X�e�[�g
	m_stateTracker->Update(key);
	//�}�E�X���
	auto mouse = m_mouse->GetState();

	//�����x��������
	m_velocity = { 0.0f };

	//�O�i
	if (key.W)			m_velocity.z += frameTimer * RUN_SPEED;
	//��i
	if (key.S)			m_velocity.z += frameTimer * -RUN_SPEED;
	//�E�ړ�
	if (key.D)			m_velocity.x += frameTimer * -RUN_SPEED;
	//���ړ�
	if (key.A)			m_velocity.x += frameTimer *  RUN_SPEED;
}


void Player::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//�f�o�C�X�ƃR���e�L�X�g
	auto device = m_deviceResource->GetD3DDevice();
	auto context = m_deviceResource->GetD3DDeviceContext();

	//�s��ϊ�(��])
	m_rot_Y = SimpleMath::Matrix::CreateRotationY(Camera::GetInstance()->GetMouseRotate());
	//�s��ϊ�(�ړ�)
	m_trans = SimpleMath::Matrix::CreateTranslation(m_position);

	//�s��v�Z
	m_world = m_rot_Y * m_trans;

	//���f���̕`��
	//m_model->Draw(context, *m_states, m_world, view, proj);
	m_geometricModel->Draw(m_world, view, proj,Colors::Red);
}

void Player::Finalize()
{
}