#include "pch.h"
#include "PlayerUI.h"
#include "BinaryFile.h"

using namespace DirectX;

const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

PlayerUI::PlayerUI(DX::DeviceResources* m_pDR,DirectX::Keyboard* key, DirectX::Mouse* mouse)
	:m_deviceResources(m_pDR),		//デバイスリソース
	m_SceneChange(false),			//シーン遷移用
	m_keyboard(key),				//キーボード
	m_mouse(mouse),					//マウス
	PlayerUIIndex(0)
{
	//キーステート
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//ユーザーインターフェイスをクリア
	/*m_UI.clear();
	m_baseUI.clear();*/
}

PlayerUI::~PlayerUI()
{
	
}

void PlayerUI::Initialize()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	//画面サイズを取得
	m_windowSize.x = m_deviceResources->GetOutputSize().right;
	m_windowSize.y = m_deviceResources->GetOutputSize().bottom;

	m_states = std::make_unique<DirectX::CommonStates>(device);

	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);

	//スプライトバッチ
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	//描画割合(EXP)
	m_UI[1]->SetRenderRatio(0.5f);
	m_baseUI[1]->SetRenderRatio(1.0f);
}

void PlayerUI::Update()
{
	//キー情報の取得
	auto key = m_keyboard->Get().GetState();
	//キー情報の更新
	m_keyState->Update(key);

}

void PlayerUI::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	
}

void PlayerUI::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
	std::unique_ptr<UserInterface> m_userInterFace = std::make_unique<UserInterface>();

	//UIの情報を作成
	m_userInterFace->Create(
		m_deviceResources,
		path,
		pos, 
		scale,
		anchor
	);
	m_userInterFace->SetWindowSize(m_windowSize.x, m_windowSize.y);

	m_UI.push_back(std::move(m_userInterFace));

	std::unique_ptr<UserInterface> base = std::make_unique<UserInterface>();

	//UIのベース情報を作成
	base->Create(
		m_deviceResources,
		m_baseTexturePath,
		pos,
		scale,
		anchor
	);
	base->SetWindowSize(m_windowSize.x, m_windowSize.y);

	m_baseUI.push_back(std::move(base));
}

//画像サイズ取得関数
DirectX::SimpleMath::Vector2 PlayerUI::GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
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