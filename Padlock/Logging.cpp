#include <fstream>

#include "Logging.h"

// TODO: these functions can error out, so displaying an error messagebox is a good idea

void LogInit(std::filesystem::path path)
{
	// todo: errors
	if (!std::filesystem::exists(path)) {
		if (!std::filesystem::exists(path.parent_path()))
			std::filesystem::create_directories(path.parent_path());

		std::ofstream{ path };
	}
}

void LogAppend(std::string& message, std::filesystem::path path)
{
	std::ofstream log;
	log.open(path, std::ios::app);
	log << message;
}
