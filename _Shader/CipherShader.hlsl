//	constant Buffer
cbuffer CB : register(b0)
{
	unsigned int g_iCount;
};

//	Input Buffer
StructuredBuffer<float> Input : register(t0);

//	Output Buffer
RWStructuredBuffer<float> Result : register(u0);

//	Shared Memory
groupshared float sharedData[128];


//	Ndivia Graphic Card should be x32	= Warp size
//	AMD Graphic Card should be x64		= Wavefront size
[numthreads(128, 1, 1)]
void CS(uint3 Gid : SV_GroupID,
		uint3 DTid : SV_DispatchThreadID,
		uint3 GTid : SV_GroupThreadID,
		uint GI : SV_GroupIndex)
{
	//	Read Data
	sharedData[GI] = DTid.x < g_iCount ? Input[DTid.x] : 0;

	//	Barrier Memory Sync
	GroupMemoryBarrierWithGroupSync();
	if (GI < 64) sharedData[GI] += sharedData[GI + 64];

	GroupMemoryBarrierWithGroupSync();
	if (GI < 32) sharedData[GI] += sharedData[GI + 32];

	GroupMemoryBarrierWithGroupSync();
	if (GI < 16) sharedData[GI] += sharedData[GI + 16];

	GroupMemoryBarrierWithGroupSync();
	if (GI < 8) sharedData[GI] += sharedData[GI + 8];

	GroupMemoryBarrierWithGroupSync();
	if (GI < 4) sharedData[GI] += sharedData[GI + 4];

	GroupMemoryBarrierWithGroupSync();
	if (GI < 2) sharedData[GI] += sharedData[GI + 2];

	GroupMemoryBarrierWithGroupSync();
	if (GI < 1) sharedData[GI] += sharedData[GI + 1];

	if (GI == 0)
		Result[Gid.x] = sharedData[0];
}