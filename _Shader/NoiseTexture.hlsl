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

SamplerState Noize_Sampler : register(s1);
Texture2D Noize : register(t1);

cbuffer PS_Color : register(b0)
{
	float4 Color;
}

cbuffer PS_Noise : register(b1)
{
	float	startY;
	float	limitAlpha;		//	Limit Value of Alpha (0 ~ 255)
	float2	_padding;		//	Padding Byte Memory
}

float4 PS(PixelInput input) : SV_TARGET
{
	//input.UV.x += startY;
	//if (input.UV.x < 0.0F)
	//	input.UV.x += 1.0F;

	float4 color = Texture.Sample(Sampler, input.UV);

	float4 noise_Color;
	float2 revUV;

	//	float4 random = (tex2D(noiseTexture, texCoord * noiseScale + noiseOffset));

	const float _bit = 0.00390625f;
	const float limit = _bit * limitAlpha;

	revUV.x = input.UV.x;
	revUV.y = input.UV.y - startY;

	if (revUV.y < 0.0F)
		revUV.y += 1.0F;

	//	Sampling Color From 'NOIZE' image
	noise_Color = Noize.Sample(Noize_Sampler, revUV);

	if (noise_Color.r > limit && noise_Color.g > limit &&  noise_Color.b > limit)
	{
		if (startY > 0.4)
			noise_Color.a *= startY;
		else
			noise_Color.a *= 0.4;
	}
	else
		noise_Color.a = 0;

	color = color * Color;
	color.a *= noise_Color.a;

	return color;
}