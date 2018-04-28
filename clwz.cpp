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
    //cout << "a : " << (int)a << endl;
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
        cout << "*i : " <<hex<<(int)*i << endl;
        c.assign(1,*i);
        cout << "c : " <<c << endl;
        print_str(c,(char*)"c");
        //cout << "c : "<<(short)c[0]<<endl;
        //cout << "w : "<<w<<endl;
        p = w + c;
        print_str(p,(char*)"p");
        map<string,short>::iterator f;
        f = dict.find(p);
        if(f != dict.end()){
          //cout << "dict[p] : " << dict[p] << endl;
          w = p;
          cout << "found w : "<<w<<endl;
        }
        else{
          dict.emplace(p,nb);
          //cout << "dict[w] : " <<dict[w]<< endl;
          for (int i = 0; i < w.size();++i){
            cout << "w["<< i << "]  : " <<w[i]<<endl;
          }
          short val = dict[w];
          cout << "val : "<< val << endl;
          //bitset<16> x(val);
          //cout << "Res : " << x << endl;
          //x = x.to_ulong();
          bitset<16> x(val);
          //cout << "debug : " << x << endl;
          if(val < 256){
            if(val > 64 && val < 127)//cout << "1 a ecrire : " << x << endl;
              cout << "1 a ecrire : " << val << endl;
            else
              cout << "1 a ecrire : " << val << endl;
            file2.write((char*)&val,2);
          }
          else{
            cout << "a ecrire : " << val << endl;
            file2.write((char*)&val,2);
          }
          w = c;
          // if(nb < (2^16))
          ++nb;
          cout << "nb : "<< nb << endl;
        }
        ++i;
      }
    }
  //print_map(dict);
    //print_dict(dict);
}

int main(int argc,char* argv[]){
  compress(argv[1],argv[2]);
  /*short a(224);
  bitset<16>x(a);
  cout << "x : " << x << endl;*/
}
