#include "Particle.hlsli"

static const int vnum = 4;

static const float4 offset_array[vnum] =
{
	//���^
	//float4( 0.0f,   0.5f, 0.0f, 0.0f),	// �O�p�`��
	//float4(-0.15f,  0.0f, 0.0f, 0.0f),	// �O�p�`
	//float4( 0.15f,  0.0f, 0.0f, 0.0f),	// �O�p�`
	//									 
	//float4(-0.5f,   0.0f, 0.0f, 0.0f),	// �O�p�`�^��
	//float4( 0.5f,   0.0f, 0.0f, 0.0f),	// �O�p�`
	//float4( 0.0f,  -0.35f, 0.0f,0.0f),	// �O�p�`
	//									  
	//float4( 0.35f, -0.85f, 0.0f, 0.0f),	// �O�p�`�E��
	//float4( 0.0f, -0.35f, 0.0f, 0.0f),	// �O�p�`
	//float4( 0.15f, 0.0f, 0.0f, 0.0f),	// �O�p�`
	//									  
	//float4(-0.35f, -0.85f, 0.0f, 0.0f),	// �O�p�`����
	//float4(-0.15f, 0.0f, 0.0f, 0.0f),	// �O�p�`
	//float4( 0.0f, -0.35f, 0.0f, 0.0f),	// �O�p�`

	//�Z䊐�
	//float4( 0.0f,   0.5f, 0.0f, 0.0f),	// �O�p�`��



	//float4( 0.5f,  -0.0f, -0.0f, 0.0f),	// �O�p�`
	//float4( 0.3f,  -0.0f, -0.3f, 0.0f),	// �O�p�`

	//float4( -0.3f,  -0.0f, -0.3f, 0.0f),	// �O�p�`�^��
	//float4(-0.5f,   0.0f, 0.0f, 0.0f),	// �O�p�`

	//float4(-0.1f,  0.0f, 0.3f,0.0f),	// �O�p�`

	/*float4(0.0f, 0.1f,  0.0f, 0.0f),
	float4(-0.1f, 0.0f, 0.1f, 0.0f),
	float4(0.1f,  0.0f, 0.1f, 0.0f),

	float4(0.0f, 0.1f, 0.0f, 0.0f),
	float4(0.1f, 0.0f, 0.1f, 0.0f),
	float4(0.1f, 0.0f, -0.1f, 0.0f),

	float4(0.0f, 0.1f, 0.0f, 0.0f),
	float4(0.1f, 0.0f, -0.1f, 0.0f),
	float4(-0.1f, 0.0f, -0.1f, 0.0f),

	float4(-0.1f, 0.0f, -0.1f, 0.0f),
	float4(0.0f, 0.1f, 0.0f, 0.0f),
	float4(-0.1f, 0.0f, 0.1f, 0.0f),*/


	//�l�p�`
	float4(-0.5f,   0.5f, 0.0f, 0.0f),	//����
	float4(0.5f,  0.5f, 0.0f, 0.0f),	//�E��
	float4(-0.5f, -0.5f, 0.0f, 0.0f),	//����
	float4(0.5f,   -0.5f, 0.0f, 0.0f),	//�E��
};

//��

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{

	for (int i = 0; i < vnum; i++)
	{
		////���_��(if�����V�F�[�_�[�ɏ����̂͗ǂ��͂Ȃ�)
		//if (i % 3 == 0)
		//{
		//	//�w�肵�����_���ɂȂ�����Ȃ����؂�
		//	output.RestartStrip();
		//}

		PS_INPUT element;

		float4 res = offset_array[i];

		//3D��Ԃɕϊ�(���[���h���W�Ɋ|����)
		element.Pos = input[0].Pos + mul(offset_array[i], matWorld);

		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		//2D��Ԃɕϊ�(�X�N���[�����W(1 > -1))��w�͒P�ʃx�N�g���Ȃ̂�1�����
		/*element.Pos.xyzw = float4(0, 0, 0, 1);
		element.Pos.xy = offset_array[i].xy;*/


		element.Color = input[0].Color;
		element.Tex.x =  offset_array[i].x + 0.5f;
		element.Tex.y = -offset_array[i].y + 0.5f;


		
		output.Append(element);
	}

	output.RestartStrip();
}