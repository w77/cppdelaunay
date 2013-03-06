/*
 * Site.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef SITE_H_
#define SITE_H_

#include "geom/Rectangle.h"
#include "geom/Point.h"
#include "delaunay/ICoord.h"
#include "delaunay/LR.h"
#include "defines.h"
#include <vector>


namespace Delaunay
{
	class Edge;

	namespace BoundsCheck
	{
		enum Sides
		{
			TOP = 1, BOTTOM = 2, LEFT = 4, RIGHT = 8
		};

		/**
		 *
		 * @param point
		 * @param bounds
		 * @return an int with the appropriate bits set if the Point lies on the corresponding bounds lines
		 *
		 */
		int check( const Point* point, const Rectangle& bounds )
		{
			int value = 0;
			if( point->x == bounds.left() ){
				value |= LEFT;
			}
			if( point->x == bounds.right() ){
				value |= RIGHT;
			}
			if( point->y == bounds.top() ){
				value |= TOP;
			}
			if( point->y == bounds.bottom() ){
				value |= BOTTOM;
			}
			return value;
		}

	}

	class Site : public ICoord
	{
	public:
		Site( Point* p, int index, Number weight, unsigned color );
		~Site( );

		static Site* create( Point* p, int index, Number weight, unsigned color );
		void dispose( );

		static void sortSites( std::vector< Site* >& sites );

		inline const Point* coord( ) const
		{
			return _coord;
		}

		inline const std::vector< Edge* >& edges( )
		{
			return _edges;
		}

		inline void addEdge( Edge* edge )
		{
			_edges.push_back( edge );
		}

		Edge* nearestEdge( );

		std::vector< Site* > neighborSites( );

		std::vector< Point* > region( const Rectangle& clippingBounds );

		inline Number x( )
		{
			return _coord->x;
		}

		inline Number y( )
		{
			return _coord->y;
		}

		inline Number dist( Site* p )
		{
			return Point::distance( p->coord( ), this->_coord );
		}

	private:
		/**
		 * sort sites on y, then x, coord
		 * also change each site's _siteIndex to match its new position in the list
		 * so the _siteIndex can be used to identify the site for nearest-neighbor queries
		 *
		 * haha "also" - means more than one responsibility...
		 *
		 */
		Site* init( Point* p, int index, Number weight, unsigned color );

		static Number compare( Site* s1, Site* s2 );
		static bool closeEnough( const Point* p0, const Point* p1 );

		static std::vector< Site* > _pool;

		Point* _coord;
		unsigned color;
		Number weight;
		unsigned _siteIndex;

		// the edges that define this Site's Voronoi region:
		std::vector< Edge* > _edges;

		// which end of each edge hooks up with the previous edge in _edges:
		std::vector< LR::Side > _edgeOrientations;

		// ordered list of points that define the region clipped to bounds:
		std::vector< Point* > _region;

		void move( Point* p );

		void clear( );

		Site* neighborSite( Edge* edge );

		void reorderEdges( );

		std::vector< Point* > clipToBounds( const Rectangle& bounds );

		void connect( std::vector< Point* >&, int, const Rectangle&, bool closingUp = false );

	};

} /* namespace Delaunay */
#endif /* SITE_H_ */
