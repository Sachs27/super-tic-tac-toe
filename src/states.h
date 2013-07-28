#ifndef STATES_H
#define STATES_H


void state_begin_update(double delta);
void state_begin_render();

void state_main_update(double delta);
void state_main_render();

void state_end_update(double delta);
void state_end_render();


#endif /* STATES_H */
