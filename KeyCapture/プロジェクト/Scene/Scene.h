#pragma once
#include "PlayScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "ResultScene.h"
#include "Camera/Camera.h"

class Scene
{
private:

	enum  SceneState
	{
		Title = 0,
		Play,
		Select,
		Setting,
		Quit,
		Result
	};

private:
	//ゲームシーンのポインタ
	std::unique_ptr<PlayScene> m_playScene;
	//タイトルシーンのポインタ
	std::unique_ptr<TitleScene> m_titleScene;
	//セレクトシーンのポインタ
	std::unique_ptr<SelectScene> m_selectScene;
	//リザルトシーンのポインタ
	std::unique_ptr<ResultScene> m_resultScene;
	//デバイス
	ID3D11Device* m_device;
	//コンテキスト
	ID3D11DeviceContext* m_context;

	//デバイスリソース
	DX::DeviceResources* m_deviceResource;

	//シーン更新用列挙型変数
	SceneState m_sceneState;

	//キーボードとマウス
	DirectX::Mouse* m_mouse;
	DirectX::Keyboard* m_keyBoard;	

	//キーステート
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyTracker;
private:
	//シーン遷移用変数
	bool m_sceneChanger;
	
public:
	Scene(DX::DeviceResources* pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~Scene();

	void Initialize();
	void Update(DX::StepTimer timer);
	void Render();
	void Finalize();

	void EndGame();

	//ステージ選択用の関数
	void StageSelectFunc();

public:
	//シーンの状態を設定
	void SetSceneState(SceneState& sceneState) { m_sceneState = sceneState; }
	//シーンの状態を取得
	SceneState GetSceneState() { return m_sceneState; }
};
