#pragma once
#include "DeviceResources.h"
#include "UserInterFace/Button.h"

class TitleScene
{
private:
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	//�L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	//�L�[�{�[�h�X�e�[�g
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyState;
	//�}�E�X
	DirectX::Mouse* m_mouse;
	//�}�E�X�X�e�[�g�g���b�J�[
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonTracker;
	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
 
	//�{�^���̃|�C���^
	std::unique_ptr<Button> m_button;
	//�w�i�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backGroundTexture;
	//�^�C�g�����摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleNameTexture;
	//�t�F�C�h�p�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_fadeTexture;
	//�I��p�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectTexture;

	//�I�����摜(����k��)
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_selectButtonTexture;

	//�w�i�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_backGroundSize;
	//�w�i�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_titleNameSize;
	//�N���b�N�摜�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_clickTexSize;
	//�I��p�摜�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_selectTexSize;

	//�����V�[���J��
	bool m_SceneChange;
	//�c���ۑ��p
	float namePosY;

	//���Ԍv���p
	float fadeTimer;
	bool timerSwitch;

	//�����x
	float m_alpha;
	float timer;

	//�I�𒆂̃{�^��
	int buttonindex;

private:
	//�摜�̃p�X
	const wchar_t* texPath[3] =
	{ L"Resources/Texture/StartButton.png",
	  L"Resources/Texture/Setting.png",
	  L"Resources/Texture/quit.png" };

private:
	//�w�i�ʒu
	DirectX::SimpleMath::Vector3 m_backPosition;
	//�^�C�g���ʒu
	DirectX::SimpleMath::Vector3 m_titleNamePosition;
	//�N���b�N�摜�ʒu
	DirectX::SimpleMath::Vector3 m_clickTexPosition;
	//�I��p�摜�ʒu
	DirectX::SimpleMath::Vector2 m_selectTexPosition;


public:
	TitleScene(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~TitleScene();

	//������
	void Initilize();
	//�X�V
	void Update( DX::StepTimer& m_stepTimer);
	//�`��
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//�I������
	void Finalize();

public:
	//�V�[���J�ڐ؂�ւ��֐�
	bool GetSceneChanger() { return m_SceneChange; }
	//�摜�T�C�Y�擾�֐�
	DirectX::SimpleMath::Vector2 GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);
	//�{�^���ԍ��ݒ�
	void SetButtonIndex(int index) { buttonindex = index; }
	//�{�^���ԍ��擾
	int GetButtonIndex() { return buttonindex; }

};