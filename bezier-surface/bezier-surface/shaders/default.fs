#version 330 core
out vec4 FragColor;

in vec3 NormalV;
in vec3 Pos;
in vec2 TexCoord;

uniform bool WithNormal;
uniform sampler2D texture_normal1;
uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform float kd;
uniform float ks; 
uniform vec3 LightV;
uniform int m;

void main()
{           
	vec2 fix = TexCoord;
	
	
	vec3 N = NormalV;
	
	if (WithNormal)
	{
		vec3 N_tex = texture(texture_normal1, TexCoord).rgb;
		N_tex = normalize(N_tex * 2.0 - 1.0);
		
		
		vec3 B = cross(NormalV, vec3(0.0,0.0,1.0));
		vec3 T = cross(B, NormalV);
		
		
		mat3 M = mat3(T.x, T.y, T.z, B.x, B.y, B.z, N.x, N.y, N.z);
		//M[0] = T;
		//M[1] = B;
		//M[2] = NormalV;
		
		N = normalize(M * N_tex);
	}
  
	vec3 L = normalize(LightV - Pos);
	vec3 Diffusal = kd * ObjectColor * LightColor * clamp(dot(N, L),0.0,1.0);
	vec3 V = vec3(0.0, 0.0, 1.0);
	vec3 R = normalize(2 * dot(N, L) * N - L);
	vec3 Spectral = ks * ObjectColor * LightColor * pow(clamp(dot(V, R),0.0,1.0), m);
	vec3 Final = Diffusal + Spectral;

    FragColor = vec4(clamp(Final,0.0,1.0), 1.0);
}  