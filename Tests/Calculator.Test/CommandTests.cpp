#include <string>

#include "pch.h"
#include "CppUnitTest.h"

#include "../../Command.h"
#include "../../Command.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalculatorTest
{
	TEST_CLASS(CommandCalculatorTest)
	{
	public:

		TEST_METHOD(ConstructorTest_EmptyStrings_ShouldReturnSuccess)
		{
			std::string registerName = "";
			std::string command = "";
			std::string value = "";

			Command testCommand(registerName, command, value);
		}

		TEST_METHOD(ConstructorTest_RealData_ShouldReturnSuccess)
		{
			std::string registerName = "a";
			std::string command = "add";
			std::string value = "5";

			Command testCommand(registerName, command, value);
		}
	};
}