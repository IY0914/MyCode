#include "pch.h"
#include "UI.h"
#include "UserInterface/UserInterface.h"
#include "UserInterface/BinaryFile.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>


//試験的なアルゴリズムヘッダー
//#include <algorithm>
#include <iostream>

using namespace DirectX;

//画像表示サイズ
const float UI::TEXTURE_SIZE_WIDTH = 100.0f;
const float UI::TEXTURE_SIZE_HEIGHT = 100.0f;

UI::UI()
    : m_windowHeight(0)
    , m_windowWidth(0)
    , m_keyNum(0)
    , m_pDR(nullptr)
    , m_baseTexturePath(nullptr)
    , m_key(nullptr)
    , m_gauge(nullptr)
    , m_base(nullptr)
    , m_keyTexPosition(SimpleMath::Vector2::Zero)
    , m_numberTexPosition(SimpleMath::Vector2::Zero)
{

}

UI::~UI()
{
}

void UI::Initialize(DX::DeviceResources* pDR, int width, int height)
{
    m_pDR = pDR;
    m_windowWidth = width;
    m_windowHeight = height;

    // 鍵画像のロード
    DirectX::CreateWICTextureFromFile(
        m_pDR->GetD3DDevice(),
        L"Resources/Texture/key.png",
        nullptr,
        m_UITex.ReleaseAndGetAddressOf()
    );

    // 数字,記号画像のロード
    DirectX::CreateWICTextureFromFile(
        m_pDR->GetD3DDevice(),
        L"Resources/Texture/number.png",
        nullptr,
        m_NumberTex.ReleaseAndGetAddressOf()
    );

    Add(L"Resources/Texture/key.png"
        , SimpleMath::Vector2(0, 0)
        , SimpleMath::Vector2(1.0f, 1.0f)
        , UserInterface::ANCHOR::TOP_LEFT);

    m_batch = std::make_unique<DirectX::SpriteBatch>(m_pDR->GetD3DDeviceContext());
    
    //位置情報の初期化
    m_keyTexPosition = SimpleMath::Vector2(10.0f, 10.0f);
    //鍵の画像のサイズ + 位置
    m_numberTexPosition = m_keyTexPosition + SimpleMath::Vector2(TEXTURE_SIZE_WIDTH, 0.0f); 
}

void UI::Update(int keynum)
{
    auto keystate = Keyboard::Get().GetState();
    m_tracker.Update(keystate);

    m_keyNum = keynum;
}

void UI::Render()
{
    //数字切り取りサイズ
    RECT rect = { 0,0,0,0 };
    rect.left = (TEXTURE_SIZE_WIDTH * m_keyNum);
    rect.right = (TEXTURE_SIZE_WIDTH * m_keyNum) + 100.0f;
    rect.top = 0.0f;
    rect.bottom = TEXTURE_SIZE_HEIGHT;

    m_batch->Begin();

    //鍵画像の表示
    m_batch->Draw(m_UITex.Get(), m_keyTexPosition);

    //数字画像の表示
    m_batch->Draw(
        m_NumberTex.Get(),
        SimpleMath::Vector2(
            m_numberTexPosition.x + TEXTURE_SIZE_WIDTH,
            m_numberTexPosition.y
        ),
        &rect 
    );

    rect.left = 0.0f;
    rect.right = TEXTURE_SIZE_WIDTH;
    rect.top = TEXTURE_SIZE_HEIGHT;
    rect.bottom += TEXTURE_SIZE_HEIGHT;

    //個数補助表示
    m_batch->Draw(
        m_NumberTex.Get(),
        m_numberTexPosition,
        &rect
    );

    
    m_batch->End();

    //鍵の画像描画
   //m_key->Render();
}

void UI::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, UserInterface::ANCHOR anchor)
{
    //フレームの作成
    m_key = std::make_unique<UserInterface>();
    m_key->Create(m_pDR
        , path
        , position
        , scale
        , anchor);
    m_key->SetWindowSize(m_windowWidth, m_windowHeight);

}