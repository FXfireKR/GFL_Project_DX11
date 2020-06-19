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

	output.Normal = mul(input.Normal, World);	// ���� ���������� �븻 ������ ���
	output.Normal = normalize(output.Normal);	// �븻 ������ ����ȭ
	
    output.Texture = input.Texture;

    return output;
}

cbuffer PS_Light :  register(b0)
{
	float4 Ambient;				// �ֺ���
	float4 Diffuse;				// �ݻ籤
	float4 LightDirection;		// ���� ���� (�𷺼ų� ����Ʈ)
}

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 tex = Texture.Sample(Sampler, input.Texture);

	float4 LightDir = -LightDirection;					// �繰�� �ݻ��ϴ� ���� ����
	float lightIntensity = dot(input.Normal, LightDir);	// ���� ����(����)
	lightIntensity = saturate(lightIntensity);			// 0 ~ 1���̷� ��ȯ (�߶󳻱� ��ȯ)

	float4 lightColor = saturate(Diffuse * lightIntensity + Ambient);	// ���� �� �ݻ���� ���� ������ ���ؼ� �ݻ�Ǵ� ���� ������ ��������.

	tex = tex * lightColor; // ���� ���� ���ؼ� ���������� �ؽ����� ������ ���� �ȴ�.

    return tex;
}