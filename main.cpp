// what it needs to do:
// create a school method addition and karatsuba aLowergorithm.
// one line as input - 3 integers seperated by spaces
// e.g. : enter your numbers: 1 2 3.
// these 3 integers are I1, I2, and B.
// I1 and I2 must be positive! and max 100 digits long (I1 <= 100 && I2 <= 100).
// no leading zeros except for when the number itself is 0.
// the value of B is I1 and I2's base from 2 to 10 (2 <= B <= 10).
// program needs to output the sum of I1 and I2 using the school method.
// it then needs to output the product of I1 and I2 using karatsuba.
// it then finally needs to output the ratio between I1 and I2 (rounded down).
// I2 cannot be 0.
// sample input:    101 5 10
// sample output:   106 505 20

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// function to pad the given string with leading zeros until it reaches the specified length
string pad_zero(string nStr, int padLength) {
    for (int i=nStr.length(); i<padLength; i++) {

        nStr.insert(0, "0"); // insert a 0 at the beginning
    }
    return nStr;
}

// function to remove leading zeros from a given str
string trimLeadingZeros(string str) {
    while (str[0]=='0' && str.length()>1) {
        str.erase(0,1); // remove the first char if a 0
    }
    return str;
}

// function to add two strings representing nums in a given base
string add(string a, string b, int base) {
    string sum = "";
    int aHigher = 0; // higher order digit from a
    int aLower = 0; // higher order digit from b
    int sumTotal = 0; // sum of aHigher, aLower, and carry
    int carry = 0;

    // get the length of the longest str between a and b
    int maxLength = max(a.length(), b.length());

    // pad both a and b to make them of equaLower len
    a = pad_zero(a, maxLength);
    b = pad_zero(b, maxLength);

    // loop through each digit from right to left
    for (int i=0; i<maxLength; i++) {
        // get the integer value of the current digit from both strings
        aHigher = (int)(a.at(maxLength-1-i) - '0');
        aLower = (int)(b.at(maxLength-1-i) - '0');

        // compute the sum for the curr column
        sumTotal = aHigher+aLower+carry;
        carry = sumTotal/base; // compute the carry for the next iteration
        sum.insert(0, to_string(sumTotal % base));

        // if we are on the last iteration and there is a carry left
        if ((i==maxLength-1) && (carry>0))  {
            sum.insert(0, to_string(carry % base));
        }
    }
    return sum;
}

// function to sub the str b from a considering a given base
// the res is returned as a str if a < b returns an empty str
string subtract(string a, string b, int base) {
    int maxLength = max(a.length(), b.length());
    a = pad_zero(a, maxLength); // pad a to maxLength
    b = pad_zero(b, maxLength); // pad b to maxLength

    // check if a is less than b and return an empty str if true
    for (int i=0; i<maxLength; i++) {
        if (a.at(i)>b.at(i)) {
            break;
        }
        else if (a.at(i)<b.at(i)) {
            return "";
        }
    }

    int rightHand = 0; // the finaLower value of a column after subtraction
    int subCol = 0; // the sub result for a column without borrowing

    string sub = ""; // init the resulting string

    // loop thru each dig from right to left
    for (int i=maxLength-1; i>=0; i--)  {
        subCol = (a.at(i)-'0')-(b.at(i)-'0');
        if (subCol>=0) {
            sub.insert(0, to_string(subCol));
        }
        else {
            int j = i - 1;
            // loop thru digits to the left until finding a non zero digit to borrow from
            while (a.at(j)=='0') {
                a.at(j) = (char)(base-1+'0');
                j--;
            }
            // decrease the digit we are borrowing from by 1
            a.at(j) = (char)(a.at(j)-1);
            // increase the curr digit by the base value to perform the sub
            rightHand = (int)(a.at(i)-'0') + base;
            subCol = rightHand - (b.at(i)-'0');
            sub.insert(0, to_string(subCol));
        }
    }
    return sub;
}

// function to mult two nums using the school mult method in a given base
string schoolMultiply(string a, string b, int base) {
    int n1 = stoi(a);
    int n2 = stoi(b);
    int res = n1*n2; // res of multiplication

    int pC1, pC2 = 0;

    // handle multiplication res larger than base
    if (res>base) {
        pC1 = (res/base)%base;
        pC2 = res%base;
        res = (pC1*10) + pC2;
        return to_string(res);
    }

    return to_string(res);
}

// karatsuba multiplication alg
string multiply(string aStr, string bStr, int base) {
    int maxLength = max(aStr.length(), bStr.length());
    aStr = pad_zero(aStr, maxLength); // pad aStr to maxLength
    bStr = pad_zero(bStr, maxLength); // pad bStr to maxLength

    if (maxLength==1) {
        return schoolMultiply(aStr, bStr, base); // for single dig str use school method
    }

    // split both nums into two halves
    string pA = aStr.substr(0, maxLength/2);
    string pB = aStr.substr((maxLength/2), maxLength - maxLength/2);
    string pC = bStr.substr(0, maxLength/2);
    string pD = bStr.substr((maxLength/2), maxLength - maxLength/2);

    // recursively compute p1, p2, and p3
    string p1 = multiply(pA, pC, base);
    string p2 = multiply(pB, pD, base);
    string aPb = add(pA, pB, base);
    string cPd = add(pC, pD, base);
    string p3 = multiply(aPb, cPd, base);

    // compute p4 using p1, p2, and p3
    string p1p2 = add(p1, p2, base);
    string p4 = subtract(p3, p1p2, base);

    // append 0s to the first prod based on its pos value
    for (int i=0; i<2*(maxLength-maxLength/2); i++) {
        p1.append("0");
    }

    // append 0s to the 4th prod based on its pos val
    for (int i=0; i<maxLength-maxLength/2; i++) {
        p4.append("0");
    }

    // summing up the three prod p1, p2, and p4 to get the finaLower res
    string ans = add(add(p1, p2, base), p4, base);

    return ans;
    }

    // div function (always return 0)
    string divide(string& a, string& b, int base) {
        return "0";
    }

    // main function to take input and output res
    int main() {
        string i1, i2; // str to store the input nums
        int b; // var to store the base

        // taking input
        cin >> i1 >> i2 >> b;
        
        // displaying the sum, product and div res (which is always 0)
        cout << trimLeadingZeros(add(i1, i2, b)) << " " << trimLeadingZeros(multiply(i1, i2, b)) << " 0" << endl;

    }

