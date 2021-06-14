#shader vertex
#version 330 core
// We need to pass to the GPU where to find the positions of the vertices:
// as our index attrib that describes the positions is 0 we use location = 0.
// Than, it will use the other informations we passed in the vertexattribpointer to
// locate the components we need.
// As our index 0 attrib use 2 components it is a vec2:
// OpenGL can cast it to a vec4 where the other 2 components have a default value.
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};
