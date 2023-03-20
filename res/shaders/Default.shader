#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 vs_Color;

void main()
{
   gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.f);
   vs_Color = a_Color;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 fs_Color;

in vec4 vs_Color;

void main()
{
	fs_Color = vs_Color;
};
