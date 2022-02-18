// constant buffer b로 시작
// [A][RC][BS]
// API BindSlot
// Root constant
// HLSL bind slot
// https://docs.microsoft.com/en-us/windows/win32/direct3d12/example-root-signatures
// root테이블이 정해진 사이즈가 있어서 
// descriptor를 따로 만들어서 관리
// root cbv
cbuffer TEST_B0: register(b0)
{
    float4 offset0;
}

cbuffer TEST_B1 : register(b1)
{
    float4 offset1;
}

struct VS_IN
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos, 1.f);
    output.pos += offset0;

    output.color = input.color;
    output.color += offset1;
    

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    return input.color;
}