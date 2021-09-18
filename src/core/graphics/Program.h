#pragma once
#include <core/graphics/Shader.h>
#include <vector>

class Program {
   public:
    virtual ~Program() = default;

    virtual void addShader(const std::string& iSrc, unsigned int iType)   = 0;
    virtual void loadShader(const std::string& iPath, unsigned int iType) = 0;
    virtual bool link() const                                             = 0;
    virtual void bind() const                                             = 0;
    virtual void unbind() const                                           = 0;

    virtual void setUniform(const std::string& iName, int iValue) const   = 0;
    virtual void setUniform(const std::string& iName, float iValue) const = 0;
    virtual void setUniform(const std::string& iName, size_t iSize,
                            int* iValues) const                           = 0;
    virtual void setUniformMatrix3(const std::string& iName,
                                   const float* iData) const              = 0;
    virtual void setUniformMatrix4(const std::string& iName,
                                   const float* iData) const              = 0;

    virtual void execute(unsigned int iWorkGroupX, unsigned int iWorkGroupY,
                         unsigned int iWorkGroupZ) const = 0;

   protected:
    void addShader(std::shared_ptr<const Shader> iShader) {
        _shaderList.emplace_back(iShader);
    };

   protected:
    std::vector<std::shared_ptr<const Shader>> _shaderList;
};