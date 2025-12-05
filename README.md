Entregable 2: Aplicación Web Interactiva en C++
Materia: Fundamentos de Programación

Autor: Enrique Quezada Fragoso

Tecnología: C++, WebAssembly (Emscripten), Dear ImGui

📄 Descripción del Proyecto
Esta es una aplicación gráfica interactiva desarrollada en C++ y compilada a WebAssembly para ejecutarse en navegadores web. El objetivo principal del proyecto es demostrar la implementación práctica de estructuras de control fundamentales en programación estructurada mediante una interfaz visual moderna.

La aplicación integra:

Estructuras de Repetición (Ciclos): Generación dinámica de series numéricas.

Estructuras de Decisión (Switch/If): Lógica para una calculadora aritmética básica.

⚠️ Nota Importante de Compatibilidad
SOLO PARA ESCRITORIO (PC/LAPTOP)

Esta aplicación ha sido diseñada y optimizada exclusivamente para su uso en entornos de escritorio con teclado y ratón.

No está optimizada para dispositivos móviles (celulares o tablets).

La interfaz gráfica puede no responder adecuadamente a eventos táctiles o no ajustarse correctamente a pantallas verticales pequeñas.

🚀 Funcionalidades
La aplicación cuenta con un sistema de pestañas (Tabs) que divide los ejercicios:

1. Generador de Listas (Ciclo for)
Permite al usuario configurar los parámetros de un bucle para generar una secuencia de números.

Entradas: Inicio, Fin y Salto (incremento).

Validaciones: Previene bucles infinitos (saltos <= 0) y rangos inválidos.

Salida: Muestra la lista resultante en un área con desplazamiento (scroll).

2. Calculadora Básica (Estructura switch)
Realiza operaciones aritméticas fundamentales seleccionando la opción desde un menú.

Operaciones: Suma, Resta, Multiplicación y División.

Validaciones: Incluye manejo de errores para evitar la división entre cero.

🛠️ Tecnologías Utilizadas
Lenguaje: C++ (Estándar C++11 o superior).

Compilador Web: Emscripten (emcc).

Interfaz Gráfica: Dear ImGui.

Backend Gráfico: OpenGL ES 3.0 / WebGL 2 via GLFW.

💻 Instrucciones de Compilación
Para compilar este proyecto desde el código fuente (main.cpp) a una aplicación web (index.html), se requiere tener instalado el SDK de Emscripten.

Ejecute el siguiente comando en la terminal, asegurándose de estar en el directorio raíz del proyecto y tener las carpetas de imgui correctamente ubicadas:

Bash

emcc main.cpp -o index.html \
    -s USE_GLFW=3 \
    -s USE_WEBGL2=1 \
    -I./imgui/ -I./imgui/backends/ \
    ./imgui/*.cpp \
    ./imgui/backends/imgui_impl_glfw.cpp \
    ./imgui/backends/imgui_impl_opengl3.cpp
    
🎮 Cómo Usar
Abra el archivo index.html generado en un navegador web compatible (Chrome, Firefox, Edge).

Utilice las pestañas superiores para navegar entre el "Ejercicio 2" y el "Ejercicio 3".

Introduzca los valores numéricos en las cajas de texto.

Presione los botones de acción ("GENERAR LISTA" o "REALIZAR OPERACIÓN") para ver los resultados en pantalla.