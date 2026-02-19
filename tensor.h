#ifndef TENSOR_H
#define TENSOR_H

#include <stdint.h>
#include <stdlib.h>

// Desteklenen veri tipleri
typedef enum {
    T_FLOAT32,
    T_FLOAT16,
    T_INT8_QUANTIZED
} TensorType;

// Tensör yapısı
typedef struct {
    TensorType type;
    uint32_t size;          // Toplam eleman sayısı
    void* data;             // Dinamik veri alanı
    
    // Quantization parametreleri (Sadece INT8 için kullanılır)
    float scale;
    int8_t zero_point;
} Tensor;

// Fonksiyon prototipleri
Tensor* create_tensor(TensorType type, uint32_t size);
void free_tensor(Tensor* t);
void set_float32(Tensor* t, uint32_t index, float value);
float get_as_float32(Tensor* t, uint32_t index);

#endif
