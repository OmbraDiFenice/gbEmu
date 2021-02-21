#include <gtest/gtest.h>

#include <core/ui/opengl/GLVertexBufferBatch.h>

class TestGLVertexBufferBatch : public ::testing::Test {
   protected:
    GLVertexBufferBatch _batch;
};

TEST_F(TestGLVertexBufferBatch,
       isLayoutCompatible_givesTrueIfLayoutWasNotAlreadyInitialized) {
    VertexLayout layout = {{2, GL_FLOAT}};

    EXPECT_TRUE(_batch.isLayoutCompatible(layout));
}

TEST_F(TestGLVertexBufferBatch,
       isLayoutCompatible_givesFalseIfNotCompaibleBecauseOfSize) {
    VertexLayout layout1 = {{2, GL_FLOAT}};
    VertexLayout layout2 = {{1, GL_FLOAT}};

    _batch.setVertexLayout(layout1);

    EXPECT_FALSE(_batch.isLayoutCompatible(layout2));
}

TEST_F(TestGLVertexBufferBatch,
       isLayoutCompatible_givesFalseIfNotCompaibleBecauseOfNumberOfElements) {
    VertexLayout layout1 = {{2, GL_FLOAT}};
    VertexLayout layout2 = {{2, GL_FLOAT}, {1, GL_FLOAT}};

    _batch.setVertexLayout(layout1);

    EXPECT_FALSE(_batch.isLayoutCompatible(layout2));
}

TEST_F(TestGLVertexBufferBatch,
       isLayoutCompatible_givesFalseIfNotCompaibleBecauseOfType) {
    VertexLayout layout1 = {{2, GL_FLOAT}};
    VertexLayout layout2 = {{2, GL_UNSIGNED_BYTE}};

    _batch.setVertexLayout(layout1);

    EXPECT_FALSE(_batch.isLayoutCompatible(layout2));
}

TEST_F(TestGLVertexBufferBatch,
       isLayoutCompatible_givesTrueIfCompatibleWithAlreadySetLayout) {
    VertexLayout layout1 = {{2, GL_FLOAT}};
    VertexLayout layout2 = {{2, GL_FLOAT}};

    _batch.setVertexLayout(layout1);

    EXPECT_TRUE(_batch.isLayoutCompatible(layout2));
}