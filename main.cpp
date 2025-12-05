/*
 * ENTREGABLE 2: Aplicación Web Interactiva C++
 * Materia: Programación Estructurada
 * Autor: Enrique Quezada
 * Descripción: Aplicación que demuestra el uso de estructuras de repetición (Ciclos)
 * y estructuras de decisión (Switch/If) mediante una interfaz gráfica WebAssembly.
 * * INSTRUCCIONES DE COMPILACIÓN:
 * emcc main.cpp -o index.html -s USE_GLFW=3 -s USE_WEBGL2=1 -I./imgui/ -I./imgui/backends/ ./imgui/*.cpp ./imgui/backends/imgui_impl_glfw.cpp ./imgui/backends/imgui_impl_opengl3.cpp
 */

#include <stdio.h>
#include <vector>
#include <string>

// Cabeceras de Emscripten
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Cabeceras Gráficas
#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// --- VARIABLES GLOBALES (ESTADO DE LA APP) ---
GLFWwindow* g_window;
ImVec4 clear_color = ImVec4(0.20f, 0.25f, 0.30f, 1.00f); // Fondo azul grisáceo elegante

// Variables Ejercicio 2 (Listas)
int lista_inicio = 10;
int lista_fin = 50;
int lista_salto = 5;
std::vector<int> lista_resultado; // Almacenamos la lista generada aquí
bool lista_generada = false;
char mensaje_error_lista[256] = "";

// Variables Ejercicio 3 (Calculadora)
float calc_v1 = 0.0f;
float calc_v2 = 0.0f;
int calc_opcion = 0; // 0:Suma, 1:Resta, 2:Mult, 3:Div
float calc_resultado = 0.0f;
bool calc_mostrar = false;
char mensaje_error_calc[256] = "";

// --- LÓGICA DE LA INTERFAZ ---
void dibujar_interfaz() {
    // Configuramos una ventana grande y centrada
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);

    ImGui::Begin("Entregable 2 - Programación Estructurada", NULL, ImGuiWindowFlags_NoCollapse);

    // Encabezado Institucional
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "ALUMNO: ENRIQUE QUEZADA");
    ImGui::Text("Esta aplicación interactiva resuelve los ejercicios solicitados en el Entregable 2.");
    ImGui::Separator();
    ImGui::Spacing();

    // SISTEMA DE PESTAÑAS
    if (ImGui::BeginTabBar("MenuEjercicios")) {
        
        // --- PESTAÑA 1: EJERCICIO 2 (LISTAS) ---
        if (ImGui::BeginTabItem("Ejercicio 2: Listas (Ciclos)")) {
            ImGui::Spacing();
            ImGui::TextWrapped("INSTRUCCIONES: Este módulo utiliza un ciclo 'for' para generar una secuencia numérica. Ingresa el inicio, el fin y el salto entre números.");
            ImGui::Separator();

            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Configuración de la Lista:");
            
            // Entradas de datos
            ImGui::InputInt("Inicio de la lista", &lista_inicio);
            ImGui::InputInt("Fin de la lista", &lista_fin);
            ImGui::InputInt("Saltos", &lista_salto);

            ImGui::Spacing();

            // Botón de Acción
            if (ImGui::Button("GENERAR LISTA", ImVec2(200, 40))) {
                // Lógica de C++ (Validación y Ciclo)
                lista_resultado.clear();
                mensaje_error_lista[0] = '\0'; // Limpiar error

                if (lista_salto <= 0) {
                    sprintf(mensaje_error_lista, "Error: El salto debe ser mayor a 0 para evitar bucles infinitos.");
                    lista_generada = false;
                } else if (lista_inicio > lista_fin) {
                    sprintf(mensaje_error_lista, "Error: El inicio no puede ser mayor que el fin.");
                    lista_generada = false;
                } else {
                    // EL BUCLE (Ciclo FOR solicitado)
                    for (int i = lista_inicio; i <= lista_fin; i += lista_salto) {
                        lista_resultado.push_back(i);
                    }
                    lista_generada = true;
                }
            }

            // Visualización de Resultados
            ImGui::Spacing();
            ImGui::Separator();
            
            if (mensaje_error_lista[0] != '\0') {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", mensaje_error_lista);
            }

            if (lista_generada) {
                ImGui::Text("Resultado generado:");
                // Creamos una caja con scroll para ver los números
                ImGui::BeginChild("ScrollingRegion", ImVec2(0, 150), true, ImGuiWindowFlags_HorizontalScrollbar);
                for (size_t i = 0; i < lista_resultado.size(); i++) {
                    ImGui::Text("Numero %d:  %d", (int)i + 1, lista_resultado[i]);
                }
                ImGui::EndChild();
            }

            ImGui::EndTabItem();
        }

        // --- PESTAÑA 2: EJERCICIO 3 (CALCULADORA) ---
        if (ImGui::BeginTabItem("Ejercicio 3: Calculadora (Switch)")) {
            ImGui::Spacing();
            ImGui::TextWrapped("INSTRUCCIONES: Utiliza la estructura 'switch' para seleccionar una operación aritmética básica entre dos valores decimales.");
            ImGui::Separator();

            // Entradas
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Ingreso de Datos:");
            ImGui::InputFloat("Primer Valor", &calc_v1);
            ImGui::InputFloat("Segundo Valor", &calc_v2);

            ImGui::Spacing();
            ImGui::Text("Selecciona la operación deseada del menú:");
            
            // Radio Buttons para el menú
            ImGui::RadioButton("1.- Suma", &calc_opcion, 0); ImGui::SameLine();
            ImGui::RadioButton("2.- Resta", &calc_opcion, 1);
            ImGui::RadioButton("3.- Multiplicación", &calc_opcion, 2); ImGui::SameLine();
            ImGui::RadioButton("4.- División", &calc_opcion, 3);

            ImGui::Spacing();

            // Botón Calcular
            if (ImGui::Button("REALIZAR OPERACIÓN", ImVec2(200, 40))) {
                mensaje_error_calc[0] = '\0';
                calc_mostrar = false;

                // Estructura SWITCH solicitada
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
                        if (calc_v2 != 0) {
                            calc_resultado = calc_v1 / calc_v2;
                            calc_mostrar = true;
                        } else {
                            sprintf(mensaje_error_calc, "Error Matemático: No se puede dividir entre cero.");
                        }
                        break;
                }
            }

            // Resultado
            ImGui::Spacing();
            ImGui::Separator();

            if (mensaje_error_calc[0] != '\0') {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", mensaje_error_calc);
            }

            if (calc_mostrar) {
                // Mostramos el resultado en grande
                ImGui::SetWindowFontScale(1.5f); // Aumentar letra temporalmente
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "El resultado es: %.2f", calc_resultado);
                ImGui::SetWindowFontScale(1.0f); // Regresar letra normal
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

// --- BUCLE PRINCIPAL (NO TOCAR) ---
void main_loop() {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    dibujar_interfaz();
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(g_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(g_window);
}

// --- MAIN ---
int main(int, char**) {
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    g_window = glfwCreateWindow(1280, 720, "Entregable 2", NULL, NULL);
    if (g_window == NULL) return 1;
    glfwMakeContextCurrent(g_window);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark(); // Tema Oscuro
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
    #else
    while (!glfwWindowShouldClose(g_window)) { main_loop(); }
    #endif

    return 0;
}