#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{   
    // 获得所有颜色元素，然后将它们平均化
    // FragColor = texture(screenTexture, TexCoords);
    // float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // FragColor = vec4(average, average, average, 1.0);

    // 为了获得更精确的符合人体物理的结果，我们需要使用加权通道
    FragColor = texture(screenTexture, TexCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
} 