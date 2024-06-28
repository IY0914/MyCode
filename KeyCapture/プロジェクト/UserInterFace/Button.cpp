#include "pch.h"
#include "Button.h"

using namespace DirectX;

Button::Button(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse,int texnum)
	:m_deviceResources(m_pDR),		//�f�o�C�X���\�[�X
	m_SceneChange(false),			//�V�[���J�ڗp
	m_keyboard(key),				//�L�[�{�[�h
	m_mouse(mouse),					//�}�E�X
	m_buttonNum(ButtonNum::PlayButton),	//�{�^���ɂ��V�[�����
	buttonIndex(0),
	texIndex(0),
	texNum(texnum)
{
	//�z��̃N���A
	m_buttonTexture.clear();
	m_buttonSize.clear();

	//�L�[�X�e�[�g
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//�z�񐔂��m��
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

	//��ʃT�C�Y���擾
	m_windowSize.x = m_deviceResources->GetOutputSize().right;
	m_windowSize.y = m_deviceResources->GetOutputSize().bottom;

	//�T�C�Y�擾
	m_swordTextureSize = GetTextureSize(
		L"Resources/Texture/sword.png",
		m_sword
	);
	m_selectButtonSize = GetTextureSize(
		L"Resources/Texture/SelectButton.png",
		m_selectButtonTex
	);
	m_posX = m_selectPos.x;


	//�I���摜�̈ʒu
	m_selectPos = SimpleMath::Vector2(
		m_windowSize.x / 2 - m_swordTextureSize.x - 150,
		m_windowSize.y / 2 - m_swordTextureSize.y / 2);

	//�X�v���C�g�o�b�`
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

void Button::Update()
{
	//�L�[���̎擾
	auto key = m_keyboard->Get().GetState();
	//�L�[���̍X�V
	m_keyState->Update(key);
	//�L�[�Ń{�^���̔ԍ���ύX
	if (m_keyState->pressed.Down)
	{
		buttonIndex++;
	}
	if (m_keyState->pressed.Up)
	{
		buttonIndex--;
	}
	//�{�^���̐���
	if (buttonIndex > 2)
	{
		buttonIndex = 2;
	}
	if (buttonIndex < 0)
	{
		buttonIndex = 0;
	}
	//�I�����̃{�^���ɉ���������
	if (m_keyState->pressed.Enter)
	{
		switch (buttonIndex)
		{
		case 0:
			m_buttonNum = PlayButton;	//�Q�[����ʂ̃{�^��
			break;
		case 1:
			m_buttonNum = SettingButton;	//�ݒ��ʂ̃{�^��
			break;
		case 2:
			m_buttonNum = EndButton;	//�Q�[���I���̃{�^��
			break;
		default:
			break;
		}
	}

	//�c���ړ�
	m_selectPos.y = (m_windowSize.y / 2) - (m_swordTextureSize.y / 2) + (buttonIndex * 100);
}

void Button::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//��ʒu
	SimpleMath::Vector2 texPos = m_windowSize / 2;

	//�`��J�n
	m_spriteBatch->Begin();

	//�{�^���̕`��
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

	//�I��p���`��
	m_spriteBatch->Draw(
		m_sword.Get(),
		m_selectPos,
		nullptr,
		Colors::White,
		0.0f,
		SimpleMath::Vector2::Zero
	);

	//�I��p�l�p�`�`��
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

//�摜�T�C�Y�擾�֐�
DirectX::SimpleMath::Vector2 Button::GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
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

	//�摜���̌���
	texNum--;

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

void Button::AddTexture(const wchar_t* path)
{

	

	//�摜�T�C�Y
	DirectX::SimpleMath::Vector2 textureSize;
	textureSize = SimpleMath::Vector2::Zero;//�摜�T�C�Y�擾�p
	Microsoft::WRL::ComPtr<ID3D11Resource> m_resource;

	//�摜�̃f�[�^�쐬
	DirectX::CreateWICTextureFromFile(
		m_deviceResources->GetD3DDevice(), path,
		m_resource.ReleaseAndGetAddressOf(), m_buttonTexture[texIndex].ReleaseAndGetAddressOf()
	);

	//�w�i�摜�̃T�C�Y���擾
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;	//�ϊ��p�ϐ�
	DX::ThrowIfFailed(m_resource.As(&tex));			//�񎟌��摜�Ƃ��ĕϊ��E���s������~�߂�
	D3D11_TEXTURE2D_DESC desc;						//�T�C�Y�p�̕ϐ�
	tex->GetDesc(&desc);							//�T�C�Y�擾

	//�摜�̃T�C�Y����
	textureSize.x = desc.Width;
	textureSize.y = desc.Height;

	m_buttonSize[texIndex] = textureSize;

	//�摜���̑���
	if(texIndex < texNum) texIndex++;


	//return textureSize;
}
