#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include <windows.h>
using namespace std;
void xor_bits(string &input,string &key,int element, int len)
{
    if (element>=len) return;
    input[element]^=key[element];
    element++;
    input[element];
    xor_bits(input,key,element,len);
}
string random_key(int length) 
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charsetSize = sizeof(charset) - 1;
    string randomString;
    randomString.reserve(length);
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < length; ++i) 
    {
        int randomIndex = rand() % charsetSize;
        randomString.push_back(charset[randomIndex]);
    }
    return randomString;
}
void changing_bits(string &input, int start, int last, string &key, int element)
{
    if (start>=last) return;
    if (key[element]%4==0)
    {
        char save_first =   input[start]&0b00001111;
        char save_last  =   input[last]&0b00001111;
        input[start]    &=  0b11110000;
        input[last]     &=  0b11110000;
        input[start]    |=  save_last;
        input[last]     |=  save_first;
        start++;
        last--;
        return changing_bits(input,start,last,key,element+1);
    }
    else if (key[element]%4==3)
    {
        char save_first =   input[start]&0b00000111;
        char save_last  =   input[last]&0b00000111;
        input[start]    &=  0b11111000;
        input[last]     &=  0b11111000;
        input[start]    |=  save_last;
        input[last]     |=  save_first;
        start++;
        last--;
        return changing_bits(input,start,last,key,element+1);
    }
    else if (key[element]%4==2)
    {
        char save_first =   input[start]&0b00000011;
        char save_last  =   input[last]&0b00000011;
        input[start]    &=  0b11111100;
        input[last]     &=  0b11111100;
        input[start]    |=  save_last;
        input[last]     |=  save_first;
        start++;
        last--;
        return changing_bits(input,start,last,key,element+1);
    }
    else if (key[element]%4==1)
    {
        char save_first =   input[start]&0b00000001;
        char save_last  =   input[last]&0b00000001;
        input[start]    &=  0b11111110;
        input[last]     &=  0b11111110;
        input[start]    |=  save_last;
        input[last]     |=  save_first;
        start++;
        last--;
        return changing_bits(input,start,last,key,element+1);
    }
    start++;
    last--;
    return changing_bits(input,start,last,key,element+1);
}
string add_reminder(string &input,int len)
{
    string reminder="";
    for (int i=0;i<len;i++)
    {
        if (input[i]%2==1) reminder+='1'; 
        else reminder+='0'; 
    }
    return reminder;
}
void divided_and_added(string &input, int len)
{
    for (int i=0;i<len;i++)
    {
        input[i]/=2;
        input[i]+=35;
    }
}
string binary_to_decimal(string bin)
{
    uint8_t dec=0;
    int step=0;
    vector <int> arr;
    uint8_t num;
    for (int i=0;i<bin.size();i++)
    {
        if (step==8)
        {
            arr.push_back(dec);
            dec=0;
            step=0;
        }
        if (bin[i]=='0') num=0b00000000;
        else if (bin[i]=='1') num=0b00000001;
        num=num<<(7-step);
        dec ^= num;
        step++;
    }
    arr.push_back(dec);
    string answer="";
    for (int i=0;i<arr.size();i++)
    {
        answer+=to_string(arr[i]);
        answer+='/';
    }
    return answer;
}
string decToBin(int num)
{
    string answer;
    while (num > 0) 
    {
        int remainder = num % 2;
        answer += to_string(remainder);
        num /= 2;
    }
    while(answer.size()!=8) answer+='0';
    reverse(answer.begin(), answer.end());
    return answer;
}
string decimal_to_binary(string input, int element)
{
    vector <string> decimals;
    string number;
    for (element;element<=input.size();element++)
    {
        if (input[element]=='/')
        {
            decimals.push_back(number);
            number="";
            continue;
        }
        number+=input[element];
    }
    string reminder="";
    for (int i=0;i<decimals.size();i++) reminder+=decToBin(stoi(decimals[i]));
    return reminder;
}
void letters_out_of_range(string &input,int len,string reminder)
{
for (int i=0;i<len;i++)
{
    input[i]-=35;
    input[i]*=2;
    if(reminder[i]=='1') input[i]++;
}
}
int main()
{
    int choice;
    cout<<"Encryption - 1   Decryption - 2\nAnswer: ";
    cin>>choice;
    cin.ignore();
    if (choice==1)
    {
    string input,key;
    cout<<"\nWrite a message: ";
    getline(cin,input);
    int len=input.length();
    key=random_key(len);
    changing_bits(input,0,len-1,key,0);
    xor_bits(input,key,0,len);
    string reminder=add_reminder(input,len);
    divided_and_added(input,len);
    reminder=binary_to_decimal(reminder);
    input+=reminder;
    cout<<"Encrypted message: '"<<input<<"'"<<"\nKey: '"<<key<<"'";
    }
    else if (choice==2) 
    {
        string input,key;
        cout<<"\nWrite an encrypted message: ";
        getline(cin,input);
        cout<<"Write a key: ";
        getline(cin,key);
        int only_letters_len=key.size();// changed into key.size()
        int start_of_reminders=only_letters_len;
        string reminder=decimal_to_binary(input,start_of_reminders);
        letters_out_of_range(input,only_letters_len,reminder);
        xor_bits(input,key,0,only_letters_len);
        changing_bits(input,0,only_letters_len-1,key,0);
        cout<<"Decrypted message: ";
        for (int i=0;i<only_letters_len;i++)  cout<<input[i];
    }
    Sleep(10000);
}