#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <complex>
#include <cmath>

// Helper: convert state vector to Bloch vector
sf::Vector3f blochVector(const std::complex<double> psi[2]) {
    // <sigma_x> = 2 Re(psi0* psi1)
    // <sigma_y> = 2 Im(psi0* psi1)
    // <sigma_z> = |psi0|^2 - |psi1|^2
    double x = 2.0 * (psi[0].real() * psi[1].real() + psi[0].imag() * psi[1].imag());
    double y = 2.0 * (psi[0].imag() * psi[1].real() - psi[0].real() * psi[1].imag());
    double z = std::norm(psi[0]) - std::norm(psi[1]);
    return {float(x), float(y), float(z)};
}

// Draw sphere wireframe
void drawSphere(int latSegments = 20, int longSegments = 20) {
    glColor3f(0.7f, 0.7f, 0.7f);
    // Latitude lines
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
    // Longitude lines
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

// Draw axes
void drawAxes(float length = 1.2f) {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X axis red
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0); glVertex3f(length, 0, 0);
    // Y axis green
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0); glVertex3f(0, length, 0);
    // Z axis blue
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0); glVertex3f(0, 0, length);
    glEnd();
    glLineWidth(1.0f);
}

// Draw Bloch vector as arrow (line + cone-ish)
void drawVector(const sf::Vector3f& v) {
    glColor3f(1, 1, 0); // yellow
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(v.x, v.y, v.z);
    glEnd();
    glLineWidth(1.0f);

    // Simple cone tip: just a small sphere around tip for demo
    glPushMatrix();
    glTranslatef(v.x, v.y, v.z);
    // glutSolidSphere would be nice here, but let's fake it:
    // Draw a tiny icosahedron or point
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
