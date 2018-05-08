#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <bitset>
#include <typeinfo>

using namespace std;
map<string,short> create_dict(){
  map<string,short> dict;
  int a = 0;
  while(a < 256){
    dict.emplace(string(1,a),a);
    a++;
  }
  return dict;
}
void print_str(string& str,char* name){
  for(int i= 0; i < str.size();++i){
    cout << name <<"[" << i << "] : " <<str[i] << " ";
  }
  cout <<"\n";
}
void print_dict(map<string,short>& dict){
  map<string,short>::iterator it= dict.begin();

}

void print_map(map<string,short>& mymap){
  map<string,short>::iterator it=mymap.begin();
  while(it != mymap.end()){
    cout << "map["<<it->first<<"] : "<<it->second<<endl;
    ++it;
  }
}

void compress(char* myfile,char* file_created){
  map<string,short> dict;
  dict = create_dict();
  //  print_map(dict);
  ifstream file(myfile, ios::binary | ::ios::ate);
  streamsize size = file.tellg();
  file.seekg(0, ios::beg);
  vector<unsigned char> buffer(size);
  if (file.read((char*)buffer.data(), size))
    {
      ofstream file2;
      file2.open(file_created);
      string w = "";
      vector<unsigned char>::const_iterator i = buffer.begin();
      int nb(256);
      string c;
      string p;
      while(i < buffer.end()){
        c.assign(1,*i);
        p = w + c;
        map<string,short>::iterator f;
        f = dict.find(p);
        if(f != dict.end())
          w = p;
        else{
          dict.emplace(p,nb);
          short val = dict[w];
          cout << "val : "<< val << endl;
          bitset<16> x(val);
          file2.write((char*)&val,2);
          w = c;
          ++nb;
        }
        ++i;
      }
    }
}

int main(int argc,char* argv[]){
  compress(argv[1],argv[2]);
  /*short a(224);
  bitset<16>x(a);
  cout << "x : " << x << endl;*/
}
