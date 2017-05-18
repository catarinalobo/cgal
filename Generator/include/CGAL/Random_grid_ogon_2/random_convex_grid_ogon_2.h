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


#ifndef CGAL_RANDOM_CONVEX_GRID_OGON_2_H
#define CGAL_RANDOM_CONVEX_GRID_OGON_2_H

#include <CGAL/Random_grid_ogon_2/random_row_convex_grid_ogon_2.h>


namespace CGAL {

  /* Generate a convex grid ogon by Inflate-Paste Algorithm. */
  inline Grid_ogon_2 Random_grid_ogon_2::generate_convex_grid_ogon_2() {

    init_grid_ogon();
    int n_reflex_vertices = (n_vertices-4)/2;

    while(n_reflex_vertices-- > 0) {
      
      int v = convex_vertices[rand->uniform_int<int>(0,3)];
      int v_H_adjacent = get_adjacent(v,CGAL_H);  

      FSN_info* fsn_info = get_FSN_convex(v,v_H_adjacent);

      int* cell_coordinates = get_random_cell_in_FSN_row_convex(fsn_info);

      bool cell_is_in_eH;
      inflate_paste(v,v_H_adjacent,cell_coordinates,cell_is_in_eH);

      /* Update convex_vertices set. */
      int c = get_adjacent(v,CGAL_H);
      vertex_info[c].convex_id = vertex_info[v_H_adjacent].convex_id;  
      convex_vertices[vertex_info[v_H_adjacent].convex_id] = c;
      vertex_info[v_H_adjacent].convex_id = -1;

      delete [] cell_coordinates;
    }
    
    std::vector<Random_grid_ogon_2::Point_2> grid_ogon = get_grid_ogon();
    return Grid_ogon_2(grid_ogon.begin(), grid_ogon.end());
  }

  /* Return the restricted FSN of the vertex with id v, accordingly to the state of the horizontal edge of that vertex. */
	inline FSN_info* Random_grid_ogon_2::get_FSN_convex(int v, int v_H_adjacent) {
    
    int v_V_adjacent = get_adjacent(v,CGAL_V);

    int delta_h = (get_coordinate(v,CGAL_Y)-get_coordinate(v_V_adjacent,CGAL_Y))/
                  abs(get_coordinate(v,CGAL_Y)-get_coordinate(v_V_adjacent,CGAL_Y));
    int delta_v = (get_coordinate(v_H_adjacent,CGAL_X)-get_coordinate(v,CGAL_X))/
                  abs(get_coordinate(v_H_adjacent,CGAL_X)-get_coordinate(v,CGAL_X));

    int x_limit = get_coordinate(v_H_adjacent,CGAL_X);
    if(is_coordinate_in_grid_boundary(x_limit+delta_v))
      x_limit += delta_v;

    FSN_info* fsn_info = new FSN_info(1,delta_h,delta_v);
    
    FSN rectangle;
    rectangle.y_start = get_coordinate(v,CGAL_Y);
    rectangle.x_interval[CGAL_MIN] = std::min(get_coordinate(v,CGAL_X),x_limit);
    rectangle.x_interval[CGAL_MAX] = std::max(get_coordinate(v,CGAL_X),x_limit);
    rectangle.y_finish = get_coordinate(v,CGAL_Y)+delta_h;
    fsn_info->add_rectangle(rectangle);

    return fsn_info;
  }
}

#endif // CGAL_RANDOM_CONVEX_GRID_OGON_2_H //