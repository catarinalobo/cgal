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


#ifndef CGAL_GRID_OGON_2_H
#define CGAL_GRID_OGON_2_H

#include <utility>
#include <vector>


namespace CGAL {
  
  class Grid_ogon_2 {

  protected:
    std::vector< std::pair<int,int> > vertices;

    friend void swap(Grid_ogon_2& fst, Grid_ogon_2& snd) {
      swap(fst.vertices, snd.vertices);
    }

	public:
		Grid_ogon_2();
    template<class InputIterator> Grid_ogon_2(InputIterator first, InputIterator last);
    Grid_ogon_2(const Grid_ogon_2& grid_ogon);
    
    Grid_ogon_2& operator=(Grid_ogon_2 grid_ogon) {
      swap(*this, grid_ogon);
      return *this;
    }

    int get_number_of_vertices();

		template<class OutputIterator> OutputIterator get_polygon_2(OutputIterator result);
  };

  /* Constructors */
  inline Grid_ogon_2::Grid_ogon_2() {

    vertices.push_back(std::pair<int,int>(0,0));
    vertices.push_back(std::pair<int,int>(1,0));
    vertices.push_back(std::pair<int,int>(1,1));
    vertices.push_back(std::pair<int,int>(0,1));
  }


  template<class InputIterator> inline Grid_ogon_2::Grid_ogon_2(InputIterator first, InputIterator last) {

    for(InputIterator it=first; it!=last; ++it) {
      vertices.push_back(std::pair<int,int>(it->x(), it->y()));
    }
  }

  /* Copy Constructor */
  inline Grid_ogon_2::Grid_ogon_2(const Grid_ogon_2& grid_ogon) {

    this->vertices = grid_ogon.vertices;
  }


  inline int Grid_ogon_2::get_number_of_vertices() {
    return vertices.size();
  }

  template<class OutputIterator> inline OutputIterator Grid_ogon_2::get_polygon_2(OutputIterator result) {

  	typedef typename OutputIterator::value_type Point_2;

    for(int i=0; i<vertices.size(); i++) {
      *result = Point_2(vertices[i].first, vertices[i].second);
      ++result;
    }
    
    return result;
  }
}

#endif // CGAL_GRID_OGON_2_H //