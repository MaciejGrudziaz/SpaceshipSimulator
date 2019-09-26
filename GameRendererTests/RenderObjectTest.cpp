#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameRendererTests
{
	TEST_CLASS(RenderObjectTest)
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

		bool areEqualMat4(const glm::mat4& mat1, const glm::mat4& mat2, float eps = 0.00001)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (std::abs(mat1[i][j] - mat2[i][j]) > eps)
						return false;
				}
			}

			return true;
		}

	public:

		TEST_METHOD(CreatingRenderObject)
		{
			createOpenGLContext();

			RenderObject renderObj;
			renderObj.loadShader("../../GameRendererTests/shaders/testShader.vert", "../../GameRendererTests/shaders/testShader.frag");

			Assert::AreEqual(static_cast<int>(renderObj.getErrorCode()), static_cast<int>(RenderObject::NO_ERROR));

			std::shared_ptr<UniformDataMat4> uniform = std::make_shared<UniformDataMat4>("transform");
			std::shared_ptr<glm::mat4> testMat = std::make_shared<glm::mat4>(1.0f);
			uniform->mat = testMat;

			renderObj.addUniform(uniform);

			ShaderAttribute attrib;
			attrib.location = 0;
			attrib.size = 3;
			attrib.offset = (void*)0;

			renderObj.addShaderAttribute(attrib);

			attrib.location = 1;
			attrib.offset = (void*)(3 * sizeof(float));

			renderObj.addShaderAttribute(attrib);

			renderObj.init();

			Assert::AreEqual(static_cast<int>(renderObj.getErrorCode()), static_cast<int>(RenderObject::NO_ERROR));

			glfwTerminate();
		}

	};
}