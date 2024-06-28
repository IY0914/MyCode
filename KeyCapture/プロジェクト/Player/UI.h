#pragma once

#include "StepTimer.h"
#include "UserInterface/UserInterface.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

class UI
{
private:
	DX::DeviceResources* m_pDR;

	std::unique_ptr<UserInterface> m_key;
	std::unique_ptr<UserInterface> m_gauge;
	std::unique_ptr<UserInterface> m_base;

	const wchar_t* m_baseTexturePath;

	std::unique_ptr<UserInterface> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	//�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_UITex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NumberTex;
	//�摜�T�C�Y�̌Œ�l
	static const float	TEXTURE_SIZE_WIDTH;
	static const float	TEXTURE_SIZE_HEIGHT;
	//�e�摜�̈ʒu���
	DirectX::SimpleMath::Vector2 m_keyTexPosition;
	DirectX::SimpleMath::Vector2 m_numberTexPosition;

	//���̌��̎󂯎��
	int m_keyNum;

	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_batch;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;
	//�֐�
public:
	UI();
	~UI();

	void Initialize(DX::DeviceResources* pDR, int width, int height);
	void Update(int keynum);
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, UserInterface::ANCHOR anchor);

};