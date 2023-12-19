#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 NormalV;
out vec3 Pos;
out vec2 TexCoord;
uniform float alfa;
uniform float beta;

void main()
{
	TexCoord = aTexCoord;
	Pos = aPos;
	
	Pos.x = Pos.x - 1/2.0;
	Pos.y = Pos.y - 1/2.0;
	
	float x = Pos.x;
	Pos.x = Pos.x * cos(alfa) - Pos.y * sin(alfa);
	Pos.y = x * sin(alfa) + Pos.y * cos(alfa);
	
	float y = Pos.y;
	Pos.y = y * cos(beta) - Pos.z * sin(beta);
	Pos.z = y * sin(beta) + Pos.z * cos(beta);
	
	Pos.x = Pos.x + 1/2.0;
	Pos.y = Pos.y + 1/2.0;
	
	NormalV = aNormal;
    gl_Position = vec4(Pos.xyz, 1.0f);
}