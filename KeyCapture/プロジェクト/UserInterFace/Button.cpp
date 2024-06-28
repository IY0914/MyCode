#include "pch.h"
#include "Button.h"

using namespace DirectX;

Button::Button(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse,int texnum)
	:m_deviceResources(m_pDR),		//デバイスリソース
	m_SceneChange(false),			//シーン遷移用
	m_keyboard(key),				//キーボード
	m_mouse(mouse),					//マウス
	m_buttonNum(ButtonNum::PlayButton),	//ボタンによるシーン状態
	buttonIndex(0),
	texIndex(0),
	texNum(texnum)
{
	//配列のクリア
	m_buttonTexture.clear();
	m_buttonSize.clear();

	//キーステート
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//配列数を確保
	m_buttonTexture.resize(texNum);
	m_buttonSize.resize(texNum);
}

Button::~Button()
{

}

void Button::Initialize()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	//画面サイズを取得
	m_windowSize.x = m_deviceResources->GetOutputSize().right;
	m_windowSize.y = m_deviceResources->GetOutputSize().bottom;

	//サイズ取得
	m_swordTextureSize = GetTextureSize(
		L"Resources/Texture/sword.png",
		m_sword
	);
	m_selectButtonSize = GetTextureSize(
		L"Resources/Texture/SelectButton.png",
		m_selectButtonTex
	);
	m_posX = m_selectPos.x;


	//選択画像の位置
	m_selectPos = SimpleMath::Vector2(
		m_windowSize.x / 2 - m_swordTextureSize.x - 150,
		m_windowSize.y / 2 - m_swordTextureSize.y / 2);

	//スプライトバッチ
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

void Button::Update()
{
	//キー情報の取得
	auto key = m_keyboard->Get().GetState();
	//キー情報の更新
	m_keyState->Update(key);
	//キーでボタンの番号を変更
	if (m_keyState->pressed.Down)
	{
		buttonIndex++;
	}
	if (m_keyState->pressed.Up)
	{
		buttonIndex--;
	}
	//ボタンの制限
	if (buttonIndex > 2)
	{
		buttonIndex = 2;
	}
	if (buttonIndex < 0)
	{
		buttonIndex = 0;
	}
	//選択時のボタンに応じた判定
	if (m_keyState->pressed.Enter)
	{
		switch (buttonIndex)
		{
		case 0:
			m_buttonNum = PlayButton;	//ゲーム画面のボタン
			break;
		case 1:
			m_buttonNum = SettingButton;	//設定画面のボタン
			break;
		case 2:
			m_buttonNum = EndButton;	//ゲーム終了のボタン
			break;
		default:
			break;
		}
	}

	//縦軸移動
	m_selectPos.y = (m_windowSize.y / 2) - (m_swordTextureSize.y / 2) + (buttonIndex * 100);
}

void Button::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//基準位置
	SimpleMath::Vector2 texPos = m_windowSize / 2;

	//描画開始
	m_spriteBatch->Begin();

	//ボタンの描画
	for (int i = 0; i < m_buttonTexture.size(); i++)
	{
		m_spriteBatch->Draw(
			m_buttonTexture[i].Get(),
			SimpleMath::Vector2(
				texPos.x - (m_buttonSize[i].x / 2),
				texPos.y - (m_buttonSize[i].y / 2 - (i * 100))
			)

		);
	}

	//選択用剣描画
	m_spriteBatch->Draw(
		m_sword.Get(),
		m_selectPos,
		nullptr,
		Colors::White,
		0.0f,
		SimpleMath::Vector2::Zero
	);

	//選択用四角形描画
	m_spriteBatch->Draw(
		m_selectButtonTex.Get(),
		SimpleMath::Vector2(
			m_selectPos.x + m_swordTextureSize.x - 15.0f,
			m_selectPos.y + 90),
		nullptr,
		SimpleMath::Vector4(0.0f, 0.0f,0.0f, 0.9f),
		0.0f
	);

	m_spriteBatch->End();
}

//画像サイズ取得関数
DirectX::SimpleMath::Vector2 Button::GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
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

	//画像個数の減少
	texNum--;

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

void Button::AddTexture(const wchar_t* path)
{

	

	//画像サイズ
	DirectX::SimpleMath::Vector2 textureSize;
	textureSize = SimpleMath::Vector2::Zero;//画像サイズ取得用
	Microsoft::WRL::ComPtr<ID3D11Resource> m_resource;

	//画像のデータ作成
	DirectX::CreateWICTextureFromFile(
		m_deviceResources->GetD3DDevice(), path,
		m_resource.ReleaseAndGetAddressOf(), m_buttonTexture[texIndex].ReleaseAndGetAddressOf()
	);

	//背景画像のサイズを取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;	//変換用変数
	DX::ThrowIfFailed(m_resource.As(&tex));			//二次元画像として変換・失敗したら止める
	D3D11_TEXTURE2D_DESC desc;						//サイズ用の変数
	tex->GetDesc(&desc);							//サイズ取得

	//画像のサイズを代入
	textureSize.x = desc.Width;
	textureSize.y = desc.Height;

	m_buttonSize[texIndex] = textureSize;

	//画像個数の増加
	if(texIndex < texNum) texIndex++;


	//return textureSize;
}
