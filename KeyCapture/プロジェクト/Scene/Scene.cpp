#include "pch.h"
#include "Scene.h"

Scene::Scene(DX::DeviceResources* pDR,DirectX::Keyboard* key, DirectX::Mouse* mouse):
	m_deviceResource(pDR),
	m_keyBoard(key),
	m_mouse(mouse),
	m_sceneChanger(false),
	m_sceneState(SceneState::Title)
{
	
	//�f�o�C�X
	m_device = m_deviceResource->GetD3DDevice();
	//�R���e�L�X�g
	m_context = m_deviceResource->GetD3DDeviceContext();
}

Scene::~Scene()
{
}

void Scene::Initialize()
{
	Camera::GetInstance()->SetDeviceResource(m_deviceResource);
	//�J�����̃Z�b�e�B���O
	Camera::GetInstance()->CreateCameraSetting(m_keyBoard, m_mouse);

	//�v���C�V�[���̍쐬
	m_playScene = std::make_unique<PlayScene>(m_deviceResource, m_keyBoard, m_mouse);
	//�^�C�g���V�[���̍쐬
	m_titleScene = std::make_unique<TitleScene>(m_deviceResource, m_keyBoard, m_mouse);
	//�Z���N�g�V�[���̍쐬
	m_selectScene = std::make_unique<SelectScene>(m_deviceResource, m_keyBoard, m_mouse);
	//���U���g�V�[���̍쐬
	m_resultScene = std::make_unique<ResultScene>(m_deviceResource, m_keyBoard, m_mouse);

	//�L�[�X�e�[�g
	m_keyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//�V�[���X�e�[�g�̏�����
	m_sceneState = SceneState::Title;

	//������
	switch (m_sceneState)
	{
	case Scene::Title:
		m_titleScene->Initilize();
		break;
	case Scene::Play:
		m_playScene->Initilize(1);
		break;
	case Scene::Select:
		m_selectScene->Initilize();
		break;
	case Scene::Setting:
		break;
	case Scene::Quit:
		break;
	case Scene::Result:
		m_resultScene->Initilize();
		break;
	default:
		break;
	}
	
	//�V�[���J�ڂ̔���̏�����
	m_sceneChanger = false;
}

void Scene::Update(DX::StepTimer timer)
{
	//�L�[���
	auto key = m_keyBoard->Get().GetState();
	//�L�[�X�e�[�g
	auto keystate = m_keyTracker->GetLastState();

	//�G�X�P�[�v�ŃQ�[���I��
	if (keystate.Escape)
	{
		EndGame();
	}

	//�J�����̍X�V
	Camera::GetInstance()->Update();
	

	//scene�Ǘ�
	if (m_sceneChanger)
	{
		switch (m_sceneState)
		{
		case Scene::Title:
			m_selectScene->Initilize();
			if (m_titleScene->GetButtonIndex() == 0) m_sceneState = SceneState::Select;
			if (m_titleScene->GetButtonIndex() == 1) m_sceneState = SceneState::Setting;
			if (m_titleScene->GetButtonIndex() == 2) m_sceneState = SceneState::Quit;
			m_sceneChanger = false;
			break;
		case Scene::Play:
			m_resultScene->Initilize();
			m_sceneState = SceneState::Result;
			m_sceneChanger = false;
			break;
		case Scene::Select:
			m_playScene->Initilize(m_selectScene->GetButtonIndex());
			m_sceneState = SceneState::Play;
			m_sceneChanger = false;
			break;
		case Scene::Setting:
			m_sceneState = SceneState::Quit;
			m_sceneChanger = false;
			break;
		case Scene::Quit:
			break;
		case Scene::Result:
			m_titleScene->Initilize();
			m_sceneState = SceneState::Title;
			m_sceneChanger = false;
			break;
		default:
			break;
		}
	}

	//�X�V
	switch (m_sceneState)
	{
	case Title:
		m_titleScene->Update(timer);
		m_sceneChanger = m_titleScene->GetSceneChanger();
		break;
	case Play:
		m_playScene->Update(timer);
		m_sceneChanger = m_playScene->GetSceneChanger();
		break;
	case Select:
		m_selectScene->Update(timer);
		m_sceneChanger = m_selectScene->GetSceneChanger();
		break;
	case Setting:
		//m_sceneState = SceneState::Title;
		break;
	case Quit:
		EndGame();
		break;
	case Result:
		m_resultScene->Update(timer);
		m_sceneChanger = m_resultScene->GetSceneChanger();
		break;
	}
}
	

void Scene::Render()
{

	//�X�V
	switch (m_sceneState)
	{
	case Title:
		m_titleScene->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());
		break;
	case Play:
		m_playScene->Render();
		break;
	case Select:
		m_selectScene->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());
		break;
	case Result:
		m_resultScene->Render(Camera::GetInstance()->GetView(), Camera::GetInstance()->GetProjection());
		break;
	}
}

void Scene::Finalize()
{
}

//�Q�[���I��
void Scene::EndGame()
{
	PostQuitMessage(0);
}