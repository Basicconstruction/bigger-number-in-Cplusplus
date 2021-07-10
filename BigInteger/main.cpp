#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>
using namespace std;
template <typename T>
string convertNumToString(T x) {
    // transfer int number to string
    x = abs(x);
    string s;
    x *= 10;
    while (x >= 10) {
        x /= 10;
        s = static_cast<char>(x % 10 + 48) + s;
    }
    return s;
}
int convertCharToInt(const char c){
    assert(c>=48&&c<=57);
    return static_cast<int>(c) - 48;
}
char convertIntToChar(int c){
    return static_cast<char>(c+48);
}
bool convertStringToBool(string && bo){
    if(bo=="0"){
        return false;
    }else{
        return true;
    }
}
size_t nonZero(string && getStr){
    string temp = string(getStr);
    for(size_t i = 0; i < temp.length();++i){
        if(temp[i]!='0'){
            return i;
        }
    }
    return temp.length();
}
class BigInteger{
public:
    size_t totalLength;
    size_t effectiveLength;
    bool over0;
//    long long int Value;
    string value;
    string& v = value;
    string deploy;
    explicit BigInteger(int initNum){
        if(initNum>0){
            over0 = true;
        }else{
            over0 = false;
        }
        this->value = convertNumToString(initNum);
        this->totalLength = this->value.length();
        this->effectiveLength = this->totalLength;
        this->deploy = this->getDeploy();
    }
    BigInteger(const string && initWithString,string simplify){
        string initHelper = initWithString;
        if(initHelper[0]=='-'){
            this->over0 = false;
            this->value = string(initHelper.begin()+1,initHelper.end());
            this->totalLength = this->value.length();
            this->effectiveLength = this->totalLength;
            this->deploy = this->getDeploy();
        }else if(initHelper[0]=='0'){
            size_t nonZeroBit;
            nonZeroBit = nonZero(string(initHelper));
            this->over0 = true;
            if(simplify=="true"){
                this->value = string(initHelper.begin()+nonZeroBit,initHelper.end());
                this->totalLength = this->value.length();
                this->effectiveLength = this->totalLength;
                this->deploy = this->getDeploy();
            }else{
                this->value = initHelper;
                this->totalLength = initHelper.length();
                this->effectiveLength = initHelper.length() - nonZeroBit - 1;
                this->deploy = this->getDeploy();
            }

        }else if(initHelper[0]=='+'){
            this->over0 = true;
            this->value = string(initHelper.begin()+1,initHelper.end());
            this->totalLength = this->value.length();
            this->effectiveLength = this->totalLength;
            this->deploy = this->getDeploy();
        }else{
            this->over0 = true;
            this->value = string(initHelper);
            this->totalLength = this->value.length();
            this->effectiveLength = this->totalLength;
            this->deploy = this->getDeploy();
        }

    }
    BigInteger(const string&& initWithDeploy,bool signWithDeploy){
        vector<string> x = getOrigin(string(initWithDeploy));
        this->over0 = convertStringToBool(string(x[0]));
        this->value = x[1];
        this->totalLength = this->value.length();
        this->effectiveLength = this->totalLength;
        this->deploy = string(initWithDeploy);
    }
    explicit BigInteger(const BigInteger &bi){
        this->over0 = bi.over0;
        this->value = bi.value;
        this->totalLength = bi.totalLength;
        this->effectiveLength = bi.effectiveLength;
        this->deploy = bi.deploy;
    }
    BigInteger(const BigInteger &bi,int && newTotalLength){
        assert(newTotalLength>bi.totalLength);
        this->value = bi.value;
        this->totalLength = newTotalLength;
        this->effectiveLength = bi.effectiveLength;
        this->deploy = bi.deploy;
    }
    string getDeploy(){
        if(over0){
            if(this->effectiveLength<this->totalLength){
                return this->v;
            }else{
                return "0"+this->v;
            }
        }
        string deployTemp;
        int p;
        if(v[totalLength-1]!='0'){
            deployTemp = convertIntToChar(10-convertCharToInt(v[totalLength-1]));
        }else{
            deployTemp = "0";
        }
        for(int iter = 2;iter <= effectiveLength;++iter){
            p = totalLength - iter;
            deployTemp = convertIntToChar(9-convertCharToInt(v[p]))+deployTemp;
        }
        return '9'+deployTemp;
    }
    string getDeploy(string && valueTemp,bool over0Temp){
        //计算“10补数”
        if(over0Temp){
            return "0"+valueTemp;
        }
        int length = valueTemp.length();
        string deployTemp;
        int p;
        if(v[length-1]!='0'){
            deployTemp = convertIntToChar(10-convertCharToInt(valueTemp[length-1]));
        }else{
            deployTemp = "0";
        }
        for(int iter = 2;iter <= length;++iter){
            p = length - iter;
            deployTemp = convertIntToChar(9-convertCharToInt(valueTemp[p]))+deployTemp;
        }
        return '9'+deployTemp;
    }
    vector<string> getOrigin(string&& deployed){
        //将计算结果转化为分开的包含符号位和数字位的组合
        vector<string> x(2);
        if(deployed[0]=='0'){
            x[0] = "1";
            x[1] = string(deployed.begin()+1,deployed.end());
        }else{
            assert(deployed[0]=='9');
            x[0] = "0";//false
            string originTemp = getDeploy(string(deployed.begin()+1,deployed.end()),false);
            x[1] = string(originTemp.begin()+1,originTemp.end());
        }
        return x;
    }
    BigInteger add(const BigInteger& b1){

    }
    BigInteger add(const BigInteger&b1, const BigInteger& b2){

    }
};
int main() {
//    BigInteger x(100);
//    cout<<x.v<<" "<<x.value<<" "<<x.totalLength<<" "<<x.effectiveLength<<endl;
//    cout<<x.getDeploy()<<endl;
//    BigInteger y(-100);
//    cout<<y.deploy<<" "<<y.over0<<endl;
//    vector<string> h = y.getOrigin("912189");
//    cout<<h[0]<<" "<<h[1]<<endl;
    BigInteger c("00900089100","false");
    cout<<c.v<<" "<<c.deploy<<" "<<c.v.length()<<" "<<c.totalLength<<" "<<c.over0<<" "<<c.effectiveLength;
    return 0;
}
