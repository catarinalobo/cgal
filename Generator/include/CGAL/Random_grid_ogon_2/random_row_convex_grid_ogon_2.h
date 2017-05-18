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


#ifndef CGAL_RANDOM_ROW_CONVEX_GRID_OGON_2_H
#define CGAL_RANDOM_ROW_CONVEX_GRID_OGON_2_H

#include <CGAL/Random_grid_ogon_2/Random_grid_ogon_2.h>


namespace CGAL {

  /* Generate a row-convex grid ogon by Inflate-Paste Algorithm. */
  inline Grid_ogon_2 Random_grid_ogon_2::generate_row_convex_grid_ogon_2() {
    
    init_grid_ogon();
    int n_reflex_vertices = (n_vertices-4)/2;

    while(n_reflex_vertices-- > 0) {
      
      int v = convex_vertices[rand->uniform_int<int>(0,3)];
      int v_H_adjacent = get_adjacent(v,CGAL_H);  

      FSN_info* fsn_info = get_FSN_row_convex(v,v_H_adjacent);

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
  
  /* Return the FSN of the vertex with id v. */
	inline FSN_info* Random_grid_ogon_2::get_FSN_row_convex(int v, int v_H_adjacent) {
    
    int v_V_adjacent = get_adjacent(v,CGAL_V);

    int delta_h = (get_coordinate(v,CGAL_Y)-get_coordinate(v_V_adjacent,CGAL_Y))/
                  abs(get_coordinate(v,CGAL_Y)-get_coordinate(v_V_adjacent,CGAL_Y));
    int delta_v = (get_coordinate(v_H_adjacent,CGAL_X)-get_coordinate(v,CGAL_X))/
                  abs(get_coordinate(v_H_adjacent,CGAL_X)-get_coordinate(v,CGAL_X));

    int x_limit = (current_dim+1)-((delta_v+2)%3)*(current_dim+1);

    FSN_info* fsn_info = new FSN_info(1,delta_h,delta_v);
    
    FSN rectangle;
    rectangle.y_start = get_coordinate(v,CGAL_Y);
    rectangle.x_interval[CGAL_MIN] = std::min(get_coordinate(v,CGAL_X),x_limit);
    rectangle.x_interval[CGAL_MAX] = std::max(get_coordinate(v,CGAL_X),x_limit);
    rectangle.y_finish = get_coordinate(v,CGAL_Y)+delta_h;
    fsn_info->add_rectangle(rectangle);     

    return fsn_info;
  }

  /* Pick randomly a cell in the FSN fsn_info and determine the coordinates of the northwest corner of that cell. */
  inline int* Random_grid_ogon_2::get_random_cell_in_FSN_row_convex(FSN_info* fsn_info) {

    int area = (fsn_info->rectangles[0].x_interval[CGAL_MAX]-fsn_info->rectangles[0].x_interval[CGAL_MIN])*
                abs(fsn_info->rectangles[0].y_finish-fsn_info->rectangles[0].y_start);

    int cell = rand->uniform_int<int>(1,area);

    int interval_size = fsn_info->rectangles[0].x_interval[CGAL_MAX]-fsn_info->rectangles[0].x_interval[CGAL_MIN]; 

    int* cell_coordinates = new int[2];
    cell_coordinates[CGAL_X] = fsn_info->rectangles[0].x_interval[(fsn_info->delta_v+2)%3]+fsn_info->delta_v*((cell-1)%interval_size)-(fsn_info->delta_v+2)%3;    
    cell_coordinates[CGAL_Y] = fsn_info->rectangles[0].y_start+fsn_info->delta_h*((cell-1)/interval_size)-(fsn_info->delta_h+2)%3;

    return cell_coordinates;
  }
}

#endif // CGAL_RANDOM_ROW_CONVEX_GRID_OGON_2_H //