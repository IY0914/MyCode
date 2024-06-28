#pragma once
#include "DeviceResources.h"
#include <vector>

class Button
{
private:
	//選択中のボタン
	enum ButtonNum
	{
		PlayButton,
		SettingButton,
		EndButton
	};

private:
	//デバイスリソース
	DX::DeviceResources* m_deviceResources;
	//キーボード
	DirectX::Keyboard* m_keyboard;
	//キーボードステート
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyState;
	//マウス
	DirectX::Mouse* m_mouse;
	//スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//画像保存用可変長配列
	std::vector <Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_buttonTexture;
	//保存した画像のサイズ配列
	std::vector<DirectX::SimpleMath::Vector2> m_buttonSize;

	//剣画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_sword;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectButtonTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_startButtonTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_settingButtonTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_quitButtonTex;
	
	//剣画像サイズ
	DirectX::SimpleMath::Vector2 m_swordTextureSize;
	//画像サイズ
	DirectX::SimpleMath::Vector2 m_selectButtonSize;
	//各ボタンサイズ
	DirectX::SimpleMath::Vector2 m_startButtonSize;
	DirectX::SimpleMath::Vector2 m_settingButtonSize;
	DirectX::SimpleMath::Vector2 m_quitButtonSize;

	//画像数
	int texNum;
	int texIndex;

private:

	//画面サイズを
	DirectX::SimpleMath::Vector2 m_windowSize;

	//ボタンを仕分ける変数
	ButtonNum m_buttonNum;
	int buttonIndex;
	//選択肢の位置
	DirectX::SimpleMath::Vector2 m_selectPos;
	float m_posX;
	//加算時間
	float timer;

	//仮作りシーン遷移
	bool m_SceneChange;

public:
	Button(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse,int texNum);
	~Button();

	//画像設定
	void Initialize();
	//更新関数
	void Update();
	//描画関数
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//画像サイズ取得関数
	DirectX::SimpleMath::Vector2 GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);

public:
	//シーン遷移管理の関数
	bool GetSceneChanger() { return m_SceneChange; }

	//ボタンの状態を返す関数
	ButtonNum GetButtonNumState() { return m_buttonNum; }

	//選択ボタン位置を返す関数
	DirectX::SimpleMath::Vector2 GetSelectButtonPos() { return m_selectPos; }

	//画像サイズ取得関数
	void SetSelectTextureSize(DirectX::SimpleMath::Vector2 size) { m_swordTextureSize = size; }

public:
	//画像を追加
	void AddTexture(const wchar_t* path);
};