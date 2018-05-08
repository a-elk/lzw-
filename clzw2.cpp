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

void print_str(string& str,char* name){
  for(int i= 0; i < str.size();++i){
    cout << name <<"[" << i << "] : " <<str[i] << " ";
  }
  cout <<"\n";
}

template<typename T,typename P>
void print_2vec(vector<T>& vec1,vector<P>& vec2){
  cout << "vec1.size() : " << vec1.size() << endl;
  cout << "vec2.size() : " << vec2.size() << endl;
  for(int i =0;(i < vec2.size()) || (i < vec1.size());++i){
    bitset<8>v1(vec1[i]);
    bitset<64>v2(vec2[i]);
    cout << "vec[" << i<<" " <<"] : " << v1 << "     ";
    cout << "vec[" << i <<" " <<"] : " << v2 << endl;
  }
}

void print_map(map<string,short>& mymap){
  map<string,short>::iterator it=mymap.begin();
  while(it != mymap.end()){
    cout << "map["<<it->first<<"] : "<<it->second<<endl;
    ++it;
  }
}

vector<unsigned char> pack(vector<unsigned int>& unpacked_buff){
  unsigned int var_width = 9;
  vector<unsigned char> packed_buff;
  unsigned int reste;
  unsigned int n = var_width;
  int i;
  for(i = 0; i < unpacked_buff.size();++i){
    //cout << "var_width : " << var_width << endl;
    if(i >= pow(2,var_width)-1)
      ++var_width;
    //    cout << " i : " << i << endl;
    bitset<64>e0(unpacked_buff[i]);
    //cout << "n : " << n << endl;
    //cout << "e0 : " << e0 << endl;
    packed_buff.push_back(unpacked_buff[i] >> (n - 8));
    bitset<8>s0(packed_buff.back());
    //cout << "S0 : " << s0 << "  ";
    bitset<64>r0(unpacked_buff[i] -(packed_buff.back()<<(n - 8)));
    //cout << "r0 : " << r0 << endl;
    bitset<64>e1(unpacked_buff[i+1]);
    //cout << "e1 : " << e1<< endl;
    bitset<64>pa(packed_buff.back());
    //    cout << "packed_buff[j] : "<< pa << endl;

    reste = unpacked_buff[i] - (packed_buff.back()<<(n - 8));
    if(/*(n-8) + var_width >= 16*/n >= 16){
      //cout << "TO_TEST" << endl;
      int v = (reste << var_width) | unpacked_buff[i+1];
      bitset<64>vb(v);
      //cout << "v: "<< vb << endl;
      int first_val = v >> (((n-8)-8)+var_width);
      //int first_val = unpacked_buff[i+1] >> (((n-8)+var_width)-8);
      bitset<8>first(first_val);
      //cout << "first_val : " << first << endl;
      packed_buff.push_back(first_val);
      int rnb = ((n-8) + var_width - 16);
      int not_r =  v >> rnb;
      int second_val = not_r -( first_val << 8);
      bitset<8>scnd(second_val);
      //cout << "second_val : " << scnd << endl;
      packed_buff.push_back(second_val);
      bitset<64>e12(unpacked_buff[i+2]);
      //cout << "unpacked_buff[i+2] : " << e12 << endl;
      int r = v - (not_r << rnb);
      bitset<16>re(r);
      //cout << "r : " << re << endl;
      if(i+2 >= pow(2,var_width)-1)
        ++var_width;
      unpacked_buff[i+2] = r << var_width | unpacked_buff[i+2];
      bitset<64>e22(unpacked_buff[i+2]);
      //cout << "unpacked_buff[i+2] : " << e22 << endl;
      n = var_width + rnb;
      ++i;
    }
    else{
      unpacked_buff[i+1] = reste << var_width | unpacked_buff[i+1];
      bitset<64>e12(unpacked_buff[i+1]);
      //cout << "e1 apres : " << e12 << endl;
      n = var_width + (n - 8);
    }
    cout << endl;
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
      print_2vec(packed_buff,compressed_file);
      //cout << "packed_buff.size() : " << packed_buff.size() << endl;
      f.write((char*)&packed_buff[0],packed_buff.size());
    }
}

int main(int argc,char* argv[]){
  compress(argv[1],argv[2]);
}
