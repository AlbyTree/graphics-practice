#shader vertex
#version 330 core
layout(location = 0) in vec3 AttrPosition;

uniform mat4 u_MVP;
uniform vec3 u_Color;

out vec4 VertColor;

void main()
{
   gl_Position =  u_MVP * vec4(AttrPosition, 1.0f);
   gl_PointSize = 5.0;

   VertColor = vec4(u_Color, 1.0f);
};

#shader fragment
#version 330 core

in vec4 VertColor;

layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor = VertColor;
};
