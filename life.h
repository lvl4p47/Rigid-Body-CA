#ifndef LIFE_H
#define LIFE_H

#include "grid.h"

uint32_t Find_Free_Id();

void Cells_Init();
void Cell_Create(int16_t x, int16_t y, uint32_t parent);
void Cell_Destroy(uint32_t id);
void Cells_Update();
void Cell_Update_Start(uint32_t id);
void Cell_Update_Finish(uint32_t id);

#endif