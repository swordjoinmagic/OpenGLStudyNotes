#version 330 core
in vec2 uv;
out vec4 fragColor;

struct Light{
    vec3 position;
    vec3 color;
    float linear;
    float quadratic;
};

uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D gAldebo;

const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main(){
    vec3 worldPos = texture(gPos,uv).xyz;
    vec3 worldNormal = normalize(texture(gNormal,uv).xyz);
    vec4 aldebo = texture(gAldebo,uv);
    vec3 viewDir = normalize(viewPos - worldPos);

    // 最终颜色,初始为环境光颜色
    vec3 finalColor = 0.1 * aldebo.rbg;

    // 对每个光源分别执行光照计算
    for(int i=0;i<NR_LIGHTS;i++){
        Light light = lights[i];
        vec3 lightDir = normalize(light.position - worldPos);
        
        // 计算该片元的漫反射光照
        float NdotL = max(0,dot(lightDir,worldNormal));
        vec3 diffuse = NdotL * aldebo.rgb * light.color;

        // 计算该点的高光反射
        vec3 halfDir = normalize(lightDir + viewDir);
        float spec = pow(max(0,dot(worldNormal,halfDir)),32.0);
        vec3 specular = light.color * spec;

        // 计算光源衰减
        float distance = length(light.position - worldPos);
        float attenuation = 1.0 / (1.0+light.linear * distance + light.quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;

        finalColor += diffuse + specular;
    }
    
    fragColor = vec4(finalColor,1.0);
}