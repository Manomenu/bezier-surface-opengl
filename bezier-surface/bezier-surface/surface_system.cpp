#include "surface_system.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <filesystem>


bool SurfaceSystem::filling = 1;
bool SurfaceSystem::grid = 0;

uint SurfaceSystem::TextureFromFile(const char* path/*, const std::string& directory*/)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


SurfaceSystem::SurfaceSystem(std::shared_ptr<Shader> shader, std::shared_ptr<Shader> texShader) : shader(shader), texShader(texShader)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			control_points[j + 4 * i] = glm::vec3(
				i * 1.0f / 3.0f,
				j * 1.0f / 3.0f,
				0
			);
		}
	}

	std::vector<Vertex> vertices = updated_vertices();
	std::vector<uint> indices = updated_indices();
	auto diffTexture = Texture();
	diffTexture.path = std::filesystem::current_path().string() + "/textures/baboon.jpg";
	diffTexture.id = TextureFromFile(diffTexture.path.c_str());
	diffTexture.type = "texture_diffuse";
	textures.push_back(diffTexture);
	auto normalTex = Texture();
	normalTex.path = std::filesystem::current_path().string() + "/textures/4631-normal.jpg";
	normalTex.id = TextureFromFile(normalTex.path.c_str());
	normalTex.type = "texture_normal";
	textures.push_back(normalTex);

	_surface = Mesh(vertices, indices, textures);
}

void SurfaceSystem::update_triangles_density(int density)
{
	triangles_density = density;
	update();
}

void SurfaceSystem::draw()
{
	if (filling)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (fill_mode == 0)
			_surface.Draw(*shader);
		else if (fill_mode == 1)
			_surface.Draw(*texShader);
	}
	if (grid)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		

				if (fill_mode == 0)
				{
					shader->setVec3("ObjectColor", 0, 0, 0);
					_surface.Draw(*shader);
				}
				else if (fill_mode == 1)
				{
					texShader->setVec3("ObjectColor", 0, 0, 0);
					_surface.Draw(*texShader);
				}
	}
}

void SurfaceSystem::update()
{
	_surface.Update(updated_vertices(), updated_indices());
}

void SurfaceSystem::update_reference_points(float points[16])
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			// up left up left up left etc.
			control_points[i + j * 4].z = points[i + j * 4];
		}
	}

	update();
}

std::vector<Vertex> SurfaceSystem::updated_vertices()
{
	std::vector<Vertex> res;
	float step = 1.0 / triangles_density;

	for (float i = 0; i <= 1.0001; i += step)
	{
		for (float j = 0; j <= 1.0001; j += step)
		{
			res.push_back(
				{
					glm::vec3(i, j, calculate_height(i,j)),
					calculate_normal_vec(i , j),
					glm::vec2(i, j)
				}
			);

		}
	}

	return res;
}

float SurfaceSystem::calculate_width(float x, float y)
{
	float res = 0;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			res += control_points[i + j * 4].x * B(i, 3, x) * B(j, 3, y);
			//std::cout << "h: " << control_points[i + 4 * j].z << "x: " << control_points[i + 4 * j].x << "y: " << control_points[i + 4 * j].y << "\n";
		}
	}

	return res;
}
float SurfaceSystem::calculate_y(float x, float y)
{
	float res = 0;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			res += control_points[i + j * 4].y * B(i, 3, x) * B(j, 3, y);
			//std::cout << "h: " << control_points[i + 4 * j].z << "x: " << control_points[i + 4 * j].x << "y: " << control_points[i + 4 * j].y << "\n";
		}
	}

	return res;
}


#pragma region help

std::vector<uint> SurfaceSystem::updated_indices()
{
	std::vector<uint> res;

	for (int i = 0; i < triangles_density; i++)
		for (int j = 0; j < triangles_density; j++)
		{
			//lower triangle
			


			//upper triangle
			res.push_back(j + (triangles_density + 1) * i);
			res.push_back(j + (triangles_density + 1) * (i + 1));
			res.push_back(j + 1 + (triangles_density + 1) * (i + 1));

			res.push_back(j + (triangles_density + 1) * i);
			res.push_back(j + 1 + (triangles_density + 1) * (i + 1));
			res.push_back(j + 1 + (triangles_density + 1) * i);
		}

	return res;
}

float SurfaceSystem::calculate_height(float x, float y)
{
	//system("cls");
	//std::cout << std::fixed << std::setprecision(2);
	float res = 0;
	if (!powFun)
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
			
					res += control_points[i + j * 4].z * B(i, 3, x) * B(j, 3, y);
				
					//res = 
				//std::cout << "h: " << control_points[i + 4 * j].z << "x: " << control_points[i + 4 * j].x << "y: " << control_points[i + 4 * j].y << "\n";
			}
		}
	else
		res = glm::sin(1 / 2.0f * M_PI * (x + y));

	return res;
}

int SurfaceSystem::calculateNewtonSymbol(unsigned int n, unsigned int k) {
	if (k > n)
		return 0;

	if (k == 0 || k == n)
		return 1;

	double Wynik = 1;     

	for (unsigned int i = 1; i <= k; i++)  
	{
		Wynik = Wynik * (n - i + 1) / i;    
	}

	return (int)Wynik;      
}

float SurfaceSystem::B(int i, int n, float t)
{
	return std::pow(t, i) * std::pow((1 - t), n - i) * calculateNewtonSymbol(n, i);
}


void SurfaceSystem::update_uniforms(int fill_mode,
	glm::vec3 solid_color, glm::vec3 light_color,
	float kd, float ks, int m, float alfa, float beta)
{
	this->fill_mode = fill_mode;
	if (fill_mode == 0) // solid color
	{
		shader->use();
		shader->setVec3("ObjectColor", solid_color);
		shader->setVec3("LightColor", light_color);
		shader->setFloat("kd", kd);
		shader->setFloat("ks", ks);
		shader->setInt("m", m);
		shader->setFloat("alfa", alfa);
		shader->setFloat("beta", beta);
	}
	else if (fill_mode == 1)
	{
		texShader->use();
		texShader->setVec3("LightColor", light_color);
		texShader->setFloat("kd", kd);
		texShader->setFloat("ks", ks);
		texShader->setInt("m", m);
		texShader->setFloat("alfa", alfa);
		texShader->setFloat("beta", beta);
		texShader->setVec3("ObjectColor", glm::vec3(1, 1, 1));
	}
	
}

void SurfaceSystem::update_diffuse(std::string path)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i].type == "texture_diffuse")
		{
			textures[i].path = path;
			textures[i].id = TextureFromFile(path.c_str());
			_surface.UpdateTextures(textures);
			break;
		}
	}
}

void SurfaceSystem::update_normal(std::string path)
{
	auto normalTex = Texture();
	normalTex.path = path;
	normalTex.id = TextureFromFile(path.c_str());
	normalTex.type = "texture_normal";

	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i].type == "texture_normal")
		{
			textures[i].path = path;
			textures[i].id = normalTex.id;
			_surface.UpdateTextures(textures);
			break;
		}
	}
	textures.push_back(normalTex);
}

void SurfaceSystem::update_normalMap_status(bool isNormalMapOn)
{
	texShader->use();
	texShader->setBool("WithNormal", isNormalMapOn);
	shader->use();
	shader->setBool("WithNormal", isNormalMapOn);
}

void SurfaceSystem::update_powFun(int val)
{
	if (val != powFun)
	{
		powFun = val;
		update();
	}
}

glm::vec3 SurfaceSystem::calculate_normal_vec(float x, float y)
{
	glm::vec3 P_u = { 0, 0, 0 };
	glm::vec3 P_v = { 0, 0, 0 };


	if (!powFun)
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				P_u.z +=(control_points[i + 1 + j * 4].z - control_points[i + j * 4].z) 
					* B(i, 2, x) * B(j, 3, y);
				P_u.x +=(control_points[i + 1 + j * 4].x - control_points[i + j * 4].x) 
					* B(i, 2, x) * B(j, 3, y);
				P_u.y +=(control_points[i + 1 + j * 4].y - control_points[i + j * 4].y) 
					* B(i, 2, x) * B(j, 3, y);
			}
		}
	else
	{
		P_u = { 1, 0,  1 / 2.0f * M_PI * glm::cos(1 / 2.0f * M_PI * (x + y)) };
	}
	
	if (!powFun)
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			P_v.z += (control_points[i + (j + 1) * 4].z - control_points[i + j * 4].z) 
				* B(i, 3, x) * B(j, 2, y);
			P_v.x +=(control_points[i + (j + 1) * 4].x - control_points[i + j * 4].x) 
				* B(i, 3, x) * B(j, 2, y);
			P_v.y +=(control_points[i + (j + 1) * 4].y - control_points[i + j * 4].y) 
				* B(i, 3, x) * B(j, 2, y);
		}
	}
	else
		P_v = { 0, 1,  1 / 2.0f * M_PI * glm::cos(1 / 2.0f * M_PI * (x + y)) };
	if (!powFun)
	{
		P_u.x *= 3;
		P_u.y *= 3;
		P_u.z *= 3;
		P_v.x *= 3;
		P_v.y *= 3;
		P_v.z *= 3;
	}

	if (!powFun)
	{
		auto N = glm::normalize(glm::cross(P_v, P_u));
		//std::cout << N.x << " " << N.y << " " << N.z << "\n";
		return N;
	}
	else
		return glm::normalize(glm::cross(P_u, P_v));
}


#pragma endregion









