// main.cpp - Advanced Calculator Entry Point
// Optimized for Apple M-series processors

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>

// Include computation modules
#include "computation/standard_calculator.hpp"
#include "computation/complex_calculator.hpp"
#include "computation/matrix_calculator.hpp"

// Include parser
#include "parser/expression_parser.hpp"

// Include Metal acceleration
#include "acceleration/metal_interface.hpp"

// Include UI
#include "ui/calculator_ui.hpp"

void print_banner() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Advanced Calculator - M-Series Build" << std::endl;
    std::cout << "  Hardware Accelerated Computing" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
}

void print_help() {
    std::cout << "Available Commands:" << std::endl;
    std::cout << "  help       - Show this help message" << std::endl;
    std::cout << "  exit/quit  - Exit the calculator" << std::endl;
    std::cout << "  clear      - Clear the screen" << std::endl;
    std::cout << "  gpu        - Toggle GPU acceleration" << std::endl;
    std::cout << "  mode       - Switch calculation mode" << std::endl;
    std::cout << "\nModes:" << std::endl;
    std::cout << "  standard   - Standard precision (double)" << std::endl;
    std::cout << "  complex    - Complex number calculations" << std::endl;
    std::cout << "  matrix     - Matrix operations" << std::endl;
    std::cout << "  symbolic   - Symbolic mathematics" << std::endl;
    std::cout << "\nExamples:" << std::endl;
    std::cout << "  2 + 2 * 3" << std::endl;
    std::cout << "  sin(pi/2)" << std::endl;
    std::cout << "  sqrt(144)" << std::endl;
    std::cout << "  ln(e^2)" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        print_banner();
        
        // Initialize Metal acceleration
        std::cout << "Initializing Metal GPU acceleration..." << std::endl;
        auto metal = MetalInterface::create();
        if (metal && metal->is_available()) {
            std::cout << "✓ Metal GPU available: " << metal->get_device_name() << std::endl;
            std::cout << "✓ Compute units: " << metal->get_compute_units() << std::endl;
        } else {
            std::cout << "⚠ Metal not available, using CPU fallback" << std::endl;
        }
        std::cout << std::endl;
        
        // Initialize calculators
        StandardCalculator std_calc;
        ComplexCalculator<double> cmplx_calc;
        MatrixCalculator mtx_calc;
        
        // Initialize expression parser
        ExpressionParser parser;
        
        print_help();
        
        // Main REPL loop
        std::string input;
        std::string mode = "standard";
        bool gpu_enabled = metal && metal->is_available();
        
        while (true) {
            std::cout << "[" << mode << "]";
            if (gpu_enabled) std::cout << "[GPU]";
            std::cout << " > ";
            
            if (!std::getline(std::cin, input)) {
                break;  // EOF
            }
            
            // Trim whitespace
            input.erase(0, input.find_first_not_of(" \t"));
            input.erase(input.find_last_not_of(" \t") + 1);
            
            if (input.empty()) continue;
            
            // Handle commands
            if (input == "exit" || input == "quit") {
                std::cout << "Goodbye!" << std::endl;
                break;
            }
            else if (input == "help") {
                print_help();
                continue;
            }
            else if (input == "clear") {
                std::system("clear");
                print_banner();
                continue;
            }
            else if (input == "gpu") {
                gpu_enabled = !gpu_enabled;
                std::cout << "GPU acceleration: " 
                          << (gpu_enabled ? "ENABLED" : "DISABLED") 
                          << std::endl;
                continue;
            }
            else if (input.rfind("mode ", 0) == 0) {
                mode = input.substr(5);
                std::cout << "Switched to " << mode << " mode" << std::endl;
                continue;
            }
            
            // Parse and evaluate expression
            try {
                double result = parser.evaluate(input);
                std::cout << "Result: " << result << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
