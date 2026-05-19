#pragma once

#include <Eigen/Core>
#include <string>

class Shader 
{
    public:
        Shader();
        void Compile(const char** shaderSrc, int srcCount);
        
        void SetValue(const std::string& name, float value);
        void SetValue(const std::string& name, Eigen::Vector2f value);
        void SetValue(const std::string& name, Eigen::Vector3f values);
        void SetValue(const std::string& name, Eigen::Matrix4f matrix);
        
        uint32_t programId;
};

namespace ShaderSrc
{
    inline const char* surfaceShaderSrc[2] = {

        //VERTEX SHADER
        R"(
            #version 330 core

            layout (location = 0) in vec3 pos;
            layout (location = 1) in vec3 norm;
            layout (location = 2) in vec3 col;

            uniform mat4 projection;
            uniform mat4 view;

            out vec3 Normal;
            out vec3 Position;
            out vec3 Color;

            void main()
            {
                gl_Position = projection * view * vec4(pos, 1.0);

                Normal = norm;
                Position = pos;
                Color = col;
            }
        )",

        //FRAGMENT SHADER
        R"(
            #version 330 core

            in vec3 Position;
            in vec3 Normal;
            in vec3 Color;

            out vec4 FragColor;

            uniform float ambientStrength;

            uniform vec3 lightColor;
            uniform vec3 lightPos;
            uniform vec3 cameraPos;

            void main()
            {
                float Ia = ambientStrength;

                vec3 N = normalize(Normal);
                vec3 Lm = normalize(lightPos - Position);
                float Id = max(dot(N, Lm), 0.0);

                vec3 V = normalize(cameraPos - Position);
                vec3 Rm = reflect(-Lm, N);
                float Is = pow(max(dot(Rm, V), 0.0), 16) * 0.5;

                vec3 finalColor = (Ia + Id + Is) * lightColor * Color;

                FragColor = vec4(finalColor, 1.0);
            }
        )"
    };
    
    inline const char* curveShaderSrc[3] = {
        //VERTEX SHADER
        R"(
            #version 330 core

            layout (location = 0) in vec3 pos;
            layout (location = 2) in vec3 col;

            out VS_OUT {
                vec3 vColor; 
            } vs_out;

            uniform mat4 projection;
            uniform mat4 view;

            void main()
            {
                gl_Position = projection * view * vec4(pos, 1.0);

                vs_out.vColor = col;
            }

        )",

        //FRAGMENT SHADER
        R"(
            #version 330 core

            in vec3 gColor;

            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(gColor, 1.0);
            }

        )",

        //GEOMETRY SHADER
        R"(
            #version 330 core

            in VS_OUT {
                vec3 vColor;
            } gs_in[];  

            layout (lines) in;
            layout (triangle_strip, max_vertices = 4) out;

            out vec3 gColor;

            uniform vec2 viewportSize;
            uniform float width;
            
            void main()
            {
                vec2 widthPixels = width / viewportSize;

                gColor = gs_in[0].vColor;

                vec2 dir = normalize(
                        gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w - 
                        gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w
                );
                vec2 perp = vec2(-dir.y, dir.x) * widthPixels;

                vec4 offset0 = vec4(perp, 0.0, 0.0) * gl_in[0].gl_Position.w;
                vec4 offset1 = vec4(perp, 0.0, 0.0) * gl_in[1].gl_Position.w;

                gl_Position = gl_in[0].gl_Position + offset0;   EmitVertex();
                gl_Position = gl_in[0].gl_Position - offset0;   EmitVertex();
                gl_Position = gl_in[1].gl_Position + offset1;   EmitVertex();
                gl_Position = gl_in[1].gl_Position - offset1;   EmitVertex();
            }
        )"
    };
}