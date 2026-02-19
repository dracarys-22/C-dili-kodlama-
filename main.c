#include <stdio.h>
#include "tensor.h"

int main() {
    uint32_t element_count = 10;

    // 1. Standart Float32 Tensör (40 Byte bellek kullanır)
    Tensor* f32_tensor = create_tensor(T_FLOAT32, element_count);

    // 2. Quantized INT8 Tensör (10 Byte bellek kullanır! %75 Tasarruf)
    Tensor* int8_tensor = create_tensor(T_INT8_QUANTIZED, element_count);
    int8_tensor->scale = 0.1f;    // Hassasiyet
    int8_tensor->zero_point = 0;

    // Örnek verileri yükleyelim
    float raw_data[] = {25.5, 26.2, 24.8, 25.0, 27.5, 22.1, 23.9, 24.5, 25.8, 26.0};

    printf("--- Bellek ve Veri Analizi ---\n");
    for (int i = 0; i < element_count; i++) {
        set_float32(f32_tensor, i, raw_data[i]);
        set_float32(int8_tensor, i, raw_data[i]);

        printf("Index[%d] -> Orijinal: %.1f | Quantized: %.1f\n", 
                i, get_as_float32(f32_tensor, i), get_as_float32(int8_tensor, i));
    }

    printf("\nSonuc: INT8 yapisi bellekte 4 kat daha az yer kapladi.\n");

    free_tensor(f32_tensor);
    free_tensor(int8_tensor);
    return 0;
}
