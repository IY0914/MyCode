#pragma once
#include "DeviceResources.h"
#include "UserInterFace/Button.h"

class ResultScene
{
private:
	//デバイスリソース
	DX::DeviceResources* m_deviceResources;
	//キーボード
	DirectX::Keyboard* m_keyboard;
	//キーボードステート
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyState;
	//マウス
	DirectX::Mouse* m_mouse;
	//マウスステートトラッカー
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonTracker;
	//スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
 
	//ボタンのポインタ
	std::unique_ptr<Button> m_button;
	//背景画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backGroundTexture;
	//タイトル名画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleBackTexture;
	//フェイド用画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_fadeTexture;

	//背景のサイズ
	DirectX::SimpleMath::Vector2 m_backGroundSize;
	//背景のサイズ
	DirectX::SimpleMath::Vector2 m_titleBackSize;

	//仮作りシーン遷移
	bool m_SceneChange;
	bool m_buttonOpen;

	//時間計測用
	float fadeTimer;
	bool timerSwitch;

	//透明度
	float m_alpha;
	float timer;

	//選択中のボタン
	int buttonindex;

private:
	//背景位置
	DirectX::SimpleMath::Vector3 m_backPosition;
	//タイトル位置
	DirectX::SimpleMath::Vector3 m_titleNamePosition;
	//クリック画像位置
	DirectX::SimpleMath::Vector3 m_clickTexPosition;
	//選択用画像位置
	DirectX::SimpleMath::Vector2 m_selectTexPosition;


public:
	ResultScene(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~ResultScene();

	//初期化
	void Initilize();
	//更新
	void Update( DX::StepTimer& m_stepTimer);
	//描画
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//終了処理
	void Finalize();

public:
	//シーン遷移切り替え関数
	bool GetSceneChanger() { return m_SceneChange; }
	//画像サイズ取得関数
	DirectX::SimpleMath::Vector2 GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);
	//ボタン番号設定
	void SetButtonIndex(int index) { buttonindex = index; }
	//ボタン番号取得
	int GetButtonIndex() { return buttonindex + 1; }

};