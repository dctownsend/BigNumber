//
//  bigNumber.cpp
//  bigNumberDLL
//
//  Created by dakota townsend on 5/30/18.
//  Copyright © 2018 dakota townsend. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class BigNumbers {
private:
    vector<int> numbers; //!< private vector
    
public:
    //! throw exception if the answer is negative
    class NegativeNumberException{};
    
    //! throw an exception if the number size exceeds 10,000 digits
    class NumberOverflowException{};
    
    //! throw an exception when trying to divide by 0
    class DivideByZeroException{};
    
    //! throw an exception if the second modulus argument is 0
    class ModulusByZeroException{};
    
    
    //! default constructor
    BigNumbers(){}
    
    //! overloaded constructor to initialize an object
    BigNumbers(string stringNumber) {
        for (int i = 0; i < stringNumber.length(); i++) {
            int converted = (int)stringNumber.at(i) - '0';
            numbers.push_back(converted);
            
            if (int(numbers.size()) > 10000) {
                throw NumberOverflowException();
            }
        }
    }
    
    //! addition(+) operator overload for addition with big numbers
    BigNumbers operator + (BigNumbers num) {
        BigNumbers temp;
        
        if (num.numbers.size() > numbers.size()) {
            temp.numbers.resize(int(num.numbers.size()));
            
            // if they are the same size or if num.numbers.size() < numbers.size()
        } else {
            temp.numbers.resize(int(numbers.size()));
        }
        
        int i = int(numbers.size()) - 1;
        int i2 = int(num.numbers.size()) - 1;
        int i3 = int(temp.numbers.size()) - 1;
        int addition;
        bool carry = false;
        bool added = false;
        
        // while-loop until each numbers have been added from each vector
        while (!added) {
            if (i < 0) {
                addition = num.numbers[i2--];
            }
            else if (i2 < 0) {
                addition = numbers[i--];
            } else {
                addition = numbers[i--] + num.numbers[i2--];
            }
            
            if (carry) {
                addition++;
                carry = false;
            }
            
            if (addition >= 10) {
                carry = true;
                addition -= 10;
            }
            
            temp.numbers[i3--] = addition;
            
            
            if (i < 0 && i2 < 0) {
                if (carry) {
                    //  if there is a carry put 1 in front of vector
                    vector<int>::iterator it = temp.numbers.begin();
                    temp.numbers.insert(it, 1);
                }
                
                added = true;
            }
            
            if (int(temp.numbers.size()) > 10000) {
                throw NumberOverflowException();
            }
        }
        
        return temp;
    }
    
    //! subtraction(-) operator overload to do subtraction with big nums
    BigNumbers operator - (BigNumbers num) {
        BigNumbers temp;
        
        // if number subtracting is too big, throw a NegativeNumberException
        if (num.numbers.size() >= numbers.size()) {
            bool valid = true;
            
            if (num.numbers.size() > numbers.size()) {
                valid = false;
                
                //num.numbers.size() == numbers.size()
            } else {
                for (int i = 0; i < int(numbers.size()); i++) {
                    if (num.numbers[i] > numbers[i]) {
                        valid = false;
                        break;
                    } else if (num.numbers[i] < numbers[i]) {
                        break;
                    }
                }
            }
            
            if (!valid) {
                throw NegativeNumberException(); //throw exception
            }
        }
        
        // check to make sure answer vector is at least the size of highest num
        temp.numbers.resize(int(numbers.size()));
        
        int i = int(numbers.size()) - 1;
        int i2 = int(num.numbers.size()) - 1;
        int i3 = int(temp.numbers.size()) - 1;
        int subtraction;
        bool carry = false;
        bool subtracted = false;
        
        while (!subtracted) {
            if (carry) {
                if (numbers[i] == 0) {
                    numbers[i] = 9;
                } else {
                    numbers[i] -= 1;
                    carry = false;
                }
            }
            
            if (i2 < 0) {
                subtraction = numbers[i--];
            } else {
                if (numbers[i] < num.numbers[i2]) {
                    numbers[i] += 10;
                    carry = true;
                }
                
                subtraction = numbers[i--] - num.numbers[i2--];
            }
            
            temp.numbers[i3--] = subtraction;
            
            if(i < 0 && i2 < 0) {
                subtracted = true;
            }
        }
        
        int loopCondition = int(temp.numbers.size());
        
        for (int i = 0; i < loopCondition; i++) {
            if (temp.numbers[0] == 0) {
                temp.numbers.erase(temp.numbers.begin());
            } else {
                break;
            }
        }
        
        return temp;
    }
    
    //! multiplication(*) operator overload to perform multiplication with big numbers
    BigNumbers operator * (BigNumbers num) {
        BigNumbers temp("0");
        BigNumbers subtract("1");
        int addLarger = 0;
        
        if (num.numbers.size() > numbers.size()) {
            addLarger = 0;
            
        } else if(num.numbers.size() < numbers.size()) {
            addLarger = 1;
            
        } else {
            for (int i = 0; i < int(numbers.size()); i++) {
                if (num.numbers[i] > numbers[i]) {
                    addLarger = 0;
                    break;
                    
                } else if (num.numbers[i] < numbers[i]) {
                    addLarger = 1;
                    break;
                }
            }
        }
        
        if (addLarger == 0) {
            while (int(numbers.size()) != 0) {
                temp = temp + num;
                *this = *this - subtract;
            }
            
        } else {
            while (int(num.numbers.size()) != 0) {
                temp = temp + *this;
                num = num - subtract;
            }
        }
        return temp;
    }
    
    //! division(/) operator overload to perform division with big numbers
    BigNumbers operator / (BigNumbers num) {
        //cannot divide by 0
        if (int(num.numbers.size()) == 1 && num.numbers[0] == 0) {
            throw new DivideByZeroException();
        }
        
        BigNumbers temp;
        
        //check if num is bigger
        if (num.numbers.size() >= numbers.size()) {
            bool canDivide = true;
            
            if (num.numbers.size() > numbers.size()) {
                canDivide = false;
                
            } else {
                for (int i = 0; i < int(numbers.size()); i++) {
                    if (num.numbers[i] > numbers[i]) {
                        canDivide = false;
                        break;
                    } else if (num.numbers[i] < numbers[i]) {
                        break;
                    }
                }
            }
            
            if (!canDivide) {
                BigNumbers result("0");
                return result;
            }
        }
        
        BigNumbers addOne("1");
        
        while (numbers.size() != 0) {
            try {
                *this = *this - num;
                
            } catch (NegativeNumberException) {
                break;
            }
            temp = temp + addOne;
        }
        return temp;
    }
    
    //! modulus(%) operator overload to perform modulus with big numbers
    BigNumbers operator % (BigNumbers num) {
        // cannot be 0
        if (int(num.numbers.size()) == 1 && num.numbers[0] == 0) {
            throw new ModulusByZeroException();
        }
        
        while(int(numbers.size()) != 0) {
            try {
                *this = *this - num;
                
            } catch (NegativeNumberException) {
                return *this;
            }
        }
        
        BigNumbers temp("0");
        return temp;
    }
    
    //! print method
    void print()
    {
        for (int i = 0; i < int(numbers.size()); i++)
        {
            cout << numbers[i];
        }
        cout << endl;
    }
    
    //! object to string
    void set(string string) {
        numbers.clear();
        
        for (int i = 0; i < string.length(); i++) {
            int result = (int)string.at(i) - '0';
            numbers.push_back(result);
            
            if (int(numbers.size()) > 10000) {
                throw NumberOverflowException();
            }
        }
    }
};
