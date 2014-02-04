/*
 * Flower polygon test
 *
 * Author: Michael Dominic K. <mdk@mdk.am>
 * 
 * This program is free software; you can redistribute it and/or modify it    
 * under the terms of the GNU General Public License as published by the      
 * Free Software Foundation; either version 2, or (at your option) any later  
 * version.                                                                   
 *                                                                          
 * This program is distributed in the hope that it will be useful, but        
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   
 * for more details.                                                          
 *                                                                            
 * You should have received a copy of the GNU General Public License along    
 * with this program; if not, write to the Free Software Foundation, Inc., 59 
 * Temple Place - Suite 330, Boston, MA 02111-1307, USA.                      
 *
 */
#include <time.h>

#include "flower-common.h"

float 
random_stand_range()
{
	return (float)rand()/RAND_MAX;
}

float 
random_special_range(float min, float max)
{
	float step = max - min;

	return min + random_stand_range() * step;
}


void 
randomize_flower (FlowerData *flower)
{
    flower->x = random_special_range (0, 640);
    flower->y = random_special_range (0, 480);
    flower->scale = random_special_range (10, 150);
    flower->rotation = random_special_range (0, 1000);

    flower->r1 = random_stand_range();
    flower->g1 = random_stand_range();
    flower->b1 = random_stand_range();
    flower->a1 = random_stand_range();

    flower->r1 = random_stand_range();
    flower->g1 = random_stand_range();
    flower->b1 = random_stand_range();
    flower->a1 = random_stand_range();
}

void 
randomize_flower_list (FlowerData *data, int floweCount)
{
    int i;

    for (i = 0; i < floweCount; i++)
        randomize_flower (&data [i]);
}

