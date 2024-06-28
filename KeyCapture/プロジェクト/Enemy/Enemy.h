#pragma once

class Enemy
{
public:
	// �Փ˒��t���O���擾����
	bool GetInCollision() { return m_inCollision; }
	// �Փ˒��t���O���Z�b�g/���Z�b�g����
	void SetInCollision(bool inCollision) { m_inCollision = inCollision; }

public:
	//�R���X�g���N�^
	Enemy(DX::DeviceResources* pDR, DirectX::Model* model, DirectX::Keyboard* key, DirectX::Mouse* mouse, DirectX::SimpleMath::Vector3 pos);
	//�f�X�g���N�^
	~Enemy();

	void Initilize(DirectX::SimpleMath::Vector3 pos);
	void Update(const float& m_stepTimer);
	void SeekNextPoint(int radius);
	//�ړ��̊֐�
	DirectX::SimpleMath::Vector3 ReturnVelocity(DirectX::SimpleMath::Vector3 range);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

public:
	//�ڕW�ʒu��ݒ�
	void SetTargetPos(DirectX::SimpleMath::Vector3 pos) { m_targetPos = pos; }

	//�ʒu�����擾
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	//���̏�Ԃ��擾
	void SetFloorPosition(std::vector<DirectX::SimpleMath::Vector3> pos) { m_floorPosition = pos; }

private:
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResource;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//���f��
	DirectX::Model* m_model;
	//�ʒu���W
	DirectX::SimpleMath::Vector3 m_position;
	//�ڕW���W
	DirectX::SimpleMath::Vector3 m_targetPos;
	//���̈ڈʒu
	DirectX::SimpleMath::Vector3 m_nextPos;
	//�����x
	DirectX::SimpleMath::Vector3 m_velocity;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�ړ��\�ȏ��̏��
	std::vector<DirectX::SimpleMath::Vector3> m_floorPosition;

	// �Փ˒�
	bool m_inCollision;

	//�G�ɏՌ�
	float m_inpact;
};