#include <core/emu/utils.h>

unsigned char* loadData(const char* filename) {
    LOG_INFO("loading file " << filename);
    std::ifstream file{filename, std::ios::binary};

    if(file.fail()) {
        LOG_ERROR("failed to open file '" << filename << "'");
        return nullptr;
    }

    LOG_DBG("measuring file size");
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    LOG_DBG("file size is " << fileSize << " bytes");

    LOG_DBG("reading data");
    auto _data = new unsigned char[fileSize];
    file.read(reinterpret_cast<char*>(_data), fileSize);
    LOG_DBG("data read");

    if(file.fail()) {
        LOG_ERROR("failed to read file '" << filename <<"'");
        delete[] _data;
        return nullptr;
    }
    return _data;
}
