#pragma once
#include <core/ui/Shader.h>
#include <vector>

class Program {
   public:
    virtual ~Program() = default;

    virtual bool link() const   = 0;
    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual void setUniform(const std::string& iName, int value) const = 0;
    virtual void setUniformMatrix3(const std::string& iName,
                                   const float* iData) const           = 0;

   protected:
    void addShader(std::shared_ptr<const Shader> iShader) {
        _shaderList.emplace_back(iShader);
    };

   protected:
    std::vector<std::shared_ptr<const Shader>> _shaderList;
};