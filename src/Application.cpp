#include <Pch.h>

#include "Application.h"

#include <core/Tile.h>
#include <core/TileMapPatternAdapter.h>
#include <core/emu/Video.h>
#include <core/emu/utils.h>
#include <core/ui/Texture.h>
#include <core/ui/opengl/GLProgram.h>
#include <core/ui/opengl/GLRenderer.h>
#include <core/ui/opengl/GLShader.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utils/GLErrorMacros.h>

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

    unsigned char* tileMapPatterns = loadData("tileMap_8800.DMP");
    Video video;
    video.decodeTileMapPatterns(tileMapPatterns);

    /* decode tile data table texture */
    TileMapPatternAdapter tileMapPatternAdapter;
    const Texture& texture = tileMapPatternAdapter.toTexture(
        reinterpret_cast<unsigned char*>(video.tileMap),
        Video::kTileWidth * Video::kTileDataTableSize, Video::kTileHeight,
        Video::kTileWidth, Video::kTileHeight);
    texture.bind(0);
    program.setUniform("u_Texture", 0);

    /* create tile data table out of many individual tiles */
    Tile tileDataTable[16][16];
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            tileDataTable[x][y].setIndex(x + 16 * y);
            tileDataTable[x][y].setPosition(x - 8, 8 - y);
        }
    }

    glm::mat3 proj = glm::ortho(-16.0f, 16.0f, -16.0f, 16.0f);
    program.setUniformMatrix3("u_Proj", &proj[0][0]);

    const Renderer& renderer = GLRenderer();

    while (keepRunning()) {
        renderer.clear(0.15f, 0.15f, 0.15f, 1);

        /* draw tile data table */
        renderer.startBatch(
            16 * 16 *
            tileDataTable[0][0].getVertexBuffer().getVertexBufferCount());
        for (int y = 0; y < 16; ++y) {
            for (int x = 0; x < 16; ++x) {
                renderer.draw(tileDataTable[x][y].getVertexBuffer());
            }
        }
        renderer.endBatch();

        for (auto c : _components) {
            c->update();
        }
    }

    delete[] tileMapPatterns;
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
