#version 330 core
out vec4 fragColor;

struct Light{
    vec3 direction; // 平行光的方向
    vec3 ambient;  // 环境光颜色
    vec3 diffuse;   // 漫反射颜色
    vec3 specular;   // 高光反射颜色
};

uniform Light light;
uniform sampler2D MainTex;
uniform vec3 viewPos;       // 观察位置
in vec3 worldPos;
in vec2 uv;
in vec3 worldNormal;

void main(){
    vec3 worldLightDir = normalize(light.direction);
    vec3 normal = normalize(worldNormal);

    vec4 ambient = vec4(light.ambient,1.0) * texture(MainTex,uv);

    // 计算辐照度
    float NdotL = max(0,dot(worldLightDir , normal));

    // 计算漫反射颜色
    vec4 diffuse = vec4(light.diffuse,1.0) * texture(MainTex,uv) * NdotL;

    // 计算高光反射
    
    // 计算从当前片元指向观察位置的方向
    vec3 viewDir = normalize( -worldPos + viewPos );
    vec3 reflectDir = normalize(reflect(-worldLightDir,normal));

    // 计算观察方向与反射方向的夹角,夹角越小,高光反射越强烈,即与cosΘ成正比
    float spec = pow(max(0,dot(viewDir,reflectDir)),32);

    // 计算高光反射颜色
    vec4 specular = spec * vec4(light.specular,1.0);

    fragColor = diffuse + specular + ambient;
}