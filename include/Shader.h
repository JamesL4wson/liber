#include <Eigen/Core>
#include <string>

class Shader 
{
    public:
        Shader();
        void Compile();
        
        void SetValue(const std::string& name, float value);
        void SetValue(const std::string& name, Eigen::Vector3f values);
        void SetValue(const std::string& name, Eigen::Matrix4f matrix);
        
    private: 
        uint32_t programId;
};