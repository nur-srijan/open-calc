// standard_calculator.hpp
// Standard precision calculator using double arithmetic
// Optimized with Accelerate framework for M-series

#ifndef STANDARD_CALCULATOR_HPP
#define STANDARD_CALCULATOR_HPP

#include <cmath>
#include <complex>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

class StandardCalculator {
public:
    StandardCalculator() = default;
    ~StandardCalculator() = default;
    
    // Basic arithmetic operations
    static constexpr double add(double a, double b) noexcept {
        return a + b;
    }
    
    static constexpr double subtract(double a, double b) noexcept {
        return a - b;
    }
    
    static constexpr double multiply(double a, double b) noexcept {
        return a * b;
    }
    
    static double divide(double a, double b) {
        if (b == 0.0) {
            throw std::domain_error("Division by zero");
        }
        return a / b;
    }
    
    static double modulo(double a, double b) {
        if (b == 0.0) {
            throw std::domain_error("Modulo by zero");
        }
        return std::fmod(a, b);
    }
    
    // Power and root operations
    static double power(double base, double exponent) {
        return std::pow(base, exponent);
    }
    
    static double sqrt(double x) {
        if (x < 0.0) {
            throw std::domain_error("Square root of negative number");
        }
        return std::sqrt(x);
    }
    
    static double cbrt(double x) {
        return std::cbrt(x);
    }
    
    // Exponential and logarithmic functions
    static double exp(double x) {
        return std::exp(x);
    }
    
    static double exp2(double x) {
        return std::exp2(x);
    }
    
    static double log(double x) {
        if (x <= 0.0) {
            throw std::domain_error("Logarithm of non-positive number");
        }
        return std::log(x);
    }
    
    static double log10(double x) {
        if (x <= 0.0) {
            throw std::domain_error("Logarithm of non-positive number");
        }
        return std::log10(x);
    }
    
    static double log2(double x) {
        if (x <= 0.0) {
            throw std::domain_error("Logarithm of non-positive number");
        }
        return std::log2(x);
    }
    
    // Trigonometric functions (angles in radians)
    static double sin(double x) {
        return std::sin(x);
    }
    
    static double cos(double x) {
        return std::cos(x);
    }
    
    static double tan(double x) {
        return std::tan(x);
    }
    
    static double asin(double x) {
        if (x < -1.0 || x > 1.0) {
            throw std::domain_error("asin domain error: x must be in [-1, 1]");
        }
        return std::asin(x);
    }
    
    static double acos(double x) {
        if (x < -1.0 || x > 1.0) {
            throw std::domain_error("acos domain error: x must be in [-1, 1]");
        }
        return std::acos(x);
    }
    
    static double atan(double x) {
        return std::atan(x);
    }
    
    static double atan2(double y, double x) {
        return std::atan2(y, x);
    }
    
    // Hyperbolic functions
    static double sinh(double x) {
        return std::sinh(x);
    }
    
    static double cosh(double x) {
        return std::cosh(x);
    }
    
    static double tanh(double x) {
        return std::tanh(x);
    }
    
    static double asinh(double x) {
        return std::asinh(x);
    }
    
    static double acosh(double x) {
        if (x < 1.0) {
            throw std::domain_error("acosh domain error: x must be >= 1");
        }
        return std::acosh(x);
    }
    
    static double atanh(double x) {
        if (x <= -1.0 || x >= 1.0) {
            throw std::domain_error("atanh domain error: x must be in (-1, 1)");
        }
        return std::atanh(x);
    }
    
    // Special functions
    static double factorial(int n) {
        if (n < 0) {
            throw std::domain_error("Factorial of negative number");
        }
        if (n > 170) {
            throw std::overflow_error("Factorial overflow (use arbitrary precision)");
        }
        return std::tgamma(n + 1.0);
    }
    
    static double abs(double x) {
        return std::fabs(x);
    }
    
    static double floor(double x) {
        return std::floor(x);
    }
    
    static double ceil(double x) {
        return std::ceil(x);
    }
    
    static double round(double x) {
        return std::round(x);
    }
    
    // Vectorized operations using Accelerate framework
#ifdef __APPLE__
    static void vector_add(const std::vector<double>& a, 
                          const std::vector<double>& b,
                          std::vector<double>& result) {
        if (a.size() != b.size()) {
            throw std::invalid_argument("Vector sizes must match");
        }
        result.resize(a.size());
        vDSP_vaddD(a.data(), 1, b.data(), 1, result.data(), 1, a.size());
    }
    
    static void vector_multiply(const std::vector<double>& a,
                               const std::vector<double>& b,
                               std::vector<double>& result) {
        if (a.size() != b.size()) {
            throw std::invalid_argument("Vector sizes must match");
        }
        result.resize(a.size());
        vDSP_vmulD(a.data(), 1, b.data(), 1, result.data(), 1, a.size());
    }
    
    static double vector_dot_product(const std::vector<double>& a,
                                    const std::vector<double>& b) {
        if (a.size() != b.size()) {
            throw std::invalid_argument("Vector sizes must match");
        }
        double result;
        vDSP_dotprD(a.data(), 1, b.data(), 1, &result, a.size());
        return result;
    }
    
    static double vector_sum(const std::vector<double>& v) {
        double result;
        vDSP_sveD(v.data(), 1, &result, v.size());
        return result;
    }
#endif
    
    // Constants
    static constexpr double PI = 3.14159265358979323846;
    static constexpr double E = 2.71828182845904523536;
    static constexpr double GOLDEN_RATIO = 1.61803398874989484820;
};

#endif // STANDARD_CALCULATOR_HPP
