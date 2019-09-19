#version 330 core
layout(location = 0) out vec4 gPosDepth;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAldebo;

in vec3 viewFragPos;
in vec2 uv;
in vec3 viewFragNormal;

uniform float near = 0.1f;
uniform float far = 50.0f;

// 将屏幕空间下的z分量变换为观察空间下的线性深度
float LinearizeDepth(float depth){
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main(){
    gPosDepth.xyz = viewFragPos;
    gPosDepth.a = LinearizeDepth(gl_FragCoord.z);
    gNormal = normalize(viewFragNormal);
    gAldebo = vec3(0.95);
}