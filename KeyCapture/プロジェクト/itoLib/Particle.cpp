//--------------------------------------------------------------------------------------
// File: Particle.h
//
// �p�[�e�B�N���N���X
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
/// �C���v�b�g���C�A�E�g
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> tito::Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3)+ sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// �R���X�g���N�^
/// </summary>
tito::Particle::Particle()
	:m_pDR(nullptr)
	,m_position(SimpleMath::Vector3::Zero)
	,transparent(0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
tito::Particle::~Particle()
{
}

/// <summary>
/// �e�N�X�`�����\�[�X�ǂݍ��݊֐�
/// </summary>
/// <param name="path">���΃p�X(Resources/Textures/�E�E�E.png�Ȃǁj</param>
void tito::Particle::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());
	
	m_texture.push_back(texture);
}

/// <summary>
/// �����֐�
/// </summary>
/// <param name="pDR">���[�U�[���\�[�X�����玝���Ă���</param>
void tito::Particle::Create(DX::DeviceResources* pDR, DirectX::SimpleMath::Vector3 pos)
{	
	m_pDR = pDR;
	auto device = pDR->GetD3DDevice();

	m_position = pos;

	//�V�F�[�_�[�̍쐬
	CreateShader();

	//�摜�̓ǂݍ��݁i�Q���Ƃ��f�t�H���g�͓ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Texture/Luster.png");
	//LoadTexture(L"Resources/Textures/ball.png");

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

}

void tito::Particle::Update(float time)
{
	//�����x��ύX
	transparent = time;
}

/// <summary>
/// Shader�쐬�����������������֐�
/// </summary>
void tito::Particle::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/ParticlePS.cso");

	//�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

/// <summary>
/// �`��֐�
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void tito::Particle::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Matrix world)
{
	auto context = m_pDR->GetD3DDeviceContext();
		// ���_���(�|���S���̂S���_�̍��W���j
	VertexPositionColorTexture vertex[4] =
		{
			VertexPositionColorTexture(m_position + SimpleMath::Vector3(cos(transparent),3.5f,sin(-transparent)),SimpleMath::Vector4(1,1,1,transparent),SimpleMath::Vector2(0.0f, 0.0f)),
			//VertexPositionColorTexture(SimpleMath::Vector3(-0.5f, -0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 1.0f)),
			//VertexPositionColorTexture(SimpleMath::Vector3( 0.5f, -0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(1.0f, 1.0f)),
			//VertexPositionColorTexture(SimpleMath::Vector3( 0.5f,  0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(1.0f, 0.0f)),
		};

		//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
		ConstBuffer cbuff;
		cbuff.matView = view.Transpose();
		cbuff.matProj = proj.Transpose();
		cbuff.matWorld = world.Transpose();
		cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, sinf(transparent));

		//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
		context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

		//�V�F�[�_�[�Ƀo�b�t�@��n��
		ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
		context->VSSetConstantBuffers(0, 1, cb);
		context->GSSetConstantBuffers(0, 1, cb);
		context->PSSetConstantBuffers(0, 1, cb);

		//�摜�p�T���v���[�̓o�^
		ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
		context->PSSetSamplers(0, 1, sampler);

		//�������`��w��
		ID3D11BlendState* blendstate = m_states->NonPremultiplied();

		// �������菈��
		context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

		// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
		context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

		// �J�����O�͍�����
		context->RSSetState(m_states->CullNone());

		//�V�F�[�_���Z�b�g����
		context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
		context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
		//context->PSSetShaderResources(0, 1, m_texture[0].GetAddressOf());
		//context->PSSetShaderResources(1, 1, m_texture[1].GetAddressOf());
		for (int i = 0; i < m_texture.size(); i++)
		{
			context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
		}

		//�C���v�b�g���C�A�E�g�̓o�^
		context->IASetInputLayout(m_inputLayout.Get());

		// �|���S����`��
		m_batch->Begin();
		m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
		m_batch->End();

		//�V�F�[�_�̓o�^���������Ă���
		context->VSSetShader(nullptr, nullptr, 0);
		context->GSSetShader(nullptr, nullptr, 0);
		context->PSSetShader(nullptr, nullptr, 0);

}
