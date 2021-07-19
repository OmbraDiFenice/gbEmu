#include <Pch.h>

#include "Application.h"

#include <core/emu/screen/BackgroundMap.h>
#include <core/emu/screen/TileBuffer.h>
#include <core/emu/screen/TilePatternAdapter.h>
#include <core/emu/screen/Video.h>
#include <core/emu/utils.h>
#include <core/ui/opengl/GLBatchRenderer.h>
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

    TilePatternAdapter tileMapPatternAdapter;

    unsigned char* tileMapPatterns = loadData("tileDataTable_8800.DMP");
    unsigned char* spritePatterns = loadData("spriteDataTable_8000.DMP");
    Video video(tileMapPatternAdapter);

    video.decodeTileMapPatterns(reinterpret_cast<Video::CompressedTileData*>(tileMapPatterns));

    video.decodeSpritePatterns(reinterpret_cast<Video::CompressedTileData*>(spritePatterns));
    program.setUniform("u_SpriteTableTexture", 1);

    /* create background map */
    unsigned char* tileMap = loadData("tileMap_9800.DMP");

    BackgroundMap backgroundMap;
    backgroundMap.reindex(tileMap, true);

    /* create sprite map */
    TileBuffer spriteMap[32][32];
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            spriteMap[x][y].setIndex(x + 16 * y);
            spriteMap[x][y].setPosition(x - 16, 16 - y);
        }
    }

    glm::mat3 proj = glm::ortho(-32.0f, 32.0f, -32.0f, 32.0f);
    program.setUniformMatrix3("u_Proj", &proj[0][0]);

    const Renderer& renderer = GLBatchRenderer(32*32);

    while (keepRunning()) {
        renderer.clear(0.15f, 0.15f, 0.15f, 1);

        backgroundMap.render(renderer, video, program);

        /* draw sprites */
        // video.getSpriteTableTexture()->bind(1);
        // program.setUniform("u_RelativeTileWidth", 1.0f / Video::kSpriteTableSize);
        // for (int y = 0; y < 16; ++y) {
        //     for (int x = 0; x < 16; ++x) {
        //         renderer.draw(spriteMap[x][y].getVertexBuffer());
        //     }
        // }

        renderer.flush();

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
