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


#ifndef CGAL_OGON_2_H
#define CGAL_OGON_2_H

#include <CGAL/Random_grid_ogon_2/Grid_ogon_2.h>


namespace CGAL {
  
  class Ogon_2: public Grid_ogon_2 {

  protected:
    Grid_ogon_2 grid_ogon;

    friend void swap(Ogon_2& fst, Ogon_2& snd) {
      swap(fst.vertices, snd.vertices);
      swap(fst.grid_ogon, snd.grid_ogon);
    }

  public:
  	Ogon_2();
    template<class InputIterator> Ogon_2(Grid_ogon_2 grid_ogon, InputIterator first, InputIterator last);
    Ogon_2(const Ogon_2& ogon);

    Ogon_2& operator=(Ogon_2 ogon) {
      swap(*this, ogon);
      return *this;
    }

  	Grid_ogon_2 get_grid_ogon_2();
  };

  /* Constructors */
  inline Ogon_2::Ogon_2() : Grid_ogon_2() {

    this->grid_ogon = Grid_ogon_2();
  }


  template<class InputIterator> inline Ogon_2::Ogon_2(Grid_ogon_2 grid_ogon, InputIterator first, InputIterator last) : Grid_ogon_2(first, last) {

    this->grid_ogon = grid_ogon;
  }

  /* Copy Constructor */
  inline Ogon_2::Ogon_2(const Ogon_2& ogon) : Grid_ogon_2(ogon) {

    this->grid_ogon = ogon.grid_ogon;
  }

  inline Grid_ogon_2 Ogon_2::get_grid_ogon_2() {

  	return grid_ogon;
  }    
}

#endif // CGAL_OGON_2_H //