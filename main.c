#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// --- 1. BÖLÜM: TENSOR YAPI TASARIMI (Mühendislik Kısmı) ---

typedef enum {
    T_FLOAT32,        // Standart hassasiyet (4 Byte)
    T_FLOAT16,        // Yarı hassasiyet (2 Byte - TinyML'de yaygın)
    T_INT8_QUANTIZED  // Quantized (1 Byte - En yüksek tasarruf)
} TensorType;

typedef struct {
    TensorType type;    // Veri tipi
    uint32_t size;      // Eleman sayısı
    void* data;         // Dinamik bellek alanı
    
    // Quantization Parametreleri
    float scale;        // Ölçekleme katsayısı
    int8_t zero_point;  // Sıfır noktası kayması
} Tensor;

// Tensör Oluşturma Fonksiyonu
Tensor* create_tensor(TensorType type, uint32_t size) {
    Tensor* t = (Tensor*)malloc(sizeof(Tensor));
    t->type = type;
    t->size = size;
    t->scale = 0.1f;      // Örnek ölçekleme (Hassasiyet)
    t->zero_point = 0;

    size_t bytes = 0;
    if (type == T_FLOAT32) bytes = size * sizeof(float);
    else if (type == T_FLOAT16) bytes = size * sizeof(uint16_t);
    else if (type == T_INT8_QUANTIZED) bytes = size * sizeof(int8_t);

    t->data = malloc(bytes);
    return t;
}

// Veriyi Tensöre Yazma (Quantization Uygulayarak)
void set_value(Tensor* t, uint32_t index, float val) {
    if (index >= t->size) return;

    if (t->type == T_FLOAT32) {
        ((float*)t->data)[index] = val;
    } 
    else if (t->type == T_INT8_QUANTIZED) {
        // Formül: q = (float / scale) + zero_point
        int32_t q = (int32_t)roundf(val / t->scale) + t->zero_point;
        if (q > 127) q = 127;
        if (q < -128) q = -128;
        ((int8_t*)t->data)[index] = (int8_t)q;
    }
}

// Veriyi Tensörden Okuma (De-quantization Uygulayarak)
float get_value(Tensor* t, uint32_t index) {
    if (index >= t->size) return 0.0f;

    if (t->type == T_FLOAT32) {
        return ((float*)t->data)[index];
    } 
    else if (t->type == T_INT8_QUANTIZED) {
        // Formül: f = (q - zero_point) * scale
        int8_t q = ((int8_t*)t->data)[index];
        return (float)(q - t->zero_point) * t->scale;
    }
    return 0.0f;
}

void free_tensor(Tensor* t) {
    free(t->data);
    free(t);
}

// --- 2. BÖLÜM: ÖRNEK UYGULAMA (Demo Kısmı) ---

int main() {
    printf("--- TinyML Dinamik Tensor Demo Başladı ---\n\n");

    uint32_t eleman_sayisi = 5;
    float sensor_verileri[] = {22.5, 23.1, 21.8, 25.4, 24.0};

    // 1. Durum: Standart Float32 (Çok bellek harcar)
    Tensor* f32_t = create_tensor(T_FLOAT32, eleman_sayisi);
    
    // 2. Durum: Quantized INT8 (Az bellek harcar - ESP32/Arduino dostu)
    Tensor* int8_t = create_tensor(T_INT8_QUANTIZED, eleman_sayisi);

    // Verileri yükleyelim
    for(uint32_t i = 0; i < eleman_sayisi; i++) {
        set_value(f32_t, i, sensor_verileri[i]);
        set_value(int8_t, i, sensor_verileri[i]);
    }

    // Sonuçları karşılaştıralım
    printf("ID | Orijinal | Float32 Çıktısı | Int8 (Quantized) Çıktısı\n");
    printf("----------------------------------------------------------\n");
    for(uint32_t i = 0; i < eleman_sayisi; i++) {
        printf("%u  |  %.1f    |     %.1f       |       %.1f\n", 
               i, sensor_verileri[i], get_value(f32_t, i), get_value(int8_t, i));
    }

    // Bellek Analizi
    printf("\n[Analiz]: Float32 toplam %lu byte yer tuttu.\n", eleman_sayisi * sizeof(float));
    printf("[Analiz]: Int8 (Quantized) toplam %lu byte yer tuttu.\n", eleman_sayisi * sizeof(int8_t));
    printf("[Sonuç]: Bellekten %%75 tasarruf sağlandı!\n");

    free_tensor(f32_t);
    free_tensor(int8_t);

    return 0;
}
