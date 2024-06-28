#include "pch.h"
#include "Scene/PlayScene.h"
#include "BaseClass/Object.h"
#include "UserInterFace/BinaryFile.h"

using namespace DirectX;

// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Object::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Object::Object(DX::DeviceResources* deviceResource, DirectX::Model* model, const wchar_t* effectPath)// DirectX::Model* model)
	: m_deviceResources(deviceResource)
	, m_position(0.0f)
	, m_model(model)
	, m_popItem(true)
	, m_objectNum(0)
	, m_rorate(0.0f)
	, m_path(effectPath)
{	
	//シェーダーの作成
	CreateShader();

}

Object::~Object()
{
	
}

void Object::Initilize(DirectX::SimpleMath::Vector3 pos)
{
	//ステートオブジェクトの作成
	m_state = std::make_unique<DirectX::CommonStates>(m_deviceResources->GetD3DDevice());
	//プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<VertexPositionColorTexture>>(
		m_deviceResources->GetD3DDeviceContext());
	//ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_deviceResources->GetD3DDevice());
	m_basicEffect->SetProjection(Camera::GetInstance()->GetProjection());
	m_basicEffect->SetVertexColorEnabled(true);

	//画像の読み込み
	DirectX::CreateWICTextureFromFile(
		m_deviceResources->GetD3DDevice(),
		m_path,
		nullptr,
		m_tex.ReleaseAndGetAddressOf()
	);

	//シェーダーの作成
	CreateShader();

	//アイテムの出現
	m_popItem = true;

	//位置の情報
	m_position = pos;

	
	m_particle = std::make_unique<tito::Particle>();
	m_particle->Create(m_deviceResources, m_position);

}

void Object::Update(const DX::StepTimer& timer)
{
	//回転の加算
	m_rorate = timer.GetTotalSeconds();

	m_particle->Update(timer.GetTotalSeconds());

	//取得したら描画消し
	if (Collision::GetInstance()->GetItemCollider(m_objectNum))
	{
		m_popItem = false;
	}
}

void Object::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	//行列
	SimpleMath::Matrix world;
	SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_rorate);
	//行列計算
	world = rotY * trans;

	//モデル描画
	if(m_popItem)m_model->Draw(context, *m_state, world, view, proj);

	/*------------------------------シェーダーの描画関係-------------------------------------*/
	/*
	// 頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(SimpleMath::Vector3(-0.5f,  0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionColorTexture(SimpleMath::Vector3(0.5f,  0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionColorTexture(SimpleMath::Vector3(-0.5f, -0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 1.0f)),
		VertexPositionColorTexture(SimpleMath::Vector3(0.5f, -0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(1.0f, 1.0f)),
	};

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.view = view.Transpose();
	cbuff.proj = proj.Transpose();
	cbuff.world = world.Transpose();
	//cbuff.transParent = 1.0f;

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_state->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = m_state->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_state->DepthDefault(), 0);

	// カリングは左周り
	context->RSSetState(m_state->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_objectVS.Get(), nullptr, 0);
	context->GSSetShader(m_objectGS.Get(), nullptr, 0);
	context->PSSetShader(m_objectPS.Get(), nullptr, 0);

	//ピクセルシェーダーに画像を設定する
	context->PSSetShaderResources(0, 1, m_tex.GetAddressOf());

	//エフェクトにコンテキストの設定を
	m_basicEffect->Apply(context);

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
	*/

	m_particle->Render(view, proj,world);
}

void Object::Finalize()
{
}

void Object::CreateShader()
{
	auto device = m_deviceResources->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/KeyEffectVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/KeyEffectGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/KeyEffectPS.cso");

	//インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_objectVS.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_objectGS.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_objectPS.ReleaseAndGetAddressOf())))
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
