#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameRendererTests
{		
	TEST_CLASS(ShadersTest)
	{
	public:
		
		TEST_METHOD(CreatingShaders)
		{
			GLFWwindow* window;

			assert(glfwInit() != 0);

			window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);
			if (!window)
			{
				glfwTerminate();
			}

			glfwHideWindow(window);

			glfwMakeContextCurrent(window);

			assert(glewInit() == 0);

			std::string vertexShader("../../GameRendererTests/shaders/shader1_Dynamic.vert");
			std::string fragmentShader("../../GameRendererTests/shaders/shader1_Dynamic.frag");
			
			Shader shader("testShader", vertexShader, fragmentShader);

			glfwTerminate();

			Assert::AreEqual(static_cast<int>(shader.getErrorCode()), static_cast<int>(Shader::NO_ERROR));
			Assert::AreNotEqual(shader.getProgram(), -1);
		}

	};
}