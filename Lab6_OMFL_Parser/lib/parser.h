#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <iterator>

enum Types {
    integer = 0, floating, boolean, string, array, null_obj
};

std::vector<std::string> SplitStringDots(const std::string& str);

namespace omfl {


// Help Part
    class Object {
    public:
        Types type;
        std::string value;
        std::vector<Object> arr;

        Object(): type(null_obj){}

        Object(Types t, std::string val) : type(t), value(std::move(val)) {}

        Object(Types t, std::vector<Object> a) : type(t), arr(std::move(a)) {}

        Object operator[](int index) {
            return arr[index];
        }

        Object operator[](int index) const {
            return arr[index];
        }

        Object& operator=(const Object& object) {
            this->type = object.type;
            this->value = object.value;
            //this->arr = object.arr;
            for (auto x : object.arr) {
                this->arr.emplace_back(x);
            }
            return *this;
        }

        ~Object() {
            for (auto o: arr) {
                //delete o;
            }
        };
    };


    class Section {
    public:
        std::map<std::string, Object> m_KeyValue;
        std::map<std::string, Section> m_Sections;

        Section() = default;

        ~Section() {
            /*
            for (const auto& s: m_Sections) {
                delete s.second;
            }
            for (const auto& o: m_KeyValue) {
                delete o.second;
            }
            */
        }

        //Section(const Section& section) {
        //}
    };

    class Union {
    public:
        Object obj;
        Section sect;
        bool is_valid = true;

        Union(Object obj_, Section sect_) : obj(obj_), sect(sect_) {}

        Union operator[](int index) {

            if (index >= obj.arr.size()) {
                //Section* null_section = new Section();
                //Object* null_object = new Object(null_obj, "");
                //Section* null_section = new Section();
                Union u(Object(null_obj, ""), Section());
                return u;
            }
            //Section* null_section = new Section();
            //Union u(&(*obj)[index], null_section);
            Union u(obj[index], Section());
            return u;
        }

        Union Get(const std::string& str) const{
            std::vector path = SplitStringDots(str);
            Section last_ptr = this->sect;
            for (size_t i = 0; i < path.size(); i++) {
                try {
                    last_ptr = last_ptr.m_Sections.at(path[i]);
                } catch (std::out_of_range& exception) {
                    //Section* null_section = new Section();
                    Union u(last_ptr.m_KeyValue.at(path[i]), Section());
                    return u;
                }
            }
            Union u(Object(null_obj, ""), last_ptr);
            return u;
        }

        bool valid() const {
            return is_valid;
        }

        // ARRAY
        bool IsArray() const {
            return obj.type == array;
        }

        // INT
        bool IsInt() const {
            return obj.type == integer;
        }

        int AsInt() const {
            return std::stoi(obj.value);
        }

        int AsIntOrDefault(int default_value) const {
            return (obj.type == integer ? std::stoi(obj.value) : default_value);
        }

        // FLOAT
        bool IsFloat() const {
            return obj.type == floating;
        }

        float AsFloat() const {
            return std::stof(obj.value);
        }

        float AsFloatOrDefault(float default_value) const {
            return (obj.type == floating ? std::stof(obj.value) : default_value);
        }

        // STRING
        bool IsString() const {
            return obj.type == string;
        }

        std::string AsString() const {
            return (obj.value);
        }

        std::string AsStringOrDefault(const std::string& default_value) const {
            return (obj.type == string ? obj.value : default_value);
        }

        // BOOl
        bool IsBool() const {
            return obj.type == boolean;
        }

        bool AsBool() const {
            return (obj.value == "true");
        }

        bool AsBoolOrDefault(bool default_value) const {
            return (obj.type == boolean ? (obj.value == "true") : default_value);
        }
    };

// Main Part
    class Parser {
        friend Union parse(const std::filesystem::path& path);

        friend Union parse(const std::string& str);

        Section* root = new Section();
        std::map<std::string, Section> m_Sections;

    public:
        bool is_valid = true;

        Parser() = default;

        ~Parser() {
            //for (const auto& kv: m_Sections) {
            //    delete kv.second;
            //}
            //delete root;
        }

        /*Union Get(const std::string& str) const {

            std::vector path = SplitStringDots(str);
            Section* last_ptr = root;
            for (size_t i = 0; i < path.size(); i++) {
                try {
                    last_ptr = last_ptr->m_Sections.at(path[i]);
                } catch (std::out_of_range& exception) {
                    Union u(last_ptr->m_KeyValue[path[i]], nullptr);
                    return u;
                }
            }
            Union u(nullptr, last_ptr);
            return u;
        }

        bool valid() const {
            return is_valid;
        }
        */

    };

    Union parse(const std::filesystem::path& path);

    Union parse(const std::string& str);

}// namespace
