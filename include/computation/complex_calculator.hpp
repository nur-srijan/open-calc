// complex_calculator.hpp
// Complex number calculator with template support

#ifndef COMPLEX_CALCULATOR_HPP
#define COMPLEX_CALCULATOR_HPP

#include <complex>
#include <cmath>
#include <stdexcept>

template<typename T = double>
class ComplexCalculator {
public:
    using Complex = std::complex<T>;
    
    // Basic arithmetic
    static Complex add(const Complex& a, const Complex& b) {
        return a + b;
    }
    
    static Complex subtract(const Complex& a, const Complex& b) {
        return a - b;
    }
    
    static Complex multiply(const Complex& a, const Complex& b) {
        return a * b;
    }
    
    static Complex divide(const Complex& a, const Complex& b) {
        if (std::abs(b) == T(0)) {
            throw std::domain_error("Division by zero");
        }
        return a / b;
    }
    
    // Conversion functions
    static std::pair<T, T> to_polar(const Complex& c) {
        return {std::abs(c), std::arg(c)};
    }
    
    static Complex from_polar(T magnitude, T phase) {
        return std::polar(magnitude, phase);
    }
    
    // Complex functions
    static Complex power(const Complex& base, const Complex& exponent) {
        return std::pow(base, exponent);
    }
    
    static Complex sqrt(const Complex& c) {
        return std::sqrt(c);
    }
    
    static Complex exp(const Complex& c) {
        return std::exp(c);
    }
    
    static Complex log(const Complex& c) {
        return std::log(c);
    }
    
    static Complex sin(const Complex& c) {
        return std::sin(c);
    }
    
    static Complex cos(const Complex& c) {
        return std::cos(c);
    }
    
    static Complex tan(const Complex& c) {
        return std::tan(c);
    }
    
    static Complex sinh(const Complex& c) {
        return std::sinh(c);
    }
    
    static Complex cosh(const Complex& c) {
        return std::cosh(c);
    }
    
    static Complex tanh(const Complex& c) {
        return std::tanh(c);
    }
    
    // Properties
    static T magnitude(const Complex& c) {
        return std::abs(c);
    }
    
    static T phase(const Complex& c) {
        return std::arg(c);
    }
    
    static Complex conjugate(const Complex& c) {
        return std::conj(c);
    }
    
    static T real(const Complex& c) {
        return c.real();
    }
    
    static T imag(const Complex& c) {
        return c.imag();
    }
};

#endif // COMPLEX_CALCULATOR_HPP