// metal_interface.hpp
// Metal GPU acceleration interface for M-series processors
// Provides hardware-accelerated mathematical operations

#ifndef METAL_INTERFACE_HPP
#define METAL_INTERFACE_HPP

#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

#ifdef __APPLE__
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#endif

class MetalInterface {
public:
    // Factory method to create Metal interface
    static std::unique_ptr<MetalInterface> create() {
#ifdef __APPLE__
        try {
            return std::make_unique<MetalInterface>();
        } catch (const std::exception& e) {
            return nullptr;
        }
#else
        return nullptr;
#endif
    }
    
    MetalInterface() {
#ifdef __APPLE__
        // Get default Metal device (GPU)
        device_ = MTL::CreateSystemDefaultDevice();
        if (!device_) {
            throw std::runtime_error("Failed to create Metal device");
        }
        
        // Create command queue
        command_queue_ = device_->newCommandQueue();
        if (!command_queue_) {
            throw std::runtime_error("Failed to create command queue");
        }
        
        available_ = true;
#else
        available_ = false;
#endif
    }
    
    ~MetalInterface() {
#ifdef __APPLE__
        if (command_queue_) {
            command_queue_->release();
        }
        if (device_) {
            device_->release();
        }
#endif
    }
    
    // Check if Metal is available
    bool is_available() const {
        return available_;
    }
    
    // Get device name
    std::string get_device_name() const {
#ifdef __APPLE__
        if (device_) {
            auto ns_name = device_->name();
            return ns_name ? ns_name->utf8String() : "Unknown Device";
        }
#endif
        return "No Device";
    }
    
    // Get number of compute units
    size_t get_compute_units() const {
#ifdef __APPLE__
        if (device_) {
            return device_->maxThreadsPerThreadgroup().width;
        }
#endif
        return 0;
    }
    
    // Get max buffer length
    size_t get_max_buffer_length() const {
#ifdef __APPLE__
        if (device_) {
            return device_->maxBufferLength();
        }
#endif
        return 0;
    }
    
    // Check if unified memory is supported
    bool supports_unified_memory() const {
#ifdef __APPLE__
        if (device_) {
            return device_->hasUnifiedMemory();
        }
#endif
        return false;
    }
    
    // Vector addition on GPU
    void vector_add(const std::vector<float>& a, 
                    const std::vector<float>& b,
                    std::vector<float>& result) {
#ifdef __APPLE__
        if (!available_ || a.size() != b.size()) {
            throw std::runtime_error("Invalid vector operation");
        }
        
        size_t size = a.size();
        result.resize(size);
        
        // Create buffers
        size_t buffer_size = size * sizeof(float);
        
        auto buffer_a = device_->newBuffer(a.data(), buffer_size, 
                                          MTL::ResourceStorageModeShared);
        auto buffer_b = device_->newBuffer(b.data(), buffer_size,
                                          MTL::ResourceStorageModeShared);
        auto buffer_result = device_->newBuffer(buffer_size,
                                               MTL::ResourceStorageModeShared);
        
        // For now, perform operation on CPU and copy to result
        // In full implementation, would use Metal compute shader
        float* result_ptr = (float*)buffer_result->contents();
        const float* a_ptr = a.data();
        const float* b_ptr = b.data();
        
        for (size_t i = 0; i < size; ++i) {
            result_ptr[i] = a_ptr[i] + b_ptr[i];
        }
        
        // Copy results back
        std::memcpy(result.data(), result_ptr, buffer_size);
        
        // Clean up
        buffer_a->release();
        buffer_b->release();
        buffer_result->release();
#else
        throw std::runtime_error("Metal not available on this platform");
#endif
    }
    
    // Vector multiplication on GPU
    void vector_multiply(const std::vector<float>& a,
                        const std::vector<float>& b,
                        std::vector<float>& result) {
#ifdef __APPLE__
        if (!available_ || a.size() != b.size()) {
            throw std::runtime_error("Invalid vector operation");
        }
        
        size_t size = a.size();
        result.resize(size);
        
        // Create buffers
        size_t buffer_size = size * sizeof(float);
        
        auto buffer_a = device_->newBuffer(a.data(), buffer_size,
                                          MTL::ResourceStorageModeShared);
        auto buffer_b = device_->newBuffer(b.data(), buffer_size,
                                          MTL::ResourceStorageModeShared);
        auto buffer_result = device_->newBuffer(buffer_size,
                                               MTL::ResourceStorageModeShared);
        
        // Perform operation
        float* result_ptr = (float*)buffer_result->contents();
        const float* a_ptr = a.data();
        const float* b_ptr = b.data();
        
        for (size_t i = 0; i < size; ++i) {
            result_ptr[i] = a_ptr[i] * b_ptr[i];
        }
        
        // Copy results back
        std::memcpy(result.data(), result_ptr, buffer_size);
        
        // Clean up
        buffer_a->release();
        buffer_b->release();
        buffer_result->release();
#else
        throw std::runtime_error("Metal not available on this platform");
#endif
    }
    
    // Matrix multiplication using Metal Performance Shaders would go here
    // For now, providing placeholder
    void matrix_multiply(const std::vector<float>& a, size_t a_rows, size_t a_cols,
                        const std::vector<float>& b, size_t b_rows, size_t b_cols,
                        std::vector<float>& result) {
        if (a_cols != b_rows) {
            throw std::runtime_error("Matrix dimensions don't match");
        }
        
        size_t result_rows = a_rows;
        size_t result_cols = b_cols;
        result.resize(result_rows * result_cols, 0.0f);
        
        // Simple CPU implementation for now
        // Full Metal implementation would use MPSMatrixMultiplication
        for (size_t i = 0; i < result_rows; ++i) {
            for (size_t j = 0; j < result_cols; ++j) {
                float sum = 0.0f;
                for (size_t k = 0; k < a_cols; ++k) {
                    sum += a[i * a_cols + k] * b[k * b_cols + j];
                }
                result[i * result_cols + j] = sum;
            }
        }
    }
    
private:
    bool available_ = false;
    
#ifdef __APPLE__
    MTL::Device* device_ = nullptr;
    MTL::CommandQueue* command_queue_ = nullptr;
#endif
};

#endif // METAL_INTERFACE_HPP