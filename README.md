# Whack-a-Mole – Zamana Karşı Refleks Oyunu

## 1. Proje Hakkında
**Whack-a-Mole**, C++ programlama dili ve SDL2 kütüphanesi kullanılarak geliştirilmiş, reflekslere ve hız dayalı tek oyunculu bir arcade oyunudur. Oyuncu, 3x3'lük bir ızgarada (toplam 9 delik) rastgele beliren köstebeklere 60 saniyelik kısıtlı süre içerisinde fare ile tıklayarak en yüksek skoru elde etmeye çalışır.

**Murat Uymaz**

**Öğrenci No:** 240229026

---

## 2. Özellikler

### Dinamik Oyun Akışı
* **Rastgele Üretim:** Köstebeklerin deliklerden çıkış aralıkları ve ekranda kalma süreleri her defasında dinamik olarak belirlenir.
* **Katmanlı Çizim (Z-Index):** Özelleştirilmiş bir `renderQueue` kuyruk mimarisi ile arkaplan, delikler ve köstebekler doğru derinlik sırasıyla ekrana çizilir.

### Kalıcı Skor Sistemi
* **Skor Takibi:** Her başarılı vuruşta anlık skor artışı ve kalan sürenin ekranda gösterimi.
* **Kayıt Sistemi:** Oyun bitiminde skorlar `assets/data/highscores.txt` dosyasına kalıcı olarak kaydedilir ve sıralanarak ilk 5 skor "High Scores" tablosunda listelenir.

### Arayüz ve Durum Yönetimi
* **Durum Makinesi (State Machine):** Ana Menü, Oyun İçi, Oyun Sonu ve Yüksek Skorlar ekranları arasında sorunsuz ve akıcı geçişler.
* **Görsel Geri Bildirim:** Vurulan köstebekler için anında farklı bir "vurulma" dokusunun (texture) devreye girmesi.
* **Ses ve Müzik:** Arkaplan müziği ve köstebek vurma ses efektleri eklenmiş, menülere ve oyun içine müziği açıp kapatmayı sağlayan etkileşimli bir buton entegre edilmiştir.

---

## 3. Teknoloji Yığını

| Kategori | Teknoloji | Açıklama |
|----------|-----------|----------|
| **Programlama Dili** | C++ (Standart C++17) | Ana oyun mantığı, bellek ve durum yönetimi |
| **Grafik Kütüphanesi** | SDL2 | Pencere, görüntü çizimi (SDL2_image),fontlar (SDL2_ttf) ve ses yönetimi (SDL2_mixer) |
| **Derleme Sistemi** | CMake | Proje yapılandırması, derleme ve linkleme işlemleri |
| **Geliştirme Ortamı** | VS Code | Tercih edilen IDE/Editör |

---

## 4. Kontroller

Oyun tamamen fare reflekslerine dayalı olarak tasarlanmıştır:

| Eylem | Tuş / Kontrol |
|-------|---------------|
| **Etkileşim / Vurma** | **Farenin Sol Tuşu** |
| **Müzik Aç/Kapat** | **Arayüzdeki Müzik Butonu (Fare Sol Tık)** |
| **Tam Ekran Geçişi** | **F11** Tuşu |
| **Oyundan Çıkış** | **Backspace** veya Pencereyi Kapatma İkonu |

---

## 5. Proje Nasıl Çalıştırılır?

### Ön Gereksinimler
* C++ Derleyicisi (MinGW / GCC)
* CMake (3.10 veya üzeri)
* SDL2, SDL2_image, SDL2_ttf ve SDL2_mixer kütüphanelerinin sisteme tanımlı olması

#### Windows üzerinde MinGW, CMake ve gerekli SDL2 kütüphanelerini kurmak için MSYS2'yi bilgisayarınıza kurun. Ardından MSYS2 terminalini (UCRT64 veya MINGW64) açarak aşağıdaki komutu çalıştırın:
```bash
# Derleyici (GCC), CMake ve tüm SDL2 kütüphanelerinin kurulumu
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_image mingw-w64-ucrt-x86_64-SDL2_ttf mingw-w64-ucrt-x86_64-SDL2_mixer
```
#### Linux dağıtımlarında gerekli paketleri resmi depolardan tek satırda kurabilirsiniz. Terminali açın ve aşağıdaki komutu çalıştırın:
```bash
# Paket listesini güncelleme ve gerekli derleme araçları ile kütüphanelerin kurulumu
sudo apt update
sudo apt install -y build-essential cmake libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```
#### Eğer macOS kullanıyorsanız ve sisteminizde Homebrew yüklüyse, terminal üzerinden şu komutla ön gereksinimleri kurabilirsiniz:
```bash
# CMake ve SDL2 kütüphanelerinin kurulumu
brew install cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

### Kurulum Adımları

**1. Depoyu İndirin**

Proje dosyalarını bilgisayarınıza indirin veya git ile klonlayın.
* **Not:** Oyunun düzgün çalışabilmesi için proje dosyalarını bilgisayarınızda özel harf içermeyen bir disk veya klasör içerisine kurun.

**2. Derleme (Terminal üzerinden)**

Proje dizinine gidin ve aşağıdaki komutları çalıştırın:
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

**3. Çalıştırma**

Oluşturulan çalıştırılabilir dosyayı başlatın:
```bash
cd ..
./main
```

* **Not:** Oyunun düzgün çalışabilmesi için assets/ klasörünün ve gerekli tüm DLL dosyalarının (dlls/ içerisindekiler) çalıştırılabilir .exe dosyası ile aynı dizinde bulunduğundan emin olun.

---

## 6. Proje Yapısı

```text
Whack-a-Mole/
├── assets/
├── screenshots/
├── include/
│   ├── game.hpp
│   ├── button.h
│   └── renderQueue.h
├── src/
│   ├── game.cpp
│   └── main.cpp
├── CMakeLists.txt
└── README.md
```
## 7. Oyun Görselleri

### Ana Menü
---
![Ana Menü](screenshots/main-menu.png)

### Oyun İçi
---
![Oyun Ekranı](screenshots/playing-menu.png)

### Oyun Sonu
---
![Oyun Sonu Ekranı](screenshots/game-over-menu.png)

### Yüksek Skorlar
---
![Yüksek Skorlar Ekranı](screenshots/high-scores.png)

## 8.Oynanış Videosu

https://github.com/user-attachments/assets/fc9f7583-e1f4-4d83-83c8-b899bfad4e3a