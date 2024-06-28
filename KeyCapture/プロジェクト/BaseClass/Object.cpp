#include "pch.h"
#include "Scene/PlayScene.h"
#include "BaseClass/Object.h"
#include "UserInterFace/BinaryFile.h"

using namespace DirectX;

// <summary>
/// �C���v�b�g���C�A�E�g
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
	//�V�F�[�_�[�̍쐬
	CreateShader();

}

Object::~Object()
{
	
}

void Object::Initilize(DirectX::SimpleMath::Vector3 pos)
{
	//�X�e�[�g�I�u�W�F�N�g�̍쐬
	m_state = std::make_unique<DirectX::CommonStates>(m_deviceResources->GetD3DDevice());
	//�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<VertexPositionColorTexture>>(
		m_deviceResources->GetD3DDeviceContext());
	//�x�[�V�b�N�G�t�F�N�g�̍쐬
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_deviceResources->GetD3DDevice());
	m_basicEffect->SetProjection(Camera::GetInstance()->GetProjection());
	m_basicEffect->SetVertexColorEnabled(true);

	//�摜�̓ǂݍ���
	DirectX::CreateWICTextureFromFile(
		m_deviceResources->GetD3DDevice(),
		m_path,
		nullptr,
		m_tex.ReleaseAndGetAddressOf()
	);

	//�V�F�[�_�[�̍쐬
	CreateShader();

	//�A�C�e���̏o��
	m_popItem = true;

	//�ʒu�̏��
	m_position = pos;

	
	m_particle = std::make_unique<tito::Particle>();
	m_particle->Create(m_deviceResources, m_position);

}

void Object::Update(const DX::StepTimer& timer)
{
	//��]�̉��Z
	m_rorate = timer.GetTotalSeconds();

	m_particle->Update(timer.GetTotalSeconds());

	//�擾������`�����
	if (Collision::GetInstance()->GetItemCollider(m_objectNum))
	{
		m_popItem = false;
	}
}

void Object::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	//�s��
	SimpleMath::Matrix world;
	SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_rorate);
	//�s��v�Z
	world = rotY * trans;

	//���f���`��
	if(m_popItem)m_model->Draw(context, *m_state, world, view, proj);

	/*------------------------------�V�F�[�_�[�̕`��֌W-------------------------------------*/
	/*
	// ���_���(�|���S���̂S���_�̍��W���j
	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(SimpleMath::Vector3(-0.5f,  0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionColorTexture(SimpleMath::Vector3(0.5f,  0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionColorTexture(SimpleMath::Vector3(-0.5f, -0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 1.0f)),
		VertexPositionColorTexture(SimpleMath::Vector3(0.5f, -0.5f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(1.0f, 1.0f)),
	};

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.view = view.Transpose();
	cbuff.proj = proj.Transpose();
	cbuff.world = world.Transpose();
	//cbuff.transParent = 1.0f;

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_state->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = m_state->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_state->DepthDefault(), 0);

	// �J�����O�͍�����
	context->RSSetState(m_state->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_objectVS.Get(), nullptr, 0);
	context->GSSetShader(m_objectGS.Get(), nullptr, 0);
	context->PSSetShader(m_objectPS.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�[�ɉ摜��ݒ肷��
	context->PSSetShaderResources(0, 1, m_tex.GetAddressOf());

	//�G�t�F�N�g�ɃR���e�L�X�g�̐ݒ��
	m_basicEffect->Apply(context);

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
	*/

	m_particle->Render(view, proj,world);
}

void Object::Finalize()
{
}

void Object::CreateShader()
{
	auto device = m_deviceResources->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shader/KeyEffectVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shader/KeyEffectGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shader/KeyEffectPS.cso");

	//�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_objectVS.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_objectGS.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_objectPS.ReleaseAndGetAddressOf())))
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
