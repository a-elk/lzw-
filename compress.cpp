#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <bitset>
#include <typeinfo>
#include <cmath>

using namespace std;

void create_dict(map<string,int>& dict){
  int a = 0;
  while(a < 256){
    dict.emplace(string(1,a),a);
    a++;
  }
}

void clear_dict(map<string,int>& dict){
  dict.clear();
  create_dict(dict);
}

vector<unsigned char> pack(vector<int>& unpacked_buff){
  int var_width = 9;
  vector<unsigned char> packed_buff;
  int reste;
  int n = var_width;
  int i,j;
  unsigned char to_push;
  int size = 9;
  for(i = 0,j = 256; i < unpacked_buff.size()+1;++i,++j){
    if(j > (pow(2,var_width)-1)){
      ++var_width;
    }
    if(j > (pow(2,13)-2)){
      var_width = 9;
      j = 256;
    }
    to_push = unpacked_buff[i] >> (n - 8);
    packed_buff.push_back(to_push);
    reste = unpacked_buff[i] - (packed_buff.back()<<(n - 8));
    if(n >= 17){
      unpacked_buff[i] = reste;
      n -= 8;
      if(i+1 >= unpacked_buff.size()+1){
	break;
      }
      --i;
      --j;
    }
    else{
      if(j+1 > (pow(2,var_width)-1))
	++var_width;
      if(j+1 > (pow(2,13)-2)){
	var_width = 9;
	j = 255;
      }
      unpacked_buff[i+1] = reste << var_width | unpacked_buff[i+1];
      n = var_width + (n - 8);
      
    }
  }
  return packed_buff;
}



void compress(char* myfile,char* file_created){
  map<string,int> dict;
  create_dict(dict);
  ifstream file(myfile, ios::binary | ::ios::ate);
  streamsize size = file.tellg();
  file.seekg(0, ios::beg);
  vector<unsigned char> buffer(size);
  vector<int>compressed_file;
  if (file.read((char*)buffer.data(), size))
    {
      ofstream file2;
      file2.open(file_created);
      string w = "";
      vector<unsigned char>::const_iterator i = buffer.begin();
      int nb(257);
      string c;
      string p;
      while(i != buffer.end()){
	if(nb > pow(2,13)-2){
	  clear_dict(dict);
	  compressed_file.push_back(256);
	  nb = 257;
	}
	c.assign(1,*i);
	p = w + c;
	map<string,int>::iterator f;
	f = dict.find(p);
	if(f != dict.end())
	  w = p;
	else{
	  dict.emplace(p,nb);
          int val = dict[w];
	  compressed_file.push_back(val);
          w = c;
          ++nb;
        }
	if(i + 1 == buffer.end()){
	  if(nb > pow(2,13)-2)
	    compressed_file.push_back(256);
	  compressed_file.push_back(dict[w]);
	  break;
	}
        ++i;
      }
      vector<unsigned char> packed_buff = pack(compressed_file);
      file2.write((char*)&packed_buff[0],packed_buff.size());
    }
  else
    cout << "Ficher non ouvert" << endl;
}

int main(int argc,char* argv[]){
  compress(argv[1],argv[2]);
}
