//
// Created by Joseph Yesselman on 11/27/17.
//

#ifndef RNAMAKE_NEW_OPTION_H
#define RNAMAKE_NEW_OPTION_H

#include <base/types.h>

namespace base {

class OptionException : public std::runtime_error {
public:
    OptionException(
            String const & message):
            std::runtime_error(message)
    {}
};


class Option {
public:
    Option(
            String const & s_value):
            s_value_(s_value) {}

    virtual
    ~Option() {}

protected:
    String s_value_;
};

class StringOption : public Option {
public:
    StringOption(): Option("") {}

    StringOption(
            String const & s_value) : Option(s_value) {}

public:
    inline
    String const &
    get_value() { return this->s_value_; }

    inline
    void
    set_value(
            String value) { s_value_ = value; }
};

class IntOption : public Option {
public:
    IntOption(): IntOption("1") {}

    IntOption(
            String const & s_value) : Option(s_value) {
        try {
            value_ = std::stoi(this->s_value_);
        }
        catch(std::invalid_argument) {
            throw OptionException("cannot use " + s_value + " as int option value");
        }
    }

public:
    inline
    int
    get_value() { return value_; }

    inline
    void
    set_value(
            int value) { value_ = value; }

private:
    int value_;

};

class DoubleOption : public Option {
public:
    DoubleOption(): DoubleOption("1.0") {}

    DoubleOption(
            String const & s_value) : Option(s_value) {
        try {
            value_ = std::stod(this->s_value_);
        }
        catch(std::invalid_argument) {
            throw OptionException("cannot use " + s_value + " as double option value");
        }
    }

public:
    inline
    double
    get_value() { return value_; }

    inline
    void
    set_value(
            double value) { value_ = value; }

private:
    double value_;

};

class BoolOption : public Option {
public:
    BoolOption(): BoolOption("false") {}

    BoolOption(
            String const & s_value) : Option(s_value) {

        if     (this->s_value_ == "true")  { value_ = true; }
        else if(this->s_value_ == "false") { value_ = false; }
        else                               {
            throw OptionException("cannot use " + s_value + " as bool option value");
        }
    }

public:
    inline
    bool
    get_value() { return value_; }

    inline
    void
    set_value(
            bool value) { value_ = value; }

private:
    bool value_;

};

enum class OptionType {
    STRING,
    INT,
    DOUBLE,
    BOOL
};

struct Optinfo {
    String name, value;
    OptionType type;
};

#define OPT_INFO_STRING(name, value) Optinfo {name, value, base::OptionType::STRING}
#define OPT_INFO_INT(name, value)    Optinfo {name, value, base::OptionType::INT}
#define OPT_INFO_DOUBLE(name, value) Optinfo {name, value, base::OptionType::DOUBLE}
#define OPT_INFO_BOOL(name, value)   Optinfo {name, value, base::OptionType::BOOL}

typedef std::vector<Optinfo> OptInfos;

class Options {
public:
    Options() {}

    Options(OptInfos const & opts) {
        for(auto const & oi: opts) {
            if(_does_option_exist(oi.name)) {
                throw OptionException(oi.name + " already exists cannot add it again");
            }
            _add_option(oi.name, oi.value, oi.type);
        }
    }

    ~Options() {}

public:
    String const &
    get_string_value(
        String const & name) {
        if(string_options_.find(name) == string_options_.end()) {
            throw OptionException("no string option by name: " + name);
        }
        return string_options_[name].get_value();
    }

    int
    get_int_value(
            String const & name) {
        if(int_options_.find(name) == int_options_.end()) {
            throw OptionException("no int option by name: " + name);
        }
        return int_options_[name].get_value();
    }

    double
    get_double_value(
            String const & name) {
        if(double_options_.find(name) == double_options_.end()) {
            throw OptionException("no double option by name: " + name);
        }
        return double_options_[name].get_value();
    }

    bool
    get_bool_value(
            String const & name) {
        if(bool_options_.find(name) == bool_options_.end()) {
            throw OptionException("no int option by name: " + name);
        }
        return bool_options_[name].get_value();
    }

public:
    void
    set_values(
            StringStringMap const & keys_values) {
        for(auto const & kv : keys_values) {
            auto type = _option_type(kv.first);
            _add_option(kv.first, kv.second, type); // overwrite options with new values
        }
    }

    void
    set_string_value(
            String const & name,
            String const & value) {
        if(string_options_.find(name) == string_options_.end()) {
            throw OptionException("no string option by name: " + name);
        }
        string_options_[name].set_value(value);
    }

    void
    set_int_value(
            String const & name,
            int value) {
        if(int_options_.find(name) == int_options_.end()) {
            throw OptionException("no int option by name: " + name);
        }
        int_options_[name].set_value(value);
    }

    void
    set_double_value(
            String const & name,
            double value) {
        if(double_options_.find(name) == double_options_.end()) {
            throw OptionException("no double option by name: " + name);
        }
        double_options_[name].set_value(value);
    }

    void
    set_bool_value(
            String const & name,
            bool value) {
        if(bool_options_.find(name) == bool_options_.end()) {
            throw OptionException("no bool option by name: " + name);
        }
        bool_options_[name].set_value(value);
    }

private:
    void
    _add_option(
            String const & name,
            String const & value,
            OptionType type) {

        if     (type == OptionType::STRING) { string_options_[name] = StringOption(value); }
        else if(type == OptionType::INT)    { int_options_[name] = IntOption(value); }
        else if(type == OptionType::DOUBLE) { double_options_[name] = DoubleOption(value); }
        else if(type == OptionType::BOOL)   { bool_options_[name] = BoolOption(value); }
        else {
            throw OptionException("not a valid option type");
        }
    }

    bool
    _does_option_exist(
            String const & key) {
        if(string_options_.find(key) != string_options_.end()) { return true; }
        if(int_options_.find(key)    != int_options_.end())    { return true; }
        if(double_options_.find(key) != double_options_.end()) { return true; }
        if(bool_options_.find(key)   != bool_options_.end())   { return true; }
        return false;
    }

    OptionType
    _option_type(
            String const & key) {
        if(string_options_.find(key) != string_options_.end()) { return OptionType::STRING; }
        if(int_options_.find(key)    != int_options_.end())    { return OptionType::INT; }
        if(double_options_.find(key) != double_options_.end()) { return OptionType::DOUBLE; }
        if(bool_options_.find(key)   != bool_options_.end())   { return OptionType::BOOL;}
        throw OptionException(key + " is not a valid option");
    }

private:
    std::map<String, StringOption> string_options_;
    std::map<String, IntOption>    int_options_;
    std::map<String, DoubleOption> double_options_;
    std::map<String, BoolOption>   bool_options_;

};



}


#endif //RNAMAKE_NEW_OPTION_H
