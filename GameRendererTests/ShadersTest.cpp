#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameRendererTests
{		
	TEST_CLASS(ShadersTest)
	{
		void createOpenGLContext()
		{
			GLFWwindow* window;

			assert(glfwInit() != 0);

			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
			window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);
			if (!window)
			{
				glfwTerminate();
			}

			glfwHideWindow(window);

			glfwMakeContextCurrent(window);

			assert(glewInit() == 0);
		}

	public:
		
		TEST_METHOD(CreatingShaders)
		{
			createOpenGLContext();

			std::string vertexShader("../../GameRendererTests/shaders/testShader.vert");
			std::string fragmentShader("../../GameRendererTests/shaders/testShader.frag");
			
			Shader shader("testShader", vertexShader, fragmentShader);

			glfwTerminate();

			Assert::AreEqual(static_cast<int>(shader.getErrorCode()), static_cast<int>(Shader::NO_ERROR));
			Assert::AreNotEqual(shader.getProgram(), -1);
		}

	};
}