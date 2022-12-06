//
//DO NOT MODIFY THIS FILE
//
#ifndef GRAPHTEST
#define GRAPHTEST
#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>
#include <exception>

class BadIndex : public std::exception
{
	private:
		size_t index;

	public:
		BadIndex(size_t i) : index(i) {};
		virtual const char *what(void) const throw()
		{
			static char buff[80];
			sprintf(buff, "Bad index: %li", index);
			return buff;
		}
};

template <typename VertexType, typename EdgeType>
class Graph {
	public:

		// vertex = vertex index (A, B, etc)
		typedef VertexType Vertex;
		typedef EdgeType   Edge;
		////////////////////////////////////////////////////////////
		void InsertEdge( const EdgeType & e ) { 
			outgoining_edges[ GetVertex(e.GetID1()) ].push_back ( e );
		}
		////////////////////////////////////////////////////////////
		void InsertVertex( VertexType v ) { 
			outgoining_edges.insert( 
                    std::pair<VertexType,std::vector<EdgeType> >( v, std::vector<EdgeType>() )
                    );
		}
		////////////////////////////////////////////////////////////
		const VertexType & GetVertex( size_t id) const {
			typename 
				std::map< VertexType, std::vector<EdgeType> >::const_iterator it =
				outgoining_edges.find( VertexType(id) );
			if ( it	!= outgoining_edges.end() ) return it->first;
			else {
				throw BadIndex( id );
			}
		}
		////////////////////////////////////////////////////////////
        //notice return type is a const reference - avoid unnecessary duplication
		const typename std::vector<EdgeType> & GetOutEdges( size_t id ) const {
			typename 
				std::map< VertexType, std::vector<EdgeType> >::const_iterator it =
				outgoining_edges.find( VertexType(id) );
			if ( it	!= outgoining_edges.end() ) return it->second;
			else throw BadIndex( id );
		}
		////////////////////////////////////////////////////////////
        //notice return type is a const reference - avoid unnecessary duplication
		const typename std::vector<EdgeType> & GetOutEdges( const VertexType & v) const {
			typename 
				std::map< VertexType, std::vector<EdgeType> >::const_iterator it =
				outgoining_edges.find( v );
			if ( it	!= outgoining_edges.end() ) return it->second;
			else throw BadIndex( v.ID() );
		}
		////////////////////////////////////////////////////////////
        void Print() const {
            typename std::map< VertexType, std::vector<EdgeType> >::const_iterator 
                it = outgoining_edges.begin();
            for (;it!=outgoining_edges.end();++it) {
                //std::cout << "Vertex " << it->first.GetName() 
				std::cout << "Vertex " << it->first.ID()
                    << " ("<<it->first.X()<<","<<it->first.Y()<<")" << std::endl;
                size_t size = it->second.size();
                for(size_t i=0;i<size;++i) {
                    std::cout << "\t" << /*it->second[i].GetName() <<*/ " (" << it->second[i].GetID1()
                        << " -> " << it->second[i].GetID2() << ")" << std::endl;
                }
            }
        }

	private:
		// maps vertex to a vector of outgoing edges, 
		// adjacency list
		std::map< VertexType, std::vector<EdgeType> > outgoining_edges;
};
#endif

