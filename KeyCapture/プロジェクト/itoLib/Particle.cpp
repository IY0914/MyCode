//--------------------------------------------------------------------------------------
// File: Particle.h
//
// パーティクルクラス
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Particle.h"

#include "UserInterface/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

using namespace DirectX;

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> tito::Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3)+ sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// コンストラクタ
/// </summary>
tito::Particle::Particle()
	:m_pDR(nullptr)
	,m_position(SimpleMath::Vector3::Zero)
	,transparent(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
tito::Particle::~Particle()
{
}

/// <summary>
/// テクスチャリソース読み込み関数
/// </summary>
/// <param name="path">相対パス(Resources/Textures/・・・.pngなど）</param>
void tito::Particle::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	
	m_texture.push_back(texture);
}

/// <summary>
/// 生成関数
/// </summary>
/// <param name="pDR">ユーザーリソース等から持ってくる</param>
void tito::Particle::Create(DX::DeviceResources* pDR, DirectX::SimpleMath::Vector3 pos)
{	
	m_pDR = pDR;
	auto device = pDR->GetD3DDevice();

	m_position = pos;

	//シェーダーの作成
	CreateShader();

	//画像の読み込み（２枚ともデフォルトは読み込み失敗でnullptr)
	LoadTexture(L"Resources/Texture/Luster.png");
	//LoadTexture(L"Resources/Textures/ball.png");

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

}

void tito::Particle::Update(float time)
{
	//透明度を変更
	transparent = time;
}

/// <summary>
/// Shader作成部分だけ分離した関数
/// </summary>
void tito::Particle::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/ParticlePS.cso");

	//インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void tito::Particle::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Matrix world)
{
	auto context = m_pDR->GetD3DDeviceContext();
		// 頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionColorTexture vertex[4] =
		{
			VertexPositionColorTexture(m_position + SimpleMath::Vector3(cos(transparent),3.5f,sin(-transparent)),SimpleMath::Vector4(1,1,1,transparent),SimpleMath::Vector2(0.0f, 0.0f)),
			//VertexPositionColorTexture(SimpleMath::Vector3(-0.5f, -0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 1.0f)),
			//VertexPositionColorTexture(SimpleMath::Vector3( 0.5f, -0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(1.0f, 1.0f)),
			//VertexPositionColorTexture(SimpleMath::Vector3( 0.5f,  0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(1.0f, 0.0f)),
		};

		//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
		ConstBuffer cbuff;
		cbuff.matView = view.Transpose();
		cbuff.matProj = proj.Transpose();
		cbuff.matWorld = world.Transpose();
		cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, sinf(transparent));

		//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
		context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

		//シェーダーにバッファを渡す
		ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
		context->VSSetConstantBuffers(0, 1, cb);
		context->GSSetConstantBuffers(0, 1, cb);
		context->PSSetConstantBuffers(0, 1, cb);

		//画像用サンプラーの登録
		ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
		context->PSSetSamplers(0, 1, sampler);

		//半透明描画指定
		ID3D11BlendState* blendstate = m_states->NonPremultiplied();

		// 透明判定処理
		context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

		// 深度バッファに書き込み参照する
		context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

		// カリングは左周り
		context->RSSetState(m_states->CullNone());

		//シェーダをセットする
		context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
		context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		//ピクセルシェーダにテクスチャを登録する。
		//context->PSSetShaderResources(0, 1, m_texture[0].GetAddressOf());
		//context->PSSetShaderResources(1, 1, m_texture[1].GetAddressOf());
		for (int i = 0; i < m_texture.size(); i++)
		{
			context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
		}

		//インプットレイアウトの登録
		context->IASetInputLayout(m_inputLayout.Get());

		// 板ポリゴンを描画
		m_batch->Begin();
		m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
		m_batch->End();

		//シェーダの登録を解除しておく
		context->VSSetShader(nullptr, nullptr, 0);
		context->GSSetShader(nullptr, nullptr, 0);
		context->PSSetShader(nullptr, nullptr, 0);

}
