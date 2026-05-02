#include "life.h"

Cell cells[MAX_CELLS];

uint32_t next_id, free_id;

uint32_t mutation_rarity = 1000000;

uint32_t Find_Free_Id()
{
    uint32_t counter = 0;
    while((cells[free_id].used != 0 || free_id == 0)
    && counter < MAX_CELLS)
    {
        free_id = mod(free_id + 1, MAX_CELLS);
        counter++;
    }
    
    if(cells[free_id].used == 0) return free_id;
    else 
    {
        printf("no free cells\n");
        return 0;
    }
}

void Cells_Init()
{
    for(uint32_t id = 0; id < MAX_CELLS; id++)
    {
        cells[id].x = 0;
        cells[id].y = 0;
        cells[id].prev = 0;
        cells[id].next = 0;
        cells[id].used = 0;
    }
    
    free_id = 1;
}

void Cell_Create(int16_t x, int16_t y, uint32_t parent)
{
    if(Grid_Get(x, y)->type != 1 || Grid_Get(x, y)->id != 0) return;
    
    uint32_t id = Find_Free_Id();

    if(id == 0) return;
    
    cells[id].x = mod(x, grid_width);
    cells[id].y = mod(y, grid_height);
    cells[id].prev = cells[parent].prev;
    cells[id].next = parent;
    cells[id].used = 1;
    
    cells[cells[id].prev].next = id;
    cells[parent].prev = id;
    
    Grid_Get(x, y)->id = id;
}

void Cell_Destroy(uint32_t id)
{
    if(id == 0) return;
    
    cells[cells[id].prev].next = cells[id].next;
    cells[cells[id].next].prev = cells[id].prev;
    
    cells[id].used = 0;
    
    Grid_Get(cells[id].x, cells[id].y)->id = 0;
    
    free_id = id;
}

void Cells_Update()
{
    uint32_t id = 0;
    next_id = 0;
    do
    {
        next_id = cells[id].next;
        if(cells[id].used
        )
        {
            Cell_Update_Start(id);
        }
    
        id = next_id;
    }
    while(id != 0);
    
    id = 0;
    next_id = 0;
    do
    {
        next_id = cells[id].next;
        if(cells[id].used
        )
        {
            Cell_Update_Finish(id);
        }
    
        id = next_id;
    }
    while(id != 0);
}

void Cell_Update_Start(uint32_t id)
{
    uint16_t x, y;
    x = cells[id].x;
    y = cells[id].y;

    Tile *itself, *neighbor;
    int8_t dx, dy;
    uint8_t mask, n_state;
    
    itself = Grid_Get(x, y);
    itself->buf_state = itself->state;
    
    if(itself->type != 1) return;
    
    if(itself->state == 0)
    {
        itself->buf_state = 1;
        return;
    }
    if(itself->state != 1)
    {
        itself->buf_state = 0;
        return;
    }
    
    for(int dir = 0; dir < 8; dir++)
    {
        dx = dir_to_coords[dir][0];
        dy = dir_to_coords[dir][1];
        mask = (uint8_t)1 << dir;
        
        if(itself->links & mask)
        {
            neighbor = Grid_Get(x + dx, y + dy);
            n_state = neighbor->state;
            
            if(n_state == 0 || n_state == 1) continue;
            
            if(abs(itself->buf_state - itself->des_state) > abs(itself->des_state - n_state))
                itself->buf_state = n_state;
        }
    }
}

void Cell_Update_Finish(uint32_t id)
{
    uint16_t x, y;
    x = cells[id].x;
    y = cells[id].y;
    
    Tile *itself, *neighbor;
    int8_t dx, dy;
    uint8_t mask, n_state;
    int16_t temp1;
    uint8_t membrane = Is_Membrane(x, y);
    
    itself = Grid_Get(x, y);
    itself->state = itself->buf_state;
    
    // switch (itself->state)
    // {
    // case 2:
    //     if(membrane < 6) break;
    //     for(int dir = 0; dir < 8; dir++)
    //     {
    //         dx = dir_to_coords[dir][0];
    //         dy = dir_to_coords[dir][1];
    //         mask = (uint8_t)1 << dir;
            
    //         if(itself->links & mask)
    //         {
    //             neighbor = Grid_Get(x + dx, y + dy);
    //             n_state = neighbor->state;
                
    //             if(n_state == 0)
    //             {
    //                 temp1 = mod(coords_to_dir[dy + 1][dx + 1] + 1, 8);
    //                 Rec_Push(x, y, dir_to_coords[temp1][0], dir_to_coords[temp1][1], 16, 0);
    //                 break;
    //             }
    //         }
    //     }
    //     break;
    // case 3:
    //     if(membrane < 6) break;
    //     for(int dir = 0; dir < 8; dir++)
    //     {
    //         dx = dir_to_coords[dir][0];
    //         dy = dir_to_coords[dir][1];
    //         mask = (uint8_t)1 << dir;
            
    //         if(itself->links & mask)
    //         {
    //             neighbor = Grid_Get(x + dx, y + dy);
    //             n_state = neighbor->state;
                
    //             if(n_state == 0)
    //             {
    //                 temp1 = mod(coords_to_dir[dy + 1][dx + 1] - 1, 8);
    //                 Rec_Push(x, y, dir_to_coords[temp1][0], dir_to_coords[temp1][1], 16, 0);
    //                 break;
    //             }
    //         }
    //     }
    //     break;
    
    // default:
    //     break;
    // }
    
    // if(rnd() % mutation_rarity == 0)
    //     Grid_Signal(x, y, (rnd() % max_states) * (rnd() % 2));
}