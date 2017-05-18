#include <iostream>
#include <vector>

#include <CGAL/ogons_generators_2.h>
#include <CGAL/Cartesian.h>


typedef CGAL::Cartesian<int>             Kernel;
typedef Kernel::Point_2                  Point_2;
typedef CGAL::Grid_ogon_2                Grid_ogon_2;
typedef CGAL::Ogon_2                     Ogon_2;


void print_polygon(Ogon_2 ogon) {

  std::vector<Point_2> p;
  p.resize(ogon.get_number_of_vertices());

  ogon.get_polygon_2(p.begin());

  std::cout << ogon.get_number_of_vertices() << std::endl;

  for(std::vector<Point_2>::iterator it=p.begin(); it!=p.end(); ++it)
    std::cout << *it << std::endl;
  std::cout << std::endl;

}

int main() {

  int n_vertices = 64;
  Point_2 max_coordinates;
  float colinearity_probability = 0.5;
  int n_stretchings = n_vertices;

  Grid_ogon_2 grid_ogon = CGAL::random_grid_ogon_2(n_vertices);
  Ogon_2 ogon;

  max_coordinates = Point_2(n_vertices, 2*n_vertices);
  print_polygon(CGAL::random_ogon_from_grid_ogon_2(grid_ogon, max_coordinates, colinearity_probability, n_stretchings));
  
  max_coordinates = Point_2(2*n_vertices, n_vertices);
  print_polygon(CGAL::random_ogon_from_grid_ogon_2(grid_ogon, max_coordinates, colinearity_probability, n_stretchings));    
    
}