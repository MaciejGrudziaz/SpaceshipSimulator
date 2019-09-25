#include "stdafx.h"
#include "CppUnitTest.h"

#include <stdlib.h>
#include <files/mgrimportfile.h>
#include <gameobjects/gameobject.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameResourcesTests
{		
	TEST_CLASS(ImportFileTest)
	{
	public:
		
		TEST_METHOD(Import)
		{
			MgrImportFile importFile;

			importFile.import("../../GameResourcesTests/models/spaceship_tex.mgr");

			Assert::IsNotNull(importFile.getModel().get());

			BasicModelPtr model = importFile.getModel();
			Assert::AreEqual(model->getObjectsCount(), 1);

			BasicObjectPtr obj = model->getObject(0);
			Assert::AreEqual(static_cast<int>(obj->controlPoints.size()), 1041);
			Assert::AreEqual(static_cast<int>(obj->faces.size()), 2020);
			Assert::AreEqual(static_cast<int>(obj->skeleton->joints.size()), 5);
			Assert::IsNotNull(model->getMainHitbox(0).get());
			Assert::IsNotNull(model->getHitboxes(0).get());
			Assert::AreEqual(static_cast<int>(model->getHitboxes(0)->size()), 3);
			Assert::AreEqual(obj->textureName, std::string("texture.png"));
		}

	};
}