#include <glm/glm.hpp>
#include <string>

class Shader 
{
    public:
        Shader(const std::string& vertexCode, const std::string& fragmentCode);
        void Use();

        static std::string ReadShaderFile(const std::string& fileName);

        void SetValue(const std::string& name, glm::vec3 values);
        void SetValue(const std::string& name, glm::mat4 matrix);
    private: 
        uint32_t programId;
};