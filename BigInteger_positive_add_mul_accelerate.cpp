#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <climits>
//加法和乘法专用，正数专用
using namespace std;
using namespace std::string_literals;
bool same(const bool a, const bool b) {
    if ((a && b)||((!a) && (!b))) {
        return true;
    }else {
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
long long int convertStringToLong(const string& s) {
    long long int x = 0;
    for (char i : s) {
        x *= 10;
        x += convertCharToInt(i);
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
bool convertStringToBool(const string& bo) {
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
size_t nonZero(const string& getStr) {
    for (size_t i = 0; i < getStr.length(); ++i) {
        if (getStr[i] != '0') {
            return i;
        }
    }
    //获取整数字符串中第一个非零字符的位置，用于简化字符串存储
    //比如传入 0000123 那么该函数将会返回4，后续函数可以利用该数字来处理简化
    return getStr.length();
}
//无减法和除法
string simplifyBigIntegerValue(string BigIntegerValue) {
    string v = std::move(BigIntegerValue);
    //用于简化BigInteger.add(BigInteger&,BigInteger&)的输出
    //简化 00001为 1，事实上由于没有减法
    size_t bn0 = nonZero(v);
    if (bn0 == v.length()) {
        return "0";
    }
    return string(v.begin() + bn0, v.end());
}
class BigInteger {
public:
    size_t effectiveLength;
    string value;
    explicit BigInteger(long long int initNum) {
        assert(initNum>=0);
        this->value = convertNumToString(initNum);
        this->effectiveLength = this->value.length();
    }
    explicit BigInteger(const string& initWithString) {
        string initHelper = initWithString;
        if (initHelper[0] == '0') {
            //08990
            size_t nonZeroBit;
            nonZeroBit = nonZero(string(initHelper));
            if (nonZeroBit == initHelper.length()) {
                this->value = "0";
            }
            else {
                this->value = string(initHelper.begin() + nonZeroBit, initHelper.end());
            }
            this->effectiveLength = this->value.length();
            return;
        }else{
            this->value = string(initHelper.begin(), initHelper.end());
            this->effectiveLength = this->value.length();
        }

    }
    BigInteger(const BigInteger& bi) {
        //拷贝构造函数，不应再函数前面添加explicit，即使它只有一个参数
        //似乎可以看作构造函数，即从BigInteger构造BigInteger
        this->value = bi.value;
        this->effectiveLength = bi.effectiveLength;
    }
    ~BigInteger() = default;
    void toString() {
        //用于对齐
        printf("symbol:BigInteger\nvalue: %s \neffectLength: %d\n", this->getValue().c_str(), this->effectiveLength);

    }
    void getDetails() {
        this->toString();
    }
    BigInteger add(const BigInteger& b2) const {
        const BigInteger b1(this->value);
        size_t l1 = b1.effectiveLength;
        size_t l2 = b2.effectiveLength;
        string effects1, effects2;
        effects1 = b1.value;
        effects2 = b2.value;
        if (l1 >= l2) {
            effects2 = string(l1 - l2, '0') + effects2;
        }
        else {
            effects1 = string(l2 - l1, '0') + effects1;
        }
        size_t push = 0;
        size_t temp;
        size_t n1;
        size_t n2;
        string getRes;
        size_t ll = effects1.length();
        for (size_t j = 0; j < ll; j++) {
            n1 = convertCharToInt(effects1[ll - j - 1]);
            n2 = convertCharToInt(effects2[ll - j - 1]);
            temp = n1 + n2 + push;
            push = 0;
            if (temp >= 10) {
                push = 1;
            }
            temp = temp % 10;
            getRes = convertIntToChar(temp) + getRes;
        }
        getRes = push>0?convertIntToChar(push)+getRes:getRes;
        return BigInteger(getRes);
    }
    BigInteger add(const string& b2) const {
        BigInteger b1 = BigInteger(this->value);
        BigInteger bx = BigInteger(b2);
        return b1.add(bx);
    }
    BigInteger add(const long long int b2) const {
        BigInteger b1 = BigInteger(this->value);
        BigInteger bx = BigInteger(b2);
        return b1.add(bx);
    }
    BigInteger operator+() const {
        return *this;
    }
    BigInteger& operator = (const BigInteger& b2) = default;//simplify by clion
    BigInteger operator+(const BigInteger& b2) const {
        return this->add(b2);
    }
    BigInteger operator+(const string& b2) const {
        return this->add(BigInteger(b2));
    }
    BigInteger operator+(const long long int b2) const {
        return this->add(BigInteger(b2));
    }
    //pre
    BigInteger& operator++() {
        (*this) += 1;
        return *this;
    }
    //post
    BigInteger operator++(int dummy) {
        const BigInteger x = BigInteger(*this);
        (*this) += 1;
        return x;
    }
    char operator[](size_t index) const {
        assert(index + 1 <= this->value.length());
        return this->value[index];
    }
    BigInteger& operator+=(const BigInteger& b2) {
        *this = this->add(b2);
        return *this;
    }
    BigInteger& operator+=(const string& b2) {
        *this = this->add(b2);
        return *this;
    }
    BigInteger& operator+=(const long long int b2) {
        *this = this->add(b2);
        return *this;
    }
    int compareTo(const BigInteger& b2) {
        //注意要比较的是有效值长度
        int y = 2;
        if (this->value.length() > b2.value.length()) {
            y = 3;
        }
        else if (this->value.length() < b2.value.length()) {
            y = 1;
        }
        else {
            for (unsigned int c = 0; c < this->value.length(); c++) {
                if (this->value[c] > b2.value[c]) {
                    y = 3;
                    break;
                }
                else if (this->value[c] < b2.value[c]) {
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
        }else if (y == 3) {
            return 1;
        }
        else {
            return -1;
        }
    }
    int compareTo(const string& b2) {
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
    bool equals(const string& b2) {
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
        BigInteger t(this->value + string(x, '0'));
        this->value = t.value;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }
    BigInteger operator<<(const string& y) const {
        //这个并非是为冗余重载设计，而是用于提供一个不改变原对象（返回新对象）的方法
        long long int x = convertStringToLong(y);
        assert(x >= 0);
        BigInteger t(this->value + string(x, '0'));
        return t;
    }
    BigInteger& operator>>(const long long int x) {
        assert(x >= 0);
        if (x >= this->effectiveLength) {
            this->value = "0";
            this->effectiveLength = 1;
            return *this;
        }
        else {
            BigInteger t(string((this->value).begin(), (this->value).end() - x));
            this->value = t.value;
            this->effectiveLength = t.effectiveLength;
            return *this;
        }
    }
    BigInteger operator>>(const string& b) {
        //这个并非是为冗余重载设计，而是用于提供一个不改变原对象（返回新对象）的方法
        size_t x = convertStringToLong(b);
        assert(x >= 0);
        if (x >= this->effectiveLength) {
            return BigInteger(0);
        }
        else {
            BigInteger t(string((this->value).begin(), (this->value).end() - x));
            return t;
        }
    }
    //    BigInteger operator<<(const BigInteger& x) {
    //        //自举左移重载，但实际用途不大
    //        assert(x.over0);
    //       较好的实现这个函数并不简单，况且考虑到处理器的性能与思维方式广阔度决定暂时不实现该方法
    //    }
    string getValue() const {
        return this->value;
    }
    BigInteger singleMul(const unsigned int b) const {
        string c;
        string b1 = this->value;
        size_t le = b1.length();
        size_t push = 0;
        size_t multi;
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
        return c;
    }
    BigInteger mul(const string& b2) {
        return this->mul(BigInteger(b2));
    }
    BigInteger mul(const long long int b2) {
        return this->mul(BigInteger(b2));
    }
    BigInteger operator*(const BigInteger& b2) {
        return this->mul(b2);
    }
    BigInteger operator*(const string& b2) {
        return this->mul(b2);
    }
    BigInteger operator*(const long long int b2) {
        return this->mul(b2);
    }
    BigInteger& operator*=(const BigInteger& b2) {
        BigInteger x = this->mul(b2);
        this->value = x.value;
        this->effectiveLength = x.effectiveLength;
        return *this;
    }
    BigInteger& operator*=(const string& b2) {
        BigInteger x = this->mul(b2);
        this->value = x.value;
        this->effectiveLength = x.effectiveLength;
        return *this;
    }
    BigInteger& operator*=(const long long int b2) {
        BigInteger x = this->mul(b2);
        this->value = x.value;
        this->effectiveLength = x.effectiveLength;
        return *this;
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
string getJc(long int x){
    if(x==0){
        return "1";
    }
    BigInteger res(1);
    for(long int y = 1;y < x;y++){
        res *= y;
    }
    res.getDetails();
    return res.value;
}
int main() {
    cout<<getFibo(30001);
//    getJc(1000);
    return 0;
}
