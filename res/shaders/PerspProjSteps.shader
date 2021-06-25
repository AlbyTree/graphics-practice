#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform vec3 u_NDCPos;

void main()
{
   gl_Position = u_NDCPos;
   gl_PointSize = 10.0;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec3(0.3, 0.5, 0.2);
};
