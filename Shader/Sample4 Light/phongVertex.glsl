#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

out vec2 uv;
out vec3 worldNormal;
out vec3 worldPos;

// mvp矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1);
    worldPos = vec3(model * vec4(aPos,1.0));
    uv = texcoord;
    // mat3表示不使用平移变换
    // 对于不等比缩放,用逆矩阵转置修正
    worldNormal = mat3(transpose(inverse(model))) * normal;

    worldNormal = normalize(worldNormal);
}