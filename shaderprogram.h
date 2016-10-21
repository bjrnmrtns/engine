#ifndef SHADER_H
#define SHADER_H

class ShaderProgram
{
public:
    ShaderProgram(const char* vss, const char* fss);
    unsigned int GetId();
    void Use();
    virtual ~ShaderProgram();
    
private:
    unsigned int id;
};

#endif
