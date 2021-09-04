#include <EmuApplication.h>

int main(int argc, char **argv) {
    try {
        EmuApplication app;
        app.run();
    } catch (...) {
        return 1;
    }
}