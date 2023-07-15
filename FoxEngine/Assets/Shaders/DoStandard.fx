// Description: Simple shader that does transform.
//Vertex shader
//PixelShader

cbuffer TransformBuffer : register(b0) //Bind vs or ps (b0)-> slots
{
    matrix world;
	matrix wvp;
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
    float3 lightDirection;
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float materialpower;
}

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
    float2 texCoord : TEXCOORD2;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldNormal = mul(input.normal, (float3x3)world);
    //output.worldTangent;
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - mul(float4(input.position, 1.0f), world).xyz);
    output.texCoord = input.texCoord;
     
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 L = normalize(input.dirToLight);
    float3 v = normalize(input.dirToView);
    
    float4 ambient = lightAmbient * materialAmbient;
    
    float d = saturate(dot(L, n));
    float4 diffuse = d * lightAmbient * materialDiffuse;
    
    float3 r = reflect(-L, n);
    float base = dot(r, v);
    float s = saturate(pow(base, materialpower));
    float4 specular = s * lightAmbient * materialSpecular;
    
    float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);
    return (ambient + diffuse) * textureColor + specular;
}