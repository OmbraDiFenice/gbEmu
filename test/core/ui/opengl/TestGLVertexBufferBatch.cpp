#include <gtest/gtest.h>

#include <core/ui/opengl/GLVertexBufferBatch.h>

class TestGLVertexBufferBatch : public ::testing::Test {
   protected:
    GLVertexBufferBatch _batch;

   protected:
    std::tuple<std::vector<GLVertexBuffer>, std::vector<unsigned int>>
    initBatch(float* iData, unsigned int iBufferNum, unsigned int iVertexNum,
              const VertexLayout& iLayout,
              const std::vector<std::vector<unsigned int>>& iIndexBuffers);

   private:
    GLVertexBuffer createBuffer(float* iData, int iVertexNum,
                                const VertexLayout& iLayout,
                                const std::vector<unsigned int>& iIndexes);
};

std::tuple<std::vector<GLVertexBuffer>, std::vector<unsigned int>>
TestGLVertexBufferBatch::initBatch(
    float* iData, unsigned int iBufferNum, unsigned int iVertexNum,
    const VertexLayout& iLayout,
    const std::vector<std::vector<unsigned int>>& iIndexBuffers) {
    ASSERT(iBufferNum == iIndexBuffers.size(),
           "number of index buffers doesn't match the amount of buffers");

    std::vector<unsigned int> expectedIndexes;
    std::vector<GLVertexBuffer> buffers;
    buffers.resize(iBufferNum);

    unsigned int counter = 0;
    for (int i = 0; i < iBufferNum; ++i) {
        const auto& indexes = iIndexBuffers.at(i);

        buffers[i] =
            createBuffer(&iData[i * iVertexNum], iVertexNum, iLayout, indexes);

        for (unsigned int index : indexes) {
            expectedIndexes.push_back(index + counter);
        }
        counter += indexes.size();
    }

    return {buffers, expectedIndexes};
}

GLVertexBuffer TestGLVertexBufferBatch::createBuffer(
    float* iData, int iVertexNum, const VertexLayout& iLayout,
    const std::vector<unsigned int>& iIndexes) {
    GLVertexBuffer buffer;
    buffer.setVertexBuffer(iData, iVertexNum);
    buffer.setIndexBuffer(iIndexes);
    buffer.setVertexLayout(iLayout);
    return buffer;
}

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

TEST_F(TestGLVertexBufferBatch, addBuffer_correctlyLayoutTheData) {
    constexpr unsigned int bufferNum = 2;
    constexpr unsigned int vertexNum = 3;

    // clang-format off
    float data[bufferNum][vertexNum] = {
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f}
    };
    // clang-format on
    VertexLayout layout = {{3, GL_FLOAT}};
    std::vector<unsigned int> indexes{1, 2, 3};

    auto [buffers, expectedIndexes] = initBatch(
        &data[0][0], bufferNum, vertexNum, layout, {indexes, indexes});

    float batchBuf[bufferNum * vertexNum];
    _batch.setVertexBuffer(batchBuf, bufferNum * vertexNum);
    for (int i = 0; i < bufferNum; ++i) {
        _batch.addBuffer(buffers[i]);
    }

    EXPECT_EQ(layout, _batch.getLayout());
    EXPECT_EQ(expectedIndexes, _batch.getIndexBuffer());
    for (int i = 0; i < bufferNum * vertexNum; ++i) {
        EXPECT_FLOAT_EQ(static_cast<float*>(&data[0][0])[i],
                        _batch.getVertexBuffer()[i]);
    }
}