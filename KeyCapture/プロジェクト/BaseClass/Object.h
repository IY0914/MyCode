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
	//�f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;
	//�X�e�[�g�I�u�W�F�N�g
	std::unique_ptr<DirectX::CommonStates> m_state;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	//�ʒu���
	DirectX::SimpleMath::Vector3 m_position;
	//���f�����
	DirectX::Model* m_model;
	//��]
	float m_rorate;
	//�I�u�W�F�N�g�ԍ�
	int m_objectNum;
	//�o�����Ă��邩
	bool m_popItem;
	//�p�X
	const wchar_t* m_path;

private:
	//�V�F�[�_�[�������Ȃ��̂Ńe�X�g
	std::unique_ptr<tito::Particle> m_particle;

private:
	//�摜���
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tex;

	//�C���v�b�g���C�A�E�g
	const static std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//�e�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_objectPS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_objectVS;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_objectGS;

	//�o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

public:
	//�o�����Ă��邩�̔���
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