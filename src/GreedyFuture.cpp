#include <iostream>
#include <vector>

using namespace std;

struct Point{
    int x;
    int y;
};

int main(){
    int n, input, h;

    cin >> n;
    std::vector<int> v(n + 1, 0);
    std::vector<Point> points;

    h = 1;

    while(cin >> input){
        int max_left = -1;
        int max_right = -1;

        int before = v[input];

        for(int i = input - 1; i >= 1; i--){
            if(v[i] > before && v[i] > max_left){
                points.push_back({i,h});
                max_left = v[i];
                v[i] = h;
            }
        }
        
        points.push_back({input,h});

        for(int i = input + 1; i <= n; i++){
            if(v[i] > before && v[i] > max_right){
                points.push_back({i,h});
                max_right = v[i];
                v[i] = h;
            }
        }

        v[input] = h;
        h++;
    }

    for(int i = 0; i < points.size(); i++){
        cout << points[i].x << " " << points[i].y <<  endl;
    }
}