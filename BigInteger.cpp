#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>
using namespace std;
using namespace std::string_literals;
bool same(const bool a, const bool b) {
    if (a && b) {
        return true;
    }
    else if ((!a) && (!b)) {
        return true;
    }
    else {
        return false;
    }
}
size_t convertCharToInt(const char c) {
    assert(c >= 48 && c <= 57);
    //cout << c << " push " << endl;
    return static_cast<size_t>(c) - 48;
}
char convertIntToChar(size_t c) {
    return static_cast<char>(c + 48);
}
long long int convertStringToLong(string s) {
    long long int x = 0;
    for (size_t i = 0; i < s.length(); i++) {
        x *= 10;
        x += convertCharToInt(s[i]);
    }
    return x;
}
template <typename T>
string convertNumToString(T x) {
    // transfer int number to string
    if (x == 0) {
        return "0";
    }
    string s;
    x *= 10;
    while (x >= 10) {
        x /= 10;
        s = static_cast<char>(x % 10 + 48) + s;
    }
    return s;
}
bool convertStringToBool(string bo) {
    if (bo == "0") {
        return false;
    }
    else {
        return true;
    }
}
string convertBoolToString(bool bo) {
    if (bo) {
        return "true";
    }
    else {
        return "false";
    }
}
size_t nonZero(string getStr) {
    string temp = string(getStr);
    for (size_t i = 0; i < temp.length(); ++i) {
        if (temp[i] != '0') {
            return i;
        }
    }
    //获取整数字符串中第一个非零字符的位置，用于简化字符串存储
    //比如传入 0000123 那么该函数将会返回4，后续函数可以利用该数字来处理简化
    return temp.length();
}
string simplifyBigIntegerValue(string BigIntegerValue) {
    string v = BigIntegerValue;
    //用于简化BigInteger.add(BigInteger&,BigInteger&)的输出
    //比如将-00001,简化为-1
    //将00001简化为1
    if (v[0] == '-' || v[0] == '+') {
        size_t bn0 = nonZero(string(v.begin() + 1, v.end()));
        if (bn0 + 1 == v.length()) {
            return "0";
        }
        return '-' + string(v.begin() + bn0 + 1, v.end());
    }
    else {
        size_t bn0 = nonZero(v);
        if (bn0 == v.length()) {
            return "0";
        }
        return string(v.begin() + bn0, v.end());
    }
}
class BigInteger {
public:
    size_t totalLength;
    size_t effectiveLength;
    bool over0;
    //    long long int Value;
    string value;
    string& v = value;
    string deploy;
    explicit BigInteger(int initNum) {
        if (initNum >= 0) {
            over0 = true;
        }
        else {
            initNum = -initNum;
            over0 = false;
        }
        this->value = convertNumToString(initNum);
        this->totalLength = this->value.length();
        this->effectiveLength = this->totalLength;
        this->deploy = this->getDeploy();
    }
    BigInteger(const string initWithString) {
        string initHelper = initWithString;
        size_t posFix = 0;
        if (initWithString == "-0") {
            this->value = "0";
            this->over0 = true;
            this->deploy = this->getDeploy();
            this->totalLength = 1;
            this->effectiveLength = 1;
            return;
        }
        if (initHelper[0] == '-') {
            //-12344
            //no -000989
            this->over0 = false;
            posFix = 1;
        }
        else if (initHelper[0] == '0') {
            //08990
            //0000798
            //0000
            size_t nonZeroBit;
            nonZeroBit = nonZero(string(initHelper));
            this->over0 = true;
            if (nonZeroBit == initHelper.length()) {
                this->value = "0";
            }
            else {
                this->value = string(initHelper.begin() + nonZeroBit, initHelper.end());
            }
            this->totalLength = this->value.length();
            this->effectiveLength = this->totalLength;
            this->deploy = this->getDeploy();
            return;

        }
        else if (initHelper[0] == '+') {
            //+9009
            //no +000789
            this->over0 = true;
            posFix = 1;
        }
        else {
            //989080
            this->over0 = true;
        }
        this->value = string(initHelper.begin() + posFix, initHelper.end());
        this->totalLength = this->value.length();
        this->effectiveLength = this->totalLength;
        this->deploy = this->getDeploy();
    }
    BigInteger(const string initWithDeploy, bool simplifyDeploy) {
        vector<string> x = getOrigin(string(initWithDeploy));
        this->over0 = convertStringToBool(string(x[0]));
        this->value = x[1];
        this->totalLength = this->value.length();
        this->effectiveLength = this->totalLength;
        if (simplifyDeploy) {
            if (this->value == "0") {
                this->over0 = true;
            }
            this->deploy = getDeploy();
        }
        else {
            this->deploy = string(initWithDeploy);
        }
    }
    BigInteger(const BigInteger& bi) {
        //拷贝构造函数，不应再函数前面添加explicit，即使它只有一个参数
        //似乎可以看作构造函数，即从BigInteger构造BigInteger
        this->over0 = bi.over0;
        this->value = bi.value;
        this->totalLength = bi.totalLength;
        this->effectiveLength = bi.effectiveLength;
        this->deploy = bi.deploy;
    }
    ~BigInteger() {}
    string getDeploy() {
        if (this->over0) {
            if (this->effectiveLength < this->totalLength) {
                return this->v;
            }
            else {
                return "0" + this->v;
            }
        }
        int length = this->totalLength;
        string deployTemp;
        size_t p;
        size_t push = 0;
        size_t endBit = 10 - convertCharToInt(string(this->v)[length - 1]);
        if (endBit == 10) {
            deployTemp = '0';
            push = 1;
        }
        else {
            deployTemp = convertIntToChar(endBit);
        }
        for (int iter = 2; iter <= length; ++iter) {
            p = length - iter;
            endBit = 9 - convertCharToInt(string(this->v)[p]) + push;
            if (endBit == 10) {
                deployTemp = '0' + deployTemp;
                push = 1;
            }
            else {
                deployTemp = convertIntToChar(endBit) + deployTemp;
                push = 0;
            }

        }
        return '9' + deployTemp;
    }
    string getDeploy(string valueTemp, bool over0Temp) {
        //计算“10补数”
        if (over0Temp) {
            return "0" + valueTemp;
        }
        int length = valueTemp.length();
        string deployTemp;
        size_t p;
        size_t push = 0;
        size_t endBit = 10 - convertCharToInt(valueTemp[length - 1]);
        if (endBit == 10) {
            deployTemp = '0';
            push = 1;
        }
        else {
            deployTemp = convertIntToChar(endBit);
        }
        for (int iter = 2; iter <= length; ++iter) {
            p = length - iter;
            endBit = 9 - convertCharToInt(valueTemp[p]) + push;
            if (endBit == 10) {
                if (iter == length) {
                    //                    deploy =
                }
                deployTemp = '0' + deployTemp;
                push = 1;
            }
            else {
                deployTemp = convertIntToChar(endBit) + deployTemp;
                push = 0;
            }

        }
        return '9' + deployTemp;
    }
    vector<string> getOrigin(string deployed) {
        //将计算结果转化为分开的包含符号位和数字位的组合
        vector<string> x(2);
        size_t getBack = 1;
        if (deployed[0] == '0') {
            x[0] = "1";
            const string t = string(deployed.begin() + 1, deployed.end());
            x[1] = simplifyBigIntegerValue(t);
        }
        else {
            assert(deployed[0] == '9');
            //            cout<<"&&&!!!"<<deployed<<" "<<deployed[0]<<endl;
            x[0] = "0";//false
            string originTemp = getDeploy(string(deployed.begin() + 1, deployed.end()), false);
            const string t = string(originTemp.begin() + 1, originTemp.end());
            x[1] = simplifyBigIntegerValue(t);
        }
        return x;
    }
    void toString() {
        //用于对齐
        if (this->over0) {
            printf("symbol:BigInteger\nover0: %s\nvalue:  %s \ndeploy: %s\ntotalLength: %d \neffectLength: %d\n", convertBoolToString(this->over0).c_str(), this->getValue().c_str(), this->deploy.c_str(), this->totalLength, this->effectiveLength);
        }
        else {
            printf("symbol:BigInteger\nover0: %s\nvalue: %s \ndeploy: %s\ntotalLength: %d \neffectLength: %d\n", convertBoolToString(this->over0).c_str(), this->getValue().c_str(), this->deploy.c_str(), this->totalLength, this->effectiveLength);
        }
    }
    void getDetails() {
        this->toString();
    }
    BigInteger add(const BigInteger& b2) {
        const BigInteger b1(this->deploy, true);
        size_t l1 = b1.effectiveLength;
        size_t l2 = b2.effectiveLength;
        string effects1, effects2;
        effects1 = b1.deploy;
        effects2 = b2.deploy;
        effects1 = string(effects1.begin() + (totalLength - effectiveLength + 1), effects1.end());
        effects2 = string(effects2.begin() + (totalLength - effectiveLength + 1), effects2.end());
        if (l1 >= l2) {
            if (b2.over0) {
                effects2 = (string(b2.deploy))[0] + string(l1 - l2, '0') + effects2;
            }
            else {
                effects2 = (string(b2.deploy))[0] + string(l1 - l2, '9') + effects2;
            }
            effects1 = (string(b1.deploy))[0] + effects1;
        }
        else {
            if (b1.over0) {
                effects1 = (string(b1.deploy))[0] + string(l2 - l1, '0') + effects1;
            }
            else {
                effects1 = (string(b1.deploy))[0] + string(l2 - l1, '9') + effects1;
            }
            effects2 = (string(b2.deploy))[0] + effects2;
        }
        size_t push = 0;
        size_t temp;
        size_t n1;
        size_t n2;
        string deployed = "";
        size_t ll = effects1.length();
        assert(ll == effects1.length());
        for (size_t j = 0; j < ll; j++) {
            n1 = convertCharToInt(effects1[ll - j - 1]);
            n2 = convertCharToInt(effects2[ll - j - 1]);
            temp = n1 + n2 + push;
            push = 0;
            if (temp >= 10) {
                push = 1;
            }
            temp = temp % 10;
            deployed = convertIntToChar(temp) + deployed;
        }
        if (b1.over0 && b2.over0 && deployed[0] == '1') {
            deployed = '0' + deployed;
        }
        vector<string> x = getOrigin(deployed);
        return BigInteger(simplifyBigIntegerValue(convertStringToBool(string(x[0])) ? x[1] : '-' + x[1]));
    }
    BigInteger add(string b2) {
        BigInteger b1 = BigInteger(this->deploy);
        BigInteger bx = BigInteger(b2, true);
        return b1.add(bx);
    }
    BigInteger add(long long int b2) {
        BigInteger b1 = BigInteger(this->deploy);
        BigInteger bx = BigInteger(b2);
        return b1.add(bx);
    }
    BigInteger operator+() const {
        return *this;
    }
    BigInteger operator-() const {
        return BigInteger(this->over0 ? '-' + this->v : this->v);
    }
    BigInteger& operator = (const BigInteger& b2) {
        this->over0 = b2.over0;
        this->deploy = b2.deploy;
        this->value = b2.value;
        this->totalLength = b2.totalLength;
        this->effectiveLength = b2.effectiveLength;
        return *this;
    }
    BigInteger sub(const BigInteger& b2) {
        return this->add(-b2);
    }
    BigInteger sub(const string b2) {
        return this->add(-BigInteger(b2));
    }
    BigInteger sub(long long int b2) {
        return this->add(-BigInteger(b2));
    }
    BigInteger operator+(const BigInteger& b2) {
        return this->add(b2);
    }
    BigInteger operator+(const string b2) {
        return this->add(BigInteger(b2));
    }
    BigInteger operator+(const long long int b2) {
        return this->add(BigInteger(b2));
    }
    //pre
    BigInteger& operator++() {
        (*this) += 1;
        return *this;
    }
    //post
    BigInteger operator++(int dummy) {
        BigInteger x = BigInteger(*this);
        (*this) += 1;
        return x;
    }
    //pre
    BigInteger& operator--() {
        (*this) -= 1;
        return *this;
    }
    //post
    BigInteger operator--(int dummy) {
        BigInteger x = BigInteger(*this);
        (*this) -= 1;
        return x;
    }
    BigInteger operator-(const BigInteger& b2) {
        return this->sub(b2);
    }
    BigInteger operator-(const string b2) {
        return this->sub(b2);
    }
    BigInteger operator-(const long long int b2) {
        return this->sub(b2);
    }
    char operator[](size_t index) const {
        assert(index + 1 <= this->v.length());
        return this->v[index];
    }


    BigInteger& operator+=(const BigInteger& b2) {
        BigInteger t = this->add(b2);
        this->value = t.value;
        this->over0 = t.over0;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }
    BigInteger& operator+=(const string b2) {
        BigInteger t = this->add(b2);
        this->value = t.value;
        this->over0 = t.over0;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }
    BigInteger& operator+=(const long long int b2) {
        BigInteger t = this->add(b2);
        this->value = t.value;
        this->over0 = t.over0;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }
    BigInteger& operator-=(const BigInteger& b2) {
        BigInteger t = this->sub(b2);
        this->value = t.value;
        this->over0 = t.over0;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }
    BigInteger& operator-=(const string b2) {
        BigInteger t = this->sub(b2);
        this->value = t.value;
        this->over0 = t.over0;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }
    BigInteger& operator-=(const long long int b2) {
        BigInteger t = this->sub(b2);
        this->value = t.value;
        this->over0 = t.over0;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }

    int compareTo(const BigInteger& b2) {
        //注意要比较的是有效值长度
        int y = 2;
        if (this->over0 && (!b2.over0)) {
            return 1;
        }
        if ((!this->over0) && b2.over0) {
            return -1;
        }
        if (this->v.length() > b2.v.length()) {
            y = 3;
        }
        else if (this->v.length() < b2.v.length()) {
            y = 1;
        }
        else {
            for (unsigned int c = 0; c < this->v.length(); c++) {
                if (this->v[c] > b2.v[c]) {
                    y = 3;
                    break;
                }
                else if (this->v[c] < b2.v[c]) {
                    y = 1;
                    break;
                }
                else {
                    continue;
                }
            }
        }
        if (y == 2) {
            return 0;
        }
        if (this->over0) {
            if (y == 3) {
                return 1;
            }
            else {
                return -1;
            }
        }
        else {
            if (y == 3) {
                return -1;
            }
            else {
                return 1;
            }
        }
    }
    int compareTo(const string b2) {
        const BigInteger t(b2);
        return this->compareTo(t);
    }
    int compareTo(const long long int b2) {
        const BigInteger t(b2);
        return this->compareTo(t);
    }
    //-- ++ equals()
    bool equals(const BigInteger& b2) {
        if (this->compareTo(b2) == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool equals(const string b2) {
        if (this->compareTo(b2) == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool equals(const long long int b2) {
        if (this->compareTo(b2) == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool operator < (const BigInteger& b) {
        return this->compareTo(b) < 0;
    }
    bool operator > (const BigInteger& b) {
        return this->compareTo(b) > 0;
    }
    bool operator <= (const BigInteger& b) {
        return this->compareTo(b) <= 0;
    }
    bool operator >= (const BigInteger& b) {
        return this->compareTo(b) >= 0;
    }
    bool operator == (const BigInteger& b) {
        return this->equals(b);
    }
    bool operator != (const BigInteger& b) {
        return !this->equals(b);
    }
    BigInteger& operator<<(const long long int x) {
        assert(x >= 0);
        BigInteger t(this->over0 ? this->v + string(x, '0') : '-' + this->v + string(x, '0'));
        this->value = t.v;
        this->deploy = t.deploy;
        this->effectiveLength = t.effectiveLength;
        this->totalLength = t.totalLength;
        return *this;
    }
    BigInteger operator<<(string y) {
        //这个并非是为冗余重载设计，而是用于提供一个不改变原对象（返回新对象）的方法
        long long int x = convertStringToLong(y);
        assert(x >= 0);
        BigInteger t(this->over0 ? this->v + string(x, '0') : '-' + this->v + string(x, '0'));
        return t;
    }
    BigInteger& operator>>(const long long int x) {
        assert(x >= 0);
        if (x >= this->effectiveLength) {
            this->value = "0";
            this->deploy = "00";
            this->effectiveLength = 1;
            this->totalLength = 1;
            return *this;
        }
        else {
            BigInteger t(this->over0 ? string((this->v).begin(), (this->v).end() - x) : '-' + string((this->v).begin(), (this->v).end() - x));
            this->value = t.v;
            this->deploy = t.deploy;
            this->effectiveLength = t.effectiveLength;
            this->totalLength = t.totalLength;
            return *this;
        }
    }
    BigInteger operator>>(const string b) {
        //这个并非是为冗余重载设计，而是用于提供一个不改变原对象（返回新对象）的方法
        long long int x = convertStringToLong(b);
        assert(x >= 0);
        if (x >= this->effectiveLength) {
            return BigInteger(0);
        }
        else {
            BigInteger t(this->over0 ? string((this->v).begin(), (this->v).end() - x) : '-' + string((this->v).begin(), (this->v).end() - x));
            return t;
        }
    }
    //    BigInteger operator<<(const BigInteger& x) {
    //        //自举左移重载，但实际用途不大
    //        assert(x.over0);
    //       较好的实现这个函数并不简单，况且考虑到处理器的性能与思维方式广阔度决定暂时不实现该方法
    //    }
    string getValue() {
        return over0 ? this->v : '-' + this->v;
    }
    BigInteger singleMul(const unsigned int b) {
        string c;
        string b1 = this->value;
        size_t le = b1.length();
        int push = 0;
        int multi;
        for (size_t h = 0; h < le; h++) {
            multi = convertCharToInt(b1[le - h - 1]) * b + push;
            c = convertIntToChar(multi % 10) + c;
            push = multi / 10 % 10;
        }
        c = convertIntToChar(push) + c;
        return BigInteger(c);
    }
    BigInteger mul(const BigInteger& b2) {
        BigInteger c(0);
        BigInteger c2(*this);
        const string b3 = b2.value;
        for (size_t h = 0; h < b2.value.length(); ++h) {
            c = c + (c2 << (convertNumToString(h))).singleMul(convertCharToInt(b3[b2.value.length() - h - 1]));
        }
        if ((this->over0 && b2.over0) || ((!this->over0) && (!b2.over0))) {
            c.over0 = true;
        }
        else {
            c.over0 = false;
        }
        c.deploy = c.getDeploy();
        return c;
    }
    BigInteger mul(const string b2) {
        return this->mul(BigInteger(b2));
    }
    BigInteger mul(const long long int b2) {
        return this->mul(BigInteger(b2));
    }
    BigInteger operator*(const BigInteger& b2) {
        return this->mul(b2);
    }
    BigInteger operator*(const string b2) {
        return this->mul(b2);
    }
    BigInteger operator*(const long long int b2) {
        return this->mul(b2);
    }
    BigInteger& operator*=(const BigInteger& b2) {
        BigInteger x = this->mul(b2);
        this->over0 = x.over0;
        this->value = x.value;
        this->deploy = x.deploy;
        this->totalLength = x.totalLength;
        this->effectiveLength = x.effectiveLength;
        return *this;
    }
    BigInteger& operator*=(const string b2) {
        BigInteger x = this->mul(b2);
        this->over0 = x.over0;
        this->value = x.value;
        this->deploy = x.deploy;
        this->totalLength = x.totalLength;
        this->effectiveLength = x.effectiveLength;
        return *this;
    }
    BigInteger& operator*=(const long long int b2) {
        BigInteger x = this->mul(b2);
        this->over0 = x.over0;
        this->value = x.value;
        this->deploy = x.deploy;
        this->totalLength = x.totalLength;
        this->effectiveLength = x.effectiveLength;
        return *this;
    }
    BigInteger& toAbs() {
        this->over0 = true;
        this->deploy = this->getDeploy();
        return *this;
    }
    BigInteger abs(const BigInteger b) {
        BigInteger x(b.v);
        return x;
    }
    size_t divHelp(BigInteger& b1, BigInteger& b2) {
        size_t get = 1;
        for (size_t k = 2; k <= 9; ++k) {
            if (b1 == (b2 * k)) {
                return k;
            }
            else if (b1 < (b2 * k)) {
                return --k;
            }
            else {

            }
        }
        return 9;
    }
    BigInteger div(BigInteger b2) {
        if (b2.v == "1") {
            return *this;
        }
        BigInteger b = this->abs(b2);
        if (this->abs(*this) < b) {
            return BigInteger(0);
        }
        BigInteger dh = this->abs(*this);
        size_t l1 = this->value.length();
        size_t l2 = b.value.length();
        //1000
        //div 35
        size_t p;
        string get;
        for (size_t j = 0; j < l1;) {
            if (BigInteger(string(dh.v.begin(), dh.v.begin() + 1 + j)) < b) {
                j++;
            }
            else {
                p = j;
                break;
            }
        }
        BigInteger* t = new BigInteger(string(dh.v.begin(), dh.v.begin() + 1 + p));
        size_t newp = p;
        size_t cal;
        while (true) {
            if (*t < b) {
                get = get + '0';
            }
            else {
                cal = divHelp(*t, b);
                get = get + convertIntToChar(cal);
            }
            dh -= ((b * cal) << (l1 - 1 - newp));
            if (get.length() == l1 - p) {
                break;
            }
            newp++;
            *t = ((*t - (b * cal)) << 1) + convertCharToInt((*this)[newp]);
            cal = 0;
        }
        assert(get.length() == l1 - p);//默认查错
        get = same(this->over0, b2.over0) ? get : '-' + get;
        return BigInteger(get);
    }
    BigInteger operator %(BigInteger b2) {
        BigInteger b = this->abs(b2);
        if (this->abs(*this) < b) {
            return BigInteger(0);
        }
        BigInteger dh = this->abs(*this);
        size_t l1 = this->value.length();
        size_t l2 = b.value.length();
        //1000
        //div 35
        size_t p;
        string get;
        for (size_t j = 0; j < l1;) {
            if (BigInteger(string(dh.v.begin(), dh.v.begin() + 1 + j)) < b) {
                j++;
            }
            else {
                p = j;
                break;
            }
        }
        BigInteger* t = new BigInteger(string(dh.v.begin(), dh.v.begin() + 1 + p));
        size_t newp = p;
        size_t cal;
        while (true) {
            if (*t < b) {
                get = get + '0';
            }
            else {
                cal = divHelp(*t, b);
                get = get + convertIntToChar(cal);
            }
            dh -= ((b * cal) << (l1 - newp - 1));
            if (get.length() == l1 - p) {
                break;
            }
            newp++;
            *t = ((*t - (b * cal)) << 1) + convertCharToInt((*this)[newp]);
            cal = 0;
        }
        assert(get.length() == l1 - p);//默认查错
        return (*this).over0 ? dh : -dh;
    }
    BigInteger operator % (const string b2) {
        return *this % BigInteger(b2);
    }
    BigInteger operator % (const long long int b2) {
        return *this % BigInteger(b2);
    }
    BigInteger& operator %=(BigInteger b2) {
        BigInteger t = (*this) % b2;
        this->over0 = t.over0;
        this->value = t.value;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return (*this);
    }
    BigInteger& operator %=(const string b2) {
        BigInteger t = (*this) % b2;
        this->over0 = t.over0;
        this->value = t.value;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return (*this);
    }
    BigInteger& operator %=(const long long int b2) {
        BigInteger t = (*this) % b2;
        this->over0 = t.over0;
        this->value = t.value;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return (*this);
    }
    BigInteger div(const string b2) {
        BigInteger cc(b2);
        return this->div(cc);
    }
    BigInteger div(const long long int b2) {
        BigInteger cc(b2);
        return this->div(cc);
    }
    BigInteger operator / (BigInteger b2) {
        return this->div(b2);
    }
    BigInteger operator / (const string b2) {
        BigInteger b(b2);
        return this->div(b);
    }
    BigInteger operator / (const long long int b2) {
        BigInteger b(b2);
        return this->div(b);
    }
    BigInteger& operator /= (BigInteger b) {
        BigInteger t = this->div(b);
        this->over0 = t.over0;
        this->value = t.value;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }
    BigInteger& operator /= (const string b) {
        BigInteger t = this->div(b);
        this->over0 = t.over0;
        this->value = t.value;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }
    BigInteger& operator /= (const long long int b) {
        BigInteger t = this->div(b);
        this->over0 = t.over0;
        this->value = t.value;
        this->deploy = t.deploy;
        this->totalLength = t.totalLength;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }
    BigInteger sqrt() {
        BigInteger res(1);
        bool t = true;;
        while (t) {
            res =(res + (*this) / res)/ 2;
            t = (res * res <= (*this) && (res + 1) * (res + 1) > (*this)) || (res * res >= (*this) && (res - 1) * (res - 1) < (*this));
            t = !t;
        }
        return (res * res <= (*this) && (res + 1) * (res + 1) > (*this)) ? res : res - 1;
    }

};
string getFibo(long long int p) {
    if (p == 0) {
        return "0";
    }
    else if (p == 1) {
        return "1";
    }
    else {
        BigInteger x(1);
        BigInteger y(0);
        BigInteger z(1);
        for (int h = 2; h <= p; h++) {
            x = y;
            y = z;
            z = x + y;
        }
        return z.value;
    }
}
int main() {
    //        BigInteger x(100);
    //        BigInteger y(200);
    //        BigInteger z(-300);
    //        cout<<(x+y).over0<<" "<<(x+y).v<<" "<<(x+y).getValue()<<endl;
    //        cout<<(x+z).over0<<" "<<(x+z).v<<" "<<(x+z).getValue()<<endl;
    //        cout<<(x-y).over0<<" "<<(x-y).v<<" "<<(x-y).getValue()<<endl;
    //        cout<<(-y).over0<<" "<<(-y).v<<" "<<(-y).getValue()<<endl;
    //        cout<<(x-z).over0<<" "<<(x-z).v<<" "<<(x-z).getValue()<<endl;
    //        cout<<(-x-y).over0<<" "<<(-x-y).v<<" "<<(x-100000).getValue()<<endl;
    //        BigInteger io(string(10,'3'));
    //        BigInteger ip(string(10,'7'));
    //        cout<<(++io).getValue()<<endl;cout<<(io).getValue()<<endl;
    //        cout<<(io++).getValue()<<endl;cout<<(io).getValue()<<endl;
    //        cout<<(--io).getValue()<<endl;cout<<(io).getValue()<<endl;
    //        cout<<(io--).getValue()<<endl;cout<<(io).getValue()<<endl;
    /*    cout << (BigInteger(100) + "-100").getValue() << endl;
            cout<<(BigInteger(123)<<1000000).getDetails();
        vector<int> y(0);
        y = vector<int>(2);*/
        //    BigInteger x("0000");
        //    x.getDetails();
        //    BigInteger p("33333");
        //    p >> 3;
        //    p.getDetails();
        //    p*=-100;
        //    (p.toAbs()).getDetails();
        //    p.getDetails();
        //    p *= -1;
        //    p.abs(p).getDetails();
        //    p.getDetails();
        //    for(int i = 0;i<3000;i++){
        ////        cout<<getFibonasci(i)<<endl;
        //        cout<<getFibo(i)<<endl;
        //    }
        //    for (int i = 0; i < 10; i++) {
        //        cout << getFibo(i) << endl;
        //    }
        //    cout << getFibo(30001);
        //    cout<<BigInteger(100).equals(BigInteger(33)*2);
        //    BigInteger x(100);
        //    x = - ((BigInteger(33)*0)<<1);
        //    x.getDetails();
        //    ((BigInteger(33)*0)<<1).getDetails();
        //    (BigInteger(33)*0).getDetails();
        //    BigInteger x(-1000);
        ////    x = x / 13;
        ////    x.getDetails();
        //    x = BigInteger(-1000);
        //    x %=19;
        //    x.getDetails();
    //    BigInteger y(BigInteger(144567)*"144567");
    BigInteger z(BigInteger(1234567) * 7654321);
    (z.sqrt()).getDetails();
    //    (BigInteger (1259)/2).getDetails();
    return 0;
}
//下一步的改正策略
/*
取消对value值的引用，以便于更精确的对函数进行签名
显现bug排除
创建精简 BigInteger 与 unsigned BigInteger 以便于进一步提升性能，减少内存消耗，
删除 totalLength,对每一个BigInteger 在创建时自动简化，并且在运算时也进行简化，减少内存消耗，提升性能
用减法来实现减法，用于提升性能，取消对deploy的依赖，减小内存消耗，提升性能
当数据小于long long int max时，依赖于原生的计算库，实现有尽可能提升性能，不做无用功。
提高数据精度的表示
*/
