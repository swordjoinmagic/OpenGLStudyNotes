#version 330 core
out vec4 fragColor;

in vec2 uv;
in vec3 worldNormal;
in vec3 worldPos;

// 光源位置
uniform vec3 lightPos;

// 光源颜色
uniform vec4 lightColor;

// 摄像机位置
uniform vec3 viewPos;

// 主要纹理
uniform sampler2D MainTex;

void main(){

    // 获得环境光
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;

    // 从物体指向光源的方向
    vec3 lightDir = normalize(lightPos - worldPos);
    vec3 normal = normalize(worldNormal);
    // 计算辐照度
    float NdotL = max(0.0,dot(normal,lightDir));

    // 计算漫反射光照
    vec4 diffuse = NdotL * lightColor * texture(MainTex,uv);

    // 计算高光反射
    float specularStrength = 0.5;
    // 观察方向,由片元位置指向观察位置
    vec3 viewDir = normalize(viewPos - worldPos);
    // 计算反射向量
    vec3 reflectDir = reflect(-lightDir,normal);
    // 计算观察方向和反射方向的夹角,夹角越小,高光反射越强烈
    float spec = pow(max(dot(viewDir,reflectDir),0),32);

    vec4 specular = specularStrength * spec * lightColor;

    fragColor = diffuse + specular + ambient;
}