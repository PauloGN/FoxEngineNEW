// Description: Simple shader that does transform.

cbuffer TransformBuffer : register(b0)
{
    matrix world;
	matrix wvp;
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float4 materialEmissive;
    float materialPower;
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
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
    
    matrix toWorld = world;
    matrix toNDC = wvp;    
    float3 localPosition = input.position;
    
    output.position = mul(float4(localPosition, 1.0f), toNDC);
    output.worldNormal = mul(input.normal, (float3x3) toWorld);
    output.worldTangent = mul(input.tangent, (float3x3) toWorld);
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - mul(float4(localPosition, 1.0f), toWorld).xyz);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    // set up normal from map
    float3 t = normalize(input.worldTangent);
    float3 b = normalize(cross(n, t));
    float3x3 tbnw = float3x3(t, b, n);
    float4 normalMapColor = normalMap.Sample(textureSampler, input.texCoord);
    float3 unpackedNormalMap = normalize(float3((normalMapColor.xy *2.0f) -1.0f, normalMapColor.z));
    
    n = normalize(mul(unpackedNormalMap, tbnw));
    
    //ambient color
    float4 ambient = lightAmbient * materialAmbient;
    
    //diffuse color
    float d = saturate(dot(light, n));
    float4 diffuse = d * lightDiffuse * materialDiffuse;
    
    //Specular color
    float3 r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, materialPower);
    float4 specular = s * lightSpecular * materialSpecular;
    
    //Get color from textures
    float4 diffuseMapColor = diffuseMap.Sample(textureSampler, input.texCoord);
    
    //Combine color for final result
    float4 finalColor = (ambient + diffuse + materialEmissive) * diffuseMapColor + specular;
    return finalColor;
}