#include "tensor.h"
#include <stdio.h>
#include <math.h>

// Tensör oluşturma: Tipine göre bellek ayırır
Tensor* create_tensor(TensorType type, uint32_t size) {
    Tensor* t = (Tensor*)malloc(sizeof(Tensor));
    t->type = type;
    t->size = size;
    t->scale = 1.0f;      
    t->zero_point = 0;

    size_t byte_size = 0;
    switch (type) {
        case T_FLOAT32: byte_size = size * sizeof(float); break;
        case T_FLOAT16: byte_size = size * sizeof(uint16_t); break; // 16-bit depolama
        case T_INT8_QUANTIZED: byte_size = size * sizeof(int8_t); break;
    }

    t->data = malloc(byte_size);
    return t;
}

// Veriyi tensöre yazma (Gerekirse quantize ederek)
void set_float32(Tensor* t, uint32_t index, float value) {
    if (index >= t->size) return;

    if (t->type == T_FLOAT32) {
        ((float*)t->data)[index] = value;
    } 
    else if (t->type == T_INT8_QUANTIZED) {
        // Basit Quantization: q = (f / scale) + zero_point
        int32_t q_val = (int32_t)roundf(value / t->scale) + t->zero_point;
        if (q_val > 127) q_val = 127;
        if (q_val < -128) q_val = -128;
        ((int8_t*)t->data)[index] = (int8_t)q_val;
    }
    // Not: Float16 gerçek dünyada özel donanım veya yazılım emülasyonu gerektirir.
}

// Veriyi okuma (Gerekirse de-quantize ederek)
float get_as_float32(Tensor* t, uint32_t index) {
    if (index >= t->size) return 0.0f;

    if (t->type == T_FLOAT32) {
        return ((float*)t->data)[index];
    } 
    else if (t->type == T_INT8_QUANTIZED) {
        // De-quantization: f = (q - zero_point) * scale
        int8_t q_val = ((int8_t*)t->data)[index];
        return (float)(q_val - t->zero_point) * t->scale;
    }
    return 0.0f;
}

void free_tensor(Tensor* t) {
    free(t->data);
    free(t);
}
