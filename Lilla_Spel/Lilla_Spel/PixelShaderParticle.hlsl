struct GS_OUT
{
    float4 Pos : SV_POSITION;
    float4 colorPS : COLOR;
};

float4 main(GS_OUT input) : SV_Target
{
    float4 color = input.colorPS;
    return color;
}