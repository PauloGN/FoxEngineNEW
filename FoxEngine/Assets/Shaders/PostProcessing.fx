cbuffer PostProcessBuffer : register(b0)
{
    int mode;
    float param0;
    float param1;
    float param2;
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texcoord = input.texCoord;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 0.0f;
    
    if (mode == 0)//None
    {
         finalColor = textureMap0.Sample(textureSampler, input.texcoord);
    }
    else if (mode == 1)//Monochrome
    {
        float4 color = textureMap0.Sample(textureSampler, input.texcoord);
        finalColor = (color.r + color.g + color.b) / 3.0f;
    }
    else if (mode == 2)//Invert
    {
        float4 color = textureMap0.Sample(textureSampler, input.texcoord);
        finalColor = 1.0f - color;
    }
    else if (mode == 3)//Mirror
    {
        float2 texCoord = input.texcoord;
        texCoord.x *= param0;
        texCoord.y *= param1;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    else if (mode == 4)//Blur
    {
        float u = input.texcoord.x;
        float v = input.texcoord.y;
        finalColor =
        textureMap0.Sample(textureSampler,   float2(u, v))
        + textureMap0.Sample(textureSampler, float2(u + param0, v))
        + textureMap0.Sample(textureSampler, float2(u - param0, v))
        + textureMap0.Sample(textureSampler, float2(u, v + param1))
        + textureMap0.Sample(textureSampler, float2(u, v - param1))
        + textureMap0.Sample(textureSampler, float2(u + param0, v + param1));
        + textureMap0.Sample(textureSampler, float2(u + param0, v - param1));
        + textureMap0.Sample(textureSampler, float2(u - param0, v + param1));
        + textureMap0.Sample(textureSampler, float2(u - param0, v - param1));
        
        finalColor *= 0.15f;
    }
    else if (mode == 5)//Combine2
    {
        float4 color0 = textureMap0.Sample(textureSampler, input.texcoord);
        float4 color1 = textureMap1.Sample(textureSampler, input.texcoord);
        
        finalColor = (color0 + color1) * 0.5f;
    }
    else if (mode == 6)//MotionBlur
    {
        float u = input.texcoord.x;
        float v = input.texcoord.y;
        
        float dist = distance(input.texcoord, float2(0.5f, 0.5f));
        float weight = saturate(lerp(0.0f, 1.0f, (dist - 0.25f) / 0.25f));
        
        finalColor =
        textureMap0.Sample(textureSampler, float2(u, v))
        + textureMap0.Sample(textureSampler, float2(u + param0 * weight, v))
        + textureMap0.Sample(textureSampler, float2(u - param0 * weight, v))
        + textureMap0.Sample(textureSampler, float2(u, v + param1 * weight))
        + textureMap0.Sample(textureSampler, float2(u, v - param1 * weight))
        + textureMap0.Sample(textureSampler, float2(u + param0 * weight, v + param1 * weight));
        +textureMap0.Sample(textureSampler, float2(u +  param0 * weight, v - param1 * weight));
        +textureMap0.Sample(textureSampler, float2(u -  param0 * weight, v + param1 * weight));
        +textureMap0.Sample(textureSampler, float2(u - param0 * weight, v -  param1 *  weight));
        
        finalColor *= 0.15f;
        
    }
    
    return finalColor;
}