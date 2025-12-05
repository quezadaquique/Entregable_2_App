/*
 * ENTREGABLE 2: Versión FINAL RESTAURADA (GLES 3.0 + Parche Memoria)
 * Autor: Enrique Quezada
 * * ESTA VERSIÓN FUNCIONA EN PC Y MAC.
 * * EL TRUCO PARA EL CELULAR ESTÁ EN EL COMANDO DE COMPILACIÓN.
 */

#include <stdio.h>
#include <vector>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// --- VOLVEMOS A GLES 3 (MODERNO Y ESTABLE EN PC) ---
#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// --- VARIABLES GLOBALES ---
GLFWwindow* g_window;
ImVec4 clear_color = ImVec4(0.15f, 0.16f, 0.21f, 1.00f);
float global_scale = 1.0f; 

// Variables Ejercicios
int lista_inicio = 10;
int lista_fin = 50;
int lista_salto = 5;
std::vector<int> lista_resultado;
bool lista_generada = false;
char mensaje_error_lista[256] = "";

float calc_v1 = 0.0f;
float calc_v2 = 0.0f;
int calc_opcion = 0; 
float calc_resultado = 0.0f;
bool calc_mostrar = false;
char mensaje_error_calc[256] = "";

// Funciones Auxiliares
void ControlTactilInt(const char* label, int* valor, int paso) {
    ImGui::PushID(label);
    if (ImGui::Button("-", ImVec2(40, 40))) *valor -= paso;
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(40, 40))) *valor += paso;
    ImGui::SameLine();
    ImGui::InputInt(label, valor);
    ImGui::PopID();
}

void ControlTactilFloat(const char* label, float* valor, float paso) {
    ImGui::PushID(label);
    if (ImGui::Button("-", ImVec2(40, 40))) *valor -= paso;
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(40, 40))) *valor += paso;
    ImGui::SameLine();
    ImGui::InputFloat(label, valor);
    ImGui::PopID();
}

void dibujar_interfaz() {
    ImGuiIO& io = ImGui::GetIO();
    
    // RESPONSIVE: Detectar si es celular (< 800px)
    bool es_movil = io.DisplaySize.x < 800.0f; 

    if (es_movil) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        if (global_scale == 1.0f) global_scale = 1.6f; // Zoom automático en celular
    } else {
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
        if (global_scale > 1.2f) global_scale = 1.0f;
    }

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    if (es_movil) window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    ImGui::Begin("App Entregable", NULL, window_flags);
    ImGui::SetWindowFontScale(global_scale);

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), es_movil ? "VISTA MOVIL" : "VISTA ESCRITORIO");
    ImGui::SameLine();
    ImGui::TextDisabled("| Alumno: Enrique Quezada");
    
    ImGui::Separator();
    
    if (ImGui::TreeNode("Configuración Visual")) {
        ImGui::SliderFloat("Zoom", &global_scale, 0.8f, 3.0f);
        ImGui::ColorEdit3("Fondo", (float*)&clear_color);
        ImGui::TreePop();
    }
    
    ImGui::Spacing();

    if (ImGui::BeginTabBar("MenuEjercicios")) {
        if (ImGui::BeginTabItem("Listas")) {
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Generador de Series");
            ImGui::Separator();
            ControlTactilInt("Inicio", &lista_inicio, 1);
            ControlTactilInt("Fin", &lista_fin, 5);
            ControlTactilInt("Salto", &lista_salto, 1);

            ImGui::Spacing();
            if (ImGui::Button("GENERAR LISTA", ImVec2(-1, 50))) { 
                lista_resultado.clear();
                mensaje_error_lista[0] = '\0';
                if (lista_salto <= 0) sprintf(mensaje_error_lista, "Error: Salto > 0");
                else if (lista_inicio > lista_fin) sprintf(mensaje_error_lista, "Error: Inicio <= Fin");
                else {
                    for (int i = lista_inicio; i <= lista_fin; i += lista_salto) lista_resultado.push_back(i);
                    lista_generada = true;
                }
            }
            if (mensaje_error_lista[0] != '\0') ImGui::TextColored(ImVec4(1,0,0,1), "%s", mensaje_error_lista);

            if (lista_generada) {
                ImGui::Separator();
                ImGui::BeginChild("ScrollRegion", ImVec2(0, -1), true);
                for (int num : lista_resultado) {
                    ImGui::Text("-> %d", num);
                    ImGui::Separator();
                }
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Calculadora")) {
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Operaciones");
            ImGui::Separator();
            ControlTactilFloat("Valor 1", &calc_v1, 1.0f);
            ControlTactilFloat("Valor 2", &calc_v2, 1.0f);

            ImGui::Spacing();
            ImGui::Text("Operación:");
            float btn_w = es_movil ? 80 : 100;
            if(ImGui::Button("SUM", ImVec2(btn_w, 40))) calc_opcion = 0; ImGui::SameLine();
            if(ImGui::Button("RES", ImVec2(btn_w, 40))) calc_opcion = 1; 
            if(ImGui::Button("MUL", ImVec2(btn_w, 40))) calc_opcion = 2; ImGui::SameLine();
            if(ImGui::Button("DIV", ImVec2(btn_w, 40))) calc_opcion = 3;
            
            ImGui::Spacing();
            if (ImGui::Button("CALCULAR", ImVec2(-1, 50))) {
                switch (calc_opcion) {
                    case 0: calc_resultado = calc_v1 + calc_v2; break;
                    case 1: calc_resultado = calc_v1 - calc_v2; break;
                    case 2: calc_resultado = calc_v1 * calc_v2; break;
                    case 3: if (calc_v2 != 0) calc_resultado = calc_v1 / calc_v2; else calc_resultado = 0; break;
                }
                calc_mostrar = true;
            }

            if (calc_mostrar) {
                ImGui::Separator();
                ImGui::SetWindowFontScale(global_scale * 1.5f);
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "= %.2f", calc_resultado);
                ImGui::SetWindowFontScale(global_scale);
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

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

int main(int, char**) {
    if (!glfwInit()) return 1;
    
    // --- RESTAURADO A GLES 3 (Esto funcionaba en PC) ---
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // Quitamos el CLIENT_API explicito para dejar que Emscripten decida lo mejor
    
    g_window = glfwCreateWindow(1280, 720, "Entregable 2", NULL, NULL);
    if (g_window == NULL) return 1;
    glfwMakeContextCurrent(g_window);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    
    // RESTAURADO A GLSL 300 ES
    ImGui_ImplOpenGL3_Init("#version 300 es");

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
    #else
    while (!glfwWindowShouldClose(g_window)) { main_loop(); }
    #endif
    return 0;
}