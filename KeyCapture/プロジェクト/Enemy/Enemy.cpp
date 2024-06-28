#include "pch.h"
#include "Enemy.h"
#include "Scene/PlayScene.h"
#include "BaseClass/Object.h"
#include <random>

#define ENEMY_SPEED 0.1f;		//�G�̑��xs

using namespace DirectX;

Enemy::Enemy(DX::DeviceResources*pDR, DirectX::Model* model,DirectX::Keyboard* key,
	DirectX::Mouse* mouse, DirectX::SimpleMath::Vector3 pos):
	m_deviceResource(pDR),														//�f�o�C�X���\�[�X
	m_model(model),																// �v���[���[���f��
	m_inCollision(false),														// �Փ˒�
	m_inpact(0.0f),
	m_position(pos),
	m_nextPos(SimpleMath::Vector3::Zero)
{
	
}

Enemy::~Enemy()
{
	
}

void Enemy::Initilize(DirectX::SimpleMath::Vector3 pos)
{
	auto device = m_deviceResource->GetD3DDevice();
	auto context = m_deviceResource->GetD3DDeviceContext();

	//�ʒu��}��
	m_position = pos;

	//�R�����X�e�[�g�̍쐬
	m_states = std::make_unique<DirectX::CommonStates>(device);

	//�Փ˔���
	m_inCollision = false;
}

void Enemy::Update(const float& m_stepTimer)
{
	//�ړ�����|�C���g��T��
	SeekNextPoint(2);

	int length = m_position.Length() - m_targetPos.Length();
	
    SimpleMath::Vector3 range = (m_position - m_nextPos);

	if (range.Length() != 0)
	{
		m_position += ReturnVelocity(range);
	}
	else
	{
		m_velocity = SimpleMath::Vector3::Zero;
	}

	if (Collision::GetInstance()->GetDamageCollider(m_position))
	{
		m_inCollision = true;
	}

}

void Enemy::SeekNextPoint(int radius)
{
	

	//���̏���������Ĉړ�������߂�
	for (int i = 0; i < m_floorPosition.size(); i++)
	{
		int length = (m_position - m_floorPosition[i]).Length();

		//���݂̈ʒu�Ə��̈ʒu�ňړ���
		if (radius > length )
		{
			////�ڕW�ʒu�ƌ��݂̈ʒu�̋������͂���
			//float range = m_position.Length() - m_targetPos.Length();

			//float nextRange = m_floorPosition[i].Length() - m_position.Length();
			
			m_nextPos = m_floorPosition[i];
			
		}
	}
	

}

SimpleMath::Vector3 Enemy::ReturnVelocity(SimpleMath::Vector3 range)
{
	SimpleMath::Vector3 veloctiy = SimpleMath::Vector3::Zero;

	//x���ɑ΂���
	if (range.x > 0) veloctiy.x -= ENEMY_SPEED;
	if (range.x < 0) veloctiy.x += ENEMY_SPEED;
	//z���ɑ΂���
	if (range.z > 0) veloctiy.z -= ENEMY_SPEED;
	if (range.z < 0) veloctiy.z += ENEMY_SPEED;

	return veloctiy;
}

void Enemy::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_deviceResource->GetD3DDeviceContext();

	//���s�ړ�
	m_world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	//���f���`��
	m_model->Draw(context, *m_states, m_world, view, proj);
}

void Enemy::Finalize()
{
	
}
