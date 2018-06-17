#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cmath>
#include <bitset>

using namespace std;

void create_dict(map<int,string>& dict){
  int a = 0;
  while(a < 256){
    dict.emplace(a,string(1,a));
    ++a;
  }
}

void clear_dict(map<int,string>& dict){
  dict.clear();
  create_dict(dict);
}

vector<int> unpack(vector<unsigned char>& packed_buff){
  int var_width = 9;
  vector<int>upacked2;
  for(int k = 0 ; k < packed_buff.size();++k)
    upacked2.push_back(packed_buff[k]);
  vector<int> unpacked_buff;
  int reste;
  int n = var_width;
  int i,j;
  int to_push;
  int size = 8;
  for(i = 0, j = 256; i < upacked2.size()-1;++i){
    if(j > (pow(2,13)-2)){
      var_width = 9;
      j = 256;
    }
    if(j > (pow(2,var_width)-1))
      ++var_width;
    if((8 - (var_width -size))< 0){
      upacked2[i+1] = upacked2[i] << 8 | upacked2[i+1];
      size = 8 + size;
      ++i;
    }
    reste = upacked2[i+1] >> (8 - (var_width - size));
    to_push = upacked2[i] << (var_width - size) | reste;
    unpacked_buff.push_back(to_push);
    ++j;
    upacked2[i+1] = upacked2[i+1] - (reste << (8 - (var_width - size)));
    if((8 - (var_width - size))<= 0){
      ++i;
      size = 8;
    }
    else
      size = 8 - (var_width - size);
  }
  return unpacked_buff;
}



void decompress(char* myfile,char* file_created){
  map<int,string> dict;
  create_dict(dict);
  fstream file(myfile,ios_base::in);
  vector<unsigned char>packed_buff;
  if (file.is_open())
    {
      unsigned char value;
      while(file.read(reinterpret_cast<char *>(&value),sizeof(value))){
        packed_buff.push_back(value);
      }
      vector<int> buffer = unpack(packed_buff);
      vector<int>::const_iterator it = buffer.begin();
      int v = *it;
      ofstream file2;
      file2.open(file_created);
      file2.write((char*)&dict[v][0],sizeof(char));
      string to_write;
      string w;
      w.assign(1,*it);
      int nb = 257;
      ++it;
      while(it != buffer.end()){
        v = *it;
	if(v == 256){
	  clear_dict(dict);
	  nb = 256;
	  it++;
	  w = "";
	  v = *it;
	}
        map<int,string>::iterator f;
        f = dict.find(v);
        if(f != dict.end())
          to_write = f->second;
	else
          to_write= w + w[0];
	file2.write(&to_write[0],to_write.size());
	dict.emplace(nb,w + to_write[0]);
	w = to_write;
	++nb;
	++it;
      }
    }
}

int main(int argc,char* argv[]){
  decompress(argv[1],argv[2]);
}
