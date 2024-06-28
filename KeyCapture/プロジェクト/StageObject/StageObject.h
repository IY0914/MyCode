#pragma once
#include "DeviceResources.h"
#include "BaseClass/Collision.h"
#include "KeyObject.h"
#include "GoalObject.h"
#include <vector>

class StageObject
{
public:
	struct ObjectInfo
	{
		//�ʒu���
		DirectX::SimpleMath::Vector3 position;
		//��]
		float rotate;
		//�o�E���f�B���O�{�b�N�X
		DirectX::BoundingBox m_boudingBox;
	};

private:
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	//�L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	//�L�[�{�[�h�X�e�[�g
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyState;
	//�}�E�X
	DirectX::Mouse* m_mouse;
	//�}�E�X�X�e�[�g�g���b�J�[
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonTracker;
	//���f��
	std::unique_ptr<DirectX::Model> m_wallModel;
	//���W���f��
	std::unique_ptr<DirectX::GeometricPrimitive> m_keyModel;
	//�G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_effect;
	//�I�u�W�F�N�g���
	std::vector<ObjectInfo> m_objectinfo;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_state;


	//���I�u�W�F�N�g
	std::unique_ptr<KeyObject> m_keyObject;
	//���̐�
	int num;

	//�S�[���̃I�u�W�F�N�g
	std::unique_ptr<GoalObject> m_goalObject;
	//�S�[���̊J������
	int m_keyNum;		//��
	bool m_goalOpen;
	
	//�}�b�v
	const static int WIDTH = 20;
	const static int HEIGHT = 20;
	int m_Map[WIDTH][HEIGHT];

	//�}�b�v�̔z��
	const wchar_t* pathList[3] =
	{ 
	  L"Resources/Data/MapData1.csv",
	  L"Resources/Data/MapData2.csv",
	  L"Resources/Data/MapData3.csv" 
	};

	//�J�n�n�_
	DirectX::SimpleMath::Vector3 m_startPosition;
	//�S�[���n�_
	DirectX::SimpleMath::Vector3 m_GoalPosition;
	//�G�̒n�_
	DirectX::SimpleMath::Vector3 m_enemyPosition;
	//���̒n�_
	std::vector<DirectX::SimpleMath::Vector3> m_floorPosition;

private:
	//�Ǖ`��s��
	DirectX::SimpleMath::Matrix m_wallWorld;

public:
	StageObject(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~StageObject();

	//������
	void Initilize(int StageNumber);
	//�X�V
	void Update( DX::StepTimer& m_stepTimer);
	//�`��
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//�I������
	void Finalize();
	//�}�b�v�ǂݍ���
	void LoadMap(const wchar_t* path);

public:
	//�ʒu�̏��
	DirectX::SimpleMath::Vector3 GetStartPosition() { return m_startPosition; }
	DirectX::SimpleMath::Vector3 GetGoalPosition() { return m_GoalPosition; }
	DirectX::SimpleMath::Vector3 GetEnemyPosition() { return m_enemyPosition; }
	std::vector<DirectX::SimpleMath::Vector3> GetFloorPosition() { return m_floorPosition; }
	//�S�[�������̎擾
	bool GetGoalOpen() { return m_goalOpen; }

	//���̌���Ԃ�
	int GetKeyNum() { return m_keyNum; }
};