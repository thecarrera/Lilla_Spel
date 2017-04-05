cbuffer CBUFFER
{
	float4x4 worldM;
	float4x4 viewM;
	float4x4 projM;
}

struct GS_IN
{
	float4 Pos : SV_POSITION;
	float4 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct FS_OUT
{
	float4 Pos : SV_POSITION;
	float4 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float4 wPos : POSITION;
};

[maxvertexcount(6)]
void GS_main(triangle GS_IN input[3], inout TriangleStream <FS_OUT> OutputStream)
{
	FS_OUT output = (FS_OUT)0;

	for (int i = 0; i < 3; i++)
	{
		output.Pos = mul(mul(mul(input[i].Pos, worldM), viewM), projM);
		output.Norm = normalize(mul(input[i].Norm, worldM));
		output.wPos = mul(input[i].Pos, worldM);
		output.Tex = input[i].Tex;

		OutputStream.Append(output);
	}
}