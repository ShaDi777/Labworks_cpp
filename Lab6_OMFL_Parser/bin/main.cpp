#include "lib/parser.h"


int main(int, char**) {
    std::string data = R"(
        [level1.level2.level3]
        key1 = 1)";

    auto root = omfl::parse(data);
    std::cout << (root.valid() ? "VALID\n" : "invalid\n");

    std::cout << root.Get("level1").Get("level2").Get("level3").Get("key1").AsInt() << '\n'; // 1
    //std::cout << root.Get("level1.level2-1.key2").AsInt() << '\n'; // 2
    //std::cout << root.Get("level1.level2-2.key3").AsInt() << '\n'; // 3
    return 0;
}
