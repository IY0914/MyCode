#include "pch.h"
#include "Scene.h"

Scene::Scene(DX::DeviceResources* pDR,DirectX::Keyboard* key, DirectX::Mouse* mouse):
	m_deviceResource(pDR),
	m_keyBoard(key),
	m_mouse(mouse),
	m_sceneChanger(false),
	m_sceneState(SceneState::Title)
{
	
	//デバイス
	m_device = m_deviceResource->GetD3DDevice();
	//コンテキスト
	m_context = m_deviceResource->GetD3DDeviceContext();
}

Scene::~Scene()
{
}

void Scene::Initialize()
{
	Camera::GetInstance()->SetDeviceResource(m_deviceResource);
	//カメラのセッティング
	Camera::GetInstance()->CreateCameraSetting(m_keyBoard, m_mouse);

	//プレイシーンの作成
	m_playScene = std::make_unique<PlayScene>(m_deviceResource, m_keyBoard, m_mouse);
	//タイトルシーンの作成
	m_titleScene = std::make_unique<TitleScene>(m_deviceResource, m_keyBoard, m_mouse);
	//セレクトシーンの作成
	m_selectScene = std::make_unique<SelectScene>(m_deviceResource, m_keyBoard, m_mouse);
	//リザルトシーンの作成
	m_resultScene = std::make_unique<ResultScene>(m_deviceResource, m_keyBoard, m_mouse);

	//キーステート
	m_keyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//シーンステートの初期化
	m_sceneState = SceneState::Title;

	//初期化
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
	
	//シーン遷移の判定の初期化
	m_sceneChanger = false;
}

void Scene::Update(DX::StepTimer timer)
{
	//キー情報
	auto key = m_keyBoard->Get().GetState();
	//キーステート
	auto keystate = m_keyTracker->GetLastState();

	//エスケープでゲーム終了
	if (keystate.Escape)
	{
		EndGame();
	}

	//カメラの更新
	Camera::GetInstance()->Update();
	

	//scene管理
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

	//更新
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

	//更新
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

//ゲーム終了
void Scene::EndGame()
{
	PostQuitMessage(0);
}