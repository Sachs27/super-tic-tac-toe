#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <IL/il.h>

#include "texture.h"


enum {
    IL_INIT_FAILED = 1,
    LOAD_TEXTURE_FAILED,
};

struct texture_inner {
    ILuint il_id;
    GLsizei width;
    GLsizei height;
    GLenum format;
    GLenum type;
    const GLvoid *pixels;
};


static int il_init(void) {
    ilInit();
    ilClearColor(255, 0, 0, 255);

    ILenum ilerr = ilGetError();
    if (ilerr != IL_NO_ERROR) {
        fprintf(stderr, "Failed to initialize DevIL.\n");
        return IL_INIT_FAILED;
    }

    return 0;
}

static struct texture_inner *texture_inner_load(const char *path) {
    static int il_inited = 0;
    struct texture_inner *tex_inner;

    if (!il_inited) {
        il_inited = !il_init();
        if (!il_inited)
            return NULL;
    }

    tex_inner = malloc(sizeof(*tex_inner));
    if (tex_inner == NULL)
        return NULL;

    ilGenImages(1, &tex_inner->il_id);
    ilBindImage(tex_inner->il_id);

    if (ilLoadImage(path) != IL_TRUE) {
        fprintf(stderr, "Failed to load texture: %s.\n", path);
        goto il_process_failed;
    }

    if (ilConvertImage(IL_RGBA, IL_FLOAT) != IL_TRUE)
        goto il_process_failed;

    goto il_process_success;

    il_process_failed:
    free(tex_inner);
    ilDeleteImages(1, &tex_inner->il_id);
    ilBindImage(0);
    return NULL;


    il_process_success:
    tex_inner->pixels = ilGetData();
    tex_inner->width = ilGetInteger(IL_IMAGE_WIDTH);
    tex_inner->height = ilGetInteger(IL_IMAGE_HEIGHT);
    tex_inner->format = IL_RGBA;
    tex_inner->type = IL_FLOAT;

    ilBindImage(0);

    return tex_inner;
}

static void texture_inner_destroy(struct texture_inner *tex_inner) {
    ilDeleteImages(1, &tex_inner->il_id);
    free(tex_inner);
}

struct texture *texture_load(const char *path) {
    struct texture *tex;
    struct texture_inner *tex_inner;

    tex = malloc(sizeof(*tex));
    if (tex == NULL)
        return NULL;

    tex_inner = texture_inner_load(path);
    if (tex_inner == NULL) {
        free(tex);
        return NULL;
    }

    tex->width = tex_inner->width;
    tex->height = tex_inner->height;
    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexImage2D(GL_TEXTURE_2D, 0, tex_inner->format,
            tex_inner->width, tex_inner->height, 0,
            tex_inner->format, tex_inner->type, tex_inner->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture_inner_destroy(tex_inner);

    if (glGetError() != GL_NO_ERROR) {
        free(tex);
        return NULL;
    }

    return tex;
}

void texture_destroy(struct texture *tex) {
    glDeleteTextures(1, &tex->id);
    free(tex);
}

void texture_draw(const struct texture *tex, GLfloat x, GLfloat y) {
    return texture_draw_with_transparency(tex, x, y, 1.0f);
}

void texture_draw_with_transparency(const struct texture *tex,
                                    GLfloat x, GLfloat y,
                                    GLfloat transparency) {
    if (tex->id == 0)
        return;

    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, transparency);

    glLoadIdentity();
    glTranslatef(x, y, 0.0f);

    glBindTexture(GL_TEXTURE_2D, tex->id);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(tex->width, 0.0f); glTexCoord2f(1.0f, 1.0f); glVertex2f(tex->width, tex->height);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, tex->height);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
