cbuffer VS_ViewProj : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer VS_World : register(b1)
{
    matrix World;
}

cbuffer VS_Camera : register(b2)
{
	float4 CameraPosition;
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
	float3 ViewDirection : TEXCOORD1;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = mul(World, input.Position);

	// ���� ���������� ���ؽ� ������
	output.ViewDirection = CameraPosition.xyz - output.Position.xyz;
	output.ViewDirection = normalize(output.ViewDirection);

    output.Position = mul(View, output.Position);
    output.Position = mul(Projection, output.Position);

	output.Normal = mul(input.Normal, World);	// ���� ���������� �븻 ������ ���
	output.Normal = normalize(output.Normal);	// �븻 ������ ����ȭ
	
    output.Texture = input.Texture;

    return output;
}

cbuffer PS_LightBuffer :  register(b0)
{
	float4 Ambient;				// �ֺ���
	float4 Diffuse;				// �ݻ籤
	float4 Specular;			// ���ݻ籤
	float4 SpecPower;			// ���ݻ籤�� ���� (y,z,w ���� ��� ����, 3byte padding)
	float4 LightDirection;		// ���� ���� (�𷺼ų� ����Ʈ)
}

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 tex = Texture.Sample(Sampler, input.Texture);

	//float4 LightDir = -LightDirection;			// �繰�� �ݻ��ϴ� ���� ����
	//float lightIntensity = dot(input.Normal, LightDir);	// ���� ����(����)
	//lightIntensity = saturate(lightIntensity);	// saturate() : 0 ~ 1���̷� ��ȯ (�߶󳻱� ��ȯ)

	//float4 lightColor = saturate(Diffuse * lightIntensity + Ambient);	// ���� �� �ݻ���� ���� ������ ���ؼ� �ݻ�Ǵ� ���� ������ ��������.
	//
	//float4 specular = float4(0, 0, 0, 0);

	//if (lightIntensity > 0.0f)
	//{
	//	float3 reVec = normalize(2 * lightIntensity * input.Normal - LightDir);
	//	specular = Specular * pow(saturate(dot(input.ViewDirection, reVec)), SpecPower.x);
	//}

	//tex = tex * lightColor; // ���� ���� ���ؼ� ���������� �ؽ����� ������ ���� �ȴ�.
	//tex = saturate(tex + specular);

    return tex;
}

// Ambient Color(�ֺ���orȯ�汤) : �Ϲ������δ� 15%�� ���� ����
// Diffuse Color(�ݻ籤) : ������Ʈ�� ��Ƽ� �ݻ� �Ǵ� ���� ���� ��
// Specular Color(���ݻ籤) : ���� ����� ī�޶�(����)�� ������ ��ġ�� �� �����Ǵ� ���� ����

/*
���ݻ籤�� ������ - �ݻ簭���� ���� ���� Ŭ���� ���� ������ ��������(0~1�� ���� ���� ��)
���ݻ籤 = ���ݻ���� * (���ݻ���� * (�������ͤ���������)^�ݻ簭�� * ������ * ����Ʈ���̰��

���ӿ����� �⺻ ���ݻ籤 ��� ��
���ݻ籤 = ���ݻ���� * (�交�ͤ��ݻ纤��)^�ݻ簭��

�交��(��������) : ī�޶� - ����
�ݻ纤�� : 2 * �������� * �������� - �ݻ�Ⱥ��ǹ���
*/