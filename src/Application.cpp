#include "Application.h"
#include <glad/gl.h>
#include <core/emu/Video.h>
#include <core/ui/GLShader.h>
#include <core/ui/GLProgram.h>

void Application::run() {
    LOG_DBG("start app");

    WindowProp prop;
    prop.width = prop.height = 640;
    Window* window = _windowManager->createWindow(prop);
    _components.push_back(window);

    window->setEventCallback(BIND_FN(Application::onEvent));

    // clang-format off
    float vertices[4 * 5] = {
        -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 0.0f, 1.0f
    };
    // clang-format on

    unsigned int vertexArray;
    glGenVertexArrays(1, &vertexArray);

    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);

    unsigned int indices[6] = {0, 1, 2, 2, 3, 0};
    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);

    // clang-format off
    std::string vertexSrc = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec2 a_TexCoord;

        out vec3 v_Position;
        out vec2 v_TexCoord;

        void main()
        {
            v_TexCoord = a_TexCoord;
            v_Position = a_Position;
            gl_Position = vec4(a_Position, 1.0);
        }
    )";

    std::string fragmentSrc = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Position;
        in vec2 v_TexCoord;

        uniform sampler2D u_Texture;

        void main()
        {
            color = texture(u_Texture, v_TexCoord);
            //color = vec4(v_Position + 0.5, 1.0);
        }
    )";
    // clang-format on

    GLProgram program;
    program.addShader(vertexSrc, GL_VERTEX_SHADER);
    program.addShader(fragmentSrc, GL_FRAGMENT_SHADER);
    program.link();

    // clang-format off
    unsigned char testTile[] = {
        0, 3, 3, 3, 3, 3, 0, 0,
        2, 2, 0, 0, 0, 2, 2, 0,
        1, 1, 0, 0, 0, 1, 1, 0,
        2, 2, 2, 2, 2, 2, 2, 0,
        3, 3, 0, 0, 0, 3, 3, 0,
        2, 2, 0, 0, 0, 2, 2, 0,
        1, 1, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };
    // clang-format on
    unsigned char tileMap[Video::kTileDataTableSize * Video::kDecodedTileSize];
    memcpy(&tileMap[0], testTile, Video::kDecodedTileSize);
    unsigned int textureId;
    const unsigned int WIDTH = Video::kTileDataTableSize * 8;
    const unsigned int HEIGHT = 8;

    glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
    glTextureStorage2D(textureId, 1, GL_RGBA8, WIDTH, HEIGHT);

    glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(textureId, 0, 0, 0, WIDTH, HEIGHT, GL_RED, GL_UNSIGNED_BYTE, tileMap);

    GLint location = glGetUniformLocation(textureId, "u_Texture");
    glUniform1i(location, 0);

    while (keepRunning()) {
        // data setup
        program.bind();

        glBindVertexArray(vertexArray);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3*sizeof(float)));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        // drawing
        glClearColor(0.15f, 0.15f, 0.15f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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

void Application::onKeyEvent(KeyEvent& e) {
    TRACE_CALL
}

void Application::onMouseEvent(MouseEvent& e) {
    TRACE_CALL
}

bool Application::keepRunning() {
    return WindowManager::GetWindowCount() > 0;
}
