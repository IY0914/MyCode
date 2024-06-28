#include "pch.h"
#include "ResultScene.h"

using namespace DirectX;

ResultScene::ResultScene(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse)
	:m_deviceResources(m_pDR),								//�f�o�C�X���\�[�X
	m_SceneChange(false),									//�V�[���J�ڗp
	m_buttonOpen(false),									//�I��p
	m_keyboard(key),										//�L�[�{�[�h
	m_mouse(mouse),											//�}�E�X
	m_backPosition(SimpleMath::Vector3::Zero),				//�w�i�ʒu
	m_titleNamePosition(SimpleMath::Vector3::Zero),			//�^�C�g�����̈ʒu
	fadeTimer(0.0f),
	m_alpha(0.0f),
	timer(0.0f)
{
	//�X�v���C�g�o�b�`�̍쐬
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceResources->GetD3DDeviceContext());
	//�{�^���̍쐬
	m_button = std::make_unique<Button>(m_deviceResources, m_keyboard, m_mouse,0);

	//�L�[�{�[�h�ƃ}�E�X�̃g���b�J�[
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_buttonTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
}

ResultScene::~ResultScene()
{

}

void ResultScene::Initilize()
{
	//�f�o�C�X
	auto device = m_deviceResources->GetD3DDevice();
	//�V�[���J�ڗp�ϐ�������
	m_SceneChange = false;
	//�w�i�̃T�C�Y
	m_backGroundSize = GetTextureSize(
		L"Resources/Texture/GameOver.png",
		m_backGroundTexture);
	//�^�C�g�����̃T�C�Y
	m_titleBackSize = GetTextureSize(
		L"Resources/Texture/BackTitle.png",
		m_titleBackTexture);
	//�t�F�C�h����
	GetTextureSize(
		L"Resources/Texture/fade.png",
		m_fadeTexture);

}

void ResultScene::Update(DX::StepTimer& m_stepTimer)
{
	// �o�ߎ��Ԃ��擾����
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());
	timer += elapsedTime;
	//�L�[�{�[�h�̏�Ԃ��擾
	auto key = m_keyboard->Get().GetState();
	//�L�[�{�[�h�X�e�[�g���X�V
	m_keyState->Update(key);
	auto keyState = m_keyState->GetLastState();
	//�}�E�X�̏�Ԃ��擾
	auto mouse = m_mouse->Get().GetState();
	//�}�E�X�N���b�N���X�V
	m_buttonTracker->Update(mouse);

	//�I����Ԃ���̃t�F�[�h
	if (m_keyState->pressed.Space) m_SceneChange = true;

	//�N���b�N�摜�̓����x�̕ω�
	m_alpha = 1.0f + sinf(timer);

	

	//�{�^���ԍ��̐ݒ�
	SetButtonIndex(m_button->GetButtonNumState());
}


void ResultScene::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	DirectX::SimpleMath::Vector2(0, 0);

	//�`�揈��
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(m_backGroundTexture.Get(), SimpleMath::Vector2::Zero);

	m_spriteBatch->End();

}

void ResultScene::Finalize()
{
	
}

DirectX::SimpleMath::Vector2 ResultScene::GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
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
