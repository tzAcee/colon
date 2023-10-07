#include <ColonCompiler.h>
#include <iostream>
int main()
{

	ColonCompiler compiler;
	while (true)
	{
		std::cout << "\nto compile: \n";
		std::string src = "";
		std::cin >> src;
		std::cout << "\n";
		auto res = compiler.compile(src);
		if(res.has_value())
			std::cerr << "\nSuccess! \n";
	}

	return 0;
}