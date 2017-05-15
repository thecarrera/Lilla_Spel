cbuffer CBUFFER : register(b0)
{
	float4x4 worldM;
	float4x4 viewM;
	float4x4 projM;
	float3   lightPosition;
}
cbuffer LBUFFER : register(b1)
{
	float4x4 lworldM;
	float4x4 lviewM;
	float4x4 lprojM;
	float3   lPos;
}

struct GS_IN
{
	float4 Pos : SV_POSITION;
	float2 uv : UV;
	float3 Norm : NORMAL;
};

struct FS_OUT
{
	float4 Pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float4 wPos : POSITION;
	float4 lPos : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_main(triangle GS_IN input[3], inout TriangleStream <FS_OUT> OutputStream)
{
	float3 n = normalize(mul(cross(input[1].Pos - input[0].Pos, input[2].Pos - input[0].Pos), worldM));

	FS_OUT output = (FS_OUT)0;

	for (int i = 0; i < 3; i++)
	{
		output.Pos = mul(mul(mul(input[i].Pos, worldM), viewM), projM);
		output.wPos = mul(input[i].Pos, worldM);
		output.uv = float3(input[i].uv, 0.0f);
		output.normal = n;
		output.lPos = mul(lPos, worldM);

		OutputStream.Append(output);
	}
}