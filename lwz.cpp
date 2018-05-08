#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <bitset>
#include <typeinfo>
#include <cmath>

using namespace std;
map<string,int> create_dict(){
  map<string,int> dict;
  int a = 0;
  while(a < 256){
    dict.emplace(string(1,a),a);
    a++;
  }
  return dict;
}

vector<unsigned char> pack(vector<unsigned int>& unpacked_buff){
  unsigned int var_width = 9;
  vector<unsigned char> packed_buff;
  unsigned int reste;
  unsigned int n = var_width;
  int i;
  for(i = 0; i < unpacked_buff.size();++i){
    if(i >= pow(2,var_width)-1)
      ++var_width;
    packed_buff.push_back(unpacked_buff[i] >> (n - 8));
    reste = unpacked_buff[i] - (packed_buff.back()<<(n - 8));
    if(n >= 16){
      int v = (reste << var_width) | unpacked_buff[i+1];
      int first_val = v >> (((n-8)-8)+var_width);
      packed_buff.push_back(first_val);
      int rnb = ((n-8) + var_width - 16);
      int not_r =  v >> rnb;
      int second_val = not_r -( first_val << 8);
      packed_buff.push_back(second_val);
      int r = v - (not_r << rnb);
      if(i+2 >= pow(2,var_width)-1)
        ++var_width;
      unpacked_buff[i+2] = r << var_width | unpacked_buff[i+2];
      n = var_width + rnb;
      ++i;
    }
    else{
      unpacked_buff[i+1] = reste << var_width | unpacked_buff[i+1];
      n = var_width + (n - 8);
    }
  }
  return packed_buff;
}

void compress(char* myfile,char* file_created){
  map<string,int> dict;
  dict = create_dict();
  ifstream file(myfile, ios::binary | ::ios::ate);
  streamsize size = file.tellg();
  file.seekg(0, ios::beg);
  vector<unsigned char> buffer(size);
  vector<unsigned int> compressed_file;
  if (file.read((char*)buffer.data(), size))
    {
      string w = "";
      vector<unsigned char>::const_iterator i = buffer.begin();
      int nb(256);
      string c;
      string p;
      while(i < buffer.end()){
        c.assign(1,*i);
        p = w + c;
        map<string,int>::iterator f;
        f = dict.find(p);
        if(f != dict.end())
          w = p;
        else{
          dict.emplace(p,nb);
          unsigned int val = (int)dict[w];
          compressed_file.push_back(val);
          w = c;
          ++nb;
        }
        ++i;
      }
      ofstream f(file_created);
      vector<unsigned char> packed_buff = pack(compressed_file);
      f.write((char*)&packed_buff[0],packed_buff.size());
    }
}

int main(int argc,char* argv[]){
  compress(argv[1],argv[2]);
}
