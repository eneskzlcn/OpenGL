#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

layout(location = 1) in vec4 u_Color;

uniform mat4 u_MVP;

out	vec4 v_Color;

void main()
{
	gl_Position = u_MVP * vec4(position,1.0);
	v_Color = u_Color;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;

void main()
{
	o_Color = v_Color;
};