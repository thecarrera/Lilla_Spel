struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float4 normal : NORMAL;
};

float3 PS_main(GS_OUT input) : SV_Target
{
	float3 color = float3(1.0f, 0.0f, 0.0f);
	return color;
}