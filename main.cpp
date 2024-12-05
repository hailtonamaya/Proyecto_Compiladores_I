#include <iostream>
#include <string>
#include <fstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "ExprEvaluator.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filepath = argv[1];

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return 1;
    }

    Lexer lex(file);
    Parser parser(lex);

    try {
        AstNode *root = parser.parse();
        ExprEvaluator expr_eval;
        std::cout << root->toString() << "\n";
        std::cout << expr_eval.evaluate(root) << "\n";
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << '\n';
    }

    file.close();

    return 0;
}
