#pragma once
#include <thread>
#include <string>
#include <queue>
#include <mutex>

#include "Command.h"

class Console
{
private:
	std::thread listenerThread;
	std::thread parseThread;
	std::thread writeThread;

	std::queue<std::string> rxMessageQueue;
	std::mutex rxMessageMutex;

	


	void GetUserInput();
	void ParseIncommingMessages();
	void Write(std::string);


public:
	std::atomic<bool> RunThread = true;
	std::queue<Command> CommandQueue;
	std::mutex CommandQueueMutex;

	std::queue<std::string> txMessageQueue;
	std::mutex txMessageMutex;

	void WriteOutgoingMessages();
	void Exit()
	{
		RunThread = false;
	}

	Console();
	~Console();
};

