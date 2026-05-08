#include <glm/glm.hpp>
#include <string>

class Shader 
{
    public:
        Shader();
        void Compile();
        
        void SetValue(const std::string& name, float value);
        void SetValue(const std::string& name, glm::vec3 values);
        void SetValue(const std::string& name, glm::mat4 matrix);
        
    private: 
        uint32_t programId;
};