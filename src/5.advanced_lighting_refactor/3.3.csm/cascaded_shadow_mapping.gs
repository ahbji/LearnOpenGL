#version 460 core

// GS 被执行 5 次
layout(triangles, invocations = 5) in; // invocations 最大值 MAX_GEOMETRY_SHADER_INVOCATIONS
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

void main()
{          
	for (int i = 0; i < 3; ++i)
	{
		// gl_InvocationID 范围 [0,5)
		gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
}  