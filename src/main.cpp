#include <Application.h>

int main(int argc, char **argv) {
    try {
        Application app;
        app.run();
    } catch (...) {
        return 1;
    }
}