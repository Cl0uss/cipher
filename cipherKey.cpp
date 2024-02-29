#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <algorithm>
using namespace std;
void xor_bits(string &input,string &key,int element, int len)
{
    if (element>=len) return;
    input[element]^=key[element];
    element++;
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
void shortenKey(string& key, int len, int first, int second)
{
    if (key.size()==len) return;
    if (second>=key.length()) second=0;
    if (first>=key.length()) first=0;
    if (key[first]>=key[second])
    {
        key=key.erase(second,1);   
    }
    else key=key.erase(first,1);
    return shortenKey(key,len,first++,second++);

}
void increaseKey (string& key,int len,int start, int end) //change to another algorithm but for now it's okay
{
    if (key.length()==len) return;
    uint8_t bitwise=key[start]^key[end];
    key+=bitwise;
    start++; 
    end--;
    if (start>=end) return increaseKey(key,len,0,key.length()-1);
    return increaseKey(key,len,start,end);
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
string binary_to_decimal(string bin, int& quantity)
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
        quantity++;
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
int findElement(string key, int& reminderStart)
{
    string numStr="";
    int element=0;
    while (key[element]!='/')
    {
        numStr+=key[element];
        element++;
    }
    int number=stoi(numStr);
    element++;
    reminderStart=element;
    while (number!=0)
    {
        if (key[element]=='/') number --;
        element++;
    }
    return element;
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
    srand(time(0));
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
    key=random_key(16+(rand()%10));
    string saveKey=key;
    if (key.length()>input.length()) shortenKey(key,len,0,1);
    else if (key.length()<input.length()) increaseKey(key,len,0,len-1);
    changing_bits(input,0,len-1,key,0);
    xor_bits(input,key,0,len);
    string reminder=add_reminder(input,len);
    divided_and_added(input,len);
    int quantity=0;
    reminder=binary_to_decimal(reminder,quantity);
    string quantityStr=to_string(quantity)+'/';
    reminder=quantityStr+reminder;
    saveKey=reminder+saveKey;
    cout<<"Encrypted message: '"<<input<<"'"<<"\nKey: '"<<saveKey<<"'";
    }
    else if (choice==2) 
    {
        string input,input_key;
        cout<<"\nWrite an encrypted message: ";
        getline(cin,input);
        cout<<"Write a key: ";
        getline(cin,input_key);
        string key="";
        int reminderStart=0;
        int keyStart=findElement(input_key,reminderStart);
        key=input_key.substr(keyStart,input_key.size());
        if (key.length()>input.length()) shortenKey(key,input.size(),0,1);
        else if (key.length()<input.length()) increaseKey(key,input.size(),0,input.size()-1);
        input=input+input_key.substr(reminderStart,keyStart-1);
        int only_letters_len=key.size();// changed into key.size()
        int start_of_reminders=only_letters_len;
        string reminder=decimal_to_binary(input,start_of_reminders);
        letters_out_of_range(input,only_letters_len,reminder);
        xor_bits(input,key,0,only_letters_len);
        changing_bits(input,0,only_letters_len-1,key,0);
        cout<<"Decrypted message: ";
        for (int i=0;i<only_letters_len;i++)  cout<<input[i];
    }
}