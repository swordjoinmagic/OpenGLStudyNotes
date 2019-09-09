#version 330 core
out vec4 fragColor;

float near = 0.1;
float far = 100.0;

// 线性化深度值(将深度缓冲中非线性的值变为在near-far之间的线性深度值)
float LinearizeDepth(float depth){
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far-near) );
}

void main(){
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    fragColor = vec4(vec3(depth),1.0);
}