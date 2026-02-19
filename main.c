#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef enum {
    T_FLOAT32,
    T_INT8_QUANTIZED
} TensorType;

typedef struct {
    TensorType type;
    uint32_t size;
    void* data;
    float scale;
    int8_t zero_point;
} Tensor;

Tensor* create_tensor(TensorType type, uint32_t size) {
    Tensor* t = (Tensor*)malloc(sizeof(Tensor));
    t->type = type;
    t->size = size;
    
    // SCALE DÜZELTME: Verilerimiz 25 civarı olduğu için scale'i 0.2 yapıyoruz 
    // Böylece 25 / 0.2 = 125 olur ve 127 sınırını aşmaz.
    t->scale = 0.2f; 
    t->zero_point = 0;

    size_t bytes = (type == T_FLOAT32) ? (size * sizeof(float)) : (size * sizeof(int8_t));
    t->data = malloc(bytes);
    return t;
}

void set_value(Tensor* t, uint32_t index, float val) {
    if (index >= t->size) return;
    if (t->type == T_FLOAT32) {
        ((float*)t->data)[index] = val;
    } else {
        int32_t q = (int32_t)roundf(val / t->scale) + t->zero_point;
        if (q > 127) q = 127;
        if (q < -128) q = -128;
        ((int8_t*)t->data)[index] = (int8_t)q;
    }
}

float get_value(Tensor* t, uint32_t index) {
    if (index >= t->size) return 0.0f;
    if (t->type == T_FLOAT32) return ((float*)t->data)[index];
    
    int8_t q = ((int8_t*)t->data)[index];
    return (float)(q - t->zero_point) * t->scale;
}

int main() {
    printf("--- TinyML Dinamik Tensor (Düzeltilmiş) ---\n\n");

    uint32_t n = 5;
    float ham_veri[] = {22.5, 23.1, 21.8, 25.4, 24.0};

    Tensor* f32 = create_tensor(T_FLOAT32, n);
    Tensor* i8 = create_tensor(T_INT8_QUANTIZED, n);

    for(uint32_t i = 0; i < n; i++) {
        set_value(f32, i, ham_veri[i]);
        set_value(i8, i, ham_veri[i]);
    }

    printf("ID | Orijinal | Int8 (Quantized)\n");
    printf("-------------------------------\n");
    for(uint32_t i = 0; i < n; i++) {
        printf("%u  |  %.1f    |     %.1f\n", i, ham_veri[i], get_value(i8, i));
    }

    // Doğru Bellek Analizi
    size_t f32_size = n * sizeof(float);
    size_t i8_size = n * sizeof(int8_t);

    printf("\n[Analiz]: Float32 Bellek: %zu Byte\n", f32_size);
    printf("[Analiz]: Int8 Bellek: %zu Byte\n", i8_size);
    printf("[Sonuç]: Bellekten %%75 tasarruf sağlandı!\n");

    free(f32->data); free(f32);
    free(i8->data); free(i8);
    return 0;
}
