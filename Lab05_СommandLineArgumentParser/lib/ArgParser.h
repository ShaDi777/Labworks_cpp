#pragma once

#include <map>
#include <utility>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <variant>


namespace ArgumentParser {
    // Single Argument
    class Argument {
        friend class ArgParser;

    protected:
        char short_name;
        std::string long_name;
        std::string description;

        bool is_help = false;
        bool is_positional = false;
        bool is_defined = false;
        bool is_value_alloc = true;
        bool is_multi_value_alloc = true;

        virtual std::string PrintInfo() {
            std::string line;
            if (short_name != '\0') {
                line += '-';
                line += short_name;
                line + ", ";
            }
            if (!long_name.empty()) {
                line += "--";
                line += long_name;
                line += ", ";
            }
            if (!description.empty()) {
                line += (is_help ? "Display help and exit" : description);
            }
            line += '\n';
            return line;
        }

    public:
        //CONSTRUCTORS
        explicit Argument(char sh_name) :
                Argument(sh_name, "", "") {}

        explicit Argument(const std::string& l_name) :
                Argument(0, l_name, "") {}

        Argument(char sh_name, const std::string& l_name) :
                Argument(sh_name, l_name, "") {}

        Argument(const std::string& l_name, const std::string& desc) :
                Argument(0, l_name, desc) {}

        Argument(char sh_name, const std::string& l_name, const std::string& desc) :
                short_name(sh_name), long_name(l_name), description(desc) {}


        virtual Argument& Positional() {
            this->is_positional = true;
            return *this;
        }

        virtual ~Argument() = default;;
    };

//==================================================================================
    class IntArgument : public Argument {
        friend class ArgParser;

    private:
        int* value_ptr = new int();
        std::vector<int>* multi_value_ptr = new std::vector<int>;

        int default_value;
        bool has_default = false;

        size_t min_values_count = 0;


        int GetValue(size_t index) {
            if (this->min_values_count == 0) return *this->value_ptr;
            else return (*this->multi_value_ptr)[index];
        }

        IntArgument& SetValue(int val) {
            if (this->min_values_count == 0) {
                this->is_defined = true;
                *this->value_ptr = val;
            } else (*this->multi_value_ptr).emplace_back(val);

            return *this;
        }

        bool Defined() {
            return this->is_defined || ((*this->multi_value_ptr).size() >= this->min_values_count);
        }

    public:
        explicit IntArgument(const Argument& a) : Argument(a) {}

        IntArgument& Default(int val) {
            *this->value_ptr = val;
            this->default_value = val;
            this->has_default = true;
            return *this;
        }

        IntArgument& Positional() override {
            this->is_positional = true;
            return *this;
        }

        IntArgument& MultiValue(size_t min_count = 1) {
            this->min_values_count = min_count;
            return *this;
        }

        IntArgument& StoreValue(int& target) {
            delete value_ptr;
            this->is_value_alloc = false;
            this->value_ptr = &target;
            return *this;
        }

        IntArgument& StoreValues(std::vector<int>& target) {
            delete multi_value_ptr;
            this->is_multi_value_alloc = false;
            this->multi_value_ptr = &target;
            return *this;
        }

        ~IntArgument() override {
            if (is_value_alloc) delete value_ptr;
            if (is_multi_value_alloc) delete multi_value_ptr;
        }
    };

    class BoolArgument : public Argument {
        friend class ArgParser;

    private:
        bool* value_ptr = new bool();

        bool default_value;
        bool has_default = false;


        BoolArgument& SetValue(bool val) {
            *this->value_ptr = val;
            this->is_defined = true;
            return *this;
        }

        bool GetValue() {
            return *this->value_ptr;
        }

    public:
        explicit BoolArgument(const Argument& a) : Argument(a) {}

        BoolArgument& Default(bool val) {
            *this->value_ptr = val;
            this->default_value = val;
            this->has_default = true;
            this->is_defined = true;
            return *this;
        }

        BoolArgument& StoreValue(bool& target) {
            delete value_ptr;
            this->is_value_alloc = false;
            this->value_ptr = &target;
            return *this;
        }

        bool Defined() {
            return this->is_defined;
        }

        ~BoolArgument() override {
            if (is_value_alloc) delete value_ptr;
        }
    };

    class StringArgument : public Argument {
        friend class ArgParser;

    private:
        std::string* value_ptr = new std::string;
        std::vector<std::string>* multi_value_ptr = new std::vector<std::string>;

        std::string default_value;
        bool has_default = false;

        size_t min_values_count = 0;


        StringArgument& SetValue(const std::string& val) {
            if (this->min_values_count == 0) {
                *this->value_ptr = val;
                this->is_defined = true;
            } else (*this->multi_value_ptr).emplace_back(val);

            return *this;
        }

        std::string GetValue(size_t index) {
            if (this->min_values_count == 0) return *this->value_ptr;
            else return (*this->multi_value_ptr)[index];
        }

        bool Defined() {
            return this->is_defined ||
                   (this->min_values_count > 0 && ((*this->multi_value_ptr).size() >= this->min_values_count));
        }

    public:
        explicit StringArgument(const Argument& a) : Argument(a) {}

        StringArgument& Default(const std::string& val) {
            *this->value_ptr = val;
            this->default_value = val;
            this->has_default = true;
            this->is_defined = true;
            return *this;
        }

        StringArgument& Positional() override {
            this->is_positional = true;
            return *this;
        }

        StringArgument& MultiValue(size_t min_count = 1) {
            this->min_values_count = min_count;
            return *this;
        }

        StringArgument& StoreValue(std::string& target) {
            delete value_ptr;
            this->is_value_alloc = false;
            this->value_ptr = &target;
            return *this;
        }

        StringArgument& StoreValues(std::vector<std::string>& target) {
            delete multi_value_ptr;
            this->is_multi_value_alloc = false;
            this->multi_value_ptr = &target;
            return *this;
        }

        ~StringArgument() override {
            if (is_value_alloc) delete value_ptr;
            if (is_multi_value_alloc) delete multi_value_ptr;
        }
    };

//==================================================================================
//==================================================================================

    class ArgParser {
    private:
        std::string app_name;
        std::vector<Argument*> arguments;
        std::map<std::string, Argument*> argument_map;
        bool need_help = false;


        template<class TArg>
        void AddToMap(TArg& arg) {
            if (!arg->long_name.empty()) this->argument_map[arg->long_name] = arg;
            if (arg->short_name != '\0') this->argument_map[std::string(1, arg->short_name)] = arg;
        }

        // ADDING ARGS
        template<typename... Targs>
        Argument& AddArgument(Targs&&... t_args) {
            std::vector<std::variant<char, std::string_view>> args{t_args...};

            char short_name = '\0';
            std::string long_name;
            std::string desc;

            for (auto& name: args)
                std::visit(
                        [&short_name, &long_name, &desc](const auto& x) {
                            using T = std::decay_t<decltype(x)>;
                            if constexpr (std::is_same_v<T, char>) {
                                short_name = x;
                            } else if constexpr (std::is_same_v<T, std::string_view>) {
                                if (long_name.empty()) {
                                    long_name = x;
                                } else {
                                    desc = x;
                                }
                            }
                        }, name);

            auto* arg = new Argument(short_name, long_name, desc);
            this->arguments.emplace_back(arg);

            return *this->arguments.back();
        }
//==================================================================================
    public:
        explicit ArgParser(std::string_view name) : app_name(name) {}

        template<typename... Targs>
        IntArgument& AddIntArgument(Targs&&... t_args) {
            auto* int_argument = new IntArgument(AddArgument(t_args...));
            AddToMap(int_argument);
            return *int_argument;
        }

        template<typename... Targs>
        BoolArgument& AddFlag(Targs&&... t_args) {
            auto* bool_argument = new BoolArgument(AddArgument(t_args...));
            AddToMap(bool_argument);
            return *bool_argument;
        }

        template<typename... Targs>
        StringArgument& AddStringArgument(Targs&&... t_args) {
            auto* string_argument = new StringArgument(AddArgument(t_args...));
            AddToMap(string_argument);
            return *string_argument;
        }

        template<typename... Targs>
        Argument& AddHelp(Targs&&... t_args) {
            Argument* a = &AddArgument(t_args...);
            a->is_help = true;
            AddToMap(a);
            return *a;
        }

//==================================================================================
        // GETTING ARGS
        template<typename T>
        int GetIntValue(T arg_name, size_t index = 0) {
            std::string name;
            name += arg_name;
            auto arg = dynamic_cast<IntArgument&>(*this->argument_map[name]);
            return arg.GetValue(index);
        }

        template<typename T>
        bool GetFlag(T arg_name) {
            std::string name;
            name += arg_name;
            auto arg = dynamic_cast<BoolArgument&>(*this->argument_map[name]);
            return arg.GetValue();
        }

        template<typename T>
        std::string GetStringValue(T arg_name, size_t index = 0) {
            std::string name;
            name += arg_name;
            return dynamic_cast<StringArgument&>(*this->argument_map[name]).GetValue(index);
        }

//==================================================================================
        bool Parse(int argc, char** argv) {
            std::vector<std::string> args(argc);
            for (int i = 0; i < argc; i++) {
                args[i] = argv[i];
            }
            return Parse(std::move(args));
        }
        bool Parse(std::vector<std::string>&& argv) {
            Argument* positional_argument;
            for (const auto& name_arg: this->argument_map) {
                if (name_arg.second->is_positional) {
                    positional_argument = name_arg.second;
                    break;
                }
            }

            std::string_view param_name;
            std::string param_value;
            Argument* param = nullptr;

            for (int i = 1; i < argv.size(); i++) {
                std::string_view argv_ = argv[i];

                if (argv_.find('=') != std::string_view::npos) {
                    param_name = argv_.substr(0, argv_.find('='));
                    param_value = argv_.substr(argv_.find('=') + 1, argv_.length() - argv_.find('='));

                } else {
                    if (argv_[0] == '-') {
                        if (argv_[1] == '-') {
                            param_name = argv_;
                            param_name.remove_prefix(2);
                        } else if ('0' <= argv[i][1] && argv[i][1] <= '9') {
                            param_value = argv[i];
                        } else {
                            if (argv_.length() > 2) {
                                // Check multi-letters flags
                                // Activate flags
                                for (int j = 1; j < argv_.length(); j++) {
                                    param = this->argument_map[std::string(1, argv_[j])];
                                    auto* bool_arg = dynamic_cast<BoolArgument*>(param);
                                    bool_arg->SetValue(true);
                                }

                            } else {
                                param_name = argv_;
                                param_name.remove_prefix(1);
                            }
                        }
                    } else {
                        param_value = argv[i];
                    }
                }


                if (!param_name.empty() && param_name[0] == '-') param_name.remove_prefix(1);
                if (!param_name.empty() && param_name[0] == '-') param_name.remove_prefix(1);

                if (param == nullptr && !param_name.empty()) {
                    param = this->argument_map[std::string(param_name)];
                    if (param->is_help) {
                        this->need_help = true;
                    }
                    if (dynamic_cast<BoolArgument*>(param)) {
                        dynamic_cast<BoolArgument*>(param)->SetValue(true);
                        param_name = "";
                    }
                    param = nullptr;
                }

                if (!param_value.empty()) {
                    if (param_name.empty()) param = positional_argument;
                    else param = this->argument_map[std::string(param_name)];

                    if (dynamic_cast<IntArgument*>(param)) {
                        dynamic_cast<IntArgument*>(param)->SetValue(stoi(param_value));
                    }
                    if (dynamic_cast<StringArgument*>(param)) {
                        dynamic_cast<StringArgument*>(param)->SetValue(param_value);
                    }

                    param_value = "";
                    param = nullptr;
                }
            }

            if (this->need_help) return true;
            for (const auto& name_param: this->argument_map) {
                if (dynamic_cast<IntArgument*>(name_param.second) &&
                    !dynamic_cast<IntArgument*>(name_param.second)->Defined())
                    return false;
                if (dynamic_cast<StringArgument*>(name_param.second) &&
                    !dynamic_cast<StringArgument*>(name_param.second)->Defined())
                    return false;
            }
            return true;
        }

//==================================================================================
        bool Help() {
            return this->need_help;
        }

        std::string HelpDescription() {
            std::string help;
            help += app_name + '\n';

            for (auto argument: this->arguments) {
                if (argument->is_help) {
                    help += argument->description + "\n\n";
                }
            }

            for (auto& argument: this->arguments) {
                help += argument->PrintInfo();
            }
            return help;
        }

        ~ArgParser() {
            for (auto v: arguments) {
                delete v;
            }
        }
    };

} // namespace ArgumentParser