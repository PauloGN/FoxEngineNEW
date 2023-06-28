#include "pch.h"
#include "CppUnitTest.h"
//Week 03 Peters class
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FoxEngine::FoxMath;

namespace MathTest
{
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(TestConstructors)
		{
			const Vector3 v0;
			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 0.0f);

			const Vector3 v1(2.0f);
			Assert::AreEqual(v1.x, 2.0f);
			Assert::AreEqual(v1.y, 2.0f);
			Assert::AreEqual(v1.z, 2.0f);

			const Vector3 v2(-2.0f, 3.0f, 4.0f);
			Assert::AreEqual(v2.x, -2.0f);
			Assert::AreEqual(v2.y, 3.0f);
			Assert::AreEqual(v2.z, 4.0f);
		}

		TEST_METHOD(TestDotProduct)
		{
			const Vector3 v1(1.0f, 2.0f, 3.0f);
			const Vector3 v2(4.0f, 5.0f, 6.0f);

			const float dotProduct = Dot(v1, v2);

			Assert::AreEqual(dotProduct, 32.0f);
		}
	};
}
