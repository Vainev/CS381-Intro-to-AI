
`solution = std::vector<typename GraphType::Edge>`

**f(n) = g(n) + h(n)**
 - g(n) = cost so far 
 - h(n) = estimated cost to reach goal from n (heuristic)
 

## Graph

1. maps vertex to a vector of outgoing edges, adjacency list\
 `std::map< VertexType, std::vector<EdgeType> > outgoining_edges;`
 - vertex
 - vector of edges

2. insert vertex type outgoining_edges.insert( 
`std::pair<VertexType, std::vector<EdgeType> >( v, std::vector<EdgeType>() ));`

3. void InsertEdge( const EdgeType & e ) {
    `outgoining_edges[ GetVertex(e.GetID1()) ].push_back ( e );`

    - insert edge cost to vertex

Vertex Type
: vertext index (A, B, C, etc)
 - id = vertex name (a, b, etc)

Edge Type
: Edge costs between the vertexes
 - id1 = current vertex
 - id2 = next vertext thats connect
 - get weight = weight between the vertexes

OpenList
: nodes to expand
 - list vertex

ClosedList
: already expanded nodes
 - set vertex

Soluition
: solution
 - vector edges


