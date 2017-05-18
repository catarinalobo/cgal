#include <iostream>
#include <vector>

#include <CGAL/ogons_generators_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>


typedef CGAL::Cartesian<int>             Kernel;
typedef Kernel::Point_2                  Point_2;
typedef CGAL::Polygon_2<Kernel>          Polygon_2;
typedef CGAL::Grid_ogon_2                Grid_ogon_2;
typedef CGAL::Ogon_2                     Ogon_2;
typedef CGAL::Random                     Random;


int main() {

  Random rnd;

  int n_vertices;
  Point_2 max_coordinates;
  float colinearity_probability;
  int n_stretchings;

  n_vertices = 2 * rnd.uniform_int(25, 75);

  Grid_ogon_2 grid_ogon = CGAL::random_grid_ogon_2(n_vertices, rnd);

  Polygon_2 polygon;
  std::vector<Point_2> p;

  p.resize(n_vertices);
  grid_ogon.get_polygon_2(p.begin());
  polygon = Polygon_2(p.begin(), p.end());
  
  CGAL::set_pretty_mode(std::cout);
  std::cout << "The following " << n_vertices << "-vertex grid orthogonal polygon was generated: " << std::endl;
  std::cout << polygon << std::endl;

  Ogon_2 ogon;

  std::cout << "100 orthogonal polygons were generated from this grid orthogonal polygon..." << std::endl << std::endl; 

  for(int i=0; i<100; i++) {
    
    max_coordinates = Point_2(rnd.uniform_int(n_vertices/2, 2*n_vertices), rnd.uniform_int(n_vertices/2, 2*n_vertices));
      
    colinearity_probability = rnd.uniform_real<float>();
      
    n_stretchings = rnd.uniform_int(n_vertices/2, n_vertices);

    ogon = CGAL::random_ogon_from_grid_ogon_2(grid_ogon, max_coordinates, colinearity_probability, n_stretchings, rnd);
    
    ogon.get_polygon_2(p.begin());
    polygon = Polygon_2(p.begin(), p.end());
    
    std::cout << "Orthogonal polygon #" << i << ":" << std::endl;
    std::cout << polygon << std::endl;

  }
      
}