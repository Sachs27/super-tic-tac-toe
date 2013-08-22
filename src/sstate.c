#include <stdlib.h>

#include "sstate.h"


struct sstate *sstate_create() {
    struct sstate *ss;

    ss = malloc(sizeof(*ss));

    ss->top = NULL;

    return ss;
}

void sstate_push(struct sstate *ss, update_func_t update,
                                    render_func_t render) {
    struct sstate_node *ssn;

    ssn = malloc(sizeof(*ssn));

    ssn->update = update;
    ssn->render = render;
    ssn->next = ss->top;

    ss->top = ssn;
}

void sstate_pop(struct sstate *ss) {
    struct sstate_node *ssn;

    if (ss->top == NULL)
        return;

    ssn = ss->top;
    ss->top = ssn->next;

    free(ssn);
}

int sstate_update_and_render(struct sstate *ss, double delta) {
    if (ss->top == NULL)
        return SSTATE_TOP_IS_NULL;

    ss->top->update(delta);
    ss->top->render();

    return 0;
}
