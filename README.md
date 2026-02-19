# C-dili-kodlama-# TinyML Dinamik Tensor Yapısı

Bu proje, kısıtlı kaynaklara sahip mikrodenetleyiciler (Arduino, ESP32 vb.) için optimize edilmiş, çalışma anında veri tipi değiştirebilen bir Tensor yapısı sunar.

## Özellikler
- **Dinamik Bellek Yönetimi:** C dilindeki `void*` ve `enum` yapıları ile 8-bit'ten 32-bit'e kadar veri desteği.
- **Quantization:** Float verileri INT8 formatına dönüştürerek %75 bellek tasarrufu sağlar.
- **Gömülü Sistem Dostu:** Düşük overhead ve yüksek performans.

## Çalıştırma
Derlemek için: `gcc main.c tensor.c -o demo`
Çalıştırmak için: `./demo`
