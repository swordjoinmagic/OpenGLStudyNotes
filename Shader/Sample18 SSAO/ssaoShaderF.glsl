#version 330 core
out float fragColor;
in vec2 uv;

uniform sampler2D gPosDepth;
uniform sampler2D gNormal;
uniform sampler2D NoiseTex;

// 随机采样点(在切线空间)的样本坐标
uniform vec3 samples[64];
uniform mat4 projection;

int kernelSize = 64;
float radius = 1.0;

const vec2 noiseScale = vec2(800.0f/4.0f , 600.0f/4.0f);

void main(){
    // 获得观察空间下的片元坐标/法线
    vec3 fragPos = texture(gPosDepth,uv).xyz;
    vec3 normal = texture(gNormal,uv).xyz;
    vec3 randomVec = texture(NoiseTex,uv*noiseScale).xyz;

    // 创建 切线-观察 变换矩阵
    vec3 tangent = normalize(randomVec - normal * dot(randomVec,normal));
    vec3 bitangent = cross(normal,tangent);
    mat3 TBN = mat3(tangent,bitangent,normal);


    // 遮蔽因子
    float occulsion = 0.0;
    for(int i=0;i<kernelSize;i++){
        // 获得观察空间下该随机样本的位置
        vec3 sample = TBN * samples[i];
        // radius为ssao的有效取样半径(即半球形区域的半径)
        sample = fragPos + sample * radius;

        // 将采样点变换到屏幕空间(以便接下来进行深度对比)
        vec4 offset = vec4(sample,1.0);
        offset = projection * offset;
        offset.xyz /= offset.w; // 齐次除法
        offset.xyz = offset.xyz * 0.5 + 0.5;    // 将xyz变换到[0,1]范围

        // 通过采样点的屏幕坐标,采样深度贴图获得该点像素的深度
        float sampleDepth = -texture(gPosDepth,offset.xy).w;

        // 范围测试,只有在被测深度值在取样半径内时影响遮蔽因子
        float rangeCheck = smoothstep(0.0,1.0,radius / abs(fragPos.z - sampleDepth));

        // 检查样本的深度是否大于存储的深度值,如果大于,则为一个遮蔽贡献
        // 简单来说,如果深度纹理中该点的深度更小,说明,前方有遮蔽物挡住了,所以深度测试后面的物体没有通过
        // 这里需要注意的是最后是 1-occulsion 计算的,所以 occulsion 越大,那么环境光遮蔽越少
        occulsion += (sampleDepth >= sample.z ? 1.0 : 0.0) * rangeCheck;
    }

    occulsion = 1.0 - (occulsion / kernelSize);
    fragColor = occulsion;
}