namespace CGAL {

/*!
\ingroup PkgGenerators
The class `Grid_ogon_2` is a representation of the grid orthogonal polygons (grid ogons, for short).

An instance of `Grid_ogon_2` provides a list of vertices of a particular grid orthogonal polygon. 
Any grid ogon's vertex has non-negative integer coordinates.

\sa `CGAL::Ogon_2`
*/
  
class Grid_ogon_2 {
	public:

    /// \name Creation 
    /// @{

    /*!
    Default constructor. 
    A unit square is created. 
    */ 
	Grid_ogon_2();
  	
    /*!
    Constructor that creates the grid orthogonal polygon with vertices defined by the range `[first, last)`.
    `vertices` must follow the counterclockwise order.
    
    \cgalHeading{Requires}
    - The value type of \c InputIterator must be \c Point_2.
    */
    template<class InputIterator> Grid_ogon_2(InputIterator first, InputIterator last);

    /*!
    Copy constructor.
    */
    Grid_ogon_2(const Grid_ogon_2& grid_ogon);

    /// @}

    /// \name Operations 
    /// @{

    /*!
    assignment operator. 
    */ 
    Grid_ogon_2& operator=(Grid_ogon_2 grid_ogon);

    /*!
    returns the number of vertices of the polygon. 
    */ 
    int get_number_of_vertices();

	/*!
    returns the polygon by writing its vertices (oriented counterclockwise) to result.
    
    \cgalHeading{Requires}
    - The value type of \c OutputIterator must be \c Point_2.
    */ 
    template<class OutputIterator> OutputIterator get_polygon_2(OutputIterator result);
  
    /// @}

}; /* end Grid_ogon_2 */

} /* end namespace CGAL */