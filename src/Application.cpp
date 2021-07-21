#include <Pch.h>

#include "Application.h"

#include <core/emu/screen/TilePatternAdapter.h>
#include <core/emu/screen/Video.h>
#include <core/emu/screen/GbRenderer.h>
#include <core/ui/opengl/GLProgram.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Application::run() {
    LOG_DBG("start app");

    WindowProp prop;
    prop.width = prop.height = 640;

    Window* window = _windowManager->createWindow(prop);
    _components.push_back(window);

    window->setEventCallback(BIND_FN(Application::onEvent));

    GLProgram program;
    program.loadShader("vertex.shader", GL_VERTEX_SHADER);
    program.loadShader("fragment.shader", GL_FRAGMENT_SHADER);
    program.link();
    program.bind();

    program.setUniform("u_BackgroundPatterns", Video::TextureSlot::Background);
    program.setUniform("u_RelativeTileWidth", 1.0f / Video::kBackgroundTableSize);
    program.setUniform("u_SpritePatterns", Video::TextureSlot::Sprites);
    program.setUniform("u_RelativeTileWidth", 1.0f / Video::kSpriteTableSize);

    TilePatternAdapter tileMapPatternAdapter;
    Video video(tileMapPatternAdapter);
    video.update();

    glm::mat3 proj = glm::ortho(-32.0f, 32.0f, -32.0f, 32.0f);
    program.setUniformMatrix3("u_Proj", &proj[0][0]);

    GbRenderer renderer = GbRenderer();

    while (keepRunning()) {
        renderer.render(video);

        for (auto c : _components) {
            c->update();
        }
    }

    LOG_DBG("end app");
}

void Application::onEvent(Event& e) {
    EventDispatcher dispatcher;
    dispatcher.dispatch<KeyEvent>(BIND_FN(Application::onKeyEvent), e);
    dispatcher.dispatch<MouseEvent>(BIND_FN(Application::onMouseEvent), e);

    for (auto c : _components) {
        c->onEvent(e);
    }
}

void Application::onKeyEvent(KeyEvent& e) { TRACE_CALL }

void Application::onMouseEvent(MouseEvent& e) { TRACE_CALL }

bool Application::keepRunning() { return WindowManager::GetWindowCount() > 0; }
