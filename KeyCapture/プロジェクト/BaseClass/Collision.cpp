#include "pch.h"
#include "BaseClass/Collision.h"

using namespace DirectX;

std::unique_ptr<Collision> Collision::m_collision = nullptr;

Collision* const Collision::GetInstance()
{
	if (m_collision == nullptr)
	{
		m_collision.reset(new Collision);
	}
	//�R���W�����̃|�C���^�[��n��
	return m_collision.get();
}

Collision::Collision():
	m_playerCol(false),
	m_enemyCol(false)
{	
	
}

Collision::~Collision()
{
	// �o�E���f�B���O�{�b�N�X�����Z�b�g����
	m_enemyBoudingBox.clear();
}

void Collision::Initialize()
{
	m_objectBox.clear();
	m_wallObjectBox.clear();
	m_enemyBoudingBox.clear();
}

void Collision::Update(DirectX::SimpleMath::Vector3 playerpos, DirectX::SimpleMath::Vector3 enemypos,DirectX::SimpleMath::Vector3 weaponpos)
{
	m_playerCol = false;
	m_enemyCol = false;
	m_wallCol = false;
	m_ItemCol = false;

	for (int i = 0; i < m_enemyBoudingBox.size(); i++)
	{
		//�����蔻����ړ�������
		m_enemyBoudingBox[i].Center = enemypos;
		//m_playerBoundingBox.Center = playerpos;
		
		m_playerBoundingBox.Extents = SimpleMath::Vector3(0.10f);

		//�v���C���[�ƓG�̓����蔻��
		if (m_enemyBoudingBox[i].Intersects(m_playerBoundingBox))
		{
			m_playerCol = true;
		}
	}

	//�A�C�e���ƃv���C���[�̓����蔻��
	for (int i = 0; i < m_objectBox.size(); i++)
	{
		if (m_objectBox[i].Intersects(m_playerBoundingBox)) 
		{
			m_playerCol = true;
		}
	}
}

void Collision::Render()
{
	
}

void Collision::Finalize()
{
}

void Collision::SetBoudingBox(const DirectX::Model& model)
{
	//���f�����烁�b�V�����
	auto mesh = model.meshes;
	DirectX::BoundingBox boundBox;
	//�o�E���f�B���O�{�b�N�X��ݒ�
	for (int i = 0; i < mesh.size(); i++)
	{
		boundBox = mesh[i]->boundingBox;
	}
	//�z����
	m_enemyBoudingBox.push_back(boundBox);
}

void Collision::SetAttackBoudingBox(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scale)
{
	DirectX::BoundingBox boundBox;
	//�o�E���f�B���O�{�b�N�X�̐ݒ�
	boundBox.Center = pos;
	boundBox.Extents = scale;

	m_attackBoudingBox = boundBox;
}

void Collision::SetPlayerBoudingBox(const DirectX::Model& player,DirectX::SimpleMath::Vector3 pos)
{
	//���f�����烁�b�V�����
	auto mesh = player.meshes;
	//�o�E���f�B���O�{�b�N�X��ݒ�
	for (int i = 0; i < mesh.size(); i++)
	{
		m_playerBoundingBox = mesh[i]->boundingBox;
		m_playerBoundingBox.Center = pos;
	}
}

void Collision::SetPlayerBoudingBox(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& size)
{
	m_playerBoundingBox.Center = pos;
	m_playerBoundingBox.Extents = size;
}
