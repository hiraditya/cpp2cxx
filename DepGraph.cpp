/**
cpp2cxx is an open source software distributed under terms of the
Apache2.0 licence.

Copyrights remain with the original copyright holders.
Use of this material is by permission and/or license.

Copyright [2012] Aditya Kumar, Andrew Sutton, Bjarne Stroustrup

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "DepGraph.h"
#include "ExceptionHandler.h"
#include "debug.h"

#include <algorithm> // make_pair
#include <string>
#include <sstream>

Node* MacTree::GetParent(Node* const np)
{
  return np->parent;
}

Vertex_t MacTree::GetParent(Vertex_t const v)
{//in a uni-directed graph incoming edge can't be accessed
  Node* cn;
  Node* pn;
  cn = depGraph[v];
  pn = cn->parent;
  DEBUG_TREE(dbgs()<<"Parent nodeIndex: "<<pn->nodeIndex<<"\n";);
  //std::map<Node*,Vertex_t,NodeOrder>::iterator nodeMap_iter;
  NodeMap_t::iterator nodeMap_iter = nodeMap.find(pn);
  return nodeMap_iter->second;
}

Node const* MacTree::GetRoot() const
{
  return depGraph[startVertex];
}
/*
Node* MacTree::GetFirstChild(Vertex_t const currV)
{
  return depGraph([boost::out_edges(currV,depGraph)).first];
}
*/
std::pair<OutEdgeIterator_t, OutEdgeIterator_t>
MacTree::GetSiblings(Vertex_t const vd)
{
  Vertex_t v = GetParent(vd);
  return boost::out_edges(v,depGraph);
}

std::pair<OutEdgeIterator_t, OutEdgeIterator_t>
MacTree::GetChildren(Vertex_t const vd)
{
  return boost::out_edges(vd,depGraph);
}

int MacTree::GetNumNodes()
{
  return nodeIndex;
}

bool MacTree::IsRoot(Vertex_t const vd) const
{
  return vd == startVertex;
}

//sibling of the currently pointed vertex
bool MacTree::MakeSibling(Node& rn)
{
  DEBUG_TREE(dbgs()<<"\nMaking sibling: "<<rn.key.get_value()<<"\t"<<rn.key.get_position().get_line()<<"\n";);
  Vertex_t v_dummy;//faking
  Node* pn = new Node;
  *pn = rn;
  if(IsRoot(currVertex))
    ExceptionHandler("No sibling for root vertex.\n");
  //this line should be after the previous assignment of *pn=rn !
  Vertex_t u = GetParent(currVertex);
  pn->parent = depGraph[u];
  pn->nodeIndex = ++nodeIndex;
  NodeMap_t::iterator nodeMap_iter;
  //map::insert() returns pair<iterator,bool>
  nodeMap_iter = nodeMap.insert(std::make_pair(pn,v_dummy)).first;
  //add the pointer to node to the multimap
  //and capturing the returned vertex descriptor
  Vertex_t v = boost::add_vertex(nodeMap_iter->first, depGraph);
  nodeMap_iter-> second = v;//now assigning the vertex descriptor
  //return if the edge was created or it was already there
  //add edge returns a pair <edge_descriptor,bool>
  //Vertex_t u = GetParent(currVertex);
  bool new_edge =  boost::add_edge(u,v,depGraph).second;
  currVertex = nodeMap_iter-> second;

  DEBUG_TREE(dbgs()<<"Node Number: "<<depGraph[v]->nodeIndex<<"\n";
             dbgs()<<"n_ptr: allocated: "<<pn
                   <<"\tinserted: "<<nodeMap_iter->first<<"\n";);
  return new_edge;
}

//child to the currently pointed vertex
bool MacTree::MakeChild(Node& rn)
{
  DEBUG_TREE(dbgs()<<"\nMaking child: "<<rn.key.get_value()<<"\t"<<rn.key.get_position().get_line()<<"\n";);
  Vertex_t v_dummy;//faking
  Node* pn = new Node;
  *pn = rn;
  //this line should be after the previous one!
  pn->parent = depGraph[currVertex];
  pn->nodeIndex = ++nodeIndex;
  NodeMap_t::iterator nodeMap_iter;
  nodeMap_iter = nodeMap.insert(std::make_pair(pn,v_dummy)).first;
  Vertex_t v = boost::add_vertex(nodeMap_iter->first, depGraph);
  nodeMap_iter-> second = v;//now assigning the vertex descriptor
  //return if the edge was created or it was already there
  //add edge returns a pair <edge_descriptor,bool>
  bool new_edge = boost::add_edge(currVertex,v,depGraph).second;
  currVertex = nodeMap_iter-> second;
  DEBUG_TREE(dbgs()<<"Node Number: "<<depGraph[v]->nodeIndex<<"\n";
             dbgs()<<"n_ptr: allocated: "<<pn
                   <<"\tinserted: "<<nodeMap_iter->first<<"\n";);
  return new_edge;
}

bool MacTree::MakeChild(Vertex_t parentV, Vertex_t childV)
{
  currVertex = childV;
  DEBUG_TREE(dbgs()<<"\nMaking child\n";);
  //return if the edge was created or it was already there
  return boost::add_edge(parentV,childV,depGraph).second;
}

bool MacTree::MakeSibling(Vertex_t firstV,Vertex_t secondV)
{
  currVertex = secondV;
  DEBUG_TREE(dbgs()<<"Making sibling\n";);
  //return if the edge was created or it was already there
  Vertex_t v = GetParent(firstV);
  return boost::add_edge(v,secondV,depGraph).second;
}

void MacTree::PushBackMacro(PPMacro& mac)
{
  DEBUG_TREE(dbgs()<<"Pushing Macro: "<<mac.get_identifier_str()<<"\n";);
  depGraph[currVertex]->PushBackMacro(mac);
  //get the pointer to the macro
  PPMacro* m_ptr;
  m_ptr = depGraph[currVertex]->vecMacro.back();
  linearOrder.push_back(m_ptr);
  tokenMacroMap.insert(std::pair<token_type,PPMacro*>(m_ptr->get_identifier(),m_ptr));
}

DepList_t const& MacTree::BuildMacroDependencyList()
{
  PPMacro* m_ptr;
  std::list<token_type> id_list;
  std::list<token_type>::iterator id_list_iter;
  std::stringstream err_msg;
  TokenMacroMap_t::iterator tmm_iter;
  tmm_iter = tokenMacroMap.begin();
  std::vector<PPMacro*>::iterator mp_iter;
  mp_iter = linearOrder.begin();
  //putting the macros in macroDepList as they occur
  for(; mp_iter != linearOrder.end(); mp_iter++) {
    m_ptr = *mp_iter;
    //every loop should have a new instance to do away with emptying
    std::vector<PPMacro*> vec_mp;
    DEBUG_TREE(dbgs()<<"Processing Macro: "<<m_ptr->get_identifier_str()<<"\n";);

    id_list = m_ptr->get_replacement_list_dep_idlist();
    id_list_iter = id_list.begin();
    for(; id_list_iter != id_list.end();id_list_iter++) {
      tmm_iter = tokenMacroMap.find(*id_list_iter);
      //check if the token was not found
      try {
        if(tmm_iter != tokenMacroMap.end()) {
          vec_mp.push_back(tmm_iter->second);//if macro found
        }
        else {//if not then throw the error
          err_msg << "Exception Line Number: "
                  << id_list_iter->get_position().get_line()
                  << ", No macro found for token: "
                  << id_list_iter->get_value()<<"\n";
          throw ExceptionHandler(err_msg.str());
        }
      } catch(ExceptionHandler &e) {
        std::cout<<e.GetMessage();;
      }
    }
    macroDepList.push_back(std::make_pair(m_ptr,vec_mp));
  }
  return macroDepList;
}

void MacTree::GotoParent()
{
  DEBUG_TREE(dbgs()<<"Going to parent\n";);
  currVertex = GetParent(currVertex);
}

void MacTree::DeleteNodes()
{
  //for each out_edges call the delete
  std::pair<VertexIterator_t, VertexIterator_t> vp;
  //vertices(g) returns the pair of vertex iterators
  //the first one points to the first vertex
  //the second points past the end of the last vertex
  //dereferencing the vertex iterator gives the vertex object
  for(vp = vertices(depGraph); vp.first != vp.second; ++vp.first) {
    delete depGraph[*(vp.first)];
  }
}

bool MacTree::DeleteVertex(Vertex_t v)
{
  //first delete all the macros in the node as they are allocated on the store
  if(v == startVertex)
    return false;
  if(currVertex == v)
    currVertex = GetParent(v);//point to the parent now
  //not deleting the node now,
  //will delete the node during destruction via nodeMap
  boost::clear_vertex(v,depGraph);
  boost::remove_vertex(v,depGraph);
return true;
}

PairMacroIter_t MacTree::GetMacro(token_type const& tok)
{
  return tokenMacroMap.equal_range(tok);
}

bool MacTree::IsMacro(token_type const& tok)
{
  PairMacroIter_t m_iter = GetMacro(tok);
  //if there is atleast a macro with the token_type tok as identifier
  if(m_iter.first != m_iter.second)
    return true;
  else
    return false;
}

//checks the token to get the use case of macro
void MacTree::CheckToken(token_iterator tok_iter)
{
  PairMacroIter_t pm_iter = GetMacro(*tok_iter);

  //if there is no macro corresponding to this token

  if(pm_iter.first == pm_iter.second) {
    DEBUG_MACRO_USE_CASE(dbgs()<<"Putting token into the existing state:\n";);
    //are we collecting tokens in a macro invocation
    if(macroUseCaseState.DoneCollection()) {
      return;
    }
    else
      macroUseCaseState.PutToken(tok_iter);
  }
  else {
    /// @todo this token is a macro we are taking the first entry
    /// of the multimap which keeps macro
    /// *********collecting the args of function like macro **********
    if(pm_iter.first->second->is_function_like()) {
      macroUseCaseState.PutArgBegin(tok_iter, pm_iter.first->second);
    }
  }
}

  //algorithms
void MacTree::Dfs(Vertex_t const startV)
{
  //boost::depth_first_search(depGraph);
}

void MacTree::Bfs(Vertex_t const startV)
{

}

void MacTree::TopSort(std::vector<Graph_t::vertex_descriptor>& topo_order)
{
  boost::topological_sort(depGraph,std::back_inserter(topo_order));
}
