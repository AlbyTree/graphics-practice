#shader vertex
#version 330 core
layout(location = 0) in vec3 AttrPosition;

uniform mat4 u_MVP;

void main()
{
   gl_Position =  u_MVP * vec4(AttrPosition, 1.0f);
   gl_PointSize = 5.0;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.3f, 0.85f, 0.4f, 1.0f);
};
