#ifndef SSTATE_H
#define SSTATE_H


enum {
    SSTATE_TOP_IS_NULL = 1,
};

typedef void (*update_func_t)(double);
typedef void (*render_func_t)(void);

struct sstate_node {
    update_func_t update;
    render_func_t render;

    struct sstate_node *next;
};

struct sstate {
    struct sstate_node *top;
};


struct sstate *sstate_create();

void sstate_push(struct sstate *ss, update_func_t update,
                                    render_func_t render);

void sstate_pop(struct sstate *ss);

int sstate_update_and_render(struct sstate *ss, double delta);


#endif /* SSTATE_H */
