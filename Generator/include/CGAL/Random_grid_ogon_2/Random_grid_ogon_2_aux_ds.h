// Copyright (c) 2015-2017  
// Faculty of Sciences of the University of Porto (Portugal). All rights reserved. 
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licenses holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Catarina Lobo Ferreira <up201105249@fc.up.pt>, DCC/FCUP
//                 Ana Paula Tomás <apt@dcc.fc.up.pt>, DCC/FCUP


#ifndef CGAL_RANDOM_GRID_OGON_2_AUX_DS_H
#define CGAL_RANDOM_GRID_OGON_2_AUX_DS_H

#include <CGAL/Random_grid_ogon_2/Random_grid_ogon_2_macros.h>

#include <cstddef>


namespace CGAL {

  /* A class that defines any vertex in the grid ogon. Vertices are saved into a circular list. */
  class Vertex_2 {
  
  public:
    int line_id[2];     //line_id[CGAL_H]: id of the vertex's horizontal edge (id of its horizontal line in the grid)
                        //line_id[CGAL_V]: id of the vertex's vertical edge (id of its vertical line in the grid)
    
    int prev;           //id of the previous vertex in the polygon 
    int next;           //id of the next vertex in the polygon
    
    int convex_id;      //if vertex is in convex_vertices set, then it owns an unique convex_id


    /* Constructors */
    Vertex_2() {}
    Vertex_2(int id_H, int id_V, int convex_id) {
  
      this->line_id[CGAL_H] = id_H;
      this->line_id[CGAL_V] = id_V;
      
      this->prev = -1;
      this->next = -1;
      
      this->convex_id = convex_id;
    }
    Vertex_2(int id_H, int id_V, int prev, int next, int convex_id) {
  
      this->line_id[CGAL_H] = id_H;
      this->line_id[CGAL_V] = id_V;
      
      this->prev = prev;
      this->next = next;
      
      this->convex_id = convex_id;
    }
  };

  
  /* A structure that defines any line in the grid. */
  struct Line_info {
    int coordinate;   //x-coordinate if line is vertical; y-coordinate if line is horizontal
    int vertex_id;    //id of one of line's vertices 
  };


  /* Auxiliary structures to define any FSN (free neighborhood) region. */
	struct FSN {
    int y_start;
    int x_interval[2];
    int y_finish;
  };
  
  class FSN_info {
  
  public:
    FSN* rectangles;
    int n_rectangles;
    int delta_h;
    int delta_v;


    /* Constructors */
    FSN_info() {}
    FSN_info(int max_n_rectangles, int delta_h, int delta_v) {

      this->rectangles = new FSN[max_n_rectangles];
      this->n_rectangles = 0;

      this-> delta_h = delta_h;
      this-> delta_v = delta_v;
    }

    /* Destructor */
    ~FSN_info() {

      delete [] rectangles;
    }

    void add_rectangle(FSN& rectangle) {
     
      rectangles[n_rectangles].y_start = rectangle.y_start;
      rectangles[n_rectangles].x_interval[CGAL_MIN] = rectangle.x_interval[CGAL_MIN];
      rectangles[n_rectangles].x_interval[CGAL_MAX] = rectangle.x_interval[CGAL_MAX];
      rectangles[n_rectangles].y_finish = rectangle.y_finish;

      n_rectangles++;
    }
  };

}

#endif // CGAL_RANDOM_GRID_OGON_2_AUX_DS_H //