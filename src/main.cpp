#include <ColonCompiler.h>

int main()
{
	auto const test_source = ":a(){:u=1963;}";

	ColonCompiler compiler;
	auto res = compiler.compile(test_source);

	return 0;
}