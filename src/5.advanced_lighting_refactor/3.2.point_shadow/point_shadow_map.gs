#version 330 core
layout (triangles) in; // 输入三角形
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        // 指定发送的图元送到 cubemap 的哪个面
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            // 记录顶点的位置
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos; // 将顶点的位置转换为光源视角坐标
            EmitVertex();
        }    
        EndPrimitive();
    }
} 