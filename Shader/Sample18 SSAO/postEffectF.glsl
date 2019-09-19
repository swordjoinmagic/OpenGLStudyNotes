#version 330 core
out vec4 fragColor;
in vec2 uv;

uniform sampler2D gPosDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

struct Light{
    vec3 pos;
    vec3 color;
    
    float linear;
    float quadratic;
};

uniform Light light;

void main(){
    vec3 FragPos = texture(gPosDepth,uv).xyz;
    vec3 Normal = texture(gNormal,uv).xyz;
    vec3 Diffuse = texture(gAlbedo,uv).rgb;
    float AmbientOcculusion = texture(ssao,uv).r;

    vec3 ambient = vec3(0.3 * AmbientOcculusion);
    vec3 lighting = ambient;

    // 计算漫反射光照(在观察空间下)
    vec3 viewDir = normalize(-FragPos);
    vec3 lightDir = normalize(light.pos - FragPos);
    vec3 diffuse = max(dot(Normal,lightDir),0.0) * Diffuse * light.color;

    // 镜面光照(在观察空间下)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal,halfwayDir),0.0),8.0);
    vec3 specular = light.color * spec;

    // 计算衰减
    float dist = length(light.pos - FragPos);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * dist * dist);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

    fragColor = vec4(lighting,1.0);
}