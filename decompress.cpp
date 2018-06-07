#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cmath>
#include <bitset>


using namespace std;

//typedef std::basic_string <unsigned char> ustring;
map<int,string> create_dict(){
  map<int,string> dict;
  int a = 0;
  while(a < 256){
    dict.emplace(a,string(1,a));
    ++a;
  }
  return dict;
}

map<int,string> clear_dict(map<int,string>& dict){
  dict.clear();
  map<int,string> dict2 = create_dict();
  return dict2;
}

void print_dict(map<int,string>& dict){
  for(map<int,string>::iterator i =dict.begin(); i != dict.end();++i){
    ;
  }
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
  /*  for(int i = 0; i < packed_buff.size();++i){
    bitset<8>s(packed_buff[i]);
    cout << "p["<<i<<"] : " << s<<endl;
    }*/
  for(i = 0, j = 256; i < upacked2.size()-1;++i){
    //cout << "i : " << dec << i << endl;
    //cout << "var_width : " <<dec <<  var_width << endl;
    /*if(i == 24128){
      
      cout << "size : " << size << endl;
      for(int i = 24128; i < 24138;++i){
	bitset<20>test(upacked2[i]);
	cout <<"up["<<i<<"] : " << test <<endl;
	cout <<"up["<<i<<"] : " << upacked2[i] <<endl;
	cout << endl;
      }
      break;
      }*/
    /*cout << "j : " << dec << j << endl;
    bitset<20>p(upacked2[i]);
    cout << "pack[i] : " << p << endl;
    bitset<20>un(upacked2[i+1]);
    cout << "p[i+1] : " << un << endl;
    bitset<20>deux(upacked2[i+2]);
    cout << "p[i+2] : " << deux << endl;
    bitset<20>trois(upacked2[i+3]);
    cout << "p[i+3] : " << trois << endl;*/
    /*if(in == var_width){
      --j;
      in = 0;
      cout << "in == var_width" << endl;
      }*/

    if(j > (pow(2,13)-1)){
      var_width = 9;
      //cout << "var_width : 9 " << endl;
      j = 256;
      //cout << "256 " << endl;
    }
    if(j > (pow(2,var_width)-1)){
      //cout << "var_width ++ " << endl;
      ++var_width;
    }
    //cout << "size : " << size << endl;
    /*if(size <= 0){
      ++i;
      ++j;
      size = 8;
      }*/

    //cout << "size : " << size << endl;
    if((8 - (var_width -size))< 0){
      upacked2[i+1] = upacked2[i] << 8 | upacked2[i+1];
      bitset<16>p(upacked2[i+1]);
      //cout << "pack[i+1] : " << p << endl;
      size = 8 + size;
      //cout << "size : " << size << endl;
      ++i;
      bitset<16>un(upacked2[i]);
      //cout << "p[i] : " << un << endl;
    }
    reste = upacked2[i+1] >> (8 - (var_width - size));
    bitset<16>r(reste);
    //cout << "reste : " << r << endl;
    to_push = upacked2[i] << (var_width - size) | reste;
    bitset<16>t(to_push);
    //cout << "to_push : " <<t<< endl;
    //cout << "to_push : " <<hex<< to_push<< endl;
    unpacked_buff.push_back(to_push);
    ++j;
    upacked2[i+1] = upacked2[i+1] - (reste << (8 - (var_width - size)));
    bitset<16>unp(upacked2[i+1]);
    //cout << "np1 : " << unp << endl;
    if((8 - (var_width - size))<= 0){
      ++i;
      size = 8;
    }
    else
      size = 8 - (var_width - size);
    //cout << endl;
  }

  //  cout << "e[0] : " << hex << unpacked_buff[0] <<endl;
  for(int i = 0; i < unpacked_buff.size();++i){
    bitset<16>u(unpacked_buff[i]);
    cout << "e["<< dec << i<<"] : " << hex << unpacked_buff[i] <<endl;
    cout << "e["<<dec << i<<"] : "<< u<< endl;
  }
  return unpacked_buff;
}



void decompress(char* myfile,char* file_created){
  map<int,string> dict;
  dict = create_dict();
  //print_dict(dict);
  fstream file(myfile,ios_base::in);
  //vector<string> uncompressed_file;
  vector<int> buffer;
  if (file.is_open())
    {
      int value;
      while(file.read(reinterpret_cast<char *>(&value),sizeof(value))){
        buffer.push_back(value);
      }
      //vector<int> unpacked_file = unpack(packed_file);
      //vector<int>::const_iterator it = unpacked_file.begin();
      vector<int>::const_iterator it = buffer.begin();
      int v = *it;
      ofstream file2;
      file2.open(file_created);
      file2.write((char*)&dict[v][0],sizeof(char));
      string w;
      w.assign(1,*it);
      int nb(257);
      ++it;
      while(it != buffer.end()){
        v = *it;
	/*if(v == 256){
	  dict = clear_dict(dict);
	  nb == 257;
	  ++it;
	  v = *it;
	  }*/
        map<int,string>::iterator f;
        f = dict.find(v);
        string to_write;
        if(f != dict.end())
          to_write = f->second;
	else
          to_write= w + w[0];
	file2.write(&to_write[0],to_write.size());
	dict.emplace(nb,w + to_write[0]);
	w = to_write;
	++nb;
	//cout << endl;
	++it;
      }
    }
}

      
  //ofstream file2;
  /*  file2.open(file_created);
  file2.write((char*)&uncompressed_file[0],uncompressed_file.size());*/
//}

int main(int argc,char* argv[]){
  decompress(argv[1],argv[2]);
  /*ofstream f;
  f.open("debug");
  unsigned char k = 233;
  f.write((char*)&k,1);*/
}
