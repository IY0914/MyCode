#pragma once
#include "DeviceResources.h"
#include <vector>

class Button
{
private:
	//�I�𒆂̃{�^��
	enum ButtonNum
	{
		PlayButton,
		SettingButton,
		EndButton
	};

private:
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	//�L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	//�L�[�{�[�h�X�e�[�g
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyState;
	//�}�E�X
	DirectX::Mouse* m_mouse;
	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//�摜�ۑ��p�ϒ��z��
	std::vector <Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_buttonTexture;
	//�ۑ������摜�̃T�C�Y�z��
	std::vector<DirectX::SimpleMath::Vector2> m_buttonSize;

	//���摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_sword;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectButtonTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_startButtonTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_settingButtonTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_quitButtonTex;
	
	//���摜�T�C�Y
	DirectX::SimpleMath::Vector2 m_swordTextureSize;
	//�摜�T�C�Y
	DirectX::SimpleMath::Vector2 m_selectButtonSize;
	//�e�{�^���T�C�Y
	DirectX::SimpleMath::Vector2 m_startButtonSize;
	DirectX::SimpleMath::Vector2 m_settingButtonSize;
	DirectX::SimpleMath::Vector2 m_quitButtonSize;

	//�摜��
	int texNum;
	int texIndex;

private:

	//��ʃT�C�Y��
	DirectX::SimpleMath::Vector2 m_windowSize;

	//�{�^�����d������ϐ�
	ButtonNum m_buttonNum;
	int buttonIndex;
	//�I�����̈ʒu
	DirectX::SimpleMath::Vector2 m_selectPos;
	float m_posX;
	//���Z����
	float timer;

	//�����V�[���J��
	bool m_SceneChange;

public:
	Button(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse,int texNum);
	~Button();

	//�摜�ݒ�
	void Initialize();
	//�X�V�֐�
	void Update();
	//�`��֐�
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//�摜�T�C�Y�擾�֐�
	DirectX::SimpleMath::Vector2 GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);

public:
	//�V�[���J�ڊǗ��̊֐�
	bool GetSceneChanger() { return m_SceneChange; }

	//�{�^���̏�Ԃ�Ԃ��֐�
	ButtonNum GetButtonNumState() { return m_buttonNum; }

	//�I���{�^���ʒu��Ԃ��֐�
	DirectX::SimpleMath::Vector2 GetSelectButtonPos() { return m_selectPos; }

	//�摜�T�C�Y�擾�֐�
	void SetSelectTextureSize(DirectX::SimpleMath::Vector2 size) { m_swordTextureSize = size; }

public:
	//�摜��ǉ�
	void AddTexture(const wchar_t* path);
};