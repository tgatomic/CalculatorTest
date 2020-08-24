#include <string>

#include "pch.h"
#include "CppUnitTest.h"

#include "../../CommandHandler.h"
#include "../../CommandHandler.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalculatorTest
{
	TEST_CLASS(CommandHandlerTest)
	{
	public:

		TEST_METHOD(CommandHandler_EmptyString_ShouldReturnErrorMessage)
		{
			Command newCommand = CommandHandler::GetCommand("");
			std::string expected = "error";

			Assert::AreEqual(newCommand.Operation, expected);		
		}

		TEST_METHOD(CommandHandler_Quit_ShouldReturnQuit)
		{
			Command newCommand = CommandHandler::GetCommand("quit");
			std::string expected = "quit";

			Assert::AreEqual(newCommand.Operation, expected);
		}

		TEST_METHOD(CommandHandler_Exit_ShouldReturnQuit)
		{
			Command newCommand = CommandHandler::GetCommand("exit");
			std::string expected = "quit";

			Assert::AreEqual(newCommand.Operation, expected);
		}

		TEST_METHOD(CommandHandler_RegisterAndIntValue_ShouldReturnSuccess)
		{
			Command newCommand = CommandHandler::GetCommand("salaries add 200");
			std::string expectedRegister = "salaries";
			std::string expectedOperation = "add";
			int expectedValue = 200;

			Assert::AreEqual(newCommand.Register, expectedRegister);
			Assert::AreEqual(newCommand.Operation, expectedOperation);
			Assert::AreEqual(newCommand.Value, expectedValue);
		}

		TEST_METHOD(CommandHandler_RegisterCapatilizedLetter_ShouldReturnSmallCase)
		{
			Command newCommand = CommandHandler::GetCommand("Salaries add 200");
			std::string expectedRegister = "salaries";
			std::string expectedOperation = "add";
			int expectedValue = 200;

			Assert::AreEqual(newCommand.Register, expectedRegister);
			Assert::AreEqual(newCommand.Operation, expectedOperation);
			Assert::AreEqual(newCommand.Value, expectedValue);
		}

		TEST_METHOD(CommandHandler_OperationCapatilizedLetter_ShouldReturnSmallCase)
		{
			Command newCommand = CommandHandler::GetCommand("Salaries Add 200");
			std::string expectedRegister = "salaries";
			std::string expectedOperation = "add";
			int expectedValue = 200;

			Assert::AreEqual(newCommand.Register, expectedRegister);
			Assert::AreEqual(newCommand.Operation, expectedOperation);
			Assert::AreEqual(newCommand.Value, expectedValue);
		}

		TEST_METHOD(CommandHandler_RegisterAsValue_ShouldReturnSmallCase)
		{
			Command newCommand = CommandHandler::GetCommand("Salaries Add b");
			std::string expectedRegister = "salaries";
			std::string expectedOperation = "add";
			std::string expectedSourceRegister = "b";
			int expectedValue = 0;

			Assert::AreEqual(newCommand.Register, expectedRegister);
			Assert::AreEqual(newCommand.Operation, expectedOperation);
			Assert::AreEqual(newCommand.SourceRegister, expectedSourceRegister);
			Assert::AreEqual(newCommand.Value, expectedValue);
		}
	};
}