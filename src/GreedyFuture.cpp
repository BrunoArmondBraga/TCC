#include <iostream>
#include <vector>

using namespace std;

int main(){
    int n, input, h;

    cin >> n;
    std::vector<int> v(n + 1, 0);

    h = 1;

    while(cin >> input){
        int max_left = -1;
        int max_right = -1;

        int before = v[input];

        for(int i = input - 1; i >= 1; i--){
            if(v[i] > before && v[i] > max_left){
                cout << "(" << i << "," << h << ")" << endl;
                max_left = v[i];
                v[i] = h;
            }
        }
        
        cout << "(" << input << "," << h << ")" << endl;

        for(int i = input + 1; i <= n; i++){
            if(v[i] > before && v[i] > max_right){
                cout << "(" << i << "," << h << ")" << endl;
                max_right = v[i];
                v[i] = h;
            }
        }

        v[input] = h;
        h++;
    }
    return 1;
}