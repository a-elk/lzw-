#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <bitset>
#include <typeinfo>
#include <cmath>



using namespace std;
//typedef std::basic_string <unsigned char> ustring;
map<string,int> create_dict(){
  map<string,int> dict;
  int a = 0;
  while(a < 256){
    dict.emplace(string(1,a),a);
    a++;
  }
  return dict;
}

map<string,int> clear_dict(map<string,int>& dict){
  dict.clear();
  map<string,int> dict2 = create_dict();
  return dict2;
}

/*void print_dict(map<string,int>& dict){
  for(map<string,int>::iterator i =dict.begin(); i != dict.end();++i){
    wcout << "dict[" << i->first<<"] : ";
    wcout<< (int)i->second <<endl;
  }
  }*/

vector<unsigned char> pack(vector<int>& unpacked_buff){
  int var_width = 9;
  vector<unsigned char> packed_buff;
  int reste;
  int n = var_width;
  int i,j;
  unsigned char to_push;
  for(int i =0; i < unpacked_buff.size();++i){
    bitset<16>u(unpacked_buff[i]);
    cout << "e["<< dec << i<<"] : " << hex << unpacked_buff[i] <<endl;
    cout << "e["<<dec << i<<"] : "<< u<< endl;
  }
  //cout << "size : " << unpacked_buff.size() << endl;
  //  bitset<16>e0(unpacked_buff[0]);
  //cout << "e[0] : " << e0 << endl;
  int size = 9;
  for(i = 0,j = 256; i < unpacked_buff.size()+1;++i,++j){
    //cout << "j : " << j << endl;
    if(j > (pow(2,var_width)-1)){
      ++var_width;
    }
    if(j > (pow(2,13)-1)){
      var_width = 9;
      j = 256;
    }
    /*cout << "n : " <<n <<endl;
      cout << "var_width" << var_width << endl;*/
    to_push = unpacked_buff[i] >> (n - 8);
    packed_buff.push_back(to_push);
    bitset<8>push(to_push);
    //cout << "to_push : "<<push <<  endl;
    reste = unpacked_buff[i] - (packed_buff.back()<<(n - 8));
    bitset<16>r(reste);
    //cout << "reste : " << r << endl ;
    if(n >= 17){
      //cout << "n >= 17" << endl;
      unpacked_buff[i] = reste;
      //cout << "upacked_buff[i] : "<< r<< endl;
      n -= 8;
      --i;
      --j;
      //cout << "n : " <<n << "i : " << i << "j : " << j << endl;
    }
    else{
      if(j+1 > (pow(2,var_width)-1)){
	//cout <<"var_width ++ " <<endl;
	++var_width;
      }
      if(j+1 > (pow(2,13)-1)){
	//cout << "j+1 > pow(2,13)-1" << endl;
	var_width = 9;
	j = 255;
      }
      bitset<16>un(unpacked_buff[i+1]);
      unpacked_buff[i+1] = reste << var_width | unpacked_buff[i+1];
      //cout << "un : "<<un <<endl;
      n = var_width + (n - 8);
    }
    //cout << endl;
  }
  for(int j = 0; j < packed_buff.size();++j){
    bitset<8>s(packed_buff[j]);
    //cout << "s : "<< s << endl;
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
	c.assign(1,*i);
	p = w + c;
	map<string,int>::iterator f;
	f = dict.find(p);
	if(f != dict.end())
	  w = p;
	else{
	  /*  if(nb > pow(2,13)-2){
	      dict = clear_dict(dict);
	      nb = 256;
	      com pressed_file.push_back(nb);
	      bitset<11>n(nb);
	      ++nb;
	      //cout << "nb : " << nb << endl;
	      }*/
	  dict.emplace(p,nb);
          int val = dict[w];
	  file2.write((char*)&val,sizeof(int));
          w = c;
	  if(i + 1 >= buffer.end()){
	    file2.write((char*)&dict[w],sizeof(int));
	    cout << "break "<<endl;
	    break;
	  }
          ++nb;
        }
        ++i;
      }
      //ofstream f;
      //f.open(file_created);
      //vector<unsigned char> packed_buff = pack(compressed_file);
      /*for(int i = 0; i < packed_buff.size();++i){
	unsigned char val = packed_buff[i];
	f.write((char*)&val,sizeof(unsigned char));
	}*/
      //f.write((char*)&packed_buff[0],packed_buff.size());
      //f.write((char*)&compressed_file[0],compressed_file.size()*sizeof(int));
  
    }
}

int main(int argc,char* argv[]){
  compress(argv[1],argv[2]);
  //cout << sizeof(wchar_t) ;
  /*vector<int> test;
  for(int i = 256; i < pow(2,13)+10 ;++i){
    if (i > pow(2,13)-1){
      test.push_back(256);
      for(int j =257; j < 267;++j)
	test.push_back(j);
      break;
    }
    test.push_back(i);
  }
  
  vector<unsigned char>packed;
  for(int i=0; i < test.size();++i){
    bitset<14>t(test[i]);
    cout << test[i] << "  " << t << endl ;
  }
  packed = pack(test);
  vector<int>unpacked;
  unpacked = unpack(packed);
  for(int i=0; i < unpacked.size();++i){
    bitset<14>un(unpacked[i]);
    cout << unpacked[i] << "  " << un << endl ;
  }
  cout<< endl;
  for(int i=0; i < packed.size();++i){
    bitset<8>p(packed[i]);
    cout << (int)packed[i] << " " <<p << endl;
    }*/
}
