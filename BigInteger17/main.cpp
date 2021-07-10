#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>
using namespace std;
using namespace std::string_literals;
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
size_t convertCharToInt(const char c){
    assert(c>=48&&c<=57);
    return static_cast<size_t>(c) - 48;
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
    BigInteger(const string initWithString,string simplify){
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
    BigInteger(const string initWithDeploy){
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
    BigInteger(const BigInteger &bi,int newTotalLength){
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
        int length = this->totalLength;
        string deployTemp;
        size_t p;
        size_t push = 0;
        size_t endBit = 10-convertCharToInt(string(this->v)[length-1]);
        if(endBit==10){
            deployTemp = '0';
            push = 1;
        }else{
            deployTemp = convertIntToChar(endBit);
        }
        for(int iter = 2;iter <= length;++iter){
            p = length - iter;
            endBit = 9-convertCharToInt(string(this->v)[p])+push;
            if(endBit==10){
                deployTemp = '0'+deployTemp;
                push = 1;
            }else{
                deployTemp = convertIntToChar(endBit)+deployTemp;
                push = 0;
            }

        }
        return '9'+deployTemp;
    }
    string getDeploy(string valueTemp,bool over0Temp){
        //计算“10补数”
        if(over0Temp){
            return "0"+valueTemp;
        }
        int length = valueTemp.length();
        string deployTemp;
        size_t p;
        size_t push = 0;
        size_t endBit = 10-convertCharToInt(valueTemp[length-1]);
        if(endBit==10){
            deployTemp = '0';
            push = 1;
        }else{
            deployTemp = convertIntToChar(endBit);
        }
        for(int iter = 2;iter <= length;++iter){
            p = length - iter;
            endBit = 9-convertCharToInt(valueTemp[p])+push;
            if(endBit==10){
                deployTemp = '0'+deployTemp;
                push = 1;
            }else{
                deployTemp = convertIntToChar(endBit)+deployTemp;
                push = 0;
            }

        }
        return '9'+deployTemp;
    }
    vector<string> getOrigin(string deployed){
        //将计算结果转化为分开的包含符号位和数字位的组合
        vector<string> x(2);
        size_t getBack = 1;
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
    string add(const BigInteger&b1, const BigInteger& b2){
        size_t l1 = b1.effectiveLength;
        size_t l2 = b2.effectiveLength;
        string effects1,effects2;
        effects1 = b1.deploy;
        effects2 = b2.deploy;
        effects1 = string(effects1.begin()+(totalLength-effectiveLength+1),effects1.end());
        effects2 = string(effects2.begin()+(totalLength-effectiveLength+1),effects2.end());
        if(l1>=l2){
            if(b2.over0){
                effects2 = (string(b2.deploy))[0]+string(l1-l2,'0')+effects2;
            }else{
                effects2 = (string(b2.deploy))[0]+string(l1-l2,'9')+effects2;
            }
            effects1 = (string(b1.deploy))[0]+effects1;
        }else{
            if(b1.over0){
                effects1 = (string(b1.deploy))[0]+string(l2-l1,'0')+effects1;
            }else{
                effects1 = (string(b1.deploy))[0]+string(l2-l1,'9')+effects1;
            }
            effects2 = (string(b2.deploy))[0]+effects2;
        }
        size_t push = 0;
        size_t temp;
        size_t n1;
        size_t n2;
        string deployed = "";
        size_t ll = effects1.length();
        assert(ll==effects1.length());
        for(size_t j = 0;j < ll;j++){
            n1 = convertCharToInt(effects1[ll-j-1]);
            n2 = convertCharToInt(effects2[ll-j-1]);
            temp = n1 + n2 + push;
            push = 0;
            if(temp>=10){
                push = 1;
            }
            temp = temp % 10;
            deployed = convertIntToChar(temp)+deployed;
        }
        if(b1.over0&&b2.over0&&deployed[0]=='1'){
            deployed = '0'+deployed;
        }
        vector<string> x = getOrigin(deployed);
        return convertStringToBool(string(x[0]))?x[1]:'-'+x[1];
        //ignore push

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
    BigInteger c("90900089100");
    cout<<c.v<<" "<<c.deploy<<" "<<c.v.length()<<" "<<c.totalLength<<" "<<c.over0<<" "<<c.effectiveLength<<endl;
    BigInteger a(-190);
    cout<<a.deploy<<" "<<a.effectiveLength<<endl;
    BigInteger b(100);
    cout<<b.deploy<<" "<<b.effectiveLength<<endl;
    cout<<a.getOrigin(a.getDeploy())[1]<<endl;
    BigInteger p("378127","true");
    cout<<a.add(a,b);
    return 0;
}
