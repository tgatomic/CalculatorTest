#include <string>
#include <map>

#include "pch.h"
#include "CppUnitTest.h"

//#include "../../Command.h"
//#include "../../Command.cpp"

#include "../../Calculator.h"
//#include "../../Calculator.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalculatorTest
{
	int Add(int a, int b) {
		return a + b;
	}

	TEST_CLASS(CalculatorTest)
	{
	public:

		TEST_METHOD(CalculateResult_CorrectData_AddToMap)
		{
			/*
			Never got this working unfortunatelly. Would have been great to be able to test my register handeling and
			calculation but for one or other reason I always got some linker error.
			*/

			/*
			Calculator calcTest(NULL);

			std::string registerName = "a";
			std::string command = "add";
			std::string value = "5";

			std::string expectedRegisterName = "a";
			std::string expectedCommand = "add";
			int expectedValue = 5;
			

			Command testCommand(registerName, command, value);
			
			typedef int (*funptr)(int, int);
			funptr arethmicFunctions[] = { Add };
			
			
			calcTest.ProcessRegisterChange(testCommand, arethmicFunctions);
			
			bool registerFound = (calcTest.Registers.find("a") != calcTest.Registers.end())? true:false;

			Assert::AreEqual(registerFound, true);
			Assert::AreEqual(calcTest.Registers.find("a")->second.Operation, expectedCommand);
			Assert::AreEqual(calcTest.Registers.find("a")->second.Value, expectedValue);

			expectedRegisterName = "a";
			expectedCommand = "add";
			expectedValue = 10;

			Command commandTwo(registerName, command, value);
			calcTest.ProcessRegisterChange(commandTwo, NULL);

			registerFound = (calcTest.Registers.find("a") != calcTest.Registers.end()) ? true : false;

			Assert::AreEqual(registerFound, true);
			Assert::AreEqual(calcTest.Registers.find("a")->second.Operation, expectedCommand);
			Assert::AreEqual(calcTest.Registers.find("a")->second.Value, expectedValue);
			*/	
		}
	};
}