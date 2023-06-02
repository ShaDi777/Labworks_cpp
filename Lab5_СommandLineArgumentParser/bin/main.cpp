#include <functional>
#include <lib/ArgParser.h>

#include <iostream>
#include <numeric>

struct Options {
    bool sum = false;
    bool mult = false;
};

#include <chrono>
int main(int argc, char** argv) {
    ArgumentParser::ArgParser parser1("parser1");
    parser1.AddIntArgument('a');
    parser1.AddIntArgument('b');
    parser1.AddIntArgument('c');
    parser1.AddIntArgument('d');
    parser1.AddIntArgument('e');
    parser1.AddIntArgument('f');

    ArgumentParser::ArgParser parser2("parser2");
    parser2.AddIntArgument('a');
    parser2.AddIntArgument('b');
    parser2.AddIntArgument('c');
    parser2.AddIntArgument('d');
    parser2.AddIntArgument('e');
    parser2.AddIntArgument('f');

    auto start_time = std::chrono::high_resolution_clock::now();
    parser1.Parse(argc,argv);
    auto end_time_1 = std::chrono::high_resolution_clock::now();
    parser2.Parse2(argc,argv);
    auto end_time_2 = std::chrono::high_resolution_clock::now();

    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end_time_1 - start_time);
    std::cout << "STRING: " << duration1.count() << std::endl;
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end_time_2 - end_time_1);
    std::cout << "STRING_VIEW: " << duration2.count() << std::endl;
    return 0;

    /*
    Options opt;
    std::vector<int> values;

    ArgumentParser::ArgParser parser("Program");
    parser.AddIntArgument("N").MultiValue(1).Positional().StoreValues(values);
    parser.AddFlag("sum", "add args").StoreValue(opt.sum);
    parser.AddFlag("mult", "multiply args").StoreValue(opt.mult);
    parser.AddHelp('h', "help", "Program accumulate arguments");

    if(!parser.Parse(argc, argv)) {
        std::cout << "Wrong argument" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return 1;
    }

    if(parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
        return 0;
    }

    if(opt.sum) {
        std::cout << parser.GetIntValue("sum");
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
    } else if(opt.mult) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<int>()) << std::endl;
    } else {
        std::cout << "No one options had chosen" << std::endl;
        std::cout << parser.HelpDescription();
        return 1;
    }

    return 0;
     */
}
