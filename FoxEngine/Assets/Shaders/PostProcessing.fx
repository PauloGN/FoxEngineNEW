cbuffer PostProcessBuffer : register(b0)
{
    int mode;
    float param0;
    float param1;
    float param2;
    float param3;
    float param4;
    float param5;
    float param6;
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
        
    switch (mode)
    {
        case 0: // None
            finalColor = textureMap0.Sample(textureSampler, input.texcoord);
            break;
        case 1: // Monochrome
        {
                float4 color = textureMap0.Sample(textureSampler, input.texcoord);
                finalColor = (color.r + color.g + color.b) / 3.0f;
            }
            break;
        case 2: // Invert
        {
                float4 color = textureMap0.Sample(textureSampler, input.texcoord);
                finalColor = 1.0f - color;
            }
            break;
        case 3: // Mirror
        {
                float2 texCoord = input.texcoord;
                texCoord.x *= param0;
                texCoord.y *= param1;
                finalColor = textureMap0.Sample(textureSampler, texCoord);
            }
            break;
        case 4: // Blur
        {
                float u = input.texcoord.x;
                float v = input.texcoord.y;
                finalColor =
                textureMap0.Sample(textureSampler, float2(u, v)) +
                textureMap0.Sample(textureSampler, float2(u + param0, v)) +
                textureMap0.Sample(textureSampler, float2(u - param0, v)) +
                textureMap0.Sample(textureSampler, float2(u, v + param1)) +
                textureMap0.Sample(textureSampler, float2(u, v - param1)) +
                textureMap0.Sample(textureSampler, float2(u + param0, v + param1)) +
                textureMap0.Sample(textureSampler, float2(u + param0, v - param1)) +
                textureMap0.Sample(textureSampler, float2(u - param0, v + param1)) +
                textureMap0.Sample(textureSampler, float2(u - param0, v - param1));
            
                finalColor *= 0.15f;
            }
            break;
        case 5: // Combine2
        {
                float4 color0 = textureMap0.Sample(textureSampler, input.texcoord);
                float4 color1 = textureMap1.Sample(textureSampler, input.texcoord);
                finalColor = (color0 + color1) * 0.5f;
            }
            break;
        case 6: // MotionBlur
        {
                float u = input.texcoord.x;
                float v = input.texcoord.y;

                float dist = distance(input.texcoord, float2(0.5f, 0.5f));
                float weight = saturate(lerp(0.0f, 1.0f, (dist - 0.25f) / 0.25f));

                finalColor =
                textureMap0.Sample(textureSampler, float2(u, v)) +
                textureMap0.Sample(textureSampler, float2(u + param0 * weight, v)) +
                textureMap0.Sample(textureSampler, float2(u - param0 * weight, v)) +
                textureMap0.Sample(textureSampler, float2(u, v + param1 * weight)) +
                textureMap0.Sample(textureSampler, float2(u, v - param1 * weight)) +
                textureMap0.Sample(textureSampler, float2(u + param0 * weight, v + param1 * weight)) +
                textureMap0.Sample(textureSampler, float2(u + param0 * weight, v - param1 * weight)) +
                textureMap0.Sample(textureSampler, float2(u - param0 * weight, v + param1 * weight)) +
                textureMap0.Sample(textureSampler, float2(u - param0 * weight, v - param1 * weight));

                finalColor *= 0.15f;
            }
            break;
        case 7: // Smooth Bloom Effect
        {
                float4 color = textureMap0.Sample(textureSampler, input.texcoord);
                float4 coppyOfColor = textureMap2.Sample(textureSampler, input.texcoord);
                float4 blurredColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

                 // Apply a smoother blur to the bright areas of the image
                if (color.r + color.g + color.b > param0)
                {
                    float blurAmount = param1; // Adjust this value for intensity and blur size
                    int numSamples = 31; // Increase the number of samples for smoother results
                    float sampleWeight = 1.0 / float(numSamples * numSamples); // Weight of each sample

                    for (int i = -numSamples / 2; i <= numSamples / 2; i++)
                    {
                        for (int j = -numSamples / 2; j <= numSamples / 2; j++)
                        {
                            float2 offset = float2(i, j) * blurAmount;
                            blurredColor += textureMap0.Sample(textureSampler, input.texcoord + offset) * sampleWeight;
                        }
                    }
                }

                // Apply threshold to prevent over-brightening in the bloom
                blurredColor = max(blurredColor - param3, 0.0);

                // Combine the original color and the bloomed color
                float4 finalBloomedColor = color + blurredColor * param2;

                // Apply a smoother spread blur to simulate spreading of the glow
                float4 spreadBlur = float4(0.0f, 0.0f, 0.0f, 0.0f);
                float spreadBlurAmount = param4 * 0.5; // Reduce spread intensity for smoother effect
                int spreadSamples = 15; // Increase the number of samples for smoother results
                float spreadSampleWeight = 1.0 / float(spreadSamples * spreadSamples); // Weight of each sample

                for (int i = -spreadSamples / 2; i <= spreadSamples / 2; i++)
                {
                    for (int j = -spreadSamples / 2; j <= spreadSamples / 2; j++)
                    {
                        float2 offset = float2(i, j) * spreadBlurAmount;
                        spreadBlur += finalBloomedColor * spreadSampleWeight;
                    }
                }

                // Use coppyOfColor information to enhance glow
                float4 enhancedGlow = spreadBlur + coppyOfColor * param6; // Adjust param6 for intensity

                // Apply the final result
                    finalColor = color + enhancedGlow * param5; // Adjust intensity of spread glow
        }
            break;
        case 8: //Temperature Simulator
        {  
               
             float4 color = textureMap0.Sample(textureSampler, input.texcoord);
             float temperatureOffset = param0; // Adjust this to control temperature shift
    
             // Convert RGB to XYZ color space
             float3 XYZ = float3(
                    0.4124564 * color.r + 0.3575761 * color.g * color.b + 0.1804375,
                    0.2126729 * color.r + 0.7151522 * color.g * color.b + 0.0721750,
                    0.0193339 * color.r + 0.1191920 * color.g * color.b + 0.9503041
             );
    
             // Calculate the current color temperature
             float currentTemperature = XYZ.x;
    
             // Calculate the target color temperature
             float targetTemperature = currentTemperature + temperatureOffset;
    
             // Calculate von Kries chromatic adaptation matrix
             //https://santhalakshminarayana.github.io/blog/chromatic-adaptation
             float3 vonKriesMatrix = float3(
                 targetTemperature / currentTemperature,
                 1.0,
                 (1.0 - targetTemperature) / (1.0 - currentTemperature)
             );
    
             // Apply von Kries chromatic adaptation to XYZ
             XYZ *= vonKriesMatrix;
    
             // Convert XYZ back to RGB color space
             float3 correctedColor = float3(
                 3.2404542 * XYZ.x - 1.5371385 * XYZ.y - 0.4985314 * XYZ.z,
                -0.9692660 * XYZ.x + 1.8760108 * XYZ.y + 0.0415560 * XYZ.z,
                 0.0556434 * XYZ.x - 0.2040259 * XYZ.y + 1.0572252 * XYZ.z
             );
    
             correctedColor = clamp(correctedColor, 0.0f, 1.0f);
    
             // Apply smooth transition to the new color
             float3 blendedColor = lerp(color.rgb, correctedColor, param1); // Adjust param1 for smoothness
                         finalColor = float4(blendedColor, 1.0f);
        }
            break;
        default:
        // Handle any other cases here
            finalColor = textureMap0.Sample(textureSampler, input.texcoord);
            break;
    }
        
    return finalColor;
}