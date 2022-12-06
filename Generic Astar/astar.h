/**
 * @file astar.h
 * @author Joanna Li (li.j@digipen.edu)
 * @brief 
 *  This file contains the function declarations/implementations for the Astar class and Callback class (not used).
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef ASTAR
#define ASTAR

#include <list>
#include <vector>
#include <unordered_map>

//callback object for Astar
template <typename GraphType, typename AstarType>
class Callback 
{
    protected:
        GraphType const& g;
    public:
        Callback( GraphType const& _g) : g(_g) {}
        virtual ~Callback() {}
        virtual void OnIteration( AstarType const& ) { }
        virtual void OnFinish( AstarType const& )    { }
};

template <typename GraphType, typename Heuristic> 
class Astar 
{
    // to tell which list the vertex (node) looking at is on
    enum OnList
    {
        Neither,
        Closed,
        Open
    };

    struct Node
    {
        /**
         * @brief Construct a new Node object
         */
        Node(size_t i, float g, float h, size_t f, typename GraphType::Edge e, OnList o) :id(i), gCost(g), hCost(h), cameFrom(f), edge(e), listState(o)
        {
            
        }

        /**
         * @brief For sorting from least to greatest
         * 
         * @param rhs 
         * @return true 
         * @return false 
         */
        bool operator<(const Node&rhs ) const
        {
            return gCost + hCost < rhs.gCost + rhs.hCost;
        }

        size_t id;              // id of node
        float gCost = 0;        // cost to get to this node
        float hCost = 0;        // heuristic of this node
        size_t cameFrom;        // which node it came from
        typename GraphType::Edge edge;  // edge info
        OnList listState;               // which list its on
    };

    // typedefs
    using OpenListContainer = std::list<Node>;
    using SolutionContainer = std::vector<typename GraphType::Edge>;

    public:
        ////////////////////////////////////////////////////////////
        Astar( GraphType const& _graph, Callback<GraphType,Astar> & cb ) : 
            graph(_graph),
            openlist(),
            solution(),
            callback(cb),
            start_id(0),
            goal_id(0)
        {}

        ////////////////////////////////////////////////////////////
        // this function should not be used in the actual code
        void sample_function(size_t s, size_t g) 
        {
            start_id = s;
            goal_id  = g;
            openlist.clear();
            //closedlist.clear();
            solution.clear();
            Heuristic heuristic;
            // note "const&", since Graph returns const references, we save a 
            // temporary
            typename GraphType::Vertex const& vertex_start = graph.GetVertex(start_id);
            typename GraphType::Vertex const& vertex_goal  = graph.GetVertex(goal_id);
            //heuristic from start to goal
            typename Heuristic::ReturnType h = heuristic( graph, vertex_start, vertex_goal );
            std::cout << "Heuristic at start " << h << std::endl;


            // note "const&", since Graph returns const references, we save a 
            // temporary
			std::vector<typename GraphType::Edge> const& outedges = graph.GetOutEdges( vertex_goal );
			size_t outedges_size = outedges.size();
			for (size_t i = 0; i < outedges_size; ++i) {
				std::cout << "goal has a neighbor " << outedges[i].GetID2() << " at distance " << outedges[i].GetWeight() << std::endl;
			}
        }

        /**
         * @brief Generic A star algorithm
         * 
         * @param s 
         *  Start vertex/id
         * @param g 
         *  End vertex/id
         * @return std::vector<typename GraphType::Edge> 
         *  Contains the optimal path found from s to g
         */
        std::vector<typename GraphType::Edge> search(size_t s, size_t g)
        {
            start_id = s;
            goal_id  = g;

            openlist.clear();
            solution.clear();
            Heuristic heuristic;

            //heuristic from start to goal
            typename Heuristic::ReturnType h = heuristic( graph, graph.GetVertex(start_id), graph.GetVertex(goal_id) );
            
            // push intial node onto list
            Node newNode = Node(start_id, 0, h, start_id, typename GraphType::Edge(start_id, start_id, 0), OnList::Open);
            openlist.push_back(newNode);

            // id and pos in nodes vcector
            std::unordered_map<int, int> nodeMap;

            nodes.clear();
            nodes.reserve(100000);

            // push node onto the vector and record its position
            nodes.emplace_back(newNode);
            nodeMap[static_cast<int>(start_id)] = nodes.size() - 1;            

            while(openlist.size() > 0) 
            {   
                // grab first node off list
                auto c = openlist.begin();
                auto current = *c;

                // change the node's state to closed
                nodes[nodeMap[current.id]].listState = OnList::Closed;

                // remove from openlist
                openlist.erase(c);
                
                // stop when we reached goal
                if(current.id == goal_id)
                {
                    break;
                }

                // look at all of current node's neighbors
                std::vector<typename GraphType::Edge> neighbors = graph.GetOutEdges(current.id);

                for(unsigned i = 0; i < neighbors.size(); ++i)
                {
                    // cost of going to the neighbor
                    h = heuristic( graph, graph.GetVertex(neighbors[i].GetID2()), graph.GetVertex(goal_id) );
                    float cost = current.gCost + neighbors[i].GetWeight() + h;
                    
                    // nieghbor to check
                    size_t nextNeighbor = neighbors[i].GetID2();

                    // if vertex doesn't exist, create and add to the nodes vector and record position
                    if(nodeMap.count(static_cast<int>(nextNeighbor)) == 0)
                    {
                        nodes.emplace_back(Node(nextNeighbor, current.gCost + neighbors[i].GetWeight(), h, current.id, neighbors[i], OnList::Neither));
                        nodeMap[static_cast<int>(nextNeighbor)] = nodes.size() - 1;
                    }

                    // on close list
                    if(nodes[nodeMap[static_cast<int>(nextNeighbor)]].listState == OnList::Closed)
                    {
                        Node child = nodes[nodeMap[static_cast<int>(nextNeighbor)]];

                        // if current fvalue is less than child's
                        if(cost < child.gCost + child.hCost)
                        {
                            // remove it from closed
                            nodes[nodeMap[static_cast<int>(nextNeighbor)]].listState = OnList::Open;

                            // change its costs, parent, etc. to be current node's info
                            nodes[nodeMap[static_cast<int>(nextNeighbor)]].edge = neighbors[i];
                            nodes[nodeMap[static_cast<int>(nextNeighbor)]].gCost = current.gCost + neighbors[i].GetWeight();
                            nodes[nodeMap[static_cast<int>(nextNeighbor)]].hCost = h;
                            nodes[nodeMap[static_cast<int>(nextNeighbor)]].cameFrom = current.id;

                            // add to open list
                            openlist.push_back( nodes[nodeMap[static_cast<int>(nextNeighbor)]]);
                        }
                        else
                        {
                            // skip child if new cost is more
                            continue;
                        }
                    }
                    // not in open list
                    else if(nodes[nodeMap[static_cast<int>(nextNeighbor)]].listState != OnList::Open)
                    {
                        // add to open list
                        nodes[nodeMap[static_cast<int>(nextNeighbor)]].listState = OnList::Open;
                        openlist.push_back( nodes[nodeMap[static_cast<int>(nextNeighbor)]]);
                    }
                    // in open list
                    else if(nodes[nodeMap[static_cast<int>(nextNeighbor)]].listState == OnList::Open)
                    {
                        // using g cost as measure
                        // check if this path to neighbor is better
                        if(cost - h < nodes[nodeMap[static_cast<int>(nextNeighbor)]].gCost)
                        {
                            // update the values to be current node's info
                            nodes[nodeMap[static_cast<int>(nextNeighbor)]].cameFrom = current.id;
                            nodes[nodeMap[static_cast<int>(nextNeighbor)]].gCost = cost - h;
                            nodes[nodeMap[static_cast<int>(nextNeighbor)]].hCost = h;
                            nodes[nodeMap[static_cast<int>(nextNeighbor)]].edge = neighbors[i];

                            // remove the old node
                            auto found = std::find_if(openlist.begin(), openlist.end(), [&nextNeighbor] 
                                                    (const Node &n)
                                                    {
                                                        return n.id == nextNeighbor;
                                                    });
                            openlist.erase(found);

                            // insert the newly updated node back into list
                            openlist.push_back(nodes[nodeMap[static_cast<int>(nextNeighbor)]]);
                        }
                    }
                }
            }

            solution = recontructPath(nodeMap);
            return solution;
        }

    private:

        // do not modify 
        const GraphType &            graph;
    
        OpenListContainer            openlist;
        SolutionContainer            solution;

        // keep track of all the vertexs
        std::vector<Node> nodes;

        // do not modify 
        Callback<GraphType, Astar>  & callback;
        
        // start and goal
        size_t                       start_id, goal_id;

        std::vector<typename GraphType::Edge> recontructPath(std::unordered_map<int, int> &nodeMap);
};

#include "astar.cpp"

#endif
