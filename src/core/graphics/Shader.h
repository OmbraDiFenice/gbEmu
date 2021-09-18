#pragma once

class Shader {
   public:
    virtual bool compile() const = 0;
};

class ComputeShader {
   public:
    virtual bool compile() const = 0;
    virtual void execute() const = 0;
};