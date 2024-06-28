#include "pch.h"
#include "ResultScene.h"

using namespace DirectX;

ResultScene::ResultScene(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse)
	:m_deviceResources(m_pDR),								//デバイスリソース
	m_SceneChange(false),									//シーン遷移用
	m_buttonOpen(false),									//選択用
	m_keyboard(key),										//キーボード
	m_mouse(mouse),											//マウス
	m_backPosition(SimpleMath::Vector3::Zero),				//背景位置
	m_titleNamePosition(SimpleMath::Vector3::Zero),			//タイトル名の位置
	fadeTimer(0.0f),
	m_alpha(0.0f),
	timer(0.0f)
{
	//スプライトバッチの作成
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceResources->GetD3DDeviceContext());
	//ボタンの作成
	m_button = std::make_unique<Button>(m_deviceResources, m_keyboard, m_mouse,0);

	//キーボードとマウスのトラッカー
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_buttonTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
}

ResultScene::~ResultScene()
{

}

void ResultScene::Initilize()
{
	//デバイス
	auto device = m_deviceResources->GetD3DDevice();
	//シーン遷移用変数初期化
	m_SceneChange = false;
	//背景のサイズ
	m_backGroundSize = GetTextureSize(
		L"Resources/Texture/GameOver.png",
		m_backGroundTexture);
	//タイトル名のサイズ
	m_titleBackSize = GetTextureSize(
		L"Resources/Texture/BackTitle.png",
		m_titleBackTexture);
	//フェイド込み
	GetTextureSize(
		L"Resources/Texture/fade.png",
		m_fadeTexture);

}

void ResultScene::Update(DX::StepTimer& m_stepTimer)
{
	// 経過時間を取得する
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());
	timer += elapsedTime;
	//キーボードの状態を取得
	auto key = m_keyboard->Get().GetState();
	//キーボードステートを更新
	m_keyState->Update(key);
	auto keyState = m_keyState->GetLastState();
	//マウスの状態を取得
	auto mouse = m_mouse->Get().GetState();
	//マウスクリックを更新
	m_buttonTracker->Update(mouse);

	//選択状態からのフェード
	if (m_keyState->pressed.Space) m_SceneChange = true;

	//クリック画像の透明度の変化
	m_alpha = 1.0f + sinf(timer);

	

	//ボタン番号の設定
	SetButtonIndex(m_button->GetButtonNumState());
}


void ResultScene::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	DirectX::SimpleMath::Vector2(0, 0);

	//描画処理
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(m_backGroundTexture.Get(), SimpleMath::Vector2::Zero);

	m_spriteBatch->End();

}

void ResultScene::Finalize()
{
	
}

DirectX::SimpleMath::Vector2 ResultScene::GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
	//画像サイズ
	DirectX::SimpleMath::Vector2 textureSize;
	textureSize = SimpleMath::Vector2::Zero;//画像サイズ取得用
	Microsoft::WRL::ComPtr<ID3D11Resource> m_resource;

	//画像のデータ作成
	DirectX::CreateWICTextureFromFile(
		m_deviceResources->GetD3DDevice(), path,
		m_resource.ReleaseAndGetAddressOf(), texture.ReleaseAndGetAddressOf()
	);

	//背景画像のサイズを取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;	//変換用変数
	DX::ThrowIfFailed(m_resource.As(&tex));			//二次元画像として変換・失敗したら止める
	D3D11_TEXTURE2D_DESC desc;						//サイズ用の変数
	tex->GetDesc(&desc);							//サイズ取得

	//画像のサイズを代入
	textureSize.x = desc.Width;
	textureSize.y = desc.Height;

	return textureSize;
}
