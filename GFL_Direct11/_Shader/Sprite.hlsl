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

cbuffer PS_Color : register(b0)
{
    float4 Color = { 1, 1, 1, 1 };
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 color = Texture.Sample(Sampler, input.UV);

    //color *= Color;

    return color;
}
