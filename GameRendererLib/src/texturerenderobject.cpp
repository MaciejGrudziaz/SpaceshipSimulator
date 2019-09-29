#include <stdafx.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureRenderObject::TextureRenderObject()
	: texture(0)
{}

TextureRenderObject::TextureRenderObject(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
	: RenderObject(vertexShaderFilename, fragmentShaderFilename)
	, texture(0)
{}

void TextureRenderObject::deepCopy(const RenderObject& object)
{
	RenderObject::deepCopy(object);
	this->texture = static_cast<const TextureRenderObject*>(&object)->texture;
}

void TextureRenderObject::loadTexture(const std::string& filename)
{
	if (checkTextureFile(filename))
	{
		if (texture != 0)
		{
			glDeleteTextures(1, &texture);
		}

		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
		else {
			glDeleteTextures(1, &texture);
			errorCode.push(TEXTURE_FILE_ERROR);
		}
	}
	else errorCode.push(TEXTURE_FILE_ERROR);
}

bool TextureRenderObject::checkTextureFile(const std::string& filename)
{
	std::fstream file(filename);
	if (file.is_open())
	{
		file.close();
		return true;
	}
	else return false;
}

void TextureRenderObject::process()
{
	if (shader->getErrorCode() == Shader::NO_ERROR)
	{
		bindVertexArray();
		updateUniforms();
		bindTexture();
		drawArrays();
	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void TextureRenderObject::invalidate()
{
	RenderObject::invalidate();
	glDeleteTextures(1, &texture);
}

void TextureRenderObject::bindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}