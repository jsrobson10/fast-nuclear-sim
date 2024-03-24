R"GLSL(

#ifdef USE_BINDLESS_TEXTURES
#extension GL_ARB_bindless_texture : require
#endif

const float PI = 3.141592f;

in VS_OUT {
	mat3 tbn;
	vec3 pos;
	vec2 tex_pos;
	flat vec4 colour;
	flat vec3 material;
} vin;

struct Light
{
	vec4 pos;
	vec4 colour;
};

layout(std140, binding = 2) readonly buffer LightBuffer
{
	Light lights[];
};

#ifdef USE_BINDLESS_TEXTURES

in flat sampler2D frag_tex_diffuse;
in flat sampler2D frag_tex_normal;

#define ReadTexture(tex, uv) texture(tex, uv)

#else

in flat uint frag_tex_diffuse;
in flat uint frag_tex_normal;

uniform sampler2D tex_atlas;

struct AtlasPart
{
	vec2 uv_min;
	vec2 uv_max;
};

layout(std140, binding = 5) readonly buffer AtlasBuffer
{
	AtlasPart atlas[];
};

vec4 ReadTexture(uint tex, vec2 uv)
{
	AtlasPart a = atlas[tex];
	uv = mod(uv, 1.f) * (a.uv_max - a.uv_min) + a.uv_min;
	return texture(tex_atlas, uv);
}

#endif

out vec4 frag_colour;

uniform vec3 brightness;
uniform vec3 camera_pos;
uniform float far_plane;
uniform bool shadows_enabled;
uniform int lights_count;

uniform samplerCubeArrayShadow shadow_maps;

float Map(float v, float i_min, float i_max, float o_min, float o_max)
{
	return o_min + (o_max - o_min) * (v - i_min) / (i_max - i_min);
}

float Ramp(float v, float i_min, float i_max, float o_min, float o_max)
{
	float t = clamp(v, i_min, i_max);
	return Map(t, i_min, i_max, o_min, o_max);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.f - F0) * pow(clamp(1.f - cosTheta, 0.f, 1.f), 5.f);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a	  = roughness*roughness;
	float a2	 = a*a;
	float NdotH  = max(dot(N, H), 0.f);
	float NdotH2 = NdotH*NdotH;
	
	float num   = a2;
	float denom = (NdotH2 * (a2 - 1.f) + 1.f);
	denom = PI * denom * denom;
	
	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.f);
	float k = (r*r) / 8.f;

	float num   = NdotV;
	float denom = NdotV * (1.f - k) + k;
	
	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.f);
	float NdotL = max(dot(N, L), 0.f);
	float ggx2  = GeometrySchlickGGX(NdotV, roughness);
	float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

vec3 LinRGB_To_sRGB(vec3 c)
{
	bvec3 th = lessThan(c, vec3(0.0031308f));
	vec3 high = pow(c, vec3(1.0f / 2.4f)) * vec3(1.055f) - vec3(0.055f);
	vec3 low = c * vec3(12.92f);

	return mix(high, low, th);
}

vec3 sRGB_To_LinRGB(vec3 c)
{
	bvec3 th = lessThan(c, vec3(0.04045f));
	vec3 high = pow((c + vec3(0.055f)) * vec3(1.0f / 1.055f), vec3(2.4f));
	vec3 low = c * vec3(1.0f / 12.92f);

	return mix(high, low, th);
}

vec4 LinRGBA_To_sRGBA(vec4 c)
{
	return vec4(LinRGB_To_sRGB(c.rgb), c.a);
}

vec4 sRGBA_To_LinRGBA(vec4 c)
{
	return vec4(sRGB_To_LinRGB(c.rgb), c.a);
}

void main()
{
	vec4 albedo = sRGBA_To_LinRGBA(ReadTexture(frag_tex_diffuse, vin.tex_pos)) * vin.colour;
	
	if(albedo.a == 0)
	{
		discard;
	}

	if(vin.material[2] >= 1)
	{
		frag_colour = LinRGBA_To_sRGBA(albedo);
		return;
	}

	float luminance = min(vin.material[2], 1.f);
	vec3 tangent = ReadTexture(frag_tex_normal, vin.tex_pos).rgb * 2.f - 1.f;
	float roughness = vin.material[0];
	float metalness = vin.material[1];

	vec3 N = normalize(vin.tbn * tangent);
	vec3 V = normalize(camera_pos - vin.pos.xyz);

	vec3 F0 = mix(vec3(0.04f), albedo.rgb, metalness);
	vec3 ambient = vec3(Map(dot(N, vec3(0.f, 0.f, 1.f)), -1.f, 1.f, 0.2f, 0.25f)) * albedo.rgb * brightness;
	vec3 Lo = vec3(0.f);

	for(int i = 0; i < lights_count; i++)
	{
		Light l = lights[i];
		float d = length(vin.pos - l.pos.xyz);
		vec3 L = (l.pos.xyz - vin.pos) / d;
		float light_m;

		if(shadows_enabled)
		{
			if(dot(vin.tbn[2], L) < 0.f)
			{
				continue;
			}

			light_m = texture(shadow_maps, vec4(-L, i), d / far_plane);
		
			if(light_m <= 0.f)
			{
				continue;
			}
		}

		else
		{
			light_m = 1.f;
		}

		vec3 H = normalize(V + L);
		float atten = 1.f / (d*d);
		vec3 radiance = l.colour.rgb * atten;

		// cook-torrance brdf
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = FresnelSchlick(max(dot(H, V), 0.f), F0);
		
		vec3 kS = F;
		vec3 kD = vec3(1.f) - kS;
		kD *= 1.f - metalness;
		
		vec3 numerator = NDF * G * F;
		float denominator = 4.f * max(dot(N, V), 0.f) * max(dot(N, L), 0.f) + 1e-4f;
		vec3 specular = numerator / denominator;

		// add to outgoing radiance Lo
		float NdotL = max(dot(N, L), 0.f);
		Lo += (kD * albedo.rgb / PI + specular) * radiance * NdotL * light_m;
	}
	
	vec3 light = LinRGB_To_sRGB(ambient + Lo);
	light = mix(light, albedo.rgb, luminance);
	frag_colour = vec4(light, albedo.a);
}

)GLSL";
