#include "KeyEffect.hlsli"

static const unsigned int vnum = 4;

//w��0���ƍs��v�Z�����������Ȃ�̂�1�ɕύX
static const float4 offset_array[vnum] =
{
	float4(0.0f,  1.0f, 0.0f, 1.0f),	// ����
	float4(1.0f,  1.0f, 0.0f, 1.0f),	// �E��
	float4(0.0f,  0.0f, 0.0f, 1.0f),	// ����
	float4(1.0f,  0.0f, 0.0f, 1.0f),	// �E��

};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		PS_INPUT element;

		float4 res = offset_array[i];

		//3D��Ԃɕϊ�(���[���h���W�Ɋ|����)
		element.pos = input[0].pos + mul(offset_array[i], world);

		element.pos = mul(element.pos, view);
		element.pos = mul(element.pos, proj);

		//2D��Ԃɕϊ�(�X�N���[�����W(1 > -1))��w�͒P�ʃx�N�g���Ȃ̂�1�����
		/*element.Pos.xyzw = float4(0, 0, 0, 1);
		element.Pos.xy = offset_array[i].xy;*/


		element.color = input[0].color;
		element.tex.x = offset_array[i].x + 0.5f;
		element.tex.y = -offset_array[i].y + 0.5f;



		output.Append(element);
	}
}