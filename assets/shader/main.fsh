
#version 460 core
#extension GL_ARB_bindless_texture : require

const float PI = 3.141592;
const float Epsilon = 0.00001;

// Constant normal incidence Fresnel factor for all dielectrics.
const vec3 Fdielectric = vec3(0.04);

in VS_OUT {
	vec3 normal;
	vec4 colour;
	vec3 pos;
	vec2 tex_pos;
	vec3 material;
} vin;

struct Light
{
	vec4 pos;
	vec4 colour;
};

layout(std140, binding = 1) buffer ssbo_lights
{
	Light lights[];
};

in flat sampler2D frag_tex;
out vec4 frag_colour;

uniform mat4 tex_mat;
uniform vec3 brightness;
uniform vec3 camera_pos;

uniform int lights_count;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

void main()
{
	vec4 albedo = (tex_mat * texture2D(frag_tex, vin.tex_pos)) * vin.colour;
	
	float roughness = vin.material[0];
	float metalness = vin.material[1];
	float luminance = min(vin.material[2], 1.f);

	vec3 N = normalize(vin.normal);
	vec3 V = normalize(camera_pos - vin.pos.xyz);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo.rgb, metalness);

	vec3 Lo = vec3(0.0);
	for(int i = 0; i < lights_count; i++)
	{
		Light l = lights[i];

		vec3 L = normalize(l.pos.xyz - vin.pos);
		vec3 H = normalize(V + L);

		float d = length(vin.pos - l.pos.xyz);
		float atten = 1.f / (d*d);
		vec3 radiance = l.colour.rgb * atten;

		// cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metalness;
        
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo.rgb / PI + specular) * radiance * NdotL;
	}
	
	vec3 ambient = vec3(0.03f) * albedo.rgb * brightness;
	vec3 light = ambient + Lo;

	light = light / (light + vec3(1.f));
	light = pow(light, vec3(1.f/2.2f));
	light = light * (1 - luminance) + albedo.rgb * luminance;
	frag_colour = vec4(light, albedo.a);

	if(frag_colour.a == 0.f) discard;
}

