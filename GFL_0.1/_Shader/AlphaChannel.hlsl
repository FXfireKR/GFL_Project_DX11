cbuffer VS_ViewProj : register(b0)
{
	matrix View;
	matrix Projection;
}

cbuffer VS_World : register(b1)
{
	matrix World;
}

struct VertexInput
{
	float4 Position : POSITION0;
	float2 UV : UV0;
};

struct PixelInput
{
	float4 Position : SV_POSITION;
	float2 UV : UV0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.UV = input.UV;

	return output;
}

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

SamplerState alpha_Sampler : register(s1);
Texture2D TextureAlpha : register(t1);

cbuffer PS_Color : register(b0)
{
	float4 worldColor;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 color = Texture.Sample(Sampler, input.UV);
	float4 colorAlpha = TextureAlpha.Sample(alpha_Sampler, input.UV);

	color.a = colorAlpha.r;
	color *= worldColor;

	return color;
}