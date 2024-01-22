#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

#include <unordered_map>
#include "render/Shader.hpp"

namespace ShaderManager
{
	static std::unordered_map<std::string, Shader> shaders;

	static void Register(Shader shader)
	{
		shaders.insert({ shader.name, shader });
	}

	static void Generate()
	{
		for(auto& [name, shader] : shaders)
			shader.Generate();
	}

	static Shader& Get(const std::string& name)
	{
		return shaders[name];
	}

	static void CleanUp()
	{
		for (auto& [name, shader] : shaders)
			shader.CleanUp();
	}
}

#endif // !SHADER_MANAGER_HPP