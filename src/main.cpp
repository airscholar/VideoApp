#include <iostream>
#include <stdio.h>
#include <GLFW/glfw3.h>

bool loadFrame(const char *filename, unsigned char **data, int *width, int *height);

int main(int argc, const char **argv) {
    GLFWwindow *window = nullptr;

    if (!glfwInit()) {
        printf("Couldn't initialise GLFW\n");
        return 1;
    }

    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);

    if (!window) {
        printf("Couldn't load window\n");
    }

    int frame_width, frame_height;
    unsigned char *frame_data;

    if (!loadFrame("/Users/airscholar/Movies/Movie on 1-21-23 at 5.00 PM.mov", &frame_data, &frame_width,
                   &frame_height)) {
        printf("Couldn't load frame\n");
        return 1;
    }

    glfwMakeContextCurrent(window);

    GLuint tex_handle;
    glGenTextures(1, &tex_handle);
    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_data);

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // setup orphopgraphic projection
        int window_width, window_height;
        glfwGetFramebufferSize(window, &window_width, &window_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, window_width, window_height, 0, 0, -1);
        glMatrixMode(GL_MODELVIEW);

        // render the 2d
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex_handle);
        glBegin(GL_QUADS);
            glTexCoord2d(0, 0);
            glVertex2i(0, 0);
            glTexCoord2d(1, 0);
            glVertex2i(frame_width, 0);
            glTexCoord2d(1, 1);
            glVertex2i(frame_width, frame_height);
            glTexCoord2d(0, 1);
            glVertex2i(0, frame_height);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    return 0;
}
