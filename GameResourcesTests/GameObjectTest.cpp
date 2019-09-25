#include "stdafx.h"
#include "CppUnitTest.h"

#include <stdlib.h>
#include <files/mgrimportfile.h>
#include <gameobjects/gameobject.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameResourcesTests
{
	TEST_CLASS(GameObjectTest)
	{
		void testVec3(const glm::vec3& vec1, const glm::vec3& vec2)
		{
			Assert::AreEqual(vec1.x, vec2.x);
			Assert::AreEqual(vec1.y, vec2.y);
			Assert::AreEqual(vec1.z, vec2.z);
		}

	public:

		TEST_METHOD(ImportAndAssign)
		{
			GameObjectPtr gameObject;
			MgrImportFile importFile;

			importFile.import("../../GameResourcesTests/models/spaceship_tex.mgr");

			Assert::IsNotNull(importFile.getModel().get());

			BasicModelPtr model = importFile.getModel();
			Assert::AreEqual(model->getObjectsCount(), 1);

			BasicObjectPtr obj = model->getObject(0);
			
			std::string name = "ObjectName";
			gameObject = std::make_shared<GameObject>(name);

			Assert::IsNotNull(gameObject.get());
			Assert::AreEqual(gameObject->getName(), name);

			gameObject->addModel(obj);
			Assert::AreEqual(static_cast<int>(gameObject->getModelsNames().size()), 1);
			Assert::AreEqual(gameObject->getModelsNames().front(), obj->name);
		}

		TEST_METHOD(PropertiesTest)
		{
			struct TestProperty_1 : public Property<GameObject>
			{
				TestProperty_1(const std::string& name, GameObject& obj)
					: Property<GameObject>(name, obj)
				{}

				void init()override { object.getTransform().setPosition(glm::vec3(1.0f, 0.0f, 0.0f)); }
				void process()override { object.getTransform().setPosition(glm::vec3(0.0f, 1.0f, 0.0f)); }
				void invalidate()override { object.getTransform().setPosition(glm::vec3(0.0f, 0.0f, 1.0f)); }
			};

			struct TestProperty_2 : public Property<GameObject>
			{
				TestProperty_2(const std::string& name, GameObject& obj)
					: Property<GameObject>(name, obj)
				{}

				void init()override { object.getTransform().setRotation(glm::vec3(45.0f, 0.0f, 0.0f)); }
				void process()override { object.getTransform().setRotation(glm::vec3(0.0f, 45.0f, 0.0f)); }
				void invalidate()override { object.getTransform().setRotation(glm::vec3(0.0f, 0.0f, 45.0f)); }
			};

			GameObjectPtr gameObject;
			gameObject = std::make_shared<GameObject>();
			Assert::IsNotNull(gameObject.get());

			std::string propName1("property1"), propName2("property2");
			gameObject->addProperty<TestProperty_1>(propName1);
			gameObject->addProperty<TestProperty_2>(propName2);

			Assert::AreEqual(static_cast<int>(gameObject->getPropertiesNames().size()), 2);
			Assert::AreEqual(gameObject->getPropertiesNames().front(), propName1);
			Assert::AreEqual(*(++gameObject->getPropertiesNames().begin()), propName2);

			gameObject->init();

			testVec3(gameObject->getTransform().getPosition(), glm::vec3(1.0f, 0.0f, 0.0f));
			testVec3(gameObject->getTransform().getRotation(), glm::vec3(45.0f, 0.0f, 0.0f));

			gameObject->process();

			testVec3(gameObject->getTransform().getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
			testVec3(gameObject->getTransform().getRotation(), glm::vec3(0.0f, 45.0f, 0.0f));

			gameObject->invalidate();

			testVec3(gameObject->getTransform().getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
			testVec3(gameObject->getTransform().getRotation(), glm::vec3(0.0f, 0.0f, 45.0f));
		}

	};
}