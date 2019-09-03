#include "main.h"
#include "Sample2_Texture.h"
#include "stb_image.h"

void Sample2::initVertex() {
	// 顶点数据(包括位置/颜色/uv属性)
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};
	unsigned int VBO;

	// 创建顶点数组对象,并绑定
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//创建顶点缓冲对象
	glGenBuffers(1, &VBO);
	// 绑定顶点缓冲对象,并把顶点数据输入进缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 生成顶点索引缓冲
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	// 根据顶点索引数组生成顶点缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// 绑定顶点索引缓冲,并把数据输进去
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置各顶点属性指针,并启用顶点属性

	// 设置顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 设置顶点颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 设置顶点uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Sample2::createTexture() {
	// 生成纹理对象
	glGenTextures(1, &texture);
	// 绑定纹理,后续所有纹理指令都可以配置当前绑定的纹理
	glBindTexture(GL_TEXTURE_2D, texture);

	// 设置当前绑定纹理对象的采样/过滤模式

	// 设置采样模式为repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 设置纹理过滤模式为线性过滤模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 读取纹理
	int width, height, nrChannels;
	// 返回该图像的宽度/高度/颜色通道个数
	unsigned char *data = stbi_load("Image/container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		// 使用前面载入的图片数据生成一个纹理
		glTexImage2D(
			GL_TEXTURE_2D,	// 目标纹理缓冲类型
			0,				// 多级减员纹理的级别
			GL_RGB,			// 告诉OpenGL把纹理存储为何种格式
			width, height,	// 纹理的宽度和高度
			0,				// 应该总是被设为0
			GL_RGB, GL_UNSIGNED_BYTE, // 定义了原图的格式和数据类型
			data			// 图像数据
		);
		// 生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "无法读取纹理" << std::endl;
	}
	// 生成纹理对象后,将读取进来的图像内存释放
	stbi_image_free(data);
}

void Sample2::init() {
	// 初始化着色器
	shader = std::make_shared<Shader>("Shader/Sample2/vertex.glsl", "Shader/Sample2/fragment.glsl");

	initVertex();
	createTexture();
}

void Sample2::render() {

	glUseProgram(shader->ID);

	glBindTexture(GL_TEXTURE_2D,texture);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
}

void Sample2::onDestory() {
	glDeleteVertexArrays(1,&VAO);
}