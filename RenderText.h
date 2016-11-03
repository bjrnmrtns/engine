#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexDefinitions.h"
#include "RenderTools.h"

namespace Render {
    namespace Text {
        class Source {
        public:
            Source();
            void BufferData(const ::Text::Vertex data[], int elements);
            ~Source();
            void Draw();

            unsigned int vbo;
            unsigned int vao;
            int elements = -1;
        };

        class Renderer {
        public:
            Renderer();
            void Draw(Source &source, const glm::mat4 &projection) const;

        private:
            const int shaderProgramId;
            const int projectionlocation;
        };
    }
}


#endif
