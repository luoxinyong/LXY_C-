#include "data_stream.h"
#include <iostream>
using namespace std;



int main()
{
    data_stream ds;
    class A{
        public:
        A(int a, double b, bool c , string d):A_age(a),A_garde(b),A_gender(c),A_name(d){}
        ~A(){}
    private:
        int A_age;
        double A_garde;
        bool A_gender;
        string A_name;
        SERIALIZE(A_age,A_garde,A_gender,A_name);
    };
    return 0;

    A boo(2,123.5,true,"kitty");
    ds<<boo;
}