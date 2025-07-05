#include "stdio.h"
#include "../include/linalg.hpp"
#include "../include/sphere.hpp"
#include "../include/pauli.hpp"
#include "../include/evolve.hpp"
#include "hamiltonian.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>


int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 900), "Bloch Sphere Viewer", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);


    float rotationX = 20.0f, rotationY = -30.0f;
    float zoom = 3.0f;
    double time;
    sf::Clock deltaClock;
    bool dragging = false;
    bool playing = false;
    sf::Vector2i lastMousePos;

    std::vector<sf::Vector3f> blochTrail;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                dragging = true;
                lastMousePos = sf::Mouse::getPosition(window);
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                dragging = false;
            }
            if (event.type == sf::Event::MouseMoved && dragging) {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                sf::Vector2i delta = pos - lastMousePos;
                lastMousePos = pos;
                rotationX += delta.y * 0.5f;
                rotationY += delta.x * 0.5f;
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                zoom -= event.mouseWheelScroll.delta * 0.2f;
                if (zoom < 1.0f) zoom = 1.0f;
                if (zoom > 10.0f) zoom = 10.0f;
            }
        }
        if (playing)
            time += ImGui::GetIO().DeltaTime;


        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::StyleColorsLight();
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 5.0f;
        style.FrameRounding = 4.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 4.0f;
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);

        ImGui::Begin("Controls", nullptr, flags);
        double minTime = 0.0;
        double maxTime = 20.0;
        ImGui::SliderScalar("Time", ImGuiDataType_Double, &time, &minTime, &maxTime);
        ImGui::SliderFloat("Zoom", &zoom, 1.0f, 10.0f);
        if (ImGui::Button(playing ? "Pause" : "Play") )
            playing = !playing;
        
        if (playing and time > maxTime){
            time = 0;
            blochTrail.clear();
        }
        ImGui::Text("Rotation X: %.1f", rotationX);
        ImGui::Text("Rotation Y: %.1f", rotationY);
        ImGui::End();

        // Clear and setup OpenGL
        window.clear(sf::Color::Black);

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspect = float(window.getSize().x) / window.getSize().y;
        setPerspective(45.0f, aspect, 0.1f, 100.f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Camera transform: move back and rotate
        glTranslatef(0, 0, -zoom);
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 1, 0);

        // Draw sphere grid
        drawSphere();
        // Draw Bloch vector
        // Initial qubit state (|0>)

        auto psi = ComplexVector2({1, 0});
        psi = evolve(LandauZener_hamiltonian, psi, &time, 0.0001);
        sf::Vector3f bVec = blochVector(psi);
        blochTrail.push_back(bVec);
        if (blochTrail.size() > 1000) // limit trail length
            blochTrail.erase(blochTrail.begin());

        drawVector(bVec);
        drawTrail(blochTrail);

        // Draw ImGui on top
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}