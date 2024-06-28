#pragma once
#include "DeviceResources.h"
#include "UserInterface.h"
#include <vector>

class PlayerUI
{

private:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	windowSize;
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
	//���ʃX�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;


	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//�R���X�g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constBuffer;

	//�摜
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textrue;
	//�摜�T�C�Y
	std::vector<DirectX::SimpleMath::Vector2> m_textureSize;

	//���摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_sword;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectPlayerUITex;
	//���摜�T�C�Y
	DirectX::SimpleMath::Vector2 m_swordTextureSize;
	//�摜�T�C�Y
	DirectX::SimpleMath::Vector2 m_selectPlayerUISize;

private:

	//��ʃT�C�Y��
	DirectX::SimpleMath::Vector2 m_windowSize;
	int PlayerUIIndex;
	//�I�����̈ʒu
	DirectX::SimpleMath::Vector2 m_selectPos;
	float m_posX;
	//���Z����
	float timer;

	//�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_baseTextures;
	const wchar_t* m_baseTexturePath;

	//���[�U�[�C���^�[�t�F�C�X
	std::vector<std::unique_ptr<UserInterface>> m_UI;
	std::vector<std::unique_ptr<UserInterface>> m_baseUI;

	//�s��
	DirectX::SimpleMath::Matrix m_world;
	//�����V�[���J��
	bool m_SceneChange;

public:
	//�C���v�b�g���C�A�E�g�̒萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	PlayerUI(DX::DeviceResources* m_pDR, DirectX::Keyboard* key, DirectX::Mouse* mouse);
	~PlayerUI();

	//�摜�ݒ�
	void Initialize();
	//�X�V�֐�
	void Update();
	//�`��֐�
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	//�{�^���ǉ��֐�
	void Add(const wchar_t* path, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor);
	//�摜�T�C�Y�擾�֐�
	DirectX::SimpleMath::Vector2 GetTextureSize(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);

public:
	
	//�摜�T�C�Y�擾�֐�
	void SetSelectTextureSize(DirectX::SimpleMath::Vector2 size) { m_swordTextureSize = size; }

	void SetHPGauge(float playerHp) { m_UI[0]->SetRenderRatio(playerHp); }

};