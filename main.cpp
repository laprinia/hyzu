#define GLEW_STATIC

#include "window/SampleWindow.h"
#include "shaders/ShaderManager.h"
int main() {
    SampleWindow sampleWindow = SampleWindow(800, 600, "hyzu");
    return 0;
}