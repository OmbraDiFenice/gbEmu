#include <Pch.h>

#include "Application.h"

#include <core/Tile.h>
#include <core/TileMapAdapter.h>
#include <core/emu/Video.h>
#include <core/emu/utils.h>
#include <core/ui/Texture.h>
#include <core/ui/opengl/GLProgram.h>
#include <core/ui/opengl/GLShader.h>
#include <glad/gl.h>
#include <utils/GLErrorMacros.h>
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

    unsigned char* tileMap = loadData("tileMap_8800.DMP");
    Video video;
    video.decodeTileMap(tileMap);

    TileMapAdapter tileMapAdapter;
    const Texture& texture = tileMapAdapter.toTexture(
        reinterpret_cast<unsigned char*>(video.tileMap),
        Video::kTileWidth * Video::kTileDataTableSize, Video::kTileHeight,
        Video::kTileWidth, Video::kTileHeight);
    texture.bind(0);
    program.setUniform("u_Texture", 0);

    Tile tile1(0, 1, 139);
    Tile tile2(1, 1, 140);
    Tile tile3(0, 0, 155);
    Tile tile4(1, 0, 156);
    Tile tile5(0, -1, 171);
    Tile tile6(1, -1, 172);

    glm::mat3 proj = glm::ortho(-16.0f, 16.0f, -16.0f, 16.0f);
    program.setUniformMatrix3("u_Proj", &proj[0][0]);

    while (keepRunning()) {
        GLCall(glClearColor(0.15f, 0.15f, 0.15f, 1));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        tile1.bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        tile2.bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        tile3.bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        tile4.bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        tile5.bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        tile6.bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        for (auto c : _components) {
            c->update();
        }
    }

    delete[] tileMap;
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
