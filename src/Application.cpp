#include <Pch.h>

#include "Application.h"

#include <core/Tile.h>
#include <core/TileMapPatternAdapter.h>
#include <core/emu/Video.h>
#include <core/emu/utils.h>
#include <core/ui/Texture.h>
#include <core/ui/opengl/GLProgram.h>
#include <core/ui/opengl/GLRenderer.h>
#include <core/ui/opengl/GLBatchRenderer.h>
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

    TileMapPatternAdapter tileMapPatternAdapter;

    unsigned char* tileMapPatterns = loadData("tileDataTable_8800.DMP");
    unsigned char* spritePatterns = loadData("spriteDataTable_8000.DMP");
    Video video(tileMapPatternAdapter);

    video.decodeTileMapPatterns(tileMapPatterns);
    program.setUniform("u_BackgroundTableTexture", 0);

    video.decodeSpritePatterns(reinterpret_cast<Video::CompressedSprite*>(spritePatterns));
    program.setUniform("u_SpriteTableTexture", 1);

    /* create background map */
    unsigned char* tileMap = loadData("tileMap_9800.DMP");
    Tile backgroundMap[32][32];
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 32; ++x) {
            backgroundMap[x][y].setIndex((char)(tileMap[x + 32 * y]) + 128);
            backgroundMap[x][y].setPosition(x - 16, 16 - y);
        }
    }

    /* create sprite map */
    Tile spriteMap[Video::kTilesPerRow][Video::kTilesPerColumn];
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

        /* draw background */
        video.getBackgroundTableTexture()->bind(0);
        program.setUniform("u_RelativeTileWidth", 1.0f / Video::kBackgroundTableSize);
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 32; ++x) {
                renderer.draw(backgroundMap[x][y].getVertexBuffer());
            }
        }

        /* draw sprites */
        // video.getSpriteTableTexture()->bind(1);
        //program.setUniform("u_RelativeTileWidth", 1.0f / Video::kSpriteTableSize);
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
