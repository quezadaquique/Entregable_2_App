/*
 * ENTREGABLE 2: Aplicación Web Interactiva C++. (VERSION ESTABLE EN PC)
 * Materia: Fundamentos de Programación
 * Autor: Enrique Quezada Fragoso
 * Descripción: Aplicación gráfica que demuestra el uso de estructuras de repetición (Ciclos)
 * y estructuras de decisión (Switch/If) mediante la librería Dear ImGui y WebAssembly.
 * * * INSTRUCCIONES DE COMPILACIÓN (Para PC/Web):
 * emcc main.cpp -o index.html -s USE_GLFW=3 -s USE_WEBGL2=1 -I./imgui/ -I./imgui/backends/ ./imgui/*.cpp ./imgui/backends/imgui_impl_glfw.cpp ./imgui/backends/imgui_impl_opengl3.cpp
 */

// --- BIBLIOTECAS ESTÁNDAR ---
#include <stdio.h>  // Para funciones de entrada/salida estándar (printf, etc.)
#include <vector>   // Para usar std::vector (arreglos dinámicos que cambian de tamaño)
#include <string>   // Para manejo de cadenas de texto

// --- BIBLIOTECAS DE EMSCRIPTEN (WEB) ---
// Este bloque solo se compila si estamos usando el compilador emcc para la web.
#ifdef __EMSCRIPTEN__
#include <emscripten.h> // Funciones para interactuar con el navegador y el bucle principal web
#endif

// --- BIBLIOTECAS GRÁFICAS (OPENGL/GLFW) ---
#define GLFW_INCLUDE_ES3 // Define que usaremos OpenGL ES 3.0 (Estándar moderno para web/móviles)
#include <GLES3/gl3.h>   // Cabecera de OpenGL ES 3
#include <GLFW/glfw3.h>  // GLFW: Librería para manejar ventanas, ratón y teclado de forma multiplataforma
#include "imgui.h"              // Núcleo de Dear ImGui
#include "imgui_impl_glfw.h"    // Adaptador de ImGui para GLFW
#include "imgui_impl_opengl3.h" // Adaptador de ImGui para OpenGL 3

// --- VARIABLES GLOBALES (ESTADO DE LA APP) ---
// Variables que deben persistir durante toda la vida del programa.
GLFWwindow* g_window; // Puntero a la ventana principal de la aplicación
ImVec4 clear_color = ImVec4(0.20f, 0.25f, 0.30f, 1.00f); // Color de fondo (Rojo, Verde, Azul, Alpha)

// -- Variables para el Ejercicio 2 (Listas/Ciclos) --
int lista_inicio = 10; // Valor donde empieza la serie
int lista_fin = 50;    // Valor donde termina la serie
int lista_salto = 5;   // Incremento en cada paso
std::vector<int> lista_resultado; // Vector para guardar la lista de números generada
bool lista_generada = false;      // Bandera para saber si ya hay datos para mostrar
char mensaje_error_lista[256] = ""; // Buffer de texto para mensajes de error

// -- Variables para el Ejercicio 3 (Calculadora/Switch) --
float calc_v1 = 0.0f; // Primer operando
float calc_v2 = 0.0f; // Segundo operando
int calc_opcion = 0;  // Opción seleccionada en el menú (0=Suma, 1=Resta, etc.)
float calc_resultado = 0.0f; // Resultado de la operación
bool calc_mostrar = false;   // Bandera para mostrar/ocultar el resultado
char mensaje_error_calc[256] = ""; // Buffer para errores matemáticos (ej. división por cero)

// --- LÓGICA DE LA INTERFAZ GRÁFICA (GUI) ---
// Esta función se llama en cada cuadro (frame) para dibujar la pantalla.
void dibujar_interfaz() {
    // Configuración inicial de la ventana (Posición y Tamaño)
    // ImGuiCond_FirstUseEver significa que solo se aplica la primera vez, luego el usuario puede moverla.
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);

    // Inicio de la ventana principal
    ImGui::Begin("Entregable 2 - Programación Estructurada", NULL, ImGuiWindowFlags_NoCollapse);

    // Encabezado con datos del alumno
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "ALUMNO: ENRIQUE QUEZADA");
    ImGui::Text("Esta aplicación interactiva resuelve los ejercicios solicitados en el Entregable 2.");
    ImGui::Separator(); // Línea divisoria
    ImGui::Spacing();   // Espacio vertical

    // SISTEMA DE PESTAÑAS (Tabs) para organizar los ejercicios
    if (ImGui::BeginTabBar("MenuEjercicios")) {
        
        // >>> PESTAÑA 1: EJERCICIO 2 (LISTAS) <<<
        if (ImGui::BeginTabItem("Ejercicio 2: Listas (Ciclos)")) {
            ImGui::Spacing();
            ImGui::TextWrapped("INSTRUCCIONES: Este módulo utiliza un ciclo 'for' para generar una secuencia numérica. Ingresa el inicio, el fin y el salto entre números.");
            ImGui::Separator();

            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Configuración de la Lista:");
            
            // Entradas de datos (Inputs) vinculadas a las variables globales
            ImGui::InputInt("Inicio de la lista", &lista_inicio);
            ImGui::InputInt("Fin de la lista", &lista_fin);
            ImGui::InputInt("Saltos", &lista_salto);

            ImGui::Spacing();

            // Botón de Acción: Al hacer clic se ejecuta el bloque if
            if (ImGui::Button("GENERAR LISTA", ImVec2(200, 40))) {
                // --- INICIO LÓGICA C++ EJERCICIO 2 ---
                lista_resultado.clear(); // Limpiamos resultados anteriores
                mensaje_error_lista[0] = '\0'; // Limpiamos errores previos

                // Validaciones de seguridad (Defensive Programming)
                if (lista_salto <= 0) {
                    sprintf(mensaje_error_lista, "Error: El salto debe ser mayor a 0 para evitar bucles infinitos.");
                    lista_generada = false;
                } else if (lista_inicio > lista_fin) {
                    sprintf(mensaje_error_lista, "Error: El inicio no puede ser mayor que el fin.");
                    lista_generada = false;
                } else {
                    // EL BUCLE (Ciclo FOR solicitado en la rúbrica)
                    // i inicia en 'lista_inicio'; continúa mientras i <= 'lista_fin'; incrementa 'lista_salto' en cada vuelta
                    for (int i = lista_inicio; i <= lista_fin; i += lista_salto) {
                        lista_resultado.push_back(i); // Guardamos el valor en el vector
                    }
                    lista_generada = true;
                }
                // --- FIN LÓGICA ---
            }

            // Visualización de Resultados
            ImGui::Spacing();
            ImGui::Separator();
            
            // Mostrar mensaje de error si existe
            if (mensaje_error_lista[0] != '\0') {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", mensaje_error_lista);
            }

            // Mostrar la lista generada
            if (lista_generada) {
                ImGui::Text("Resultado generado:");
                // Creamos una "caja hija" con scroll para que la lista no ocupe toda la pantalla
                ImGui::BeginChild("ScrollingRegion", ImVec2(0, 150), true, ImGuiWindowFlags_HorizontalScrollbar);
                // Iteramos sobre el vector de resultados para imprimirlos
                for (size_t i = 0; i < lista_resultado.size(); i++) {
                    ImGui::Text("Numero %d:  %d", (int)i + 1, lista_resultado[i]);
                }
                ImGui::EndChild();
            }

            ImGui::EndTabItem(); // Fin de la pestaña 1
        }

        // >>> PESTAÑA 2: EJERCICIO 3 (CALCULADORA) <<<
        if (ImGui::BeginTabItem("Ejercicio 3: Calculadora (Switch)")) {
            ImGui::Spacing();
            ImGui::TextWrapped("INSTRUCCIONES: Utiliza la estructura 'switch' para seleccionar una operación aritmética básica entre dos valores decimales.");
            ImGui::Separator();

            // Entradas numéricas con decimales
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Ingreso de Datos:");
            ImGui::InputFloat("Primer Valor", &calc_v1);
            ImGui::InputFloat("Segundo Valor", &calc_v2);

            ImGui::Spacing();
            ImGui::Text("Selecciona la operación deseada del menú:");
            
            // Radio Buttons para el menú de selección única
            // Modifican la variable 'calc_opcion' asignando 0, 1, 2 o 3 según lo seleccionado
            ImGui::RadioButton("1.- Suma", &calc_opcion, 0); ImGui::SameLine();
            ImGui::RadioButton("2.- Resta", &calc_opcion, 1);
            ImGui::RadioButton("3.- Multiplicación", &calc_opcion, 2); ImGui::SameLine();
            ImGui::RadioButton("4.- División", &calc_opcion, 3);

            ImGui::Spacing();

            // Botón Calcular
            if (ImGui::Button("REALIZAR OPERACIÓN", ImVec2(200, 40))) {
                mensaje_error_calc[0] = '\0';
                calc_mostrar = false;

                // --- INICIO LÓGICA C++ EJERCICIO 3 ---
                // Estructura SWITCH solicitada en la rúbrica
                switch (calc_opcion) {
                    case 0: // Suma
                        calc_resultado = calc_v1 + calc_v2;
                        calc_mostrar = true;
                        break;
                    case 1: // Resta
                        calc_resultado = calc_v1 - calc_v2;
                        calc_mostrar = true;
                        break;
                    case 2: // Multiplicación
                        calc_resultado = calc_v1 * calc_v2;
                        calc_mostrar = true;
                        break;
                    case 3: // División
                        if (calc_v2 != 0) { // Validación matemática importante
                            calc_resultado = calc_v1 / calc_v2;
                            calc_mostrar = true;
                        } else {
                            sprintf(mensaje_error_calc, "Error Matemático: No se puede dividir entre cero.");
                        }
                        break;
                }
                // --- FIN LÓGICA ---
            }

            // Resultado
            ImGui::Spacing();
            ImGui::Separator();

            if (mensaje_error_calc[0] != '\0') {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", mensaje_error_calc);
            }

            if (calc_mostrar) {
                // Hacemos el texto más grande para resaltar el resultado
                ImGui::SetWindowFontScale(1.5f); 
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "El resultado es: %.2f", calc_resultado);
                ImGui::SetWindowFontScale(1.0f); // Regresamos al tamaño normal
            }

            ImGui::EndTabItem(); // Fin de la pestaña 2
        }

        ImGui::EndTabBar(); // Fin del sistema de pestañas
    }

    ImGui::End(); // Fin de la ventana principal
}

// --- BUCLE PRINCIPAL (MAIN LOOP) ---
// Esta función gestiona el redibujado constante de la pantalla.
void main_loop() {
    glfwPollEvents(); // Procesa eventos de entrada (teclado/mouse)
    
    // Inicia un nuevo frame para ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // Dibuja nuestra interfaz personalizada
    dibujar_interfaz();
    
    // Renderizado (Pintado en pantalla)
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(g_window, &display_w, &display_h); // Obtiene tamaño actual
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w); // Color de fondo
    glClear(GL_COLOR_BUFFER_BIT); // Limpia la pantalla
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Dibuja los datos de ImGui
    glfwSwapBuffers(g_window); // Intercambia buffers (doble buffer para evitar parpadeo)
}

// --- PUNTO DE ENTRADA (MAIN) ---
int main(int, char**) {
    // Inicialización de GLFW (Ventanas)
    if (!glfwInit()) return 1;
    
    // Configuración de versión de OpenGL ES 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    // Crear la ventana gráfica
    g_window = glfwCreateWindow(1280, 720, "Entregable 2", NULL, NULL);
    if (g_window == NULL) return 1;
    
    // Hacer contexto actual
    glfwMakeContextCurrent(g_window);
    
    // Inicialización de Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark(); // Tema Oscuro
    
    // Conectar ImGui con GLFW y OpenGL
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    // Iniciar el bucle de la aplicación
    #ifdef __EMSCRIPTEN__
    // En Web, el navegador controla el bucle para no bloquear la pestaña
    emscripten_set_main_loop(main_loop, 0, 1);
    #else
    // En Escritorio nativo, usamos un bucle while estándar
    while (!glfwWindowShouldClose(g_window)) { main_loop(); }
    #endif

    return 0; // Fin exitoso
}