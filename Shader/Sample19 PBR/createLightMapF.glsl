#version 330 core
out vec4 fragColor;
in vec3 modelPos;

uniform samplerCube environmentMap;

const float PI = 3.141559265359;

/*
    预处理环境贴图,将其变成一张辐照度贴图(原理是将环境贴图cubemap中每个纹素都当成光源来处理)
    何为辐照度贴图?
    即当我们使用观察向量采样该辐照度贴图时,得到的纹素就是
    某个片元在该观察方向上受到环境光作用的辐照度了~

    如何预处理:
    以采样方向(即观察方向)为原点,以该方向作为朝向,画一个半圆,
    在这个半圆内使用基于物理的BRDF公式积分就能得到当前采样方向的辐照度,
    这里可以用离散的几个固定采样方向(然后结果取平均)来模拟积分过程
*/
void main(){
    // 采样方向,也可以认为是观察方向viewDir
    vec3 normal = normalize(modelPos);
    vec3 irradiance = vec3(0.0);

    // 通过环境贴图,计算漫反射辐照度贴图
    vec3 up = vec3(0.0,1.0,0.0);
    vec3 right = cross(up,normal);
    up = cross(normal,right);

    float sampleDelta = 0.025;
    float nrSamples = 0.0;

    // 在当前观察方向(即采样方向)的半球内通过离散的固定的采样方向来模拟BRDF中的积分
    for(float phi=0.0;phi<2.0*PI;phi+=sampleDelta){
        for(float theta=0.0;theta<0.5*PI;theta+=sampleDelta){
            vec3 tangentSamle = vec3(
                sin(theta)*cos(phi),
                sin(theta)*sin(phi),
                cos(theta)
            );
            vec3 sampleVec = tangentSamle.x * right + tangentSamle.x * up + tangentSamle.z * normal;
            
            irradiance += texture(environmentMap,sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0/float(nrSamples));

    fragColor = vec4(irradiance,1.0);
}