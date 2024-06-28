#pragma once
#include <vector>
#include <map>
#include <string>
#include "Camera/Camera.h"

class Collision
{
public:
	//�V���O���g��
	static Collision* const GetInstance();

private:
	//�R���W�����̃|�C���^�[
	static	std::unique_ptr<Collision> m_collision;

private:
	/*�|�C���^�[*/
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviResources;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_state;
	//���f��
	DirectX::Model* m_model;
	//�G�t�F�N�g
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//�}�E�X
	DirectX::Mouse* m_mouse;
	//�}�E�X�g���b�J�[
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonState;
	//�L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	//�L�[�{�[�h�X�e�[�g�g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyState;

private:
	/*�����蔻��*/
	//�v���C���[�̓����蔻��
	bool m_playerCol;
	//�G�l�~�[�̓����蔻��
	bool m_enemyCol;
	//�A�C�e���̓����蔻��
	bool m_ItemCol;
	//�ǂƂ̓����蔻��
	bool m_wallCol;

private:
	//�G�̓����蔻��
	std::vector<DirectX::BoundingBox> m_enemyBoudingBox;
	//�v���C���[�̓����蔻��
	DirectX::BoundingBox m_playerBoundingBox;
	//�v���C���[�̍U�������蔻��
	DirectX::BoundingBox m_attackBoudingBox;
	//�����Ȃ��I�u�W�F�N�g�̓����蔻��
	std::vector<DirectX::BoundingBox> m_objectBox;
	//�ǂɃI�u�W�F�N�g�̓����蔻��
	std::vector<DirectX::BoundingBox> m_wallObjectBox;
	//�S�[���n�_�̓����蔻��
	DirectX::BoundingBox m_goalBox;

private:
	//�����p�����蔻��
	std::vector<DirectX::GeometricPrimitive> m_geo;

public:
	//�����蔻���ݒ�
	void SetBoudingBox(const DirectX::Model& model);
	void SetAttackBoudingBox(DirectX::SimpleMath::Vector3 pos,DirectX::SimpleMath::Vector3 scale);
	void SetPlayerBoudingBox(const DirectX::Model& player, DirectX::SimpleMath::Vector3 pos);
	void SetPlayerBoudingBox(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& size);

	//�G�ƍU���̓����蔻��̎擾
	bool GetEnemyCollder() { return m_enemyCol; }

	//�ǂƂ̓����蔻��
	bool GetWallCollider() { return m_wallCol; }

public:
	// �R���X�g���N�^
	Collision();

	// ����͋��e���Ȃ�
	void operator=(const Collision& object) = delete;
	// �R�s�[�R���X�g���N�^�͋��e���Ȃ�
	Collision(const Collision& object) = delete;
	// �f�X�g���N�^
	~Collision();

	//������
	void Initialize();

	//�X�V
	 void Update(DirectX::SimpleMath::Vector3 playerpos,DirectX::SimpleMath::Vector3 
		 ,DirectX::SimpleMath::Vector3 weaponpos);

	 //�`��
	 void Render();

	//�I������
	 void Finalize();

	 //�ÓI�ȃI�u�W�F�N�g�̒ǉ�
	 void SetWallObjectModel(DirectX::Model* model, DirectX::SimpleMath::Matrix worldMatrix,DirectX::SimpleMath::Vector3 scale)
	 {
		 //�o�E���f�B���O�{�b�N�X
		 DirectX::BoundingBox m_boundingBox;

		 for (int i = 0; i < model->meshes.size(); i++)
		 {
			 m_boundingBox = model->meshes[i]->boundingBox;
			 m_boundingBox.Center = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Zero, worldMatrix);
			 m_boundingBox.Extents = scale;
		 }

		 m_wallObjectBox.push_back(m_boundingBox);
	 };

	 //�S�[���n�_�̔���
	 void SetGoalBoundingBox(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 scale)
	 {
		 //�o�E���f�B���O�{�b�N�X
		 DirectX::BoundingBox boudingBox;

		 //�����蔻�������
		 boudingBox.Center = position;
		 boudingBox.Extents = scale;

		 m_goalBox = boudingBox;
	 }

	 //�W�I���g���I�u�W�F�N�g�̒ǉ�
	 void SetObjectModel(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scale)
	 {
		 //�o�E���f�B���O�{�b�N�X
		 DirectX::BoundingBox m_boundingBox;

		 //�o�E���f�B���O�{�b�N�X�ɏ���ǉ�����
		 m_boundingBox.Center = pos;
		 m_boundingBox.Extents = scale;


		 m_objectBox.push_back(m_boundingBox);
	 };

	 //�I�u�W�F�N�g�̒ǉ�

	 //---------------------�����蔻��--------------------------//

	 bool GetNextWallBound(DirectX::SimpleMath::Vector3 nextPos)
	 {
		 //����p�R�s�[
		 DirectX::BoundingBox playerBox = m_playerBoundingBox;
		 //�v���C���[�̓����蔻����ړ�
		 playerBox.Center.x += nextPos.x;
		 playerBox.Center.y += nextPos.y;
		 playerBox.Center.z += nextPos.z;

		 //�ǂƐڐG���Ă��邩�̔���
		 for (int i = 0; i < m_wallObjectBox.size(); i++)
		 {
			 if (m_wallObjectBox[i].Intersects(playerBox))
			 {
				 return true;
			 }
		 }

		 //�ړ��ł���̂ő}��
		 m_playerBoundingBox = playerBox;

		 return false;
	 }

	 //�v���C���[�ƃA�C�e���̓����蔻��̎擾
	 bool GetItemCollider(int number) 
	 { 
		 
		 if (m_objectBox[number].Intersects(m_playerBoundingBox))
		 {
			 //�v�f�̍폜
			 m_objectBox.erase(m_objectBox.begin() + number);

			return true;
		 }
		 

		 return false;

	 }

	 //�����蔻��̎擾
	 bool GetDamageCollider(DirectX::SimpleMath::Vector3 pos)
	 {
		 for (int i = 0; i < m_enemyBoudingBox.size(); i++)
		 {
			 m_enemyBoudingBox[i].Center = pos;

			 if (m_enemyBoudingBox[i].Intersects(m_playerBoundingBox))
			 {
				 return true;
			 }
		 }

		 return false;
	 }

	 //�S�[���̓����蔻��̎擾
	 bool GetGoalCollider()
	 {
		 if (m_playerBoundingBox.Intersects(m_goalBox))
		 {
			 return true;
		 }

		 return false;
	 }
};