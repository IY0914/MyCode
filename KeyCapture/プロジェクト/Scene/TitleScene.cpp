#include "pch.h"
#include "TitleScene.h"

using namespace DirectX;
#define MOVE_HEIGHT_TITLENAME (10)

TitleScene::TitleScene(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse)
	:m_deviceResources(m_pDR),								//デバイスリソース
	m_SceneChange(false),									//シーン遷移用
	m_keyboard(key),										//キーボード
	m_mouse(mouse),											//マウス
	m_backPosition(SimpleMath::Vector3::Zero),				//背景位置
	m_titleNamePosition(SimpleMath::Vector3::Zero),			//タイトル名の位置
	fadeTimer(0.0f),
	m_alpha(0.0f),
	timer(0.0f)
{
	//キーステートの作成
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	//スプライトバッチの作成
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceResources->GetD3DDeviceContext());

	//必要画像数
	const int texNum = sizeof(texPath) / sizeof(const wchar_t*);
	//ボタンの作成
	m_button = std::make_unique<Button>(m_deviceResources, m_keyboard, m_mouse, texNum);

	//キーボードとマウスのトラッカー
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_buttonTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	//画像の追加
	for (int i = 0; i < texNum; i++)
	{
		m_button->AddTexture(texPath[i]);
	}
}

TitleScene::~TitleScene()
{

}

void TitleScene::Initilize()
{
	//デバイス
	auto device = m_deviceResources->GetD3DDevice();
	//シーン遷移用変数初期化
	m_SceneChange = false;
	//フェード用変数の初期化
	timerSwitch = false;

	//フェイド時間の初期化
	fadeTimer = 0.0f;

	//背景のサイズ
	m_backGroundSize = GetTextureSize(
		L"Resources/Texture/TitleBackGround.png",
		m_backGroundTexture);
	//タイトル名のサイズ
	m_titleNameSize = GetTextureSize(
		L"Resources/Texture/Logo2.png",
		m_titleNameTexture);
	//フェイド用画像読み込み
	GetTextureSize(
		L"Resources/Texture/fade.png",
		m_fadeTexture);

	//タイトルの場所計算
	m_titleNamePosition.x = (m_backGroundSize.x / 2.0) - (m_titleNameSize.x / 2.0);
	m_titleNamePosition.y = m_backGroundSize.y * 0.1f;
	//初期位置
	namePosY = m_titleNamePosition.y;

	//選択画像位置計算
	m_selectTexPosition = m_button->GetSelectButtonPos() - m_selectTexSize;

	//ボタンの初期化
	m_button->Initialize();
	
}

void TitleScene::Update(DX::StepTimer& m_stepTimer)
{
	// 経過時間を取得する
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());
	timer += elapsedTime;
	//キーボードの状態を取得
	auto key = m_keyboard->Get().GetState();
	//キーボードステートの取得
	m_keyState->Update(key);
	//マウスの状態を取得
	auto mouse = m_mouse->Get().GetState();
	//マウスクリックを更新
	m_buttonTracker->Update(mouse);

	//タイトルを縦に少し動かす
	m_titleNamePosition = SimpleMath::Vector3::Lerp(
		DirectX::SimpleMath::Vector3(m_titleNamePosition.x,namePosY + MOVE_HEIGHT_TITLENAME,m_titleNamePosition.z),
		DirectX::SimpleMath::Vector3(m_titleNamePosition.x, namePosY - MOVE_HEIGHT_TITLENAME, m_titleNamePosition.z),
		sin(timer));

	//選択状態からのフェード
	if (m_keyState->pressed.Enter) timerSwitch = true;

	//フェードする時間で非透明度を加算
	if (timerSwitch == true) fadeTimer += elapsedTime;
	//シーン遷移する
	if (fadeTimer > 1.0f)	m_SceneChange = true;
	
	//クリック画像の透明度の変化
	m_alpha = 1.0f + sinf(timer);

	//ボタンの更新
	m_button->Update();

	//ボタン番号の設定
	SetButtonIndex(m_button->GetButtonNumState());
}


void TitleScene::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{

	//描画開始
	m_spriteBatch->Begin();


	//背景描画
	m_spriteBatch->Draw(
		m_backGroundTexture.Get(),
		m_backPosition
	);

	//タイトル名描画
	m_spriteBatch->Draw(
		m_titleNameTexture.Get(),
		m_titleNamePosition,
		nullptr,Colors::White,
		0.0f,
		SimpleMath::Vector2::Zero,
		SimpleMath::Vector2(1.0,1.0f)
	);

	//描画終了
	m_spriteBatch->End();

	//ボタンの描画
	m_button->Render(view, proj);

	m_spriteBatch->Begin();

	//描画開始
	//フェイドの描画
	m_spriteBatch->Draw(
		m_fadeTexture.Get(),
		SimpleMath::Vector2::Zero,
		nullptr,
		SimpleMath::Vector4(0.0f, 0.0f, 0.0f, fadeTimer),
		0.0f
	);

	//描画終了
	m_spriteBatch->End();
}

void TitleScene::Finalize()
{
	
}

DirectX::SimpleMath::Vector2 TitleScene::GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
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
