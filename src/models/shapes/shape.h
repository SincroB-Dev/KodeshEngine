#ifndef SHAPE_H
#define SHAPE_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/gl.h>

#include "../../maths/maths.h"
#include "../../utils/utils.h"

class Shape2D : public Serializable
{
public:
    Transform2D &transform;
    Color &color;
    bool visible = true;
    std::vector<maths::vec2> vertices;
    std::vector<maths::vec2> t_coord;
    GLenum mode;

    inline Shape2D(Transform2D &t, Color &c, GLenum mode = GL_QUADS): Serializable(), transform(t), color(c), mode(mode) {
        Register();
    }

    inline virtual ~Shape2D() override 
    {
    }

    virtual void render() final;
    
    virtual Shape2D* Clone();

    inline void Register() override 
    {
        REGISTER_FIELD(visible, "Visivel");
        REGISTER_FIELD(color, "Cor");
    }
};

#endif
