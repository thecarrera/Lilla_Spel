struct VS_IN
{
    float3 Pos : POSITION;
    float sizeIn : SIZE;
    float4 colorIn : COLOR;
    int whatTriangleIn : WHATTRIANGLE;
    float3 padding : PADDING;
};

struct VS_OUT
{
    float4 position : SV_POSITION; //SV_POSITION?
    float size : SIZE;
    float4 color : COLOR;
    int whatTriangle : WHATTRIANGLE;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.position = float4(input.Pos, 1);

    output.size = input.sizeIn;

    output.color = input.colorIn;

    output.whatTriangle = input.whatTriangleIn;

    return output;
}