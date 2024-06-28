#pragma once
#include "DeviceResources.h"
#include <vector>


class UserInterface
{
public:
	//UIのアンカーポイントの列挙数
	 enum ANCHOR
	{
		TOP_LEFT = 0,
		TOP_CENTER,
		TOP_RIGHT,

		MIDDLE_LEFT,
		MIDDLE_CENTER,
		MIDDLE_RIGHT,

		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT

	};

private:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix worldview;
		DirectX::SimpleMath::Matrix worldproj;
		DirectX::SimpleMath::Vector4 diffuse;
		DirectX::SimpleMath::Vector4 windowSize;
	};

private:
	//デバイスリソース
	DX::DeviceResources* m_deviceResources;
	//共通ステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//コンストバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constBuffer;

	//画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tex;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;
	//ジオメトリシェーダー
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	//ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

	//行列
	DirectX::SimpleMath::Matrix m_world;
	//画像と画面サイズ
	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;
	//位置と拡大率
	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Vector2 m_scale;
	DirectX::SimpleMath::Vector2 m_baseScale;

	//3D空間上の位置
	DirectX::SimpleMath::Vector3 m_fieldPosition;

	//描画基準位置にの列挙数
	ANCHOR m_anchor;

	//描画割合(0～1)
	float m_renderRatio;

public:
	//インプットレイアウトの定数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	UserInterface();
	~UserInterface();

	//作成関数
	void Create(DX::DeviceResources* m_pDR,
		const wchar_t* path, DirectX::SimpleMath::Vector2 pos,
		DirectX::SimpleMath::Vector2 scale, ANCHOR anchor);
	//画像設定
	void LoadTexture(const wchar_t* path);
	//シェーダー作成クラス
	void CreateShader();
	//描画関数
	void Render();
	//フィールド描画関数
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//画面サイズ
	void SetWindowSize(const int& width, const int& height);

public:
	//スケールの設定
	void SetScale(DirectX::SimpleMath::Vector2 scale);
	//スケールの取得
	DirectX::SimpleMath::Vector2 GetScale() { return m_scale; }
	//基準のスケールの取得
	DirectX::SimpleMath::Vector2 GetBaseScale() { return m_baseScale; }
	//位置を設定
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; }
	//位置を取得
	DirectX::SimpleMath::Vector2 GetPosition() { return m_position; }
	//表示基準位置を設定
	void SetAnchor(ANCHOR anchor);
	//表示基準位置を取得
	ANCHOR GetAnchor() { return m_anchor; }
	//?
	void SetRenderRatio(float ratio);
	float GetRednerRatio() { return m_renderRatio; }

};