cmake_minimum_required(VERSION 3.10)

# Projenin adını ve dilini (C++) belirliyoruz
project(KostebekOyunu CXX)

# C++ standartını belirliyoruz
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# main.exe dosyasının "build" klasörüne değil doğrudan projenin ana dizinine çıkmasını sağlıyoruz.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Çalıştırılacak kaynak (.cpp) dosyalarımızı listeliyoruz
set(SOURCES 
    src/main.cpp 
    src/game.cpp
)

# Çalıştırılabilir dosyamızı oluşturuyoruz (Adı main.exe olacak)
add_executable(main ${SOURCES})

# Kendi yazdığımız header (.h) dosyalarının yerini gösteriyoruz
target_include_directories(main PRIVATE "include")

# Dışarıdan yol girilmezse, geliştiricinin (senin) varsayılan yolunu kullan:
set(SDL2_PATH "C:/dev/SDL2" CACHE PATH "Manuel SDL2 Kurulum Yolu")

if(EXISTS "${SDL2_PATH}")
    message(STATUS "==> Lokal SDL2 klasoru bulundu: ${SDL2_PATH}")
    
    target_include_directories(main PRIVATE "${SDL2_PATH}/include")
    target_link_directories(main PRIVATE "${SDL2_PATH}/lib")
    
    # Windows/MinGW için zorunlu derleme sırası
    if(WIN32)
        target_link_libraries(main PRIVATE mingw32 SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer)
    else()
        target_link_libraries(main PRIVATE SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer)
    endif()

else()
    message(STATUS "==> Lokal C:/dev/SDL2 bulunamadi. Sistem geneli (System-wide) SDL2 araniyor...")
    
    # Bu komutlar Linux, macOS veya vcpkg kullanan bilgisayarlarda otomatik bulur
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_PKGS REQUIRED sdl2 SDL2_image SDL2_ttf SDL2_mixer)
    
    target_include_directories(main PRIVATE ${SDL2_PKGS_INCLUDE_DIRS})
    target_link_directories(main PRIVATE ${SDL2_PKGS_LIBRARY_DIRS})
    target_link_libraries(main PRIVATE ${SDL2_PKGS_LIBRARIES})
    
endif()