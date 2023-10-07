#include <ColonCompiler.h>
#include <iostream>

void printTree(const std::string& prefix, ASTNode& node, bool isLeft)
{
	if (&node == nullptr) {
		return;
	}

	std::cout << prefix;
	std::cout << "|--";
	std::cout << node.Identifier() << std::endl;

	for (size_t i = 0; i < node.Children().size(); ++i) {
		printTree(prefix + (isLeft ? "|   " : "    "), node.Children()[i], i < node.Children().size() - 1);
	}
}

void printAST(ColonAST& ast)
{
	printTree("", ast.Head(), false);
}

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
		if (res.has_value())
		{
			std::cerr << "\nSuccess!\n";
			auto ast = res.value().AST;
			printAST(ast);
		}

	}

	return 0;
}