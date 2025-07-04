#include "stdio.h"
#include "../include/linalg.hpp"
#include "../include/pauli.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 700), "Bloch Sphere");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::StyleColorsDark(); // Or StyleColorsClassic() or StyleColorsLight()

// Optional: tweak the colors
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 5.0f;
        style.FrameRounding = 4.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 4.0f;

        // Customize colors (example)
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg]        = ImVec4(0.1f, 0.1f, 0.12f, 1.0f);
        colors[ImGuiCol_Header]          = ImVec4(0.2f, 0.2f, 0.3f, 1.0f);
        colors[ImGuiCol_Button]          = ImVec4(0.3f, 0.3f, 0.4f, 1.0f);
        colors[ImGuiCol_FrameBg]         = ImVec4(0.2f, 0.2f, 0.25f, 1.0f);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSize(ImVec2(400, 70), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);

        ImGui::Begin("Controls", nullptr, flags);
        float time;
        ImGui::SliderFloat("Time", &time, 0.0f, 10.0f);
        ImGui::End();



        window.clear();
        // Your drawing code here
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}