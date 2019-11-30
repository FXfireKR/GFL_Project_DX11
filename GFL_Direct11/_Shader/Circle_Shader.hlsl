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

float getDist(float x, float y, float cent)
{
	float4 trg;

	trg.x = x; trg.y = y;
	trg.z = trg.w = 0;

	float4 center;

	center.y = center.x = cent;
	center.z = center.w = 0;

	float r = abs(distance(trg, center));

	return r;
}

cbuffer PS_Color : register(b0)
{
	float4 Color;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float x = input.UV.x;
	float y = input.UV.y;
	float4 color = Color;
	float d = getDist(x, y, 0.5f);
	
	if (d > 0.495f || d < 0.49f)
		color.a = 0.0f;

	else
	{
		color.a = Color.a;
		//float val = 0.65f - (d);
		//color.a = smoothstep(0, 1, val);
		//color.a = (1.0f - (d * 1.05f));
	}

	return color;
}