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
	:m_deviceResources(m_pDR),		//�f�o�C�X���\�[�X
	m_SceneChange(false),			//�V�[���J�ڗp
	m_keyboard(key),				//�L�[�{�[�h
	m_mouse(mouse),					//�}�E�X
	PlayerUIIndex(0)
{
	//�L�[�X�e�[�g
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//���[�U�[�C���^�[�t�F�C�X���N���A
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

	//��ʃT�C�Y���擾
	m_windowSize.x = m_deviceResources->GetOutputSize().right;
	m_windowSize.y = m_deviceResources->GetOutputSize().bottom;

	m_states = std::make_unique<DirectX::CommonStates>(device);

	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);

	//�X�v���C�g�o�b�`
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	//�`�抄��(EXP)
	m_UI[1]->SetRenderRatio(0.5f);
	m_baseUI[1]->SetRenderRatio(1.0f);
}

void PlayerUI::Update()
{
	//�L�[���̎擾
	auto key = m_keyboard->Get().GetState();
	//�L�[���̍X�V
	m_keyState->Update(key);

}

void PlayerUI::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	
}

void PlayerUI::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
	std::unique_ptr<UserInterface> m_userInterFace = std::make_unique<UserInterface>();

	//UI�̏����쐬
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

	//UI�̃x�[�X�����쐬
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

//�摜�T�C�Y�擾�֐�
DirectX::SimpleMath::Vector2 PlayerUI::GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
	//�摜�T�C�Y
	DirectX::SimpleMath::Vector2 textureSize;
	textureSize = SimpleMath::Vector2::Zero;//�摜�T�C�Y�擾�p
	Microsoft::WRL::ComPtr<ID3D11Resource> m_resource;

	//�摜�̃f�[�^�쐬
	DirectX::CreateWICTextureFromFile(
		m_deviceResources->GetD3DDevice(), path,
		m_resource.ReleaseAndGetAddressOf(), texture.ReleaseAndGetAddressOf()
	);

	//�w�i�摜�̃T�C�Y���擾
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;	//�ϊ��p�ϐ�
	DX::ThrowIfFailed(m_resource.As(&tex));			//�񎟌��摜�Ƃ��ĕϊ��E���s������~�߂�
	D3D11_TEXTURE2D_DESC desc;						//�T�C�Y�p�̕ϐ�
	tex->GetDesc(&desc);							//�T�C�Y�擾

	//�摜�̃T�C�Y����
	textureSize.x = desc.Width;
	textureSize.y = desc.Height;

	return textureSize;
}