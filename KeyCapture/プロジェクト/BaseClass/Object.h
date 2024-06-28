#pragma once
#include "IState.h"
#include "Camera/Camera.h"
#include "Collision.h"
#include <VertexTypes.h>
#include "Camera/Camera.h"
#include "itoLib/Particle.h"

class Object
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix proj;
		//float transParent;
	};
	

private:
	//デバイスリソース
	DX::DeviceResources* m_deviceResources;
	//ステートオブジェクト
	std::unique_ptr<DirectX::CommonStates> m_state;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	//位置情報
	DirectX::SimpleMath::Vector3 m_position;
	//モデル情報
	DirectX::Model* m_model;
	//回転
	float m_rorate;
	//オブジェクト番号
	int m_objectNum;
	//出現しているか
	bool m_popItem;
	//パス
	const wchar_t* m_path;

private:
	//シェーダーが動かないのでテスト
	std::unique_ptr<tito::Particle> m_particle;

private:
	//画像情報
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tex;

	//インプットレイアウト
	const static std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//各シェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_objectPS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_objectVS;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_objectGS;

	//バッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

public:
	//出現しているかの判定
	bool GetPopItem() { return m_popItem; }

	void SetObjectNumber(int num) { m_objectNum = num; }

public:

	Object(DX::DeviceResources* deviceResources, DirectX::Model* model,const wchar_t* effectPath = nullptr);//,DirectX::Model* model);
	~Object();

	void Initilize(DirectX::SimpleMath::Vector3 pos);
	void Update(const DX::StepTimer& timer);
	void Render(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);
	void Finalize();

	void CreateShader();
};