#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

struct texture {
	GLuint id;
	GLsizei width;
	GLsizei height;
};


struct texture *texture_load(const char *path);
void texture_destroy(struct texture *tex);
void texture_draw(const struct texture *tex, GLfloat x, GLfloat y);
void texture_draw_with_transparency(const struct texture *tex,
                                GLfloat x, GLfloat y, GLfloat transparency);

#endif
