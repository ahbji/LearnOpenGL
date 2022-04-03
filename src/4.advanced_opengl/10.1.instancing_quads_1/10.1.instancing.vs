#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

void main()
{
    fColor = aColor;

    // 从右上向左下缩小每个四边形
    vec2 pos = aPos * (gl_InstanceID / 100.0f);
    gl_Position = vec4(pos + aOffset, 0.0, 1.0);
}