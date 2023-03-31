# include <iostream>
# include <cctype>
# include <cstring>
using namespace std;

int main()
{
    char str[] = {"25.7,72.10,26.22"};
    cout<<"The string bytes for str[] is: "<<strlen(str);
    cout << "The string bytes for size of[] is: " << sizeof(str);
    return 0;
}