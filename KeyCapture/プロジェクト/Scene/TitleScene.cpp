#include "pch.h"
#include "TitleScene.h"

using namespace DirectX;
#define MOVE_HEIGHT_TITLENAME (10)

TitleScene::TitleScene(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse)
	:m_deviceResources(m_pDR),								//�f�o�C�X���\�[�X
	m_SceneChange(false),									//�V�[���J�ڗp
	m_keyboard(key),										//�L�[�{�[�h
	m_mouse(mouse),											//�}�E�X
	m_backPosition(SimpleMath::Vector3::Zero),				//�w�i�ʒu
	m_titleNamePosition(SimpleMath::Vector3::Zero),			//�^�C�g�����̈ʒu
	fadeTimer(0.0f),
	m_alpha(0.0f),
	timer(0.0f)
{
	//�L�[�X�e�[�g�̍쐬
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	//�X�v���C�g�o�b�`�̍쐬
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceResources->GetD3DDeviceContext());

	//�K�v�摜��
	const int texNum = sizeof(texPath) / sizeof(const wchar_t*);
	//�{�^���̍쐬
	m_button = std::make_unique<Button>(m_deviceResources, m_keyboard, m_mouse, texNum);

	//�L�[�{�[�h�ƃ}�E�X�̃g���b�J�[
	m_keyState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_buttonTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	//�摜�̒ǉ�
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
	//�f�o�C�X
	auto device = m_deviceResources->GetD3DDevice();
	//�V�[���J�ڗp�ϐ�������
	m_SceneChange = false;
	//�t�F�[�h�p�ϐ��̏�����
	timerSwitch = false;

	//�t�F�C�h���Ԃ̏�����
	fadeTimer = 0.0f;

	//�w�i�̃T�C�Y
	m_backGroundSize = GetTextureSize(
		L"Resources/Texture/TitleBackGround.png",
		m_backGroundTexture);
	//�^�C�g�����̃T�C�Y
	m_titleNameSize = GetTextureSize(
		L"Resources/Texture/Logo2.png",
		m_titleNameTexture);
	//�t�F�C�h�p�摜�ǂݍ���
	GetTextureSize(
		L"Resources/Texture/fade.png",
		m_fadeTexture);

	//�^�C�g���̏ꏊ�v�Z
	m_titleNamePosition.x = (m_backGroundSize.x / 2.0) - (m_titleNameSize.x / 2.0);
	m_titleNamePosition.y = m_backGroundSize.y * 0.1f;
	//�����ʒu
	namePosY = m_titleNamePosition.y;

	//�I���摜�ʒu�v�Z
	m_selectTexPosition = m_button->GetSelectButtonPos() - m_selectTexSize;

	//�{�^���̏�����
	m_button->Initialize();
	
}

void TitleScene::Update(DX::StepTimer& m_stepTimer)
{
	// �o�ߎ��Ԃ��擾����
	float elapsedTime = float(m_stepTimer.GetElapsedSeconds());
	timer += elapsedTime;
	//�L�[�{�[�h�̏�Ԃ��擾
	auto key = m_keyboard->Get().GetState();
	//�L�[�{�[�h�X�e�[�g�̎擾
	m_keyState->Update(key);
	//�}�E�X�̏�Ԃ��擾
	auto mouse = m_mouse->Get().GetState();
	//�}�E�X�N���b�N���X�V
	m_buttonTracker->Update(mouse);

	//�^�C�g�����c�ɏ���������
	m_titleNamePosition = SimpleMath::Vector3::Lerp(
		DirectX::SimpleMath::Vector3(m_titleNamePosition.x,namePosY + MOVE_HEIGHT_TITLENAME,m_titleNamePosition.z),
		DirectX::SimpleMath::Vector3(m_titleNamePosition.x, namePosY - MOVE_HEIGHT_TITLENAME, m_titleNamePosition.z),
		sin(timer));

	//�I����Ԃ���̃t�F�[�h
	if (m_keyState->pressed.Enter) timerSwitch = true;

	//�t�F�[�h���鎞�ԂŔ񓧖��x�����Z
	if (timerSwitch == true) fadeTimer += elapsedTime;
	//�V�[���J�ڂ���
	if (fadeTimer > 1.0f)	m_SceneChange = true;
	
	//�N���b�N�摜�̓����x�̕ω�
	m_alpha = 1.0f + sinf(timer);

	//�{�^���̍X�V
	m_button->Update();

	//�{�^���ԍ��̐ݒ�
	SetButtonIndex(m_button->GetButtonNumState());
}


void TitleScene::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{

	//�`��J�n
	m_spriteBatch->Begin();


	//�w�i�`��
	m_spriteBatch->Draw(
		m_backGroundTexture.Get(),
		m_backPosition
	);

	//�^�C�g�����`��
	m_spriteBatch->Draw(
		m_titleNameTexture.Get(),
		m_titleNamePosition,
		nullptr,Colors::White,
		0.0f,
		SimpleMath::Vector2::Zero,
		SimpleMath::Vector2(1.0,1.0f)
	);

	//�`��I��
	m_spriteBatch->End();

	//�{�^���̕`��
	m_button->Render(view, proj);

	m_spriteBatch->Begin();

	//�`��J�n
	//�t�F�C�h�̕`��
	m_spriteBatch->Draw(
		m_fadeTexture.Get(),
		SimpleMath::Vector2::Zero,
		nullptr,
		SimpleMath::Vector4(0.0f, 0.0f, 0.0f, fadeTimer),
		0.0f
	);

	//�`��I��
	m_spriteBatch->End();
}

void TitleScene::Finalize()
{
	
}

DirectX::SimpleMath::Vector2 TitleScene::GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
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
