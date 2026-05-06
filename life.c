#include "life.h"

Particle particles[MAX_PARTICLES];

uint32_t next_id, free_id;

uint32_t Find_Free_Id()
{
    uint32_t counter = 0;
    while((particles[free_id].used != 0 || free_id == 0)
    && counter < MAX_PARTICLES)
    {
        free_id = mod(free_id + 1, MAX_PARTICLES);
        counter++;
    }
    
    if(particles[free_id].used == 0) return free_id;
    else 
    {
        printf("no free cells\n");
        return 0;
    }
}

void Cells_Init()
{
    for(uint32_t id = 0; id < MAX_PARTICLES; id++)
    {
        particles[id].prev = 0;
        particles[id].next = 0;
        particles[id].used = 0;
        
        particles[id].rec_str = 0;
        particles[id].on_edge = 0;
        
        for(int i = 0; i < 8; i++)
        {
            particles[id].links[i] = 0;
            particles[id].buf_links[i] = 0;
        }
        
        particles[id].x = 0;
        particles[id].y = 0;
    }
    
    free_id = 1;
}

void Cell_Create(int16_t x, int16_t y, uint32_t parent, uint8_t Z)
{
    if(Grid_Get(x, y)->type != 1 || Grid_Get(x, y)->id != 0) return;
    
    uint32_t id = Find_Free_Id();

    if(id == 0) return;
    
    particles[id].x = mod(x, grid_width);
    particles[id].y = mod(y, grid_height);
    particles[id].prev = particles[parent].prev;
    particles[id].next = parent;
    particles[id].used = 1;
    
    particles[particles[id].prev].next = id;
    particles[parent].prev = id;
    
    Tile *center = Grid_Get(x, y);
    Particle *tile_part = &particles[center->id];
        
    for(int dir = 0; dir < 8; dir++)
    {
        tile_part->links[dir] = 0;
    }
    
    Grid_Get(x, y)->id = id;
    
    // add particle properties here
}

void Cell_Destroy(uint32_t id)
{
    if(id == 0) return;
    
    particles[particles[id].prev].next = particles[id].next;
    particles[particles[id].next].prev = particles[id].prev;
    
    particles[id].used = 0;
    
    Grid_Get(particles[id].x, particles[id].y)->id = 0;
    
    free_id = id;
}

void Cells_Update()
{
    uint32_t id = 0;
    next_id = 0;
    do
    {
        next_id = particles[id].next;
        if(particles[id].used
        )
        {
            Cell_Update_Stage_1(id);
        }
    
        id = next_id;
    }
    while(id != 0);
    
    id = 0;
    next_id = 0;
    do
    {
        next_id = particles[id].next;
        if(particles[id].used
        )
        {
            Cell_Update_Stage_2(id);
        }
    
        id = next_id;
    }
    while(id != 0);
}

void Cell_Update_Stage_1(uint32_t id)
{
    uint16_t x, y;
    x = particles[id].x;
    y = particles[id].y;
    
    Tile *itself;
    Particle *itself_part;
    
    itself = Grid_Get(x, y);
    itself_part = &particles[itself->id];
    
    // add update logic here
}

void Cell_Update_Stage_2(uint32_t id)
{
    uint16_t x, y;
    x = particles[id].x;
    y = particles[id].y;
    
    Tile *itself, *neighbor;
    Particle *itself_part, *neighbor_part;
    int8_t dx, dy;
    
    itself = Grid_Get(x, y);
    itself_part = &particles[itself->id];
    
    for(int dir = 0; dir < 8; dir++)
    {
        dx = dir_to_coords[dir][0];
        dy = dir_to_coords[dir][1];
        
        neighbor = Grid_Get(x + dx, y + dy);
        neighbor_part = &particles[neighbor->id];
        
        // add update logic for handling links with neighbors here
    }
}

// add functions here

