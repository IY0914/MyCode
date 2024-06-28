cbuffer ConstBuffer : register(b1)	//èÓïÒÇìoò^
{
	matrix world;
	matrix view;
	matrix proj;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float4 color: COLOR;
	float2 tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color: COLOR;
	float2 tex : TEXCOORD;
};