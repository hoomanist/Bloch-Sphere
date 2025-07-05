#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <complex>
#include <cmath>
#include "expectation.hpp"
#include "pauli.hpp"
sf::Vector3f blochVector(ComplexVector2 psi) {
    psi.normalize(); 

    double x = expectation(Pauli::X, psi); 
    double y = expectation(Pauli::Y, psi); 
    double z = expectation(Pauli::Z, psi); 

    return {float(x), float(y), float(z)};

}

void drawSphere(int latSegments = 20, int longSegments = 20) {
    glColor3f(0.7f, 0.7f, 0.7f);
    for (int i = 0; i <= latSegments; ++i) {
        float lat = M_PI * (-0.5f + float(i) / latSegments);
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j <= longSegments; ++j) {
            float lon = 2.0f * M_PI * float(j) / longSegments;
            float x = cos(lat) * cos(lon);
            float y = cos(lat) * sin(lon);
            float z = sin(lat);
            glVertex3f(x, y, z);
        }
        glEnd();
    }
    for (int j = 0; j <= longSegments; ++j) {
        float lon = 2.0f * M_PI * float(j) / longSegments;
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= latSegments; ++i) {
            float lat = M_PI * (-0.5f + float(i) / latSegments);
            float x = cos(lat) * cos(lon);
            float y = cos(lat) * sin(lon);
            float z = sin(lat);
            glVertex3f(x, y, z);
        }
        glEnd();
    }
}

void drawAxes(float length = 1.2f) {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0); glVertex3f(length, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0); glVertex3f(0, length, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0); glVertex3f(0, 0, length);
    glEnd();
    glLineWidth(1.0f);
}


void drawTrail(const std::vector<sf::Vector3f>& bt) {
    glBegin(GL_LINE_STRIP);
    glColor3f(0.5f, 1.0f, 0.5f);

    for (const auto& p : bt)
        glVertex3f(p.x, p.y, p.z);

    glEnd();

}

void drawVector(const sf::Vector3f& v) {
    glColor3f(1, 1, 0); // yellow
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(v.x, v.y, v.z);
    glEnd();
    glLineWidth(1.0f);

    glPushMatrix();
    glTranslatef(v.x, v.y, v.z);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex3f(0, 0, 0);
    glEnd();
    glPopMatrix();
}

void setPerspective(float fovY, float aspect, float nearZ, float farZ) {
    float f = 1.0f / tanf(fovY * 0.5f * (3.14159265f / 180.0f));
    float nf = 1.0f / (nearZ - farZ);

    float m[16] = {0};
    m[0] = f / aspect;
    m[5] = f;
    m[10] = (farZ + nearZ) * nf;
    m[11] = -1.0f;
    m[14] = 2.0f * farZ * nearZ * nf;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m);
}
