#include<iostream>
#include<fstream>
#include<set>
#include<algorithm>
#include<map>
#include<string>

template<typename T>
std::ostream& operator<<(std::ostream& os,std::set<T> const& l)
{
  std::for_each(l.begin(),l.end(),
      [&os](T t){
        os<<"  - "<<t<<"\n";
      });
return os;
}

using namespace std;
// this files only requires list of files from grep and aftre that a little preprocessing
// grep '#include' * > files_list
// it outputs in following format
// file_name: #include "header.h" or <header.h>
// and this program takes format
// file_name header.h
// so all it requires is:
//  a. removal of #include,
//  b. removal of quotes/ angle brackets
//  c. removal of the colon(:) sign
int main()
{
  //the files is represented as follows
  //file1 file2
  //that means file1 includes file2 as header
  const char* file_name = "files_list";
  ifstream fp(file_name);
  if(!fp.is_open())
    return -1;

  pair<string,string> file_include_pair;
  map<string,set<string> > dep_list;
  while(fp.good()){
    fp >> file_include_pair.first;
    fp >> file_include_pair.second;
    //what we want is an index on header file
    //so that we know how many files include this particular header-file
    dep_list[file_include_pair.second].insert(file_include_pair.first);
  }

  //print the list
  //make sure the dm_dir is already there
  ofstream ofp("dm_dir/include_dependency.yaml");
  if(!ofp.is_open())
    return -1;
  map<string,set<string> >::iterator dep_list_iter;
  for_each(dep_list.begin(), dep_list.end(), 
      [&ofp](pair<string,set<string> > p){
        std::cout<<"\nPrinting "<<p.first<<" and "<<p.second;
        ofp<<p.first<<":\n"<<p.second<<"\n";
      });
return 0;
}
