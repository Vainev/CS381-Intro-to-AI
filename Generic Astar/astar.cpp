/**
 * @file astar.cpp
 * @author Joanna Li (li.j@digipen.edu)
 * @brief 
 *  This file contains the function implementation for the Astar class.
 * 
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "astar.h"

/**
 * @brief 
 *  Helps reconstruct the path (vector of edges)
 * @param nodeMap 
 *  Mapping of all the nodes and their index position in nodes vector
 * @return std::vector<typename GraphType::Edge> 
 */
template <typename GraphType, typename Heuristic>
std::vector<typename GraphType::Edge> Astar<GraphType, Heuristic>::recontructPath(std::unordered_map<int, int> &nodeMap)
{
    SolutionContainer result;
    result.reserve(100);
    size_t goal = goal_id;

    Node found = nodes[nodeMap[static_cast<int>(goal)]];

    while(found.edge.GetID1() != found.edge.GetID2())
    {
        result.push_back(found.edge);

        // stop when we reach the start
        if(result.back().GetID1() == start_id || result.back().GetID2() == start_id)
        {
            break;
        }

        size_t nextVert = result.back().GetID1();
        found = nodes[nodeMap[static_cast<int>(nextVert)]];
    }

    return result;
}
