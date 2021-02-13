#include <Pch.h>

#include "Application.h"

#include <core/ui/GLProgram.h>
#include <core/ui/GLShader.h>
#include <core/ui/GLTexture.h>
#include <core/ui/Texture.h>
#include <glad/gl.h>
#include <utils/GLErrorMacros.h>

#include <core/emu/Video.h>

void Application::run() {
    LOG_DBG("start app");

    WindowProp prop;
    prop.width = prop.height = 640;
    Window* window = _windowManager->createWindow(prop);
    _components.push_back(window);

    window->setEventCallback(BIND_FN(Application::onEvent));

    // clang-format off
    unsigned char testTileData1[] = {
        0x7C, 0x7C, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0xFE,
        0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x00, 0x00, 0x00,
    };
    unsigned char testTileData2[] = {
        0xFF, 0xF8, 0xFF, 0x84, 0xFF, 0x84, 0xFF, 0xFC,
        0xFF, 0x82, 0xFF, 0x82, 0xFF, 0xFC, 0xFF, 0x00,
    };
    unsigned char tilePcTopLeft[] = {
        0x9F, 0x7F, 0xA0, 0x60, 0xA0, 0x60, 0xA0, 0x60,
        0xBF, 0x60, 0xBF, 0x6F, 0xF9, 0xEB, 0xBF, 0xAF,
    };
    unsigned char tilePcTopRight[] = {
        0xF9, 0xFF, 0x05, 0x07, 0x05, 0x07, 0x05, 0x07,
        0xFD, 0x07, 0xFD, 0xF7, 0xFF, 0xF7, 0xFD, 0xF5,
    };
    unsigned char tilePcBottomLeft[] = {
        0xBF, 0xAF, 0xBF, 0xAF, 0xBF, 0xA0, 0xA0, 0xBF,
        0xBF, 0xA0, 0xA0, 0xBF, 0x9F, 0x9F, 0x80, 0x80,
    };
    unsigned char tilePcBottomRight[] = {
        0xFF, 0xF5, 0xFF, 0xF5, 0xFF, 0x05, 0xF7, 0xFD,
        0xFF, 0x05, 0x07, 0xFD, 0xFD, 0xF9, 0x01, 0x01,
    };
    // clang-format on
    unsigned char tileMap[Video::kTileMapSize];
    memcpy(&tileMap[0*Video::kTileDataSize], testTileData1, Video::kTileDataSize);
    memcpy(&tileMap[1*Video::kTileDataSize], testTileData2, Video::kTileDataSize);
    memcpy(&tileMap[2*Video::kTileDataSize], tilePcTopLeft, Video::kTileDataSize);
    memcpy(&tileMap[3*Video::kTileDataSize], tilePcTopRight, Video::kTileDataSize);
    memcpy(&tileMap[4*Video::kTileDataSize], tilePcBottomLeft, Video::kTileDataSize);
    memcpy(&tileMap[5*Video::kTileDataSize], tilePcBottomRight, Video::kTileDataSize);

    Video video;
    video.decodeTileMap(tileMap);

    // clang-format off
    float vertices[4 * 5] = {
        -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 0.0f, 1.0f
    };
    // clang-format on

    unsigned int vertexArray;
    GLCall(glGenVertexArrays(1, &vertexArray));

    unsigned int vertexBuffer;
    GLCall(glGenBuffers(1, &vertexBuffer));

    unsigned int indices[6] = {0, 1, 2, 2, 3, 0};
    unsigned int indexBuffer;
    GLCall(glGenBuffers(1, &indexBuffer));

    GLProgram program;
    program.loadShader("vertex.shader", GL_VERTEX_SHADER);
    program.loadShader("fragment.shader", GL_FRAGMENT_SHADER);
    program.link();
    program.bind();

    unsigned char* testTile = video.tileMap[1];
    const unsigned int WIDTH = Video::kTileWidth;
    const unsigned int HEIGHT = Video::kTileHeight;

    // vertically flip image
    unsigned char buf[WIDTH];
    for (int i = 0; i < HEIGHT / 2; ++i) {
        memcpy(buf, &testTile[i * WIDTH], WIDTH);
        memcpy(&testTile[i * WIDTH], &testTile[(HEIGHT - 1 - i) * WIDTH],
               WIDTH);
        memcpy(&testTile[(HEIGHT - 1 - i) * WIDTH], buf, WIDTH);
    }

    // map int color to rgba
    uint32_t colorMap[] = {0xF0E0D0, 0x989898, 0x686868, 0x383838};
    unsigned char tileData[WIDTH * HEIGHT * 4];
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        memcpy(&tileData[i * 4], &colorMap[testTile[i]], 4);
    }

    // GLTexture texture("test.png");
    GLTexture texture(tileData, WIDTH, HEIGHT, 4);
    texture.bind(0);
    program.setUniform("u_Texture", 0);

    while (keepRunning()) {
        // data setup

        GLCall(glBindVertexArray(vertexArray));

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                            GL_STATIC_DRAW));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                     5 * sizeof(float), nullptr));
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                                     5 * sizeof(float),
                                     (const void*)(3 * sizeof(float))));

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                            GL_STATIC_DRAW));

        // drawing
        GLCall(glClearColor(0.15f, 0.15f, 0.15f, 1));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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
