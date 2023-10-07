#include <ColonCompiler.h>
#include <iostream>
int main()
{

	ColonCompiler compiler;
	while (true)
	{
		std::cout << "to compile: \n";
		std::string src = "";
		std::cin >> src;
		try {
			std::cout << "\n";
			compiler.compile(src);
			std::cerr << "\nSuccess! \n";
		}
		catch (std::runtime_error& err)
		{
			std::cerr << "\n" << err.what() << "\n";
		}

	}

	return 0;
}