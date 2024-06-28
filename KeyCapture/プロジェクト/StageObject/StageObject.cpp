#include "pch.h"
#include "StageObject.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace DirectX;
#define MOVE_HEIGHT_TITLENAME (10)
#define KEY_NUM (3)

//int StageObject::m_Map[WIDTH][HEIGHT] =
//{
//	 {5, 5, 5, 5, 5, 5, 5, 5, 5},
//	 {3, 0, 0, 0, 0, 0, 0, 0, 5},
//	 {5, 0, 5, 5, 0, 5, 5, 0, 5},
//	 {5, 0, 5, 0, 0, 0, 5, 0, 5},
//	 {5, 0, 0, 0, 5, 0, 0, 0, 5},
//	 {5, 0, 5, 0, 0, 0, 5, 0, 5},
//	 {5, 0, 5, 5, 0, 5, 5, 0, 5},
//	 {5, 0, 0, 0, 0, 0, 0, 0, 1},
//	 {5, 5, 5, 5, 5, 5, 5, 5, 5}
//};

StageObject::StageObject(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse)
	:m_deviceResources(m_pDR)								//�f�o�C�X���\�[�X
	, m_keyboard(key)										//�L�[�{�[�h
	, m_mouse(mouse)											//�}�E�X
	, num(0)
	, m_goalOpen(false)

{
	//�L�[�{�[�h�ƃ}�E�X�̃g���b�J�[
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_buttonTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	//������
	m_objectinfo.clear();
}

StageObject::~StageObject()
{

}

void StageObject::Initilize(int StageNumber)
{
	//�f�o�C�X
	auto device = m_deviceResources->GetD3DDevice();
	//�R���e�L�X�g
	auto context = m_deviceResources->GetD3DDeviceContext();

	//�}�b�v�̃f�[�^�쐬
	LoadMap(pathList[StageNumber]);

	//�G�t�F�N�g
	m_effect = std::make_unique<DirectX::EffectFactory>(device);

	//�R�����X�e�[�g�I�u�W�F�N�g�̍쐬
	m_state = std::make_unique<DirectX::CommonStates>(device);

	//�L�[�I�u�W�F�N�g�̍쐬
	m_keyObject = std::make_unique<KeyObject>(m_deviceResources);

	//�S�[���I�u�W�F�N�g�̍쐬
	m_goalObject = std::make_unique<GoalObject>(m_deviceResources);

	//�G�t�F�N�g�̃p�X��ݒ�
	m_effect->SetDirectory(L"Resources/cmo");

	//�ǃ��f���̃��[�h
	m_wallModel = DirectX::Model::CreateFromCMO(
		device,
		L"Resources/cmo/Wall.cmo",
		*m_effect
	);

	//���̃��f��(��)
	m_keyModel = DirectX::GeometricPrimitive::CreateBox(context, SimpleMath::Vector3(1.0f, 0.5f, 1.0f));

	//�s��̏�����
	m_wallWorld = DirectX::SimpleMath::Matrix::Identity;

	//���̐�
	num = KEY_NUM;

	for (int w = 0; w < WIDTH; w++)
	{
		for (int h = 0; h < HEIGHT; h++)
		{
			//���̈ʒu��n��
			if (m_Map[h][w] == 0)
			{
				m_floorPosition.push_back(SimpleMath::Vector3(w * 2.0f, 1.0f, h * 2.0f));
			}

			//���̈ʒu��n��
			if (m_Map[h][w] == 4)
			{
				m_keyObject->PushPosition(SimpleMath::Vector3(w * 2.0f, 1.0f, h * 2.0f));
			}

			//�����̓G�̈ʒu��n��
			if (m_Map[h][w] == 2)
			{
				m_enemyPosition = SimpleMath::Vector3(w * 2.0f, 1.0f, h * 2.0f);
			}

			//�X�^�[�g�n�_��ݒ肷��
			if (m_Map[h][w] == 1)
			{
				m_startPosition = SimpleMath::Vector3(w * 2.0f, 0.0f, h * 2.0f);
			}

			//�S�[���n�_��ݒ肷��
			if (m_Map[h][w] == 3)
			{
				m_goalObject->Initilize(DirectX::SimpleMath::Vector3(w * 2.0f, 0.0f, h * 2.0f));
			}

			//�ǂ̒n�_��n��
			if (m_Map[h][w] == 5)
			{
				m_wallWorld = DirectX::SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(w * 2.0f, 0.0f, h * 2.0f));

				//�����蔻��̐ݒ�
				Collision::GetInstance()->SetWallObjectModel(m_wallModel.get(), m_wallWorld, SimpleMath::Vector3::One);
			}
		}
	}



	//�L�[�̏�����
	m_keyObject->Initilize(3);
}

void StageObject::Update(DX::StepTimer& m_stepTimer)
{
	// �o�ߎ��Ԃ��擾����
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());
	//�L�[�{�[�h�̏�Ԃ��擾
	auto keystate = m_keyboard->Get().GetState();
	//�L�[�{�[�h�X�e�[�g���X�V
	m_keyState->Update(keystate);
	//�}�E�X�̏�Ԃ��擾
	auto mouse = m_mouse->Get().GetState();
	//�}�E�X�N���b�N���X�V
	m_buttonTracker->Update(mouse);

	//�L�[�I�u�W�F�N�g���X�V����
	m_keyObject->Update(m_stepTimer);

	//�S�[���I�u�W�F�N�g�X�V����
	m_goalObject->Update(m_stepTimer,m_keyObject->Getkeyget());

	//���̐��̍X�V
	m_keyNum = m_keyObject->GetkeyNum();

	//�S�[������(�������)
	if (m_keyObject->Getkeyget())
	{
		m_goalOpen = true;
	}


}


void StageObject::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto device =m_deviceResources->GetD3DDevice();
	auto context =m_deviceResources->GetD3DDeviceContext();
	
	//�X�e�[�W�̕`��
	for (int w = 0; w < WIDTH; w++)
	{
		for (int h = 0; h < HEIGHT; h++)
		{
			//�ǂ̔z�u
			if (m_Map[h][w] == 5)
			{
				//���s�ړ�
				DirectX::SimpleMath::Matrix trans =
					DirectX::SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(w * 2.0f, 0.0f, h * 2.0f));

				m_wallWorld = trans;

				//�ǂ̕`��
				m_wallModel->Draw(
					context,
					*m_state,
					m_wallWorld,
					view,
					proj
				);
			}
		}
	}

	//���̕`��
	m_keyObject->Render(view, proj);

	//�S�[���̕`��
	m_goalObject->Render(view, proj);

}

void StageObject::Finalize()
{
	
}

/// <summary>
/// �}�b�v�ǂݍ���
/// 0 = ��
/// 1 = �X�^�[�g
/// 2 = �G
/// 3 = �S�[��
/// 4 = ��
/// 5 = ��
/// </summary>
/// <param name="path"></param>
void StageObject::LoadMap(const wchar_t* path)
{
	//�C���|�[�g�t�@�C���p�ϐ�
	std::ifstream ifs;

	//�ϐ��ɊO���t�@�C�����J��
	ifs.open(path);

	//�ǂݍ��ݎ��s�����ꍇ
	if (ifs.fail())
		return;

	//������擾
	std::string buf1;

	//�s�̎擾
	for (int i = 0; i < WIDTH; i++)
	{
		//��s��
		std::getline(ifs, buf1);

		//��������i�[
		std::stringstream ss(buf1);

		//�����ϐ�
		std::string buf2;

		//��̎擾
		for (int j = 0; j < HEIGHT; j++)
		{
			std::getline(ss, buf2, ',');

			m_Map[i][j] = atoi(buf2.c_str());
		}
	}

	ifs.close();
}
