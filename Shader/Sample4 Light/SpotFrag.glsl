#version 330 core
out vec4 fragColor;

struct Light{
    vec3 lightPos;  // 点光源的位置
    
    vec3 ambient;  // 环境光颜色
    vec3 diffuse;   // 漫反射颜色
    vec3 specular;   // 高光反射颜色

    // 点光源衰减公式: atten = 1.0 / (Kc + kl*d + kq*d^2)
    float constant; // 衰减公式的常数项
    float linear;   // 衰减公式的一次项
    float quadratic;    // 衰减公式的二次项

    // 聚光灯外角,当物体与聚光灯形成的夹角Θ>外角时,
    // 该物体不会被聚光灯照亮
    float cutOff;
    // 聚光灯所指向的方向
    vec3 spotDirection;
    // 聚光灯内角,在内角范围的物体被照亮的幅度最大,
    // 被照亮的幅度从内角到外角逐级减少
    float innerCone;
};

uniform Light light;
uniform sampler2D MainTex;
uniform vec3 viewPos;       // 观察位置
in vec3 worldPos;
in vec2 uv;
in vec3 worldNormal;

void main(){
    // 从物体指向光源的方向
    vec3 worldLightDir = normalize( -worldPos + light.lightPos );
    vec4 ambient = vec4(light.ambient,1.0) * texture(MainTex,uv);
    // 计算夹角Θ值
    float theta = dot(worldLightDir,normalize(-light.spotDirection));
    float epsilon = light.innerCone - light.cutOff;
    float intensity = clamp((theta-light.cutOff)/epsilon,0.0,1.0);

    vec3 normal = normalize(worldNormal);
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

    // 计算光源衰减值
    float distance = length(light.lightPos - worldPos);  // 光源距离物体的距离
    float atten = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance*distance));

    fragColor = intensity * atten * ( diffuse + specular) + ambient;
}