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
	float4 Color : COLOR0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
    float2 UV : UV0;
	float4 Color : COLOR0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
	
    output.UV = input.UV;
	output.Color = input.Color;

    return output;
}

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 color = Texture.Sample(Sampler, input.UV);

	//0.5 [float] 보다 낮은 값들은 0.0 [float]으로 치환합니다
	if (color.a < 0.5f)
		color.a = 0.0f;

	//그 이상인 값들은 PX_shader에서 받아오는 Sample PixelARGB값에 의해서 적용됩니다.
	else
	{
		//input STVertex :: D3DXCOLOR Color (R G B A)
		color.a = color.a * input.Color.a;
		//color.r = input.Color.r;
		//color.g = input.Color.g;
		//color.b = input.Color.b;
	}

	return color;
}
