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
	float4 Normal : NORMAL;
    float2 Texture : TEXCOORD0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
	float4 Normal : NORMAL;
	float2 Texture : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

	output.Normal = mul(input.Normal, World);	// 월드 공간에서의 노말 값으로 계산
	output.Normal = normalize(output.Normal);	// 노말 벡터의 정규화
	
    output.Texture = input.Texture;

    return output;
}

cbuffer PS_Light :  register(b0)
{
	float4 Ambient;				// 주변광
	float4 Diffuse;				// 반사광
	float4 LightDirection;		// 빛의 방향 (디렉셔날 라이트)
}

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 tex = Texture.Sample(Sampler, input.Texture);

	float4 LightDir = -LightDirection;					// 사물이 반사하는 빛의 방향
	float lightIntensity = dot(input.Normal, LightDir);	// 빛의 조도(강도)
	lightIntensity = saturate(lightIntensity);			// 0 ~ 1사이로 변환 (잘라내기 변환)

	float4 lightColor = saturate(Diffuse * lightIntensity + Ambient);	// 설정 된 반사색상에 빛의 강도를 곱해서 반사되는 빛의 색상이 정해진다.

	tex = tex * lightColor; // 빛의 색상에 의해서 최종적으로 텍스쳐의 색상이 결정 된다.

    return tex;
}