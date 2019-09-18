#version 330 core
out vec4 fragColor;
in vec2 uv;

uniform sampler2D image;
// 是否是横向高斯模糊
uniform bool horizontal;

// 5*5的矩阵进行高斯模糊
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main(){
    // 获得纹理像素的偏移值(即每个像素之间间隔多少)
    vec2 texOffset = 1.0 / textureSize(image,0);

    // 这是当前uv所对应的像素值对最终颜色所做的贡献
    vec3 result = texture(image,uv).rgb * weight[0];

    if(horizontal){
        for(int i=1;i<5;i++){
            result += texture(image,uv+vec2(texOffset.x * i,0.0)).rgb * weight[i];
            result += texture(image,uv-vec2(texOffset.x*i,0.0)).rgb * weight[i];
        }
    }else{
        for(int i=1;i<5;i++){
            result += texture(image,uv+vec2(0.0,texOffset.y*i)).rgb * weight[i];
            result += texture(image,uv-vec2(0.0,texOffset.y*i)).rgb * weight[i];
        }
    }

    fragColor = vec4(result,1.0);
}