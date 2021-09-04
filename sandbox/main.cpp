#include <SandboxApplication.h>

int main(int argc, char **argv) {
    try {
        SandboxApplication app;
        app.run();
    } catch (...) {
        return 1;
    }
}