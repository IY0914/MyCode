#include "KeyEffect.hlsli"

static const unsigned int vnum = 4;

//wが0だと行列計算がおかしくなるので1に変更
static const float4 offset_array[vnum] =
{
	float4(0.0f,  1.0f, 0.0f, 1.0f),	// 左上
	float4(1.0f,  1.0f, 0.0f, 1.0f),	// 右上
	float4(0.0f,  0.0f, 0.0f, 1.0f),	// 左下
	float4(1.0f,  0.0f, 0.0f, 1.0f),	// 右下

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

		//3D空間に変換(ワールド座標に掛ける)
		element.pos = input[0].pos + mul(offset_array[i], world);

		element.pos = mul(element.pos, view);
		element.pos = mul(element.pos, proj);

		//2D空間に変換(スクリーン座標(1 > -1))※wは単位ベクトルなので1が絶対
		/*element.Pos.xyzw = float4(0, 0, 0, 1);
		element.Pos.xy = offset_array[i].xy;*/


		element.color = input[0].color;
		element.tex.x = offset_array[i].x + 0.5f;
		element.tex.y = -offset_array[i].y + 0.5f;



		output.Append(element);
	}
}