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

	//画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_UITex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_NumberTex;
	//画像サイズの固定値
	static const float	TEXTURE_SIZE_WIDTH;
	static const float	TEXTURE_SIZE_HEIGHT;
	//各画像の位置情報
	DirectX::SimpleMath::Vector2 m_keyTexPosition;
	DirectX::SimpleMath::Vector2 m_numberTexPosition;

	//鍵の個数の受け取り
	int m_keyNum;

	//スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_batch;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;
	//関数
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