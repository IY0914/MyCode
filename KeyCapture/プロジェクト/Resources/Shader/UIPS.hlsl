#include "UI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//�g���̉摜�\��
	float4 output = tex.Sample(samLinear, input.tex);

	//�^�����Ȕ|���S��
	float4 outputw = float4(1, 1, 1, 1);
	
	//�o�͂���摜�̓����x���Ԃ���(step�֐��͒i�K�֐�)
	//output.a *= lerp(1.0f, 0.0f, step(input.color.x, input.tex.x));

	//float4 ret;
	//ret = lerp(float4(1, 1, 1, 1), float4(1, 1, 1, 1),1);
	//return ret;

	return output;
}