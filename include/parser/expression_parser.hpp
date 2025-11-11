// expression_parser.hpp
// Simple expression parser with operator precedence
// Uses recursive descent parsing

#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP

#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <cctype>
#include "../computation/standard_calculator.hpp"

class ExpressionParser {
public:
    ExpressionParser() {
        // Register built-in functions
        register_function("sin", [](double x) { return StandardCalculator::sin(x); });
        register_function("cos", [](double x) { return StandardCalculator::cos(x); });
        register_function("tan", [](double x) { return StandardCalculator::tan(x); });
        register_function("asin", [](double x) { return StandardCalculator::asin(x); });
        register_function("acos", [](double x) { return StandardCalculator::acos(x); });
        register_function("atan", [](double x) { return StandardCalculator::atan(x); });
        
        register_function("sinh", [](double x) { return StandardCalculator::sinh(x); });
        register_function("cosh", [](double x) { return StandardCalculator::cosh(x); });
        register_function("tanh", [](double x) { return StandardCalculator::tanh(x); });
        
        register_function("sqrt", [](double x) { return StandardCalculator::sqrt(x); });
        register_function("cbrt", [](double x) { return StandardCalculator::cbrt(x); });
        register_function("abs", [](double x) { return StandardCalculator::abs(x); });
        
        register_function("exp", [](double x) { return StandardCalculator::exp(x); });
        register_function("ln", [](double x) { return StandardCalculator::log(x); });
        register_function("log", [](double x) { return StandardCalculator::log10(x); });
        register_function("log2", [](double x) { return StandardCalculator::log2(x); });
        
        register_function("floor", [](double x) { return StandardCalculator::floor(x); });
        register_function("ceil", [](double x) { return StandardCalculator::ceil(x); });
        register_function("round", [](double x) { return StandardCalculator::round(x); });
        
        // Register constants
        register_constant("pi", StandardCalculator::PI);
        register_constant("e", StandardCalculator::E);
        register_constant("phi", StandardCalculator::GOLDEN_RATIO);
    }
    
    double evaluate(const std::string& expression) {
        input_ = expression;
        pos_ = 0;
        return parse_expression();
    }
    
    void register_function(const std::string& name, std::function<double(double)> func) {
        functions_[name] = func;
    }
    
    void register_constant(const std::string& name, double value) {
        constants_[name] = value;
    }
    
private:
    std::string input_;
    size_t pos_;
    std::map<std::string, std::function<double(double)>> functions_;
    std::map<std::string, double> constants_;
    
    // Skip whitespace
    void skip_whitespace() {
        while (pos_ < input_.length() && std::isspace(input_[pos_])) {
            ++pos_;
        }
    }
    
    // Parse expression: handles addition and subtraction
    double parse_expression() {
        double result = parse_term();
        
        while (pos_ < input_.length()) {
            skip_whitespace();
            
            if (pos_ >= input_.length()) break;
            
            char op = input_[pos_];
            if (op == '+' || op == '-') {
                ++pos_;
                double right = parse_term();
                if (op == '+') {
                    result = StandardCalculator::add(result, right);
                } else {
                    result = StandardCalculator::subtract(result, right);
                }
            } else {
                break;
            }
        }
        
        return result;
    }
    
    // Parse term: handles multiplication and division
    double parse_term() {
        double result = parse_factor();
        
        while (pos_ < input_.length()) {
            skip_whitespace();
            
            if (pos_ >= input_.length()) break;
            
            char op = input_[pos_];
            if (op == '*' || op == '/' || op == '%') {
                ++pos_;
                double right = parse_factor();
                if (op == '*') {
                    result = StandardCalculator::multiply(result, right);
                } else if (op == '/') {
                    result = StandardCalculator::divide(result, right);
                } else {
                    result = StandardCalculator::modulo(result, right);
                }
            } else {
                break;
            }
        }
        
        return result;
    }
    
    // Parse factor: handles power, unary operators, parentheses, functions, numbers
    double parse_factor() {
        skip_whitespace();
        
        if (pos_ >= input_.length()) {
            throw std::runtime_error("Unexpected end of expression");
        }
        
        // Handle unary minus or plus
        if (input_[pos_] == '-' || input_[pos_] == '+') {
            char op = input_[pos_];
            ++pos_;
            double value = parse_factor();
            return (op == '-') ? -value : value;
        }
        
        // Handle parentheses
        if (input_[pos_] == '(') {
            ++pos_;
            double result = parse_expression();
            skip_whitespace();
            if (pos_ >= input_.length() || input_[pos_] != ')') {
                throw std::runtime_error("Mismatched parentheses");
            }
            ++pos_;
            
            // Check for power operator
            skip_whitespace();
            if (pos_ < input_.length() && input_[pos_] == '^') {
                ++pos_;
                double exponent = parse_factor();
                result = StandardCalculator::power(result, exponent);
            }
            
            return result;
        }
        
        // Handle functions and constants
        if (std::isalpha(input_[pos_])) {
            std::string name;
            while (pos_ < input_.length() && (std::isalnum(input_[pos_]) || input_[pos_] == '_')) {
                name += input_[pos_];
                ++pos_;
            }
            
            skip_whitespace();
            
            // Check if it's a function
            if (pos_ < input_.length() && input_[pos_] == '(') {
                ++pos_;
                double arg = parse_expression();
                skip_whitespace();
                if (pos_ >= input_.length() || input_[pos_] != ')') {
                    throw std::runtime_error("Mismatched parentheses in function call");
                }
                ++pos_;
                
                auto it = functions_.find(name);
                if (it == functions_.end()) {
                    throw std::runtime_error("Unknown function: " + name);
                }
                
                return it->second(arg);
            }
            
            // Check if it's a constant
            auto const_it = constants_.find(name);
            if (const_it != constants_.end()) {
                return const_it->second;
            }
            
            throw std::runtime_error("Unknown identifier: " + name);
        }
        
        // Parse number
        double result = parse_number();
        
        // Check for power operator
        skip_whitespace();
        if (pos_ < input_.length() && input_[pos_] == '^') {
            ++pos_;
            double exponent = parse_factor();
            result = StandardCalculator::power(result, exponent);
        }
        
        return result;
    }
    
    // Parse number
    double parse_number() {
        skip_whitespace();
        
        if (pos_ >= input_.length()) {
            throw std::runtime_error("Expected number");
        }
        
        size_t start = pos_;
        bool has_dot = false;
        
        while (pos_ < input_.length() && 
               (std::isdigit(input_[pos_]) || input_[pos_] == '.')) {
            if (input_[pos_] == '.') {
                if (has_dot) {
                    throw std::runtime_error("Invalid number format");
                }
                has_dot = true;
            }
            ++pos_;
        }
        
        // Handle scientific notation
        if (pos_ < input_.length() && (input_[pos_] == 'e' || input_[pos_] == 'E')) {
            ++pos_;
            if (pos_ < input_.length() && (input_[pos_] == '+' || input_[pos_] == '-')) {
                ++pos_;
            }
            while (pos_ < input_.length() && std::isdigit(input_[pos_])) {
                ++pos_;
            }
        }
        
        if (start == pos_) {
            throw std::runtime_error("Expected number");
        }
        
        std::string num_str = input_.substr(start, pos_ - start);
        try {
            return std::stod(num_str);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid number: " + num_str);
        }
    }
};

#endif // EXPRESSION_PARSER_HPP