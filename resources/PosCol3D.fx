// Globals
float3 gCameraPosition : CameraPosition;

float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorldMatrix : WorldMatrix;

Texture2D gDiffuseMap : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gGlossinessMap : GlossinessMap;
#define LIGHTDIRECTION float3(0.577f, -0.577f, 0.577f)
#define PI 3.1415926535
#define KD 7

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};
//------------------------------------------------
// Input/Output Struct
//------------------------------------------------
struct VS_INPUT
{
	float3 Position : POSITION;
    float3 Color : COLOR;
    float2 Uv : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : WORLD;
    float2 Uv : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

// Rasterizer, Blend and DepthStencilState

RasterizerState gRasterizerState
{
    CullMode = none;
    FrontCounterClockwise = false; //default
};


BlendState gBlendState
{
    BlendEnable[0] = true;
    SrcBlend = src_alpha;
    DestBlend = inv_src_alpha;
    BlendOp = add;
    SrcBlendAlpha = zero;
    DestBlendAlpha = zero;
    BlendOpAlpha = add;
    RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState gDepthStencilState
{
    DepthEnable = true;
    DepthWriteMask = all;
    DepthFunc = less;
    StencilEnable = false;
    // Others are redundant because StencilEnable is false (for demo only)
    StencilReadMask = 0;
    StencilWriteMask = 0;

    FrontFaceStencilFunc = always;
    BackFaceStencilFunc = always;

    FrontFaceStencilDepthFail = keep;
    BackFaceStencilDepthFail = keep;

    FrontFaceStencilPass = keep;
    BackFaceStencilPass = keep;

    FrontFaceStencilFail = keep;
    BackFaceStencilFail = keep;
};

// Functions

float4 SampleNormal(float3 normalInput, float3 tangentInput, float4 normalSample)
{
    
    float3 biNormal = cross(normalInput, tangentInput);
    float4x4 tangentSpaceAxis = float4x4(float4(tangentInput, 1.f), float4(biNormal, 1.f), float4(normalInput, 1.f), float4(0.f, 0.f, 0.f, 0.f));

    float4 normal = float4(2 * normalSample.xyz - float3(1.f, 1.f, 1.f), 1.f);
    
    return mul(normal, tangentSpaceAxis);
}

float3 PhongSpecular(float3 worldPosition, float3 normal, float glossSample, float4 specularSample)
{
    float shininess = 25;
    float exp = glossSample * shininess;
    
    float3 invViewDirection = normalize(gCameraPosition - worldPosition);
    
    float3 reflectedRay = reflect(-LIGHTDIRECTION, normal);
    float angle = saturate(dot(reflectedRay, -invViewDirection));
    
    return pow(angle, exp) * specularSample.rgb;
}

float4 ObservedArea(float3 normal)
{
    float4 colour;
    colour.rgb = saturate(dot(normal, -LIGHTDIRECTION));
    colour.a = 1.f;
    return colour;
}

//------------------------------------------------
// Vertex Shader
//------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

    float3 modelSpacePosition = input.Position;

    output.Position = mul(float4(modelSpacePosition, 1.0f), gWorldViewProj);
    output.WorldPosition = mul(float4(input.Position, 1.f), gWorldViewProj);
    output.Uv = input.Uv;
    
    output.Normal = normalize(mul(input.Normal, (float3x3)gWorldMatrix));
    output.Tangent = normalize(mul(input.Tangent, (float3x3)gWorldMatrix));
	return output;
}



//------------------------------------------------
// Pixel Shader
//------------------------------------------------
float4 PS_Point(VS_OUTPUT input) : SV_TARGET
{
    float4 lambert = (gDiffuseMap.Sample(samPoint, input.Uv) * KD) / PI;
    
    float4 normal = SampleNormal(input.Normal, input.Tangent, gNormalMap.Sample(samPoint, input.Uv));
    
    float3 phong = PhongSpecular(input.WorldPosition.xyz, normal.xyz, gGlossinessMap.Sample(samPoint, input.Uv).r, gSpecularMap.Sample(samPoint, input.Uv));
    
    lambert.rgb += phong;
    return lambert * ObservedArea(normal.xyz);
}

float4 PS_Linear(VS_OUTPUT input) : SV_TARGET
{
    float4 lambert = (gDiffuseMap.Sample(samLinear, input.Uv) * KD) / PI;
    
    float4 normal = SampleNormal(input.Normal, input.Tangent, gNormalMap.Sample(samLinear, input.Uv));
    
    float3 phong = PhongSpecular(input.WorldPosition.xyz, normal.xyz, gGlossinessMap.Sample(samLinear, input.Uv).r, gSpecularMap.Sample(samLinear, input.Uv));
    
    lambert.rgb += phong;
    return lambert * ObservedArea(normal.xyz);
}
float4 PS_Anisotropic(VS_OUTPUT input) : SV_TARGET
{
    float4 lambert = (gDiffuseMap.Sample(samAnisotropic, input.Uv) * KD) / PI;
    
    float3 normal = SampleNormal(input.Normal, input.Tangent, gNormalMap.Sample(samAnisotropic, input.Uv));
    
    float3 phong = PhongSpecular(input.WorldPosition.xyz, normal.xyz, gGlossinessMap.Sample(samAnisotropic, input.Uv).r, gSpecularMap.Sample(samAnisotropic, input.Uv));
    
    lambert.rgb += phong;
    return lambert * ObservedArea(normal.xyz);
}


//------------------------------------------------
// Technique
//------------------------------------------------
technique11 DefaultTechnique
{
	pass P0
	{
        SetRasterizerState(gRasterizerState);
        SetDepthStencilState(gDepthStencilState, 0);
        SetBlendState(gBlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Point()));
    }
	pass P1
	{
        SetRasterizerState(gRasterizerState);
        SetDepthStencilState(gDepthStencilState, 0);
        SetBlendState(gBlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Linear()));
    }
	pass P2
	{
        SetRasterizerState(gRasterizerState);
        SetDepthStencilState(gDepthStencilState, 0);
        SetBlendState(gBlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_Anisotropic()));
    }
}