#pragma once
#include "DeviceResources.h"
#include "UserInterface.h"
#include <vector>

class PlayerUI
{

private:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	windowSize;
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
	//共通ステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;


	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//コンストバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constBuffer;

	//画像
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textrue;
	//画像サイズ
	std::vector<DirectX::SimpleMath::Vector2> m_textureSize;

	//剣画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_sword;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectPlayerUITex;
	//剣画像サイズ
	DirectX::SimpleMath::Vector2 m_swordTextureSize;
	//画像サイズ
	DirectX::SimpleMath::Vector2 m_selectPlayerUISize;

private:

	//画面サイズを
	DirectX::SimpleMath::Vector2 m_windowSize;
	int PlayerUIIndex;
	//選択肢の位置
	DirectX::SimpleMath::Vector2 m_selectPos;
	float m_posX;
	//加算時間
	float timer;

	//画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_baseTextures;
	const wchar_t* m_baseTexturePath;

	//ユーザーインターフェイス
	std::vector<std::unique_ptr<UserInterface>> m_UI;
	std::vector<std::unique_ptr<UserInterface>> m_baseUI;

	//行列
	DirectX::SimpleMath::Matrix m_world;
	//仮作りシーン遷移
	bool m_SceneChange;

public:
	//インプットレイアウトの定数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	PlayerUI(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~PlayerUI();

	//画像設定
	void Initialize();
	//更新関数
	void Update();
	//描画関数
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//ボタン追加関数
	void Add(const wchar_t* path, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor);
	//画像サイズ取得関数
	DirectX::SimpleMath::Vector2 GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);

public:
	
	//画像サイズ取得関数
	void SetSelectTextureSize(DirectX::SimpleMath::Vector2 size) { m_swordTextureSize = size; }

	void SetHPGauge(float playerHp) { m_UI[0]->SetRenderRatio(playerHp); }

};