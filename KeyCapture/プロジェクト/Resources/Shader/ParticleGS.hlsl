#include "Particle.hlsli"

static const int vnum = 4;

static const float4 offset_array[vnum] =
{
	//星型
	//float4( 0.0f,   0.5f, 0.0f, 0.0f),	// 三角形上
	//float4(-0.15f,  0.0f, 0.0f, 0.0f),	// 三角形
	//float4( 0.15f,  0.0f, 0.0f, 0.0f),	// 三角形
	//									 
	//float4(-0.5f,   0.0f, 0.0f, 0.0f),	// 三角形真ん中
	//float4( 0.5f,   0.0f, 0.0f, 0.0f),	// 三角形
	//float4( 0.0f,  -0.35f, 0.0f,0.0f),	// 三角形
	//									  
	//float4( 0.35f, -0.85f, 0.0f, 0.0f),	// 三角形右下
	//float4( 0.0f, -0.35f, 0.0f, 0.0f),	// 三角形
	//float4( 0.15f, 0.0f, 0.0f, 0.0f),	// 三角形
	//									  
	//float4(-0.35f, -0.85f, 0.0f, 0.0f),	// 三角形左下
	//float4(-0.15f, 0.0f, 0.0f, 0.0f),	// 三角形
	//float4( 0.0f, -0.35f, 0.0f, 0.0f),	// 三角形

	//六芒星
	//float4( 0.0f,   0.5f, 0.0f, 0.0f),	// 三角形上



	//float4( 0.5f,  -0.0f, -0.0f, 0.0f),	// 三角形
	//float4( 0.3f,  -0.0f, -0.3f, 0.0f),	// 三角形

	//float4( -0.3f,  -0.0f, -0.3f, 0.0f),	// 三角形真ん中
	//float4(-0.5f,   0.0f, 0.0f, 0.0f),	// 三角形

	//float4(-0.1f,  0.0f, 0.3f,0.0f),	// 三角形

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


	//四角形
	float4(-0.5f,   0.5f, 0.0f, 0.0f),	//左上
	float4(0.5f,  0.5f, 0.0f, 0.0f),	//右上
	float4(-0.5f, -0.5f, 0.0f, 0.0f),	//左下
	float4(0.5f,   -0.5f, 0.0f, 0.0f),	//右下
};

//★

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{

	for (int i = 0; i < vnum; i++)
	{
		////頂点数(if文をシェーダーに書くのは良くはない)
		//if (i % 3 == 0)
		//{
		//	//指定した頂点数になったらつながりを切る
		//	output.RestartStrip();
		//}

		PS_INPUT element;

		float4 res = offset_array[i];

		//3D空間に変換(ワールド座標に掛ける)
		element.Pos = input[0].Pos + mul(offset_array[i], matWorld);

		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		//2D空間に変換(スクリーン座標(1 > -1))※wは単位ベクトルなので1が絶対
		/*element.Pos.xyzw = float4(0, 0, 0, 1);
		element.Pos.xy = offset_array[i].xy;*/


		element.Color = input[0].Color;
		element.Tex.x =  offset_array[i].x + 0.5f;
		element.Tex.y = -offset_array[i].y + 0.5f;


		
		output.Append(element);
	}

	output.RestartStrip();
}