#include <iostream>
#include <fstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(){
    fstream s; 
    int a{5}, b{0};
    

    s.open("../sample");

    s.write((const char*)(&a), sizeof(int));
    s.seekg(0);
    s.read((char*)(&b), sizeof(int));

    cout << b << endl;
    return 0;
}
