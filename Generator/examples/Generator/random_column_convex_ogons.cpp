#include <iostream>
#include <vector>

#include <CGAL/ogons_generators_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>


typedef CGAL::Cartesian<int>             Kernel;
typedef Kernel::Point_2                  Point_2;
typedef CGAL::Polygon_2<Kernel>          Polygon_2;
typedef CGAL::Ogon_2                     Ogon_2;
typedef CGAL::Random                     Random;


int main() {

  Random rnd;

  int n_vertices;
  Point_2 max_coordinates;
  float colinearity_probability;
  int n_stretchings;
  
  Ogon_2 column_convex_ogon;

  Polygon_2 polygon;
  std::vector<Point_2> p;

  std::cout << "100 column-convex orthogonal polygons were generated..." << std::endl << std::endl; 
  CGAL::set_pretty_mode(std::cout);

  for(int i=0; i<100; i++) {

    n_vertices = 2 * rnd.uniform_int(25, 75);
    max_coordinates = Point_2(rnd.uniform_int(n_vertices/2, n_vertices), rnd.uniform_int(n_vertices/2, n_vertices));
    colinearity_probability = rnd.uniform_real<float>();
    n_stretchings = rnd.uniform_int(1, 2*n_vertices);

    column_convex_ogon = CGAL::random_column_convex_ogon_2(n_vertices, max_coordinates, colinearity_probability, n_stretchings, rnd);

    p.resize(n_vertices);

    column_convex_ogon.get_polygon_2(p.begin());
    polygon = Polygon_2(p.begin(), p.end());

    std::cout << "Column-convex orthogonal polygon #" << i << ":" << std::endl;
    std::cout << polygon << std::endl;

  } 
     
}