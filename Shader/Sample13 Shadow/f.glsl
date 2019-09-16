#version 330 core
out vec4 fragColor;
in vec2 uv;
in vec3 worldNormal;
in vec3 worldPos;
in vec4 lightClipPos;

uniform sampler2D MainTex;
uniform sampler2D depthMap;

// 点光源深度图
uniform samplerCube pointDepthMap;
uniform vec3 lightPos;
uniform float far_plane;

float PointShadowCalculation(vec3 fragPos){
    vec3 fragToLight = fragPos - lightPos;
    float closestDepth = texture(pointDepthMap,fragToLight).r;
    closestDepth *= far_plane;

    float currentDepth = length(fragToLight);

    // 比较最近深度和点光源空间深度
    float bias = 0.1;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main(){

    // 当前片元在光源裁剪空间的坐标
    // vec4 lightClipPos = lightView * lightProjection * vec4(worldPos,1.0);
    // 齐次除法变换到NDC空间
    vec3 lightNDCPos = lightClipPos.xyz / lightClipPos.w;
    lightNDCPos = lightNDCPos * 0.5 + 0.5;
    // 对NDC空间的x,y进行缩放
    vec2 xy = lightNDCPos.xy;

    // 采样当前片元对应光源空间的最小深度
    float lightCloestDepth = texture(depthMap,xy).r;

    // 获得当前片元在光源空间的深度
    float currentFragDepth = lightNDCPos.z;

    // 阴影偏移,用于纠正阴影失真(shadow acne)的效果
    float bias = 0.01;

    vec4 finalColor = texture(MainTex,uv);

    float shadow = 0.0;

    // 使用PCF重复采样
    vec2 texelSize = 1.0 / textureSize(depthMap,0);
    for(int x=-1;x<=1;x++){
        for(int y=-1;y<=1;y++){
            float pcfDepth = texture(depthMap,xy+vec2(x,y)*texelSize).r;
            // 比较当前片元在光源空间的深度和当前片元对应光源空间的最小深度
            // 如果当前片元在光源空间大于最近深度,那么当前片元不可见
            shadow += currentFragDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    float pointShadow = PointShadowCalculation(worldPos);

    shadow = lightNDCPos.z>1.0?0:shadow;

    fragColor = finalColor * (1-pointShadow) * (1-shadow);    
}