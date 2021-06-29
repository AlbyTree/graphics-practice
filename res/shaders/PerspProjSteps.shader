#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform vec3 u_NDCPos;

void main()
{
   gl_PointSize = 10.0;
   gl_Position = vec4(u_NDCPos, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.3, 0.5, 0.2, 1.0);
};
