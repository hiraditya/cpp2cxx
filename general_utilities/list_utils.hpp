#ifndef UTILS_LIST_HPP
#define UTILS_LIST_HPP
/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */

namespace general_utilities{
  
  template<typename Node_type>
  // equality_comparable<Node_type>
  // @brief deletes a node from the list
  Node_type* delete_node(Node_type* head, Node_type* victim)
  {
    Node_type* n = head;
    if(*n == *victim){ //equality comparable
      head = head->next;
      delete n;
      return head;
    }
    while(n->next){
      if(*(n.next) == *victim){
        Node_type* temp = n.next;
        n.next = n.next.next;
        delete temp;
        break;
      }
    }
  return head;
  }

  template<typename Node_type>
  // equality_comparable<Node_type>
  // @brief finds the n-th last element of the list in
  // in linear time complexity
  Node_type* nth_to_last(Node_type* head, unsigned int n)
  {
    if(!head)
      return 0;
    Node_type *p1,*p2;
    p1 = p2 = head;
    for(unsigned int i = 0; i < n-1; ++i){
      if(!p2)
        return 0;//size of the list is < n
      else
        p2 = p2->next;
    }
    while(p2->next){
      p1 = p1->next;
      p2 = p2->next;
    }
    return p1;
  }

#include<iterator>
  // @brief Prints the elements of the list, one element per line
  template<class T>
  std::ostream& operator<<(std::ostream& os, const std::list<T>& v)
  {
      std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, "\n"));
      return os;
  }

#endif
}
#endif // UTILS_LIST_HPP

