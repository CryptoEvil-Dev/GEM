<p align="center">
  <img width="360px" src="https://github.com/user-attachments/assets/cab4ee17-588e-45e8-8781-8cf07d08806f" />
</p>

# GEM
Graphic Environment Manager(**GEM**):<br />Менеджер графических окружений. Позволяет выбирать графическую оболочку в интерактивном графическом режиме.
### Библиотеки
- SDL2 (Отрисовка графики)
- yaml-cpp (Парсинг yaml файла конфигурации)
# GEM.yaml

### Конфигурация GEM

**mouse-src**: Путь до изображения мыши (По умолчанию белый кружок с чёрной обводкой)<br />
**background-color**: Цвет фона (По умолчанию 30 30 45 255)<br />
**background-image**: Фоновое изображение (По умолчанию нет)<br />

---

### Конфигурация оболочек
**name**: Название оболочки<br />
**sorce**: Путь до исполняемого файла оболочки<br />
**description**: Описание оболочки<br />
**image**: Путь до изображения оболочки<br />
**background-color**: Фоновый цвет плитки оболочки<br />
**background-image**: Фоновое изображение оболочки<br />

### Эффекты
**Наведение (hover)**<br />
Может изменяться любое значение конфигурации, есть дополнительные свойства:<br />
Для добавления эффекта необходимо добавить Map "hover": [...]<br />
**transition**: Временной промежуток изменения элемента в секундах<br />
**effect**: Эффект с которым преобразуется объект из одного состояния в другое (**ease**, **ease-in-out**)

---

# Сборка проекта
cmake --workflow --preset=build_all


# Зависимости

1. **Библиотеки SDL2**:
   - `libsdl2-dev` (основная библиотека SDL2)
   - `libsdl2-image-dev` (для работы с изображениями: PNG, JPEG и др.)
   - `libsdl2-ttf-dev` (для работы с векторными шрифтами)
   - `libsdl2-gfx-dev` (для расширенной графики: примитивы, антиалиасинг)
2. **Инструменты сборки**:
   - `build-essential` (компиляторы gcc/g++, make и другие базовые инструменты)
   - `cmake` (система сборки)
   - `ninja-build` (альтернативная система сборки, быстрее make)
3. **Дополнительные зависимости**:
   - `pkg-config` (для поиска библиотек через .pc файлы)
   - `libdrm-dev` и `libgbm-dev` (для работы с KMS/DRM, хотя обычно они уже установлены)
4. **Шрифты** (опционально, для примеров):
   - `fonts-roboto` (шрифт Roboto)
   - `fonts-dejavu` (шрифт DejaVu)

# Установка
``` bash
sudo sudo apt install -y \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-gfx-dev \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    fonts-roboto \
    fonts-dejavu \
    libsdl2-mixer-dev

```