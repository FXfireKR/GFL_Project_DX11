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

	// 월드 공간에서의 버텍스 포지션
	output.ViewDirection = CameraPosition.xyz - output.Position.xyz;
	output.ViewDirection = normalize(output.ViewDirection);

    output.Position = mul(View, output.Position);
    output.Position = mul(Projection, output.Position);

	output.Normal = mul(input.Normal, World);	// 월드 공간에서의 노말 값으로 계산
	output.Normal = normalize(output.Normal);	// 노말 벡터의 정규화
	
    output.Texture = input.Texture;

    return output;
}

cbuffer PS_LightBuffer :  register(b0)
{
	float4 Ambient;				// 주변광
	float4 Diffuse;				// 반사광
	float4 Specular;			// 정반사광
	float4 SpecPower;			// 정반사광의 강도 (y,z,w 값은 사용 안함, 3byte padding)
	float4 LightDirection;		// 빛의 방향 (디렉셔날 라이트)
}

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

float4 PS(PixelInput input) : SV_TARGET
{
    float4 tex = Texture.Sample(Sampler, input.Texture);

	//float4 LightDir = -LightDirection;			// 사물이 반사하는 빛의 방향
	//float lightIntensity = dot(input.Normal, LightDir);	// 빛의 조도(강도)
	//lightIntensity = saturate(lightIntensity);	// saturate() : 0 ~ 1사이로 변환 (잘라내기 변환)

	//float4 lightColor = saturate(Diffuse * lightIntensity + Ambient);	// 설정 된 반사색상에 빛의 강도를 곱해서 반사되는 빛의 색상이 정해진다.
	//
	//float4 specular = float4(0, 0, 0, 0);

	//if (lightIntensity > 0.0f)
	//{
	//	float3 reVec = normalize(2 * lightIntensity * input.Normal - LightDir);
	//	specular = Specular * pow(saturate(dot(input.ViewDirection, reVec)), SpecPower.x);
	//}

	//tex = tex * lightColor; // 빛의 색상에 의해서 최종적으로 텍스쳐의 색상이 결정 된다.
	//tex = saturate(tex + specular);

    return tex;
}

// Ambient Color(주변광or환경광) : 일반적으로는 15%의 빛을 설정
// Diffuse Color(반사광) : 오브젝트에 닿아서 반사 되는 색상에 대한 값
// Specular Color(정반사광) : 빛의 방향과 카메라(보는)의 방향이 일치할 때 강조되는 빛의 색상

/*
정반사광의 방정식 - 반사강도는 적용 값이 클수록 빛의 강도는 약해진다(0~1에 대한 제곱 값)
정반사광 = 정반사색상 * (정반사색상 * (법선벡터ㆍ하프벡터)^반사강도 * 감쇄계수 * 스포트라이계수

게임에서의 기본 정반사광 계산 식
정반사광 = 정반사색상 * (뷰벡터ㆍ반사벡터)^반사강도

뷰벡터(정점기준) : 카메라 - 정점
반사벡터 : 2 * 빛의조도 * 법선벡터 - 반사된빛의방향
*/