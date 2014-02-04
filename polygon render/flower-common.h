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

#include <stdlib.h>

typedef struct {

    float x;
    float y;
    float scale;
    float rotation;
    float r1;
    float g1;
    float b1;
    float a1;
    float r2; 
    float b2;
    float g2;
    float a2;
} FlowerData;

float 
random_stand_range();

float 
random_special_range(float min, float max);

void 
randomize_flower (FlowerData *flower);

void 
randomize_flower_list (FlowerData *data, int flowerCount);

