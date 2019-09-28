#version 330 core
out vec4 fragColor;
in vec2 uv;
in vec3 worldPos;
in vec3 worldNormal;

uniform vec3 viewPos;

// 材质颜色
uniform vec3 albedo;
// 这里目前暂时用一个浮点数控制整体的金属度/粗糙度等
// 金属度
uniform float metallic;
// 粗糙度
uniform float roughness;
// 环境光遮蔽
uniform float ao;

//===========================
// 间接光部分
// 漫反射辐照度贴图
uniform samplerCube irradianceMap;



// 四个点光源位置及颜色
uniform vec3 lightPos[4];
uniform vec3 lightColor[4];

const float PI = 3.14159265359;

// 菲涅尔反射函数,可以用来计算漫反射和高光反射的能量比值,
// f0为菲涅尔系数,cosTheta:观察方向与法线夹角的cos
vec3 fresnelSchlick(float cosTheta,vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0-cosTheta,5.0);
}

// 法线分布函数
float DistributionGGX(vec3 N,vec3 H,float roughness){
    // α表示表面粗糙度,h为中间向量,N为表面法线
    float a = roughness * roughness;
    float a2 = a*a;
    float NdotH = max(0,dot(N,H));
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2-1.0) + 1.0);
    denom = PI * denom * denom;

    return nom/denom;
}

// 阴影遮盖函数(用于表示有多少入射/出射光被微平面挡住了)
float GeometrySchlickGGX(float NdotV,float roughness){
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness){
    float NdotV = max(dot(N,V),0.0);
    float NdotL = max(dot(N,L),0.0);
    float ggx2 = GeometrySchlickGGX(NdotV,roughness);
    float ggx1 = GeometrySchlickGGX(NdotL,roughness);

    return ggx1*ggx2;
}

void main(){
    vec3 worldViewDir = normalize(viewPos - worldPos);
    vec3 normal = normalize(worldNormal);


    // 最后出射光的辐照度(由所有光源的渲染方程得出(也就是计算漫反射和高光反射BRDF))
    vec3 L0 = vec3(0.0);
    // 对所有点光源进行直接光照计算
    for(int i=0;i<4;i++){
        // 计算光源方向
        vec3 worldLightDir = normalize(lightPos[i] - worldPos);
        // h向量(即可以将入射光反射至观察方向的标准法线)
        vec3 halfDir = normalize(worldLightDir + worldViewDir);

        // 计算光源衰减因子
        float distance = length(lightPos[i] - worldPos);    // 光源距离片元的位置
        float attenuation = 1.0/(distance * distance);  // 只使用而二次项衰减
        // 计算经过衰减后的辐照度
        vec3 radiance = lightColor[i] * attenuation;

        //===============================
        // 计算PBR直接光照中的高光反射
        // 高光反射BRDF = DGF / 4(viewDir · normal)(lightDir · normal)


        // 计算F函数(即高光反射中的菲涅尔函数),用于计算高光反射与漫反射光线的比值
        // 即反射占所有光线(反射/折射)的比值
        vec3 F0 = vec3(0.04);
        // 对于绝缘体来说,它的菲涅尔系数在0.04的时候是比较正常的,
        // 对于金属表面,会吸收所有漫反射(折射)光线.
        F0 = mix(F0,albedo,metallic);
        vec3 F = fresnelSchlick(max(dot(halfDir,worldViewDir),0.0),F0);

        // 计算D函数(即法线分布函数),用于计算有多少微面元的法线取向与h一致
        float NDF = DistributionGGX(normal,halfDir,roughness);
        float G = GeometrySmith(normal,worldViewDir,worldLightDir,roughness);

        // 计算高光反射BRDF
        vec3 nominator = NDF * G * F;
        // +0.001避免除0
        float denominator = 4.0 * max(dot(normal,worldViewDir),0.0) * max(dot(normal,worldLightDir),0.0) + 0.001;
        vec3 specular = nominator / denominator;

        // 计算折射光在总的光线中的比值
        vec3 KS = F;    // 高光反射占所有光线的比值
        vec3 KD = vec3(1.0 - KS);   // 折射光(即漫反射)占所有光线的比值

        // 对于金属物体,它吸收所有折射光
        KD *= 1.0 - metallic;

        //==========================
        // 计算直接光照的漫反射BRDF

        float NdotL = max(dot(normal,worldLightDir),0.0);
        // 其中可以认为漫反射的BRDF=c/π,radiance为L函数(即入射光辐照度)
        L0 += (KD * albedo / PI + specular) * radiance * NdotL;
    }
    vec3 F0 = vec3(0.04);
    F0 = mix(F0,albedo,metallic);
    vec3 kS = fresnelSchlick(max(dot(normal,worldViewDir),0.0),F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    vec3 irradiance = texture(irradianceMap,normal).rgb;
    vec3 diffuse = irradiance * albedo;
    vec3 ambient = (kD * diffuse) * ao;
    // ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + L0;
    // 伽马矫正
    color = color / (color + vec3(1.0));
    color = pow(color,vec3(1.0/2.2));

    fragColor = vec4(color,1.0);
}