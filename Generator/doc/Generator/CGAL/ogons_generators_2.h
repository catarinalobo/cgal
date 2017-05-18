namespace CGAL {

/// \addtogroup PkgGenerators
/// @{

/*!
\brief computes a random grid orthogonal polygon and return it into an object of type `Grid_ogon_2`.
The polygon generated will have a number of vertices equal to `n`.

\pre \f$ n \ge 4 \f$.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\sa `CGAL::random_ogon_2()`

\cgalHeading{Implementation}

The algorithm is based on the Inflate-Paste technique.
To generate a grid ogon with `n` vertices, this method starts from a unit square and performs
\f$ (n-4)/2 \f$ iterations. In each iteration, it applies two transformations
(called Inflate and Paste) to glue a new rectangle to the previous grid ogon,
yielding a new grid ogon with one more reflex vertex.
The worst-case running time is \f$ O(n^2) \f$.
*/
Grid_ogon_2 random_grid_ogon_2(int n=4, Random &rnd=get_default_random());


/*!
\brief computes a random row-convex grid orthogonal polygon and return it into an object of type `Grid_ogon_2`.
The polygon generated will have a number of vertices equal to `n`.

\pre \f$ n \ge 4 \f$.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\sa `CGAL::random_grid_ogon_2()`
\sa `CGAL::random_row_convex_ogon_2()`

\cgalHeading{Implementation}

The algorithm is a tailored version of the Inflate-Paste algorithm,
to create only row-convex grid orthogonal polygons.
The worst-case running time is \f$ O(n) \f$.
*/
Grid_ogon_2 random_row_convex_grid_ogon_2(int n=4, Random &rnd=get_default_random());


/*!
\brief computes a random column-convex grid orthogonal polygon and return it into an object of type `Grid_ogon_2`.
The polygon generated will have a number of vertices equal to `n`.

\pre \f$ n \ge 4 \f$.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\sa `CGAL::random_grid_ogon_2()`
\sa `CGAL::random_column_convex_ogon_2()`

\cgalHeading{Implementation}

The algorithm is a tailored version of the Inflate-Paste algorithm,
to create only column-convex grid orthogonal polygons.
The worst-case running time is \f$ O(n) \f$. 
*/
Grid_ogon_2 random_column_convex_grid_ogon_2(int n=4, Random &rnd=get_default_random());


/*!
\brief computes a random convex grid orthogonal polygon and return it into an object of type `Grid_ogon_2`.
The polygon generated will have a number of vertices equal to `n`.

\pre \f$ n \ge 4 \f$.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\sa `CGAL::random_grid_ogon_2()`
\sa `CGAL::random_convex_ogon_2()`

\cgalHeading{Implementation}

The algorithm is a tailored version of the Inflate-Paste algorithm,
to create only convex grid orthogonal polygons.
The worst-case running time is \f$ O(n) \f$.
*/
Grid_ogon_2 random_convex_grid_ogon_2(int n=4, Random &rnd=get_default_random());


/*!
\brief computes a random path grid orthogonal polygon and return it into an object of type `Grid_ogon_2`.
The polygon generated will have a number of vertices equal to `n`.

\pre \f$ n \ge 4 \f$.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\sa `CGAL::random_grid_ogon_2()`
\sa `CGAL::random_path_ogon_2()`

\cgalHeading{Implementation}

The algorithm is a tailored version of the Inflate-Paste algorithm,
to create only path grid orthogonal polygons.
The worst-case running time is \f$ O(n^2) \f$.

\cgalHeading{Example}

The following program creates 100 path grid ogons with the same number of vertices. A path grid ogon
is actually defined as a thin grid ogon \cgalCite{btm-popea-04}, which is a grid ogon that has exactly \f$ 2r+1 \f$ pieces in its
rectilinear partition, where \f$ r \f$ is the number of reflex vertices. This number of pieces is minimum for the class of grid ogons. 

\cgalExample{Generator/random_thin_grid_ogons.cpp}
*/
Grid_ogon_2 random_path_grid_ogon_2(int n=4, Random &rnd=get_default_random());


/*!
\brief computes a random spiral grid orthogonal polygon and return it into an object of type `Grid_ogon_2`.
The polygon generated will have a number of vertices equal to `n`.

\pre \f$ n \ge 4 \f$.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\sa `CGAL::random_grid_ogon_2()`
\sa `CGAL::random_spiral_ogon_2()`

\cgalHeading{Implementation}

The algorithm is a tailored version of the Inflate-Paste algorithm,
to create only spiral grid orthogonal polygons.
The worst-case running time is \f$ O(n^2) \f$.
*/
Grid_ogon_2 random_spiral_grid_ogon_2(int n=4, Random &rnd=get_default_random());


/*!
\brief computes a random orthogonal polygon and return it into an object of type `Ogon_2`.
The polygon generated will have a number of vertices equal to `n`. `max_coords` has the maximum x- and y-coordinates that vertices of the polygon may have.


\pre \f$ n \ge 4 \f$.
\pre \f$ max\_coords.x() \ge n/2-1 \f$.
\pre \f$ max\_coords.y() \ge n/2-1 \f$.
\pre \f$ col\_prob \in [0,1] \f$, and it is the probability of moving an edge of the
current polygon to the same line of the previous one (if that is possible).
\pre \f$ s \ge 0 \f$, and it is the number of times that the sliding procedure is executed.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\note If `max_coords` is \f$ (0,0) \f$, it will be replaced by coordinates \f$ (n/2-1,n/2-1) \f$.

\sa `CGAL::random_grid_ogon_2()`

\cgalHeading{Implementation}

An orthogonal polygon is created by sliding edges of a grid orthogonal polygon generated by Inflate-Paste algorithm.
The sliding procedure keeps always the relative order of the edges and it is applied `s` times. This takes,
in the worst case, \f$ O(sn) \f$ running time. 

The generator may create polygons with collinear edges. For that purpose, it
performs a final transformation making use of parameter `col_prob`.
This transformation runs in \f$ O(n^2) \f$ running time.
*/
template<class Point_2> Ogon_2 random_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random());
	

/*!
\brief computes a random row-convex orthogonal polygon and return it into an object of type `Ogon_2`.
The polygon generated will have a number of vertices equal to `n`. `max_coords` has the maximum x- and y-coordinates that vertices of the polygon may have.

\pre \f$ n \ge 4 \f$.
\pre \f$ max\_coords.x() \ge n/2-1 \f$.
\pre \f$ max\_coords.y() \ge n/2-1 \f$.
\pre \f$ col\_prob \in [0,1] \f$, and it is the probability of moving an edge of the
current polygon to the same line of the previous one (if that is possible).
\pre \f$ s \ge 0 \f$, and it is the number of times that the sliding procedure is executed.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\note If `max_coords` is \f$ (0,0) \f$, it will be replaced by coordinates \f$ (n/2-1,n/2-1) \f$.

\sa `CGAL::random_row_convex_grid_ogon_2()`

\cgalHeading{Implementation}

A row-convex orthogonal polygon is created by sliding edges of a row-convex grid orthogonal polygon generated by
a tailored version of the Inflate-Paste algorithm.
The sliding procedure keeps always the relative order of the edges and it is applied `s` times. This takes,
in the worst case, \f$ O(sn) \f$ running time. 

The generator may create polygons with collinear edges. For that purpose, it
performs a final transformation making use of parameter `col_prob`.
This transformation runs in \f$ O(n^2) \f$ running time.
*/
template<class Point_2> Ogon_2 random_row_convex_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random());


/*!
\brief computes a random column-convex orthogonal polygon and return it into an object of type `Ogon_2`.
The polygon generated will have a number of vertices equal to `n`. `max_coords` has the maximum x- and y-coordinates that vertices of the polygon may have.

\pre \f$ n \ge 4 \f$.
\pre \f$ max\_coords.x() \ge n/2-1 \f$.
\pre \f$ max\_coords.y() \ge n/2-1 \f$.
\pre \f$ col\_prob \in [0,1] \f$, and it is the probability of moving an edge of the
current polygon to the same line of the previous one (if that is possible).
\pre \f$ s \ge 0 \f$, and it is the number of times that the sliding procedure is executed.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\note If `max_coords` is \f$ (0,0) \f$, it will be replaced by coordinates \f$ (n/2-1,n/2-1) \f$.

\sa `CGAL::random_column_convex_grid_ogon_2()`

\cgalHeading{Implementation}

A column-convex orthogonal polygon is created by sliding edges of a column-convex grid orthogonal polygon generated by
a tailored version of the Inflate-Paste algorithm.
The sliding procedure keeps always the relative order of the edges and it is applied `s` times. This takes,
in the worst case, \f$ O(sn) \f$ running time. 

The generator may create polygons with collinear edges. For that purpose, it
performs a final transformation making use of parameter `col_prob`.
This transformation runs in \f$ O(n^2) \f$ running time.

\cgalHeading{Example}

The following program creates 100 column-convex orthogonal polygons with a number of vertices
in the range `[50, 150]`. 

\cgalExample{Generator/random_column_convex_ogons.cpp}
*/
template<class Point_2> Ogon_2 random_column_convex_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random());


/*!
\brief computes a random convex orthogonal polygon and return it into an object of type `Ogon_2`.
The polygon generated will have a number of vertices equal to `n`. `max_coords` has the maximum x- and y-coordinates that vertices of the polygon may have.

\pre \f$ n \ge 4 \f$.
\pre \f$ max\_coords.x() \ge n/2-1 \f$.
\pre \f$ max\_coords.y() \ge n/2-1 \f$.
\pre \f$ col\_prob \in [0,1] \f$, and it is the probability of moving an edge of the
current polygon to the same line of the previous one (if that is possible).
\pre \f$ s \ge 0 \f$, and it is the number of times that the sliding procedure is executed.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\note If `max_coords` is \f$ (0,0) \f$, it will be replaced by coordinates \f$ (n/2-1,n/2-1) \f$.

\sa `CGAL::random_convex_grid_ogon_2()`

\cgalHeading{Implementation}

A convex orthogonal polygon is created by sliding edges of a convex grid orthogonal polygon generated by
a tailored version of the Inflate-Paste algorithm.
The sliding procedure keeps always the relative order of the edges and it is applied `s` times. This takes,
in the worst case, \f$ O(sn) \f$ running time. 

The generator may create polygons with collinear edges. For that purpose, it
performs a final transformation making use of parameter `col_prob`.
This transformation runs in \f$ O(n^2) \f$ running time.
*/  
template<class Point_2> Ogon_2 random_convex_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random());


/*!
\brief computes a random path orthogonal polygon and return it into an object of type `Ogon_2`.
The polygon generated will have a number of vertices equal to `n`. `max_coords` has the maximum x- and y-coordinates that vertices of the polygon may have.

\pre \f$ n \ge 4 \f$.
\pre \f$ max\_coords.x() \ge n/2-1 \f$.
\pre \f$ max\_coords.y() \ge n/2-1 \f$.
\pre \f$ col\_prob \in [0,1] \f$, and it is the probability of moving an edge of the
current polygon to the same line of the previous one (if that is possible).
\pre \f$ s \ge 0 \f$, and it is the number of times that the sliding procedure is executed.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\note If `max_coords` is \f$ (0,0) \f$, it will be replaced by coordinates \f$ (n/2-1,n/2-1) \f$.

\sa `CGAL::random_path_grid_ogon_2()`

\cgalHeading{Implementation}

A path orthogonal polygon is created by sliding edges of a path grid orthogonal polygon generated by
a tailored version of the Inflate-Paste algorithm.
The sliding procedure keeps always the relative order of the edges and it is applied `s` times. This takes,
in the worst case, \f$ O(sn) \f$ running time. 

The generator may create polygons with collinear edges. For that purpose, it
performs a final transformation making use of parameter `col_prob`.
This transformation runs in \f$ O(n^2) \f$ running time.
*/
template<class Point_2> Ogon_2 random_path_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random());


/*!
\brief computes a random spiral orthogonal polygon and return it into an object of type `Ogon_2`.
The polygon generated will have a number of vertices equal to `n`. `max_coords` has the maximum x- and y-coordinates that vertices of the polygon may have.

\pre \f$ n \ge 4 \f$.
\pre \f$ max\_coords.x() \ge n/2-1 \f$.
\pre \f$ max\_coords.y() \ge n/2-1 \f$.
\pre \f$ col\_prob \in [0,1] \f$, and it is the probability of moving an edge of the
current polygon to the same line of the previous one (if that is possible).
\pre \f$ s \ge 0 \f$, and it is the number of times that the sliding procedure is executed.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\note If `max_coords` is \f$ (0,0) \f$, it will be replaced by coordinates \f$ (n/2-1,n/2-1) \f$.

\sa `CGAL::random_spiral_grid_ogon_2()`

\cgalHeading{Implementation}

A spiral orthogonal polygon is created by sliding edges of a spiral grid orthogonal polygon generated by
a tailored version of the Inflate-Paste algorithm.
The sliding procedure keeps always the relative order of the edges and it is applied `s` times. This takes,
in the worst case, \f$ O(sn) \f$ running time. 

The generator may create polygons with collinear edges. For that purpose, it
performs a final transformation making use of parameter `col_prob`.
This transformation runs in \f$ O(n^2) \f$ running time.
*/  
template<class Point_2> Ogon_2 random_spiral_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random());


/*!
\brief computes a random orthogonal polygon from a grid orthogonal polygon already created, and return it into an object of type `Ogon_2`.
`max_coords` has the maximum x- and y-coordinates that vertices of the orthogonal polygon may have.

\pre \f$ max\_coords.x() \ge n/2-1 \f$, where
`n` is the number of vertices of `grid_ogon`.
\pre \f$ max\_coords.y() \ge n/2-1 \f$, where
`n` is the number of vertices of `grid_ogon`.
\pre \f$ col\_prob \in [0,1] \f$, and it is the probability of moving an edge of the
current polygon to the same line of the previous one (if that is possible).
\pre \f$ s \ge 0 \f$, and it is the number of times that the sliding procedure is executed.

\cgalHeading{Requires}
- `Random` is a random number generator type as provided by the STL or by `Random`.

\note If `max_coords` is \f$ (0,0) \f$, it will be replaced by coordinates \f$ (n/2-1,n/2-1) \f$, where
`n` is the number of vertices of `grid_ogon`.

\sa `CGAL::random_grid_ogon_2()`
\sa `CGAL::random_row_convex_grid_ogon_2()`
\sa `CGAL::random_column_convex_grid_ogon_2()`
\sa `CGAL::random_convex_grid_ogon_2()`
\sa `CGAL::random_path_grid_ogon_2()`
\sa `CGAL::random_spiral_grid_ogon_2()`

\cgalHeading{Implementation}

An orthogonal polygon is obtained from the grid orthogonal polygon by sliding its edges,
in such a way that the order defined by a planar sweep is kept.
The sliding procedure is applied `s` times. This takes,
in the worst case, \f$ O(sn) \f$ running time, where `n` is
the number of vertices of the grid orthogonal polygon. 

The generator may create polygons with collinear edges. For that purpose, it
performs a final transformation making use of parameter `col_prob`.
This transformation runs in \f$ O(n^2) \f$ running time.

\cgalHeading{Example}

The following program creates 100 orthogonal polygons with the same
grid orthogonal polygon, which is created as well.

\cgalExample{Generator/random_orthogonal_polygons2.cpp} 
*/
template<class Point_2> Ogon_2 random_ogon_from_grid_ogon_2(Grid_ogon_2 grid_ogon=Grid_ogon_2(), Point_2 max_coords=Point_2(0,0), float col_prob=0.0,
															int s=0, Random &rnd=get_default_random());
/// @}

} /* namespace CGAL */
