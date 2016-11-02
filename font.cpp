#include "font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <array>
#include "TextRenderer.h"

struct Character
{
    glm::vec2 position;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

bool GenerateText(const char text[], std::vector<Text::Vertex>& textbuf)
{
    FT_Library ft;
    
    if(FT_Init_FreeType(&ft)) {
        fprintf(stderr, "Could not init freetype library\n");
        return false;
    }
    
    FT_Face face;
    
    if(FT_New_Face(ft, "../OpenSans-Regular.ttf", 0, &face)) {
        fprintf(stderr, "Could not open font\n");
        return 1;
    }

    FT_Set_Pixel_Sizes(face, 0, 20);
    unsigned int width = 0;
    unsigned int height = 0;
    for(int i = 0; i < 128; i++)
    {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Could not load character \n");
            continue;
        }
        width += face->glyph->bitmap.width;
        height = std::max(height, face->glyph->bitmap.rows);
    }
    
    GLuint tex;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
    std::vector<Character> characters;
    int x = 0;
    for(int i = 0; i < 128; i++)
    {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Could not load character \n");
            continue;
        }
        FT_GlyphSlot g = face->glyph;
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
        x += g->bitmap.width;
        Character character = {
            glm::vec2(x / (float)width, 0.0f),
            glm::ivec2(g->bitmap.width, g->bitmap.rows),
            glm::ivec2(g->bitmap_left, g->bitmap_top),
            g->advance.x / 64
        };
        characters.push_back(character);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    int currentx = 0;
    for(size_t i = 0; i < strlen(text); i++)
    {
        Character c = characters[text[i]];
        int cx = currentx + c.bearing.x;
        int cy = 50 - c.bearing.y;
        int cw = c.size.x;
        int ch = c.size.y;
        currentx = currentx + c.advance;
        float tw = cw / (float)width;
        float th = ch / (float)height;
        glm::vec3 color(0.0f, 1.0f, 1.0f);
        textbuf.push_back(Text::Vertex(glm::vec2(cx + cw, cy + ch), color, glm::vec2(c.position.x, th)));
        textbuf.push_back(Text::Vertex(glm::vec2(cx, cy + ch), color, glm::vec2(c.position.x - tw, th)));
        textbuf.push_back(Text::Vertex(glm::vec2(cx, cy), color, glm::vec2(c.position.x - tw, 0.0f)));
        textbuf.push_back(Text::Vertex(glm::vec2(cx + cw, cy + ch), color, glm::vec2(c.position.x, th)));
        textbuf.push_back(Text::Vertex(glm::vec2(cx, cy), color, glm::vec2(c.position.x - tw, 0.0f)));
        textbuf.push_back(Text::Vertex(glm::vec2(cx + cw, cy), color, glm::vec2(c.position.x, 0.0f)));
    }
    
    for(Character& c : characters)
    {
        std::cout << "pos:(" << c.position.x << "), size:(" << c.size.x << "," << c.size.y << "), bearing:(" << c.bearing.x << "," << c.bearing.y << "), advance:" << c.advance << std::endl;
    }
    
    return true;
}
