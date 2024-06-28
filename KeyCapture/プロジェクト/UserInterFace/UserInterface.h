#pragma once
#include "DeviceResources.h"
#include <vector>


class UserInterface
{
public:
	//UI�̃A���J�[�|�C���g�̗񋓐�
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
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix worldview;
		DirectX::SimpleMath::Matrix worldproj;
		DirectX::SimpleMath::Vector4 diffuse;
		DirectX::SimpleMath::Vector4 windowSize;
	};

private:
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	//���ʃX�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//�R���X�g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constBuffer;

	//�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tex;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;
	//�W�I���g���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	//�s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

	//�s��
	DirectX::SimpleMath::Matrix m_world;
	//�摜�Ɖ�ʃT�C�Y
	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;
	//�ʒu�Ɗg�嗦
	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Vector2 m_scale;
	DirectX::SimpleMath::Vector2 m_baseScale;

	//3D��ԏ�̈ʒu
	DirectX::SimpleMath::Vector3 m_fieldPosition;

	//�`���ʒu�ɂ̗񋓐�
	ANCHOR m_anchor;

	//�`�抄��(0�`1)
	float m_renderRatio;

public:
	//�C���v�b�g���C�A�E�g�̒萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	UserInterface();
	~UserInterface();

	//�쐬�֐�
	void Create(DX::DeviceResources* m_pDR,
		const wchar_t* path, DirectX::SimpleMath::Vector2 pos,
		DirectX::SimpleMath::Vector2 scale, ANCHOR anchor);
	//�摜�ݒ�
	void LoadTexture(const wchar_t* path);
	//�V�F�[�_�[�쐬�N���X
	void CreateShader();
	//�`��֐�
	void Render();
	//�t�B�[���h�`��֐�
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//��ʃT�C�Y
	void SetWindowSize(const int& width, const int& height);

public:
	//�X�P�[���̐ݒ�
	void SetScale(DirectX::SimpleMath::Vector2 scale);
	//�X�P�[���̎擾
	DirectX::SimpleMath::Vector2 GetScale() { return m_scale; }
	//��̃X�P�[���̎擾
	DirectX::SimpleMath::Vector2 GetBaseScale() { return m_baseScale; }
	//�ʒu��ݒ�
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; }
	//�ʒu���擾
	DirectX::SimpleMath::Vector2 GetPosition() { return m_position; }
	//�\����ʒu��ݒ�
	void SetAnchor(ANCHOR anchor);
	//�\����ʒu���擾
	ANCHOR GetAnchor() { return m_anchor; }
	//?
	void SetRenderRatio(float ratio);
	float GetRednerRatio() { return m_renderRatio; }

};