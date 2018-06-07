#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <bitset>
#include <typeinfo>

using namespace std;
map<short,string> create_dict(){
  map<short,string> dict;
  int a = 0;
  while(a < 256){
    dict.emplace(a,string(1,a));
    a++;
  }
  return dict;
}

void decompress(char* myfile,char* file_created){
  map<short,string> dict;
  dict = create_dict();
  fstream file(myfile,ios_base::in);
  //streamsize size = file.tellg();
  //file.seekg(0, ios::beg);
  vector<short> buffer;
  if (file.is_open())
    {
      short value;
      while(file.read(reinterpret_cast<char *>(&value), sizeof(value))){
//        cout <<"value : "<<(char)value << endl;
        buffer.push_back(value);
      }
      vector<short>::const_iterator it = buffer.begin();
      ofstream file2;
      file2.open(file_created);
      short v = *it;
      cout << "ecris: "<< dict[v] << endl;
      file2.write(&dict[v][0], sizeof(char));
      string w;
      w.assign(1,*it);
      int nb(256);
      for(unsigned int i = 2; i <buffer.size();++i){
        it++;
        v = *it;
        cout << "v : "<< v << endl;
        //cout << "dict[v] : "<< dict[v] << endl;
        //cout << "w : "<< w << endl;
        map<short,string>::iterator f;
        f = dict.find(v);
        string to_write;
        if(f != dict.end()){
          cout << "f->first: " << f->first << endl;
          cout << "f->second: " << hex << f->second << endl;
   //       cout << "dict[v]: " << dict[v] << endl;
          to_write = f->second;
          //cout << "if to_write : " << &to_write[0] << endl;
        }
        else{
          cout << "not_in dict" << endl;
          to_write = w + w[0];
          cout << "w : " << w <<"w[0] : "<<w[0]<< endl;
        }
        if(to_write.length() == 1){
          cout << "to_write.length(): "<<to_write.length()<<endl;
          //cout << "brut :  " << to_write << endl;
          //cout << "1 ecris: "<< hex << (int)to_write[0] << endl;
          cout << "1 ecris: "<< (char)to_write[0] << endl;
          file2.write(&to_write[0],to_write.length());
        }
        else{
          cout << "to_write.length(): "<<to_write.length()<<endl;
          for(unsigned int i = 0; i < to_write.length();++i){
            //cout << "to_write["<<i<<"] : "<<hex<<(int)to_write[i]<<" ";
            cout << "to_write["<<i<<"] : "<<to_write[i]<<" ";
            cout << endl;
          }
          //cout << "ecris: "<< hex << to_write << endl;
          cout << endl;
          file2.write(&to_write[0],to_write.length());
        }
        dict.emplace(nb,w + to_write[0]);
        w = to_write;
        ++nb;
        }
      }
}

int main(int argc,char* argv[]){
  decompress(argv[1],argv[2]);
}


