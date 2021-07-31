#include <Pch.h>

#include "Video.h"

#include <core/emu/utils.h>

Video::Video(TilePatternAdapter& adapter) : _adapter(adapter) {
    _proj = glm::ortho(-32.0f, 32.0f, -32.0f, 32.0f);
}

void Video::update() {
    unsigned char* tileMapPatterns = loadData("tileDataTable_8800.DMP");
    unsigned char* tileMap         = loadData("tileMap_9800.DMP");

    auto backgroundTableTexture = decodeTileMapPatterns(
        reinterpret_cast<Video::CompressedTileData*>(tileMapPatterns));

    _background.setBackgroundTableTexture(backgroundTableTexture);
    _background.reindex(tileMap, true);

    unsigned char* spritePatterns = loadData("spriteDataTable_8000.DMP");
    unsigned char* oam            = loadData("OAM_FE00.DMP");

    auto spriteTableTexture = decodeSpritePatterns(
        reinterpret_cast<Video::CompressedTileData*>(spritePatterns));

    _sprites.setSpriteTableTexture(spriteTableTexture);
    _sprites.setOam(reinterpret_cast<ObjectAttributeMemory>(oam));
    _sprites.update();

    delete[] tileMapPatterns;
    delete[] spritePatterns;
    delete[] tileMap;
}

void Video::render(const GbRenderer& renderer) const {
    renderer.clear(0.15f, 0.15f, 0.15f, 1);

    renderer.render(_background);
    setCommonUniforms(_background.getProgram());

    renderer.render(_sprites);
    setCommonUniforms(_sprites.getProgram());

    renderer.flush();
}

std::shared_ptr<Texture> Video::decodeTileMapPatterns(
    CompressedTileData* iBackgroundPatterns) {
    _adapter.setTransparentColorIndex(-1);

    TileData tileMapData[kBackgroundTableSize];

    decodeTilePatterns(iBackgroundPatterns, kBackgroundTableSize, tileMapData);

    return _adapter.toTexture(reinterpret_cast<unsigned char*>(tileMapData),
                              kTileWidth, kTileHeight * kBackgroundTableSize,
                              Video::TextureSlot::Background);
}

std::shared_ptr<Texture> Video::decodeSpritePatterns(
    CompressedTileData* iSpritePatterns) {
    _adapter.setTransparentColorIndex(0);

    TileData spriteMapData[kSpriteTableSize];

    decodeTilePatterns(iSpritePatterns, kSpriteTableSize, spriteMapData);

    return _adapter.toTexture(reinterpret_cast<unsigned char*>(spriteMapData),
                              kTileWidth, kTileHeight * kSpriteTableSize,
                              Video::TextureSlot::Sprites);
}

void Video::decodeTilePatterns(
    const Video::CompressedTileData* iCompressedTilePatterns,
    const unsigned int iSize, Video::TileData* oDecodedTilePatterns) {
    for (int tileIndex = 0; tileIndex < iSize; ++tileIndex) {
        decodeTile(iCompressedTilePatterns[tileIndex],
                   oDecodedTilePatterns[tileIndex]);
    }
}

void Video::decodeTile(const Video::CompressedTileData& iTileData,
                       Video::TileData& oDecodedTile) {
    for (int row = 0; row < kTileDataSize; row += 2) {  // 2 bytes per pixel row
        unsigned char lsbyte = iTileData[row];
        unsigned char msbyte = iTileData[row + 1];

        for (int col = 0; col < 8; ++col) {  // loop over bits in each byte
            oDecodedTile[col + 8 * (row / 2)] =
                (BIT(msbyte, 7 - col) << 1) | BIT(lsbyte, 7 - col);
        }
    }
}

void Video::setCommonUniforms(const std::unique_ptr<Program>& iProgram) const {
    iProgram->setUniformMatrix3("u_Proj", &_proj[0][0]);
}
