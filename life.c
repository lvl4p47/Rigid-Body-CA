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
        
        cells[id].Z = 0;
        cells[id].e = 0;
        cells[id].energy = 0;
    }
    
    free_id = 1;
    
    // for(int z = 1; z < 37; z++)
    // {
    //     printf("z %d exs %d req %d\n", z, Valence(z), Required_Electrons(z));
    // }
}

void Cell_Create(int16_t x, int16_t y, uint32_t parent, uint8_t Z)
{
    if(Grid_Get(x, y)->type != 1 || Grid_Get(x, y)->id != 0) return;
    
    uint32_t id = Find_Free_Id();

    if(id == 0) return;
    
    cells[id].x = mod(x, grid_width);
    cells[id].y = mod(y, grid_height);
    cells[id].prev = cells[parent].prev;
    cells[id].next = parent;
    cells[id].used = 1;
    
    cells[id].Z = Z;
    cells[id].e = Z;
    cells[id].energy = 0;
    cells[id].shared = 0;
    
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
    // cells[id].shared = Required_Electrons(cells[id].e);
}

void Cell_Update_Finish(uint32_t id)
{
    uint8_t req = Required_Electrons(cells[id].e);
    if(req == 0) return;

    uint16_t x, y;
    x = cells[id].x;
    y = cells[id].y;
    
    Tile *itself, *neighbor;
    int8_t dx, dy;
    uint8_t mask, n_state;
    int16_t temp1;
    uint8_t membrane = Is_Membrane(x, y);
    
    itself = Grid_Get(x, y);
    
    for(int dir = 0; dir < 8; dir++)
    {
        dx = dir_to_coords[dir][0];
        dy = dir_to_coords[dir][1];
        mask = (uint8_t)1 << dir;
        
        if(!(itself->links & mask))
        {
            neighbor = Grid_Get(x + dx, y + dy);
            
            if(cells[id].shared < Valence(cells[id].e)
            && Required_Electrons(cells[id].e) - cells[id].shared > 0
            && cells[neighbor->id].shared < Valence(cells[neighbor->id].e)
            && Required_Electrons(cells[neighbor->id].e) - cells[neighbor->id].shared > 0)
            {
                cells[id].shared++;
                cells[neighbor->id].shared++;
                Link_Two(x, y, x + dx, y + dy);
            }
        }
    }
}

uint8_t Valence(uint8_t e)
{
    uint8_t total_e = e;
    uint8_t level = 1;
    uint8_t valence = 0;
    uint8_t max_on_level;
    
    while(total_e > 0)
    {
        max_on_level = 2 * level * level;
        if(total_e <= max_on_level)
        {
            valence = total_e;
            break;
        }
        total_e -= max_on_level;
        level += 1;
    }
    
    return valence;
}

uint8_t Required_Electrons(uint8_t e)
{
    uint8_t total_e = e;
    uint8_t level = 1;
    uint8_t valence = 0;
    uint8_t max_on_level;
    
    while(total_e > 0)
    {
        max_on_level = 2 * level * level;
        if(total_e <= max_on_level)
        {
            valence = total_e;
            break;
        }
        total_e -= max_on_level;
        level += 1;
    }
    
    return max_on_level - valence;
}

uint16_t Energy(uint32_t id1, uint32_t id2)
{
    
}