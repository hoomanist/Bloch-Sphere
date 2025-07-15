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



void setuo() {}

void loop() {}


int main() {

    // window initialize
    sf::RenderWindow window(sf::VideoMode(1400, 900), "Bloch Sphere Viewer", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
    double time;
    sf::Clock deltaClock;

    // constants and settings
    float rotationX = 20.0f, rotationY = -30.0f;
    float zoom = 3.0f;
    bool dragging = false;
    bool playing = false;
    sf::Vector2i lastMousePos;

    // schrodinger dynamics
    auto init_psi = RotatingField_Hamiltonian(0).eigenvectors().second;
    auto evolution = Evolution(RotatingField_Hamiltonian, init_psi, 0.001);
    evolution.evolve(100000);
    
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

        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);

        ImGui::Begin("Controls", nullptr, flags);
        double minTime = 0.0;
        double maxTime = 50.0;
        ImGui::SliderScalar("Time", ImGuiDataType_Double, &time, &minTime, &maxTime);
        ImGui::SliderFloat("Zoom", &zoom, 1.0f, 10.0f);
        if (ImGui::Button(playing ? "Pause" : "Play") )
            playing = !playing;
        
        ImGui::Text("Rotation X: %.1f", rotationX);
        ImGui::Text("Rotation Y: %.1f", rotationY);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

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

        glTranslatef(0, 0, -zoom);
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 1, 0);


        drawSphere();
        drawAxes();

        //dynamics
        auto bloch_trail = evolution.get_trail(time);     
        auto psi = evolution.get_current(time);
        if (playing){
            time += ImGui::GetIO().DeltaTime*5;
            evolution.increment_processed();

        }
        
        // drawing the vectors
        sf::Vector3f bVec = blochVector(psi);
        drawVector(bVec);
        drawTrail(bloch_trail);

        // Draw ImGui on top
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}