#include "KeyEffect.hlsli"

//PS_INPUT�^�ŕԂ�(������VS_INPUT)
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;		//�O�ŏ�����?
	//���_�ʒu���擾
	output.pos = float4(input.pos, 1);

	//�摜�̐F���擾
	output.color = input.color;
	//�摜�����擾
	output.tex = input.tex;

	return output;	//�o��
}